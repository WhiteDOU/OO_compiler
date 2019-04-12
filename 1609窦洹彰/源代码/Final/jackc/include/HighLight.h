//
// Created by White on 2018/12/20.
//

#ifndef JACK_HIGHLIGHT_H
#define JACK_HIGHLIGHT_H


#include <iostream>
#include <set>
using namespace std;

class HighLight
{
public:
    bool is_keyword(string str);                //关键字
    bool is_operator(char ch);                  //运算符
    bool is_seprator(char ch);                  //分界符
    bool is_notes(string str);                   //注释
    bool is_quotes(char ch);                     //双引号
    char get_ch(ifstream &src);                 //读入一个字符
    void point_back(ifstream &src);             //回退光标
    void lex(ifstream &src,ofstream &dst);      //lex from function

    set<string> keyWords;
    void keyWordsInit();
    HighLight();


};





#endif //JACK_HIGHLIGHT_H
