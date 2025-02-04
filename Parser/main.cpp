#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "Scanner.h"
#include "Parser.h"

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
    s.scan();
    s.printTotal();
    vector<Token> tokens = s.tokens;

    //NOTE: Build a try/catch block around this call
    Parser p(tokens);
    while (true) {
        cout << p.tokenType() << endl;
        p.advanceToken();
    }

     // Prints the total number of tokens scanned

    return 0;
}
