#pragma once
#include <string>
#include <iostream>

template <typename T>
class ApiResponse {
private:
    bool success;
    std::string message;
    T data;

public:
    ApiResponse(bool success, const std::string& message, T data)
        : success(success), message(message), data(data) {}

    bool isSuccess() const { return success; }
    std::string getMessage() const { return message; }
    T getData() const { return data; }

    friend std::ostream& operator<<(std::ostream& os, const ApiResponse<T>& res) {
        os << "[" << (res.success ? "SUCCESS" : "ERROR") << "] " 
           << res.message << " (Payload: " << res.data << ")";
        return os;
    }
};