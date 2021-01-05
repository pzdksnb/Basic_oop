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
using namespace std;

Program::Program() {

}

Program::~Program() {
    this->clear();
}

void Program::clear() {
   map<int,string>mp;
   int nsize=mp.size();
   for(int index=0;index<nsize;index++){
       mp.erase(index);
   }
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
    //map<int,Statement>mp;
    //TODO
    mp[lineNumber]=*stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
   // map<int,Statement*>mp;
   //TODO
    if(mp.count(lineNumber)==1) return mp[lineNumber];
    if(mp.count(lineNumber)==0) return NULL;

}

int Program::getFirstLineNumber() {
    //TODO
    if(mp.empty()){
      return -1;
  }
   else return mp[0];

}

int Program::getNextLineNumber(int lineNumber) {
    if(mp.count(lineNumber+1)==1) return lineNumber+1;
    if(mp.count(lineNumber)==0 || mp.count(lineNumber+1)==0) return -1;
}
