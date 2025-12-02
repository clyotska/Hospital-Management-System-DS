#pragma once
#include "Patient.h"

class CircularQueue {
private:
    int front;
    int rear;
    int capacity;
    int size;
    Patient* queue; // dynamic array of Patient

public:
    explicit CircularQueue(int cap);
    ~CircularQueue();

    bool isFull() const;
    bool isEmpty() const;

    void Enqueue(const Patient& p);
    Patient Dequeue();

    void Display() const;
};