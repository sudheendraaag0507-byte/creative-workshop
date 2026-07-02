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

template< typename T >
class list;

template< typename T >
class node {
    friend class list<T>;
    private:

        node* next_node = nullptr;// this jus points to the node after it 

        node* previous_node = nullptr;// this jus points to the node jus before it

        T value = 0;// this knows the value that is being stored
    public:
        void reverse(){
            node<T>* temp = this->next_node;
            this->next_node = this->previous_node;
            this->previous_node = temp;
        }
};

template< typename T >
class list{
    private:
        buffer arena;
        node<T>* head_node = nullptr;
        node<T>* end_node = nullptr;
        int no_of_nodes = 0;

    public:

    //adding elements
    void insert(int index , T value){
        node<T>* current_node = head_node;
        int curNodeNo = 0;
        node<T>* current_node_end = end_node;
        int curNodeNoEnd = no_of_nodes - 1;

        node<T>* current_node_update = arena.query<node<T>>();
        current_node_update->value = value;

        while(true){
            if(curNodeNo == index){
                node<T>* temp1 = current_node->previous_node;

                if(temp1 != nullptr){
                    temp1->next_node = current_node_update;
                    current_node_update->previous_node = temp1;
                    current_node_update->next_node = current_node;
                    current_node->previous_node = current_node_update;
                }

                else{
                    head_node = current_node_update;
                    current_node_update->next_node = current_node;
                    current_node->previous_node = current_node_update;
                    no_of_nodes++;
                    return;
                }

                no_of_nodes++;

                return;
            }
            else if(curNodeNoEnd == index){
                node<T>* temp1 = current_node_end->previous_node;

                temp1->next_node = current_node_update;
                current_node_update->previous_node = temp1;
                current_node_update->next_node = current_node_end;
                current_node_end->previous_node = current_node_update;
                
                no_of_nodes++;

                return;
            }
            
            else {
                current_node = current_node->next_node;
                curNodeNo++;
                current_node_end = current_node_end->previous_node;
                curNodeNoEnd--;
            }    
        }

    }

    void push_front(T value){
        if(head_node == nullptr){
            head_node = arena.query<node<T>>();
            end_node = head_node;
            head_node->value = value;
            no_of_nodes++;
        }
        else{

            node<T>* current_node = arena.query<node<T>>();
            current_node->next_node = head_node;
            head_node->previous_node = current_node;
            head_node = current_node;
            current_node->value = value;
            no_of_nodes++;

        }

    }

    void push_back(T value){
        if(head_node == nullptr){
            head_node = arena.query<node<T>>();
            end_node = head_node;
            head_node->value = value;
            no_of_nodes++;
        }
        else{
            node<T>* current_node = arena.query<node<T>>();
            current_node->previous_node = end_node;
            end_node->next_node = current_node;
            end_node = current_node;
            current_node->value = value;
            no_of_nodes++;
        }
    }

    //removing elements
    void remove(T value){
        node<T>* current_node = head_node;
        int curNodeNo = 0;
        node<T>* current_node_end = end_node;
        int curNodeNoEnd = no_of_nodes - 1;

        while(true){
            if(current_node->value == value){
                node<T>* temp1 = current_node->previous_node;
                node<T>* temp2 = current_node->next_node;
                no_of_nodes--;

                if(temp1 != nullptr)
                    temp1->next_node = temp2;

                else{
                    head_node = temp2;
                    temp2->previous_node = nullptr;
                    return;
                }

                if(temp2 != nullptr)
                    temp2->previous_node = temp1;

                else{
                    end_node = temp1;
                    temp1->next_node = nullptr;
                    return;
                }
                return;
            }
            else if(current_node_end->value == value){
                node<T>* temp1 = current_node_end->previous_node;
                node<T>* temp2 = current_node_end->next_node;
                no_of_nodes--;

                if(temp1 != nullptr)
                    temp1->next_node = temp2;

                else{
                    head_node = temp2;
                    temp2->previous_node = nullptr;
                    return;
                }

                if(temp2 != nullptr){
                
                    temp2->previous_node = temp1;
                }

                else{
                    end_node = temp1;
                    temp1->next_node = nullptr;
                    return;
                }
                return;
            }
            else if(curNodeNo >= curNodeNoEnd || curNodeNoEnd == -1){
                return ;
            }
            else {
                current_node = current_node->next_node;
                curNodeNo++;
                current_node_end = current_node_end->previous_node;
                curNodeNoEnd--;
            }    
        }  
    }

