#include "PriorityManagement_BTS.h"
#include <iostream>
#include <stdexcept>
#include <functional>

using namespace std;

// define the BSTNode structure privately
struct DepartmentBST::BSTNode {
    Patient data;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const Patient& p) : data(p), left(nullptr), right(nullptr) {}
};

// --------- DepartmentBST implementations ----------
DepartmentBST::DepartmentBST(const string& dept) : root(nullptr), departmentName(dept) {}

DepartmentBST::~DepartmentBST() {
    // postorder delete
    if (!root) return;
    // simple iterative stack-free deletion using recursion helper (lambda)
    std::function<void(BSTNode*)> destroy = [&](BSTNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    };
    destroy(root);
    root = nullptr;
}

bool DepartmentBST::isHigherPriority(const Patient& a, const Patient& b) {
    if (a.getSeverity() != b.getSeverity())
        return a.getSeverity() > b.getSeverity();
    return a.getArrivalTime() < b.getArrivalTime();
}

DepartmentBST::BSTNode* DepartmentBST::insertNode(BSTNode* node, const Patient& p) {
    if (node == nullptr) return new BSTNode(p);
    if (isHigherPriority(p, node->data))
        node->right = insertNode(node->right, p);
    else
        node->left = insertNode(node->left, p);
    return node;
}

void DepartmentBST::insert(const Patient& p) {
    root = insertNode(root, p);
}

DepartmentBST::BSTNode* DepartmentBST::findMaxNode(BSTNode* node) {
    if (!node) return nullptr;
    while (node->right != nullptr) node = node->right;
    return node;
}

DepartmentBST::BSTNode* DepartmentBST::removeNode(BSTNode* node, const Patient& pat) {
    if (node == nullptr) return nullptr;

    if (isHigherPriority(pat, node->data)) {
        node->right = removeNode(node->right, pat);
    } else if (isHigherPriority(node->data, pat)) {
        node->left = removeNode(node->left, pat);
    } else {
        if (node->left == nullptr) {
            BSTNode* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            BSTNode* temp = node->left;
            delete node;
            return temp;
        }
        BSTNode* maxLeft = findMaxNode(node->left);
        node->data = maxLeft->data;
        node->left = removeNode(node->left, maxLeft->data);
    }
    return node;
}

Patient DepartmentBST::getNextPatient() {
    if (root == nullptr) throw runtime_error("No patients in " + departmentName + " department.");
    BSTNode* maxNode = findMaxNode(root);
    Patient p = maxNode->data;
    root = removeNode(root, p);
    return p;
}

bool DepartmentBST::isEmpty() const {
    return root == nullptr;
}

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
    // cast away const to reuse internal traversal (or duplicate traversal as const)
    const_cast<DepartmentBST*>(this)->displayDescending(root);
}

string DepartmentBST::getDepartmentName() const { return departmentName; }

// ---------- PriorityBST implementations ----------
class PriorityBST::Impl {
public:
    DepartmentBST* departments[10];
    int deptCount;
    Impl() : deptCount(0) {
        for (int i = 0; i < 10; ++i) departments[i] = nullptr;
    }
    ~Impl() {
        for (int i = 0; i < deptCount; ++i) delete departments[i];
    }
    int findDepartment(const string& dept) const {
        for (int i = 0; i < deptCount; ++i) {
            if (departments[i]->getDepartmentName() == dept) return i;
        }
        return -1;
    }
    void addDepartment(const string& dept) {
        if (deptCount >= 10) { cout << "Maximum departments reached.\n"; return; }
        if (findDepartment(dept) != -1) { cout << "Department already exists.\n"; return; }
        departments[deptCount++] = new DepartmentBST(dept);
    }
};

PriorityBST::PriorityBST() : pImpl(new Impl()) {}
PriorityBST::~PriorityBST() { delete pImpl; }

void PriorityBST::insert(const Patient& p, const string& dept) {
    int idx = pImpl->findDepartment(dept);
    if (idx == -1) {
        pImpl->addDepartment(dept);
        idx = pImpl->deptCount - 1;
    }
    pImpl->departments[idx]->insert(p);
}

Patient PriorityBST::getNextPatient(const string& dept) {
    int idx = pImpl->findDepartment(dept);
    if (idx == -1) throw runtime_error("Department not found.");
    return pImpl->departments[idx]->getNextPatient();
}

bool PriorityBST::isEmpty(const string& dept) const {
    int idx = pImpl->findDepartment(dept);
    if (idx == -1) return true;
    return pImpl->departments[idx]->isEmpty();
}

void PriorityBST::display(const string& dept) {
    int idx = pImpl->findDepartment(dept);
    if (idx == -1) { cout << "Department not found.\n"; return; }
    pImpl->departments[idx]->display();
}

void PriorityBST::displayAll() {
    for (int i = 0; i < pImpl->deptCount; ++i) pImpl->departments[i]->display();
}
