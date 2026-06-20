#include <iostream>

class buffer;

class slots {
    friend class buffer;
private: 
    int totalstorage = 1024 * 1024 * 16;
    
    // creating 16Mb of space by default
    char* memPointer = new char[totalstorage];
    
    // to keep track of the end
    char* currentEnd = memPointer;
    
    // to keep track of the storage available
    int remainingStorage = totalstorage;

public:
    ~slots() {
        delete[] memPointer;
    } 
};

class buffer {
private:
    int numberOfSlots;
    slots* currentSlot = nullptr;
    slots* slot;

public:
    buffer();  
    buffer(int totalSlots);
    void changeSlot(int size);

    template<typename T>
    // this is to allocate from the memory when requested 
    T* query() {
        if (sizeof(T) <= currentSlot->remainingStorage) {
            currentSlot->remainingStorage -= sizeof(T);
    
            T* sendQuery = reinterpret_cast<T*>(currentSlot->currentEnd);

            // here am changing the way the compiler interprets this region of memory
            T* temp = sendQuery + 1;

            // here am incrementing the pointer such that it points to unallocated region of data
            currentSlot->currentEnd = reinterpret_cast<char*>(temp);

            // here again changing how the compiler interprets that region of memory in order to restore functionality
            return sendQuery;
        }
        else {
            changeSlot(sizeof(T));

            currentSlot->remainingStorage -= sizeof(T);
    
            T* sendQuery = reinterpret_cast<T*>(currentSlot->currentEnd);

            // here am changing the way the compiler interprets this region of memory
            T* temp = sendQuery + 1;

            // here am incrementing the pointer such that it points to unallocated region of data
            currentSlot->currentEnd = reinterpret_cast<char*>(temp);

            // here again changing how the compiler interprets that region of memory in order to restore functionality
            return sendQuery;
        }
    }

    void resizeLogic() {
        // creating a whole new resized array of slots
        slots* temp = new slots[++numberOfSlots];

        for (int i = 0 ; i < numberOfSlots - 1 ; i++) {
            // deleting the memory chunks assigned to the temporary slots 
            delete[] (temp + i)->memPointer;

            // registering the old pointer to the memory chunk on the new pointers
            (temp + i)->memPointer = (slot + i)->memPointer;
            (temp + i)->currentEnd = (slot + i)->currentEnd;
            (temp + i)->remainingStorage = (slot + i)->remainingStorage;

            // to safely unpoint the old mempointer to destroy the old memory chunks
            slot[i].memPointer = nullptr;
        }
        delete[] slot;
        slot = temp;
        currentSlot = slot + (numberOfSlots - 1);
    }

    ~buffer() {
        delete[] slot;
    }
};

buffer::buffer() {
    numberOfSlots = 10;
    slot = new slots[numberOfSlots];
    currentSlot = slot;
}

buffer::buffer(int totalSlots) {
    numberOfSlots = totalSlots;
    slot = new slots[numberOfSlots];
    currentSlot = slot;
}

void buffer::changeSlot(int size) {
    for (int i = 0 ; i < numberOfSlots ; i++) {
        if ((slot + i)->remainingStorage >= size) {
            // this loop ensures there is no memory space is wasted in the previous chunks
            currentSlot = slot + i;
            return;
        }
    }
    resizeLogic();
}

buffer arena;

template <typename T>
class list {
private:
    class node {
    public:
        T data;
        node* before = nullptr;
        node* after = nullptr;
    };
    node* current_head = nullptr;
    node* current_tail = nullptr;

public:
    void push_back(T a) {
        node* current_node = arena.query<node>(); 
        if (current_head == nullptr) { 
            current_head = current_node;
        }
        else {
            current_tail->after = current_node;
        }
        current_node->before = current_tail;
        current_node->data = a;
        
        current_tail = current_node;
    } 

    void push_front(T a) {
        node* current_node = arena.query<node>(); 
        if (current_tail == nullptr) {
            current_tail = current_node;
        }
        else {
            current_head->before = current_node;
        }
        current_node->after = current_head;
        current_node->data = a;
        current_head = current_node;
    }

    void show() {
        node* temp = current_head;
        
        while (temp != nullptr) {
            std::cout << temp->data << " ";
            temp = temp->after;
        }
    }
};

int main() {
    list<int> myList;
    
    myList.push_back(10);
    myList.push_back(12);
    myList.push_front(14);
    
    myList.show();
    return 0;
}