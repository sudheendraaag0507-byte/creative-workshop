#include <iostream>

class node {
public:
    long long int value;
    node* pointer = NULL;
};

class linkedList {
private:
    node* headNode = new node;
    node* currentNode = headNode;

public:
    void expand(long long int value) {
        currentNode->pointer = new node;
        currentNode = currentNode->pointer;
        currentNode->value = value;
    }

    void view() {
        node* temp = headNode->pointer;
        while (temp != NULL) {
            std::cout << temp->value << "=>";
            temp = temp->pointer;
        }
        std::cout << "NULL" << "\n";
    }
};

int main() {
    linkedList storage;
    
    storage.expand(3);
    storage.expand(7);
    storage.expand(110);
    storage.expand(45);
    storage.expand(90);
    storage.expand(30);
    storage.expand(87);
    
    storage.view();
}