    T pop_front(){
        if(head_node == nullptr){
            std::cout << "there is no element present" << "\n";
            return -1;
        }
        else if(no_of_nodes == 1){
            node<T>* temp = head_node;
            head_node = nullptr;
            end_node = nullptr;
            no_of_nodes--;
            return temp->value;
        }
        else{
            node<T>* temp = head_node;
            head_node = temp->next_node;
            no_of_nodes--;
            return temp->value;   
        }
    }

    T pop_back(){
        if(end_node == nullptr){
            std::cout << "there is no element present" << "\n";
            return -1;
        }
        else if(no_of_nodes == 1){
            node<T>* temp = head_node;
            head_node = nullptr;
            end_node = nullptr;
            no_of_nodes--;
            return temp->value;
        }
        else{
            node<T>* temp = end_node;
            end_node = temp->previous_node;
            no_of_nodes--;
            return temp->value;
        }
    }

    void clear(){
        head_node = nullptr;
        end_node = nullptr;
        no_of_nodes = 0;
    }

    //searching 

    int find(T value){

        node<T>* current_node = head_node;
        int curNodeNo = 0;
        node<T>* current_node_end = end_node;
        int curNodeNoEnd = no_of_nodes - 1;

        while(true){
            
            if(current_node->value == value){
                return curNodeNo;
            }
            else if(current_node_end->value == value){
                return curNodeNoEnd;
            }
            else if(curNodeNo >= curNodeNoEnd || curNodeNoEnd == -1){
                return -1;
            }
            else {
                
                current_node = current_node->next_node;
                
                curNodeNo++;
                
                current_node_end = current_node_end->previous_node;
                
                curNodeNoEnd--;
                
            }    
        }
    }

    void reverse(){
        node<T>* current_node = head_node;
        while(current_node != nullptr){
            current_node->reverse();

            current_node = current_node->previous_node;
        
        }
        
        node<T>* temp = end_node;
        end_node = head_node;
        head_node = temp;
    }

    //size and memory availablity

    bool is_empty(){
        if(head_node == nullptr && end_node == nullptr)
            return true ;
        
        else return false;
    }

    int size(){
        return no_of_nodes ;
    }

    T get(int index){
        node<T>* traversing_node;
        if(index < no_of_nodes - index - 1){
            int point = 0;
            traversing_node = head_node;
            while(true){
                if(index == point) return traversing_node->value;
                traversing_node = traversing_node->next_node;
                point++;
            }
        }

        else{
            int point = no_of_nodes - 1;
            traversing_node = end_node;
            while(true){
                if(index == point) return traversing_node->value;
                traversing_node = traversing_node->previous_node;
                point--;
            }
        }
    }

    //+ -> such that it adds two lists 
    //- -> such that it removes the common elements
    //== -> such that it checks whether two lists are same
    
    list<T>& operator + (list<T> &other){
        for(int i = 0 ; i < other.no_of_nodes ; i++){
            this->push_back(other.get(i));
        }
        return *this;
    }

    list<T>& operator += (list<T> &other){
        for(int i = 0 ; i < other.no_of_nodes ; i++){
            this->push_back(other.get(i));
        }
        return *this;
    }

    list<T>& operator - (list<T> &other){
        if(this->no_of_nodes < other.no_of_nodes ){
            return *this;
        }
        for(int i = 0 ; i < other.no_of_nodes ; i++){
            this->remove(other.get(i));
        }
        return *this;
    }

    bool operator == (const list<T> &other) {
        if(this->no_of_nodes != other.no_of_nodes) return false;
        
        node<T>* current_this = this->head_node;
        node<T>* current_other = other.head_node; 
        
        while(current_this != nullptr && current_other != nullptr) {
            if(current_this->value != current_other->value) return false;
            current_this = current_this->next_node;
            current_other = current_other->next_node;
        }
        return true;
    }

    list<T>& operator = (const list<T> &other){

        if(this == &other){
            return *this;
        }

        this->clear();

        for(int i = 0 ; i < other.no_of_nodes ; i++ ){

            this->push_back(const_cast<list<T>&>(other).get(i));
            
        }

        return *this ;

    }

};


int main(){
    list<int> list1 ;
    list<int> list2 ;
    
    list1.push_back(7);
    list1.push_back(3);
    list1.push_back(8);
    list1.push_back(9);
    list2.push_back(8);

    list1.reverse();

    list1.insert(0,11);

    list1 = list2;
    list1.remove(3);
    
    
    std::cout << list1.size() << "\n";
    std::cout << list1.find(8) << "\n";

    list1.remove(11);
    
    std::cout<< list1.pop_front() << "\n";

    std::cout<< list1.pop_back() << "\n";

    list1.clear();

    std::cout << list1.size() << "\n";

    std::cout << list1.is_empty();

    return 0;
}
