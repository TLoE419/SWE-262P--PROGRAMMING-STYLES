#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> partition(const string& data_str, int nlines) 
{
    vector<string> result;
    vector<string> lines;

    // Split into lines
    stringstream ss(data_str);
    string line;
    while (getline(ss, line)) 
    {
        lines.push_back(line);
    }

    // Partition into chunks
    for (size_t i = 0; i < lines.size(); i += nlines) 
    {
        string chunk;
        for (size_t j = i; j < min(i + nlines, lines.size()); j++) 
        {
            chunk += lines[j];
            if (j < min(i + nlines, lines.size()) - 1) 
                chunk += "\n";
        }
        result.push_back(chunk);
    }

    return result;
}

vector<pair<string, int>> split_words(const string& data_str) 
{
    // Helper: scan and extract words
    auto scan = [](const string& str_data) -> vector<string> 
    {
        regex pattern("[\\W_]+");
        string clean = regex_replace(str_data, pattern, " ");
        transform(clean.begin(), clean.end(), clean.begin(),
                 [](unsigned char c) { return tolower(c); });

        vector<string> words;
        stringstream ss(clean);
        string word;
        while (ss >> word) 
        {
            words.push_back(word);
        }
        return words;
    };

    // Helper: remove stop words
    auto remove_stop_words = [](const vector<string>& word_list) -> vector<string> 
    {
        // Load stop words
        ifstream f("../stop_words.txt");
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        f.close();

        vector<string> stop_words;
        stringstream ss(content);
        string word;
        while (getline(ss, word, ',')) 
        {
            stop_words.push_back(word);
        }

        // Add single letters
        for (char c = 'a'; c <= 'z'; c++) 
        {
            stop_words.push_back(string(1, c));
        }

        // Filter out stop words
        vector<string> result;
        for (const auto& w : word_list) 
        {
            if (find(stop_words.begin(), stop_words.end(), w) == stop_words.end())
                result.push_back(w);
        }
        return result;
    };

    // The actual work of the mapper
    vector<pair<string, int>> result;
    vector<string> words = remove_stop_words(scan(data_str));
    for (const auto& w : words) 
    {
        result.push_back(make_pair(w, 1));
    }
    return result;
}

map<string, vector<pair<string, int>>> regroup(const vector<vector<pair<string, int>>>& pairs_list) 
{
    map<string, vector<pair<string, int>>> mapping;

    for (const auto& pairs : pairs_list) 
    {
        for (const auto& p : pairs) 
        {
            mapping[p.first].push_back(p);
        }
    }

    return mapping;
}

pair<string, int> count_words(const pair<string, vector<pair<string, int>>>& mapping) 
{
    int sum = 0;
    for (const auto& p : mapping.second) {
        sum += p.second;
    }
    return make_pair(mapping.first, sum);
}

string read_file(const string& path_to_file) 
{
    ifstream f(path_to_file);
    string data((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
    f.close();
    return data;
}

vector<pair<string, int>> sort_word_freq(const vector<pair<string, int>>& word_freq) 
{
    vector<pair<string, int>> sorted_freq = word_freq;
    sort(sorted_freq.begin(), sorted_freq.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) 
         {
             return a.second > b.second;
         });
    return sorted_freq;
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    // Read file and partition
    string data = read_file(argv[1]);
    vector<string> partitions = partition(data, 200);

    // Split_words on each partition
    vector<vector<pair<string, int>>> splits;
    for (const auto& part : partitions) 
    {
        splits.push_back(split_words(part));
    }

    // Regroup by word
    map<string, vector<pair<string, int>>> splits_per_word = regroup(splits);

    // Count words
    vector<pair<string, int>> word_freqs;
    for (const auto& item : splits_per_word) 
    {
        word_freqs.push_back(count_words(item));
    }

    // Sort
    vector<pair<string, int>> sorted_freqs = sort_word_freq(word_freqs);

    // Print top 25
    for (size_t i = 0; i < min(size_t(25), sorted_freqs.size()); i++) 
    {
        cout << sorted_freqs[i].first << " - " << sorted_freqs[i].second << endl;
    }

    return 0;
}
