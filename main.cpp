#include <iostream>
#include <cstring>
#include <cstdlib>
#include "errorReporter/errorReporter.h"
#include "scanner/scanner.h"
#include "parser/parser.h"
#include "semantic/semantic.h"

using namespace std;

int main(int argc, char **argv, char **envp)
{
    char    choice;

    if (argc < 2)
        err_exit("Usage: ./GraphCompiler [object file]", "", -EFAULT);

    // init the scanner, open the input file
    if (!InitScanner(argv[1]))
        err_exit("failed to open the source file: %s", argv[1],  -EFAULT);

    
    // test for scanner
    if (argv[2] && !strcmp(argv[2], "test"))
    {
        cout << "Testing Token..." << endl;
        Token *t;
        t = GetToken();
        for (int i = 0; !(t->type == ERRTOKEN || t->type == NONTOKEN); i++)
        {
            cout << "token " << i << ": " << "letme: " << ((t->lexeme) ? t->lexeme : "NULL") << " type: " << t->type << " val: " << t->value << " func ptr: " << (size_t)(t->FuncPtr) << endl;
            t = GetToken();
            // cout<<"haha: "<<t->type<<endl;
        }
        exit(0);
    }

    // parsing the code, translate into python code
    cout << "Start to parse..." << endl;
    parser();

    // close the scanner
    cout << "Done." << endl;
    CloseScanner();
    CloseDrawer();

    // run the python to draw the pic
    while (true)
    {
        cout << "Interpretation done. run it now? [Y/n]" << endl;
        cin.get(choice);
        if (toupper(choice) == 'Y' || choice == '\n')
        {
            cout << "Press enter to exit..." << endl;
            system("python ./drawer.py");
            break;
        }
        else if (toupper(choice) == 'N')
        {
            cout << "Result saved in ./drawer.py" << endl;
            exit(0);
        }
        else
        {
            cout << "Invalid choice" << endl;
        }
    }

}