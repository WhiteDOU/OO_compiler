//
// Created by White on 2018/12/17.
//
#include "Parser.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Parser::Parser(vector<string> &filenames)
{
    this->filenames = filenames;
    syntaxTree = nullptr;
}

Scanner::Token Parser::getToken()
{
    if (tokenBuffer2.size() == 0)
    {
        auto token = scanner.nextToken();
        tokenBuffer1.push_back(token);
        if (tokenBuffer1.size() > 10)
            tokenBuffer1.pop_front();
    } else
    {
        auto token = tokenBuffer2.front();
        tokenBuffer1.push_back(token);
        tokenBuffer2.pop_front();
    }
    ofstream tokenOutput;
    tokenOutput.open("Token.txt", ios::app);
    string outKind = "";
    switch (tokenBuffer1.back().kind)
    {
        case 0:
            outKind = "KEY_WORD";
            break;
        case 1:
            outKind = "ID";
            break;
        case 2:
            outKind = "INT";
            break;
        case 3:
            outKind = "BOOL";
            break;
        case 4:
            outKind = "CHAR";
            break;
        case 5:
            outKind = "STRING";
            break;
        case 6:
            outKind = "SYMBOL";
            break;
        case 7:
            outKind = "FLOAT";
            break;
        case 8:
            outKind = "NONE";
            break;
        case 9:
            outKind = "ERROR";
            break;
        case 10:
            outKind = "END_OF_FILE";
            break;
        default:
            break;
    }
    cout << '<' << outKind << ',' << tokenBuffer1.back().tokenValue
         << ">" << endl;
    tokenOutput << '<' << outKind << ',' << tokenBuffer1.back().tokenValue
                << ">" << endl;
    tokenOutput.close();
    return tokenBuffer1.back();
}

Scanner::Token Parser::ungetToken()
{
    assert(tokenBuffer1.size() > 0);
    auto token = tokenBuffer1.back();
    tokenBuffer2.push_front(token);
    tokenBuffer1.pop_back();
    return tokenBuffer1.back();
}

string Parser::getFullName(string name)
{
    string fullname = currentParserFilename + "." + name;
    return fullname;
}

string Parser::getCallerName(const string &fullName)
{
    auto iter = fullName.cbegin();
    while (iter != fullName.cend())
    {
        if (*iter == '.')
            break;
        ++iter;
    }
    return string(fullName.cbegin(), iter);
}

string Parser::getFunctionName(const string &fullName)
{
    auto iter = fullName.cbegin();
    while (iter != fullName.cend())
    {
        if (*iter == '.')
            break;
        ++iter;
    }
    return string(++iter, fullName.cend());
}

void Parser::parse_program()
{ syntaxTree = parseClassList(); }

Parser::TreeNode *Parser::parseClassList()
{
    TreeNode *t = nullptr;
    TreeNode *p = nullptr;
    for (auto filenameIter = filenames.cbegin(); filenameIter != filenames.cend();
         ++filenameIter)
    {
        scanner.openFile(*filenameIter);
        auto classNameIter = filenameIter->rbegin();
        int begin = 0;
        while (classNameIter != filenameIter->rend())
        {
            if (*classNameIter == '/')
                break;
            begin++;
            ++classNameIter;
        }
        currentParserFilename =
                filenameIter->substr(filenameIter->size() - begin, begin - 5);
        scanner.resetRow();
        TreeNode *q = parseClass();
        if (getToken().kind != Scanner::ENDOFFILE)
            cerr << "Syntax Error in class " << currentParserFilename
                 << ": unexpected token before EOF " << endl;
        if (q != nullptr)
        {
            if (t == nullptr)
            {
                t = p = q;
            } else
            {
                p->next = q;
                p = q;
            }
        }
        scanner.closeFile();
    }
    return t;
}

