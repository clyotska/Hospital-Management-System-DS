#pragma once // preventing the header from being included multiple times

#include "Patient.h"
#include <string>

class Data // defining a class to hold all data related to a single visit
{
private:
    Patient patient; // holding a copy of the patient object at the time of the visit
    std::string date;
    std::string visitCause;
    std::string doctor;
    std::string result;

public:
    // constructor for initializing a data object with all details
    Data(const Patient &p, const std::string &d, const std::string &vc,
         const std::string &doc, const std::string &res);
    
    Data(); // default constructor

    std::string getDate() const;
    std::string getVisitCause() const;
    std::string getDoctor() const;
    std::string getResult() const;
    const Patient &getPatient() const; // returning a const reference to avoid copying the patient object

    
    std::string getFullInfo() const;
};


class HistoryNode // defining a node for the linked list
{
private:
    Data data;
    HistoryNode *next; // pointing to the next node in the list

public:
    HistoryNode(const Data &d);

    void setNext(HistoryNode *n);
    HistoryNode *getNext() const;
    const Data &getData() const; // returning a const reference to the data object
};


class LinkedList // defining a singly linked list for storing history nodes
{
private:
    HistoryNode *head; // pointing to the start of the list
    int visitCount;

public:
    LinkedList();

    // prohibiting copying and moving to manage memory manually and avoid issues
    // like double-freeing memory, as the default copy would only create a shallow copy
    LinkedList(const LinkedList &) = delete; // deleting the copy constructor
    LinkedList &operator=(const LinkedList &) = delete; // deleting the copy assignment operator
    LinkedList(LinkedList &&) = delete; // deleting the move constructor
    LinkedList &operator=(LinkedList &&) = delete; // deleting the move assignment operator

    // destructor for freeing the memory allocated for the nodes
    ~LinkedList();

    void addVisit(const Data &d);

    int getVisitCount() const;
    void printHistory() const;

    std::string getVisitInfo(int index) const;
};

// class for managing the collection of visit history records
class VisitHistory
{
private:
    LinkedList visitList; // using a linked list to store the history

public:
    void addVisit(const Data &d);
    void printHistory() const;
    int getVisitCount() const;
};
