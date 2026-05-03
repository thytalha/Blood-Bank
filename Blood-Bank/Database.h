#pragma once

// ============================================================
//  Database.h  —  Blood Bank Management System
//  Singleton ADO.NET connection manager (C++/CLI)
//  Handles all SQL Server interactions via System::Data::SqlClient
// ============================================================

#using <System.dll>
#using <System.Data.dll>
#using <System.Windows.Forms.dll>
#using <System.Xml.dll>

using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::Windows::Forms;


// ────────────────────────────────────────────────────────────
//  Database  —  Singleton ref class
//  Usage (from any Form):
//      Database^ db = Database::GetInstance();
//      SqlDataReader^ rdr = db->ExecuteReader("SELECT ...");
// ────────────────────────────────────────────────────────────
ref class Database sealed
{
    // ── Private Members ─────────────────────────────────────
private:
    static Database^ _instance = nullptr;    // Singleton instance
    SqlConnection^ _connection = nullptr;    // Shared live connection

    // Connection string — Windows Authentication on TALHA\SQLEXPRESS
    static String^ CONNECTION_STRING =
        "Server=Talha\\SQLEXPRESS;"
        "Database=db_bloodbank;"
        "Integrated Security=True;"
        "TrustServerCertificate=True;"
        "Connect Timeout=30;"
        "MultipleActiveResultSets=True;";           // Allows multiple readers simultaneously


    // ── Private Constructor (Singleton pattern) ──────────────
private:
    Database()
    {
        _connection = gcnew SqlConnection(CONNECTION_STRING);
    }


    // ── Singleton Accessor ───────────────────────────────────
public:
    /// <summary>
    /// Returns the single shared Database instance.
    /// Creates it on first call (lazy initialization).
    /// </summary>
    static Database^ GetInstance()
    {
        if (_instance == nullptr)
            _instance = gcnew Database();

        return _instance;
    }


    // ── Connection Lifecycle ─────────────────────────────────
public:
    /// <summary>
    /// Opens the SQL connection if it is not already open.
    /// Returns true on success, false on failure (with UI alert).
    /// </summary>
    bool OpenConnection()
    {
        try
        {
            if (_connection->State == ConnectionState::Closed)
                _connection->Open();

            return true;
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Database Connection Error:\n\n" + ex->Message +
                "\n\nPlease verify that SQL Server (TALHA\\SQLEXPRESS) is running "
                "and the database 'db_bloodbank' exists.",
                "Connection Failed",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return false;
        }
        catch (Exception^ ex)
        {
            MessageBox::Show(
                "Unexpected error while connecting:\n\n" + ex->Message,
                "Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return false;
        }
    }

    /// <summary>
    /// Closes the SQL connection if it is currently open.
    /// </summary>
    void CloseConnection()
    {
        try
        {
            if (_connection != nullptr &&
                _connection->State == ConnectionState::Open)
            {
                _connection->Close();
            }
        }
        catch (Exception^) { /* Swallow — best effort close */ }
    }

    /// <summary>
    /// Returns the underlying SqlConnection^ for advanced use cases
    /// (e.g., transactions, DataAdapters).
    /// </summary>
    SqlConnection^ GetConnection()
    {
        OpenConnection();   // Ensure the connection is open
        return _connection;
    }

    /// <summary>
    /// Returns the current state of the connection.
    /// </summary>
    ConnectionState GetState()
    {
        return _connection->State;
    }

    /// <summary>
    /// Pings the database. Returns true if reachable.
    /// Useful on app startup to validate the connection.
    /// </summary>
    bool TestConnection()
    {
        SqlConnection^ testConn = gcnew SqlConnection(CONNECTION_STRING);
        try
        {
            testConn->Open();
            testConn->Close();
            return true;
        }
        catch (SqlException^)
        {
            return false;
        }
        finally
        {
            if (testConn->State == ConnectionState::Open)
                testConn->Close();
        }
    }


    // ── Core Query Methods ───────────────────────────────────
public:

    // ────────────────────────────────────────────────────────
    //  ExecuteNonQuery
    //  Use for: INSERT, UPDATE, DELETE, stored procedures
    //  Returns: number of rows affected, or -1 on error
    // ────────────────────────────────────────────────────────
    int ExecuteNonQuery(String^ query)
    {
        if (!OpenConnection()) return -1;

        SqlCommand^ cmd = gcnew SqlCommand(query, _connection);
        try
        {
            return cmd->ExecuteNonQuery();
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (NonQuery):\n\n" + ex->Message +
                "\n\nQuery:\n" + query,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return -1;
        }
        finally
        {
            delete cmd;
        }
    }

    // Overload — accepts a pre-built SqlCommand^ with parameters already bound
    // Use this overload to PREVENT SQL INJECTION (parameterized queries)
    int ExecuteNonQuery(SqlCommand^ cmd)
    {
        if (!OpenConnection()) return -1;

        cmd->Connection = _connection;
        try
        {
            return cmd->ExecuteNonQuery();
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (NonQuery):\n\n" + ex->Message,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return -1;
        }
    }


    // ────────────────────────────────────────────────────────
    //  ExecuteReader
    //  Use for: SELECT queries that return rows
    //  IMPORTANT: Caller MUST close the reader after use:
    //      SqlDataReader^ rdr = db->ExecuteReader("...");
    //      while (rdr->Read()) { ... }
    //      rdr->Close();
    // ────────────────────────────────────────────────────────
    SqlDataReader^ ExecuteReader(String^ query)
    {
        if (!OpenConnection()) return nullptr;

        SqlCommand^ cmd = gcnew SqlCommand(query, _connection);
        try
        {
            return cmd->ExecuteReader();
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (Reader):\n\n" + ex->Message +
                "\n\nQuery:\n" + query,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return nullptr;
        }
    }

    // Overload — parameterized SELECT (recommended for user-supplied values)
    SqlDataReader^ ExecuteReader(SqlCommand^ cmd)
    {
        if (!OpenConnection()) return nullptr;

        cmd->Connection = _connection;
        try
        {
            return cmd->ExecuteReader();
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (Reader):\n\n" + ex->Message,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return nullptr;
        }
    }


    // ────────────────────────────────────────────────────────
    //  ExecuteScalar
    //  Use for: COUNT(*), MAX(), single-cell SELECTs
    //  Returns: boxed Object^ — cast to the expected type
    //  Example:
    //      Object^ obj = db->ExecuteScalar("SELECT COUNT(*) FROM Users");
    //      int count = safe_cast<int>(obj);
    // ────────────────────────────────────────────────────────
    Object^ ExecuteScalar(String^ query)
    {
        if (!OpenConnection()) return nullptr;

        SqlCommand^ cmd = gcnew SqlCommand(query, _connection);
        try
        {
            return cmd->ExecuteScalar();
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (Scalar):\n\n" + ex->Message +
                "\n\nQuery:\n" + query,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return nullptr;
        }
        finally
        {
            delete cmd;
        }
    }

    // Overload — parameterized scalar
    Object^ ExecuteScalar(SqlCommand^ cmd)
    {
        if (!OpenConnection()) return nullptr;

        cmd->Connection = _connection;
        try
        {
            return cmd->ExecuteScalar();
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (Scalar):\n\n" + ex->Message,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return nullptr;
        }
    }


    // ────────────────────────────────────────────────────────
    //  FillDataTable
    //  Use for: Binding results directly to DataGridView
    //  Returns: DataTable^ ready for dataGridView->DataSource = dt;
    //  Example:
    //      DataTable^ dt = db->FillDataTable("SELECT * FROM Users");
    //      dataGridView1->DataSource = dt;
    // ────────────────────────────────────────────────────────
    DataTable^ FillDataTable(String^ query)
    {
        DataTable^ dt = gcnew DataTable();
        SqlCommand^ cmd = gcnew SqlCommand(query, GetConnection());
        SqlDataAdapter^ adapter = gcnew SqlDataAdapter(cmd);

        try
        {
            adapter->Fill(dt);
            return dt;
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (FillDataTable):\n\n" + ex->Message +
                "\n\nQuery:\n" + query,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return nullptr;
        }
        finally
        {
            delete adapter;
            delete cmd;
        }
    }

    // Overload — parameterized FillDataTable
    DataTable^ FillDataTable(SqlCommand^ cmd)
    {
        DataTable^ dt = gcnew DataTable();
        cmd->Connection = GetConnection();
        SqlDataAdapter^ adapter = gcnew SqlDataAdapter(cmd);

        try
        {
            adapter->Fill(dt);
            return dt;
        }
        catch (SqlException^ ex)
        {
            MessageBox::Show(
                "Query Error (FillDataTable):\n\n" + ex->Message,
                "SQL Error",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return nullptr;
        }
        finally
        {
            delete adapter;
        }
    }


    // ── Convenience Helpers ──────────────────────────────────
public:

    /// <summary>
    /// Checks whether a record exists.
    /// Example: db->RecordExists(cmd) where cmd SELECTs 1 row
    /// </summary>
    bool RecordExists(SqlCommand^ cmd)
    {
        Object^ result = ExecuteScalar(cmd);
        return (result != nullptr && result != DBNull::Value);
    }

    /// <summary>
    /// Validates login credentials.
    /// Sets outRole, outUserID, and outFullName on success.
    /// Returns true if credentials matched a Users row.
    /// USE THIS in the Login form.
    /// </summary>
    bool ValidateLogin(String^ username, String^ password,
        String^% outRole, int% outUserID, String^% outFullName)
    {
        SqlCommand^ cmd = gcnew SqlCommand(
            "SELECT UserID, Role, FullName "
            "FROM   Users "
            "WHERE  Username = @uname AND [Password] = @pwd",
            GetConnection()
        );
        cmd->Parameters->AddWithValue("@uname", username);
        cmd->Parameters->AddWithValue("@pwd", password);

        SqlDataReader^ rdr = ExecuteReader(cmd);
        if (rdr == nullptr) return false;

        bool found = false;
        try
        {
            if (rdr->Read())
            {
                found = true;
                outUserID = Convert::ToInt32(rdr["UserID"]);
                outRole = rdr["Role"]->ToString();
                outFullName = rdr["FullName"]->ToString();
            }
        }
        finally
        {
            rdr->Close();
        }
        return found;
    }

    /// <summary>
    /// Returns true if the given username is already taken in Users.
    /// USE THIS in the Signup / Registration form.
    /// </summary>
    bool UsernameExists(String^ username)
    {
        SqlCommand^ cmd = gcnew SqlCommand(
            "SELECT 1 FROM Users WHERE Username = @uname",
            GetConnection()
        );
        cmd->Parameters->AddWithValue("@uname", username);
        return RecordExists(cmd);
    }

    /// <summary>
    /// Returns current stock units for a given BloodGroup string (e.g. "A+").
    /// Returns -1 if blood type not found in BloodInventory.
    /// </summary>
    int GetBloodStock(String^ bloodGroup)
    {
        SqlCommand^ cmd = gcnew SqlCommand(
            "SELECT QuantityUnits FROM BloodInventory WHERE BloodGroup = @bg",
            GetConnection()
        );
        cmd->Parameters->AddWithValue("@bg", bloodGroup);
        Object^ result = ExecuteScalar(cmd);

        if (result == nullptr || result == DBNull::Value)
            return -1;

        return Convert::ToInt32(result);
    }

    /// <summary>
    /// Counts donations whose ExpiryDate is before today.
    /// Used by the Admin dashboard for the expiry alert badge.
    /// </summary>
    int CountExpiredUnits()
    {
        Object^ result = ExecuteScalar(
            "SELECT COUNT(*) FROM Donations "
            "WHERE ExpiryDate < CAST(GETDATE() AS DATE)"
        );
        return (result != nullptr) ? Convert::ToInt32(result) : 0;
    }

    /// <summary>
    /// Counts BloodRequests with RequestStatus = 'Pending'.
    /// Used by the Admin dashboard for the pending-requests badge.
    /// </summary>
    int CountPendingRequests()
    {
        Object^ result = ExecuteScalar(
            "SELECT COUNT(*) FROM BloodRequests "
            "WHERE RequestStatus = 'Pending'"
        );
        return (result != nullptr) ? Convert::ToInt32(result) : 0;
    }
};

// ============================================================
//  END OF Database.h
//
//  QUICK REFERENCE — HOW TO USE IN ANY FORM:
//
//  Step 1 — Include at the top of every form header:
//      #include "Database.h"
//
//  Step 2 — Get the singleton (does NOT open a new connection):
//      Database^ db = Database::GetInstance();
//
//  ─── Common Patterns ────────────────────────────────────────
//
//  A) Bind a SELECT to a DataGridView:
//      dataGridView1->DataSource =
//          db->FillDataTable("SELECT * FROM Users");
//
//  B) Parameterized INSERT (SQL-injection safe):
//      SqlCommand^ cmd = gcnew SqlCommand(
//          "INSERT INTO Users (Username,[Password],UserRole,FullName,"
//          "ContactDetails,Age,[Address]) "
//          "VALUES (@u,@p,@r,@fn,@cd,@age,@addr)");
//      cmd->Parameters->AddWithValue("@u",    txtUsername->Text);
//      cmd->Parameters->AddWithValue("@p",    txtPassword->Text);
//      cmd->Parameters->AddWithValue("@r",    "Donor");
//      cmd->Parameters->AddWithValue("@fn",   txtFullName->Text);
//      cmd->Parameters->AddWithValue("@cd",   txtContact->Text);
//      cmd->Parameters->AddWithValue("@age",  Convert::ToInt32(txtAge->Text));
//      cmd->Parameters->AddWithValue("@addr", txtAddress->Text);
//      db->ExecuteNonQuery(cmd);
//
//  C) Login validation (used in LoginForm):
//      String^ role; int uid; String^ fullName;
//      if (db->ValidateLogin(txtUser->Text, txtPass->Text,
//                            role, uid, fullName))
//      {
//          // route to correct dashboard based on role
//      }
//
//  D) Startup connection test (LoginForm constructor):
//      if (!Database::GetInstance()->TestConnection())
//          MessageBox::Show("Cannot reach SQL Server. Check SQLEXPRESS.");
//
// ============================================================