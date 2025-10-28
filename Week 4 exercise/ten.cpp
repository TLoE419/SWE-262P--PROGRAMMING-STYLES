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
#include <any>

using namespace std;

// Style #10: The One (Monadic Style)
// The One class wraps a value and provides a bind method
// bind() takes a function, applies it to the wrapped value, and returns this for chaining

class TFTheOne
{
private:
    any _value;

public:
    TFTheOne(any v) : _value(v) {}

    template<typename Func>
    TFTheOne& bind(Func func) 
    {
        _value = func(_value);
        return *this;
    }

    void printme() {
        // Assuming the final value is a string
        cout << any_cast<string>(_value);
    }
};

// The functions - each takes the value and returns transformed value

string read_file(any path_any)
{
    string path_to_file = any_cast<string>(path_any);
    ifstream f(path_to_file);
    string data((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();
    return data;
}

string filter_chars(any str_data_any)
{
    string str_data = any_cast<string>(str_data_any);
    regex pattern("[\\W_]+");
    return regex_replace(str_data, pattern, " ");
}

string normalize(any str_data_any)
{
    string str_data = any_cast<string>(str_data_any);
    transform(str_data.begin(), str_data.end(), str_data.begin(),
              [](unsigned char c) { return tolower(c); });
    return str_data;
}

vector<string> scan(any str_data_any)
{
    string str_data = any_cast<string>(str_data_any);
    vector<string> words;
    stringstream ss(str_data);
    string word;
    while (ss >> word)
    {
        words.push_back(word);
    }
    return words;
}

vector<string> remove_stop_words(any word_list_any)
{
    vector<string> word_list = any_cast<vector<string>>(word_list_any);

    ifstream f("../stop_words.txt");
    string stop_words_content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();

    set<string> stop_words;
    stringstream ss(stop_words_content);
    string word;
    while (getline(ss, word, ','))
    {
        stop_words.insert(word);
    }

    // Add single-letter words
    for (char c = 'a'; c <= 'z'; c++)
    {
        stop_words.insert(string(1, c));
    }

    vector<string> filtered_words;
    for (const auto& w : word_list) {
        if (stop_words.find(w) == stop_words.end() && !w.empty())
        {
            filtered_words.push_back(w);
        }
    }

    return filtered_words;
}

unordered_map<string, int> frequencies(any word_list_any)
{
    vector<string> word_list = any_cast<vector<string>>(word_list_any);
    unordered_map<string, int> word_freqs;
    for (const auto& w : word_list)
    {
        word_freqs[w]++;
    }
    return word_freqs;
}

vector<pair<string, int>> sort_freqs(any word_freq_any)
{
    unordered_map<string, int> word_freq = any_cast<unordered_map<string, int>>(word_freq_any);
    vector<pair<string, int>> sorted_freqs(word_freq.begin(), word_freq.end());
    sort(sorted_freqs.begin(), sorted_freqs.end(),
         [](const pair<string, int>& a, const pair<string, int>& b)
         {
             return a.second > b.second;
         });
    return sorted_freqs;
}

string top25_freqs(any word_freqs_any)
{
    vector<pair<string, int>> word_freqs = any_cast<vector<pair<string, int>>>(word_freqs_any);
    string top25 = "";
    int limit = min(25, (int)word_freqs.size());
    for (int i = 0; i < limit; i++)
    {
        top25 += word_freqs[i].first + " - " + to_string(word_freqs[i].second) + "\n";
    }
    return top25;
}

// The main function
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    TFTheOne(string(argv[1]))
        .bind(read_file)
        .bind(filter_chars)
        .bind(normalize)
        .bind(scan)
        .bind(remove_stop_words)
        .bind(frequencies)
        .bind(sort_freqs)
        .bind(top25_freqs)
        .printme();

    return 0;
}
