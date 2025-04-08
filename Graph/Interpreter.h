#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "Database.h"
#include "DatalogProgram.h"
#include "Graph.h"

#pragma once

using namespace std;

class Interpreter {

  private:
    Database database;
    DatalogProgram datalog;

  public:
    Interpreter(const Database &database, const DatalogProgram &datalog)
        : database(database), datalog(datalog) {}

    bool evaluateRule(Rule& rule) {
        vector<Relation> tempRelations;
        int passes = 0;

        // Step 1 of Eval
        for (Predicate& p : rule.getBody()) {
            Relation r = database.getRelation(p.getName());
            vector<Parameter> params = p.getParams();
            vector<int> projectList;
            unordered_map<string, int> seenVars;
            vector<string> renameList;

            for (size_t i = 0; i < params.size(); i++) {
                Parameter param = params[i];
                if (param.isStringLiteral()) {
                    r = r.select(i, param.toString());
                } else {
                    string varName = param.toString();
                    if (seenVars.count(varName)) {
                        r = r.select2(seenVars[varName], i);
                    } else {
                        seenVars[varName] = i;
                        projectList.push_back(i);
                        renameList.push_back(varName);
                    }
                }
            }
            r = r.project(projectList);
            r = r.rename(renameList);
            tempRelations.push_back(r);
        }

        // Step 2 of eval

        // Set result to first relation; iterate through rest of relations appending joins
        // to result
        Relation result = tempRelations[0];
        if (tempRelations.size() > 1) {
            for (size_t i = 1; i < tempRelations.size(); i++) {
                result = result.join(tempRelations[i]);
            }
        }
        // cout << "Joined but not Projected" << endl << result.toString();

        // Step 3 of eval
        vector<int> projectList;
        vector<string> headAttributes;

        // Get all attributes from head pred into comparison list
        for (Parameter& p : rule.getHead().getParams()) {
            headAttributes.push_back(p.toString());
        }

        // Searches for each head attribute in the relation's scheme and returns it's index
        // to be projected with
        for (string& attr : headAttributes) {
            for (size_t i = 0; i < result.getScheme().size(); i++) {
                if (result.getScheme()[i] == attr) {
                    projectList.push_back(i);
                }
            }
        }

        result = result.project(projectList);
        // cout << result.toString();

        // Step 4 of Eval
        vector<string> renameAttributes;
        for (Relation r : database.getRelations()) {

            if (r.getName() == rule.getHead().getName()) {
                // cout << "Found it!" << endl;
                for (string s : r.getScheme()) {
                    renameAttributes.push_back(s);
                }
            }
        }

        // Create a new relation with the renamed scheme
        result = result.rename(renameAttributes);

        // Step 5 of Eval

        Relation& target = database.getRelation(rule.getHead().getName());

        // Iteration tracking
        size_t beforeSize = target.getTuples().size();
        set<Tuple> tuplesBefore = target.getTuples();

        target.uni(result);

        size_t afterSize = target.getTuples().size();
        set<Tuple> tuplesAfter = target.getTuples();

        vector<Tuple> newlyAdded;
            for (Tuple t : tuplesAfter) {
                if (tuplesBefore.find(t) == tuplesBefore.end()) {
                    newlyAdded.push_back(t);
                }
            }
        
        if (!newlyAdded.empty()) {
            for (Tuple t : newlyAdded){    
                for (size_t i = 0; i < t.size(); i++) {
                    cout << "  " << target.getScheme().at(i) << "=" << t.at(i);
                    if (i < t.size() - 1) {
                        cout << ",";
                    }
                }
                cout << endl;
            }
        }

        if (beforeSize < afterSize) {
            //cout << "Rule R"<< " added " << (afterSize - beforeSize) << " new tuples." << endl;
            return true;
        } else {
            //cout << "Rule R" << " added no new tuples." << endl;
            return false;
        }
    }

    void evaluateSCCs(vector<set<int>>& SCCs, Graph ogGraph) {
        for (set<int>& SCC : SCCs) {
            int passes = 0;

            cout << "SCC: ";
            for (int ruleID : SCC) {
                cout << "R" << ruleID << " ";
            }
            cout << endl;
            
        
            bool addedNewTuples = true;
            bool singleRule = (SCC.size() == 1);
            int ruleID = *SCC.begin();

            bool selfDepend = singleRule && ogGraph.getNodes().at(ruleID).getEdges().count(ruleID);

            if (singleRule && !selfDepend) {
                //cout << "Evaluating single rule R" << ruleID << endl;
                evaluateRule(datalog.getRules()[ruleID]);
                passes = 1;

            } 
            else {
                while (addedNewTuples) {
                    addedNewTuples = false;

                    Rule rule = datalog.getRules()[ruleID];
                    cout << rule.toString() << "." << endl;

                    Relation target = database.getRelation(rule.getHead().getName());

                    for (int ruleID : SCC) {
                        bool newTuples = evaluateRule(datalog.getRules()[ruleID]);
                        if (newTuples) {
                            addedNewTuples = true;
                        }
                        passes++;
                    }
                }
            }
            cout << passes << " passes:" << " R" << ruleID << endl;
        }

    }

