#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include "Database.h"
#include "DatalogProgram.h"

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

                // Fill values with parameters from fact
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
            
            // TA notes:
            // Ensure matching lowercase letters do in fact match the uppercase N
            // So a RENAME may need to happen to make all parts of the scheme uppercase and matchable
            // 

            // RULES
            // FOR PROJECT TO WORK I NEED TO MAKE SURE AN ORDERED LIST IS PASSED INTO IT
            // I MIGHT BE ABLE TO ACCOMPLISH THIS BY USING A SET AS THE PROJETLIST DATA TYPE 
            // THEN CONVERTING THAT SET INTO A VECTOR SO IT MATCHES THE RIGHT TYPE FOR SCHEME?
            vector<Relation> tempRelations;
            for (Rule rule : datalog.getRules()) {
                tempRelations.clear(); // Wipes the list before new temps are appended

            //     vector<Predicate> rightHand = rule.getBody();
            //      // Set left hand relation to first relation and right hand equal to next relation
                
            //     // COPIED FROM SCHEME EVALUATION ABOVE
            //     // STEP 1 of Rule Evaluation
            //     for (Predicate p : rightHand) {
            //         vector<string> attributes;
            //         vector<Parameter> param = p.getParams();
                    
            //         // Extracts the values of each parameter of the scheme to be inserted into the database
            //         for (Parameter p : param) {
            //             string val = p.toString();
            //             attributes.push_back(val);
            //         }
            //         // Creates the temp relation
            //         Relation temp(p.getName(), attributes);
            //         // How do I get the facts into the relation?? I can use the scheme logic to build the relation scheme, 
            //         // but what about the values?? Just use the facts logic.

            //         // COPIED FACTS LOGIC AND ADJUSTED
            //         for (Predicate fact : datalog.getFacts()) {
            //             vector<string> values;
            //             vector<Parameter> param = fact.getParams();
        
            //             // Fill values with parameters from fact
            //             for (Parameter f : param) {
            //                 string value = f.toString();
            //                 values.push_back(value);
            //             }
        
            //             // Create a tuple object with values from Parameters
            //             Tuple tup(values);
        
            //             // Find the relation the fact belongs to and then add it as a tuple
            //             if (fact.getName() == temp.getName()) {
            //                 temp.addTuple(tup);
            //             }
            //         }
            //         tempRelations.push_back(temp);
            //     }
                
                // Step 1 of Eval

                for (Predicate p : rule.getBody()) {
                    Relation r = database.getRelation(p.getName()); // & reference?
                    vector<Parameter> params = p.getParams();
                    vector<int> projectList;
                    unordered_map<string, int> seenVars;
                    vector<string> renameList;

                    for (size_t i = 0; i < params.size(); i++) {
                        Parameter param = params[i];
                        if (param.isStringLiteral()) {
                            r = r.select(i, param.toString());
                        }
                        else {
                            string varName = param.toString();
                            if (seenVars.count(varName)) {
                                r = r.select2(seenVars[varName], i);
                            }
                            else {
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
                // Set result to first relation; iterate through rest of relations appending joins to result
                Relation result = tempRelations[0];
                if (tempRelations.size() > 1) {
                    for (size_t i = 1; i < tempRelations.size(); i++) {
                        result = result.join(tempRelations[i]);
                    }
                } // There might be excess values here or duplicates
                // cout << result.toString();

                // Step 3 of eval
                vector<int> projectList;
                vector<string> headAttributes;

                // Get all attributes from head pred into comparison list
                for (Parameter p : rule.getHead().getParams()) {
                    headAttributes.push_back(p.toString());
                }

                // Searches for each head attribute in the relation's scheme and returns it's index to be projected with
                for (string attr : headAttributes) {
                    for (size_t i = 0; i < result.getScheme().size(); i++) {
                        if (result.getScheme()[i] == attr) {
                            projectList.push_back(i);
                        }
                    }
                }

                result = result.project(projectList); // SEcond rule has two Gs G and g

                // Step 4 of Eval
                


            }
            // Relation population proof
            for (Relation r : tempRelations) {
                cout << r.toString();
            }

            

            // QUERIES - DOES THIS NEED TO BE DONE ON A COPY??
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
                        // Use project to keep only the columns from the relation that correspond to the position of the variables in the query
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