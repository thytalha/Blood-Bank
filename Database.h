#pragma once

#using <System.dll>
#using <System.Data.dll>
#using <System.Windows.Forms.dll>
#using <System.Xml.dll>

using namespace System;
using namespace System::Data;
using namespace System::Data::SqlClient;
using namespace System::Windows::Forms;


ref class Database sealed
{
private:
    static Database^ _instance = nullptr;
    SqlConnection^ _connection = nullptr;

    static String^ CONNECTION_STRING =
        "Server=Talha\\SQLEXPRESS;"
        "Database=db_bloodbank;"
        "Integrated Security=True;"
        "TrustServerCertificate=True;"
        "Connect Timeout=30;"
        "MultipleActiveResultSets=True;";


private:
    Database()
    {
        _connection = gcnew SqlConnection(CONNECTION_STRING);
    }


public:
    static Database^ GetInstance()
    {
        if (_instance == nullptr)
            _instance = gcnew Database();

        return _instance;
    }


public:
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
        catch (Exception^) { }
    }

    SqlConnection^ GetConnection()
    {
        OpenConnection();
        return _connection;
    }

    ConnectionState GetState()
    {
        return _connection->State;
    }

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

public:
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

public:
    bool RecordExists(SqlCommand^ cmd)
    {
        Object^ result = ExecuteScalar(cmd);
        return (result != nullptr && result != DBNull::Value);
    }

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

    bool UsernameExists(String^ username)
    {
        SqlCommand^ cmd = gcnew SqlCommand(
            "SELECT 1 FROM Users WHERE Username = @uname",
            GetConnection()
        );
        cmd->Parameters->AddWithValue("@uname", username);
        return RecordExists(cmd);
    }

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

    int CountExpiredUnits()
    {
        Object^ result = ExecuteScalar(
            "SELECT COUNT(*) FROM Donations "
            "WHERE ExpiryDate < CAST(GETDATE() AS DATE)"
        );
        return (result != nullptr) ? Convert::ToInt32(result) : 0;
    }

    int CountPendingRequests()
    {
        Object^ result = ExecuteScalar(
            "SELECT COUNT(*) FROM BloodRequests "
            "WHERE RequestStatus = 'Pending'"
        );
        return (result != nullptr) ? Convert::ToInt32(result) : 0;
    }
};