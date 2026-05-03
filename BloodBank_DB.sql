-- ==========================================
-- LIFEBLOOD MS - MASTER DATABASE SCRIPT
-- ==========================================

-- 1. Create the Database (if missing)
IF DB_ID('db_bloodbank') IS NULL
    CREATE DATABASE db_bloodbank;
GO

USE db_bloodbank;
GO

-- 2. Create the BloodGroups Lookup Table (if missing)
IF OBJECT_ID('dbo.BloodGroups', 'U') IS NULL
BEGIN
    CREATE TABLE BloodGroups (
        BloodGroupID INT IDENTITY(1,1) PRIMARY KEY,
        GroupName VARCHAR(3) NOT NULL UNIQUE
    );
END
GO

-- 3. Create the Main Users Table (if missing)
IF OBJECT_ID('dbo.Users', 'U') IS NULL
BEGIN
    CREATE TABLE Users (
        UserID INT IDENTITY(1,1) PRIMARY KEY,
        Username VARCHAR(50) NOT NULL UNIQUE,
        Password VARCHAR(50) NOT NULL,
        Role VARCHAR(20) NOT NULL,
        FullName VARCHAR(100) NOT NULL,
        ContactDetails VARCHAR(20) NOT NULL,
        Age INT NOT NULL,
        Address VARCHAR(255) NOT NULL,
        BloodGroup VARCHAR(3) NULL
    );
END
GO

-- 3b. Migrate Users.BloodGroup to literal values (if needed)
IF OBJECT_ID('dbo.Users', 'U') IS NOT NULL
BEGIN
    IF EXISTS (SELECT 1 FROM sys.columns WHERE object_id = OBJECT_ID('dbo.Users') AND name = 'BloodGroup' AND system_type_id = 56)
    BEGIN
        IF COL_LENGTH('dbo.Users', 'BloodGroupText') IS NULL
            ALTER TABLE dbo.Users ADD BloodGroupText VARCHAR(3) NULL;

        EXEC('UPDATE u SET BloodGroupText = bg.GroupName FROM dbo.Users u LEFT JOIN dbo.BloodGroups bg ON u.BloodGroup = bg.BloodGroupID');

        IF EXISTS (SELECT 1 FROM sys.foreign_keys WHERE parent_object_id = OBJECT_ID('dbo.Users'))
        BEGIN
            DECLARE @fkName nvarchar(128);
            SELECT TOP 1 @fkName = fk.name FROM sys.foreign_keys fk WHERE fk.parent_object_id = OBJECT_ID('dbo.Users');
            IF @fkName IS NOT NULL EXEC('ALTER TABLE dbo.Users DROP CONSTRAINT ' + @fkName);
        END

        ALTER TABLE dbo.Users DROP COLUMN BloodGroup;
        EXEC sp_rename 'dbo.Users.BloodGroupText', 'BloodGroup', 'COLUMN';
    END
END
GO

-- 4. Create BloodInventory Table (if missing)
IF OBJECT_ID('dbo.BloodInventory', 'U') IS NULL
BEGIN
    CREATE TABLE BloodInventory (
        BloodGroup VARCHAR(3) NOT NULL PRIMARY KEY,
        QuantityUnits INT NOT NULL
    );
END
GO

-- 5. Create BloodRequests Table (if missing)
IF OBJECT_ID('dbo.BloodRequests', 'U') IS NULL
BEGIN
    CREATE TABLE BloodRequests (
        RequestID INT IDENTITY(1,1) PRIMARY KEY,
        UserID INT NOT NULL,
        BloodGroup VARCHAR(3) NOT NULL,
        QuantityRequested INT NOT NULL,
        RequestStatus VARCHAR(20) NOT NULL,
        RequestDate DATETIME NOT NULL DEFAULT GETDATE(),
        FOREIGN KEY (UserID) REFERENCES Users(UserID)
    );
END
GO

-- 6. Create Donations Table (if missing)
IF OBJECT_ID('dbo.Donations', 'U') IS NULL
BEGIN
    CREATE TABLE Donations (
        DonationID INT IDENTITY(1,1) PRIMARY KEY,
        UserID INT NOT NULL,
        BloodGroup VARCHAR(3) NOT NULL,
        QuantityUnits INT NOT NULL,
        DonationDate DATETIME NOT NULL DEFAULT GETDATE(),
        ExpiryDate DATE NOT NULL,
        FOREIGN KEY (UserID) REFERENCES Users(UserID)
    );
END
GO

