#pragma once

#include "Patient.h"
#include <string>

class Data
{
private:
    Patient patient;
    std::string date;
    std::string visitCause;
    std::string doctor;
    std::string result;

public:
    Data(const Patient &p, const std::string &d, const std::string &vc,
         const std::string &doc, const std::string &res);
    
    Data();

    std::string getDate() const;
    std::string getVisitCause() const;
    std::string getDoctor() const;
    std::string getResult() const;
    const Patient &getPatient() const;

    // Я хз какие есть методы у пациента, вызовите их тут для доп инфы
    std::string getFullInfo() const;
};

// узел линкед листа
class HistoryNode
{
private:
    Data data;
    HistoryNode *next;

public:
    HistoryNode(const Data &d);

    void setNext(HistoryNode *n);
    HistoryNode *getNext() const;
    const Data &getData() const;
};

// сам линкед лист
class LinkedList
{
private:
    HistoryNode *head;
    int visitCount;

public:
    LinkedList();

    // prohibiting copy/moving to avoid double deletions
    LinkedList(const LinkedList &) = delete;
    LinkedList &operator=(const LinkedList &) = delete;
    LinkedList(LinkedList &&) = delete;
    LinkedList &operator=(LinkedList &&) = delete;

    // destructor to free memory
    ~LinkedList();

    void addVisit(const Data &d);

    int getVisitCount() const;
    void printHistory() const;

    std::string getVisitInfo(int index) const;
};

class VisitHistory
{
private:
    LinkedList visitList;

public:
    void addVisit(const Data &d);
    void printHistory() const;
    int getVisitCount() const;
};
