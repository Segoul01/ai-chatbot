#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <map>
#include <cstdint>
using namespace std;

class Tokenizer
{
public:
    Tokenizer(vector<string> tokens);
    int64_t encodeToken(const string &word);
    vector<int64_t> encodeTokenSet(const vector<string> &tokens);
    string decodeToken(int64_t tokenId) const;
    vector<string> decodeTokenSet(const vector<int64_t> &tokenIds) const;

    map<string, int64_t> vocabulary;
    map<int64_t, string> vocabularyInvert;
};

#endif