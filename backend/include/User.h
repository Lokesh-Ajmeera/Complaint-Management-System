#pragma once
#include <string>

class User {
protected:
    std::string email;
    std::string name;

public:
    User(const std::string& email, const std::string& name)
        : email(email), name(name) {}

    virtual ~User() {}

    virtual std::string getRole() const = 0;

    std::string getEmail() const { return email; }
    std::string getName() const { return name; }
};

class Student : public User {
private:
    std::string password;

public:
    Student(const std::string& email, const std::string& name, const std::string& password = "")
        : User(email, name), password(password) {}

    std::string getRole() const override {
        return "STUDENT";
    }

    std::string getPassword() const { 
        return password; 
    }
};

class Admin : public User {
private:
    int adminId;

public:
    Admin(int id, const std::string& email, const std::string& name)
        : User(email, name), adminId(id) {}

    std::string getRole() const override {
        return "ADMIN";
    }

    int getAdminId() const { 
        return adminId; 
    }
};