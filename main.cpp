#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "tokenizer.h"

using namespace std;

vector<string> tokenizeFile(string path)
{
    vector<string> tokenized;
    ifstream f(path);
    if (!f.is_open())
    {
        cerr << "Error opening file: " + path;
        return tokenized;
    }

    string s;
    while (getline(f, s))
    {
        string word;
        for (size_t i = 0; i < s.size(); ++i)
        {
            unsigned char ch = static_cast<unsigned char>(s[i]);
            bool apostropheInWord = (ch == '\'' || ch == '`') && !word.empty() && i + 1 < s.size() && isalpha(static_cast<unsigned char>(s[i + 1]));

            if (apostropheInWord)
            {
                word += static_cast<char>(ch);
            }
            else if (ispunct(ch))
            {
                if (!word.empty())
                {
                    for (auto &x : word)
                        x = tolower(x);
                    tokenized.push_back(word);
                    word.clear();
                }
                tokenized.push_back(string(1, static_cast<char>(ch)));
            }
            else if (isspace(ch))
            {
                if (!word.empty())
                {
                    tokenized.push_back(word);
                    word.clear();
                }
            }
            else
            {
                word += static_cast<char>(ch);
            }
        }
        if (!word.empty())
        {
            tokenized.push_back(word);
        }
    }

    return tokenized;
}

vector<vector<int64_t>> tokenizedToDataSet(vector<int64_t> tokens)
{
    const int maxTensorLength = 4;
    vector<vector<int64_t>> tensors;
    for(int i = maxTensorLength; i < tokens.size(); i++)
    {
        vector<int64_t> tensor;
        for (int k = max(i - maxTensorLength, 0); k < i; k++)
        {
            tensor.push_back(tokens[k]);            
        }
        tensors.push_back(tensor);
    }
    return tensors;
}


int main() {
    vector<string> tokens = tokenizeFile("Data/The_Verdict.txt");
    Tokenizer tokenizer = Tokenizer(tokens);
    vector<vector<int64_t>> dataSet = tokenizedToDataSet(tokenizer.encodeTokenSet(tokens));
    for (const auto &entry : dataSet)
    {
        for (const auto &token : entry)
        {
            cout << tokenizer.decodeToken(token) << " ";
        }
        cout << "\n";
    }
    return 0;
}