#include "Shell.h"

void Shell(){
    cout << "Enter user name: ";
    string name;
    getline (cin, name);
    cout << "`````````````````````\n";
    cout << "    OS SIMULATOR\n";
    cout << "`````````````````````\n";
    cout << "[" << name << "@OS_SIMULATOR /]$ ";
    FileSystem fs;
    CommandParser commandparser (&fs);
    string input;
    getline (cin, input);
    while (true)
    {
        if (!commandparser.execute (input))
            break;
        cout << "[" << name<< "@OS_SIMULATOR " << fs.current->name << "]$ ";
        getline (cin, input);
    }
}

