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

        // Purely for Debugging for now, but might be part of the lab
        string toString() {
            stringstream out;
            for (Relation r : relations) {
                out << r.toString();
            }
        }
};