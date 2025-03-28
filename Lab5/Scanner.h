#pragma once

#include <iostream>
#include <sstream>
#include <cctype>
#include <vector>
#include "Token.h"

using namespace std;

class Scanner {
    private:
        string input;
        int line = 1; // Initialize to line 1
        TokenType type;
        string value;
        int total = 0;
        int incompleteStr = 0;
        int lineDiff = 0; // Holds the difference between EOF and string start on incomplete string

    public:
        Scanner(const string& input) : input(input) {}
        vector<Token> tokens;

    void scan() {
        //cout << "Input Length in Scan before while: " << input.length() << endl;
        
        if (input.empty()){ // Handles files that are completely empty
            total++;
            
            Token t = eof();
            // cout << t.toString() << endl;
            tokens.push_back(t);    // Add the current token to vector for Parser
        }

        while (!input.empty()) {

            while(isspace(input[0])) {
                if (input[0] == '\n') {
                    line++;
                }
                input = input.substr(1);
            }

            if (input[0] == '#') {
                skipComment();
                continue;
            }
            
            Token t = scanToken();
            // cout << t.toString() << endl;
            tokens.push_back(t);

            if (incompleteStr == 1){ // Places EOF after an incomplete string
                    total++;
                    line = line + lineDiff; // Places EOF at correct line
            
                    Token t = eof();
                    // cout << t.toString() << endl;
                    tokens.push_back(t);
                }   

            total++;   
        }
    }

    Token eof() {
        return Token(END, "", line);
    }

    void printTotal() {
        cout << "Total Tokens = " << total << endl; // Need to + 1 if eof isn't checked in loop
    }

    Token scanToken() {  

        if (input.empty()){
            return eof();
        }

        // I am having a really hard time handling multiple
        // newlines at the ned of the file in a row 
        // Handles newlines before text
        while (isspace(input[0])) {
            // Clears whitespace
            if (input[0] == '\n') {
                line++;
            }   

            input = input.substr(1); // updates 0 index to clear white space
        }

        char current = input[0];
        // cout << current << endl;

        switch(current) {
            case ',':
                type = COMMA;
                value = ",";
                input = input.substr(1);
                break;
            case '.':
                type = PERIOD;
                value = ".";
                input = input.substr(1);
                break;
            case '?':
                type = Q_MARK;
                value = "?";
                input = input.substr(1);
                break;
            case '(':
                type = LEFT_PAREN;
                value = "(";
                input = input.substr(1);
                break;
            case ')':
                type = RIGHT_PAREN;
                value = ")";
                input = input.substr(1);
                break;
            case ':':
                colonCheck();
                break;
            case '*':
                type = MULTIPLY;
                value = "*";
                input = input.substr(1);
                break;
            case '+':
                type = ADD;
                value = "+";
                input = input.substr(1);
                break;
            case '\'':
            // White space is not skipped inside strings
                checkString();
                break;
            case '#':               // NOTE: Project 2 should not parse comments. might just need to comment out this logic
                //checkComment();
                //break;
            default:
                if (current == '\0') {
                    return Token(END, "", line); // Handles end of string character without going out of bounds
                }
                else {
                    keyCheck();
                }
                break;
        }  
          
        return Token(type, value, line);
    }

    void colonCheck() {
        if (input[1] == '-') {
            type = COLON_DASH;
            value = ":-";
            input = input.substr(2); // Jumps past my while loop condition because it overflows 
        }
        else {
            type = COLON;
            value = ":";
            input = input.substr(1);
        }
    }

    void keyCheck() {
        if (isalpha(input[0])) {
            int i = 0;
            // Check for keywords
            while (!isspace(input[i]) && input[i] != ':') {
                i++;
            }
            // TEST: This will not pass all tests in the project due to colon logic
            if (input.substr(0, i) == "Schemes" || input.substr(0, i) == "Schemes:") { // Colon added to only explicitly allow Keys preceeding colons
                type = SCHEMES;
                value = "Schemes";
                input = input.substr(7);
            }
            else if (input.substr(0, i) == "Facts" || input.substr(0, i) == "Facts:") {
                type = FACTS;
                value = "Facts";
                input = input.substr(5);
            }
            else if (input.substr(0, i) == "Rules" || input.substr(0, i) == "Rules:") {
                type = RULES;
                value = "Rules";
                input = input.substr(5);
            }
            else if (input.substr(0, i) == "Queries" || input.substr(0, i) == "Queries:") {
                type = QUERIES;
                value = "Queries";
                input = input.substr(7);
            }
            // This function builds the string for the ID
            else {
                buildId();
            }
        }
        else {
            type = UNDEFINED;
            value = input[0];
            // cout << "Reached undefined case" << endl;
            input = input.substr(1);
            // cout << "Completed undefined case" << endl;
            
        }
    }

    void buildId() {
        int i = 0;
        while (isalnum(input[i])) {
            i++;
        }
        string id = input.substr(0, i);
        input = input.substr(i);

        type = ID;
        value = id;
    }

    void checkString() {
        size_t i = 1;
        int completeStr = 0; // Exit boolean

        while (!(input[i] == '\'')) { // Increments until string delimeter if string is terminated
        // TEST: This will pass test 82 now EXCEPT the line for the string needs to be the FIRST not LAST line
            if (input[i] == '\n') {
                line++;
                lineDiff++;
            }
            if (i == input.length() - 1){ // Stops at EOF if string is never terminated
                completeStr = 1;
                break;
            }

            i++;
        }

        string s = input.substr(0, i + 1); // + 1 inlcudes string delimeter
        input = input.substr(i + 1);

        if (completeStr == 0) {
            type = STRING;
            value = s;
        }
        else {
            incompleteStr = 1;
            type = UNDEFINED;
            value = s;
            line = line - lineDiff; // Prints incomplete string at line it starts not ends
        }

    }

    void skipComment() {
        int i = 0;

        if (input[1] == '|') {
            // Block comment logic
            while (!(input[i] == '|' && input[i + 1] == '#')) {
                i++;
            }

            string com = input.substr(0, i + 2);
            input = input.substr(i + 2);

            // type = COMMENT;
            // value = com;
        }

        else {
            // Line comment logic looks for a newline character
            while (!(input[i] == '\n' || input[i] == '\0')) {
                i++;
            }
            
            // Establishes the string to be printed by Token
            // Infinite loop came from i + 1 which pushed the index past the end of the string; surpassing the main while loop condition
            string com = input.substr(0, i);
            // Advances the index to the correct position for scanning
            input = input.substr(i);

            // type = COMMENT;
            // value = com;
        }  

    }
    // void checkComment() {
    //     int i = 0;
    
    //     if (input[1] == '|') {
    //         // Block comment: Skips everything until '|#' is found
    //         while (i < input.length() - 1 && !(input[i] == '|' && input[i + 1] == '#')) {
    //             if (input[i] == '\n') line++; // Track newlines
    //             i++;
    //         }
    //         if (i < input.length() - 1) i += 2; // Move past '|#' if found
    //     } else {
    //         // Line comment: Skip everything until newline
    //         while (i < input.length() && input[i] != '\n') {
    //             i++;
    //         }
    //         if (i < input.length()) i++; // Move past newline
    //     }
    
    //     // Update input to remove the comment
    //     input = input.substr(i);
    // }
    

};