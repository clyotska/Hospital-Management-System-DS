#include "Patient.cpp"
#include "PriorityManagement_BTS.cpp"
#include "Circular queue.cpp"
#include "VisitHistory.h"

#include <iostream>
#include <string>

using namespace std;

CircularQueue currentQueue(200);
PriorityBST currentPpriorityQueue;
VisitHistory currentVisitHistory;

void registeringNewPatient(){
    
    int patId; int severity; int age;
    string name; string disease; char gender; string bloodGroup; string complaints;
    
    cout << "Enter patient's full name: ";
    std::getline(cin >> ws, name); // ws is to clear buffer of input stream if anything is left

    // validating severity (1-10)
    cout << "Enter patient's severity (1-10): ";
    while(!(cin >> severity) || severity < 1 || severity > 10) {
        cout << "Invalid severity. Enter an integer between 1 and 10: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter patient's gender (M/F/O): ";
    cin >> gender;
    gender = toupper(static_cast<unsigned char>(gender)); // To Upper for the beauty, static cast to get rid of unpredicted error 

    cout << "Enter patient's age: ";
    while(!(cin >> age) || age < 0) {
        cout << "Invalid age. Enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // clearing leftover newline before getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (severity < 6){
        cout << "Enter patient's complaints: ";
        std::getline(cin, complaints);
        cout << "Enter patient's blood group (if unknown - press Enter): ";
        std::getline(cin, bloodGroup);

        if (bloodGroup.empty() && complaints.empty()){
            Patient currentPatient = Patient( name, severity, gender, age);
            currentQueue.Enqueue(currentPatient);
            cout << "New patient registered.";
        }
        else if (!bloodGroup.empty() && complaints.empty()){
            Patient currentPatient = Patient(severity, gender, name, age, bloodGroup);
            currentQueue.Enqueue(currentPatient);
            cout << "New patient registered.";
        }
        else if (bloodGroup.empty() && !complaints.empty()){
            Patient currentPatient = Patient(severity, name, gender, age, complaints);
            currentQueue.Enqueue(currentPatient);
            cout << "New patient registered.";
        }
        else{
            Patient currentPatient = Patient(name, severity, gender,  age, complaints, bloodGroup);
            currentQueue.Enqueue(currentPatient);
            cout << "New patient registered.";
        }

    }
    else{
        Patient currentPatient = Patient(severity, name, gender, age);
        currentPpriorityQueue.insert(currentPatient);
        cout << "New patient registered.";
    }
}

void visitDoctor(){
    
}