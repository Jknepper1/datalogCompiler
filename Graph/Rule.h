#pragma once

#include <vector>
#include <iostream>
#include "Predicate.h"

using namespace std;

class Rule {
    
    private:
        Predicate head;
        vector<Predicate> body;
        string r;

    public:
        Rule(Predicate head, vector<Predicate> body) : head(head), body(body) {}
        Rule(Predicate head) : head(head) {}
    // Body may need to be initialized differently with this function. Predicate.h may need the same function applied
    // void const addPredicate(Predicate& pred) {
    //     body.push_back(pred);

    // }

    // Added this for lab 5
    void addBodyPredicate(Predicate newPred) {
        body.push_back(newPred);
    }


    string toString() {
        r += head.toString() + " :- ";
        for(int i = 0; i < body.size(); i++) {
            r += body[i].toString();
            if (i < body.size() - 1) {
                r += ",";
            }
        }
        // r += ".";

        return r;
    }

    vector<Predicate> getBody() {
        return body;
    }
    
    Predicate getHead() {
        return head;
    }

    // string toString() {
    //     return r;
    // }


};