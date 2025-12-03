#include "Patient.h"
#include "PriorityManagement_BTS.h"
#include "CircularQueue.h"
#include "VisitHistory.h"

#include <iostream>
#include <string>
#include <limits> // including the limits library for numeric limits to ignore \n left in buffer after cin

using namespace std;

// helper for trimming whitespace from both ends of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r"); // finding the first non-whitespace character
    // " \t\n\r": C-style string literal containing the characters considered as whitespace:
    if (string::npos == first) { // checking if the string is empty
        // npos roughly equals to -1, it's unsigned integer value
        return ""; // returning an empty string
    }
    size_t last = str.find_last_not_of(" \t\n\r"); // finding the last non-whitespace character
    return str.substr(first, (last - first + 1)); // returning the trimmed string
}

CircularQueue currentQueue(200); // creating a circular queue with a capacity of 200
PriorityBST currentPpriorityQueue; // creating a priority binary search tree
VisitHistory currentVisitHistory; // creating a visit history object

void registeringNewPatient();
void sendToDoctor();
void getConsultationAndDiagnosis(Patient &patient);
void displayAllWaitingPatients(); 

void registeringNewPatient()
{

    int severity, age;
    string name, department, bloodGroup, complaints;
    char gender;

    cout << "Enter patient's full name: ";
    std::getline(cin >> ws, name);

    // validating severity
    cout << "Enter patient's severity (1-10): ";
    while (!(cin >> severity) || severity < 1 || severity > 10) // looping until a valid severity is entered
    {
        cout << "Invalid severity. Enter an integer between 1 and 10: ";
        cin.clear(); // clearing the input stream
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of the line
    }

    cout << "Enter patient's gender (M/F/O): ";
    cin >> gender;
    gender = toupper(static_cast<unsigned char>(gender)); // converting the gender to uppercase to look pretty

    cout << "Enter patient's age: ";
    while (!(cin >> age) || age < 0) // looping until a valid age is entered
    {
        cout << "Invalid age. Enter a non-negative integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // clearing leftover newline before getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter patient's department (Cardiology, Emergency, Pediatrics, General): ";
    std::getline(cin, department);
    department = trim(department); // trimming whitespace from the department string
    if (department.empty()) {
        department = "General";
    }


    if (severity < 6) // checking if the severity is less than 6
    // we aren't in rush, so asking for both complaints and diagnosis
    {
        cout << "Enter patient's complaints: ";
        std::getline(cin, complaints);
        cout << "Enter patient's blood group (if unknown - press Enter): ";
        std::getline(cin, bloodGroup);

        if (bloodGroup.empty() && complaints.empty()) // checking if both blood group and complaints are empty
        {
            Patient currentPatient = Patient(name, severity, gender, age, department); // creating a patient object
            currentQueue.Enqueue(currentPatient); // enqueuing the patient in the circular queue
            cout << "New patient registered."; // printing a success message
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
        else // if both blood group and complaints are not empty
        {
            Patient currentPatient = Patient(name, severity, gender, age, department, complaints, bloodGroup);
            currentQueue.Enqueue(currentPatient);
        }
    }
    else // if severity is 6 or greater
    // skipping complaint to rush a person into Priority, the doctors will understand themselves
    {
        Patient currentPatient = Patient(severity, name, gender, age, department);
        currentPpriorityQueue.insert(currentPatient, department);
    }
}

void sendToDoctor() // defining the function for sending a patient to a doctor
{
    string department; 
    cout << "Enter department to send to doctor from (Leave empty for General): "; 
    std::getline(cin, department);
    department = trim(department); // trimming whitespace from the department string

    if (department.empty()) // checking if the department is empty
    {
        department = "General"; // setting the department to "General" if it's empty
    }

    try
    {
        // checking if the requested department's priority queue has patients
        if (!currentPpriorityQueue.isEmpty(department)) // if there are patients in the priority queue
        {
            // calling next patient
            Patient nextPatient = currentPpriorityQueue.getNextPatient(department);
            cout << "The chosen from priority queue patient is: \n";
            cout << nextPatient.info() << "\n";
            getConsultationAndDiagnosis(nextPatient); // calling the function to get consultation and diagnosis
        }
        // if department priority queue is empty, usual queue is used
        else if (!currentQueue.isEmpty()) // if the circular queue is not empty
        {
            Patient nextPatient = currentQueue.Dequeue(); // dequeuing a patient from the circular queue
            cout << "The chosen from line patient is: \n";
            cout << nextPatient.info() << "\n";
            getConsultationAndDiagnosis(nextPatient); // calling the function to get consultation and diagnosis
        }
        // if both are empty
        else // if both queues are empty
        {
            cout << "No patients available in " << department << " department or General Queue.\n";
        }
    }
    catch (const exception &e) // catching any exceptions
    {
        cout << "Error: " << e.what() << "\n"; // printing the error message
    }
}

void getConsultationAndDiagnosis(Patient &patient){ // defining the function for getting consultation and diagnosis
    string doctorName, diagnosis, bloodGroup;
    int enterBloodGroup;

    cout << "Patient's name: " << patient.getName() << "\n";

    cout << "Patient's complaints: " << patient.getComplaints() << "\n";

    cout << "Enter doctor's name: "; // prompting for the doctor's name
    std::getline(cin, doctorName);

    cout << "Enter diagnosis: "; // prompting for the diagnosis
    std::getline(cin, diagnosis);

    cout << "Need to enter patient's blood group (0 - No | 1 - Yes): "; // prompting for blood group entry
    while (!(cin >> enterBloodGroup) || enterBloodGroup < 0 || enterBloodGroup > 1) // looping until a valid choice is entered
    {
        cout << "Invalid choice. Pick 0 or 1: "; // prompting for a valid choice
        cin.clear(); // clearing the input stream
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of the line
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of the line

    if (enterBloodGroup == 1) // checking if the user wants to enter the blood group
    {
        cout << "Enter patient's blood group: "; // prompting for the blood group
        std::getline(cin, bloodGroup); // getting the blood group
        patient.setBloodGroup(bloodGroup); // setting the bloodGroup for the patient
        patient.setDiagnosis(diagnosis); // setting the diagnosis for the patient
    }
    else // if the user does not want to enter the blood group
    {
        patient.setDiagnosis(diagnosis); // setting the diagnosis for the patient
    }

    auto now = std::chrono::system_clock::now(); // getting the current time
    auto t = std::chrono::system_clock::to_time_t(now); // converting the time to a time_t object
    string dateStr = std::ctime(&t); // converting the time to a string
    if (!dateStr.empty() && dateStr.back() == '\n') dateStr.pop_back(); // removing the newline character from the date string

    // creating and storing a visit record
    Data visitRecord(patient, dateStr, patient.getComplaints(), doctorName, patient.getDiagnosis()); // creating a data object
    currentVisitHistory.addVisit(visitRecord); // adding the visit record to the visit history

    cout << "Patient updated and visit recorded.\n"; // printing a success message
}

void displayAllWaitingPatients(){ // defining the function for displaying all waiting patients
    cout << "\n=== Priority Queues (All Departments) ===\n"; // printing a header
    currentPpriorityQueue.displayAll(); // displaying all patients in the priority queue

    cout << "\n=== General Queue ===\n"; // printing a header
    if (currentQueue.isEmpty()) { // checking if the circular queue is empty
        cout << "No patients in General Queue.\n"; // printing a message
    } else { // if the circular queue is not empty
        currentQueue.Display(); // displaying the patients in the circular queue
    }
}


void showMenu() {

    cout << "\n========================================\n";
    cout << "   HOSPITAL SYSTEM  \n";
    cout << "========================================\n";
    cout << "1. Register New Patient\n";
    cout << "2. Send Next Patient to Doctor\n";
    cout << "3. View All Waiting Patients (Queue + BST)\n";
    cout << "4. View Visit History\n"; 
    cout << "5. Exit\n";
    cout << "========================================\n";
    cout << "Select Option: ";
}

int main() { // defining the main function

    // adding departments
    currentPpriorityQueue.addDepartment("Emergency");
    currentPpriorityQueue.addDepartment("Cardiology");
    currentPpriorityQueue.addDepartment("Pediatrics");
    currentPpriorityQueue.addDepartment("General");

    int choice; // declaring an integer variable for user choice
    while (true) { // starting an infinite loop
        showMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // clearing the input stream
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignoring the rest of the line
            continue; // continuing to the next iteration of the loop
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing the buffer

        switch (choice) { // starting a switch statement for the user's choice
        case 1:
            registeringNewPatient();
            break;
        case 2:
            sendToDoctor();
            break;
        case 3:
            displayAllWaitingPatients();
            break;
        case 4:
            cout << "\n=== FULL VISIT HISTORY ===\n";
            currentVisitHistory.printHistory();
            break;
        case 5:
            cout << "Shutting down system...\n";
            return 0;
        default:
            cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}