-- 7. Seed the 8 Universal Blood Types (skip existing)
IF NOT EXISTS (SELECT 1 FROM BloodGroups)
BEGIN
    INSERT INTO BloodGroups (GroupName) 
    VALUES ('A+'), ('A-'), ('B+'), ('B-'), ('O+'), ('O-'), ('AB+'), ('AB-');
END
GO

-- 8. Insert Admin Accounts (skip existing)
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'talha')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('talha', 'fast2026', 'Admin', 'Talha Pasha', '0328-8179714', 20, 'Lahore', 'B+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'nabeera')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('nabeera', 'pwd6', 'Admin', 'Nabeera Saqib', '0300-1234566', 19, 'FAST Campus', 'A-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'maryam')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('maryam', 'pwd8', 'Admin', 'Maryam Shafiq', '0300-1234568', 19, 'Lahore', 'O+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'adnan')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('adnan', 'pwd9', 'Admin', 'Adnan Ali', '0300-1234569', 20, 'Lahore', 'AB+');
GO

-- 9. Insert Donor and Recipient Accounts (skip existing)
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'safeer_p')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('safeer_p', 'pass1', 'Donor', 'Safeer Pasha', '0300-1111111', 22, 'DHA Phase 5', 'A+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'eman_f')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('eman_f', 'pass2', 'Recipient', 'Eman Fatima', '0311-2222222', 21, 'Johar Town', 'O+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'hamna_f')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('hamna_f', 'pass3', 'Donor', 'Hamna Faisal', '0322-3333333', 23, 'Gulberg III', 'B+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'shaaf_m')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('shaaf_m', 'pass4', 'Recipient', 'Shaaf Malik', '0333-4444444', 25, 'Model Town', 'AB+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'haseeb_t')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('haseeb_t', 'pass5', 'Donor', 'Hasseb Tahir', '0344-5555555', 20, 'Wapda Town', 'A-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'aliza_i')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('aliza_i', 'pass6', 'Recipient', 'Aliza Ijaz', '0300-6666666', 22, 'Iqbal Town', 'O-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'fizza_i')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('fizza_i', 'pass7', 'Donor', 'Fizza Ijaz', '0311-7777777', 24, 'DHA Phase 6', 'B-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'ammar_a')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('ammar_a', 'pass8', 'Recipient', 'Ammar Arif', '0322-8888888', 26, 'Johar Town', 'AB-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'bilal_k')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('bilal_k', 'pass9', 'Donor', 'Bilal Kashif', '0333-9999999', 21, 'Gulberg II', 'A+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'aliya_z')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('aliya_z', 'pass10', 'Recipient', 'Aliya Zahra', '0344-1010101', 23, 'Model Town', 'O+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'adina_s')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('adina_s', 'pass11', 'Donor', 'Adina Saqib', '0300-1212121', 20, 'Wapda Town', 'B+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'maryam_s')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('maryam_s', 'pass12', 'Recipient', 'Maryam Shafiq', '0311-1313131', 21, 'Iqbal Town', 'AB+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'areeba_i')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('areeba_i', 'pass13', 'Donor', 'Areeba Iqbal', '0322-1414141', 22, 'DHA Phase 5', 'A-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'aliza_f')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('aliza_f', 'pass14', 'Recipient', 'Aliza Fatima', '0333-1515151', 24, 'Johar Town', 'O-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'mustafa_s')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('mustafa_s', 'pass15', 'Donor', 'Mustafa Salman', '0344-1616161', 25, 'Gulberg III', 'B-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'shahzaib_z')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('shahzaib_z', 'pass16', 'Recipient', 'Shahzaib Zia', '0300-1717171', 26, 'Model Town', 'AB-');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'kashif_a')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('kashif_a', 'pass17', 'Donor', 'Kashif Abbas', '0311-1818181', 27, 'Wapda Town', 'A+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'hina_i')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('hina_i', 'pass18', 'Recipient', 'Hina Iqbal', '0322-1919191', 24, 'FAST Campus', 'O+');
IF NOT EXISTS (SELECT 1 FROM Users WHERE Username = 'saad_a')
    INSERT INTO Users (Username, Password, Role, FullName, ContactDetails, Age, Address, BloodGroup)
    VALUES ('saad_a', 'pass19', 'Donor', 'Saad Ali', '0333-2020202', 22, 'Gulberg II', 'B+');
GO



SELECT* FROM Users;


USE db_bloodbank;
SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME IN ('Donations','BloodRequests','BloodInventory');

