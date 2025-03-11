#include <vector>
#include <string>
#include <iostream>
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Database.h"
#include "Interpreter.h"

using namespace std;


int main() {
    vector<string> names = {"ID", "Name", "Major"};
    
    Scheme scheme(names);

    vector<string> values[]= {
        {"'42'", "'Ann'", "'CS'"},
        {"'32'", "'Bob'", "'CS'"},
        {"'64'", "'Ned'", "'EE'"},
        {"'16'", "'Jim'", "'EE'"},
    };

    

    Database d;

    d.addRelation("student", scheme);

    for (auto& value : values) {
        Tuple tuple(value);
        // cout << tuple.toString(scheme);
        // relation.addTuple(tuple);
        d.getRelation("student").addTuple(tuple);
    }
    d.toString();


    // cout << "relation:" << endl;
    // cout << relation.toString();

    // Relation result = relation.select(2, "'CS'");

    // cout << "select Major='CS' result:" << endl;
    // cout << result.toString();

    return 0;
}