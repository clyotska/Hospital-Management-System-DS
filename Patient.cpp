#include "Patient.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

// define static member once
int Patient::amountOfPatients = 0;

Patient::Patient(int p_severity, std::string p_name, char p_gender, int p_age, std::string p_department,
                 std::string p_complaints, std::string p_bloodGroup, std::string p_diagnosis)
    : patientId(++amountOfPatients), name(std::move(p_name)), diagnosis(std::move(p_diagnosis)),
      complaints(std::move(p_complaints)), gender(p_gender), age(p_age), bloodGroup(std::move(p_bloodGroup)),
      severity(p_severity), department(std::move(p_department)), arrivalTime(std::chrono::system_clock::now())
{}

Patient::Patient(int p_severity, char p_gender, std::string p_name, int p_age, std::string p_department, std::string p_bloodGroup)
    : patientId(++amountOfPatients), name(std::move(p_name)), diagnosis("No Records"),
      complaints("Unspecified"), gender(p_gender), age(p_age), bloodGroup(std::move(p_bloodGroup)),
      severity(p_severity), department(std::move(p_department)), arrivalTime(std::chrono::system_clock::now())
{}

Patient::Patient(std::string p_name, int p_severity, char p_gender, int p_age, std::string p_department, std::string p_complaints, std::string p_bloodGroup, std::string p_diagnosis)
    : patientId(++amountOfPatients), name(std::move(p_name)), diagnosis(std::move(p_diagnosis)),
      complaints(std::move(p_complaints)), gender(p_gender), age(p_age), bloodGroup(std::move(p_bloodGroup)),
      severity(p_severity), department(std::move(p_department)), arrivalTime(std::chrono::system_clock::now())
{}

Patient::Patient()
    : patientId(++amountOfPatients), name("Unknown"), diagnosis("No Records"),
      complaints("Unspecified"), gender('-'), age(0), bloodGroup("Unknown"),
      severity(0), department("General"), arrivalTime(std::chrono::system_clock::now())
{}

Patient::~Patient() = default;

int Patient::getPatientId() const { return patientId; }
std::string Patient::getName() const { return name; }
int Patient::getSeverity() const { return severity; }
std::string Patient::getDiagnosis() const { return diagnosis; }
char Patient::getGender() const { return gender; }
int Patient::getAge() const { return age; }
std::string Patient::getBloodGroup() const { return bloodGroup; }
std::string Patient::getComplaints() const { return complaints; }
std::string Patient::getDepartment() const { return department; }
std::chrono::system_clock::time_point Patient::getArrivalTime() const { return arrivalTime; }

void Patient::setDiagnosis(const std::string &p_diagnosis) { diagnosis = p_diagnosis; }
void Patient::setBloodGroup(const std::string &bg) { bloodGroup = bg; }

std::string Patient::getStringArrivalTime() const {
    std::time_t t = std::chrono::system_clock::to_time_t(arrivalTime);
    std::tm tm{};
#if defined(_MSC_VER)
    localtime_s(&tm, &t);
#elif defined(__GLIBC__)
    localtime_r(&t, &tm);
#else
    tm = *std::localtime(&t);
#endif
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buf);
}

std::string Patient::info() const {
    std::ostringstream oss;
    oss << "Patient ID: " << getPatientId() << "\n"
        << "Name: " << getName() << "\n"
        << "Severity: " << getSeverity() << "\n"
        << "Department: " << getDepartment() << "\n"
        << "Arrival: " << getStringArrivalTime() << "\n"
        << "Gender: " << getGender() << "  Age: " << getAge() << "\n"
        << "Complaints: " << getComplaints() << "\n"
        << "Blood Group: " << getBloodGroup() << "\n"
        << "Diagnosis: " << getDiagnosis();
    return oss.str();
}