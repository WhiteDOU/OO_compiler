//
// Created by White on 2018/12/17.
//
#include "Scanner.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>

using namespace std;

Scanner::Scanner() {
  row = 0;
  bufferPos = 0;
  initKeyWords();
  initSymbols();
}

void Scanner::resetRow() { row = 0; }

void Scanner::openFile(const string &filename) {
  string suffix = filename.substr(filename.size() - 5, 5);
  if (suffix != ".jack") {
    cerr << "file '" << filename
         << "' is not a standard java-- file, filename must be ended by '.j' "
            "or '.J'"
         << endl;
    exit(-1);
  }
  fin.open(filename);
  if (fin.fail()) {
    cerr << "file " << filename << " does not exit" << endl;
    exit(-1);
  }
}

void Scanner::closeFile() { fin.close(); }

void Scanner::initKeyWords() {
  keyWords.insert("class");
  keyWords.insert("constructor");
  keyWords.insert("function");
  keyWords.insert("method");
  keyWords.insert("field");
  keyWords.insert("static");
  keyWords.insert("int");
  keyWords.insert("char");
  keyWords.insert("boolean");
  keyWords.insert("void");
  keyWords.insert("true");
  keyWords.insert("false");
  //    keyWords.insert("null");
  keyWords.insert("this");
  keyWords.insert("if");
  keyWords.insert("else");
  keyWords.insert("while");
  keyWords.insert("return");
}

void Scanner::initSymbols() {
  symbols.insert("{");
  symbols.insert("}");
  symbols.insert("(");
  symbols.insert(")");
  symbols.insert("[");
  symbols.insert("]");
  symbols.insert(".");
  symbols.insert(",");
  symbols.insert(";");
  symbols.insert("+");
  symbols.insert("-");
  symbols.insert("*");
  symbols.insert("/");
  symbols.insert("&");
  symbols.insert("|");
  symbols.insert("~");
  symbols.insert("<");
  symbols.insert(">");
  symbols.insert("=");
  symbols.insert(">=");
  symbols.insert("<=");
  symbols.insert("==");
  symbols.insert("!=");
  //    symbols.insert("!");
  //    symbols.insert("&&");
  //    symbols.insert("||");
}

Scanner::TokenType Scanner::searchReserved(const string &s) {
  if (keyWords.find(s) != keyWords.end())
    return KEY_WORD;
  else
    return ID;
}

char Scanner::nextChar() {
  if (bufferPos >= lineBuffer.size()) {
    row++;
    getline(fin, lineBuffer);
    lineBuffer += '\n';
    if (!fin.fail()) {
      bufferPos = 0;
      return lineBuffer[bufferPos++];
    } else
      return EOF;
  } else {
    return lineBuffer[bufferPos++];
  }
}

void Scanner::rollBack() {
  assert(bufferPos > 0);
  bufferPos--;
}

