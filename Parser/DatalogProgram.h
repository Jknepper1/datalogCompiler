#pragma once

#include <vector>
#include <set>
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
        set<string> Domains;

        // int sNum = 0;
        // int fNum = 0;
        // int qNum = 0;
        // int rNum = 0;
    
    public:
        void addScheme(Predicate scheme) {
            Schemes.push_back(scheme);
        }

        void addFact(Predicate fact) {
            Facts.push_back(fact);
            
            for (Parameter f : fact.getParams()) { // Loops through each individual parameter in a single fact predicate
                if (f.isStringLiteral()) { // Appens string literals ONLY to Domain list
                    addDomain(f.toString());
                }
            }
        }

        void addQuery(Predicate query) {
            Queries.push_back(query);
        }

        void addRule(Rule rule) { 
            Rules.push_back(rule);
        }

        // A Domain is a string literal inside facts
        void addDomain (string domain) {
            Domains.insert(domain);
        }

    void toString () {
        // Likey going to have a problem comparing int i to size_t .size()
        cout << "Success!" << endl;
        
        cout << "Schemes(" << Schemes.size() << "):" << endl;
        for(int i = 0; i < Schemes.size(); i++) {
            cout << "  "<< Schemes[i].toString() << endl;
        }

        cout << "Facts(" << Facts.size() << "):" << endl;
        for(int i = 0; i < Facts.size(); i++) {
            cout << "  " << Facts[i].toString() << "." << endl;
        }

        cout << "Rules(" << Rules.size() << "):" << endl;
        for(int i = 0; i < Rules.size(); i++) {
            cout << "  " << Rules[i].toString() << endl;
        }

        cout << "Queries(" << Queries.size() << "):" << endl;
        for(int i = 0; i < Queries.size(); i++) {
            cout << "  " << Queries[i].toString() << "?" << endl;
        }

        cout << "Domain(" << Domains.size() << "):" << endl;
        for(auto it = Domains.begin(); it != Domains.end(); it++) {
            cout << "  " << *it << endl;
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
    set<string> getDomains() {
        return Domains;
    }


};