#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Predicate.h"
#include "Rule.h"
#include "Parameter.h"


using namespace std;
/* Will be in charge of handling the final output in list format*/

class DatalogProgram {
    private:
        vector<Predicate> Schemes;
        vector<Predicate> Facts;
        vector<Predicate> Queries;
        vector<Rule> Rules;
        vector<string> Domains;

        int sNum = 0;
        int fNum = 0;
        int qNum = 0;
        int rNum = 0;
        int dNum = 0;
    
    public:
        void addScheme(Predicate scheme) {
            Schemes.push_back(scheme);
        }

        void addFacts(Predicate fact) {
            Facts.push_back(fact);
            
            for (Parameter f : fact.getParams()) { // Loops through each individual parameter in a single fact predicate
                if (f.isStringLiteral()) { // Appens string literals ONLY to Domain list
                    addDomain(f.toString());
                }
            }
        }

        void addQueries(Predicate query) {
            Queries.push_back(query);
        }

        void addRule(Rule rule) { 
            Rules.push_back(rule);
        }

        void addDomain (string domain) {
            Domains.push_back(domain);
        }

    string toString () {
        /* Print SUCCESS! Here but maybe not in this function. Perhaps in main?*/

        cout << "Schemes(" << sNum << "):" << endl;
        for(int i = 0; i < Schemes.size(); i++) {
            cout << Schemes[i].toString() << endl;
        }

        cout << "Facts(" << fNum << "):" << endl;
        for(int i = 0; i < Facts.size(); i++) {
            cout << Facts[i].toString() << endl;
        }

        cout << "Rules(" << qNum << "):" << endl;
        for(int i = 0; i < Rules.size(); i++) {
            cout << Rules[i].toString() << endl;
        }

        cout << "Queries(" << rNum << "):" << endl;
        for(int i = 0; i < Queries.size(); i++) {
            cout << Queries[i].toString() << endl;
        }

        cout << "Domain(" << dNum << "):" << endl;
        for(int i = 0; i < Domains.size(); i++) {
            cout << Domains[i] << endl;
        }
    }

    // Getter function for future parts of the compiler
    vector<Predicate> getSchemes() {
        return Schemes;
    }
    vector<Predicate> getFacts() {
        return Facts;
    }
    vector<Predicate> getQueries() {
        return Queries;
    }
    vector<Rule> getRules() {
        return Rules;
    }
    vector<string> getDomains() {
        return Domains;
    }


};