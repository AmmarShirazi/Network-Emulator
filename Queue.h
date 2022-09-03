#pragma once
#include <iostream>

template <typename T>
struct QueueNode {
    T value;
    QueueNode* next;

    QueueNode() {
        value = {};
        next = nullptr;
    }
};

template <class T>
class Queue {
    QueueNode<T>* front;
    QueueNode<T>* rear;

public:
    Queue() {
        this->front = nullptr;
        this->rear = nullptr;
    }
    bool isEmpty() {
        return !(this->front && this->rear);
    }
    void enqueue(T& data) {
        if (isEmpty()) {
            QueueNode<T>* toInsert = new QueueNode<T>;
            toInsert->value = data;
            this->front = toInsert;
            this->rear = toInsert;
            return;
        }
        QueueNode<T>* toInsert = new QueueNode<T>;
        toInsert->value = data;
        this->rear->next = toInsert;
        this->rear = this->rear->next;
        return;
    }
    void dequeue() {
        if (isEmpty()) {
            return;
        }
        if (this->front == this->rear) {
            QueueNode<T>* toDelete = this->front;
            delete toDelete;
            toDelete = nullptr;
            this->front = nullptr;
            this->rear = nullptr;
            return;
        }
        QueueNode<T>* toDelete = this->front;
        this->front = this->front->next;
        delete toDelete;
        toDelete = nullptr;
        
    }
    void virtualDequeue() {
        if (isEmpty()) {
            return;
        }
        if (this->front == this->rear) {
            //QueueNode<T>* toDelete = this->front;
            //delete toDelete;
            //toDelete = nullptr;
            this->front = nullptr;
            this->rear = nullptr;
            return;
        }
        //QueueNode<T>* toDelete = this->front;
        this->front = this->front->next;
        //delete toDelete;
        //toDelete = nullptr;
    }
    T Front() {
        return front->value;
    }
    void makeNull() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    void print() {
        QueueNode<T>* temp = this->front;

        while (temp) {
            std::cout << temp->value << std::endl;
            temp = temp->next;
        }
    }
    int getQueueSize() {

        QueueNode<T>* temp = this->front;
        int size = 0;
        while (temp) {

            //std::cout << size << std::endl;
            size++;
            temp = temp->next;
        }
        return size;
    }
    ~Queue() {
        makeNull();
    }
};
