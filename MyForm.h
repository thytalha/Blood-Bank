#pragma once

namespace BloodBank {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Panel^ loginPanel;
	private: System::Windows::Forms::Label^ lblTitle;
	private: System::Windows::Forms::Label^ lblUsername;
	private: System::Windows::Forms::TextBox^ txtUsername;
	private: System::Windows::Forms::Label^ lblPassword;
	private: System::Windows::Forms::TextBox^ txtPassword;
	private: System::Windows::Forms::Button^ btnLogin;
	private: System::Windows::Forms::Button^ btnSignup;
	private: System::Windows::Forms::Label^ lblStatus;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->loginPanel = (gcnew System::Windows::Forms::Panel());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->btnSignup = (gcnew System::Windows::Forms::Button());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->loginPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// loginPanel
			// 
			this->loginPanel->BackColor = System::Drawing::Color::Transparent;
			this->loginPanel->Controls->Add(this->lblStatus);
			this->loginPanel->Controls->Add(this->btnSignup);
			this->loginPanel->Controls->Add(this->btnLogin);
			this->loginPanel->Controls->Add(this->txtPassword);
			this->loginPanel->Controls->Add(this->lblPassword);
			this->loginPanel->Controls->Add(this->txtUsername);
			this->loginPanel->Controls->Add(this->lblUsername);
			this->loginPanel->Controls->Add(this->lblTitle);
			this->loginPanel->Location = System::Drawing::Point(150, 100); // Temporary position
			this->loginPanel->Name = L"loginPanel";
			this->loginPanel->Size = System::Drawing::Size(400, 350);
			this->loginPanel->TabIndex = 0;
			// 
			// lblTitle
			// 
			this->lblTitle->AutoSize = true;
			this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblTitle->ForeColor = System::Drawing::Color::White;
			this->lblTitle->Location = System::Drawing::Point(45, 20);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(315, 46);
			this->lblTitle->Text = L"Admin Login Portal";
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->ForeColor = System::Drawing::Color::White;
			this->lblUsername->Location = System::Drawing::Point(50, 110);
			this->lblUsername->Text = L"Username:";
			// 
			// txtUsername
			// 
			this->txtUsername->Location = System::Drawing::Point(140, 107);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(200, 22);
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->ForeColor = System::Drawing::Color::White;
			this->lblPassword->Location = System::Drawing::Point(50, 160);
			this->lblPassword->Text = L"Password:";
			// 
			// txtPassword
			// 
			this->txtPassword->Location = System::Drawing::Point(140, 157);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->PasswordChar = '*';
			this->txtPassword->Size = System::Drawing::Size(200, 22);
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::DodgerBlue;
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogin->ForeColor = System::Drawing::Color::White;
			this->btnLogin->Location = System::Drawing::Point(70, 220);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(110, 40);
			this->btnLogin->Text = L"Login";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// btnSignup
			// 
			this->btnSignup->BackColor = System::Drawing::Color::SlateGray;
			this->btnSignup->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSignup->ForeColor = System::Drawing::Color::White;
			this->btnSignup->Location = System::Drawing::Point(210, 220);
			this->btnSignup->Name = L"btnSignup";
			this->btnSignup->Size = System::Drawing::Size(110, 40);
			this->btnSignup->Text = L"Sign Up";
			this->btnSignup->UseVisualStyleBackColor = false;
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->Location = System::Drawing::Point(70, 280);
			this->lblStatus->Name = L"lblStatus";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(2, 20, 50); // NAVY BLUE BG
			this->ClientSize = System::Drawing::Size(800, 600);
			this->Controls->Add(this->loginPanel);
			this->Name = L"MyForm";
			this->Text = L"Blood Bank Interface";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize); // Handles centering on resize
			this->loginPanel->ResumeLayout(false);
			this->loginPanel->PerformLayout();
			this->ResumeLayout(false);
		}
#pragma endregion

		// --- LOGIC TO CENTER THE PANEL ---
	private: System::Void CenterPanel() {
		int x = (this->ClientSize.Width - loginPanel->Width) / 2;
		int y = (this->ClientSize.Height - loginPanel->Height) / 2;
		loginPanel->Location = System::Drawing::Point(x, y);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		CenterPanel();
	}

	private: System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e) {
		CenterPanel();
	}

	private: System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e) {
		if (txtUsername->Text == "admin" && txtPassword->Text == "admin123") {
			MessageBox::Show("Success!", "Login", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		else {
			lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
			lblStatus->Text = "Invalid Credentials!";
		}
	}
	};
}