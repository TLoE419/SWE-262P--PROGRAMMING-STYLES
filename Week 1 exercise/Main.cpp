#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

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
    vector<string> stopwords;
    int N = 0;

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
        stopwords.push_back(word);
    }
    stop_words_list.close();

    /* Build a regex pattern to match the stop words */
    string pattern = "\\b(";
    for (size_t i = 0; i < stopwords.size(); i++) 
    {
        pattern += stopwords[i];
        if (i != stopwords.size() - 1)
        pattern += "|";
    }
    pattern += ")\\b";

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
    string remove_stopwords = regex_replace(only_lower_case, regex(pattern), "");

    /* Count the frequency of each word in the cleaned text */
    stringstream ss_letters(remove_stopwords);
    while (ss_letters >> word)
    {
        freq[word]++;
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
                if (i == N - 1) {
                    cout << kv.first << "  -  " << kv.second;
                } else {
                    cout << kv.first << "  -  " << kv.second << endl;
                }
                freq.erase(kv.first);
                break;
            }
        }
    }

    return 0;
}