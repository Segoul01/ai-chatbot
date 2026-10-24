#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include "tokenizer.h"
using namespace std;


const string END_OF_TEXT = "<|EOT|>";
const string UNKOWN_CHARACTER = "<|UNK|>";

Tokenizer::Tokenizer(vector<string> newVocabulary)
{
    vocabulary.clear();
    vocabularyInvert.clear();

    for (string &token : newVocabulary)
    {
        for (char &ch : token)
        {
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        }
    }

    sort(begin(newVocabulary), end(newVocabulary));
    newVocabulary.push_back(END_OF_TEXT);
    newVocabulary.push_back(UNKOWN_CHARACTER);

    for (size_t i = 0; i < newVocabulary.size(); ++i)
    {
        const int64_t tokenId = static_cast<int64_t>(i);
        const string &token = newVocabulary[i];

        vocabulary.emplace(token, tokenId);
        vocabularyInvert.emplace(tokenId, token);
    }
}

int64_t Tokenizer::encodeToken(const string &word)
{
    string lower = word;
    for (auto &x : lower)
        x = tolower(x);
    map<string, int64_t>::const_iterator token = vocabulary.find(lower);
    if (token != vocabulary.end())
    {
        return token->second;
    }

    throw out_of_range("Token is not present in the vocabulary");
}

vector<int64_t> Tokenizer::encodeTokenSet(const vector<string> &tokens)
{
    vector<int64_t> encoded;
    encoded.reserve(tokens.size());
    for (const string &token : tokens)
    {
        encoded.push_back(encodeToken(token));
    }
    return encoded;
}

string Tokenizer::decodeToken(int64_t tokenId) const
{
    map<int64_t, string>::const_iterator token = vocabularyInvert.find(tokenId);
    if (token != vocabularyInvert.end())
    {
        return token->second;
    }
    return UNKOWN_CHARACTER;
}

vector<string> Tokenizer::decodeTokenSet(const vector<int64_t> &tokenIds) const
{
    vector<string> decoded;
    decoded.reserve(tokenIds.size());
    for (int64_t tokenId : tokenIds)
    {
        decoded.push_back(decodeToken(tokenId));
    }
    return decoded;
}