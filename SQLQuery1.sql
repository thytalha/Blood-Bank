create database bloobbank_db


use bloobbank_db



-- 1. USER MANAGEMENT & DONOR PROFILES
-- Handles the base User class, plus Admin, Donor, and Recipient roles[cite: 15, 26].
-- Stores donor records including name, blood group, age, contact, and address[cite: 28].
CREATE TABLE Users (
    UserID INT IDENTITY(1,1) PRIMARY KEY,
    Username VARCHAR(50) UNIQUE NOT NULL,
    PasswordHash VARCHAR(255) NOT NULL, -- Keep security in mind!
    UserRole VARCHAR(20) NOT NULL CHECK (UserRole IN ('Admin', 'Donor', 'Recipient')), 
    FullName VARCHAR(100) NOT NULL,
    Age INT CHECK (Age >= 18), -- Assuming donors must be 18+
    BloodGroup VARCHAR(5) CHECK (BloodGroup IN ('A+', 'A-', 'B+', 'B-', 'O+', 'O-', 'AB+', 'AB-')),
    ContactNumber VARCHAR(15),
    UserAddress TEXT,
    RegistrationDate DATETIME DEFAULT GETDATE()
);

-- 2. BLOOD INVENTORY SUMMARY
-- Tracks total available blood units across all 8 blood types[cite: 29].
CREATE TABLE BloodInventory (
    BloodGroup VARCHAR(5) PRIMARY KEY CHECK (BloodGroup IN ('A+', 'A-', 'B+', 'B-', 'O+', 'O-', 'AB+', 'AB-')),
    TotalUnits INT DEFAULT 0
);

-- Pre-populate the 8 blood types into the inventory
INSERT INTO BloodInventory (BloodGroup, TotalUnits)
VALUES ('A+', 0), ('A-', 0), ('B+', 0), ('B-', 0), ('O+', 0), ('O-', 0), ('AB+', 0), ('AB-', 0);

-- 3. DONATION RECORDS
-- Logs each donation with donor ID, date, blood group, and quantity[cite: 30].
-- Acts as the donation history for donors and admins[cite: 37].
CREATE TABLE Donations (
    DonationID INT IDENTITY(1,1) PRIMARY KEY,
    DonorID INT FOREIGN KEY REFERENCES Users(UserID),
    BloodGroup VARCHAR(5),
    QuantityDonated INT NOT NULL,
    DonationDate DATETIME DEFAULT GETDATE()
);

-- 4. INDIVIDUAL BLOOD UNITS (FOR EXPIRY TRACKING)
-- Tracks individual units to manage expiry dates and flag expired stock[cite: 35, 36].
CREATE TABLE BloodUnits (
    UnitID INT IDENTITY(1,1) PRIMARY KEY,
    DonationID INT FOREIGN KEY REFERENCES Donations(DonationID),
    BloodGroup VARCHAR(5),
    CollectionDate DATE NOT NULL,
    ExpiryDate DATE NOT NULL,
    Status VARCHAR(20) DEFAULT 'Active' CHECK (Status IN ('Active', 'Issued', 'Expired'))
);

-- 5. BLOOD REQUESTS & ISSUANCE
-- Manages user requests specifying blood group, quantity, and location[cite: 31, 32].
-- Tracks whether the Admin has approved, rejected, or left the request pending.
CREATE TABLE BloodRequests (
    RequestID INT IDENTITY(1,1) PRIMARY KEY,
    RequesterID INT FOREIGN KEY REFERENCES Users(UserID),
    RequiredBloodGroup VARCHAR(5) NOT NULL,
    QuantityRequired INT NOT NULL,
    Location VARCHAR(255),
    RequestDate DATETIME DEFAULT GETDATE(),
    RequestStatus VARCHAR(20) DEFAULT 'Pending' CHECK (RequestStatus IN ('Pending', 'Approved', 'Rejected')),
    AdminComments TEXT
);



-- 1. Insert Dummy Users
-- Setting up an Admin, two Donors, and a Recipient
INSERT INTO Users (Username, PasswordHash, UserRole, FullName, Age, BloodGroup, ContactNumber, UserAddress)
VALUES 
('talha_admin', 'dummy_hash_123', 'Admin', 'Talha', 21, 'O+', '0300-1234567', 'Lahore, Pakistan'),
('adnan_donor', 'dummy_hash_456', 'Donor', 'Adnan Ali', 20, 'A+', '0300-9876543', 'Lahore, Pakistan'),
('nabeera_donor', 'dummy_hash_789', 'Donor', 'Nabeera Saqib', 20, 'B+', '0300-1112223', 'Lahore, Pakistan'),
('maryam_rec', 'dummy_hash_abc', 'Recipient', 'Maryam Shafiq', 21, 'AB-', '0300-4445556', 'Lahore, Pakistan');
GO

-- 2. Insert Dummy Donations
-- Assuming UserID 2 (Adnan) and UserID 3 (Nabeera) are donating
INSERT INTO Donations (DonorID, BloodGroup, QuantityDonated, DonationDate)
VALUES 
(2, 'A+', 1, '2026-04-01'), 
(3, 'B+', 2, '2026-04-15');
GO

-- 3. Insert Corresponding Individual Blood Units
-- Red blood cells typically expire after 42 days, so the expiry dates reflect that.
INSERT INTO BloodUnits (DonationID, BloodGroup, CollectionDate, ExpiryDate, Status)
VALUES 
(1, 'A+', '2026-04-01', '2026-05-13', 'Active'), -- Adnan's 1 unit
(2, 'B+', '2026-04-15', '2026-05-27', 'Active'), -- Nabeera's 1st unit
(2, 'B+', '2026-04-15', '2026-05-27', 'Active'); -- Nabeera's 2nd unit
GO

-- 4. Update the Blood Inventory Summaries
-- Note: In your final project, your C++ code should ideally update this table automatically whenever a donation happens!
UPDATE BloodInventory SET TotalUnits = 1 WHERE BloodGroup = 'A+';
UPDATE BloodInventory SET TotalUnits = 2 WHERE BloodGroup = 'B+';
GO

-- 5. Insert Dummy Blood Requests
-- Assuming UserID 4 (Maryam) needs blood at different locations
INSERT INTO BloodRequests (RequesterID, RequiredBloodGroup, QuantityRequired, Location, RequestDate, RequestStatus, AdminComments)
VALUES 
(4, 'AB-', 1, 'Shaukat Khanum Hospital, Lahore', '2026-04-20', 'Pending', NULL),
(4, 'O+', 1, 'Jinnah Hospital, Lahore', '2026-04-22', 'Approved', 'Urgent request processed.');
GO



select* from Users