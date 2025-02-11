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
    
    try {
       while (p.tokens.size() > 0) {
            p.datalogProgram();
       }
       // Print the program to string
       p.d.toString(); // This is garbage syntax and probably not very secure...
    
    }
        // Made by copilot as a temporary exception; no idea what this does
        catch (const invalid_argument& e) {
        cerr << "Failure!\n" << e.what() << endl;
    }

    return 0;
}
