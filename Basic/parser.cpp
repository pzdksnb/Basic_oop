/*
 * File: parser.cpp
 * ----------------
 * Implements the parser.h interface.
 */

#include <iostream>
#include <string>

#include "exp.h"
#include "parser.h"

#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "statement.h"
using namespace std;

/*
 * Implementation notes: parseExp
 * ------------------------------
 * This code just reads an expression and then checks for extra tokens.
 */

Expression *parseExp(TokenScanner & scanner) {
   Expression *exp = readE(scanner);
   if (scanner.hasMoreTokens()) {
      error("parseExp: Found extra token: " + scanner.nextToken());
   }
   return exp;
}

bool Check(string token){
    if(token=="LET" || token=="REM" || token=="PRINT" || token=="END" || token=="IF" ||
       token=="THEN" || token=="GOTO" || token=="RUN" || token=="LIST" || token=="CLEAR" ||
       token=="QUIT" || token=="HELP") return false;

    else return true;
}
Statement *parsestate(TokenScanner & scanner,string line){
    string token;
    Expression *exp=nullptr;
    token=scanner.nextToken();
    if(scanner.getTokenType(token)!=WORD){
        error("SYNTAX ERROR");
    }
    else
        if(token=="REM"){
            return new RemStatement();
        }
        else
            if(token=="LET"){
                if(!scanner.hasMoreTokens()){
                    error(" SYNTAX ERROR");
                }
                exp = parseExp(scanner);
                if(exp->getType()!=COMPOUND){
                    error("SYNTAX ERROR");
                    delete exp;
                }
                if(((CompoundExp *)exp)->getOp()!="="){
                    error(" SYNTAX ERROR");
                    delete exp;
                }
                if((((CompoundExp *)exp)->getLHS())->getType()!=IDENTIFIER){
                    error(" SYNTAX ERROR");
                    delete exp;
                }
                if(!Check(((IdentifierExp *)(((CompoundExp *)exp)->getLHS()))->getName())){
                    error(" SYNTAX ERROR");
                    delete exp;
                }
                return new LetStatement(exp);
            }
            else if(token=="PRINT"){
           exp=parseExp(scanner);
           if(exp->getType()==COMPOUND) {
               if ((((CompoundExp *) exp)->getOp()=="=")) {
                   delete exp;
                   error("SYNTAX ERROR111");
               }
           }
               return new PrintStatement(exp);
         }
    else if(token=="INPUT"){
               if(!scanner.hasMoreTokens()){
                   error("SYNTAX ERROR");
               }
               token=scanner.nextToken();
               if(scanner.getTokenType(token)!=WORD){
                   error("SYNTAX ERROR");
               } else if(scanner.hasMoreTokens()){
                   error("SYNTAX ERROR");
               }
               return new InputStatement(token);

           }
    else if(token=="END"){
               if(scanner.hasMoreTokens()){
                   error("SYNTAX ERROR");
               }
               return new EndStatement();
           }
    else if(token=="GOTO"){
               if (!scanner.hasMoreTokens()) {
                   error("SYNTAX ERROR");
               } else {
                   token=scanner.nextToken();
                   if (scanner.getTokenType(token)!=NUMBER) {
                       error("SYNTAX ERROR");
                   }
                   if (scanner.hasMoreTokens()) {
                       error("SYNTAX ERROR");
                   }
                   int lineNumber;
                   try {
                       lineNumber = stringToInteger(token);
                   } catch (...) {
                       error("SYNTAX ERROR");
                   }
                   return new GoToStatement(lineNumber);
               }
           }
    else if(token == "IF"){
               string Operator;
               Expression *n1;
               Expression *n2;
               GoToStatement *go;
               n1=readE(scanner);
               Operator=scanner.nextToken();

               if(line.find('=')==string::npos){

                  // cout<<"##"<<Operator<<"##"<<endl;
                  if(Operator!="<" && Operator!=">" && Operator!="="){
                       error("SYNTAX ERROR");
                       delete n1;
                   }
                   try {
                       n2=readE(scanner);
                   } catch (...) {
                       error("SYNTAX ERROR");
                       delete n1;
                   }
                   token=scanner.nextToken();
                   if (token!="THEN") {
                       error("SYNTAX ERROR");
                       delete n1;
                       delete n2;
                   }else{
                       if(!scanner.hasMoreTokens()){
                           error("SYNTAX ERROR");
                           delete n1;
                           delete n2;
                       }else{
                           token=scanner.nextToken();
                           if(scanner.getTokenType(token)!=NUMBER){
                               error("SYNTAX ERROR");
                               delete n1;
                               delete n2;
                           }
                           int lineNumber;
                           try{
                               lineNumber=stringToInteger(token);
                           }
                           catch(...){
                               delete n1;
                               delete n2;
                               error("SYNTAX ERROR");
                           }
                           go=new GoToStatement(lineNumber);
                           return new IfStatement(Operator,n1,n2,go);
                       }
                   }
               } else {
                   Operator="=";
                   string tmp;
                   scanner.setInput(line);
                   while(scanner.hasMoreTokens()){
                       token=scanner.nextToken();
                       if(token=="=") break;
                       tmp+=(token+" ");
                   }
                   n2=readE(scanner);
                   token= scanner.nextToken();
                   if (token!="THEN") {
                       error("SYNTAX ERROR");
                       delete n2;
                   }
                   token=scanner.nextToken();
                   if(scanner.getTokenType(token)!=NUMBER){
                       error("SYNTAX ERROR");
                       delete n2;
                   }
                   int lineNumber;
                   try{
                       lineNumber=stringToInteger(token);
                   }
                   catch(...){
                       error("SYNTAX ERROR");
                       delete n2;
                   }
                    go=new GoToStatement(lineNumber);
                    scanner.setInput(tmp);
                    n1=readE(scanner);
                    return new IfStatement(Operator,n1,n2,go);
               }
           }
            error("SYNTAX ERROR");
        }




/*
 * Implementation notes: readE
 * Usage: exp = readE(scanner, prec);
 * ----------------------------------
 * This version of readE uses precedence to resolve the ambiguity in
 * the grammar.  At each recursive level, the parser reads operators and
 * subexpressions until it finds an operator whose precedence is greater
 * than the prevailing one.  When a higher-precedence operator is found,
 * readE calls itself recursively to read in that subexpression as a unit.
 */

Expression *readE(TokenScanner & scanner, int prec) {
   Expression *exp = readT(scanner);
   string token;
   while (true) {
      token = scanner.nextToken();
      int newPrec = precedence(token);
      if (newPrec <= prec) break;
      Expression *rhs = readE(scanner, newPrec);
      exp = new CompoundExp(token, exp, rhs);
   }
   scanner.saveToken(token);
   return exp;
}

/*
 * Implementation notes: readT
 * ---------------------------
 * This function scans a term, which is either an integer, an identifier,
 * or a parenthesized subexpression.
 */

Expression *readT(TokenScanner & scanner) {
   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   if (type == WORD) return new IdentifierExp(token);
   if (type == NUMBER) return new ConstantExp(stringToInteger(token));
   if (token != "(") error("Illegal term in expression");
   Expression *exp = readE(scanner);
   if (scanner.nextToken() != ")") {
      error("Unbalanced parentheses in expression");
   }
   return exp;
}

/*
 * Implementation notes: precedence
 * --------------------------------
 * This function checks the token against each of the defined operators
 * and returns the appropriate precedence value.
 */

int precedence(string token) {
   if (token == "=") return 1;
   if (token == "+" || token == "-") return 2;
   if (token == "*" || token == "/") return 3;
   return 0;
}


