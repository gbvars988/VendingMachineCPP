#include "VendingMachine.h"
#include <iostream>
#include <iomanip>
#include <regex>

VendingMachine::VendingMachine() {
    foodItems = new LinkedList();
}

VendingMachine::~VendingMachine() {
    // TODO - depending on how the 'new' object allocations are made, they might need to be
    // deallocated here or inside their respective functions
    delete foodItems; 
}

// Adding item read from input stock file
void VendingMachine::addItem(std::vector<std::string> stock_tokens) {
    if (stock_tokens.size()) {
        Stock *data = new Stock();
        data->id = stock_tokens[0];
        data->name = stock_tokens[1];
        data->description = stock_tokens[2];
        Price *item_price = new Price();
        std::string price_str = stock_tokens[3];
        size_t dot_pos = price_str.find('.');
        if (dot_pos != std::string::npos) {
            item_price->dollars = std::stoul(price_str.substr(0, dot_pos), nullptr, 0);
            item_price->cents = std::stoul(price_str.substr(dot_pos + 1), nullptr, 0);
        } else {
            item_price->dollars = std::stoul(price_str, nullptr, 0);
            item_price->cents = 0;
        }
        data->price = *item_price;
        data->on_hand = std::stoul(stock_tokens[4], nullptr, 0);
        foodItems->insert(data);

        delete item_price;
    }
}

void VendingMachine::addCoin(std::vector<std::string> coin_tokens) {
    if (coin_tokens.size()) {
        Coin *coin = new Coin();
        if (coin_tokens.at(0) == "1000") {
            coin->denom = TEN_DOLLARS;
        }
        else if (coin_tokens.at(0) == "500") {
            coin->denom = FIVE_DOLLARS;
        }
        else if (coin_tokens.at(0) == "200") {
            coin->denom = TWO_DOLLARS;
        }
        else if (coin_tokens.at(0) == "100") {
            coin->denom = ONE_DOLLAR;
        }
        else if (coin_tokens.at(0) == "50") {
            coin->denom = FIFTY_CENTS;
        }
        else if (coin_tokens.at(0) == "20") {
            coin->denom = TWENTY_CENTS;
        }
        else if (coin_tokens.at(0) == "10") {
            coin->denom = TEN_CENTS;
        }
        else {
            coin->denom = FIVE_CENTS;
        }
        coin->count = std::stoul(coin_tokens[1], nullptr, 0);
        cash_register[coin->denom] = coin->count;

        delete coin;
    }
    else {
        std::cout << "oops";
    }
}

void VendingMachine::addToCashRegister(std::vector<Denomination> cash_received) {
    for (size_t i = 0; i < cash_received.size(); ++i) {
        ++cash_register[cash_received[i]];
    }
}

void VendingMachine::removeFromCashRegister(std::vector<Denomination> cash_out) {
    for (size_t i = 0; i < cash_out.size(); ++i) {
        --cash_register[cash_out[i]];
    }
}

void VendingMachine::displayCash() {
    std::cout << "Coins Summary" << std::endl;
    std::cout << std::setw(13) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
    std::cout << "Denomination" << std::setw(4) << "|" << std::setw(8) << "Count" << std::endl;
    std::cout << std::setw(25) << std::setfill('-') << "" << std::setfill(' ') << std::endl;
    for (const auto &elem : cash_register) {
        std::cout << std::setw(15) << std::left << cash_conversion.at(elem.first) << std::setw(8) << "|" << elem.second << "\n";
    }
    std::cout << std::endl;
}

