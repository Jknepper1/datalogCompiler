#include <vector>
#include "Token.h"

using namespace std;

class Parser {
    private:
        vector<Token> tokens;

    public:
        Parser(const vector<Token>& tokens) : tokens(tokens) { }


    void scheme() {
        if (tokenType() == ID){    
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

    void idList() {
        if (tokenType() == COMMA) {
            match(COMMA);
            match(ID);
            idList();
        } else {
        // lambda (epsilon)
        }
  }

    /********************************
     * SUPPORTING FUNCTIONS         *
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