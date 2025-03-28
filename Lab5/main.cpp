#include <iostream>

#include "Node.h"

using namespace std;

int main() {

    Node node;
    node.addEdge(4);
    node.addEdge(8);
    node.addEdge(2);
    cout << node.toString() << endl;

    return 0;
}