// Menu Option 1. Display Items
void VendingMachine::displayItems() {
    Node *curr_item = foodItems->get();

    std::cout << "Items Menu" << std::endl;
    std::cout << "----------" << std::endl;
    std::cout << std::left << std::setw(IDLEN) << "ID"
              << std::left << std::setw(NAMELEN + 1) << "|Name"
              << std::left << std::setw(12) << "| Available "
              << std::left << std::setw(6) << " | Price" << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;

    while (curr_item) {
        std::cout << std::left << std::setw(IDLEN) << curr_item->data->id << "|";
        std::cout << std::left << std::setw(NAMELEN) << curr_item->data->name << "|";
        std::cout << std::left << std::setw(12) << curr_item->data->on_hand << "|";
        std::cout << std::left << "$ " << curr_item->data->price.dollars << "."  
                  << curr_item->data->price.cents;
                  if (curr_item->data->price.cents == 0){ std::cout << 0;}
                  std::cout << std::endl;
        if (curr_item->sub_next == nullptr) {curr_item = curr_item->next;}
        
        // Enter this condition if current node contains a 2nd dimension
        // Traverse below node and display sub-items until nullptr reached.
        else {
            Node *sub_item = curr_item->sub_next;
            while (sub_item) {
                        std::cout << std::left << std::setw(IDLEN) << sub_item->data->id << "|";
                        std::cout << std::left << std::setw(NAMELEN) << sub_item->data->name << "|";
                        std::cout << std::left << std::setw(12) << sub_item->data->on_hand << "|";
                        std::cout << std::left << "$ " << sub_item->data->price.dollars << "."  
                        << sub_item->data->price.cents;
                        if (sub_item->data->price.cents == 0){ std::cout << 0;}
                        std::cout << std::endl;
                        sub_item = sub_item->sub_next;
            }
            curr_item = curr_item->next;
        }
        
    }
    std::cout << std::endl;
}

// Menu Option 2. Purchase Item
bool VendingMachine::purchase()
{
    bool successful = true;
    std::string input;
    std::cout << "Purchase Item\n"
              << "-------------\n"
              << std::endl;
    std::cout << "Please enter the id of the item you wish to purchase: ";

    input = Helper::readInput();
    if (input.size() < 1) {
        std::cout << "The task Purchase Item failed to run successfully." << std::endl;
    }

    else {
        Stock *purchase_item = foodItems->getID(input);
    

        // Check if item is available
        if (purchase_item->on_hand > 0) {
            std::cout << "You have selected '"
                    << purchase_item->name << " - "
                    << purchase_item->description << "'. "
                    << "This will cost you $" << "\033[31m" << purchase_item->price.dollars
                    << "." << purchase_item->price.cents << "\033[0m" << "." << std::endl;
            std::cout << "Please hand over the money - type in the value of each note/coin in cents.\n"
                    << "Press enter or ctrl-d on a new line to cancel this purchase:"
                    << std::endl;

            std::vector<Denomination> cash_received;
            unsigned owing_in_cents = (purchase_item->price.dollars * 100) + purchase_item->price.cents;
            unsigned payment = 0;
            int change = 0;
            
            // Payment process, loops until money owing is 0.
            while (owing_in_cents != 0 && !std::cin.eof()) {
                std::cout << "You still need to give us $" << "\033[31m" << owing_in_cents / 100 << "."
                        << owing_in_cents % 100 << "\033[0m" << ":" << std::endl;
                input = Helper::readInput();
                if (Helper::isNumber(input)) {
                    payment = std::stoul(input, nullptr, 0);
                    if (allowed_denoms.find(payment) != allowed_denoms.end()) {
                        if (payment > owing_in_cents) {
                            change = payment - owing_in_cents;
                            owing_in_cents = 0;
                            cash_received.push_back(allowed_denoms[payment]);
                        }
                        else {
                            owing_in_cents = owing_in_cents - payment;
                            cash_received.push_back(allowed_denoms[payment]);
                        }
                    }
                
                    else {
                        std::cout << "Error: $" << payment / 100 << "."
                                << payment % 100 << " is not a valid denomination of money. Please try again.\n"
                                << std::endl;
                    }
                }
                else {
                    std::cout << "Error: \"" << input << "\"" << " is not a valid denomination of money. Enter your money value in cents.\n";
                }
            }

            // Add received cash to cash_register before calculating change
            this->addToCashRegister(cash_received);

            bool change_possible = true;
            int change_counter = change;
            std::vector<Denomination> change_denoms;

            // Calculate change required if any
            if (change > 0) {
                while (change_counter != 0 && change_possible) {
                    if (((change_counter - 1000) >= 0) && (cash_register[TEN_DOLLARS] > 0)) {
                        change_counter = change_counter - 1000;
                        // Decrement denomination count in cash register
                        --cash_register[TEN_DOLLARS];
                        change_denoms.push_back(TEN_DOLLARS);
                    }
                    else if (((change_counter - 500) >= 0) && (cash_register[FIVE_DOLLARS] > 0)) {
                        change_counter = change_counter - 500;
                        --cash_register[FIVE_DOLLARS];
                        change_denoms.push_back(FIVE_DOLLARS);
                    }
                    else if (((change_counter - 200) >= 0) && (cash_register[TWO_DOLLARS] > 0)) {
                        change_counter = change_counter - 200;
                        --cash_register[TWO_DOLLARS];
                        change_denoms.push_back(TWO_DOLLARS);
                    }
                    else if (((change_counter - 100) >= 0) && (cash_register[ONE_DOLLAR] > 0)) {
                        change_counter = change_counter - 100;
                        --cash_register[ONE_DOLLAR];
                        change_denoms.push_back(ONE_DOLLAR);
                    }
                    else if (((change_counter - 50) >= 0) && (cash_register[FIFTY_CENTS] > 0)) {
                        change_counter = change_counter - 50;
                        --cash_register[FIFTY_CENTS];
                        change_denoms.push_back(FIFTY_CENTS);
                    }
                    else if (((change_counter - 20) >= 0) && (cash_register[TWENTY_CENTS] > 0)) {
                        change_counter = change_counter - 20;
                        --cash_register[TWENTY_CENTS];
                        change_denoms.push_back(TWENTY_CENTS);
                    }
                    else if (((change_counter - 10) >= 0) && (cash_register[TEN_CENTS] > 0)) {
                        change_counter = change_counter - 10;
                        --cash_register[TEN_CENTS];
                        change_denoms.push_back(TEN_CENTS);
                    }
                    else if (((change_counter - 5) >= 0) && (cash_register[FIVE_CENTS] > 0)) {
                        change_counter = change_counter - 5;
                        --cash_register[FIVE_CENTS];
                        change_denoms.push_back(FIVE_CENTS);
                    }
                    else {
                        change_possible = false;
                    }
                }

                // Give change if possible
                if (change_possible) {
                    std::cout << "Here is your " << purchase_item->name << " and change of $"
                            << "\033[32m" << change / 100 << "." << change % 100 << "\033[0m" << ": ";
                    for (size_t i = 0; i < change_denoms.size(); ++i) {
                        std::cout << cash_conversion[change_denoms[i]] << " ";
                    }
                    std::cout << "\nPlease come again soon.\n" <<std::endl;
                    // Decrement item count
                    --purchase_item->on_hand;
                }
                else {
                    std::cout << "Change not possible for purchase. Purchase cancelled." << std::endl;
                    // Restore decremented coins from change calculation loop
                    this->addToCashRegister(change_denoms);
                    // Give payment back to customer (remove from cash_register)
                    this->removeFromCashRegister(cash_received);
                }
            }
            // If purchase was successful and no change required
            else {
                
                std::cout << "Here is your " << purchase_item->name << " and change of $" << "\033[32m"
                          << "0" << "\033[0m" << std::endl;

                std::cout << "\nPlease come again soon.\n" <<std::endl;
                // Decrement item count
                --purchase_item->on_hand;
            }
        }
        else {
        
            if (purchase_item->id == "Not found")
            {
                successful = false;
                std::cout << "Error: you did not enter a valid id. Please try again.\n";
            }
            else
            {
                std::cout << "No stock available" << std::endl;
            }
        }
    }
    return successful;
}

