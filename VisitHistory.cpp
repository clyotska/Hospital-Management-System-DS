#include "VisitHistory.h"
#include <iostream>

// ~~~~~~~~~~~~~~~~ Data ~~~~~~~~~~~~~~~~~
Data::Data(const Patient& p, const std::string& d, const std::string& vc,
           const std::string& doc, const std::string& res)
    : patient(p), date(d), visitCause(vc), doctor(doc), result(res) {}
Data::Data() : patient(Patient()), date("N/A"), visitCause("N/A"), doctor("N/A"), result("N/A") {}

std::string Data::getDate() const { return date; }
std::string Data::getVisitCause() const { return visitCause; }
std::string Data::getDoctor() const { return doctor; }
std::string Data::getResult() const { return result; }
const Patient& Data::getPatient() const { return patient; }

std::string Data::getFullInfo() const {
    // Patient info() is now a real method
    // yay
    return "Patient: " + patient.info() + "\nDate: " + date +
           "\nCause: " + visitCause + "\nDoctor: " + doctor + "\nResult: " + result;
}


// ~~~~~~~~~~~~~~~~~ HistoryNode ~~~~~~~~~~~~~~~~~
HistoryNode::HistoryNode(const Data& d) : data(d), next(nullptr) {}
void HistoryNode::setNext(HistoryNode* n) { next = n; }
HistoryNode* HistoryNode::getNext() const { return next; }
const Data& HistoryNode::getData() const { return data; }


// ~~~~~~~~~~~~~~~~~~ LinkedList ~~~~~~~~~~~~~~~~~
LinkedList::LinkedList() : head(nullptr), visitCount(0) {}

LinkedList::~LinkedList() {
    // delete all nodes
    HistoryNode* current = head;
    while (current) {
        HistoryNode* nextNode = current->getNext();
        delete current;
        current = nextNode;
    }
}

void LinkedList::addVisit(const Data& d) {
    HistoryNode* newNode = new HistoryNode(d);
    if (!head) {
        head = newNode;
    } else {
        HistoryNode* temp = head;
        while (temp->getNext()) {
            temp = temp->getNext();
        }
        temp->setNext(newNode);
    }
    ++visitCount;
}

int LinkedList::getVisitCount() const { return visitCount; }

void LinkedList::printHistory() const {
    // if visit history empy
    if (!head) {
        std::cout << "History is empty.\n";
        return;
    }
    HistoryNode* temp = head;
    while (temp) {
        std::cout << temp->getData().getFullInfo() << "\n~~~~~~~~~~~~~~~~~~~~~\n";
        temp = temp->getNext();
    }
}

std::string LinkedList::getVisitInfo(int index) const {
    if (index < 0 || index >= visitCount || !head) {
        return std::string();
    }
    HistoryNode* temp = head;
    for (int i = 0; i < index; ++i) {
        temp = temp->getNext();
    }
    return temp->getData().getFullInfo() + "\n";
}


//!!!!!!!!!!ОСНОВНОЙ КЛАСС ЕГО ИСПОЛЬЗУЙТЕ!!!!!!!!
// ---------------- VisitHistory ----------------
void VisitHistory::addVisit(const Data& d) {
    visitList.addVisit(d);
}

void VisitHistory::printHistory() const {
    visitList.printHistory();
}

int VisitHistory::getVisitCount() const {
    return visitList.getVisitCount();
}
