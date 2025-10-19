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

using namespace std;

void count(vector<string>& words, int i, int end, set<string>& stopwords,  unordered_map<string, int>& freq)
{
    if(i >= words.size())
        return;
    else
    {
        if(stopwords.find(words[i]) == stopwords.end())
        {
            freq[words[i]]++;
        }
    }
    if(++i >= end)
        return;
    count(words, i, end, stopwords, freq);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    /* Declaration */
    ifstream stop_words_list("../stop_words.txt");
    ifstream text_file(argv[1]);
    unordered_map<string, int> freq;
    string word;
    set<string> stopwords;
    vector<string> words;  // Store all words from the text
    int N = 0;
    const int RECURSION_LIMIT = 5000;

    /* Change Stop Words to Regular Expression*/
    string stop_words_content
    (
        (istreambuf_iterator<char>(stop_words_list)),
        istreambuf_iterator<char>()
    );
    stop_words_list.close();

    /* Read stop words, store to vector, and build a regex pattern to match them */
    stringstream ss_stopwords(stop_words_content);
    while (getline(ss_stopwords, word, ',')) 
    {
        stopwords.insert(word);
    }
    stop_words_list.close();


    /* Load the entire book file into the book_content string */
    string book_content
    (
        (istreambuf_iterator<char>(text_file)),
        istreambuf_iterator<char>()
    );
    text_file.close();

    /* Clean the text: replace newlines with spaces, remove contractions, keep
    * only letters, convert to lowercase, and remove stop words */
    string turn_newline_into_whitespace = regex_replace(book_content, regex("[\\r\\n]+"), " ");
    string remove_apostrophe = regex_replace(turn_newline_into_whitespace, regex("'s|n't|'re|'ve|'ll|'d|'m\\b"), "");
    string only_letter = regex_replace(remove_apostrophe, regex("[^A-Za-z ]"), " ");
    string only_lower_case = only_letter;
    transform(only_letter.begin(), only_letter.end(), only_lower_case.begin(),[](unsigned char c) { return tolower(c); });


    /* Extract words from the cleaned text and store in vector */
    stringstream ss_letters(only_lower_case);
    while (ss_letters >> word)
    {
        words.push_back(word);
    }

    /* Count the frequency of each word using for loop */
    for (size_t i = 0; i < words.size(); i+= RECURSION_LIMIT)
    {
        count(words, i, i + RECURSION_LIMIT, stopwords, freq);
    }

    /* Output the 25 most frequent words (or fewer if less than 25), showing their
    * counts and removing them from the frequency list */
    N = (freq.size() > 25 ? 25 : freq.size());
    for (size_t i = 0; i < N; i++) 
    {
        int max = 0;
        for (const auto &kv : freq) 
        {
            if (kv.second >= max) 
            {
                max = kv.second;
            }
        }
        for (const auto &kv : freq)
        {
            if (kv.second == max) 
            {
                cout << kv.first << "  -  " << kv.second << endl;
                freq.erase(kv.first);
                break;
            }
        }
    }

    return 0;
}