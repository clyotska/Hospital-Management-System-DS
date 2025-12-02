#include "PriorityManagement_BTS.h"
#include <iostream>
#include <stdexcept>
#include <functional> // including for using std::function

using namespace std;

// defining the private bstnode structure within the cpp file
struct DepartmentBST::BSTNode {
    Patient data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Patient& p) : data(p), left(nullptr), right(nullptr) {}
};

// --------- DepartmentBST implementations ----------
DepartmentBST::DepartmentBST(const string& dept) : root(nullptr), departmentName(dept) {}

DepartmentBST::~DepartmentBST() {
    if (!root) return; // doing nothing if the tree is already empty
    // defining a recursive lambda function for performing post-order traversal to delete nodes
    // using std::function to store the lambda, capturing `this` by reference ([&])
    std::function<void(BSTNode*)> destroy = [&](BSTNode* node) {
        if (!node) return;
        destroy(node->left); // recursively destroying the left subtree
        destroy(node->right); // recursively destroying the right subtree
        delete node; // deleting the node itself after its children are deleted
    };
    destroy(root); // starting the recursive deletion from the root
    root = nullptr;
}

// determining priority based on severity first, then by arrival time as a tie-breaker
bool DepartmentBST::isHigherPriority(const Patient& a, const Patient& b) {
    if (a.getSeverity() != b.getSeverity())
        return a.getSeverity() > b.getSeverity(); // favoring higher severity value
    // if severities are equal, favoring the earlier arrival time
    return a.getArrivalTime() < b.getArrivalTime();
}

// recursively finding the correct position and inserting a new node
DepartmentBST::BSTNode* DepartmentBST::insertNode(BSTNode* node, const Patient& p) {
    if (node == nullptr) return new BSTNode(p); // creating a new node if the current spot is empty
    // deciding whether to go right (higher priority) or left (lower/equal priority)
    if (isHigherPriority(p, node->data))
        node->right = insertNode(node->right, p);
    else
        node->left = insertNode(node->left, p);
    return node;
}

void DepartmentBST::insert(const Patient& p) {
    root = insertNode(root, p);
}

// finding the node with the highest priority (the rightmost node) in a subtree
DepartmentBST::BSTNode* DepartmentBST::findMaxNode(BSTNode* node) {
    if (!node) return nullptr;
    while (node->right != nullptr) node = node->right; // repeatedly moving to the right child
    return node;
}

// recursively finding and removing a node with matching patient data
DepartmentBST::BSTNode* DepartmentBST::removeNode(BSTNode* node, const Patient& pat) {
    if (node == nullptr) return nullptr; // doing nothing if the node is not found

    // navigating the tree to find the node to remove
    if (isHigherPriority(pat, node->data)) {
        node->right = removeNode(node->right, pat);
    } else if (isHigherPriority(node->data, pat)) {
        node->left = removeNode(node->left, pat);
    } else { // node found, now handling the removal based on its children
        // case 1: node has no left child
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp; // promoting the right child
        // case 2: node has no right child
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp; // promoting the left child
        }
        // case 3: node has two children
        // finding the in-order predecessor (the max node in the left subtree)
        BSTNode* maxLeft = findMaxNode(node->left);
        // replacing the current node's data with its predecessor's data
        node->data = maxLeft->data;
        // recursively removing the predecessor node from the left subtree
        node->left = removeNode(node->left, maxLeft->data);
    }
    return node;
}

Patient DepartmentBST::getNextPatient() {
    if (root == nullptr) throw runtime_error("No patients in " + departmentName + " department.");
    BSTNode* maxNode = findMaxNode(root); // finding the highest priority patient
    Patient p = maxNode->data; // getting the patient data
    root = removeNode(root, p); // removing that patient from the tree
    return p;
}

bool DepartmentBST::isEmpty() const {
    return root == nullptr;
}

// performing a reverse in-order traversal (right, root, left) to display in descending priority
void DepartmentBST::displayDescending(BSTNode* node) {
    if (!node) return;
    displayDescending(node->right);
    cout << node->data.info() << "\n--------------------------\n";
    displayDescending(node->left);
}

void DepartmentBST::display() const {
    if (root == nullptr) {
        cout << "No patients in " << departmentName << " department.\n";
        return;
    }
    cout << "=== " << departmentName << " Department ===\n";
    // using const_cast to call a non-const method from a const one, avoiding code duplication.
    // an alternative would be creating a separate, const-correct traversal helper function.
    const_cast<DepartmentBST*>(this)->displayDescending(root);
}

string DepartmentBST::getDepartmentName() const { return departmentName; }

// ---------- PriorityBST implementations ----------

PriorityBST::PriorityBST() : deptCount(0) {
    for (int i = 0; i < 10; ++i) departments[i] = nullptr;
}

PriorityBST::~PriorityBST() {
    // deleting each dynamically allocated departmentbst object
    for (int i = 0; i < deptCount; ++i) delete departments[i];
}

int PriorityBST::findDepartment(const string& dept) const {
    for (int i = 0; i < deptCount; ++i) {
        if (departments[i]->getDepartmentName() == dept) return i;
    }
    return -1; // returning -1 if the department is not found
}

void PriorityBST::addDepartment(const string& dept) {
    if (deptCount >= 10) { cout << "Maximum departments reached.\n"; return; }
    if (findDepartment(dept) != -1) { return; } // doing nothing if department already exists
    // creating a new departmentbst object on the heap
    departments[deptCount++] = new DepartmentBST(dept);
}

void PriorityBST::insert(const Patient& p, const string& dept) {
    int idx = findDepartment(dept);
    if (idx == -1) {
        addDepartment(dept); // adding the department if it doesn't exist
        idx = findDepartment(dept);
        if(idx == -1) {
            cout << "Failed to add new department " << dept << ". Maximum probably reached.\n";
            return;
        }
    }
    departments[idx]->insert(p);
}

Patient PriorityBST::getNextPatient(const string& dept) {
    int idx = findDepartment(dept);
    if (idx == -1) throw runtime_error("Department not found.");
    return departments[idx]->getNextPatient();
}

bool PriorityBST::isEmpty(const string& dept) const {
    int idx = findDepartment(dept);
    if (idx == -1) return true; // considering a non-existent department as empty
    return departments[idx]->isEmpty();
}

void PriorityBST::display(const string& dept) {
    int idx = findDepartment(dept);
    if (idx == -1) { cout << "Department not found.\n"; return; }
    departments[idx]->display();
}

void PriorityBST::displayAll() {
    cout << "\n\n---\n";
    // iterating through all existing departments and displaying their patients
    for (int i = 0; i < deptCount; ++i) departments[i]->display();
    cout << "---\n\n";
}
