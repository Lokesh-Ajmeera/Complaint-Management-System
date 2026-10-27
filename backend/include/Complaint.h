#pragma once
#include <string>

class Complaint {
private:
    int id;
    std::string studentEmail;
    std::string studentName;
    std::string categoryName;
    std::string description;
    std::string status;

public:
    Complaint() : id(0), status("Pending") {}

    Complaint(const std::string& email, const std::string& category, const std::string& desc)
        : id(0), studentEmail(email), studentName(""), categoryName(category),
          description(desc), status("Pending") {}

    Complaint(int id, const std::string& email, const std::string& studentName,
              const std::string& category, const std::string& desc, const std::string& status)
        : id(id), studentEmail(email), studentName(studentName),
          categoryName(category), description(desc), status(status) {}

    int getId() const { return id; }
    std::string getStudentEmail() const { return studentEmail; }
    std::string getStudentName() const { return studentName; }
    std::string getCategoryName() const { return categoryName; }
    std::string getDescription() const { return description; }
    std::string getStatus() const { return status; }

    void setStatus(const std::string& newStatus) {
        status = newStatus;
    }

    void resolve() {
        status = "Resolved";
    }

    std::string toJSON() const {
        return "{\"id\":" + std::to_string(id) +
               ", \"email\":\"" + studentEmail + "\"" +
               ", \"student_name\":\"" + studentName + "\"" +
               ", \"category\":\"" + categoryName + "\"" +
               ", \"description\":\"" + description + "\"" +
               ", \"status\":\"" + status + "\"}";
    }
};