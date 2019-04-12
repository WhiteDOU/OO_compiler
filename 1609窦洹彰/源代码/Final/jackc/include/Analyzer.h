//
// Created by White on 2018/12/17.
//
#ifndef _ANALYZER_H
#define _ANALYZER_H

#include "Error.h"
#include "Parser.h"
#include "SymbolTable.h"
#include <vector>

class Analyzer {
private:
  Parser::TreeNode *tree;
  SymbolTable *symbolTable;
  string currentClassName;    //traverse the tree' name
  string currentFunctionName; // traverse the tree' function name
  void buildClassesTable(Parser::TreeNode *t);
  void checkStatements(Parser::TreeNode *t);
  void checkStatement(Parser::TreeNode *t);
  void checkExpression(Parser::TreeNode *t);
  void checkArguments(Parser::TreeNode *t, vector<string> const& parameter,
                      string const& functionName);
  void checkMain();

public:
  Analyzer(Parser::TreeNode *t);
  SymbolTable getTable();
  void check();
};

#endif
