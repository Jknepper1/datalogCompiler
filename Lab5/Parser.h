#include <iostream>
#include <vector>
#include "Token.h"
#include "DatalogProgram.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class Parser {
    private:
        // Empty

    public:
        vector<Token> tokens;
        DatalogProgram d;
        Parser(const vector<Token>& tokens) : tokens(tokens) { }


    /********************
     *   MAIN GRAMMAR   *
     ********************/

     void datalogProgram() {
        match(SCHEMES);
        match(COLON);
        scheme();
        schemeList();

        match(FACTS);
        match(COLON);
        factList();

        match(RULES);
        match(COLON);
        ruleList();

        match(QUERIES);
        match(COLON);
        query();
        queryList();

        match(END);
        //NOTE: Potentially add an EOF check to prevent out-of-bounds errors
    }

    void schemeList() {     // NOTE: Checks for terminal in first function call
        if (tokenType() == ID){
            scheme();
            schemeList();
        }
        else {
            // lambda
        }
    }

    void factList() {
        if (tokenType() == ID){ //NOTE: Might need to be more specific of a check
            fact();
            factList();
        }
        else {
            // lambda
        }
    }

    void ruleList() {
        if (tokenType() == ID) { // Might be best to narrow the check to rule specific
            rule();
            ruleList();
        }
        else {
            // lambda
        }
    }

    void queryList() {
        if (tokenType() == ID) { // NOTE: Might need to also handle Q_MARK according to ChatGPT
            query();
            queryList();
        }
        else {
            // lambda
        }
    }

    void scheme() {
        if (tokenType() == ID) {  
            vector<Parameter> params;
            string schemeName = tokens.at(0).toString();  // passes first ID as name and implants empty parameter list
            match(ID);
            match(LEFT_PAREN);
            params.push_back(Parameter(tokens.at(0).toString(), false)); 
            match(ID);
            idList(params);
            match(RIGHT_PAREN);

            Predicate schemePred(schemeName, params);
            d.addScheme(schemePred);
        }
        else {
            throwError();
        }
    }

    void fact() {
        if (tokenType() == ID) {
            string factName = tokens.at(0).toString();
            match(ID);
            match(LEFT_PAREN);
            vector<Parameter> params;
            params.push_back(Parameter(tokens.at(0).toString(), true));
            match(STRING);
            stringList(params);
            match(RIGHT_PAREN);
            match(PERIOD);

            Predicate factPred(factName, params);
            d.addFact(factPred);
        }
        else {
            throwError();
        }
    }

    void rule() {
        Predicate head = headPredicate();
        match(COLON_DASH);
        vector<Predicate> body;
        body.push_back(predicate());
        predicateList(body);
        match(PERIOD);

        Rule rule(head, body);
        d.addRule(rule);
    }

    void query() {
        Predicate queryPred = predicate();
        match(Q_MARK);
        d.addQuery(queryPred);
    }

    Predicate headPredicate() { // Will changing the return type brick this?
        if (tokenType() == ID) {
            vector<Parameter> params;
            string predName = tokens.at(0).toString();
            match(ID);
            match(LEFT_PAREN);
            params.push_back(Parameter(tokens.at(0).toString(), false));
            match(ID);
            idList(params);
            match(RIGHT_PAREN);

            Predicate head = Predicate(predName, params);
            return head;
        }
        else {
            throwError();
        }
    }

    Predicate predicate() {
        if (tokenType() == ID) {
            vector<Parameter> params;
            string predName = tokens.at(0).toString();
            match(ID);
            match(LEFT_PAREN);
            parameter(params);
            parameterList(params);
            match(RIGHT_PAREN);

            Predicate body = Predicate(predName, params);
            return body;
        }
        else {
            throwError();
        }
    }

    void predicateList(vector<Predicate>& body) {
        if (tokenType() == COMMA) {
            match(COMMA);
            Predicate pred = predicate();
            body.push_back(pred);
            predicateList(body);
        }
        else {
            // lambda
        }
    }

    void parameterList(vector<Parameter>& params) {
        if (tokenType() == COMMA) {
            match(COMMA);
            parameter(params);
            parameterList(params);
        }
        else {
            // lambda
        }
    }

    void stringList(vector<Parameter>& params) {
        if (tokenType() == COMMA) {
            match(COMMA);
            params.push_back(Parameter(tokens.at(0).toString(), true));
            match(STRING);
            stringList(params);
        }
        else {
            // Lambda
        }
    }

    void idList(vector<Parameter>& params) {
        if (tokenType() == COMMA) {
            match(COMMA);
            params.push_back(Parameter(tokens.at(0).toString(), false));
            match(ID);
            idList(params);
        } else {
        // lambda (epsilon)
        }
    }

    void parameter(vector<Parameter>& params) {
        if (tokenType() == STRING) {
            params.push_back(Parameter(tokens.at(0).toString(), true));
            match(STRING);
        }
        else if (tokenType() == ID) {
            params.push_back(Parameter(tokens.at(0).toString(), false));
            match(ID);
        }
        else {
            throwError();
        }
    }

    /********************************
     *     SUPPORTING FUNCTIONS     *
     ********************************/
    
    void match(TokenType t) {
        // cout << "match: " << t << endl; // For debugging
        // add code for matching token type t
        if (tokenType() == t) {
            advanceToken();
        }
        else {
            throwError();
        }
    }
    
    TokenType tokenType() const {
        return tokens.at(0).getType();
    }

    void advanceToken() {
        tokens.erase(tokens.begin());
    }

    void throwError() {
        throw invalid_argument(tokens.at(0).tokenData()); // Prints error token
    }
};