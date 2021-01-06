/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Stub implementation of BASIC" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();     //删除空格
   scanner.scanNumbers();          //寻找数字
   scanner.setInput(line);         //输入流
//   Expression *exp = parseExp(scanner);
//   int value = exp->eval(state);
//   cout << value << endl;
//   delete exp;
   string token;
   if(scanner.hasMoreTokens()) token=scanner.nextToken();
   else return;
    if(scanner.getTokenType(token)==NUMBER){
        int lineNumber;
        try{
            lineNumber = stringToInteger(token);
        } catch (...) {
            cout<<"SYNTAX ERROR"<<endl;
            return;
        }
        if(!scanner.hasMoreTokens()){
            program.removeSourceLine(lineNumber);
            return;
        }
        try{
            Statement *stmt=parsestate(scanner,line);
            program.addSourceLine(lineNumber,line);
            program.setParsedStatement(lineNumber,stmt);
            return;
        } catch (...) {
            cout<<"SYNTAX ERROR"<<endl;
            return;
        }
    }
   if(scanner.getTokenType(token)==WORD){
       if(token=="RUN"){
           if(scanner.hasMoreTokens()){
               error( "SYNTAX ERROR");
           }else
               try{
                   program.runprogram(state);
               }
               catch(ErrorException &a){
                   if(a.getMessage()=="end") return;
                   if(a.getMessage()=="zero"){
                      cout<<"DIVIDE BY ZERO"<<endl;
                      return;
                   }
                   if(a.getMessage()=="goto"){
                       cout<<"LINE NUMBER ERROR"<<endl;
                   }
                   else {
                       cout<<"VARIABLE NOT DEFINED"<<endl;
                       return;
                   }
               }
       }

       else if(token=="LIST"){
           if(scanner.hasMoreTokens()){
               cout<<"SYNTAX ERROR"<<endl;
               return;
           }
           program.show();
       }
       if(token=="PRINT" ||token=="INPUT" || token=="LET") {
           scanner.setInput(line);
           Statement *st=nullptr;
           try{
               st=parsestate(scanner, line);
           }
           catch (...){
               cout <<"SYNTAX ERROR"<< endl;
               return;
           }
           try{
               st->execute(state);
               delete st;
           }
           catch(ErrorException &ex){
               delete st;
               if (ex.getMessage()=="zero"){
                   cout<<"DIVIDE BY ZERO"<<endl;
                   return;
               }else{
                   cout<<"VARIABLE NOT DEFINED"<<endl;
                   return;
               }
           }
       }else if(token=="CLEAR"){
           if(scanner.hasMoreTokens()){
               error( "SYNTAX ERROR");
               return;
           }
           program.clear();
           state.clear();
           return;
       }
       else if(token=="QUIT"){
           exit(0);
       }
       else if(token=="HELP"){
       }else{
           cout<<"SYNTAX ERROR"<<endl;
           return;
       }
   }

   else{
        cout<<"SYNTAX ERROR"<<endl;
        return;
    }
}




