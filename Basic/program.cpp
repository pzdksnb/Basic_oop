/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"
using namespace std;

Program::Program() {

}

Program::~Program() {
    this->clear();
}

void Program::clear() {
    for(auto &it:mp){
        delete it.second.exp;
        it.second.exp = nullptr;
    }
    mp.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
   if(mp.count(lineNumber)!=0) mp[lineNumber]=line;
   if(mp.count(lineNumber)==0) mp[lineNumber]=line;
}

void Program::removeSourceLine(int lineNumber) {
   if(mp.count(lineNumber)==0) return;
   if(mp.count(lineNumber)==1) mp.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
   if(mp.count(lineNumber)==1) return "lineNumber";
   if(mp.count(lineNumber)==0) return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if(mp.count(lineNumber)==0) error("SYNTAX ERROR");
    else {
        delete mp[lineNumber].exp;
        mp[lineNumber].exp=stmt;
    }
}

Statement *Program::getParsedStatement(int lineNumber) {
    if(mp.count(lineNumber)==1) return mp[lineNumber].exp;
    if(mp.count(lineNumber)==0) error("SYNTAX ERROR");

}

int Program::getFirstLineNumber() {
    if(mp.empty()){
      return -1;
  }
    auto it = mp.begin();
    return it->first;

}

int Program::getNextLineNumber(int lineNumber) {
    if(mp.count(lineNumber+1)==1) return lineNumber+1;
    if(mp.count(lineNumber)==0 || mp.count(lineNumber+1)==0) return -1;
}
bool Program::Find(int a){
    auto it=mp.begin();
    for(;it!=mp.end();it++){
        if(it->first == a) return true;
    }
    return false;
}
void Program::show(){
    auto it=mp.begin();
    for(;it!=mp.end();it++){
        cout<<it->second.information<<endl;
    }
}

void Program::runprogram(EvalState &state){
    if(mp.empty()) {
        error("error running");
        return;
    }
    auto it = mp.begin();
    for( ;it!=mp.end();it++){
        try{
            (it->second.exp)->execute(state);
        }
        catch(ErrorException &a){
            TokenScanner scanner;
            if(scanner.getTokenType(a.getMessage()) == NUMBER){
                if(!this->Find(stringToInteger(a.getMessage()))){
                    error("GoTo");
                    return;
                }
                else{
                    int n=stringToInteger(a.getMessage());
                    it=mp.find(n);
                    continue;
                }
            }else{
                error(a.getMessage());
            }
        }
    }
}