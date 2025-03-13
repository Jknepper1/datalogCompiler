#include <vector>
#include <string>
#include <iostream>
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
            for (const Tuple& tup : tuples) {
                if (tup.at(index) == value) {
                    result.tuples.insert(tup);
                }            
            }
            return result;
        }

        Relation select2(int index1, int index2) const {
            Relation result(name,scheme);

            for (const Tuple& tup : tuples) {
                if (tup.at(index1) == tup.at(index2)) {
                    result.tuples.insert(tup); // POTENTIAL PROBLEM
                }
            }

            return result;
        }


        Relation project(vector<int> list) {
            // Creates a new scheme based on list of indexes passed in
            // MAY NEED TO ADD IF STATEMENT TO PREVENT DUPLICATES
            vector<string> attributes;
            for (int index : list) {
                attributes.push_back(scheme.at(index));
            }
            Scheme newScheme(attributes);

            // Create new relation result
            Relation result(name, newScheme);

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
            // if (newNames.size() == 0) {
            //     return *this;
            // }

            // Make a new scheme based on the rename list
            Scheme newScheme(newNames);

            // Make a new relation based on new attributes but append original tuples
            Relation result(name, newScheme);
            for (const Tuple& tup : tuples) {
                result.tuples.insert(tup);  // Insert each tuple into the result relation
            }

            return result;

        }

        

        static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme, const Tuple& leftTuple, const Tuple& rightTuple) {
            // AB
            // 12
            // BC
            // 34 
            // These wouldn't join because column B's tuples don't have matching values
            for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
                const string& leftName = leftScheme.at(leftIndex);
                const string& leftValue = leftTuple.at(leftIndex);
                cout << "left name: " << leftName << " value: " << leftValue << endl;
                for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                    const string& rightName = rightScheme.at(rightIndex);
                    const string& rightValue = rightTuple.at(rightIndex);
                    if (rightName == leftName) {
                        if (rightValue != leftValue) {
                            return false; 
                        } // this conditional doesnt work
                        // I might need to go back to checking for tuple values first
                        // A natural join shouldn't work when schemes dont have a matching column like AB and XY
                    }
                    cout << "right name: " << rightName << " value: " << rightValue << endl;
                }
            }

            
            return true;
        }

        string getName() {
            return name;
        }

        set<Tuple> getTuples() {
            return tuples;
        }

        Scheme getScheme() {
            return scheme;
        }

};