// Menu Option 3. Save and Exit
void VendingMachine::saveAndExit()
{
    std::fstream stock_file;
    stock_file.open(this->stock_file, std::ofstream::out | std::ofstream::trunc);
    if (stock_file.is_open()) {
        Node* curr = foodItems->get();
        while (curr) {
            stock_file << curr->data->id << "|";
            stock_file << curr->data->name << "|";
            stock_file << curr->data->description << "|";
            stock_file << curr->data->price.dollars << "." << curr->data->price.cents << "|";
            stock_file << curr->data->on_hand << "\n";

            if (curr->sub_next) {
                Node* sub = curr->sub_next;
                while (sub != nullptr) {
                    stock_file << sub->data->id << "|";
                    stock_file << sub->data->name << "|";
                    stock_file << sub->data->description << "|";
                    stock_file << sub->data->price.dollars << "." << sub->data->price.cents << "|";
                    stock_file << sub->data->on_hand << "\n";
                    sub = sub->sub_next;
                }
            }

            curr = curr->next;
        }
        stock_file.close();
    }
    std::fstream coin_file;
    coin_file.open(this->coin_file, std::ofstream::out | std::ofstream::trunc);
    if (coin_file.is_open()) {
        for (auto elem = cash_register.rbegin(); elem != cash_register.rend(); ++elem) {
            Denomination denom = elem->first;
            unsigned i = 0;
            for (auto& allowed_denom : allowed_denoms) {
                if (allowed_denom.second == denom) {
                    i = allowed_denom.first;
                }
            }
            coin_file << i << "," << elem->second << "\n";
        }

        coin_file.close();
    }

}

