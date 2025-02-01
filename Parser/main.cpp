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
    vector<Token> tokens = s.tokens;

    Parser p(tokens);


    s.printTotal(); // Prints the total number of tokens scanned

    return 0;
}
