#include <algorithm>
#include <cctype>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace std;

// Message structure for actor communication
struct Message 
{
    string type;
    vector<string> args;
};

// Base Actor class with message queue and independent thread
class ActiveWFObject 
{
    protected:
        thread actorThread;
        string name;
        queue<Message> messageQueue;
        bool stopMe;
        condition_variable cv;
        mutex queueMutex;

        virtual void dispatch(const Message& msg) = 0;

        void run() 
        {
            while (!stopMe) 
            {
                unique_lock<mutex> lock(queueMutex);
                cv.wait(lock, [this] { return !messageQueue.empty() || stopMe; });

                if (stopMe && messageQueue.empty()) 
                    break;

                if (!messageQueue.empty()) 
                {
                    Message msg = messageQueue.front();
                    messageQueue.pop();
                    lock.unlock();

                    dispatch(msg);

                    if (msg.type == "die")
                        stopMe = true;
                }
            }
        }

    public:
        ActiveWFObject(const string& actorName) : stopMe(false), name(actorName) 
        {
            actorThread = thread(&ActiveWFObject::run, this);
        }

        virtual ~ActiveWFObject() 
        {
            if (actorThread.joinable())
                actorThread.join();
        }

        void send(const Message& msg)
        {
            lock_guard<mutex> lock(queueMutex);
            messageQueue.push(msg);
            cv.notify_one(); 
        }
};

// Send messages between actors
void sendMessage(ActiveWFObject* receiver, const Message& msg) 
{
    receiver->send(msg);
}

// Forward declarations
class StopWordManager;
class WordFrequencyManager;
class DataStorageManager;
class WordFrequencyController;

// Models the word frequency data
class WordFrequencyManager : public ActiveWFObject 
{
    private:
        unordered_map<string, int> wordFreqs;
        ActiveWFObject* controller;

        void dispatch(const Message& msg) override 
        {
            if (msg.type == "word") 
            {
                incrementCount(msg.args[0]);
            } 
            else if (msg.type == "top25") 
            {
                top25(msg.args[0]);
            }
        }

        void incrementCount(const string& word) 
        {
            wordFreqs[word]++;
        }

        void top25(const string& recipientId) 
        {
            // Sort by frequency
            vector<pair<string, int>> freqsSorted(wordFreqs.begin(), wordFreqs.end());
            sort(freqsSorted.begin(), freqsSorted.end(),
                [](const pair<string, int>& a, const pair<string, int>& b) {
                    return a.second > b.second;
                });

            for (size_t i = 0; i < min(size_t(25), freqsSorted.size()); i++) {
                cout << freqsSorted[i].first << " - " << freqsSorted[i].second << endl;
            }

            // Send completion message back to controller
            if (controller != nullptr) 
            {
                Message response;
                response.type = "top25_result";
                controller->send(response);
            }
        }

    public:
        WordFrequencyManager() : ActiveWFObject("WordFrequencyManager"), controller(nullptr) {}

        void setController(ActiveWFObject* ctrl) 
        {
            controller = ctrl;
        }
};

// Models the stop word filter
class StopWordManager : public ActiveWFObject 
{
    private:
        vector<string> stopWords;
        WordFrequencyManager* wordFreqsManager;

        void dispatch(const Message& msg) override 
        {
            if (msg.type == "init") 
            {
                init(msg.args[0]);
            } else if (msg.type == "filter") 
            {
                filter(msg.args[0]);
            } else 
            {
                // Forward message to word frequency manager
                sendMessage(wordFreqsManager, msg);
            }
        }

        void init(const string& wfmId) 
        {
            // Load stop words
            ifstream f("../stop_words.txt");
            string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
            f.close();

            stringstream ss(content);
            string word;
            while (getline(ss, word, ',')) 
            {
                stopWords.push_back(word);
            }

            // Add single letters
            for (char c = 'a'; c <= 'z'; c++) 
            {
                stopWords.push_back(string(1, c));
            }
        }

        void filter(const string& word) 
        {
            if (find(stopWords.begin(), stopWords.end(), word) == stopWords.end()) 
            {
                Message msg;
                msg.type = "word";
                msg.args.push_back(word); ///////
                sendMessage(wordFreqsManager, msg);
            }
        }

