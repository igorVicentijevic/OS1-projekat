//
// Created by os on 5/12/24.
//

#ifndef LIST_H
#define LIST_H

#include "../h/print.h"
#include "../lib/mem.h"

template <typename T>
class List {

    struct Node{
        T info;
        Node* next;
        Node(T info, Node* next=nullptr):info(info), next(next){}
    };

    Node* head = nullptr;
    Node* tail = nullptr;

    int n = 0;

   /* Node* curr = nullptr;

    void resetCurrent(){
       curr = head;
    }

    void setNext(){
        curr= curr->next;
    }

    T getCurrent(){
        return curr->info;
    }

    bool existNext(){
        return curr->next != nullptr;
    }*/

    void removeNode(Node* prev, Node* curr){
        if(curr==nullptr) return;
        if(!prev){
            //izbrisi sa pocetka

            head=head->next;
        }
        else
        {
            prev->next = curr->next;
        }

        //element koji se prise je poslednji
        if(curr->next==nullptr)//i==n-1
            tail = prev;


        //delete curr;
        n--;
        if(n==0) head = nullptr;

        //delete curr;
        //__mem_free(curr);
    }

public:

    int getNumOfElements() const {return n;}

    void addNodeToBack(T info){

        Node* newNode = new Node(info);

        if(!head){
            head = newNode;
        }else{
            tail->next = newNode;
        }
         tail = newNode;
        n++;
    }
    T* getFrontElem(){
        T* elem = readElemAtPos(0);
        removeNodeFromPos(0);
        return elem;

    }

    T* readElemAtPos(int i){
        if(i<0 || i >= n) return nullptr;

        Node* curr = head;
        for(; i >0; i--,curr=curr->next);

        return &(curr->info);

    }



    void removeNodeFromBack(){
        removeNodeFromPos(n-1);

    }

    void removeGivenElem(T elemToRemove){

        if(elemToRemove == nullptr) return;

       Node* prev = nullptr;
       Node* curr = head;

       for(; curr->info!=elemToRemove; prev = curr,curr= curr->next);

       removeNode(prev,curr);

    }



    void addNodeToPos(int i, T info){
        if(i<0 || i > n) return;

        Node* prev = nullptr;
        Node* curr=head;
        for(; i >0; i--,prev =curr, curr=curr->next);

        Node* newNode = new Node(info);

        newNode->next = curr;

        if(!prev){
            //stavi na pocetak liste
            head=newNode;
        }
        else
        {
            prev->next = newNode;
        }

        if(!curr)
            tail = newNode;
        n++;
    }

    bool isEmpty(){
        return n==0;
    }



    void removeNodeFromPos(int i){
        if(i<0 || i >= n) return;

        Node* prev = nullptr;
        Node* curr=head;
        for(; i >0; i--,prev =curr, curr=curr->next);

        removeNode(prev,curr);

    }



};



#endif //LIST_H
