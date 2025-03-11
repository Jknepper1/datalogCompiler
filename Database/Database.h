#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "Relation.h"

#pragma once

using namespace std;


class Database {
    private:
        vector<Relation> relations; // The database will hold a list of all relations searchable by their names

    public:
        /**
         * this function creates a new relation and appends it to the relations vector
         */
        void addRelation(string n, Scheme s) {
            Relation newRelation(n, s);
            relations.push_back(newRelation);
        }

        // void addTuple(string f, Tuple t){
        //     for (Relation r : relations) {
        //         if (r.getName() == f){ // another for loop?
        //             r.getTuples().insert(t);
        //         }
        //     }
        // }

        // Purely for Debugging for now, but might be part of the lab
        void toString() {
            for (Relation r : relations){
                cout << r.toString();
            }
        }

        vector<Relation> getRelations() {
            return relations;
        }

        Relation& getRelation(const string& name) {
            for (Relation& r : relations) {
                if (r.getName() == name) {
                    return r;
                }
            }
            // throw runtime_error("Relation not found");
        }
        
};