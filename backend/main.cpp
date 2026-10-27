#include "include/httplib.h"
#include "include/DatabaseManager.h"
#include "include/User.h"
#include "include/Complaint.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    httplib::Server svr;

    // Student Registration
    svr.Options("/api/student/register", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    });
    svr.Post("/api/student/register", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        string email = req.has_param("email") ? req.get_param_value("email") : "";
        string name = req.has_param("student_name") ? req.get_param_value("student_name") : "";
        string password = req.has_param("password") ? req.get_param_value("password") : "";

        Student newStudent(email, name, password);

        DatabaseManager db;
        if (db.registerStudent(newStudent.getEmail(), newStudent.getName(), newStudent.getPassword())) {
            res.set_content("Success", "text/plain");
        } else {
            res.set_content("Error: Registration failed.", "text/plain");
        }
    });

    // Login Authentication
    svr.Options("/api/login", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    });
    svr.Post("/api/login", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        string email = req.has_param("email") ? req.get_param_value("email") : "";
        string password = req.has_param("password") ? req.get_param_value("password") : "";

        DatabaseManager db;
        sql::Connection* con = db.getConnection();
        if (con != nullptr) {
            // Check Admin
            try {
                sql::PreparedStatement *pstmt = con->prepareStatement("SELECT * FROM admins WHERE email=? AND password=?");
                pstmt->setString(1, email); pstmt->setString(2, password);
                sql::ResultSet *res_set = pstmt->executeQuery();
                if (res_set->next()) { 
                    res.set_content("admin|Administrator", "text/plain"); 
                    delete res_set; 
                    delete pstmt; 
                    return; 
                }
                delete res_set; 
                delete pstmt;
            } catch (sql::SQLException &e) {}

            // Check Students
            try {
                sql::PreparedStatement *pstmt = con->prepareStatement("SELECT student_name FROM students WHERE email=? AND password=?");
                pstmt->setString(1, email); pstmt->setString(2, password);
                sql::ResultSet *res_set = pstmt->executeQuery();
                if (res_set->next()) {
                    res.set_content("student|" + res_set->getString("student_name"), "text/plain");
                    delete res_set; 
                    delete pstmt; 
                    return;
                }
                delete res_set; 
                delete pstmt;
            } catch (sql::SQLException &e) {}
        }
        res.set_content("Unauthorized", "text/plain");
    });

    // Submit Complaint
    svr.Options("/api/submit", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    });
    svr.Post("/api/submit", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        string email = req.has_param("email") ? req.get_param_value("email") : "";
        string category = req.has_param("category") ? req.get_param_value("category") : "";
        string desc = req.has_param("description") ? req.get_param_value("description") : "";

        Complaint complaint(email, category, desc);

        DatabaseManager db;
        if (db.addComplaint(complaint.getStudentEmail(), complaint.getCategoryName(), complaint.getDescription())) {
            res.set_content("Success: Complaint submitted!", "text/plain");
        } else {
            res.set_content("Error: Failed to save complaint.", "text/plain");
        }
    });

    // Resolve Complaint
    svr.Options("/api/resolve", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    });
    svr.Post("/api/resolve", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        int id = req.has_param("id") ? stoi(req.get_param_value("id")) : 0;
        DatabaseManager db;
        if (db.resolveComplaint(id)) res.set_content("Success", "text/plain");
        else res.set_content("Error: Could not resolve.", "text/plain");
    });

    // Delete Complaint
    svr.Options("/api/delete", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
    });
    svr.Post("/api/delete", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        int id = req.has_param("id") ? stoi(req.get_param_value("id")) : 0;
        string email = req.has_param("email") ? req.get_param_value("email") : "";
        DatabaseManager db;
        if (db.deleteComplaint(id, email)) res.set_content("Success", "text/plain");
        else res.set_content("Error: Could not delete.", "text/plain");
    });

    // Get All Complaints
    svr.Get("/api/complaints", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        DatabaseManager db;
        res.set_content(db.getComplaintsJSON(), "application/json");
    });

    // Get Statistics
    svr.Get("/api/statistics", [](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        DatabaseManager db;
        res.set_content(db.getStatisticsJSON(), "application/json");
    });

    cout << "Server running on http://localhost:8080" << endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}