#include "Analyzer.h"
#include "CodeGen.h"
#include "Parser.h"
#include "Scanner.h"
#include "HighLight.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
    //Clear
    ofstream errorOutput;
    ofstream tokenOutput;
    ofstream mid;

    mid.open("MidCode.txt", ios::out | ios::trunc);
    errorOutput.open("error.txt", ios_base::out | ios_base::trunc);
    tokenOutput.open("Token.txt", ios_base::out | ios_base::trunc);

    errorOutput.close();
    tokenOutput.close();
    mid.close();

    //To error the args
    if (argc < 2)
    {
        cerr << "usage: " << argv[0] << " <filename, filename ... >" << endl;
        exit(-1);
    }

    //pass the args
    ifstream fin;
    vector<string> filenames;
    for (int i = 1; i < argc; i++)
    {
        fin.open(argv[i]);
        if (fin.fail())
        {
            cerr << "file '" << argv[i] << "' not exist!";
            exit(-1);
        }
        filenames.push_back(argv[i]);
        fin.close();
    }
    //compile the api
    filenames.push_back("./api/Sys.jack");
    filenames.push_back("./api/Math.jack");
    filenames.push_back("./api/Array.jack");
    filenames.push_back("./api/Memory.jack");
    filenames.push_back("./api/String.jack");
    filenames.push_back("./api/Output.jack");
    filenames.push_back("./api/Input.jack");
    filenames.push_back("./api/IO.jack");

    //parse and print the syntax tree
    Parser parser(filenames);
    parser.parse_program();
    parser.print();

    //classTable and print the table
    if (!hasError())
    {
        Analyzer analyzer(parser.getSyntaxTree());
        analyzer.check();
        // analyzer.getTable().printClassesTable();
    }

    //Target code
    if (!hasError())
    {
        CodeGen cgen;
        cgen.write(parser.getSyntaxTree());
    }

    //To output the highlight code
    //new HighLight();
    Scanner scanner;
    scanner.initKeyWords();
    scanner.initSymbols();
    scanner.printKey();
    //scanner.printSym();

    return 0;
}
