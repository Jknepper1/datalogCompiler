#pragma once

#include <iostream>
#include <sstream>

using namespace std;


enum TokenType {
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    END,

};

class Token {
    private:
        TokenType type;
        int line;
        string value;
        int total; // For the last line of the output
    // This is a constructor using a member initialization list which is what comes after the :
    // This initializes the private class variables according to what is inserted into Token(type, value, line) when called
    public:
        Token(TokenType type, string value, int line) : type(type), line(line), value(value), total(0) {};

        string toString() const {
            stringstream out;
            // Outputs something like: (STRING,"10",5)
            string token = typeName(type);
            out << "(" << token << "," << "\"" << value << "\"" << "," << line << ")";
            return out.str();
        }


        string typeName(TokenType type) const {
            // return the correct string for each TokenType value
            string token = "";

            switch (type) {
                case COMMA:
                    token = "COMMA";
                    break;
                case PERIOD:
                    token = "PERIOD";
                    break;
                case Q_MARK:
                    token = "Q_MARK";
                    break;
                case LEFT_PAREN:
                    token = "LEFT_PAREN";
                    break;
                case RIGHT_PAREN:
                    token = "RIGHT_PAREN";
                    break;
                case COLON:
                    token = "COLON";
                    break;
                case COLON_DASH:
                    token = "COLON_DASH";
                    break;
                case MULTIPLY:
                    token = "MULTIPLY";
                    break;
                case ADD:
                    token = "ADD";
                    break;
                case SCHEMES:
                    token = "SCHEMES";
                    break;
                case FACTS:
                    token = "FACTS";
                    break;
                case RULES:
                    token = "RULES";
                    break;
                case QUERIES:
                    token = "QUERIES";
                    break;
                case ID:
                    token = "ID";
                    break;
                case STRING:
                    token = "STRING";
                    break;
                case COMMENT:
                    token = "COMMENT";
                    break;
                case UNDEFINED:
                    token = "UNDEFINED";
                    break;
                case END:
                    token = "END";
                    break;
            }

            return token;
        }
};