    public:
    StopWordManager(WordFrequencyManager* wfm)
    : ActiveWFObject("StopWordManager"), wordFreqsManager(wfm) {}
};

// Models the contents of the file
class DataStorageManager : public ActiveWFObject {
    private:
        string data;
        StopWordManager* stopWordManager;

        void dispatch(const Message& msg) override 
        {
            if (msg.type == "init") 
            {
                init(msg.args[0]);
            } 
            else if (msg.type == "send_word_freqs") 
            {
                processWords(msg.args[0]);
            } 
            else 
            {
                // Forward to stop word manager
                sendMessage(stopWordManager, msg);
            }
        }

        void init(const string& pathToFile) 
        {
            // Load file
            ifstream f(pathToFile);
            data = string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
            f.close();

            // Clean data: replace non-alphanumeric with space and lowercase
            regex pattern("[\\W_]+");
            data = regex_replace(data, pattern, " ");
            transform(data.begin(), data.end(), data.begin(),
                    [](unsigned char c) { return tolower(c); });
        }

        void processWords(const string& recipientId) 
        {
            // Split into words and send each one to stop word manager
            stringstream ss(data);
            string word;
            while (ss >> word) 
            {
                Message msg;
                msg.type = "filter";
                msg.args.push_back(word);
                sendMessage(stopWordManager, msg);
            }

            // After all words, request top25
            Message msg;
            msg.type = "top25";
            msg.args.push_back(recipientId);
            sendMessage(stopWordManager, msg);
        }

    public:
        DataStorageManager(StopWordManager* swm)
            : ActiveWFObject("DataStorageManager"), stopWordManager(swm) {}
};

// Controller that orchestrates the process
class WordFrequencyController : public ActiveWFObject 
{
    private:
        DataStorageManager* storageManager;
        mutex& mainMutex;
        condition_variable& mainCv;
        bool& done;

        void dispatch(const Message& msg) override 
        {
            if (msg.type == "run") 
            {
                run();
            } 
            else if (msg.type == "top25_result") 
            {
                display(msg);
            }
        }

        void run() 
        {
            Message msg;
            msg.type = "send_word_freqs";
            msg.args.push_back("controller");
            sendMessage(storageManager, msg);
        }

        void display(const Message& msg) 
        {
            // Send die message to storage manager
            Message dieMsg;
            dieMsg.type = "die";
            sendMessage(storageManager, dieMsg);

            stopMe = true;

            // Signal main thread
            {
                lock_guard<mutex> lock(mainMutex);
                done = true;
            }
            mainCv.notify_one();
        }

    public:
        WordFrequencyController(DataStorageManager* sm, mutex& mtx,
                            condition_variable& cv, bool& doneFlag)
            : ActiveWFObject("WordFrequencyController"),
            storageManager(sm), mainMutex(mtx), mainCv(cv), done(doneFlag) {}
};

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    // Synchronization for main thread
    mutex mainMutex;
    condition_variable mainCv;
    bool done = false;

    // Create actors (similar to Python example)
    WordFrequencyManager wordFreqManager;

    StopWordManager stopWordManager(&wordFreqManager);
    Message initMsg;
    initMsg.type = "init";
    initMsg.args.push_back("word_freq_manager");
    sendMessage(&stopWordManager, initMsg);

    DataStorageManager storageManager(&stopWordManager);
    Message storageInitMsg;
    storageInitMsg.type = "init";
    storageInitMsg.args.push_back(argv[1]);
    sendMessage(&storageManager, storageInitMsg);

    WordFrequencyController controller(&storageManager, mainMutex, mainCv, done);

    // Set controller reference so WordFrequencyManager can send completion message
    wordFreqManager.setController(&controller);

    Message runMsg;
    runMsg.type = "run";
    sendMessage(&controller, runMsg);

    // Wait for completion
    unique_lock<mutex> lock(mainMutex);
    mainCv.wait(lock, [&done] { return done; });

    // Give actors time to process die message
    this_thread::sleep_for(chrono::milliseconds(100));

    return 0;
}
