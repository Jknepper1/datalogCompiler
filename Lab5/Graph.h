#pragma once

#include <map>

#include "Node.h"

class Graph {

    private:

        map<int, Node> nodes;
    
    public:

    Graph(int size) {
        for (int nodeID = 0; nodeID < size; nodeID++) {
            nodes[nodeID] = Node();
        }
    }

    void addEdge(int fromNodeID, int toNodeID) {
        nodes[fromNodeID].addEdge(toNodeID);
    }

};