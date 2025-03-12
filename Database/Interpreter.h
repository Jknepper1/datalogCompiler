#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include "Database.h"
#include "../Parser/DatalogProgram.h"

#pragma once

using namespace std;

class Interpreter {

    private:
        Database database;
        DatalogProgram datalog;

    public:
        Interpreter (const Database& database, const DatalogProgram& datalog) : database(database), datalog(datalog) { }

        void evaluate() {

            // For each scheme in the Datalog Program 
            //      add an empty relation to Database (use scheme name as the name of the relation and attributes from scheme as the relation scheme)
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

                // Fill Values with parameters from fact
                for (Parameter f : param) {
                    string value = f.toString();
                    values.push_back(value);
                }

                // Create a tuple object with values from Parameters
                Tuple tup(values);

                // Find the relation the fact belongs to and then add it as a tuple
                for (Relation& r : database.getRelations()) { // & points to actual database instead of just a copy
                    if (fact.getName() == r.getName()) {
                        r.addTuple(tup);
                    }
                }
            }

            // FOR DEBUGGING RELATION FORMAT AFTER SCHEMES AND FACTS
            // for (Relation& r : database.getRelations()) {
            //     cout << r.toString();
            // }

            // QUERIES DOES THIS NEED TO BE DONE ON A COPY??
            for (Predicate query : datalog.getQueries()) {
                for (Relation r : database.getRelations()) {
                    if (r.getName() == query.getName()) {
                        vector<Parameter> param = query.getParams();
                        
                        // Data for algebra functions check relation.h for a refresher on why these are the way they are
                        vector<string> renameList;
                        unordered_map<string, int> seenVars;
                        vector<int> projectList;


                        // Select tuples from relation that match the query
                        // iterate over the parameters of the query
                        for (size_t i = 0; i < param.size(); i++) {
                            Parameter p = param[i];
                            // If the parameter is a constant, 
                                //select the tuples from the relation with the same value as the constant
                                // n the same position as the constant
                            if (p.isStringLiteral()) { // If the Param is a CONSTANT
                                string varName = p.toString();
                                //cout << "Before Select: " << r.toString() << endl;
                                r = r.select(i, varName);
                                //cout << "After select: " << r.toString() << endl;

                                // renameList.push_back(varName); // This shouldn't be here Rename should only happen on 
                                // projectList.push_back(i);
                            }
                            // If the parameter is a variable and the same variable appears later in the query
                                // select the tuples from the relation that have the same value in both positions 
                            else {
                                string varName = p.toString();
                                if (seenVars.count(varName)) {
                                    //cout << "Before Select2: " << r.toString() << endl;
                                    r = r.select2(seenVars[varName], i);
                                    //cout << "After select2: " << r.toString() << endl;

                                }
                                else {
                                    seenVars[varName] = i;
                                    projectList.push_back(i);
                                    renameList.push_back(varName);
                                    
                                }  
                            }
                        }     
                        // Use project to keep only the oclumns from the relation that correspond to the position of the variables in the query
                        // Make sure each variable name only appears once in the resulting relatioin
                        //cout << "Project list size: " << projectList.size() << endl;
                        r = r.project(projectList);
                        //cout << "After project: " << r.toString() << endl;

                        // Use the rename operation to rename the scheme of the relation to the names of the variables found in the query
                        //cout << "Rename List Size: " << renameList.size() << endl;
                        //cout << "Before rename: " << r.toString() << endl;
                        // cout << "Rename List: " << renameList.at(0) << endl;
                        r = r.rename(renameList);
                        //cout << "After rename: " << r.toString() << endl;


                        // String Printing!
                        cout << query.toString() << "?";
                        if (r.getTuples().size() == 0) {
                            cout << " No" << endl;
                        }
                        else {
                            cout << " Yes(" << r.getTuples().size() << ")" << endl;
                        }
                        for (const Tuple& tuple : r.getTuples()) {
                            cout << " ";
                            bool first = true;
                            for (size_t i = 0; i < tuple.size(); i++) {
                                if (!first) cout << ", ";
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
        };

};