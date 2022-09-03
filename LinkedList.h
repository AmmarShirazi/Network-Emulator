#pragma once
#include <iostream>
#include <iomanip>
template <typename T>
struct ListNode { // simple node struct with a pointer to next node
                    // almost identical to runtime node
    T value;
    ListNode* next;

    ListNode() {
        value = {};
        this->next = nullptr;
    }
};

template <class T>
class LinkedList { // a simple linked list
    ListNode<T>* head;

public:
    LinkedList() {
        this->head = nullptr;
    }
    bool isEmpty() {
        return head == nullptr;
    }
    ListNode<T>*& getHead() { // here check
        return this->head;
    }
    ListNode<T>* getHeadAdd() { // here check
        return this->head;
    }
    void deleteNode(ListNode<T>* toDelete) {

        if (!this->head) {
            return;
        }

        if (toDelete == this->head) {
            this->head = head->next;
            delete toDelete;
            return;
        }

        ListNode<T>* toTraverse = this->head;

        while (toTraverse->next != toDelete && toTraverse) {
            toTraverse = toTraverse->next;
        }

        if (!toTraverse) {
            return;
        }

        toTraverse->next = toTraverse->next->next;
        delete toDelete;
    }
    void remove(T toDelete) {
        if (!this->head) {
            return;
        }

        if (this->head->value == toDelete) {
            ListNode<T>* toTraverse = this->head;
            this->head = this->head->next;
            delete toTraverse;
            return;
        }

        ListNode<T>* toTraverse = this->head;

        while (toTraverse->next->value != toDelete && toTraverse) {
            toTraverse = toTraverse->next;
        }

        if (!toTraverse) {
            return;
        }
        ListNode<T>* deleteptrnode = toTraverse->next;
        toTraverse->next = toTraverse->next->next;
        delete deleteptrnode;
    }
    T& insert(T data) { // inserts node at the end of the list
        if (this->isEmpty()) {
            ListNode<T>* toInsert = new ListNode<T>;
            toInsert->value = data;
            this->head = toInsert;
            return this->head->value;
        }
        ListNode<T>* temp = this->head;
        while (temp->next) {
            temp = temp->next;
        }
        ListNode<T>* toInsert = new ListNode<T>;
        toInsert->value = data;
        temp->next = toInsert;
        return temp->next->value;
    }
    bool isContain(T data) {
        if (!this->head) {
            return false;
        }
        ListNode<T>* temp = this->head;

        while (temp) {
            if (temp->value == data) {
                return true;
            }
            temp = temp->next;
        }

        return false;
    }
    T getAtIndex(int index) {
        ListNode<T>* temp = this->head;
        int count = 0;

        if (index < 0) {
            return {};
        }

        while (temp) {
            if (count == index) {
                return temp->value;
            }
            count++;
            temp = temp->next;
        }
        //std::cout << "fml" << std::endl;

        return {};
    }

    ListNode<T>* getNodeAtIndex(int index) {
        ListNode<T>* temp = this->head;
        int count = 0;

        while (temp) {
            if (count == index) {
                return temp;
            }
            count++;
            temp = temp->next;
        }
        //std::cout << "fml" << std::endl;

        return {};
    }

    T& getAtIndexRef(int index) {
        ListNode<T>* temp = this->head;
        int count = 0;

        while (temp) {
            if (count == index) {
                return temp->value;
            }
            count++;
            temp = temp->next;
        }
        //std::cout << "sight" << std::endl;
        T* dump = new T;
        dump = {};
        return *dump;
    }
    ListNode<T>* getLastNode() {

        if (!this->head) {
            return nullptr;
        }

        ListNode<T>* temp = this->head;

        while (temp->next) {
            temp = temp->next;
        }

        return temp;

    }
    int getListSize() {

        ListNode<T>* temp = this->head;
        int size = 0;
        while (temp) {

            //std::cout << size << std::endl;
            size++;
            temp = temp->next;
        }
        return size;
    }
    void makeNull() {
        if (!this->head) {
            return;
        }
        ListNode<T>* temp = this->head;
        //int i = 0;
        while (temp) {
            ListNode<T>* toDelete = temp;
            temp = temp->next;
            delete toDelete;
        }
        this->head = nullptr;
    }
    void printList() {
        for (int i = 0; i < getListSize(); i++) {
            std::cout << i << "-> ";
            std::cout << std::setw(8) << getAtIndex(i) << std::endl;
        }
    }
    ~LinkedList() {
        this->makeNull();
    }
};