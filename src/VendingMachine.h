#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

#include "Coin.h"
#include "LinkedList.h"
#include "Helper.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>

// Strings used for input validation
#define COMMAND_DISPLAY_ITEMS "1"
#define COMMAND_PURCHASE "2"
#define COMMAND_SAVE_EXIT "3"
#define COMMAND_ADD "4"
#define COMMAND_REMOVE "5"
#define COMMAND_DISPLAY_COINS "6"
#define COMMAND_RESET_STOCK "7"
#define COMMAND_RESET_COINS "8"
#define COMMAND_QUIT "9"

/**
 * Class representing a vending machine.
 * Contains a LinkedList data strucutre to
 * store a collection of food item objects.
 */
class VendingMachine
{
public:
    VendingMachine();
    ~VendingMachine();

    /**
     * @brief method to add a food item 
     * to the linkedlist food_items.
     * takes a vector of strings containing tokens
     * related to 1 food item.
    */
    void addItem(std::vector<std::string>);

    /**
     * @brief method to add a denomination
     * and its count to the cash_register
     * @param coin_tokens a vector of string type
     * containing a denomination and count tokens
    */
    void addCoin(std::vector<std::string> coin_tokens);

    /**
     * @brief Method to print menu items 
     * and corresponding details
    */
    void displayItems();

    /**
     * @brief Displays denomination and counts
     * available in cash register
     */
    void displayCash();

    /**
     * @brief Method for purcashing an item
     * from vending machine
     */
    bool purchase();

    void saveAndExit();

    /**
     * @brief Updates Cash Register with cash received from payment
     * @param cash_received a vector of Denomination type
     */
    void addToCashRegister(std::vector<Denomination> cash_received);

    void removeFromCashRegister(std::vector<Denomination> cash_out);

    /**
     * @brief Display the Add Item menu
     * and add a base item
    */
    void addItemMenu();

    /**
     * @brief Add an sub-item to a category
    */
    void addSubItem();
    
    void resetStock();
    void resetCoins();
    /**
     * @brief Remove an item from the menu
    */
    void removeItem();

    void store(std::string stock_file, std::string coin_file);

private:
    LinkedList *foodItems;
    // Path to stock & coin files
    std::string stock_file;
    std::string coin_file;
    // Stores denomination and amount pairs for cash
    std::map<Denomination, unsigned> cash_register;

    // Contains allowed denominations for payment
    std::map<unsigned, Denomination> allowed_denoms = {
        {5, FIVE_CENTS},
        {10, TEN_CENTS},
        {20, TWENTY_CENTS},
        {50, FIFTY_CENTS},
        {100, ONE_DOLLAR},
        {200, TWO_DOLLARS},
        {500, FIVE_DOLLARS},
        {1000, TEN_DOLLARS}};

    // Useful for converting enum Denomination to string representation
    std::map<Denomination, std::string> cash_conversion =
        {
            {TEN_DOLLARS, "$10"},
            {FIVE_DOLLARS, "$5"},
            {TWO_DOLLARS, "$2"},
            {ONE_DOLLAR, "$1"},
            {FIFTY_CENTS, "50c"},
            {TWENTY_CENTS, "20c"},
            {TEN_CENTS, "10c"},
            {FIVE_CENTS, "5c"}};
};

#endif