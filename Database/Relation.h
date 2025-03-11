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

        Relation select2(int index1, int index2) const {
            Relation result(name,scheme);

            for (const Tuple& tup : tuples) {
                if (tup.at(index1) == tup.at(index2)) {
                    result.tuples.insert(tup);
                }
            }

            return result;
        }


        Relation project(vector<int> list) {
            // Creates a new scheme based on list of indexes passed in
            vector<string> attributes;
            for (int index : list) {
                attributes.push_back(scheme.at(index));
            }
            Scheme newScheme(attributes);

            // Create new relation result
            Relation result(name, scheme);

            // Iterate through tuples and create new tuples with selected attributes
            for (const Tuple& tup : tuples) {
                vector<string> fill; // random filler variable to initialize newTuple with

                Tuple newTuple(fill);

                for (int index : list) {
                    newTuple.push_back(tup.at(index));
                }

                result.tuples.insert(newTuple);
            }

            return result;
        }

        Relation rename(vector<string> newNames) const {
            if (newNames.size() != scheme.size()){
                cout << "Mismatched attribute count." << endl;
            }

            // Make a new scheme based on the rename list
            Scheme newScheme(newNames);

            // Make a new relation based on new attributes but append original tuples
            Relation result(name, newScheme);
            result.getTuples() = tuples;

            return result;

        }


        string getName() {
            return name;
        }

        set<Tuple> getTuples() {
            return tuples;
        }

};