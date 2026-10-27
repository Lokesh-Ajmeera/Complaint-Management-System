
CREATE DATABASE IF NOT EXISTS complaint_management;
USE complaint_management;

CREATE TABLE users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    full_name VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    role ENUM('student', 'admin') DEFAULT 'student',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,
    category_name VARCHAR(50) UNIQUE NOT NULL
);

CREATE TABLE complaints (
    complaint_id INT AUTO_INCREMENT PRIMARY KEY,
    student_id INT NOT NULL,
    category_id INT NOT NULL,
    title VARCHAR(100) NOT NULL,
    description TEXT NOT NULL,
    priority ENUM('Low', 'Medium', 'High', 'Critical') DEFAULT 'Low',
    status ENUM('Submitted', 'Under Review', 'Assigned', 'In Progress', 'Resolved', 'Rejected') DEFAULT 'Submitted',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    FOREIGN KEY (student_id) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES categories(category_id) ON DELETE CASCADE
);

INSERT INTO categories (category_name) VALUES 
('Hostel'), ('Mess/Food'), ('Electricity'), ('Water'), 
('Internet'), ('Classroom'), ('Laboratory'), ('Cleaning'), ('Other');

INSERT INTO users (full_name, email, password, role) VALUES 
('Lokesh Admin', 'admin@college.edu', 'password123', 'admin'),
('Student Demo', 'student@college.edu', 'password123', 'student');

INSERT INTO complaints (student_id, category_id, title, description, priority, status) VALUES 
(2, 5, 'Wi-Fi not working', 'No internet in Hostel Block A since yesterday.', 'High', 'Submitted');