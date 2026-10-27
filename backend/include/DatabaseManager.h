#pragma once
#include <iostream>
#include <string>
#include <vector> // Required for std::vector in OOPS methods
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

// Include your new OOP domain classes
#include "User.h"
#include "Complaint.h"

class DatabaseManager {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

public:
    DatabaseManager();
    ~DatabaseManager();

    sql::Connection* getConnection();

    // Standard methods (used by your web routes)
    bool addComplaint(std::string email, std::string category, std::string desc);
    std::string getComplaintsJSON();
    bool resolveComplaint(int id);
    bool deleteComplaint(int id, std::string email);
    std::string getStatisticsJSON();
    bool registerStudent(std::string email, std::string name, std::string password);
    bool loginStudent(std::string email, std::string password);

    // OOPS Methods (Demonstrating Object-Oriented Domain Passing)
    bool addComplaintObj(const Complaint& complaint);
    std::vector<Complaint> getAllComplaintsObj();
    bool registerStudentObj(const Student& student);
};