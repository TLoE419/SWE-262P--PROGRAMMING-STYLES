#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <functional>

using namespace std;

// Style #15: Hollywood (Event-Driven/Callback)
// "Don't call us, we'll call you"
// Framework registers handlers and calls them at appropriate times

const int RECURSION_LIMIT = 5000;

//
// The "I'll call you back" Word Frequency Framework
//
class WordFrequencyFramework {
private:
    vector<function<void(string)>> _load_event_handlers;
    vector<function<void()>> _dowork_event_handlers;
    vector<function<void()>> _end_event_handlers;

public:
    void register_for_load_event(function<void(string)> handler) {
        _load_event_handlers.push_back(handler);
    }

    void register_for_dowork_event(function<void()> handler) {
        _dowork_event_handlers.push_back(handler);
    }

    void register_for_end_event(function<void()> handler) {
        _end_event_handlers.push_back(handler);
    }

    void run(const string& path_to_file) {
        for (auto& h : _load_event_handlers) {
            h(path_to_file);
        }
        for (auto& h : _dowork_event_handlers) {
            h();
        }
        for (auto& h : _end_event_handlers) {
            h();
        }
    }
};

//
// The entities of the application
//

// Forward declaration
class StopWordFilter;

class DataStorage {
    /** Models the contents of the file **/
private:
    string _data;
    vector<string> _words;
    StopWordFilter* _stop_word_filter;
    vector<function<void(vector<string>&)>> _word_event_handlers;

    void __load(const string& path_to_file) {
        ifstream text_file(path_to_file);
        string book_content((istreambuf_iterator<char>(text_file)), istreambuf_iterator<char>());
        text_file.close();

        // Clean the text: replace newlines with spaces, remove contractions, keep only letters, convert to lowercase
        string turn_newline_into_whitespace = regex_replace(book_content, regex("[\\r\\n]+"), " ");
        string remove_apostrophe = regex_replace(turn_newline_into_whitespace, regex("'s|n't|'re|'ve|'ll|'d|'m\\b"), "");
        string only_letter = regex_replace(remove_apostrophe, regex("[^A-Za-z ]"), " ");
        string only_lower_case = only_letter;
        transform(only_letter.begin(), only_letter.end(), only_lower_case.begin(),
                  [](unsigned char c) { return tolower(c); });

        // Extract words from the cleaned text and store in vector
        stringstream ss_letters(only_lower_case);
        string word;
        while (ss_letters >> word) {
            _words.push_back(word);
        }
    }

    void __produce_words() {
        /** Iterates through the list words in storage calling back handlers for words **/
        for (auto& h : _word_event_handlers) {
            h(_words);
        }
    }

public:
    DataStorage(WordFrequencyFramework& wfapp, StopWordFilter* stop_word_filter)
        : _stop_word_filter(stop_word_filter) {
        wfapp.register_for_load_event([this](string path) { this->__load(path); });
        wfapp.register_for_dowork_event([this]() { this->__produce_words(); });
    }

    void register_for_word_event(function<void(vector<string>&)> handler) {
        _word_event_handlers.push_back(handler);
    }

    bool is_stop_word(const string& word);
};

class StopWordFilter {
    /** Models the stop word filter **/
private:
    set<string> _stop_words;

    void __load(const string& ignore) {
        ifstream stop_words_list("../stop_words.txt");
        string stop_words_content((istreambuf_iterator<char>(stop_words_list)), istreambuf_iterator<char>());
        stop_words_list.close();

        stringstream ss_stopwords(stop_words_content);
        string word;
        while (getline(ss_stopwords, word, ',')) {
            _stop_words.insert(word);
        }
    }

public:
    StopWordFilter(WordFrequencyFramework& wfapp) {
        wfapp.register_for_load_event([this](string path) { this->__load(path); });
    }

    bool is_stop_word(const string& word) {
        return _stop_words.find(word) != _stop_words.end();
    }
};

// Now we can implement DataStorage methods that depend on StopWordFilter
bool DataStorage::is_stop_word(const string& word) {
    return _stop_word_filter->is_stop_word(word);
}

class WordFrequencyCounter {
    /** Keeps the word frequency data **/
private:
    unordered_map<string, int> _freq;
    StopWordFilter* _stop_word_filter;

    // Recursive count function - preserving original logic from eight.cpp
    void count(vector<string>& words, int i, int end, set<string>& stopwords) {
        if(i >= words.size())
            return;
        else {
            if(stopwords.find(words[i]) == stopwords.end()) {
                _freq[words[i]]++;
            }
        }
        if(++i >= end)
            return;
        count(words, i, end, stopwords);
    }

    void __count_words(vector<string>& words) {
        // Get stopwords as set for recursive function
        set<string> stopwords;
        ifstream stop_words_list("../stop_words.txt");
        string stop_words_content((istreambuf_iterator<char>(stop_words_list)), istreambuf_iterator<char>());
        stop_words_list.close();

        stringstream ss_stopwords(stop_words_content);
        string word;
        while (getline(ss_stopwords, word, ',')) {
            stopwords.insert(word);
        }

        // Count the frequency of each word using recursive function with chunking
        for (size_t i = 0; i < words.size(); i += RECURSION_LIMIT) {
            count(words, i, i + RECURSION_LIMIT, stopwords);
        }
    }

    void __print_freqs() {
        // Output the 25 most frequent words (or fewer if less than 25), showing their counts
        int N = (_freq.size() > 25 ? 25 : _freq.size());
        for (size_t i = 0; i < N; i++) {
            int max = 0;
            for (const auto &kv : _freq) {
                if (kv.second >= max) {
                    max = kv.second;
                }
            }
            for (const auto &kv : _freq) {
                if (kv.second == max) {
                    cout << kv.first << "  -  " << kv.second << endl;
                    _freq.erase(kv.first);
                    break;
                }
            }
        }
    }

public:
    WordFrequencyCounter(WordFrequencyFramework& wfapp, DataStorage& data_storage, StopWordFilter* stop_word_filter)
        : _stop_word_filter(stop_word_filter) {
        data_storage.register_for_word_event([this](vector<string>& words) { this->__count_words(words); });
        wfapp.register_for_end_event([this]() { this->__print_freqs(); });
    }
};

//
// The main function
//
int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    WordFrequencyFramework wfapp;
    StopWordFilter stop_word_filter(wfapp);
    DataStorage data_storage(wfapp, &stop_word_filter);
    WordFrequencyCounter word_freq_counter(wfapp, data_storage, &stop_word_filter);
    wfapp.run(argv[1]);

    return 0;
}
