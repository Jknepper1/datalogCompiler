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

    string filename = "81Test.txt";
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














// #include <vector>
// #include <string>
// #include <iostream>
// #include <fstream>

// #include "Parser.h"
// #include "DatalogProgram.h"
// #include "Scanner.h"
// #include "Token.h"
// #include "Scheme.h"
// #include "Tuple.h"
// #include "Relation.h"
// #include "Database.h"
// #include "Interpreter.h"
// #include "Graph.h"
// #include "Node.h"

// using namespace std;

// int main() {

//     // Node node;
//     // node.addEdge(4);
//     // node.addEdge(8);
//     // node.addEdge(2);
//     // cout << node.toString() << endl;

//     // Graph graph(3);
//     // graph.addEdge(1,2);
//     // graph.addEdge(1,0);
//     // graph.addEdge(0,1);
//     // graph.addEdge(1,1);
//     // cout << graph.toString();

//     // predicate names for fake rules
//     // first is name for head predicate
//     // second is names for body predicates
//     // pair<string,vector<string>> ruleNames[] = {
//     //     { "A", { "B", "C" } },
//     //     { "B", { "A", "D" } },
//     //     { "B", { "B" } },
//     //     { "E", { "F", "G" } },
//     //     { "E", { "E", "F" } },
//     //   };

//     // vector<Rule> rules;

//     // for (auto& rulePair : ruleNames) {
//     //     string headName = rulePair.first;
//     //     Rule rule = Rule(Predicate(headName));
//     //     vector<string> bodyNames = rulePair.second;
//     //     for (auto& bodyName : bodyNames)
//     //     rule.addBodyPredicate(Predicate(bodyName));
//     //     rules.push_back(rule);
//     // }

//     // Graph graph = Interpreter::makeGraph(rules);
//     // cout << graph.toString();

//     // return 0;
    
// }