// Add sub-item (2D LL)
void VendingMachine::addSubItem() {
    bool keep_running = true;

    std::cout << "Enter ID of category type without suffix (e.g. I0005)" << std::endl;
    std::string input = Helper::readInput();
    Stock* toFind = foodItems->getID(input);
    if (input.empty()) {
        keep_running = false;
        std::cout << "Process cancelled by user" << std::endl;
    }
    else if (toFind->id == "Not found") {
        std::cout << "ID does not exist - try again: " << std::endl;
        this->addSubItem();
    }
    else {
        // Generate new ID with next suffix value
        std::string next_id = foodItems->findHighestSuffix(input);
         // tokens vector to hold user inputs
        std::vector<std::string> item_details;
        input = " ";
        keep_running = true;

        std::cout << "The id of the new stock will be: " << next_id << std::endl;

        // Prompts for user to add new food item
        while (keep_running) {
            item_details.push_back(next_id);
            // Food item name
            std::cout << "Enter the item name:  " << std::endl;
            std::string add_item_name = Helper::readInput();
            if (add_item_name.empty()) {
                keep_running = false;
            } 
            else {
                while (add_item_name.length() > NAMELEN) {
                    std::cout << "Error: line entered was too long. Please try again." << std::endl;
                    std::cout << "Enter the item name:  " << std::endl;
                    add_item_name = Helper::readInput();
                }
                item_details.push_back(add_item_name);
                // Food item description
                std::string add_item_description;
                bool valid_input = false;
                bool item_description_empty = false;
                while (!valid_input && !item_description_empty) {
                    std::cout << "Enter the item description : " << std::endl;
                    add_item_description = Helper::readInput();

                    // Check if input is too long
                    if (add_item_description.length() > DESCLEN) {
                        std::cout << "Error: Description too long. Try again." << std::endl;
                    } else if (add_item_description.empty()) {
                        item_description_empty = true;
                    }
                    else {
                        valid_input = true;
                    }
                }
                item_details.push_back(add_item_description);
                // Food item price
                std::string add_item_price;
                bool valid_price = false;
                while (!valid_price && !item_description_empty) {
                    std::cout << "Enter the price for the item: " << std::endl;
                    add_item_price = Helper::readInput();
                    if (add_item_price.empty()) {
                        valid_price = true;
                        keep_running = false;
                    } 
                    else {
                        std::regex pattern("^\\d+\\.\\d{2}");
                        if (std::regex_match(add_item_price, pattern)) {
                            valid_price = true;
                            item_details.push_back(add_item_price);
                            // Add default stock value for food item
                            item_details.push_back(std::to_string(DEFAULT_STOCK_LEVEL)); 
                            VendingMachine::addItem(item_details);
                            std::cout << "This item \"" << add_item_name << " - " << add_item_description
                                        << "\" has now been added to the menu." << std::endl;
                        } 
                        else {
                            std::cout << "Error: The price is not valid. Try again. (e.g. 1.50)" << std::endl;
                        }
                    }
                }
            }
            if (!keep_running) {
                std::cout << "Cancelling \"add item\" at user's request." << std::endl;
            }
            item_details.clear();
            keep_running = false;
        }
    }
}

