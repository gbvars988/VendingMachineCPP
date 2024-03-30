#include <iostream>
#include "Helper.h"
#include <string.h>

using std::string;
using std::cout;
using std::endl;
using std::vector;


Helper::Helper(){}

void Helper::printMenu() {
    cout << "Main Menu:" << endl;
    cout << "\t\t1. Display Items" << endl;
    cout << "\t\t2. Purchase Item" << endl;
    cout << "\t\t3. Save and Exit" << endl;
    cout << "Administrator-Only Menu:" << endl;
    cout << "\t\t4. Add Item" << endl;
    cout << "\t\t5. Remove Item" << endl;
    cout << "\t\t6. Display Coins" << endl;
    cout << "\t\t7. Reset Stock" << endl;
    cout << "\t\t8. Reset Coins" << endl;
    cout << "\t\t9. Abort Program\n" << endl;
    cout << "Select your option (1-9): " << endl;
}

void Helper::splitString(string s, vector<string>& tokens, string delimeter) {
    tokens.clear();
    char* _s = new char[s.length()+1];
    strcpy(_s, s.c_str());

    char * pch;
    pch = strtok (_s, delimeter.c_str());
    while (pch != NULL)
    {
        tokens.push_back(pch);
        pch = strtok (NULL, delimeter.c_str());
    }
    delete[] _s;
}

string Helper::readInput() {
    string input;
    std::getline(std::cin, input);
    std::cout << std::endl;

    return input;
}

bool Helper::isNumber(string s)
{
    string::const_iterator it = s.begin();
    char dot = '.';
    int nb_dots = 0;
    while (it != s.end()) 
    {
        if (*it == dot)
        {
            nb_dots++;
            if (nb_dots>1)
            {
                break;
            }
        }   
        else if (!isdigit(*it))
        {
            break;
        } 

        ++it;
    }
    return !s.empty() && s[0] != dot && it == s.end();
}