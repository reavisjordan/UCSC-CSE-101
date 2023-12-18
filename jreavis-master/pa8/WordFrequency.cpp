/*
Jordan Reavis
jreavis
Spring 23 - CSE101 - pa8
WordFrequency.cpp - Dictionary ADT client file
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    Dictionary D;
    ifstream in;
    ofstream out;
    size_t begin, end, len;
    string tokenBuffer;
    string token;
    string line;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    int frequency = 1;
    
    if(argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return(EXIT_FAILURE);
    }

    in.open(argv[1]);
    if(!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return(EXIT_FAILURE);
    }

    out.open(argv[2]);
    if(!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return(EXIT_FAILURE);
    }

    while(getline(in, line)) {
      len = line.length();
      begin = min(line.find_first_not_of(delim, 0), len);
      end   = min(line.find_first_of(delim, begin), len);
      token = line.substr(begin, end-begin);

      while(token != "") {
        int token_size = token.length();
        for(int i = 0; i < token_size; i++) {
           token[i] = tolower(token[i]);
        }

        if(!D.contains(token)) {
            frequency = 1;
            D.setValue(token, frequency);
        } else {
            D.getValue(token)++;
        }

        begin = min(line.find_first_not_of(delim, end+1), len);
        end   = min(line.find_first_of(delim, begin), len);
        token = line.substr(begin, end-begin);
      }
    }

    out << D;

    in.close();
    out.close();

    return 0;
}
