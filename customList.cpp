#include <iostream>

template < typename T >
class list {

    private:
        
        class node{
            public:
                T data ;
                node* before = nullptr;
                node* after = nullptr;
        };
        node* current_head = nullptr;
        node* current_tail = nullptr;

    public:

    void push_back(T a){
        
        node* current_node = new node; 
        if(current_head == nullptr){ 
            current_head = current_node;
        }
        else{
            current_tail->after = current_node;
        }
        current_node->before = current_tail;
        current_node->data = a;
        
        current_tail = current_node;
        
    } 

    void push_front(T a){

        node* current_node = new node; 
        if(current_tail == nullptr){
            current_tail = current_node;
        }
        else{
            current_head->before = current_node;
        }
        current_node->after = current_head;
        current_node->data = a;
        current_head = current_node;
    }

    void show(){
        node*temp = current_head;
        
        while(temp != nullptr){
            std::cout << temp->data << " ";
            temp = temp->after;
        }
    }

    ~list(){
        node*temp = current_head;
        node*temp2 = nullptr;

            while(temp != nullptr){
                temp2 = temp->after;
                delete temp;
                temp = temp2;
            }       
    }
};

int main(){
    list<int> myList;

    std::cout << sizeof(char);
    
    myList.push_back(10);
    
    myList.push_back(12);

    myList.push_front(14);
    
    myList.show();
    

    return 0;
}