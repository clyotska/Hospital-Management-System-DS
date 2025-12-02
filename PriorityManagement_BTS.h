#pragma once

#include "Patient.h"
#include <string>

class DepartmentBST {
private:
    struct BSTNode;               // defined privately in the .cpp
    BSTNode* root;
    std::string departmentName;

    bool isHigherPriority(const Patient& a, const Patient& b);
    BSTNode* insertNode(BSTNode* node, const Patient& p);
    BSTNode* findMaxNode(BSTNode* node);
    BSTNode* removeNode(BSTNode* node, const Patient& p);
    void displayDescending(BSTNode* node);

public:
    explicit DepartmentBST(const std::string& dept);
    ~DepartmentBST();

    void insert(const Patient& p);
    Patient getNextPatient();
    bool isEmpty() const;
    void display() const;
    std::string getDepartmentName() const;
};

class PriorityBST {
public:
    class Impl;   // pImpl forward declaration
private:
    Impl* pImpl;
public:
    PriorityBST();
    ~PriorityBST();

    void insert(const Patient& p, const std::string& dept);
    Patient getNextPatient(const std::string& dept);
    bool isEmpty(const std::string& dept) const;
    void display(const std::string& dept);
    void displayAll();
};