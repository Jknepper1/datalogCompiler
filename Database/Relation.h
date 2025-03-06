#include <vector>
#include <string>
#include <set>
#include "Scheme.h"
#include "Tuple.h"

#pragma once

using namespace std;

// Representes a table in the database

class Relation {
    private:
        string name;
        Scheme scheme;
        set<Tuple> tuples;
    
    public:
        Relation (const string& name, const Scheme& scheme) 
        : name (name), scheme(scheme) { }

        void addTuple(const Tuple& tuple) {
            tuples.insert(tuple);
        }

        string toString() const {
            stringstream out;
            out << name << endl; // THIS LINE WILL NEED TO COME OUT, JUST FOR DEBUGGING
            for (Tuple tup : tuples) {
                out << tup.toString(scheme);
            }
            return out.str();
        }

        Relation select(int index, const string& value) const {
            Relation result(name, scheme);
            /* SUDO CODE FOR BELOW
                for each tuple in the relation
                  if the value at the given index equals the given value
                    add the tuple to the result
            */
            for (Tuple tup : tuples) {
                if (tup.at(index) == value) {
                    result.tuples.insert(tup); // This probably doesn't work
                }            
            }
            return result;
        }

};