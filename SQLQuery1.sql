-- Ensure you are in the right database
USE db_bloodbank;
GO

-- 1. Users Table (Supports Admin, Donor, and Recipient roles) [cite: 15, 26]
CREATE TABLE Users (
    UserID INT PRIMARY KEY IDENTITY(1,1),
    Username VARCHAR(50) NOT NULL UNIQUE,
    [Password] VARCHAR(50) NOT NULL, -- Encapsulated sensitive data [cite: 16]
    UserRole VARCHAR(20) CHECK (UserRole IN ('Admin', 'Donor', 'Recipient')), -- RBAC [cite: 18]
    FullName VARCHAR(100),
    ContactDetails VARCHAR(15),
    Age INT,
    [Address] VARCHAR(255)
);

-- 2. Blood Inventory Table (Covers all 8 blood types) [cite: 20, 29]
CREATE TABLE BloodInventory (
    BloodTypeID INT PRIMARY KEY IDENTITY(1,1),
    BloodGroup VARCHAR(5) UNIQUE NOT NULL, -- A+, A-, B+, B-, O+, O-, AB+, AB-
    QuantityUnits INT DEFAULT 0
);

-- 3. Donations Table (Logs donation history) [cite: 30, 37]
CREATE TABLE Donations (
    DonationID INT PRIMARY KEY IDENTITY(1,1),
    DonorID INT FOREIGN KEY REFERENCES Users(UserID),
    BloodGroup VARCHAR(5),
    Quantity INT,
    DonationDate DATE DEFAULT GETDATE(),
    ExpiryDate DATE -- For expiry tracking [cite: 35]
);

-- 4. Blood Requests Table (For admin review and allocation) [cite: 31, 33]
CREATE TABLE BloodRequests (
    RequestID INT PRIMARY KEY IDENTITY(1,1),
    UserID INT FOREIGN KEY REFERENCES Users(UserID),
    BloodGroup VARCHAR(5),
    QuantityRequested INT,
    RequestStatus VARCHAR(20) DEFAULT 'Pending', -- Pending, Approved, Rejected
    RequestDate DATE DEFAULT GETDATE()
);




USE db_bloodbank;
GO

INSERT INTO Users (Username, [Password], UserRole, FullName, ContactDetails, Age, [Address]) VALUES 
('talha_p', 'fast2026', 'Admin', 'Talha Pasha', '0300-1112223', 20, 'Lahore'),
('maryam_s', 'pass123', 'Donor', 'Maryam Shafiq', '0321-4445556', 21, 'Lahore'),
('nabeera_s', 'user456', 'Recipient', 'Nabeera Saqib', '0333-7778889', 19, 'Lahore'),
('adnan_a', 'pass789', 'Donor', 'Adnan Ali', '0345-9990001', 22, 'Lahore'),
('ali_h', 'pwd1', 'Donor', 'Ali Hamza', '0300-1234561', 25, 'Johar Town'),
('sara_k', 'pwd2', 'Recipient', 'Sara Khan', '0300-1234562', 23, 'Gulberg'),
('usman_m', 'pwd3', 'Donor', 'Usman Malik', '0300-1234563', 30, 'DHA'),
('zoya_f', 'pwd4', 'Recipient', 'Zoya Fatima', '0300-1234564', 21, 'Model Town'),
('bilal_r', 'pwd5', 'Donor', 'Bilal Raza', '0300-1234565', 28, 'Wapda Town'),
('hina_i', 'pwd6', 'Admin', 'Hina Iqbal', '0300-1234566', 35, 'FAST Campus'),
('hamza_s', 'pwd7', 'Donor', 'Hamza Sheikh', '0300-1234567', 24, 'Iqbal Town'),
('ayesha_v', 'pwd8', 'Recipient', 'Ayesha Vali', '0300-1234568', 20, 'Garden Town'),
('omer_d', 'pwd9', 'Donor', 'Omer Dar', '0300-1234569', 27, 'Sabzazar'),
('laiba_t', 'pwd10', 'Donor', 'Laiba Tariq', '0300-1234560', 22, 'Cavalry Ground'),
('faisal_j', 'pwd11', 'Donor', 'Faisal Javed', '0312-1112221', 29, 'Faisal Town'),
('noor_e', 'pwd12', 'Recipient', 'Noor E Hira', '0312-1112222', 24, 'Shadman'),
('kashif_z', 'pwd13', 'Donor', 'Kashif Zaman', '0312-1112223', 31, 'Bahria Town'),
('dua_m', 'pwd14', 'Recipient', 'Dua Maryam', '0312-1112224', 21, 'Valencia'),
('saad_g', 'pwd15', 'Donor', 'Saad Ghaffar', '0312-1112225', 26, 'Lake City'),
('amina_b', 'pwd16', 'Donor', 'Amina Bibi', '0312-1112226', 23, 'Samnabad');



-- Various donations with mixed dates to test Expiry Tracking [cite: 35]
INSERT INTO Donations (DonorID, BloodGroup, Quantity, DonationDate, ExpiryDate) VALUES 
(2, 'A+', 1, '2026-02-15', '2026-03-25'), -- Already Expired
(4, 'B+', 2, '2026-04-01', '2026-05-10'),
(5, 'O-', 1, '2026-04-05', '2026-05-15'),
(7, 'AB+', 1, '2026-04-10', '2026-05-20'),
(9, 'A-', 2, '2026-04-12', '2026-05-22'),
(11, 'O+', 1, '2026-04-15', '2026-05-25'),
(13, 'B-', 1, '2026-04-18', '2026-05-28'),
(14, 'A+', 1, '2026-04-20', '2026-05-30'),
(15, 'AB-', 1, '2026-04-21', '2026-05-31'),
(17, 'O+', 2, '2026-04-22', '2026-06-01'),
(19, 'B+', 1, '2026-04-23', '2026-06-03'),
(20, 'A-', 1, '2026-04-24', '2026-06-04'),
(2, 'A+', 1, '2026-04-25', '2026-06-05');



INSERT INTO BloodRequests (UserID, BloodGroup, QuantityRequested, RequestStatus, RequestDate) VALUES 
(3, 'O+', 2, 'Approved', '2026-04-01'),
(6, 'A+', 1, 'Pending', '2026-04-18'),
(8, 'B-', 3, 'Rejected', '2026-04-19'),
(12, 'AB-', 1, 'Pending', '2026-04-20'),
(16, 'O-', 1, 'Approved', '2026-04-21'),
(18, 'B+', 2, 'Pending', '2026-04-22'),
(3, 'A-', 1, 'Pending', '2026-04-23'),
(6, 'O+', 2, 'Approved', '2026-04-24'),
(8, 'AB+', 1, 'Pending', '2026-04-25'),
(12, 'A+', 1, 'Pending', '2026-04-25'),
(16, 'B-', 2, 'Pending', '2026-04-25');



select* from users;