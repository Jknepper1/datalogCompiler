#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Parameter.h"

using namespace std;
/* This class will store the schemes facts and query objects 
as they have the same form as a predicate*/

class Predicate {
    
    private:
        string name;
        vector<Parameter> parameters;
        string pred;
    
    public:
        Predicate(string name, vector<Parameter> parameters) : name(name), parameters(parameters) {}

    string toString() { // Builds Schemes, Facts, and Queries as well
        pred = name + "(";
        for (int i = 0; i < parameters.size(); i++) {
            pred += parameters[i].toString();
            if (i < parameters.size() - 1) {// Adds commas after all elements except last 
                pred += ",";
            }
            pred  += ")";
        }

        return pred;
    }

    vector<Parameter> getParams() {
        return parameters;
    }


};