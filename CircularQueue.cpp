#include "CircularQueue.h"
#include <iostream>
#include <stdexcept>
#include <limits>

using namespace std;

CircularQueue::CircularQueue(int cap)
    : front(0), rear(0), capacity(cap), size(0), queue(nullptr)
{
    if (capacity <= 0) capacity = 1;
    queue = new Patient[capacity];
}

CircularQueue::~CircularQueue() {
    delete[] queue;
}

bool CircularQueue::isFull() const {
    return (size == capacity);
}

bool CircularQueue::isEmpty() const {
    return (size == 0);
}

void CircularQueue::Enqueue(const Patient& p) {
    if (isFull()) {
        cout << "The queue is full\n";
        return;
    }
    // rear points to the index where we will insert
    rear = (front + size) % capacity;
    queue[rear] = p;
    ++size;
}

Patient CircularQueue::Dequeue() {
    if (isEmpty()) {
        cout << "The queue is empty\n";
        throw runtime_error("The queue is empty");
    }
    Patient result = queue[front];
    front = (front + 1) % capacity;
    --size;
    return result;
}

void CircularQueue::Display() const {

    string red = "\033[1;31m";
    string yellow = "\033[1;33m";
    string green = "\033[1;32m";
    string reset = "\033[0m";

    if (isEmpty()) {
        cout << "The queue is empty.\n";
        return;
    }
    for (int i = 0; i < size; ++i) {
        int idx = (front + i) % capacity;
        int s = queue[idx].getSeverity();
        string color = green;
        if (s >= 9) {
            color = red;
        } else if (s >= 5) {
            color = yellow;
        }
        cout << (i + 1) << ". " << queue[idx].getName() << " (" << color << s << reset << ")\n";
    }
}