    void evaluate() {

        // For each scheme in the Datalog Program
        //      add an empty relation to Database (use scheme name as the name of the relation and
        //      attributes from scheme as the relation scheme)
        for (Predicate scheme : datalog.getSchemes()) {
            vector<string> attributes;
            vector<Parameter> param = scheme.getParams();

            // Extracts the values of each parameter of the scheme to be inserted into the database
            for (Parameter p : param) {
                string val = p.toString();
                attributes.push_back(val);
            }

            database.addRelation(scheme.getName(), attributes);
        }

        // FACTS
        for (Predicate fact : datalog.getFacts()) {
            vector<string> values;
            vector<Parameter> param = fact.getParams();

            // Fill values with parameters from fact
            for (Parameter f : param) {
                string value = f.toString();
                values.push_back(value);
            }

            // Create a tuple object with values from Parameters
            Tuple tup(values);

            // Find the relation the fact belongs to and then add it as a tuple
            for (Relation &r :
                 database.getRelations()) { // & points to actual database instead of just a copy
                if (fact.getName() == r.getName()) {
                    r.addTuple(tup);
                }
            }
        }

        // FOR DEBUGGING RELATION FORMAT AFTER SCHEMES AND FACTS
        // for (Relation& r : database.getRelations()) {
        //     cout << r.toString();
        // }

        // DEPENDENCY GRAPH
        Graph ogGraph = makeGraph(datalog.getRules());
        cout << "Dependency Graph: " << endl << ogGraph.toString() << endl;

        Graph reversed = reverse(ogGraph);
        //cout << "Reversed Graph: " << endl << reversed.toString() << endl;

        stack<int> nodes = dfsForest(reversed);
        //cout << "DFS Forest Result: " << endl;
        // while (nodes.size() != 0) {
        //     int i = nodes.top();
        //     cout << i << endl;
        //     nodes.pop();
        // }

        vector<set<int>> SCCs = findSCCs(nodes, ogGraph);
        int i = 0;
        // for (auto& scc : SCCs) {
        //     cout << "SCC: " << i << endl;
        //     for (auto& j : scc) {
        //         cout << "   Node: " << j << endl;
        //     }
        //     i++;
        // }

        // RULES
        cout << "Rule Evaluation" << endl;
        evaluateSCCs(SCCs, ogGraph);
        cout << endl;

        // QUERIES - DOES THIS NEED TO BE DONE ON A COPY??
        cout << "Query Evaluation" << endl;
        for (Predicate query : datalog.getQueries()) {
            for (Relation r : database.getRelations()) {
                if (r.getName() == query.getName()) {
                    vector<Parameter> param = query.getParams();

                    // Data for algebra functions check relation.h for a refresher on why these are
                    // the way they are
                    vector<string> renameList;
                    unordered_map<string, int> seenVars;
                    vector<int> projectList;

                    // Select tuples from relation that match the query
                    // iterate over the parameters of the query
                    for (size_t i = 0; i < param.size(); i++) {
                        Parameter p = param[i];
                        // If the parameter is a constant,
                        // select the tuples from the relation with the same value as the constant
                        // n the same position as the constant
                        if (p.isStringLiteral()) { // If the Param is a CONSTANT
                            string varName = p.toString();
                            // cout << "Before Select: " << r.toString() << endl;
                            r = r.select(i, varName);
                            // cout << "After select: " << r.toString() << endl;

                            // renameList.push_back(varName); // This shouldn't be here Rename
                            // should only happen on projectList.push_back(i);
                        }
                        // If the parameter is a variable and the same variable appears later in the
                        // query select the tuples from the relation that have the same value in
                        // both positions
                        else {
                            string varName = p.toString();
                            if (seenVars.count(varName)) {
                                // cout << "Before Select2: " << r.toString() << endl;
                                r = r.select2(seenVars[varName], i);
                                // cout << "After select2: " << r.toString() << endl;

                            } else {
                                seenVars[varName] = i;
                                projectList.push_back(i);
                                renameList.push_back(varName);
                            }
                        }
                    }
                    // Use project to keep only the columns from the relation that correspond to the
                    // position of the variables in the query Make sure each variable name only
                    // appears once in the resulting relatioin
                    // cout << "Project list size: " << projectList.size() << endl;
                    r = r.project(projectList);
                    // cout << "After project: " << r.toString() << endl;

                    // Use the rename operation to rename the scheme of the relation to the names of
                    // the variables found in the query
                    // cout << "Rename List Size: " << renameList.size() << endl;
                    // cout << "Before rename: " << r.toString() << endl;
                    // cout << "Rename List: " << renameList.at(0) << endl;
                    r = r.rename(renameList);
                    // cout << "After rename: " << r.toString() << endl;

                    // String Printing!
                    cout << query.toString() << "?";
                    if (r.getTuples().size() == 0) {
                        cout << " No" << endl;
                    } else {
                        cout << " Yes(" << r.getTuples().size() << ")" << endl;
                    }
                    for (const Tuple &tuple : r.getTuples()) {
                        cout << "  ";
                        bool first = true;
                        for (size_t i = 0; i < tuple.size(); i++) {
                            if (!first) {
                                cout << ", ";
                            }
                            cout << r.getScheme().at(i) << "=" << tuple.at(i);
                            first = false;
                        }
                        if (tuple.size() > 0) {
                            cout << endl;
                        }
                    }
                }
            }
        }
    }

