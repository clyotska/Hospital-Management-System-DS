#include <iostream>
#include "VisitHistory.h" // included to story the history of a patient
// in a patient object

// My suffering was imminent 
#include <chrono> // chrono is used to get now system time
// #include <format> // i wanted to use format with chrono, but for some reason on my machine
// format + chrono returned wrong values for hour and minute in arrivalTime
#include <ctime> // so i had to stick to ctime and make a specific windows func for arrivalTime to string conversion
// i don't have a Mac, so mmy hope and prayers goes to Mac users, let's hope it'll work for them too
#include <string>


using namespace std;

class Patient
{

private:
    int patientId;
    static int amountOfPatients;
    string name;
    string diagnosis;
    string complaints;
    char gender;
    int age;
    string bloodGroup; // added bloodGroup, we can delete it tho. Idk maybe a doctor will test for bloodGroup too
    int severity;
    chrono::system_clock::time_point arrivalTime; // to store our system time

public:
// parametrized constructor, if diagnosis and bloodGroup aren't provided just makes them Unkown
    Patient(int p_severity, string p_name, char p_gender, int p_age, string p_complaints = "Severe Condition", string p_bloodGroup = "Unknown", string p_diagnosis = "No Records")
    {
        this->patientId = amountOfPatients++;
        this->severity = p_severity;
        this->name = p_name;
        this->diagnosis = p_diagnosis;
        this->gender = p_gender;
        this->age = p_age;
        this->complaints = p_complaints;
        this->bloodGroup = p_bloodGroup;
        this->arrivalTime = std::chrono::system_clock::now();; // sets the arrivalTime to current system time
        // this will work on both Mac and Windows (hopefully)
    }

    Patient(int p_severity, char p_gender, string p_name, int p_age, string p_bloodGroup)
    {
        this->patientId = amountOfPatients++; this->severity = p_severity;
        this->name = p_name; this->diagnosis = "No Records"; this->gender = p_gender;
        this->age = p_age; this->complaints = "General Health Check Up"; this->bloodGroup = p_bloodGroup;
        this->arrivalTime = std::chrono::system_clock::now();; // sets the arrivalTime to current system time
        // this will work on both Mac and Windows (hopefully)
    }

    Patient(string p_name, int p_severity, char p_gender, int p_age, string p_complaints = "Unspecified", string p_bloodGroup = "Unknown", string p_diagnosis = "No Records")
    {
        this->patientId = amountOfPatients++; this->severity = p_severity; this->name = p_name;
        this->diagnosis = p_diagnosis; this->gender = p_gender; this->age = p_age; this->complaints = p_complaints;
        this->bloodGroup = p_bloodGroup;
        this->arrivalTime = std::chrono::system_clock::now();; // sets the arrivalTime to current system time
        // this will work on both Mac and Windows (hopefully)
    }

    Patient(){
        patientId = 0;
        severity = 0;
        name = "Unknown";
        diagnosis = "Unknown";
        gender = '-';
        age = 0;
        bloodGroup = "Unknown";
        arrivalTime = std::chrono::system_clock::now();
    }

    ~Patient() {}

    // getters
    int getPatientId() const { return patientId; }
    string getName() const { return name; }
    int getSeverity() const { return severity; }
    string getDiagnosis() const { return diagnosis; }
    char getGender() const { return gender; }
    int getAge() const { return age; }
    string getBloodGroup() const { return bloodGroup; }
    string getComplaints() const { return complaints; }
    chrono::system_clock::time_point getArrivalTime() const {
        return arrivalTime;
    }

    // a horrible monstrosity of a func, and it's Windows Specific
    string getStringArrivalTime() const {
        // Arrival Time is stored using time_point type from the chono lib
        // time_point is good for calculations/comparing, but it's not hard to read for people
        auto timeT = std::chrono::system_clock::to_time_t(arrivalTime); // takes the arrival time and translates it
        // into the time_t type for easier to string conversion
        // time_t is a number of seconds since a specific epoch
        std::tm localTime; // local time is an object of a class tm, that holds year, month, day, hour and minute separately
        // we wil use localTime to store our now time_t arrivalTime in a human readable format
        localtime_s(&localTime, &timeT);  // this is a windows specific func to convert time_t into calendar format (year-month-date, etc.)
        char buffer[64]; // a block for the new calendar format string (array of 64 characters)

        // strftime (String Format Time) creates a formatted string, using localtime to fill the buffer
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %I:%M %p", &localTime);
        return string(buffer);
    }

    // a setter, use it in historyNode to set the diagnosis
    void setDiagnosis(string p_diagnosis) { this->diagnosis = p_diagnosis; }

    string info() const {
        string patientInfo = "Patient ID: " + to_string(getPatientId()) + "\nPatient Name: " + getName()
        + "\nSeverity: " + to_string(getSeverity()) + 
        "\nArrival Time: " + getStringArrivalTime() + "\nGender & Age: " + getGender() + ", " + to_string(getAge())
        + "\nComplaints: " + getComplaints() + "\nDiagnosis: " + getDiagnosis() +  "\nBlood Group: " + getBloodGroup();
        return patientInfo;
    }
};