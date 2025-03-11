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
                            if (p.isStringLiteral()) {
                                r.select(i, p.toString());
                            }
                            // If the parameter is a variable and the same variable appears later in the query
                                // select the tuples from the relation that have the same value in both positions 
                            else {
                                string varName = p.toString();
                                if (seenVars.count(varName)) {
                                    r.select2(seenVars[varName], i);
                                }
                                else {
                                    seenVars[varName] = i;
                                    projectList.push_back(i);
                                    
                                }  
                            }
                        }     
                        // Use project to keep only the oclumns from the relation that correspond to the position of the variables in the query
                        // Make sure each variable name only appears once in the resulting relatioin
                        r.project(projectList);
                        // Use the rename operation to rename the scheme of the relation to the names of the variables found in the query
                        r.rename(renameList);

                        cout << r.toString() << endl;
                    }
                }
            }
        };

};