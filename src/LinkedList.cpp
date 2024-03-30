#include "LinkedList.h"

LinkedList::LinkedList() {
    this->head = nullptr;
    this->count = 0;
}

LinkedList::~LinkedList() {
    // will call ~Node() deconstructor for each node.
    this->clear();
}

unsigned LinkedList::size() {
    return this->count;
}

void LinkedList::add_front(Stock *data) {
    Node *newNode = new Node();
    newNode->data = data;
    newNode->next = this->head;
    this->head = newNode;
    ++this->count;
}

void LinkedList::insert(Stock* data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;
    newNode->sub_next = nullptr;
    // When ID length is 5 (i.e. I0001), this is a base item to be inserted 
    // to the 1st dimension of LinkedList
    if (data->id.size() == 5) {
        if (!this->head) {
            this->head = newNode;
        }
        else if (data->name.compare(this->head->data->name) < 0) {
            newNode->next = this->head;
            this->head = newNode;
        }
        else {
            Node* curr_node = this->head;
            Node* prev_node = nullptr;
            while (curr_node && data->name.compare(curr_node->data->name) >= 0) {
                prev_node = curr_node;
                curr_node = curr_node->next;
            }
            prev_node->next = newNode;
            newNode->next = curr_node;
        }
        ++this->count;
    }
    // When ID length exceeds 5, find base item and traverse down 2nd dimension
    // of linkedlist
    else {
        std::string base_id = data->id.substr(0, 5);
        Node* curr_node = this->head;
        while (curr_node->data->id != base_id) {
            curr_node = curr_node->next;
        }
        if (curr_node->sub_next == nullptr) {
            curr_node->sub_next = newNode;
            newNode->sub_next = nullptr;
            ++this->count;
        }
        else {
            while (curr_node->sub_next) {
                curr_node = curr_node->sub_next;
            }
            curr_node->sub_next = newNode;
            newNode->sub_next = nullptr;
            ++this->count;
        }
    }
}

void LinkedList::add_back(Stock *data) {
    Node *newNode = new Node();
    newNode->data = data;
    newNode->next = nullptr;
    Node *pointer = this->head;

    // If list empty, make newNode head and increment count
    if (this->head == nullptr) {
        this->head = newNode;
        ++this->count;
    }
    // Else, find last node and point next to newNode, increment count
    else {
        bool found = false;
        while (!found) {
            if (pointer->next == nullptr) {
                pointer->next = newNode;
                ++this->count;
                found = true;
            }
            pointer = pointer->next;
        }
    }
}

Node *LinkedList::get() {
    return head;
}

Stock *LinkedList::getID(std::string id) {
    Node *curr_node = this->head;
    bool found = false;
    Stock *toReturn = nullptr;
    

    while (!found && curr_node != nullptr)
    {   
        // If baseID (i.e. I0001) matches, then found.
        if (curr_node->data->id == id)
        {
            found = true;
            toReturn = curr_node->data;
        }

        // Otherwise, find the baseID, then search for suffix 
        // (i.e. I0005, then I0005-2)
        else if (curr_node->data->id == id.substr(0,5) && curr_node->sub_next != nullptr) {
            Node *sub_node = curr_node->sub_next;
            while (!found && sub_node != nullptr) {
                if (sub_node->data->id == id) {
                    found = true;
                    toReturn = sub_node->data;
                }
                else {
                    sub_node = sub_node->sub_next;
                }
            }
            curr_node = curr_node->next;
        }
        else
        {
            curr_node = curr_node->next;
        }
    }

    
    if (!found){
        toReturn = new Stock();
        toReturn->id = "Not found";
    }
    return toReturn;
}

void LinkedList::remove_front() {
    if (this->head != nullptr) {
        Node* sub_node = this->head->sub_next;
        while (sub_node) {
            Node* toDelete = sub_node;
            sub_node = sub_node->sub_next;
            delete toDelete;
            --this->count;
        }
        Node* toDelete = this->head;
        this->head = this->head->next;

        --this->count;
        delete toDelete;
    }
    else {
        throw std::runtime_error("Nothing to remove");
    }
}

void LinkedList::remove_back() {
    // TODO
}

bool LinkedList::removeID(std::string &ID) {
    bool removed = false;
    Node* curr_node = this->head;
    Node* prev_node = nullptr;
    // Item to be removed is part of 1st dimension of LinkedList
    if (ID.size() == 5) {
        if (head != nullptr) {
            while (curr_node->data->id != ID) {
                prev_node = curr_node;
                curr_node = curr_node->next;
            }
            if (prev_node == nullptr) {
                if (curr_node->next != nullptr) {
                    this->head = curr_node->next;
                    delete curr_node;
                    removed = true;
                    --this->count;
                }
                else {
                    this->head = nullptr;
                    delete curr_node;
                    removed = true;
                    --this->count;
                }
            }
            else {
                prev_node->next = curr_node->next;
                delete curr_node;
                removed = true;
                --this->count;
            }
        }
    }
    // Item to be removed belongs to 2nd dimension of LinkedList
    else {
        std::string base_id = ID.substr(0,5);
        while (curr_node->data->id != base_id) {
            curr_node = curr_node->next;
        }
        while (curr_node->data->id != ID) {
            prev_node = curr_node;
            curr_node = curr_node->sub_next;
        }
        if (curr_node->sub_next == nullptr) {
            delete curr_node;
            prev_node->sub_next = nullptr;
            removed = true;
            --this->count;
        }
        else {
            prev_node->sub_next = curr_node->sub_next;
            delete curr_node;
            removed = true;
            --this->count;
        }
    }
    return removed;

}

void LinkedList::clear() {
    //remember that the Stock object within node needs to be deleted from memory before deleting the node
    while (this->head != nullptr) {
        this->remove_front();
    }

}

Stock* LinkedList::getLastValue() {
    if (head == nullptr) {
        throw std::runtime_error("Linked list is empty.");
    }

    Node* current = head;

    // Traverse to last node
    while (current->next != nullptr) {
        current = current->next;
    }

    return current->data;
}

std::string LinkedList::findHighestSuffix(std::string base_id) {
    
    if (this->head == nullptr) {
        throw std::runtime_error("Linked list is empty.");
    }
    Node* curr_node = this->head;
    Node* sub_node = nullptr;
    int new_id_suffix = 0; 
    //Find the base node first
    while (curr_node->data->id != base_id) {
        curr_node = curr_node->next;
    }
    if (curr_node->data->id == base_id) {
        sub_node = curr_node->sub_next;
        // Find highest suffix in existing sub-nodes
        while (sub_node != nullptr) {
            int curr_id_suffix = std::stoi(sub_node->data->id.substr(6,1));
            if (curr_id_suffix > new_id_suffix) {
                new_id_suffix = curr_id_suffix;
            }
            sub_node = sub_node->sub_next;
        }
    }

    // Increment this value for use in new ID
    ++new_id_suffix;
    std::string id = "";
    id = base_id + "-" + std::to_string(new_id_suffix);

    return id;

}

Stock* LinkedList::findHighestValue() {
    if (this->head == nullptr) {
        throw std::runtime_error("Linked list is empty.");
    }

    Stock* highestStock = nullptr;
    std::string highestString = "I0000";

    Node* currentNode = this->head;
    //Traverse LL to find highest string
    while (currentNode != nullptr) {
        std::string currentString = currentNode->data->id;
        if (currentString > highestString) {
            highestString = currentString;
            highestStock = currentNode->data;
        }
        currentNode = currentNode->next;
    }
    return highestStock;
}