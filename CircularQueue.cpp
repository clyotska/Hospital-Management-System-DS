#include "CircularQueue.h"
#include <iostream> // Include the standard input/output stream library
#include <stdexcept> // include the standard exception library for exception handling
#include <limits> // include the library for numeric limits to ignore \n left in a buffer after cin 

using namespace std;

// constructor
CircularQueue::CircularQueue(int cap)
    : front(0), rear(0), capacity(cap), size(0), queue(nullptr) // Initialize member variables.
{
    if (capacity <= 0) capacity = 1; // capacity should be at least 1.
    queue = new Patient[capacity];
}

// destructor
CircularQueue::~CircularQueue() {
    delete[] queue;
}

bool CircularQueue::isFull() const {
    return (size == capacity); // the size is equal to the capacity => queue full
}

bool CircularQueue::isEmpty() const {
    return (size == 0);
}

// adding patient to queue.
void CircularQueue::Enqueue(const Patient& p) {
    if (isFull()) {
        cout << "The queue is full\n";
        return;
    }
    // rear points to the index where we will insert
    rear = (front + size) % capacity; // calculate the new rear index
    queue[rear] = p; // add to the rear
    ++size;
}


Patient CircularQueue::Dequeue() {
    if (isEmpty()) {
        cout << "The queue is empty\n"; 
        throw runtime_error("The queue is empty");
    }
    Patient result = queue[front]; // getting patient at the front of the queue
    front = (front + 1) % capacity; // calculating the new front index
    --size;
    return result; // returning the removed patient for the proper work of getConsultationAndDiagnosis func
}


void CircularQueue::Display() const {

    string red = "\033[1;31m"; // ANSI escape code for red color
    string yellow = "\033[1;33m"; // ANSI escape code for yellow color
    string green = "\033[1;32m"; // ANSI escape code for green color
    string reset = "\033[0m"; // ANSI escape code to reset color

    if (isEmpty()) {
        cout << "The queue is empty.\n"; 
        return;
    }
    for (int i = 0; i < size; ++i) { // looping through the patients in the queue.
        int idx = (front + i) % capacity; // calculating the index of the patient in the array
        int s = queue[idx].getSeverity(); // getting the severity of the patient
        string color = green; // default color is green
        if (s >= 9) { // severity is 9 or greater
            color = red; // setting the color to red
        } else if (s >= 5) { // if severity is 5 or greater
            color = yellow; // setting the color to yellow
        }
        cout << (i + 1) << ". " << queue[idx].getName() << " (" << color << s << reset << ")\n"; // printing the patient's info
    }
}


