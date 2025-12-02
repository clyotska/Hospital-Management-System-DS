#include "Patient.cpp"
#include "PriorityManagement_BTS.cpp"
#include "Circular queue.cpp"
#include "VisitHistory.h"

#include <iostream>
#include <string>
#include <limits>

using namespace std;

CircularQueue currentQueue(200);
PriorityBST currentPpriorityQueue;
VisitHistory currentVisitHistory;

void registeringNewPatient()
{

    int severity, age;
    string name, department, bloodGroup, complaints;
    char gender;

    cout << "Enter patient's full name: ";
    std::getline(cin >> ws, name); // ws is to clear buffer of input stream if anything is left

    // validating severity (1-10)
    cout << "Enter patient's severity (1-10): ";
    while (!(cin >> severity) || severity < 1 || severity > 10)
    {
        cout << "Invalid severity. Enter an integer between 1 and 10: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Enter patient's gender (M/F/O): ";
    cin >> gender;
    gender = toupper(static_cast<unsigned char>(gender)); // To Upper for the beauty, static cast to get rid of unpredicted error

    cout << "Enter patient's age: ";
    while (!(cin >> age) || age < 0)
    {
        cout << "Invalid age. Enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // clearing leftover newline before getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter patient's department (e.g., Cardiology, Orthopedics, General): ";
    std::getline(cin, department);

    if (severity < 6)
    {
        cout << "Enter patient's complaints: ";
        std::getline(cin, complaints);
        cout << "Enter patient's blood group (if unknown - press Enter): ";
        std::getline(cin, bloodGroup);

        if (bloodGroup.empty() && complaints.empty())
        {
            Patient currentPatient = Patient(name, severity, gender, age, department);
            currentQueue.Enqueue(currentPatient);
            cout << "New patient registered.";
        }
        else if (!bloodGroup.empty() && complaints.empty())
        {
            Patient currentPatient = Patient(severity, gender, name, age, department, bloodGroup);
            currentQueue.Enqueue(currentPatient);
        }
        else if (bloodGroup.empty() && !complaints.empty())
        {
            Patient currentPatient = Patient(name, severity, gender, age, department, complaints);
            currentQueue.Enqueue(currentPatient);
        }
        else
        {
            Patient currentPatient = Patient(name, severity, gender, age, department, complaints, bloodGroup);
            currentQueue.Enqueue(currentPatient);
        }
    }
    else
    {
        Patient currentPatient = Patient(severity, name, gender, age, department);
        currentPpriorityQueue.insert(currentPatient);
    }
}

void sendToDoctor()
{
    string department;
    cout << "Enter department to send to doctor from (Leave empty for General): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    std::getline(cin, department);

    if (department.empty()) // if department was left empty/enter was pressed
    {
        department = "General";
    }

    try
    {
        // Checking if the requested department's priority queue has patients
        if (!currentPpriorityQueue.isEmpty(department)) // if there are patients in priority
        {
            // calling next patient
            Patient nextPatient = currentPpriorityQueue.getNextPatient(department);
            cout << "The chosen from priority queue patient is: \n";
            cout << nextPatient.info() << "\n";
            getConsultationAndDiagnosis(nextPatient);
        }
        // if department priority queue is empty, usual queue is used
        else if (!currentQueue.isEmpty())
        {
            Patient nextPatient = currentQueue.Dequeue();
            cout << "The chosen from line patient is: \n";
            cout << nextPatient.info() << "\n";
            getConsultationAndDiagnosis(nextPatient);
        }
        // if both are empty
        else
        {
            cout << "No patients available in " << department << " department or General Queue.\n";
        }
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << "\n";
    }
}

void getConsultationAndDiagnosis(Patient &patient){
    string doctorName, diagnosis, bloodGroup;
    int enterBloodGroup;

    cout << "Patient's name: " << patient.getName() << "\n";

    cout << "Patient's complaints: " << patient.getComplaints() << "\n";

    cout << "Enter doctor's name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    std::getline(cin, doctorName);

    cout << "Enter diagnosis: ";
    std::getline(cin, diagnosis);

    cout << "Need to enter patient's blood group (0 - No | 1 - Yes): ";
    while (!(cin >> enterBloodGroup) || enterBloodGroup < 0 || enterBloodGroup!= 1)
    {
        cout << "Invalid severity. Enter an integer between 1 and 10: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (enterBloodGroup == 1)
    {
        cout << "Enter patient's blood group: ";
        std::getline(cin, bloodGroup);
        patient.setDiagnosis(diagnosis);
    }
    else
    {
        patient.setDiagnosis(diagnosis);
    }

    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    string dateStr = std::ctime(&t);
    if (!dateStr.empty() && dateStr.back() == '\n') dateStr.pop_back();

    // create and store visit record (captures patient snapshot by value)
    Data visitRecord(patient, dateStr, patient.getComplaints(), doctorName, patient.getDiagnosis());
    currentVisitHistory.addVisit(visitRecord);

    cout << "Patient updated and visit recorded.\n";
}

void displayAllWaitingPatients(){
    cout << "\n=== Priority Queues (All Departments) ===\n";
    currentPpriorityQueue.displayAll();

    cout << "\n=== General Queue ===\n";
    if (currentQueue.isEmpty()) {
        cout << "No patients in General Queue.\n";
    } else {
        currentQueue.Display();
    }
}