//
// Created by White on 2018/12/20.
//


#include "HighLight.h"
#include "fstream"

using namespace std;

void HighLight::keyWordsInit()
{
    keyWords.insert("class");
    keyWords.insert("constructor");
    keyWords.insert("function");
    keyWords.insert("method");
    keyWords.insert("field");
    keyWords.insert("static");
    keyWords.insert("int");
    keyWords.insert("char");
    keyWords.insert("boolean");
    keyWords.insert("String");
    keyWords.insert("Array");
    keyWords.insert("void");
    keyWords.insert("true");
    keyWords.insert("Output");
    keyWords.insert("Input");
    keyWords.insert("readLine");
    keyWords.insert("false");
    keyWords.insert("printString");
    keyWords.insert("println");
    //    keyWords.insert("null");
    keyWords.insert("this");
    keyWords.insert("if");
    keyWords.insert("else");
    keyWords.insert("while");
    keyWords.insert("return");
}

void HighLight::point_back(ifstream &src)
{
    src.seekg(-1, ios::cur);
}


//To judge
bool HighLight::is_keyword(string str)
{
    if (keyWords.find(str) != keyWords.end())
        return true;
    else
        return false;
}

bool HighLight::is_operator(char ch)
{
    if (strchr("+-*/%><=", ch))
        return true;
    return false;
}

bool HighLight::is_seprator(char ch)
{
    if (strchr(";:.{},()!&[]@?\\|~", ch))
        return true;
    return false;
}

bool HighLight::is_quotes(char ch)
{
    if (ch == '\'' || ch == '\"')
        return true;
    else
        return false;
}

bool HighLight::is_notes(string str)
{
    if (str == "//" || str == "/*")
        return true;
    else
        return  false;
}

char HighLight::get_ch(ifstream &src)
{
    char res = 1;
    if (src.peek() == EOF)
        return res;
    src.get(res);
    return res;
}

//To HighLight
void HighLight::lex(ifstream &src,ofstream &dst)
{
    char ch;
    string token;
    //读入字符
    ch=get_ch(src);
    //文件结束
    if (ch==1)
        return ;
    //换行符
    if (ch=='\n'||ch=='\r')
        dst<<"<br>";
        //空格
    else if (ch==' '||ch=='\t'){
        dst<<" ";
    }
        //关键字或普通字
    else if (isalpha(ch)){
        while (isalpha(ch)||isdigit(ch)||ch=='_'){
            token.push_back(ch);
            ch=get_ch(src);

        }
        if (is_keyword(token))
            dst<<"<font color=#32CD99>"<<token<<"</font>";
        else
            dst<<token;
        point_back(src);
    }
        //数字
    else if (isdigit(ch)){
        while (isdigit(ch)||ch=='.'){
            token.push_back(ch);
            ch=get_ch(src);

        }
        dst<<"<font color=#DB70DB>"<<token<<"</font>";
        point_back(src);
    }
        //注释
    else if (ch=='/'){
        token.push_back(ch);
        ch=get_ch(src);
        token.push_back(ch);
        //不为注释（为除号），跳转到运算符
        if (is_notes(token)==0){
            point_back(src);
            ch='/';
            token.clear();
            goto a1;
        }
            //为单行注释
        else if (is_notes(token)==1){
            while (ch!='\n'){
                ch=get_ch(src);

                if (ch=='\n')
                    break;
                token.push_back(ch);
            }
            dst<<"<font color=#C0C0C0>"<<token<<"<br></font>";
        }
            //为多行注释
        else {
            int f=0;
            while (1){
                while (ch!='\n'){
                    ch=get_ch(src);

                    if (ch=='/'&&token[token.size()-1]=='*'){
                        f=1;
                        token.push_back(ch);
                        break;
                    }
                    token.push_back(ch);
                }
                if (ch=='\n')
                    dst<<"<font color=#C0C0C0>"<<token<<"<br></font>";
                else
                    dst<<"<font color=#C0C0C0>"<<token<<"</font>";
                if (f)
                    break;
                token.clear();
                ch=get_ch(src);
                token.push_back(ch);
            }
        }
    }
        //运算符
    else if (is_operator(ch)){
        //html中无法直接显示<>，需进行转义
        a1:        if (ch=='<'){
        token.push_back('&');
        token.push_back('l');
        token.push_back('t');
    }
    else if (ch=='>'){
        token.push_back('&');
        token.push_back('g');
        token.push_back('t');
    }
    else
        token.push_back(ch);
        dst<<"<font color=#FF000>"<<token<<"</font>";
    }
        //字符串或字符
    else if (is_quotes(ch)){
        token.push_back(ch);
        int flag=is_quotes(ch);
        //为单引号
        if (flag==1){
            ch=get_ch(src);
            while (ch!='\''){
                token.push_back(ch);
                ch=get_ch(src);

            }
            token.push_back(ch);
            dst<<"<font color=#6B4226>"<<token<<"</font>";
        }
            //为双引号
        else{
            ch=get_ch(src);
            while (ch!='\"'){
                token.push_back(ch);
                ch=get_ch(src);

            }
            token.push_back(ch);
            dst<<"<font color=#6B4226>"<<token<<"</font>";
        }
    }
        //分界符
    else if (is_seprator(ch)){
        if (ch=='&'){
            token.push_back('&');
            token.push_back('a');
            token.push_back('m');
            token.push_back('p');
        }
        else
            token.push_back(ch);
        dst<<"<font color=#97694F>"<<token<<"</font>";
    }
        //头文件
    else if (ch=='#'){
        while (ch!='\n'){
            if (ch=='<'){
                token.push_back('&');
                token.push_back('l');
                token.push_back('t');
            }
            else if (ch=='>'){
                token.push_back('&');
                token.push_back('g');
                token.push_back('t');
            }
            else
                token.push_back(ch);
            ch=get_ch(src);

        }
        dst<<"<font color=#32CD99>"<<token<<"<br></font>";
    }

}


HighLight::HighLight()
{
    keyWordsInit();
    ifstream src;

    src.open("Main.jack",ios::in);
    ofstream dst("out.html",ios::out);

    if (!src.is_open())
    {
        cerr<<"Open Error"<<endl;
        exit(-1);
    }
    while (src.peek()!=EOF)
        lex(src,dst);

    src.close();
    dst.close();
}
