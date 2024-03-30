#ifndef HELPER_H
#define HELPER_H
#include <vector>

class Helper
{

private:
    Helper();

public:
    static void printMenu();

    static void splitString(std::string s, std::vector<std::string>& tokens, std::string delimeter);
    static bool isNumber(std::string s);
    /**
     * @brief Read the user input (a line) from keyboard
     * 
     * @return the user input.
     */
    static std::string readInput();
};

#endif