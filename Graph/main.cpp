#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Parser.h"
#include "DatalogProgram.h"
#include "Scanner.h"
#include "Token.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Database.h"
#include "Interpreter.h"

using namespace std;


int main(int argc, char* argv[]) {

    string filename = "81test.txt";
    ifstream in(filename);

    stringstream buffer;
    buffer << in.rdbuf();
    string input = buffer.str(); 
    in.close();

    // NOTE: The << operator is an insertion operator, basically the strings in the file are inserted into the buffer
    

    Scanner s(input); 
    s.scan();
    // s.printTotal();
    vector<Token> tokens = s.tokens;
    Parser p(tokens);
    
    try {
       while (p.tokens.size() > 0) {
            p.datalogProgram();
       }
       // Print the program to string
        //p.d.toString(); // This is garbage syntax and probably not very secure...
    
    }
        // Made by copilot as a temporary exception; no idea what this does
    catch (const invalid_argument& e) {
        cout << "Failure!" << endl;
        cout << "  " << e.what() << endl;
    }

    DatalogProgram datalog = p.d;
    Database data;
    Interpreter brain(data, datalog);

    brain.evaluate();

    return 0;
}
