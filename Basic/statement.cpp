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


PrintStatement::PrintStatement(TokenScanner &scanner){
    exp=readE(scanner);
    if(scanner.hasMoreTokens()){
        error("too many tokes or have =");
    }else if(exp->getType()==2){
            if(((CompoundExp*)(exp))->getOp()=="="){
                error("you assign wrong when eval");
            }
        }
    }

PrintStatement::~PrintStatement(){
    delete exp;
}
void PrintStatement::execute(EvalState &state){
    
}

InputStatement::InputStatement(TokenScanner &scanner){
    exp=readT(scanner);
    name=exp->toString();
    if(scanner.hasMoreTokens()){
        error("input wrong");
    }
}

InputStatement::~InputStatement(){
    delete exp;
}

void InputStatement::execute(EvalState &state){
    bool flag;
    TokenScanner Numbervar;
    Numbervar.ignoreWhitespace();
    Numbervar.ignoreComments();
    flag=true;
    while(flag){
        try{
            flag=false;
            string number;
            number=getLine("?");
            Numbervar.setInput(number);
            while(Numbervar.hasMoreTokens()){
                exp=readT(Numbervar);
                if(exp->getType()==1) error("input wrong");
            }
            Numbervar.setInput(name+"="+number);
            exp=readE(Numbervar);
            if(Numbervar.hasMoreTokens() || exp->getType()!=2){
                error("input wrong");
            }
            exp->eval(state);
        }
        catch(...){
              cout<<"INVALIDE NUMBER"<<endl;
              flag=true;
        }
    }
}

EndStatement::EndStatement()=default;

EndStatement::~EndStatement()=default;

void EndStatement::execute(EvalState &state){
    error("end");
}

GoToStatement::GoToStatement(TokenScanner &scanner){
    Expression*tmp;
    tmp=readE(scanner);
    if(scanner.hasMoreTokens()){
        error("line input error");
    }
    if(tmp->getType()!=0){
        error("line input error");
    }
    line=tmp->toString();
    delete tmp;
}

GoToStatement::~GoToStatement(){

}

void GoToStatement::execute(EvalState &state){
    error("to_line"+line);
}

IfStatement::IfStatement(string l){
    for(int i=0;i<l.length();i++){
        if(l[i]=='=' || l[i]=='<' || l[i]=='>'){
            if()
        }
    }

}