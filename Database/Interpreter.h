#include <vector>
#include <string>
#include "Database.h"
#include "Parser/DatalogProgram.h"

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
                for (Parameter p : param){
                    string val = p.toString();
                    attributes.push_back(val);
                }
                
                database.addRelation(scheme.getName(), attributes);
            }
        };

};