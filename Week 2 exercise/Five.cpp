#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int count_words = 0;
const int BUCKET_COUNT = 101;

struct Node
{
    string key;
    int count;
    Node* next;
};

Node* buckets[BUCKET_COUNT] = {nullptr};
ifstream text_file;

int hash_func(const string& key)
{
    unsigned int h = 0;
    for(char c : key) h = h * 131 + c;
    return h % BUCKET_COUNT;
}

void put(string& key)
{
    int h = hash_func(key);
    Node* current = buckets[h];
    
    while(current)
    {
        if(current -> key == key)
        {
            current -> count++;
            return;
        }
        current = current -> next;
    }
    
    Node* newNode = new Node{key, 1, buckets[h]};
    buckets[h] = newNode;
    count_words++;
}

void getMax()
{
    int max = INT16_MIN;
    
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

    for(size_t i = 0; i < BUCKET_COUNT; i++)
    {
        Node* current = buckets[i];
        while(current)
        {
            if(max == current -> count)
            {
                current -> count = INT16_MIN;
                cout << current -> key << "  -  "<< max << endl;
            }
            current = current -> next;
        }
    }
}

bool isNotStopwords(string stopwords[], int count_stopwords, string temp)
{
    for(size_t i = 0; i < count_stopwords; i++)
    {
        if(temp == stopwords[i])
            return false;
    }
    return true;
}

void check_args(int argc, char* argv[])
{
    if (argc != 2) 
    {
        cout << "Usage: " << argv[0] << " <text-file>" << endl;
    }
}

void load_stop_words(string stopwords[], int& count_stopwords)
{
    ifstream stop_words_list("../stop_words.txt");
    string line;
    while(getline(stop_words_list, line, ',')) 
    {
        stopwords[count_stopwords++] = line;
    }
    stop_words_list.close();
}

void process_text_file()
{
    const string suffixes[] = {"s", "t", "re", "ve", "ll", "d", "m", "nt"};
    string line;
    string stopwords[INT16_MAX];
    int count_stopwords = 0;
    load_stop_words(stopwords, count_stopwords);
    while(getline(text_file, line)) 
    {
        string temp;
        
        for(char& c : line)
        {
            if(c >= 65 && c <= 90) c += 32;
            
            if(c >= 97 && c <= 122)
            {
                temp += c;
            }
            else if(!temp.empty()) 
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
                temp.clear();
            }
        }
        
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
}

void read_file(const string& path_to_file)
{
    text_file.open(path_to_file);
}

void output_results()
{
    int N = (count_words > 25 ? 25 : count_words);
    for (size_t i = 0; i < N; i++) 
        getMax();
}

int main(int argc, char *argv[]) 
{
    check_args(argc, argv);
    read_file(argv[1]);
    process_text_file();
    output_results();
    return 0;
}