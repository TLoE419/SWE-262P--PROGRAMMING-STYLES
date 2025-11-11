#include <algorithm>
#include <cctype>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

using namespace std;

// Thread-safe queue wrapper
template<typename T>
class ThreadSafeQueue 
{
    private:
        queue<T> data;
        mutex mtx;
        condition_variable cv;
        bool finished;

    public:
        ThreadSafeQueue() : finished(false) {}

        void put(const T& item) 
        {
            lock_guard<mutex> lock(mtx);
            data.push(item);
            cv.notify_one();
        }

        bool get(T& item, int timeoutMs = 1000) 
        {
            unique_lock<mutex> lock(mtx);
            if (cv.wait_for(lock, chrono::milliseconds(timeoutMs),
                        [this] { return !data.empty() || finished; })) 
            {
                if (!data.empty()) 
                {
                    item = data.front();
                    data.pop();
                    return true;
                }
            }
            return false;
        }

        bool empty() 
        {
            lock_guard<mutex> lock(mtx);
            return data.empty();
        }

        void setFinished() 
        {
            lock_guard<mutex> lock(mtx);
            finished = true;
            cv.notify_all();
        }
};

// Two data spaces
ThreadSafeQueue<string> word_space;
ThreadSafeQueue<unordered_map<string, int>> freq_space;

// Global stopwords set
set<string> stopwords;

// Worker function that consumes words
void process_words() 
{
    unordered_map<string, int> word_freqs;

    while (true) 
    {
        string word;
        if (!word_space.get(word)) 
        {
            break; 
        }

        if (stopwords.find(word) == stopwords.end()) 
        {
            word_freqs[word]++;
        }
    }

    freq_space.put(word_freqs);
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    // Load stop words
    ifstream stop_words_file("../stop_words.txt");
    string stop_words_content
    (
        (istreambuf_iterator<char>(stop_words_file)),
        istreambuf_iterator<char>()
    );
    stop_words_file.close();

    stringstream ss_stopwords(stop_words_content);
    string word;
    while (getline(ss_stopwords, word, ',')) 
    {
        stopwords.insert(word);
    }

    // Add single letters to stopwords
    for (char c = 'a'; c <= 'z'; c++) 
    {
        stopwords.insert(string(1, c));
    }

    // Load text file and populate word space
    ifstream text_file(argv[1]);
    string text_content
    (
        (istreambuf_iterator<char>(text_file)),
        istreambuf_iterator<char>()
    );
    text_file.close();

    // Convert to lowercase
    transform
    (
        text_content.begin(), text_content.end(), text_content.begin(),
        [](unsigned char c) { return tolower(c); }
    );

    // Extract words (2+ letters) and put them in word space
    regex word_pattern("[a-z]{2,}");
    auto words_begin = sregex_iterator(text_content.begin(), text_content.end(), word_pattern);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) 
    {
        word_space.put(i->str());
    }

    // Create workers and launch them
    const int NUM_WORKERS = 5;
    vector<thread> workers;
    for (int i = 0; i < NUM_WORKERS; i++) 
    {
        workers.push_back(thread(process_words));
    }

    // Wait for workers to finish
    for (auto& t : workers) 
    {
        t.join();
    }

    // Merge the partial frequency results
    unordered_map<string, int> word_freqs;
    while (!freq_space.empty()) 
    {
        unordered_map<string, int> freqs;
        if (freq_space.get(freqs, 100)) 
        {
            for (const auto& kv : freqs) 
            {
                word_freqs[kv.first] += kv.second;
            }
        }
    }

    // Sort and print top 25
    vector<pair<string, int>> sorted_freqs(word_freqs.begin(), word_freqs.end());
    sort(sorted_freqs.begin(), sorted_freqs.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) 
         {
             return a.second > b.second;
         });

    int count = 0;
    for (const auto& kv : sorted_freqs) 
    {
        if (count >= 25) break;
        cout << kv.first << " - " << kv.second << endl;
        count++;
    }

    return 0;
}
