#if !defined(_TOKENIZER_H_)

#define _TOKENIZER_H_

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////
//
// Given a text file, read it, and break its lines into
// tokens, using whitespace.  Then, recognize whether each
// token is a number or not.
//
//////////////////////////////////////////////////////////

class Tokenizer {
public:
    // Default constructors

    // Tokenize a file, given its name
    Tokenizer(const string& filename);
    Tokenizer(const char *filename);

    // Return next token, as a string
    string next_string();

    // Return next token, as a number
    float next_number();

    // End of file
    bool eof();

    // Get a string token, and die if it doesn't match the pattern
    void match(const string& pattern);
    void match(const char *pattern);

private:
    void open(const string& filename);
    ifstream stream;
};

#endif
