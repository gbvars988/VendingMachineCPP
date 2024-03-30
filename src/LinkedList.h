#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"
#include <iostream>
#include <string.h>


class LinkedList
{
public:
    LinkedList();
    ~LinkedList();

    unsigned size();
    void add_front(Stock* data);
    void add_back(Stock* data);
    void insert(Stock* data);

    Node* get();
    Stock* getID(std::string id);

    void remove_front();
    void remove_back();
    bool removeID(std::string& ID);
    void clear();
    Stock* getLastValue();

    /**
     * @brief this method finds 
     * the next available ID when
     * adding a base item
    */
    Stock* findHighestValue();

    /**
     * @brief when adding an item option
     * to an existing category, this method
     * finds the next available suffix for ID
    */
    std::string findHighestSuffix(std::string base_id);
    

private:
    // the beginning of the list
    Node* head;
  
    // tracks size of linkedlist - remember to update count after each add/remove.
    unsigned count;
};

#endif  // LINKEDLIST_H


