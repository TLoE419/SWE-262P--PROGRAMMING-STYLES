#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Global variables for hash table implementation
int count_words = 0;                    // Total number of unique words
const int BUCKET_COUNT = 101;           // Number of buckets in hash table

// Node structure for linked list in hash table
struct Node
{
    string key;     // The word
    int count;      // Frequency count
    Node* next;     // Pointer to next node in linked list
};

// Hash table: array of pointers to linked lists
Node* buckets[BUCKET_COUNT] = {nullptr};

// Hash function: converts string to bucket index
int hash_func(const string& key)
{
    unsigned int h = 0;
    // Simple hash algorithm: multiply by 131 and add character value
    for(char c : key) h = h * 131 + c;
    return h % BUCKET_COUNT;  // Return bucket index
}

// Insert or update word in hash table
void put(string& key)
{
    int h = hash_func(key);           // Get bucket index
    Node* current = buckets[h];       // Start from head of linked list
    
    // Search for existing word in the bucket
    while(current)
    {
        if(current -> key == key)
        {
            current -> count++;       // Increment count if word exists
            return;
        }
        current = current -> next;
    }
    
    // Word not found, create new node and add to front of list
    Node* newNode = new Node{key, 1, buckets[h]};
    buckets[h] = newNode;
    count_words++;                    // Increment total word count
}

// Find and output the word with maximum frequency
void getMax()
{
    int max = INT16_MIN;              // Initialize to minimum value
    
    // First pass: find the maximum count
    for(size_t i = 0; i < BUCKET_COUNT; i++)
    {
        Node* current = buckets[i];
        while(current)
        {
            if(current -> count > max)
                max = current -> count;
            current = current -> next;
        }
    }

    // Second pass: find and output all words with maximum count
    for(size_t i = 0; i < BUCKET_COUNT; i++)
    {
        Node* current = buckets[i];
        while(current)
        {
            if(max == current -> count)
            {
                current -> count = INT16_MIN;  // Mark as processed
                cout << current -> key << "  -  "<< max << endl;
            }
            current = current -> next;
        }
    }
}

// Check if a word is not in the stop words list
bool isNotStopwords(string stopwords[], int count_stopwords, string temp)
{
    // Linear search through stop words array
    for(size_t i = 0; i < count_stopwords; i++)
    {
        if(temp == stopwords[i])
            return false;               // Found in stop words, return false
    }
    return true;                        // Not found, return true
}

int main(int argc, char *argv[]) 
{
    // Check command line arguments
    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
        return 1;
    }

    /* Variable declarations */
    ifstream stop_words_list("../stop_words.txt");    // Stop words file
    ifstream text_file(argv[1]);                      // Input text file
    const string suffixes[] = {"s", "t", "re", "ve", "ll", "d", "m", "nt"};  // Common suffixes to ignore
    string line;                                      // Current line being processed
    string stopwords[INT16_MAX];                      // Array to store stop words
    int count_stopwords = 0;                          // Number of stop words loaded
    int N = 0;                                        // Number of words to output

    /* Load stop words from file */
    while(getline(stop_words_list, line, ',')) 
    {
        stopwords[count_stopwords++] = line;  // Store each stop word
    }
    stop_words_list.close();

    /* Process text file line by line */
    while(getline(text_file, line)) 
    {
        string temp;                          // Current word being built
        
        // Process each character in the line
        for(char& c : line)
        {
            // Convert uppercase to lowercase
            if(c >= 65 && c <= 90) c += 32;
            
            // If character is a letter, add to current word
            if(c >= 97 && c <= 122)
            {
                temp += c;
            }
            // If character is not a letter and we have a word
            else if(!temp.empty()) 
            {   
                // Check if word is a common suffix (ignore these)
                bool is_suffix = false;
                for(const string& suf : suffixes)
                    if(temp == suf) 
                    { 
                        is_suffix = true; break; 
                    }
                // If not a suffix and not a stop word, add to hash table
                if(!is_suffix)
                {
                    if(isNotStopwords(stopwords, count_stopwords, temp))
                        put(temp);
                }
                temp.clear();                 // Reset for next word
            }
        }
        
        // Handle word at end of line
        if(!temp.empty())
        {
            bool is_suffix = false;
            for(const string& suf : suffixes)
                if(temp == suf) 
                { 
                    is_suffix = true; break; 
                }
            if(!is_suffix) 
            {
                if(isNotStopwords(stopwords, count_stopwords, temp))
                    put(temp);
            }
        }
    }
    text_file.close();


    /* Output the 25 most frequent words */
    N = (count_words > 25 ? 25 : count_words);  // Limit to 25 words or total available
    for (size_t i = 0; i < N; i++) 
        getMax();                                // Find and output next most frequent word

    return 0;
}