    static Graph makeGraph(const vector<Rule> &rules) {
        Graph graph(rules.size());
        // Code to add edges
        for (int i = 0; i < rules.size(); i++) {
            Rule r = rules[i];
            // cout << "from rule R" << i << ": " << r.toString() << endl;
            for (Predicate p : r.getBody()) {
                // cout << "from body predicate: " << p.toString() << endl;
                for (int j = 0; j < rules.size(); j++) {
                    Rule toRule = rules[j];
                    // cout << "to rule " << "R" << j << ": " << toRule.toString() << endl;
                    if (p.getName() == toRule.getHead().getName()) {
                        // cout << "Dependency found: (R" << i << ",R" << j << ")" << endl;
                        graph.addEdge(i, j);
                    }
                }
            }
        }
        return graph;
    }

    Graph reverse(Graph ogGraph) {
        Graph reversed(ogGraph.getNodes().size()); // Create new graph with same number of nodes
        for (auto &pair : ogGraph.getNodes()) {    // Iterate over the nodes in original graph
            // int fromNode = pair.second;
            int fromNode = pair.first;                  // Get current node ID
            for (int toNode : pair.second.getEdges()) { // Iterate over edges of current node
                reversed.addEdge(toNode, fromNode);     // REverse edge direction in new graph
            }
        }

        return reversed;
    }

    stack<int> dfsForest(Graph &graph) {
        set<int> visited;
        stack<int> postorder;

        for (auto &pair : graph.getNodes()) {
            int nodeID = pair.first;
            if (visited.find(nodeID) == visited.end()) { // If node is not in visited
                dfs(nodeID, visited, postorder, graph);
            }
        }

        return postorder;
    }

    void dfs(int nodeID, set<int> &visited, stack<int> &postorder, Graph &graph) {
        visited.insert(nodeID);
        for (auto &neighbor :
             graph.getNodes().at(nodeID).getEdges()) { // Iterates through each node adjacent to x
            if (visited.find(neighbor) == visited.end()) {
                dfs(neighbor, visited, postorder, graph);
            }
        }
        postorder.push(nodeID);
    }

    vector<set<int>> findSCCs(stack<int> &postorder, Graph &ogGraph) {
        vector<set<int>> SCCs;
        set<int> visited;

        while (!postorder.empty()) {
            int nodeID = postorder.top();
            postorder.pop();

            if (visited.find(nodeID) == visited.end()) {
                set<int> currentSCC;
                dfsSCCs(nodeID, visited, currentSCC, ogGraph);
                SCCs.push_back(currentSCC);
            }
        }
        return SCCs;
    }

    void dfsSCCs(int nodeID, set<int> &visited, set<int> &currentSCC, Graph &graph) {
        visited.insert(nodeID);
        currentSCC.insert(nodeID);

        for (int neighbor :
             graph.getNodes().at(nodeID).getEdges()) { // Iterates through each node adjacent to x
            if (visited.find(neighbor) == visited.end()) {
                dfsSCCs(neighbor, visited, currentSCC, graph);
            }
        }
    }
};