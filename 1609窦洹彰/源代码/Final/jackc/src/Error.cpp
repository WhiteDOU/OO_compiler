//
// Created by White on 2018/12/17.
//
#include "Error.h"
#include <iostream>


using namespace std;

int errorNum;


bool hasError() { return errorNum; }

void syntaxError(string currentParserFilename, string expected,
                 Scanner::Token token) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);


  errorNum++;
  cerr << "Error in class " << currentParserFilename << " in line " << token.row
       << ": expect a " << expected << ", but got a " << token.tokenValue << "\n";
  errorOutput << "Error in class " << currentParserFilename << " in line " << token.row
              << ": expect a " << expected << ", but got a " << token.tokenValue << "\n";

  errorOutput.close();
}

// 类名和函数名不一致
void error1(string currentParserFilename) {


  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);

  errorNum++;
  cerr << "Error in file " << currentParserFilename << ".jack: "
       << "classname should be same as filename" << endl;
  errorOutput << "Error in file " << currentParserFilename << ".jack: "
              << "classname should be same as filename" << endl;
  errorOutput.close();
}

// 变量重定义
void error2(string currentClass, int row, string type, string name) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClass << " in line " << row
       << ": redeclaration of '" << type << " " << name << "'" << endl;
  errorOutput << "Error in class " << currentClass << " in line " << row
              << ": redeclaration of '" << type << " " << name << "'" << endl;
  errorOutput.close();
}

// 函数重定义
void error3(string currentClass, int row, string type, string name) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClass << " in line " << row
       << ": redeclaration of '" << type << " " << name << "()" << endl;
  errorOutput << "Error in class " << currentClass << " in line " << row
              << ": redeclaration of '" << type << " " << name << "()" << endl;
  errorOutput.close();
}

// 类型未定义
void error4(string currentClassName, int row, string type) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row << ": '"
       << type << "' not declaraed" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row << ": '"
              << type << "' not declaraed" << endl;
  errorOutput.close();
}

// 变量未定义
void error5(string currentClassName, int row, string varName) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row << ": '"
       << varName << "' does not declared in this scope" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row << ": '"
              << varName << "' does not declared in this scope" << endl;
  errorOutput.close();
}

void error6(string currentClassName, int row, string type) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row << ": "
       << type << " does not an Array type" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row << ": "
              << type << " does not an Array type" << endl;
  errorOutput.close();
}

void error7(string currentClassName, string callerName, int row,
            string functionName) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": class " << callerName << " haven't a member function '"
       << functionName << "()'" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": class " << callerName << " haven't a member function '"
              << functionName << "()'" << endl;
  errorOutput.close();
}

void error8(string currentClassName, int row, string functionName) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": subroutine " << functionName
       << " called as a method from within a function" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": subroutine " << functionName
              << " called as a method from within a function" << endl;
  errorOutput.close();
}

void error9(string currentClassName, string callerName, int row,
            string functionName) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row << ": '"
       << functionName << "' is not a function in class " << callerName << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row << ": '"
              << functionName << "' is not a function in class " << callerName << endl;
  errorOutput.close();
}

// 函数类型错误
void error10(string currentClassName, string callerName, int row,
             string functionName) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row << ": '"
       << functionName << "' is not a method in class " << callerName << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row << ": '"
              << functionName << "' is not a method in class " << callerName << endl;
  errorOutput.close();
}

// 返回值错误
void error11(string currentClassName, string type, int row) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": return-statement with no value, in function returning '" << type
       << "'" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": return-statement with no value, in function returning '" << type
              << "'" << endl;
  errorOutput.close();
}

// 返回值错误
void error12(string currentClassName, int row) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": return-statement with a value, in function returning void" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": return-statement with a value, in function returning void" << endl;

  errorOutput.close();
}

// 返回值错误
void error13(string currentClassName, int row) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": The return type of a constructor must be of the class type"
       << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": The return type of a constructor must be of the class type"
              << endl;

  errorOutput.close();
}

// 参数太少
void error14(string currentClassName, string functionName, int row) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);


  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": too few arguments to function " << functionName << "()" << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": too few arguments to function " << functionName << "()" << endl;
  errorOutput.close();
}

// 参数太多
void error15(string currentClassName, string functionName, int row) {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class " << currentClassName << " in line " << row
       << ": too many arguments to function " << functionName << endl;
  errorOutput << "Error in class " << currentClassName << " in line " << row
              << ": too many arguments to function " << functionName << endl;
  errorOutput.close();
}

void error16() {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error: Main class not exsist" << endl;
  errorOutput << "Error: Main class not exsist" << endl;
  errorOutput.close();
}

void error17() {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class Main: main function does not exsit!" << endl;
  errorOutput << "Error in class Main: main function does not exsit!" << endl;
  errorOutput.close();
}

void error18() {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class Main: the kind of subroutine main must be a function"
       << endl;
  errorOutput << "Error in class Main: the kind of subroutine main must be a function"
              << endl;
  errorOutput.close();
}

void error19() {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class Main: the type of subroutine main must be a void"
       << endl;
  errorOutput << "Error in class Main: the type of subroutine main must be a void"
              << endl;
  errorOutput.close();
}

void error20() {
  ofstream errorOutput;
  errorOutput.open("error.txt",ios::app);
  errorNum++;
  cerr << "Error in class Main: the argument size of subroutine main must be "
          "null"
       << endl;
  errorOutput << "Error in class Main: the argument size of subroutine main must be "
                 "null"
              << endl;
  errorOutput.close();
}
