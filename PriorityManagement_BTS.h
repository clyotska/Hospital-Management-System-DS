#pragma once // include guard to prevent multiple inclusions of this header


#include "Patient.h"
#include <string>

class DepartmentBST{ // BST for a single department
private:
    struct BSTNode; // defined privately in the .cpp to hide implementation details, 
    // so users of PriorityBST don't need to know about BSTNode structure
    BSTNode* root; // root of the BST
    std::string departmentName; // name of the department

    // helper functions
    bool isHigherPriority(const Patient& a, const Patient& b); // compare two patients
    BSTNode* insertNode(BSTNode* node, const Patient& p); // insert patient into BST
    BSTNode* findMaxNode(BSTNode* node); // find node with max priority (highest severity, earliest arrival)
    BSTNode* removeNode(BSTNode* node, const Patient& p); // remove patient from BST
    void displayDescending(BSTNode* node); // display patients in descending order of priority

public:
    explicit DepartmentBST(const std::string& dept); // explicit constructor to avoid implicit conversions
    ~DepartmentBST(); // destructor to free memory

    void insert(const Patient& p); // insert patient into BST
    Patient getNextPatient(); // get and remove patient with highest priority
    bool isEmpty() const; // check if BST is empty
    void display() const; // display patients in descending order of priority
    std::string getDepartmentName() const; // get the name of the department
};

class PriorityBST {
private:
    DepartmentBST* departments[10]; // array of pointers to DepartmentBSTs
    // can handle up to 10 departments
    int deptCount; // current number of departments
    int findDepartment(const std::string& dept) const; // find index of department in array
public:
    PriorityBST();
    ~PriorityBST();

    void addDepartment(const std::string& dept); // add new department
    void insert(const Patient& p, const std::string& dept); // insert patient into specified department
    Patient getNextPatient(const std::string& dept); // get next patient from specified department
    bool isEmpty(const std::string& dept) const; // check if specified department is empty
    void display(const std::string& dept); // display patients in specified department
    void displayAll(); // display patients in all departments
};