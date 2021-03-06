//
// Created by ariel on 12/14/18.
//

#include "Lexer.h"
#include <sstream>
#include <algorithm>



string Lexer::replaceSubString(string subject, const string &search,
                               const string &replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

void Lexer::addSpaces(string &s) {
    for (string &sign:this->specialSigns) {
        if (s.find(sign) != string::npos) {
            s = replaceSubString(s, sign, " " + sign + " ");
        }
    }
}

list <string> Lexer::lexer(string &s, char delimiter) {
    list <string> tokens;
    string token;
    addSpaces(s);
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        // check if there is no tab or ' ' in the list of tokens
        if (token != "") {
            tokens.push_back(token);
        }
    }

    /*
     * if we split by mistake a string between " "  - we want to build back
     * to a string and insert to the final tokens list.
     * going over all the works between the " " and appending toa  string.
     * after we will look for the quotation marks in the list and will replace
     * all the single string with one string of the quotation.
     */

    string str;
    bool take = false;
    for (auto &s : tokens) {
        if (take) {
            str += s;
        }
        if (s == "\"" && !take) {
            take = true;
            str += s;
        }
    }

    bool found = false;
    list <string> final_tokens;


    string real_sign;
    vector<string> tmp{"=", "!", "<", ">"};

    for (list<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        string cur_sign = *it;
        if (++it != tokens.end()) {
            string next_sign = *(it);

            // checking if the sign is one of those : == != <= >=. if yes will combine the two

            if (next_sign == "=" && count(tmp.begin(), tmp.end(), (cur_sign)) == 1) {
                real_sign = cur_sign + next_sign;
                final_tokens.push_back(real_sign);
            } else {
                --it;
                if (cur_sign == "\"") {
                    if (found) {
                        final_tokens.push_back(str);
                    } else {
                        found = true;
                        final_tokens.push_back(str);
                    }
                } else {
                    if (!found) {
                        final_tokens.push_back(cur_sign);
                    }
                }
            }
        }else{
            if(!found){
                final_tokens.push_back(cur_sign);
            }
            break;
        }
    }
    return final_tokens;
}