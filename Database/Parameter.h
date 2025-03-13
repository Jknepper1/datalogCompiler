#pragma once

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Parameter {
    private:
        string value;   // Takes value from token
        bool isString; // Used to determine ID | String literal


    public:
        Parameter(string value, bool isString) : value(value), isString(isString) {}

    // bool isString() {
    //     return true;
    // }

    string toString() {
        // cout << value << endl; // For Debugging?
        return value;
    }

    bool isStringLiteral() {
        return isString;
    }
};