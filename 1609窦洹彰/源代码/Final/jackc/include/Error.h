//
// Created by White on 2018/12/15.
//
#ifndef _ERROR_H
#define _ERROR_H

#include "Scanner.h"

/*
 * designed to show the row
 * where occurs the error,
 * the filename
 * which can locate the error
 */

extern int errorNum;


bool hasError();
void syntaxError(string curParserFile, string expected, Scanner::Token token);


/*The types of Error*/
void error1(string curParserFile);//class name isn't the same as function name
void error2(string curClass, int row, string type, string name);//Var re-definition
void error3(string curCLass,int row,string type,string name);//func re-definition
void error4(string curClassName,int row,string type);//type-non-definition
void error5(string curClassName,int row,string varName);//Var-non-definition
void error6(string curClassName,int row,string type);//no-matching for type
//non-definition for function
void error7(string curClassName,string callerName,int row,string functionName);
void error8(string curClassName,int row,string functionName);//not the same with function type
void error9(string curClassName,string callerName,int row,string functionName);//error-function type
void error10(string curClassName, string callerName, int row, string functionName);//error-function type
void error11(string curClassName, string type, int row);//error-return value
void error12(string curClassName, int row);//error-return value
void error13(string currentClassName, int row);//error-return value
void error14(string currentClassName, string functionName, int row);//too few args
void error15(string currentClassName, string functionName, int row);//too many args
void error16();//below are to be extended
void error17();
void error18();
void error19();
void error20();
#endif
