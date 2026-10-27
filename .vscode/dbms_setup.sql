-- 1. Reset Database
DROP DATABASE IF EXISTS complaint_management;
CREATE DATABASE complaint_management;
USE complaint_management;

-- 2. Create 5 Tables with Primary Keys, Foreign Keys, and Constraints[cite: 4]
CREATE TABLE categories (
    category_id INT AUTO_INCREMENT PRIMARY KEY,
    category_name VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE admins (
    admin_id INT AUTO_INCREMENT PRIMARY KEY,
    email VARCHAR(100) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE students (
    email VARCHAR(100) PRIMARY KEY,
    student_name VARCHAR(100) NOT NULL,
    password VARCHAR(255) NOT NULL
);

CREATE TABLE complaints (
    id INT AUTO_INCREMENT PRIMARY KEY,
    student_email VARCHAR(100) NOT NULL,
    category_id INT NOT NULL,
    description TEXT NOT NULL,
    status VARCHAR(20) DEFAULT 'Pending' CHECK (status IN ('Pending', 'Resolved')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (student_email) REFERENCES students(email) ON DELETE CASCADE,
    FOREIGN KEY (category_id) REFERENCES categories(category_id) ON DELETE RESTRICT
);

CREATE TABLE action_logs (
    log_id INT AUTO_INCREMENT PRIMARY KEY,
    complaint_id INT NOT NULL,
    action_taken VARCHAR(255) NOT NULL,
    action_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (complaint_id) REFERENCES complaints(id) ON DELETE CASCADE
);

-- 3. Insert Minimum 10 Sample Records[cite: 5]
INSERT INTO categories (category_name) VALUES 
('Hostel'), ('Academics'), ('Infrastructure'), ('Administration'), ('IT Support');

INSERT INTO admins (email, password) VALUES 
('admin@college.com', 'StrongPass@2026!');

INSERT INTO students (email, student_name, password) VALUES 
('s1@college.com', 'Amit Kumar', 'pass123'), ('s2@college.com', 'Priya Sharma', 'pass123'),
('s3@college.com', 'Rahul Verma', 'pass123'), ('s4@college.com', 'Sneha Reddy', 'pass123'),
('s5@college.com', 'Vikram Singh', 'pass123'), ('s6@college.com', 'Anjali Nair', 'pass123'),
('s7@college.com', 'Rohan Das', 'pass123'), ('s8@college.com', 'Kavya Iyer', 'pass123'),
('s9@college.com', 'Arjun Patel', 'pass123'), ('s10@college.com', 'Neha Gupta', 'pass123');

INSERT INTO complaints (student_email, category_id, description, status) VALUES 
('s1@college.com', 1, 'Fan in room 204 is making loud noise.', 'Pending'),
('s2@college.com', 3, 'WiFi is down in the main library.', 'Pending'),
('s3@college.com', 2, 'Clash in mid-term exam schedule.', 'Resolved'),
('s4@college.com', 4, 'Fee receipt not generated for current semester.', 'Pending'),
('s5@college.com', 1, 'Water cooler on 3rd floor is leaking.', 'Pending'),
('s6@college.com', 5, 'Cannot access the student portal.', 'Resolved'),
('s7@college.com', 3, 'Projector in LH-1 is out of focus.', 'Pending'),
('s8@college.com', 2, 'Need access to IEEE journals from hostel network.', 'Pending'),
('s9@college.com', 1, 'Mess food quality was poor on Tuesday.', 'Resolved'),
('s10@college.com', 4, 'ID card replacement request pending for 2 weeks.', 'Pending');

-- 4. Create a View (Combines Data using JOIN)[cite: 4]
CREATE VIEW v_complaint_details AS
SELECT 
    c.id, 
    s.student_name, 
    s.email, 
    cat.category_name, 
    c.description, 
    c.status, 
    c.created_at
FROM complaints c
JOIN students s ON c.student_email = s.email
JOIN categories cat ON c.category_id = cat.category_id;

-- 5. Create a Trigger (Auto-logs when a complaint is resolved)[cite: 4]
DELIMITER //
CREATE TRIGGER trg_after_complaint_resolve
AFTER UPDATE ON complaints
FOR EACH ROW
BEGIN
    IF NEW.status = 'Resolved' AND OLD.status = 'Pending' THEN
        INSERT INTO action_logs (complaint_id, action_taken) 
        VALUES (NEW.id, 'Complaint marked as Resolved by Admin');
    END IF;
END;
//
DELIMITER ;

-- 6. Create a Stored Procedure (Resolves complaint safely)[cite: 4]
DELIMITER //
CREATE PROCEDURE sp_resolve_complaint(IN p_complaint_id INT)
BEGIN
    UPDATE complaints SET status = 'Resolved' WHERE id = p_complaint_id;
END;
//
DELIMITER ;