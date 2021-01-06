/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}
RemStatement::RemStatement()=default;
RemStatement::~RemStatement()=default;
void RemStatement::execute(EvalState &state){

}


PrintStatement::PrintStatement(Expression *exp):exp(exp){

}

PrintStatement::~PrintStatement(){
    delete exp;
}
void PrintStatement::execute(EvalState &state){
    cout<<exp->eval(state)<<endl;
}

InputStatement::InputStatement(string &n):name(n){
}

InputStatement::~InputStatement() {
}

void InputStatement::execute(EvalState &state){
    cout<<"?";
    TokenScanner Numbervar;
    Numbervar.ignoreWhitespace();
    Numbervar.ignoreComments();
    int value;
    while(true){
            string number;
            number=getLine();
            Numbervar.setInput(number);
           if(!Numbervar.hasMoreTokens()){
               cout<<"INVALID NUMBER"<<endl<<"?";
               continue;
            }
           if(Numbervar.nextToken()!="-"){
               if(Numbervar.getTokenType(Numbervar.nextToken())!=NUMBER){
                   cout<<"INVALID NUMBER"<<endl<<"?";
                   continue;
               }
               if(Numbervar.hasMoreTokens()){
                   cout<<"INVALID NUMBER"<<endl<<"?";
                   continue;
               }
               try{
                   value=stringToInteger(Numbervar.nextToken());
               }
               catch(...){
                   cout<<"INVALIDE NUMBER"<<endl<<"?";
                   continue;
               }
           }
            //exp->eval(state);
           else{
              if(Numbervar.getTokenType(Numbervar.nextToken())!=NUMBER){
                  cout<<"INVALIDE NUMBER"<<endl<<"?";
                  continue;
              }
              if(Numbervar.hasMoreTokens()){
                  cout<<"INVALIDE NUMBER"<<endl<<"?";
                  continue;
              }
              try{
                  value=-stringToInteger(Numbervar.nextToken());
              }
              catch(...){
                  cout<<"INVALIDE NUMBER"<<endl<<"?";
                  continue;
              }
          }
          state.setValue(name,value); //set in the table
          break;
    }

}

EndStatement::EndStatement()=default;

EndStatement::~EndStatement()=default;

void EndStatement::execute(EvalState &state){
    error("end");
}

GoToStatement::GoToStatement(int lin):line_number(lin){

}

GoToStatement::~GoToStatement()=default;

void GoToStatement::execute(EvalState &state){
    error(integerToString(line_number));
}

IfStatement::IfStatement(string &op,Expression *n1,Expression *n2,GoToStatement *gt):Operator(op),n1(n1),n2(n2),go(gt){
  }
IfStatement::~IfStatement(){
    delete n1;
    delete n2;
    delete go;
}
void IfStatement::execute(EvalState &state){
    bool flag;
    if(Operator=="<"){
        if(n1->eval(state)<n2->eval(state)) flag=true;
        else flag=false;
    }
    if(Operator==">"){
        if(n1->eval(state)>n2->eval(state)) flag=true;
        else flag=false;
    }
    if(Operator=="="){
        if(n1->eval(state)==n2->eval(state)) flag=true;
        else flag=false;
    }
    if(!flag) return;
    else go->execute(state);
}


LetStatement::LetStatement(Expression *exp):exp(exp) {}
LetStatement::~LetStatement(){
    delete exp;
}
void LetStatement::execute(EvalState &state) {
    exp->eval(state);
}
