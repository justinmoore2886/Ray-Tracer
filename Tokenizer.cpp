#include "Tokenizer.h"

#include <sstream>
#include <string>

string downcase(const string &s) {
    string result = "";
    for (int i=0; i<(int)s.length(); i++) {
        char c = s[i];
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
        result += c;
    }
    return result;
}

// Tokenize a file, given its name
Tokenizer::Tokenizer(const char *filename) {
    open(string(filename));
}

Tokenizer::Tokenizer(const string& filename) {
    open(filename);
}

void Tokenizer::open(const string& filename) {
    stream.open(filename);
    if (!stream.is_open()) {
        cerr << "Can't read from " << filename << endl;
        exit(EXIT_FAILURE);
    }
}

// Return next token, as a string
string Tokenizer::next_string() {
    string token;
    stream >> token;
    return downcase(token);
}

void Tokenizer::match(const char *pattern) {
    match(string(pattern));
}

void Tokenizer::match(const string& pattern) {
    string token = next_string();
    if (token != string(pattern)) {
        cerr << "Expected \"" << pattern << "\", got \"" << token << "\"\n";
        exit(EXIT_FAILURE);
    }
}

// Return next token, as a number
float Tokenizer::next_number() {
    string token;
    stream >> token;
    // This, from stackoverflow: convert string to float, detect error
    stringstream ss(token);
    float number;
    ss >> number;
    if (ss.fail()) {
        cerr << "Can't convert " << token << " to number\n";
        exit(EXIT_FAILURE);
    }
    return number;
}

bool Tokenizer::eof() {
    return stream.eof();
}

