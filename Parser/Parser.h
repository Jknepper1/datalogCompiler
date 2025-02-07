#include <vector>
#include "Token.h"

using namespace std;

class Parser {
    private:
        vector<Token> tokens;
        

    public:
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

    void schemeList() {     //NOTE: Checks for terminal in first function call
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
            match(ID);
            match(LEFT_PAREN);
            match(ID);
            idList();
            match(RIGHT_PAREN);
        }
        else {
            throwError();
        }
    }

    void fact() {
        if (tokenType() == ID) {
            match(ID);
            match(LEFT_PAREN);
            match(STRING);
            stringList();
            match(RIGHT_PAREN);
            match(PERIOD);
        }
        else {
            throwError();
        }
    }

    void rule() {
        headPredicate();
        match(COLON_DASH);
        predicate();
        predicateList();
        match(PERIOD);
    }

    void query() {
        predicate();
        match(Q_MARK);
    }

    void headPredicate() {
        if (tokenType() == ID) {
            match(ID);
            match(LEFT_PAREN);
            match(ID);
            idList();
            match(RIGHT_PAREN);
        }
        else {
            throwError();
        }
    }

    void predicate() {
        if (tokenType() == ID) {
            match(ID);
            match(LEFT_PAREN);
            parameter();
            parameterList();
            match(RIGHT_PAREN);
        }
        else {
            throwError();
        }
    }

    void predicateList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            predicate();
            predicateList();
        }
        else {
            // lambda
        }
    }

    void parameterList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            parameter();
            parameterList();
        }
        else {
            // lambda
        }
    }

    void stringList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            match(STRING);
            stringList();
        }
        else {
            // Lambda
        }
    }

    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            match(ID);
            idList();
        } else {
        // lambda (epsilon)
        }
    }

    void parameter() {
        if (tokenType() == STRING) {
            match(STRING);
        }
        else if (tokenType() == ID) {
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
        cout << "match: " << t << endl; // For debugging
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
        // NOTE: Needs to eventually throw an error to the try catch block that will wrap main.cpp
        cout << "error" << endl;
    }
};