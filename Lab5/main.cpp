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
#include "Graph.h"
#include "Node.h"

using namespace std;

int main() {

    // Node node;
    // node.addEdge(4);
    // node.addEdge(8);
    // node.addEdge(2);
    // cout << node.toString() << endl;

    // Graph graph(3);
    // graph.addEdge(1,2);
    // graph.addEdge(1,0);
    // graph.addEdge(0,1);
    // graph.addEdge(1,1);
    // cout << graph.toString();

    // predicate names for fake rules
    // first is name for head predicate
    // second is names for body predicates
    pair<string,vector<string>> ruleNames[] = {
        { "A", { "B" } },
        { "B", { "B", "A" } },
    };

    vector<Rule> rules;

    for (auto& rulePair : ruleNames) {
        string headName = rulePair.first;
        Rule rule = Rule(Predicate(headName));
        vector<string> bodyNames = rulePair.second;
        for (auto& bodyName : bodyNames)
        rule.addBodyPredicate(Predicate(bodyName));
        rules.push_back(rule);
    }

    Graph graph = Interpreter::makeGraph(rules);
    cout << graph.toString();

    return 0;
}