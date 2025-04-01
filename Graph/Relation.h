#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <algorithm>

#include "Scheme.h"
#include "Tuple.h"

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

        Relation() { }

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
            // HOW COULD I MAKE THIS CAPABLE OF REORDERING THE COLUMNS FOR RULES STEP 3?
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
            for (const Tuple& tup : tuples) { // random filler variable to initialize newTuple with
                Tuple newTuple;

                for (int index : list) {
                    if (index < 0 || index >= tup.size()) {
                        cout << "Error: Tuple index " << index << " is out of range for tuple of size " << tup.size() << endl;
                    } else {
                        newTuple.push_back(tup.at(index));
                    }
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
            bool commonColumn = false;
            // int count = 0;
            int leftSize = leftScheme.size();

            for (unsigned leftIndex = 0; leftIndex < leftSize; leftIndex++) {
                const string& leftName = leftScheme.at(leftIndex);
                const string& leftValue = leftTuple.at(leftIndex);

                //cout << "left name: " << leftName << " value: " << leftValue << endl;
                for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
                    const string& rightName = rightScheme.at(rightIndex);
                    const string& rightValue = rightTuple.at(rightIndex);
                    if (rightName == leftName) {
                        commonColumn = true;
                        if (rightValue != leftValue) {
                            return false; 
                        }
                    }
                    //cout << "right name: " << rightName << " value: " << rightValue << endl;
                }

                // count++;
            
            }
            // if (count == leftSize) {
            //     return true;
            // }
            return true;
        }

        Relation join(const Relation& right) {
            const Relation& left = *this;
        
            // Create a new scheme by combining the schemes of both relations
            Scheme newScheme = left.scheme;
            vector<int> rightIndicesToAdd;
        
            for (size_t i = 0; i < right.scheme.size(); i++) {
                if (find(newScheme.begin(), newScheme.end(), right.scheme[i]) == newScheme.end()) {
                    newScheme.push_back(right.scheme[i]);
                    rightIndicesToAdd.push_back(i);
                }
            }
        
            Relation result(name, newScheme);
        
            // Combine tuples from both relations
            for (const Tuple& leftTuple : left.tuples) {
                for (const Tuple& rightTuple : right.tuples) {
                    if (joinable(left.scheme, right.scheme, leftTuple, rightTuple)) {
                        Tuple newTuple = leftTuple;
        
                        // Add values from the right tuple that are not already in the left tuple
                        for (int index : rightIndicesToAdd) {
                            newTuple.push_back(rightTuple[index]);
                        }
        
                        result.addTuple(newTuple);
                    }
                }
            }
        
            return result;
        }

        void uni(Relation other) {
            if (this->scheme != other.getScheme()) {
                cout << "Schemes do NOT exactly match" << endl;
            }

            // Add tuples from other relation
            for (Tuple tuple : other.getTuples()) {
                // for (Tuple t : this->tuples) {
                //     // I might need to check for duplicates here
                // }
                if (tuples.find(tuple) == tuples.end()) { // Only adds tuples if not found in tuples
                    this->addTuple(tuple);
                }
            }
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