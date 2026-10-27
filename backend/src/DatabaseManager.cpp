#include "../include/DatabaseManager.h"
#include <sstream>

using namespace std;

DatabaseManager::DatabaseManager() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "admin_user", "StrongPass@2026!");
        con->setSchema("complaint_management");
        cout << "Database connection successful!" << endl;
    } catch (sql::SQLException &e) {
        cout << "Database Connection Error: " << e.what() << endl;
        con = nullptr;
    }
}

DatabaseManager::~DatabaseManager() {
    if (con != nullptr) {
        delete con;
        cout << "Database connection closed." << endl;
    }
}

sql::Connection* DatabaseManager::getConnection() { return con; }

bool DatabaseManager::addComplaint(string email, string category, string desc) {
    if (con == nullptr) return false;
    try {
        sql::PreparedStatement *pstmt = con->prepareStatement(
            "INSERT INTO complaints (student_email, category_id, description) "
            "VALUES (?, (SELECT category_id FROM categories WHERE category_name = ?), ?)"
        );
        pstmt->setString(1, email);
        pstmt->setString(2, category);
        pstmt->setString(3, desc);
        pstmt->executeUpdate(); 
        delete pstmt;
        return true;
    } catch (sql::SQLException &e) {
        cout << "Insert Error: " << e.what() << endl;
        return false;
    }
}

// OOPS OVERLOAD: Accepts domain object Complaint
bool DatabaseManager::addComplaintObj(const Complaint& complaint) {
    return addComplaint(complaint.getStudentEmail(), complaint.getCategoryName(), complaint.getDescription());
}

// OOPS METHOD: Returns std::vector<Complaint> objects instead of raw string
vector<Complaint> DatabaseManager::getAllComplaintsObj() {
    vector<Complaint> list;
    if (con == nullptr) return list;
    try {
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM v_complaint_details ORDER BY id DESC");
        
        while (res->next()) {
            Complaint c(
                res->getInt("id"),
                res->getString("email"),
                res->getString("student_name"),
                res->getString("category_name"),
                res->getString("description"),
                res->getString("status")
            );
            list.push_back(c);
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Fetch Objects Error: " << e.what() << endl;
    }
    return list;
}

// Rewritten to reuse getAllComplaintsObj() internally to showcase Object Modeling
string DatabaseManager::getComplaintsJSON() {
    vector<Complaint> list = getAllComplaintsObj();
    string json = "[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i > 0) json += ", ";
        json += list[i].toJSON();
    }
    json += "]";
    return json;
}

bool DatabaseManager::resolveComplaint(int id) {
    if (con == nullptr) return false;
    try {
        sql::PreparedStatement *pstmt = con->prepareStatement("CALL sp_resolve_complaint(?)");
        pstmt->setInt(1, id); 
        pstmt->executeUpdate(); 
        delete pstmt;
        return true; 
    } catch (sql::SQLException &e) {
        cout << "Procedure Error: " << e.what() << endl;
        return false;
    }
}

bool DatabaseManager::deleteComplaint(int id, string email) {
    if (con == nullptr) return false;
    try {
        sql::PreparedStatement *pstmt = con->prepareStatement(
            "DELETE FROM complaints WHERE id = ? AND student_email = ?"
        );
        pstmt->setInt(1, id);
        pstmt->setString(2, email);
        int rows = pstmt->executeUpdate();
        delete pstmt;
        return rows > 0;
    } catch (sql::SQLException &e) {
        cout << "Delete Error: " << e.what() << endl;
        return false;
    }
}

string DatabaseManager::getStatisticsJSON() {
    if (con == nullptr) return "[]";
    try {
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery(
            "SELECT cat.category_name, COUNT(c.id) as total "
            "FROM categories cat LEFT JOIN complaints c ON cat.category_id = c.category_id "
            "GROUP BY cat.category_name"
        );
        
        string json = "[";
        bool first = true;
        while (res->next()) {
            if (!first) json += ", ";
            json += "{";
            json += "\"category\":\"" + res->getString("category_name") + "\", ";
            json += "\"total\":" + to_string(res->getInt("total"));
            json += "}";
            first = false;
        }
        json += "]";
        delete res;
        delete stmt;
        return json;
    } catch (sql::SQLException &e) {
        cout << "Aggregate Query Error: " << e.what() << endl;
        return "[]";
    }
}

bool DatabaseManager::registerStudent(string email, string name, string password) {
    if (con == nullptr) return false;
    try {
        sql::PreparedStatement *pstmt = con->prepareStatement(
            "INSERT INTO students (email, student_name, password) VALUES (?, ?, ?)"
        );
        pstmt->setString(1, email);
        pstmt->setString(2, name);
        pstmt->setString(3, password);
        pstmt->executeUpdate();
        delete pstmt;
        return true;
    } catch (sql::SQLException &e) {
        cout << "Registration Error: " << e.what() << endl;
        return false;
    }
}

// OOPS OVERLOAD: Accepts Student domain object
bool DatabaseManager::registerStudentObj(const Student& student) {
    return registerStudent(student.getEmail(), student.getName(), student.getPassword());
}

bool DatabaseManager::loginStudent(string email, string password) {
    if (con == nullptr) return false;
    try {
        sql::PreparedStatement *pstmt = con->prepareStatement(
            "SELECT * FROM students WHERE email = ? AND password = ?"
        );
        pstmt->setString(1, email);
        pstmt->setString(2, password);
        sql::ResultSet *res = pstmt->executeQuery();
        bool success = res->next();
        delete res;
        delete pstmt;
        return success;
    } catch (sql::SQLException &e) {
        return false;
    }
}