#include <iostream>
#include <sstream>
#include <fstream>
#include "Token.h"
#include "Scanner.h"

using namespace std;

int main(int argc, char* argv[]) {
    string filename = argv[1];
    ifstream in(filename);

    stringstream buffer;
    buffer << in.rdbuf();
    string input = buffer.str(); 
    in.close();
    // NOTE: The << operator is an insertion operator, basically the strings in the file are inserted into the buffer
    
    Scanner s(input); 
    // size_t count = input.length();

    // Debugging Log
    // cout << input << endl;
    // cout << input.length() << endl;
    // cout << count << endl;

    s.scan();

    // Token t = s.eof();
    // cout << t.toString() << endl;
    s.printTotal(); // Prints the total number of tokens scanned

    return 0;
}
