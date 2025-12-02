#pragma once
#include <string>
#include <chrono>

class Patient {
private:
    int patientId;
    static int amountOfPatients;
    std::string name;
    std::string diagnosis;
    std::string complaints;
    char gender;
    int age;
    std::string bloodGroup;
    int severity;
    std::string department;
    std::chrono::system_clock::time_point arrivalTime;

public:
    Patient(int p_severity, std::string p_name, char p_gender, int p_age, std::string p_department,
            std::string p_complaints = "Severe Condition", std::string p_bloodGroup = "Unknown", std::string p_diagnosis = "No Records");
    Patient(int p_severity, char p_gender, std::string p_name, int p_age, std::string p_department, std::string p_bloodGroup = "Unknown");
    Patient(std::string p_name, int p_severity, char p_gender, int p_age , std::string p_department, std::string p_complaints = "Unspecified", std::string p_bloodGroup = "Unknown", std::string p_diagnosis = "No Records");
    Patient();
    ~Patient();

    // getters
    int getPatientId() const;
    std::string getName() const;
    int getSeverity() const;
    std::string getDiagnosis() const;
    char getGender() const;
    int getAge() const;
    std::string getBloodGroup() const;
    std::string getComplaints() const;
    std::string getDepartment() const;
    std::chrono::system_clock::time_point getArrivalTime() const;

    // setters
    void setDiagnosis(const std::string &p_diagnosis);
    void setBloodGroup(const std::string &bg);

    // helpers
    std::string getStringArrivalTime() const;
    std::string info() const;
};