Scanner::Token Scanner::nextToken() {
  Token token;
  // unsigned tokenStringIndex = 0;
  State state = START_STATE;
  while (state != DONE_STATE) {
    char ch = nextChar();
    if (ch == EOF) {
      token.kind = ENDOFFILE;
      break;
    }
    switch (state) {
    case START_STATE: // 开始状态
      if (ch == ' ' || ch == '\t' || ch == '\n')
        ;
      else if (isalpha(ch) || ch == '_') {
        state = ID_STATE; // 进入标识符状态
        token.kind = ID;
        token.tokenValue += ch;
        token.row = row;
      } else if (isdigit(ch)) {
        state = INT_STATE; // 进入整数状态
        token.kind = INT;
        token.tokenValue += ch;
        token.row = row;
      }

      else if (symbols.find({ch}) != symbols.end()) {
        state = SYMBOL_STATE;
        token.kind = SYMBOL;
        token.tokenValue += ch;
        token.row = row;
      } else if (ch == '"') {
        state = STRING_STATE; // 进入字符串状态
        token.kind = STRING;
        token.row = row;
      } else if (ch == '\'') // 进入单字符状态
      {
        state = CHAR_STATE;
        token.kind = CHAR;
        token.row = row;
      } else // 其它非法字符
      {
        state = ERROR_STATE;
        token.kind = ERROR;
        token.tokenValue += ch;
        token.row = row;
      }
      break;
    case INT_STATE: // 整数状态
      if (isdigit(ch)) {
        token.tokenValue += ch;
      } else {
        rollBack();
        state = DONE_STATE;
      }
      break;
    case ID_STATE: // 标识符状态
      if (isalpha(ch) || isdigit(ch) || ch == '_') {
        token.tokenValue += ch;
      } else {
        rollBack();
        state = DONE_STATE;
      }
      break;
    case STRING_STATE: // 字符串状态
      if (ch == '"') {
        state = DONE_STATE;
      } else if (ch == '\\') {
        state = S_STRING_STATE;
        token.tokenValue += ch;
      } else {
        token.tokenValue += ch;
      }
      break;
    case S_STRING_STATE:
      state = STRING_STATE;
      token.tokenValue.pop_back();
      token.tokenValue += ch;
      break;
    case CHAR_STATE: // 字符状态
      if (ch != '\\' && ch != '\'') {
        state = CHAR_STATE_A;
        token.tokenValue += ch;
      } else if (ch == '\\') {
        state = CHAR_STATE_B;
        token.tokenValue += ch;
      } else if (ch == '\'') {
        state = ERROR_STATE;
        token.kind = ERROR;
        token.tokenValue += ch;
        token.tokenValue += ch;
      }
      break;
    case SYMBOL_STATE:
      if (token.tokenValue == "/") {
        if (ch == '*') {
          state = INCOMMENT_STATE;
          token.tokenValue.pop_back();
        } else if (ch == '/') {
          state = START_STATE;
          bufferPos = lineBuffer.length();
          token.tokenValue.pop_back();
        } else {
          rollBack();
          state = DONE_STATE;
        }
      } else if (token.tokenValue == "<") {
        if (ch == '=') {
          token.tokenValue += ch;
          state = DONE_STATE;
        } else {
          rollBack();
          state = DONE_STATE;
        }
      } else if (token.tokenValue == "=") {
        if (ch == '=') {
          token.tokenValue += ch;
          state = DONE_STATE;
        } else {
          rollBack();
          state = DONE_STATE;
        }
      } else if (token.tokenValue == "!") {
        if (ch == '=') {
          token.tokenValue += ch;
          state = DONE_STATE;
        } else {
          rollBack();
          state = DONE_STATE;
        }
      } else if (token.tokenValue == "&") {
        if (ch == '&') {
          token.tokenValue += ch;
          state = DONE_STATE;
        } else {
          rollBack();
          state = DONE_STATE;
        }
      } else if (token.tokenValue == "|") {
        if (ch == '|') {
          token.tokenValue += ch;
          state = DONE_STATE;
        } else {
          rollBack();
          state = DONE_STATE;
        }
      } else {
        rollBack();
        state = DONE_STATE;
      }
      break;
    case CHAR_STATE_A:
      if (ch == '\'') {
        state = DONE_STATE;
      } else {
        state = ERROR_STATE;
        token.kind = ERROR;
        string tmp = "'";
        tmp.append(token.tokenValue);
        tmp += ch;
        token.tokenValue = tmp;
      }
      break;
    case CHAR_STATE_B:
      if (ch == 'a' || ch == 'b' || ch == 'f' || ch == 'n' || ch == 'r' ||
          ch == 't' || ch == 'v' || ch == '\\' || ch == '?' || ch == '\'' ||
          ch == '"') {
        state = CHAR_STATE_C;
        token.tokenValue += ch;
      } else {
        state = ERROR_STATE;
        token.kind = ERROR;
        string tmp = "'\\";
        tmp += ch;
        token.tokenValue = tmp;
      }
      break;
    case CHAR_STATE_C:
      if (ch == '\'') {
        state = DONE_STATE;
      } else {
        state = ERROR_STATE;
        token.kind = ERROR;
        string tmp = "'";
        tmp.append(token.tokenValue);
        tmp += ch;
        token.tokenValue = tmp;
      }
      break;
    case ERROR_STATE: // 错误状态
      if (ch == ' ' || ch == '\n' || ch == '\t')
        state = DONE_STATE;
      else
        token.tokenValue += ch;
      break;
    case INCOMMENT_STATE: // 注释状态
      if (ch == '*')
        state = P_INCOMMENT_STATE;
      break;
    case P_INCOMMENT_STATE: // 接近退出注释状态
      if (ch == '/')
        state = START_STATE;
      else
        state = INCOMMENT_STATE;
      break;
    default:
        /*fallthrough*/;
    }
    if (state == DONE_STATE && token.kind == ID)
      token.kind = searchReserved(token.tokenValue);
  }
  return token;
}

void Scanner::printKey()
{
    set<string>::iterator iter;
    cout<<"*******KEY**********"<<endl;
    int i=0;
    for (iter=keyWords.begin();iter!=keyWords.end();iter++)
    {
        cout<<"<"<<*iter<<","<<i<<">"<<endl;
        i++;
    }
    cout<<"\n\n";
    cout<<"*******SYMBOL"<<endl;

    set<string>::iterator iterator1;
    i=0;
    for (iterator1=symbols.begin();iterator1!=symbols.end();iterator1++)
    {
        cout<<"<"<<*iterator1<<","<<i<<">"<<endl;
    }


}
