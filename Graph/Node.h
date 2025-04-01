#pragma once

#include <set>
#include <iostream>
#include <string>
#include <sstream>


using namespace std;

class Node {

    private:

    set<int> adjacentNodeIDs;

    public:

        void addEdge(int adjacentNodeID) {
            adjacentNodeIDs.insert(adjacentNodeID);
        }

        string toString() { 
            stringstream out;

            int i = 0; // Used to track commas
            for (int node : adjacentNodeIDs) {
                out << "R" << node;
                if (i < (adjacentNodeIDs.size() - 1)) {
                    out << ",";
                }
                i++;
            }
            return out.str();

        }
};