Parser::TreeNode *Parser::parseClass()
{
    TreeNode *t = new TreeNode;
    Scanner::Token token = getToken();
    t->nodeKind = CLASS_K;
    token = getToken();
    if (token.kind != Scanner::ID)
    {
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    t->child[0] = new TreeNode;
    t->child[0]->token = token;
    if (currentParserFilename != token.tokenValue)
    {
        error1(currentParserFilename);
        return t;
    }
    token = getToken();
    if (token.tokenValue != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[1] = parseClassVarDecList();
    t->child[2] = parseSubroutineDecList();
    token = getToken();
    if (token.tokenValue != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseClassVarDecList()
{
    TreeNode *t = nullptr;
    TreeNode *p = t;
    auto token = getToken();
    while (token.tokenValue == "static" || token.tokenValue == "field")
    {
        ungetToken();
        TreeNode *q = parseClassVarDec();
        if (q != nullptr)
        {
            if (t == nullptr)
                t = p = q;
            else
            {
                p->next = q;
                p = q;
            }
        }
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseClassVarDec()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = CLASS_VAR_DEC_K;
    Scanner::Token token = getToken();
    if (token.tokenValue != "static" && token.tokenValue != "field")
    {
        syntaxError(currentParserFilename, "static or filed", token);
        return t;
    }
    t->child[0] = new TreeNode;
    t->child[0]->token.tokenValue = token.tokenValue;
    t->child[1] = parseType();
    t->child[2] = parseVarNameList();
    token = getToken();
    if (token.tokenValue != ";")
    {
        syntaxError(currentParserFilename, ";", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseVarNameList()
{
    TreeNode *t = new TreeNode;
    Scanner::Token token = getToken();
    if (token.kind != Scanner::ID)
    {
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    t->token = token;
    TreeNode *p = t;
    token = getToken();
    while (token.tokenValue == ",")
    {
        token = getToken();
        if (token.kind != Scanner::ID)
        {
            syntaxError(currentParserFilename, "identifier", token);
            return t;
        }
        TreeNode *q = new TreeNode;
        q->token = token;
        p->next = q;
        p = q;
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseType()
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.kind == Scanner::ID)
    {
        t = new TreeNode;
        t->nodeKind = CLASS_TYPE_K;
        t->token.tokenValue = token.tokenValue;
    } else if (token.tokenValue == "int" || token.tokenValue == "char" ||
               token.tokenValue == "boolean" || token.tokenValue == "void")
    {
        t = new TreeNode;
        t->nodeKind = BASIC_TYPE_K;
        t->token.tokenValue = token.tokenValue;
    } else
    {
        syntaxError(currentParserFilename, "basic type or class type", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseSubroutineDecList()
{
    TreeNode *t = nullptr;
    TreeNode *p = t;
    auto token = getToken();
    while (token.tokenValue == "constructor" || token.tokenValue == "function" ||
           token.tokenValue == "method")
    {
        ungetToken();
        TreeNode *q = parseSubroutinDec();
        if (q != nullptr)
        {
            if (t == nullptr)
                t = p = q;
            else
            {
                p->next = q;
                p = q;
            }
        }
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseSubroutinDec()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = SUBROUTINE_DEC_K;
    Scanner::Token token = getToken();
    if (token.tokenValue == "constructor" || token.tokenValue == "function" ||
        token.tokenValue == "method")
    {
        t->child[0] = new TreeNode;
        t->child[0]->token = token;
    } else
    {
        syntaxError(currentParserFilename, "constructor or function or method",
                    token);
        return t;
    }
    t->child[1] = parseType();
    token = getToken();
    if (token.kind == Scanner::ID)
    {
        t->child[2] = new TreeNode;
        t->child[2]->token = token;
        t->child[2]->token.tokenValue = getFullName(token.tokenValue);
    } else
    {
        syntaxError(currentParserFilename, "identifile", token);
        return t;
    }

    token = getToken();
    if (token.tokenValue != "(")
    {
        syntaxError(currentParserFilename, "(", token);
        return t;
    }
    t->child[3] = parseParams();
    token = getToken();
    if (token.tokenValue != ")")
    {
        syntaxError(currentParserFilename, ")", token);
        return t;
    }
    t->child[4] = parseSubroutineBody();
    return t;
}

Parser::TreeNode *Parser::parseParams()
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.tokenValue != ")")
    {
        ungetToken();
        t = parseParamList();
    } else
        ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseParamList()
{
    TreeNode *t = parseParam();
    TreeNode *p = t;
    Scanner::Token token = getToken();
    while (token.tokenValue == ",")
    {
        TreeNode *q = parseParam();
        p->next = q;
        p = q;
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseParam()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = PARAM_K;
    t->child[0] = parseType();
    Scanner::Token token = getToken();
    if (token.kind == Scanner::ID)
    {
        t->child[1] = new TreeNode;
        t->child[1]->token = token;
    } else
    {
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseSubroutineBody()
{
    haveReturnStatement = false;

    TreeNode *t = new TreeNode;
    t->nodeKind = SUBROUTINE_BODY_K;
    Scanner::Token token = getToken();
    if (token.tokenValue != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[0] = parseVarDecList();
    t->child[1] = parseStatements();

    token = getToken();
    if (token.tokenValue != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    if (haveReturnStatement == false)
    {
        syntaxError(currentParserFilename, "return statement", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseVarDecList()
{
    TreeNode *t = nullptr;
    TreeNode *p = t;

    Loop:
    Scanner::Token token = getToken();
    if (token.tokenValue == "int" || token.tokenValue == "char" ||
        token.tokenValue == "boolean" || token.tokenValue == "string")
    {
        ungetToken();
        TreeNode *q = parseVarDec();
        if (t == nullptr)
            t = p = q;
        else
        {
            p->next = q;
            p = q;
        }
        goto Loop;
    } else if (token.kind == Scanner::ID) // Identifier
    {
        token = getToken();
        if (token.kind == Scanner::ID)
        {
            ungetToken();
            ungetToken();
            TreeNode *q = parseVarDec();
            if (t == nullptr)
                t = p = q;
            else
            {
                p->next = q;
                p = q;
            }
            goto Loop;
        }
        ungetToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseVarDec()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = VAR_DEC_K;
    Scanner::Token token;
    t->child[0] = parseType();
    t->child[1] = parseVarNameList();
    token = getToken();
    if (token.tokenValue != ";")
    {
        syntaxError(currentParserFilename, ";", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseStatements()
{
    TreeNode *t = nullptr;
    TreeNode *p = t;
    Scanner::Token token = getToken();
    while (token.tokenValue == "if" || token.tokenValue == "while" ||
           token.tokenValue == "return" || token.kind == Scanner::ID)
    {
        if (token.tokenValue == "return")
            haveReturnStatement = true;
        if (token.kind == Scanner::ID)
        {
            token = getToken();
            if (token.tokenValue == "=" || token.tokenValue == "[" || token.tokenValue == "(" ||
                token.tokenValue == ".")
            {
                ungetToken();
                ungetToken();
                TreeNode *q = parseStatement();
                if (t == nullptr)
                    t = p = q;
                else
                {
                    p->next = q;
                    p = q;
                }
            } else
            {
                ungetToken();
                break;
            }
        } else
        {
            ungetToken();
            TreeNode *q = parseStatement();
            if (t == nullptr)
                t = p = q;
            else
            {
                p->next = q;
                p = q;
            }
        }
        token = getToken();
    }
    ungetToken();

    return t;
}

Parser::TreeNode *Parser::parseStatement()
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.tokenValue == "if")
    {
        ungetToken();
        t = parseIfStatement();
    } else if (token.tokenValue == "while")
    {
        ungetToken();
        t = parseWhileStatement();
    } else if (token.tokenValue == "return")
    {
        ungetToken();
        t = parseReturnStatement();
    } else if (token.kind == Scanner::ID)
    {
        token = getToken();
        if (token.tokenValue == "=" || token.tokenValue == "[")
        {
            ungetToken();
            ungetToken();
            t = parseAssignStatement();

        } else if (token.tokenValue == "(" || token.tokenValue == ".")
        {
            ungetToken();
            ungetToken();
            t = parseCallStatement();
            token = getToken();
            if (token.tokenValue != ";")
            {
                ungetToken();
                syntaxError(currentParserFilename, ";", token);
                return t;
            }
        } else
        {
            ungetToken();
            ungetToken();
            syntaxError(currentParserFilename, "'=' or '[' or '(' or '.'", token);
            return t;
        }
    } else
    {
        ungetToken();
        syntaxError(currentParserFilename, "identifier", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseAssignStatement()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = ASSIGN_K;
    t->child[0] = parseLeftValue();// Left Value to be assigned
    Scanner::Token token = getToken();
    t->child[1] = parseExpression();
    token = getToken();
    if (token.tokenValue != ";")
    {
        syntaxError(currentParserFilename, ";", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseLeftValue()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = VAR_K;
    Scanner::Token token = getToken();
    t->token = token;
    token = getToken();
    if (token.tokenValue == "[")
    {
        t->nodeKind = ARRAY_K;
        t->child[0] = parseExpression();
        token = getToken();
        if (token.tokenValue != "]")
        {
            syntaxError(currentParserFilename, "]", token);
            return t;
        }
        t->nodeKind = ARRAY_K;
    } else if (token.tokenValue == "=")
    {
        ungetToken();
    }
    return t;
}

Parser::TreeNode *Parser::parseIfStatement()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = IF_STATEMENT_K;
    Scanner::Token token = getToken();
    token = getToken();
    if (token.tokenValue != "(")
    {
        syntaxError(currentParserFilename, "(", token);
        return t;
    }
    t->child[0] = parseExpression();
    token = getToken();
    if (token.tokenValue != ")")
    {
        syntaxError(currentParserFilename, ")", token);
        return t;
    }
    token = getToken();
    if (token.tokenValue != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[1] = parseStatements();
    token = getToken();
    if (token.tokenValue != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    token = getToken();
    if (token.tokenValue == "else")
    {
        token = getToken();
        if (token.tokenValue != "{")
        {
            syntaxError(currentParserFilename, "{", token);
            return t;
        }
        t->child[2] = parseStatements();
        token = getToken();
        if (token.tokenValue != "}")
        {
            syntaxError(currentParserFilename, "}", token);
            return t;
        }
    } else
        ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseWhileStatement()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = WHILE_STATEMENT_K;
    Scanner::Token token = getToken();
    token = getToken();
    if (token.tokenValue != "(")
    {
        syntaxError(currentParserFilename, "(", token);
        return t;
    }
    t->child[0] = parseExpression();
    token = getToken();
    if (token.tokenValue != ")")
    {
        syntaxError(currentParserFilename, ")", token);
        return t;
    }
    token = getToken();
    if (token.tokenValue != "{")
    {
        syntaxError(currentParserFilename, "{", token);
        return t;
    }
    t->child[1] = parseStatements();
    token = getToken();
    if (token.tokenValue != "}")
    {
        syntaxError(currentParserFilename, "}", token);
        return t;
    }
    return t;
}

Parser::TreeNode *Parser::parseReturnStatement()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = RETURN_STATEMENT_K;
    Scanner::Token token = getToken();
    t->token = token;
    token = getToken();
    if (token.tokenValue == ";")
        return t;
    else
    {
        ungetToken();
        t->child[0] = parseExpression();
        token = getToken();
        if (token.tokenValue != ";")
        {
            syntaxError(currentParserFilename, ";", token);
            return t;
        }
    }
    return t;
}

Parser::TreeNode *Parser::parseCallStatement()
{
    TreeNode *t = new TreeNode;
    t->nodeKind = CALL_STATEMENT_K;
    Scanner::Token token = getToken();
    Scanner::Token save = token;
    t->child[0] = new TreeNode;
    token = getToken();
    if (token.tokenValue == "(")//Function
    {
        t->child[0]->next = parseExpressions();
        token = getToken();
        if (token.tokenValue != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    } else if (token.tokenValue == ".")//Method
    {
        token = getToken();
        if (token.kind != Scanner::ID)
        {
            syntaxError(currentParserFilename, "identifier", token);
            return t;
        }
        save.tokenValue = save.tokenValue + "." + token.tokenValue;
        token = getToken();
        if (token.tokenValue != "(")
        {
            syntaxError(currentParserFilename, "(", token);
            return t;
        }
        t->child[0]->next = parseExpressions();
        token = getToken();
        if (token.tokenValue != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    t->token = save;
    return t;
}

Parser::TreeNode *Parser::parseExpressions()
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.tokenValue == ")")
    {
        ungetToken();
        return t;
    } else
    {
        ungetToken();
        t = parseExpressionList();
        return t;
    }
}

Parser::TreeNode *Parser::parseExpressionList()
{
    TreeNode *t = parseExpression();
    TreeNode *p = t;
    Scanner::Token token = getToken();
    while (token.tokenValue == ",")
    {
        TreeNode *q = parseExpression();
        p->next = q;
        p = q;
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseExpression()
{
    TreeNode *t = parseBoolExpression();
    Scanner::Token token = getToken();
    while (token.tokenValue == "&" || token.tokenValue == "|")
    {
        TreeNode *p = new TreeNode();
        p->nodeKind = BOOL_EXPRESSION_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        t->child[1] = parseBoolExpression();
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseBoolExpression()
{
    TreeNode *t = parseAdditiveExpression();
    Scanner::Token token = getToken();
    if (token.tokenValue == "<=" || token.tokenValue == ">=" || token.tokenValue == "==" ||
        token.tokenValue == "<" || token.tokenValue == ">" || token.tokenValue == "!=")
    {
        TreeNode *p = new TreeNode;
        p->nodeKind = COMPARE_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        t->child[1] = parseAdditiveExpression();
    } else
        ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseAdditiveExpression()
{
    TreeNode *t = parseTerm();
    Scanner::Token token = getToken();
    while (token.tokenValue == "+" || token.tokenValue == "-")
    {
        TreeNode *p = new TreeNode;
        p->nodeKind = OPERATION_K;
        p->token = token;
        p->child[0] = t;
        t = p;
        p->child[1] = parseTerm();
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseTerm()
{
    TreeNode *t = parseFactor();
    Scanner::Token token = getToken();
    while (token.tokenValue == "*" || token.tokenValue == "/")
    {
        TreeNode *p = new TreeNode;
        p->nodeKind = OPERATION_K;
        p->token = token;//To rotate the syntax tree
        p->child[0] = t;
        t = p;
        p->child[1] = parseFactor();
        token = getToken();
    }
    ungetToken();
    return t;
}

Parser::TreeNode *Parser::parseFactor()//To induce the factor
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.tokenValue == "-")
    {
        t = new TreeNode;       //Negetive takes up two nodes
        t->nodeKind = NEGATIVE_K;
        t->token = token;
        t->child[0] = parsePositiveFactor();
    } else
    {
        ungetToken();
        t = parsePositiveFactor();
    }
    return t;
}

Parser::TreeNode *Parser::parsePositiveFactor()
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.tokenValue == "~")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = BOOL_EXPRESSION_K;
        t->child[0] = parseNotFactor();
    } else
    {
        ungetToken();
        t = parseNotFactor();
    }
    return t;
}

Parser::TreeNode *Parser::parseNotFactor()
{
    TreeNode *t = nullptr;
    Scanner::Token token = getToken();
    if (token.tokenValue == "(")    //when meet with ( ,Namely, new expression
    {
        t = parseExpression();
        token = getToken();
        if (token.tokenValue != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    } else if (token.kind == Scanner::INT)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = INT_CONST_K;
    } else if (token.kind == Scanner::CHAR)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = CHAR_CONST_K;
    } else if (token.kind == Scanner::STRING)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = STRING_CONST_K;
    } else if (token.tokenValue == "true" || token.tokenValue == "false")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = BOOL_CONST_K;
    } else if (token.tokenValue == "this")  //this keywords
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = THIS_K;
    } else if (token.tokenValue == "null")
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = NULL_K;
    } else if (token.kind == Scanner::ID)
    {
        t = new TreeNode;
        t->token = token;
        t->nodeKind = VAR_K;
        token = getToken();
        if (token.tokenValue == "[")
        {
            TreeNode *p = parseExpression();//array expression
            t->child[0] = p;
            token = getToken();
            if (token.tokenValue != "]")
            {
                syntaxError(currentParserFilename, "]", token);
                return t;
            }
            t->nodeKind = ARRAY_K;
        } else if (token.tokenValue == "(" || token.tokenValue == ".")
        {
            ungetToken();
            ungetToken();
            t = parseCallExpression();//can be call expression
        } else
            ungetToken();
    }
    return t;
}

Parser::TreeNode *Parser::parseCallExpression() //using callStatement's return value
{
    TreeNode *t = new TreeNode;
    t->nodeKind = CALL_EXPRESSION_K;
    Scanner::Token token = getToken();
    Scanner::Token save = token;
    t->child[0] = new TreeNode;
    token = getToken();
    if (token.tokenValue == "(")
    {
        t->child[0]->next = parseExpressions();
        token = getToken();
        if (token.tokenValue != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    } else if (token.tokenValue == ".")
    {
        token = getToken();
        if (token.kind != Scanner::ID)
        {
            syntaxError(currentParserFilename, "identifier", token);
            return t;
        }
        save.tokenValue = save.tokenValue + "." + token.tokenValue;
        token = getToken();
        if (token.tokenValue != "(")
        {
            syntaxError(currentParserFilename, "(", token);
            return t;
        }
        t->child[0]->next = parseExpressions();
        token = getToken();
        if (token.tokenValue != ")")
        {
            syntaxError(currentParserFilename, ")", token);
            return t;
        }
    }
    t->token = save;
    return t;
}

void Parser::print()
{ printSyntaxTree(syntaxTree); }

void Parser::printSyntaxTree(TreeNode *tree)
{
    ofstream f1;
    f1.open("MidCode.txt", ios::app);
    static int indentno = 0;
    indentno += 2;
    while (tree != nullptr)
    {
        for (int i = 0; i < indentno; i++)
        {
            cout << " ";
            f1 << " ";
        }
        switch (tree->nodeKind)
        {
            case CLASS_K:
                cout << "class" << endl;
                f1 << "class" << endl;
                break;
            case CLASS_VAR_DEC_K:
                cout << "class_var_dec" << endl;
                f1 << "class_var_dec" << endl;
                break;
            case SUBROUTINE_DEC_K:
                cout << "subroutine_dec" << endl;
                f1 << "subroutine_dec" << endl;
                break;
            case BASIC_TYPE_K:
                cout << "basic_kind " << tree->token.tokenValue << endl;
                f1 << "basic_kind " << tree->token.tokenValue << endl;
                break;
            case CLASS_TYPE_K:
                cout << "class_kind " << tree->token.tokenValue << endl;
                f1 << "class_kind " << tree->token.tokenValue << endl;
                break;
            case PARAM_K:
                cout << "param" << endl;
                f1 << "param" << endl;
                break;
            case VAR_DEC_K:
                cout << "var_dec" << endl;
                f1 << "var_dec" << endl;
                break;
            case ARRAY_K:
                cout << "array" << endl;
                f1 << "array" << endl;
                break;
            case VAR_K:
                cout << "var" << endl;
                f1 << "var" << endl;
                break;
            case IF_STATEMENT_K:
                cout << "if_statement" << endl;
                f1 << "if_statement" << endl;
                break;
            case WHILE_STATEMENT_K:
                cout << "while_statement" << endl;
                f1 << "while_statement" << endl;
                break;
            case RETURN_STATEMENT_K:
                cout << "return_statement" << endl;
                f1 << "return_statement" << endl;
                break;
            case CALL_STATEMENT_K:
                cout << "call_statement" << endl;
                f1 << "call_statement" << endl;
                break;
            case BOOL_EXPRESSION_K:
                cout << "bool_expression " << tree->token.tokenValue << endl;
                f1 << "bool_expression " << tree->token.tokenValue << endl;
                break;
            case COMPARE_K:
                cout << "compare " << tree->token.tokenValue << endl;
                f1 << "compare " << tree->token.tokenValue << endl;
                break;
            case OPERATION_K:
                cout << "operation " << tree->token.tokenValue << endl;
                f1 << "operation " << tree->token.tokenValue << endl;
                break;
            case BOOL_K:
                cout << "bool" << endl;
                f1 << "bool" << endl;
                break;
            case ASSIGN_K:
                cout << "assign" << endl;
                f1 << "assign" << endl;
                break;
            case SUBROUTINE_BODY_K:
                cout << "subroutine_body" << endl;
                f1 << "subroutine_body" << endl;
                break;
            default:
                /*fallthrough*/;
        }
        f1.close();
        printSyntaxTree(tree->child[0]);
        printSyntaxTree(tree->child[1]);
        printSyntaxTree(tree->child[2]);
        printSyntaxTree(tree->child[3]);
        printSyntaxTree(tree->child[4]);
        tree = tree->next;
    }
    indentno -= 2;
}

Parser::TreeNode *Parser::getSyntaxTree()
{ return syntaxTree; }