// Menu Option 4. Add Item
void VendingMachine::addItemMenu() {
    std::cout << "Would you like to add an item to an existing category? (enter 'yes' or 'no')" << std::endl;
    std::string input = Helper::readInput();

    // If user wants to add an item to an existing category
    if (input == "yes") {this->addSubItem();}

    else if (input == "no") {
        // Obtaining the highest ID to increment new item from
        Stock* highest_id = foodItems->findHighestValue();
        // Extracting the numeric part of the ID after "I"
        int numeric_id = std::stoi(highest_id->id.substr(1));
        numeric_id++;

        // Generate the next ID
        std::string next_id;
        next_id = "I" + std::string(4 - std::to_string(numeric_id).length(), '0') 
                    + std::to_string(numeric_id);

        // tokens vector to hold user inputs
        std::vector<std::string> item_details;
        input = " ";
        bool keep_running = true;

        std::cout << "The id of the new stock will be: " << next_id << std::endl;

        // Prompts for user to add new food item
        while (keep_running) {
            item_details.push_back(next_id);
            // Food item name
            std::cout << "Enter the item name:  " << std::endl;
            std::string add_item_name = Helper::readInput();
            if (add_item_name.empty()) {
                keep_running = false;
            } 
            else {
                while (add_item_name.length() > NAMELEN) {
                    std::cout << "Error: line entered was too long. Please try again." << std::endl;
                    std::cout << "Enter the item name:  " << std::endl;
                    add_item_name = Helper::readInput();
                }
                item_details.push_back(add_item_name);
                // Food item description
                std::string add_item_description;
                bool valid_input = false;
                bool item_description_empty = false;
                while (!valid_input && !item_description_empty) {
                    std::cout << "Enter the item description : " << std::endl;
                    add_item_description = Helper::readInput();

                    // Check if input is too long
                    if (add_item_description.length() > DESCLEN) {
                        std::cout << "Error: Description too long. Try again." << std::endl;
                    } else if (add_item_description.empty()) {
                        item_description_empty = true;
                    }
                    else {
                        valid_input = true;
                    }
                }
                item_details.push_back(add_item_description);
                // Food item price
                std::string add_item_price;
                bool valid_price = false;
                while (!valid_price && !item_description_empty) {
                    std::cout << "Enter the price for the item: " << std::endl;
                    add_item_price = Helper::readInput();
                    if (add_item_price.empty()) {
                        valid_price = true;
                        keep_running = false;
                    } 
                    else {
                        std::regex pattern("^\\d+\\.\\d{2}");
                        if (std::regex_match(add_item_price, pattern)) {
                            valid_price = true;
                            item_details.push_back(add_item_price);
                            // Add default stock value for food item
                            item_details.push_back(std::to_string(DEFAULT_STOCK_LEVEL)); 
                            VendingMachine::addItem(item_details);
                            std::cout << "This item \"" << add_item_name << " - " << add_item_description
                                        << "\" has now been added to the menu." << std::endl;
                        } 
                        else {
                            std::cout << "Error: The price is not valid. Try again. (e.g. 1.50)" << std::endl;
                        }
                    }
                }
            }
            if (!keep_running) {
                std::cout << "Cancelling \"add item\" at user's request." << std::endl;
            }
            item_details.clear();
            keep_running = false;
        }
    }
    else {
        std::cout << "Invalid input" << std::endl;
        this->addItemMenu();
    }
}

// Menu Option 7. Reset Stock
void VendingMachine::resetStock()
{
    Node* curr_item = foodItems->get();
    while (curr_item){
        curr_item->data->on_hand = DEFAULT_STOCK_LEVEL;
        Node* sub_item = curr_item->sub_next;
        if (sub_item != nullptr) {
            while (sub_item) {
                sub_item->data->on_hand = DEFAULT_STOCK_LEVEL;
                sub_item = sub_item->sub_next;
            }
        }
        curr_item = curr_item->next;
    }
}

// Menu Option 8. Reset Coins
void VendingMachine::resetCoins()
{
    for (auto &elem : cash_register) {
        elem.second = DEFAULT_COIN_COUNT;
    }
    std::cout << "All coins have been reset to the defaul level of " << DEFAULT_COIN_COUNT << std::endl;
}

// Menu Option 5. Remove Item
void VendingMachine::removeItem() {
    std::string input;
    bool keep_running = true;
    Stock* item_to_remove;
    while (keep_running) {

        std::cout << "Enter the item id of the item to remove from the menu: ";
        input = Helper::readInput();
        item_to_remove = foodItems->getID(input);

        if (input.empty()) {
            std::cout << "No ID given." << std::endl;
            keep_running = false;
        }
        else if(item_to_remove->id == "Not found") {
            std::cout << "Item " << input << " not found." << std::endl;
        }
        else {
            // Vector to store removed_item details for printing
            std::vector<std::string> removed_details;
            removed_details.push_back(item_to_remove->id);
            removed_details.push_back(item_to_remove->name);
            removed_details.push_back(item_to_remove->description);
            if (foodItems->removeID(input)) {
                std::cout << "'" 
                          << removed_details.at(0) << " - "
                          << removed_details.at(1) << " - "
                          << removed_details.at(2)  
                          << "' has been removed from the system." 
                          << std::endl;
                keep_running = false;
            }
            
        }

    }
    // delete item_to_remove; 
    // FIXME: foodItems->getID() method returns a new stock object. 
    //        delete to avoid memory leak? currently delete item_to_remove results in memory error.

}

void VendingMachine::store(std::string stock_file, std::string coin_file) {
    this->stock_file = stock_file;
    this->coin_file = coin_file;
}

