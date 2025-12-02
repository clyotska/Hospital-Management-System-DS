#include <iostream>
#include <string>
#include <stdexcept> // for runtime_error
#include "Patient.cpp"
using namespace std;

class BSTNode { // node structure for BST
public:
    Patient data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Patient p) : data(p), left(nullptr), right(nullptr) {}
};

class DepartmentBST { // BST for a specific department
private:
    BSTNode* root;
    string departmentName;

    bool isHigherPriority(const Patient& a, const Patient& b) { // returns true if a has higher priority than b
        // if priorities are different, higher severity has higher priority
        // if priorities are same, earlier arrival time has higher priority
        if (a.getSeverity() != b.getSeverity())
            return a.getSeverity() > b.getSeverity();
        return a.getArrivalTime() < b.getArrivalTime();
    }

    BSTNode* insertNode(BSTNode* node, Patient p) {
        // base case
        if (node == nullptr) {
            return new BSTNode(p);
        }

        // recursive case
        if (isHigherPriority(p, node->data)) {
            node->right = insertNode(node->right, p);
        } else {
            node->left = insertNode(node->left, p);
        }
        return node;
    }

    // find the node with the maximum value (highest priority patient)
    BSTNode* findMaxNode(BSTNode* node) {
        if (!node) return nullptr;
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    BSTNode* removeNode(BSTNode* node, const Patient& pat) {
        // base case
        if (node == nullptr) return nullptr;

        // recursive case
        if (isHigherPriority(pat, node->data)) {
            node->right = removeNode(node->right, pat);
        }
        else if (isHigherPriority(node->data, pat)) {
            node->left = removeNode(node->left, pat);
        } // found the node to be deleted
        else {
            // node with only one child or no child
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } // node with only one child or no child
            else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }

            //two children case
            // get the inorder predecessor (max in the left subtree)
            BSTNode* maxLeft = findMaxNode(node->left);
            node->data = maxLeft->data;
            node->left = removeNode(node->left, maxLeft->data);
        }
        return node;
    }

    // display patients in descending order of priority
    void displayDescending(BSTNode* node) {
        if (!node) return;
        displayDescending(node->right);
        cout << node->data.info() << "\n--------------------------\n";
        displayDescending(node->left);
    }

public:
    DepartmentBST(string dept) : root(nullptr), departmentName(dept) {}

    
    void insert(Patient p) {
        root = insertNode(root, p);
    }

    // get the patient with the highest priority and remove from BST
    Patient getNextPatient() {
        if (root == nullptr) {
            throw runtime_error("No patients in " + departmentName + " department.");
        }
        BSTNode* maxNode = findMaxNode(root);
        Patient p = maxNode->data;
        root = removeNode(root, p);
        return p;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    // display all patients in the department
    void display() {
        if (root == nullptr) {
            cout << "No patients in " << departmentName << " department.\n";
            return;
        }
        cout << "=== " << departmentName << " Department ===\n";
        displayDescending(root);
    }

    // get the department name
    string getDepartmentName() const {
        return departmentName;
    }
};

class PriorityBST {
private:
    DepartmentBST* departments[10]; // array of department BSTs (max 10 departments)
    int deptCount;

    // helper to find department index
    int findDepartment(const string& dept) const {
        for (int i = 0; i < deptCount; i++) {
            if (departments[i]->getDepartmentName() == dept)
                return i;
        }
        return -1;
    }

public:
    // constructor
    PriorityBST() : deptCount(0) {
        for (int i = 0; i < 10; i++) {
            departments[i] = nullptr;
        }
    }

    // add a new department
    void addDepartment(const string& dept) {
        if (deptCount >= 10) {
            cout << "Maximum departments reached.\n";
            return;
        }
        if (findDepartment(dept) != -1) {
            cout << "Department already exists.\n";
            return;
        }
        departments[deptCount] = new DepartmentBST(dept);
        deptCount++;
    }

    // insert patient into appropriate department
    void insert(Patient p, const string& dept = "General") {
        int idx = findDepartment(dept);
        if (idx == -1) {
            addDepartment(dept);
            idx = deptCount - 1;
        }
        departments[idx]->insert(p);
    }

    // get the next patient from a specific department
    Patient getNextPatient(const string& dept = "General") {
        int idx = findDepartment(dept); // find the department
        if (idx == -1) { // department not found
            throw runtime_error("Department not found.");
        } // get next patient
        return departments[idx]->getNextPatient();
    }

    // check if a specific department is empty
    bool isEmpty(const string& dept = "General") const {
        int idx = findDepartment(dept);
        if (idx == -1) return true;
        return departments[idx]->isEmpty();
    }

    // display all patients in a specific department
    void display(const string& dept = "General") {
        int idx = findDepartment(dept);
        if (idx == -1) {
            cout << "Department not found.\n";
            return;
        }
        departments[idx]->display();
    }

    void displayAll() { // display all patients in all departments
        for (int i = 0; i < deptCount; i++) {
            departments[i]->display();
        }
    }

    ~PriorityBST() {
        for (int i = 0; i < deptCount; i++) {
            delete departments[i];
        }
    }
};
