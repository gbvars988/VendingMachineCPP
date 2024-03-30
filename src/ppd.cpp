#include <iostream>
#include "Helper.h"
#include <string>
#include "VendingMachine.h"

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
int main(int argc, char **argv) {
    /* validate command line arguments */
    // TODO

    // Tracks if end of file detected
    bool eof = false;
    try {
        // Check if there is not exactly 3 arguments
        if (argc != 3) {
            throw std::invalid_argument("No such file or directory\
            \nCorrect arguments are:\n\t.ppd <stockfile> <coinfile>\
            \nWhere <stockfile> and <coinfile> are two valid files in the expected format.");
        }

        VendingMachine* ppd_VM = new VendingMachine();

        // Reads stock file (1st file)
        std::fstream stock_file;
        stock_file.open(argv[1]);
        std::string stock_line;
        std::vector<std::string> stock_tokens;
        if (stock_file.is_open()) {
            // Get line and ignores newline whitespace
            while (std::getline (stock_file >> std::ws, stock_line)){
                Helper::splitString(stock_line, stock_tokens, "|");
                // Check if contents of file format token size is 5
                if (stock_tokens.size() != 5) {
                    throw std::invalid_argument("Error: Invalid format inside first file");
                }
                ppd_VM->addItem(stock_tokens);
                // Clear vector for next line to be read
                stock_tokens.clear();
            }
            // Close stock file
            stock_file.close();
        }
        else {
            throw std::invalid_argument("Unable to open first file,"
                " either no such file or directory");
        }

        // Reads coins file (2nd file)
        std::fstream coin_file;
        coin_file.open(argv[2]);
        std::string coin_line;
        std::vector<std::string> coin_tokens;
        if (coin_file.is_open()) {
            while(coin_file){
                // Get line and ignores newline whitespace
                std::getline (coin_file >> std::ws, coin_line);
                Helper::splitString(coin_line, coin_tokens, DELIM);
                // Check if contents of file format token size is 2
                if (coin_tokens.size() != 2) {
                    throw std::invalid_argument("Error: Invalid format inside second file");
                } 
                ppd_VM->addCoin(coin_tokens);
                // Clear vector for next line to be read
                coin_tokens.clear();
            }
            coin_file.close();
        }
        else {
            throw std::invalid_argument("Unable to open second file,"
                " either no file or directory");
        }
        ppd_VM->store(argv[1], argv[2]);

        std::string input;
        

        // Main menu logic
        while (input != COMMAND_QUIT && !eof) {
            Helper::printMenu();
            input = Helper::readInput();

            if (std::cin.eof() || input.empty()) {
                eof = true;
            }
            // Menu option 1. Display Items
            else if (input == COMMAND_DISPLAY_ITEMS) {
                ppd_VM->displayItems();
            }
            // Menu option 2. Purchase Item
            else if (input == COMMAND_PURCHASE) {
                while(!ppd_VM->purchase() && !std::cin.eof()){}
            }
            // Menu option 3. Save and Exit
            else if (input == COMMAND_SAVE_EXIT) {
                ppd_VM->saveAndExit();
                input = COMMAND_QUIT;
            }
            // Menu option 4. Add Item
            else if (input == COMMAND_ADD) {
                ppd_VM->addItemMenu();
            }
            // Menu option 5. Remove Item
            else if (input == COMMAND_REMOVE) {
                ppd_VM->removeItem();
            }
            // Menu option 6. Display Coins
            else if (input == COMMAND_DISPLAY_COINS) {
                ppd_VM->displayCash();
            }
            // Menu option 7. Reset Stock
            else if (input == COMMAND_RESET_STOCK) {
                ppd_VM->resetStock();
            }
            // Menu option 8. Reset Coins
            else if (input == COMMAND_RESET_COINS) {
                ppd_VM->resetCoins();
            }
        }
        delete ppd_VM;

        //ppd_VM->DisplayItems();
        //ppd_VM->DisplayCash();

    // Error state is either 0 or 1
    exit(eof);
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        // Terminate program with failure exit status
    }

    return EXIT_SUCCESS;
}
