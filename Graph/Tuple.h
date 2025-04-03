#include <vector>
#include <string>
#include "Scheme.h"
#include <sstream>

#pragma once

using namespace std;

class Tuple : public vector<string> {

    public:
   
     Tuple(vector<string> values) : vector<string>(values) { }
     Tuple() { }

     string toString(const Scheme& scheme) const {
        const Tuple& tuple = *this;
        stringstream out;
        for (size_t i = 0; i < tuple.size(); i++) {
            out << scheme.at(i);
            out << "=";
            out << tuple.at(i);
            if (i + 1 != tuple.size()){ // Prevents comma from being the trailing character
                out << ", ";
            }
            else {
                out << endl; // Prints newline
            }
            
        }
        // out << scheme.at(1);
        // out << "=";
        // out << tuple.at(1);
        // out << scheme.at(2);
        // out << "=";
        // out << tuple.at(2);
        // out << tuple.size();
        // out << tuple.at(0);
        return out.str();

     }
   
   };