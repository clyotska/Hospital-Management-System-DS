
#include <iostream>
#include <stdexcept>
#include <limits>
#include "Patient.cpp";
using namespace std;

class CircularQueue {
private:
	int front, rear, capacity, size;
	Patient* queue; ///array to store Patient class objects

public:
	CircularQueue(int cap) { ///constructor
		queue = nullptr;
        capacity = cap; // maximum size of array
        front = 0;
        rear = 0;
        size = 0; // number of patients in a queue
        queue = new Patient[capacity]; // allocate storage
	}
	bool isFull() {
		return (capacity == size);
	}

	bool isEmpty() {
		return (size == 0);
	}

	void Enqueue(Patient p){
		if (this->isFull()) {
			cout << "The queue is full";
			return;
		}
		rear = (front + size) % capacity;// circular behaviour
		queue[rear] = p;
		size++;	
	}

	Patient Dequeue() {
		Patient dequeue;
		if (this->isEmpty()) {
			cout << "The queue is empty";
			throw runtime_error("The queue is empty");
		}
		dequeue = queue[front];
		front = (front + 1) % capacity; //ensuring circular behaviour
		size--;
		return dequeue;

	}

	void Display() {
		if (!(this->isEmpty())) {
			for (int i = 0; i < size; i++) {
				cout << i + 1 << "." << queue[i].getName() <<endl;
			}
		
		}
		else {
			cout<<"The queue is empty.";
		}
	
	}
	~CircularQueue() {
		delete[] queue;
	}

};

