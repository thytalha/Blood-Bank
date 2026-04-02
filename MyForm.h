#pragma once

namespace BloodBank {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

		// --- NEW UI COMPONENTS ---
	private: System::Windows::Forms::Label^ lblTitle;
	private: System::Windows::Forms::Label^ lblUsername;
	private: System::Windows::Forms::TextBox^ txtUsername;
	private: System::Windows::Forms::Label^ lblPassword;
	private: System::Windows::Forms::TextBox^ txtPassword;
	private: System::Windows::Forms::Button^ btnLogin;
	private: System::Windows::Forms::Button^ btnSignup;
	private: System::Windows::Forms::Label^ lblStatus;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->btnSignup = (gcnew System::Windows::Forms::Button());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();

			// 
			// lblTitle
			// 
			this->lblTitle->AutoSize = true;
			this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->lblTitle->Location = System::Drawing::Point(55, 25);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(262, 38);
			this->lblTitle->TabIndex = 0;
			this->lblTitle->Text = L"Admin Login Portal";
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Location = System::Drawing::Point(40, 100);
			this->lblUsername->Name = L"lblUsername";
			this->lblUsername->Size = System::Drawing::Size(73, 16);
			this->lblUsername->TabIndex = 1;
			this->lblUsername->Text = L"Username:";
			// 
			// txtUsername
			// 
			this->txtUsername->Location = System::Drawing::Point(130, 97);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(170, 22);
			this->txtUsername->TabIndex = 2;
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Location = System::Drawing::Point(40, 150);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(70, 16);
			this->lblPassword->TabIndex = 3;
			this->lblPassword->Text = L"Password:";
			// 
			// txtPassword
			// 
			this->txtPassword->Location = System::Drawing::Point(130, 147);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->PasswordChar = '*'; // Masks the password text
			this->txtPassword->Size = System::Drawing::Size(170, 22);
			this->txtPassword->TabIndex = 4;
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::LightBlue;
			this->btnLogin->Location = System::Drawing::Point(60, 210);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(100, 35);
			this->btnLogin->TabIndex = 5;
			this->btnLogin->Text = L"Login";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// btnSignup
			// 
			this->btnSignup->BackColor = System::Drawing::Color::LightGray;
			this->btnSignup->Location = System::Drawing::Point(180, 210);
			this->btnSignup->Name = L"btnSignup";
			this->btnSignup->Size = System::Drawing::Size(100, 35);
			this->btnSignup->TabIndex = 6;
			this->btnSignup->Text = L"Sign Up";
			this->btnSignup->UseVisualStyleBackColor = false;
			this->btnSignup->Click += gcnew System::EventHandler(this, &MyForm::btnSignup_Click);
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->Location = System::Drawing::Point(60, 260);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 16);
			this->lblStatus->TabIndex = 7;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(360, 320);
			this->Controls->Add(this->lblStatus);
			this->Controls->Add(this->btnSignup);
			this->Controls->Add(this->btnLogin);
			this->Controls->Add(this->txtPassword);
			this->Controls->Add(this->lblPassword);
			this->Controls->Add(this->txtUsername);
			this->Controls->Add(this->lblUsername);
			this->Controls->Add(this->lblTitle);
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen; // Centers the form on launch
			this->Text = L"Blood Bank Interface";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		// --- EVENT HANDLERS ---

	private: System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e) {
		// Retrieve text from textboxes
		String^ username = txtUsername->Text;
		String^ password = txtPassword->Text;

		// Basic validation example (Replace this with database logic later)
		if (username == "admin" && password == "admin123") {
			lblStatus->ForeColor = System::Drawing::Color::Green;
			lblStatus->Text = "Login Successful! Welcome Admin.";

			MessageBox::Show("Connected to Blood Bank Admin Dashboard!", "Success",
				MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		else {
			lblStatus->ForeColor = System::Drawing::Color::Red;
			lblStatus->Text = "Invalid Username or Password.";
		}
	}

	private: System::Void btnSignup_Click(System::Object^ sender, System::EventArgs^ e) {
		MessageBox::Show("Sign Up functionality will be implemented soon.", "Information",
			MessageBoxButtons::OK, MessageBoxIcon::Information);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		// Code to run when the form first loads
	}
	};
}