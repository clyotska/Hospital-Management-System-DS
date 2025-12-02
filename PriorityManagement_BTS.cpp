#include <iostream>
#include "Patient.cpp"
using namespace std;

class BSTNode {
public:
    Patient data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Patient p) : data(p), left(nullptr), right(nullptr) {}
};

class PriorityBST {
private:
    BSTNode* root;

    // Compare two patients for priority
    bool isHigherPriority(const Patient& a, const Patient& b) {
        if (a.getSeverity() != b.getSeverity())
            return a.getSeverity() > b.getSeverity(); // Higher severity first

        // If same severity, arrival time = higher priority
        return a.getArrivalTime() < b.getArrivalTime();
    }

    // Insert helper
    BSTNode* insertNode(BSTNode* node, Patient p) {
        if (node == nullptr) {
            return new BSTNode(p);
        }

        if (isHigherPriority(p, node->data)) {
            // Higher priority → goes RIGHT
            node->right = insertNode(node->right, p);
        } else {
            // Lower priority → goes LEFT
            node->left = insertNode(node->left, p);
        }

        return node;
    }

    // Find highest priority (rightmost)
    BSTNode* findMaxNode(BSTNode* node) {
        if (!node) return nullptr;
        while (node->right != nullptr)
            node = node->right;
        return node;
    }

    // Remove a specific patient by matching all key fields
    BSTNode* removeNode(BSTNode* node, const Patient& target) {
        if (node == nullptr) return nullptr;

        if (isHigherPriority(target, node->data)) {
            node->right = removeNode(node->right, target);
        }
        else if (isHigherPriority(node->data, target)) {
            node->left = removeNode(node->left, target);
        }
        else {
            // Node found
            if (node->left == nullptr) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }

            // Two children → use max of left subtree
            BSTNode* maxLeft = findMaxNode(node->left);
            node->data = maxLeft->data;
            node->left = removeNode(node->left, maxLeft->data);
        }

        return node;
    }

    // Reverse inorder traversal: Right → Root → Left
    void displayDescending(BSTNode* node) {
        if (!node) return;

        displayDescending(node->right);
        cout << node->data.info() << "\n--------------------------\n";
        displayDescending(node->left);
    }

public:
    PriorityBST() {
        root = nullptr;
    }

    // Insert new patient
    void insert(Patient p) {
        root = insertNode(root, p);
    }

    // Extract highest priority patient
    Patient getNextPatient() {
        if (root == nullptr) {
            throw runtime_error("No patients in priority system.");
        }

        BSTNode* maxNode = findMaxNode(root);
        Patient p = maxNode->data;

        // Remove that patient from tree
        root = removeNode(root, p);

        return p;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    // Show all patients sorted by priority
    void display() {
        if (root == nullptr) {
            cout << "No patients in priority system.\n";
            return;
        }
        displayDescending(root);
    }
};
