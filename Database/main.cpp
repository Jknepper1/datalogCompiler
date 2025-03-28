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

    string filename = argv[1];
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
    // UNCOMMENT FOR RUNNING THE ACTUAL PROGRAM

    // Relation studentRelation("students", Scheme( {"ID", "Name", "Major"} ));

    // vector<string> studentValues[] = {
    //     {"'42'", "'Ann'", "'CS'"},
    //     {"'32'", "'Ned'", "'EE'"},
    // };

    // for (auto& value : studentValues)
    //     studentRelation.addTuple(Tuple(value));

    // //studentRelation.join(studentRelation);
    
    // Relation courseRelation("courses", Scheme( {"ID", "Course"} ));

    // vector<string> courseValues[] = {
    //     {"'42'", "'CS 100'"},
    //     {"'32'", "'CS 232'"},
    // };

    // for (auto& value : courseValues)
    //     courseRelation.addTuple(Tuple(value));

    // studentRelation.join(courseRelation);

    return 0;
}

// vector<string> names = {"ID", "Name", "Major"};
    
    // Scheme scheme(names);

    // vector<string> values[]= {
    //     {"'42'", "'Ann'", "'CS'"},
    //     {"'32'", "'Bob'", "'CS'"},
    //     {"'64'", "'Ned'", "'EE'"},
    //     {"'16'", "'Jim'", "'EE'"},
    // };


    // data.addRelation("student", scheme);

    // for (auto& value : values) {
    //     Tuple tuple(value);
    //     // cout << tuple.toString(scheme);
    //     // relation.addTuple(tuple);
    //     d.getRelation("student").addTuple(tuple);
    // }

    // cout << "relation:" << endl;
    // cout << relation.toString();

    // Relation result = relation.select(2, "'CS'");

    // cout << "select Major='CS' result:" << endl;
    // cout << result.toString();