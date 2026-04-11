#pragma once

namespace BloodBank
{

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

	private: System::Windows::Forms::Panel^ leftPanel;
	private: System::Windows::Forms::Panel^ rightPanel;
	private: System::Windows::Forms::Label^ lblLoginTitle;
	private: System::Windows::Forms::Label^ lblLoginSubTitle;
	private: System::Windows::Forms::Label^ lblUsername;
	private: System::Windows::Forms::TextBox^ txtUsername;
	private: System::Windows::Forms::Label^ lblPassword;
	private: System::Windows::Forms::TextBox^ txtPassword;
	private: System::Windows::Forms::Button^ btnLogin;
	private: System::Windows::Forms::Label^ lblNoAccount;
	private: System::Windows::Forms::Button^ btnSignup;
	private: System::Windows::Forms::Label^ lblStatus;
	private: System::Windows::Forms::Label^ lblWelcome;
	private: System::Windows::Forms::Label^ lblPortal;
	private: System::Windows::Forms::Panel^ lineUsername;
	private: System::Windows::Forms::Panel^ linePassword;
	private: System::Windows::Forms::Button^ btnTogglePassword;
	private: bool isPasswordVisible;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->leftPanel = (gcnew System::Windows::Forms::Panel());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->btnSignup = (gcnew System::Windows::Forms::Button());
			this->lblNoAccount = (gcnew System::Windows::Forms::Label());
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->lblLoginSubTitle = (gcnew System::Windows::Forms::Label());
			this->lblLoginTitle = (gcnew System::Windows::Forms::Label());
			this->rightPanel = (gcnew System::Windows::Forms::Panel());
			this->lblPortal = (gcnew System::Windows::Forms::Label());
			this->lblWelcome = (gcnew System::Windows::Forms::Label());
           this->lineUsername = (gcnew System::Windows::Forms::Panel());
			this->linePassword = (gcnew System::Windows::Forms::Panel());
			this->btnTogglePassword = (gcnew System::Windows::Forms::Button());
			this->leftPanel->SuspendLayout();
			this->rightPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// leftPanel
			// 
			this->leftPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(16)), static_cast<System::Int32>(static_cast<System::Byte>(18)),
				static_cast<System::Int32>(static_cast<System::Byte>(27)));
			this->leftPanel->Controls->Add(this->lblStatus);
			this->leftPanel->Controls->Add(this->btnSignup);
			this->leftPanel->Controls->Add(this->lblNoAccount);
			this->leftPanel->Controls->Add(this->btnLogin);
          this->leftPanel->Controls->Add(this->btnTogglePassword);
			this->leftPanel->Controls->Add(this->linePassword);
			this->leftPanel->Controls->Add(this->lineUsername);
			this->leftPanel->Controls->Add(this->txtPassword);
			this->leftPanel->Controls->Add(this->lblPassword);
			this->leftPanel->Controls->Add(this->txtUsername);
			this->leftPanel->Controls->Add(this->lblUsername);
			this->leftPanel->Controls->Add(this->lblLoginSubTitle);
			this->leftPanel->Controls->Add(this->lblLoginTitle);
			this->leftPanel->Dock = System::Windows::Forms::DockStyle::Left;
			this->leftPanel->Location = System::Drawing::Point(0, 0);
			this->leftPanel->Name = L"leftPanel";
			this->leftPanel->Size = System::Drawing::Size(950, 800);
			this->leftPanel->TabIndex = 0;
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
            this->lblStatus->Location = System::Drawing::Point(70, 597);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 20);
			this->lblStatus->TabIndex = 9;
			// 
			// btnSignup
			// 
			this->btnSignup->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(34)), static_cast<System::Int32>(static_cast<System::Byte>(37)),
				static_cast<System::Int32>(static_cast<System::Byte>(49)));
			this->btnSignup->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSignup->FlatAppearance->BorderSize = 0;
			this->btnSignup->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSignup->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->btnSignup->ForeColor = System::Drawing::Color::White;
           this->btnSignup->Location = System::Drawing::Point(285, 685);
			this->btnSignup->Name = L"btnSignup";
			this->btnSignup->Size = System::Drawing::Size(110, 38);
			this->btnSignup->TabIndex = 8;
			this->btnSignup->Text = L"Sign up";
			this->btnSignup->UseVisualStyleBackColor = false;
			this->btnSignup->Click += gcnew System::EventHandler(this, &MyForm::btnSignup_Click);
			// 
			// lblNoAccount
			// 
			this->lblNoAccount->AutoSize = true;
			this->lblNoAccount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->lblNoAccount->ForeColor = System::Drawing::Color::Silver;
         this->lblNoAccount->Location = System::Drawing::Point(70, 693);
			this->lblNoAccount->Name = L"lblNoAccount";
			this->lblNoAccount->Size = System::Drawing::Size(171, 20);
			this->lblNoAccount->TabIndex = 7;
			this->lblNoAccount->Text = L"Don\'t have an account?";
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(137)), static_cast<System::Int32>(static_cast<System::Byte>(95)),
				static_cast<System::Int32>(static_cast<System::Byte>(232)));
			this->btnLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogin->FlatAppearance->BorderSize = 0;
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 14));
			this->btnLogin->ForeColor = System::Drawing::Color::White;
         this->btnLogin->Location = System::Drawing::Point(70, 527);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(325, 44);
			this->btnLogin->TabIndex = 6;
			this->btnLogin->Text = L"Login";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// txtPassword
			// 
			this->txtPassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(26)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
          this->txtPassword->BorderStyle = System::Windows::Forms::BorderStyle::None;
           this->txtPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
           this->txtPassword->ForeColor = System::Drawing::Color::Silver;
          this->txtPassword->Location = System::Drawing::Point(70, 449);
			this->txtPassword->Name = L"txtPassword";
          this->txtPassword->Size = System::Drawing::Size(260, 32);
			this->txtPassword->TabIndex = 5;
         this->txtPassword->Text = L"Password";
			this->txtPassword->Enter += gcnew System::EventHandler(this, &MyForm::txtPassword_Enter);
			this->txtPassword->Leave += gcnew System::EventHandler(this, &MyForm::txtPassword_Leave);
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
            this->lblPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->lblPassword->ForeColor = System::Drawing::Color::LightGray;
          this->lblPassword->Location = System::Drawing::Point(70, 419);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(70, 20);
			this->lblPassword->TabIndex = 4;
			this->lblPassword->Text = L"Password";
         this->lblPassword->Visible = false;
			// 
			// txtUsername
			// 
			this->txtUsername->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(26)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
          this->txtUsername->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->txtUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
           this->txtUsername->ForeColor = System::Drawing::Color::Silver;
          this->txtUsername->Location = System::Drawing::Point(70, 364);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(325, 32);
			this->txtUsername->TabIndex = 3;
         this->txtUsername->Text = L"Username";
			this->txtUsername->Enter += gcnew System::EventHandler(this, &MyForm::txtUsername_Enter);
			this->txtUsername->Leave += gcnew System::EventHandler(this, &MyForm::txtUsername_Leave);
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
            this->lblUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->lblUsername->ForeColor = System::Drawing::Color::LightGray;
          this->lblUsername->Location = System::Drawing::Point(70, 334);
			this->lblUsername->Name = L"lblUsername";
			this->lblUsername->Size = System::Drawing::Size(184, 20);
			this->lblUsername->TabIndex = 2;
			this->lblUsername->Text = L"Username";
         this->lblUsername->Visible = false;
			// 
			// lblLoginSubTitle
			// 
			this->lblLoginSubTitle->AutoSize = true;
           this->lblLoginSubTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->lblLoginSubTitle->ForeColor = System::Drawing::Color::Silver;
			this->lblLoginSubTitle->Location = System::Drawing::Point(70, 225);
			this->lblLoginSubTitle->Name = L"lblLoginSubTitle";
			this->lblLoginSubTitle->Size = System::Drawing::Size(158, 20);
			this->lblLoginSubTitle->TabIndex = 1;
			this->lblLoginSubTitle->Text = L"Enter your account details";
			// 
			// lblLoginTitle
			// 
           this->lblLoginTitle->AutoSize = false;
          this->lblLoginTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 32, System::Drawing::FontStyle::Bold));
			this->lblLoginTitle->ForeColor = System::Drawing::Color::White;
			this->lblLoginTitle->Location = System::Drawing::Point(62, 140);
			this->lblLoginTitle->Name = L"lblLoginTitle";
         this->lblLoginTitle->Size = System::Drawing::Size(320, 92);
			this->lblLoginTitle->TabIndex = 0;
			this->lblLoginTitle->Text = L"Login";
         this->lblLoginTitle->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->lblLoginTitle->UseCompatibleTextRendering = true;
			// 
			// rightPanel
			// 
			this->rightPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(88)),
				static_cast<System::Int32>(static_cast<System::Byte>(224)));
			this->rightPanel->Controls->Add(this->lblPortal);
			this->rightPanel->Controls->Add(this->lblWelcome);
			this->rightPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rightPanel->Location = System::Drawing::Point(950, 0);
			this->rightPanel->Name = L"rightPanel";
			this->rightPanel->Size = System::Drawing::Size(740, 800);
			this->rightPanel->TabIndex = 1;
			// 
			// lblPortal
			// 
           this->lblPortal->AutoSize = false;
         this->lblPortal->Font = (gcnew System::Drawing::Font(L"Segoe UI", 32));
			this->lblPortal->ForeColor = System::Drawing::Color::White;
            this->lblPortal->Location = System::Drawing::Point(74, 285);
			this->lblPortal->Name = L"lblPortal";
         this->lblPortal->Size = System::Drawing::Size(560, 92);
			this->lblPortal->TabIndex = 1;
			this->lblPortal->Text = L"login portal";
         this->lblPortal->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->lblPortal->UseCompatibleTextRendering = true;
			// 
			// lblWelcome
			// 
          this->lblWelcome->AutoSize = false;
         this->lblWelcome->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 38, System::Drawing::FontStyle::Bold));
			this->lblWelcome->ForeColor = System::Drawing::Color::White;
			this->lblWelcome->Location = System::Drawing::Point(74, 198);
			this->lblWelcome->Name = L"lblWelcome";
            this->lblWelcome->Size = System::Drawing::Size(560, 110);
			this->lblWelcome->TabIndex = 0;
			this->lblWelcome->Text = L"Welcome to";
         this->lblWelcome->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->lblWelcome->UseCompatibleTextRendering = true;
         // 
			// lineUsername
			// 
			this->lineUsername->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(90)), static_cast<System::Int32>(static_cast<System::Byte>(95)),
				static_cast<System::Int32>(static_cast<System::Byte>(110)));
			this->lineUsername->Location = System::Drawing::Point(70, 401);
			this->lineUsername->Name = L"lineUsername";
			this->lineUsername->Size = System::Drawing::Size(325, 1);
			this->lineUsername->TabIndex = 10;
			// 
			// linePassword
			// 
			this->linePassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(90)), static_cast<System::Int32>(static_cast<System::Byte>(95)),
				static_cast<System::Int32>(static_cast<System::Byte>(110)));
			this->linePassword->Location = System::Drawing::Point(70, 486);
			this->linePassword->Name = L"linePassword";
			this->linePassword->Size = System::Drawing::Size(325, 1);
			this->linePassword->TabIndex = 11;
			// 
			// btnTogglePassword
			// 
			this->btnTogglePassword->BackColor = System::Drawing::Color::Transparent;
			this->btnTogglePassword->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnTogglePassword->FlatAppearance->BorderSize = 0;
			this->btnTogglePassword->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
          this->btnTogglePassword->Font = (gcnew System::Drawing::Font(L"Segoe UI Emoji", 11));
			this->btnTogglePassword->ForeColor = System::Drawing::Color::Silver;
			this->btnTogglePassword->Location = System::Drawing::Point(334, 444);
			this->btnTogglePassword->Name = L"btnTogglePassword";
			this->btnTogglePassword->Size = System::Drawing::Size(61, 35);
			this->btnTogglePassword->TabIndex = 12;
            this->btnTogglePassword->Text = L"👁";
			this->btnTogglePassword->UseVisualStyleBackColor = false;
			this->btnTogglePassword->Click += gcnew System::EventHandler(this, &MyForm::btnTogglePassword_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1200, 800);
			this->Controls->Add(this->rightPanel);
			this->Controls->Add(this->leftPanel);
			this->Name = L"MyForm";
			this->Text = L"Blood Bank Management System";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
           this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->leftPanel->ResumeLayout(false);
			this->leftPanel->PerformLayout();
			this->rightPanel->ResumeLayout(false);
			this->rightPanel->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void ArrangeLeftPanelControls()
	{
		int fieldWidth = txtUsername->Width;
		int x = (leftPanel->ClientSize.Width - fieldWidth) / 2;

		lblLoginTitle->Left = x;
		lblLoginSubTitle->Left = x;
		lblUsername->Left = x;
		txtUsername->Left = x;
      lineUsername->Left = x;
		lineUsername->Width = fieldWidth;
		lblPassword->Left = x;
		txtPassword->Left = x;
     linePassword->Left = x;
		linePassword->Width = fieldWidth;
		btnTogglePassword->Left = x + fieldWidth - btnTogglePassword->Width;
		btnTogglePassword->Top = txtPassword->Top - 5;
		txtPassword->Width = fieldWidth - btnTogglePassword->Width - 8;
		btnLogin->Left = x;
		lblStatus->Left = x;

		lblNoAccount->Left = x;
        btnSignup->Left = lblNoAccount->Right + 14;
		btnSignup->Top = lblNoAccount->Top - ((btnSignup->Height - lblNoAccount->Height) / 2);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		ArrangeLeftPanelControls();
	}

	private: System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e)
	{
		ArrangeLeftPanelControls();
	}

	private: System::Void txtUsername_Enter(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtUsername->Text == L"Username")
		{
			txtUsername->Text = L"";
			txtUsername->ForeColor = System::Drawing::Color::White;
		}
	}

	private: System::Void txtUsername_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(txtUsername->Text))
		{
			txtUsername->Text = L"Username";
			txtUsername->ForeColor = System::Drawing::Color::Silver;
		}
	}

	private: System::Void txtPassword_Enter(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtPassword->Text == L"Password" && txtPassword->ForeColor == System::Drawing::Color::Silver)
		{
			txtPassword->Text = L"";
			txtPassword->ForeColor = System::Drawing::Color::White;
			txtPassword->UseSystemPasswordChar = !isPasswordVisible;
		}
	}

	private: System::Void txtPassword_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(txtPassword->Text))
		{
			txtPassword->UseSystemPasswordChar = false;
			txtPassword->Text = L"Password";
			txtPassword->ForeColor = System::Drawing::Color::Silver;
          btnTogglePassword->Text = L"👁";
		}
	}

	private: System::Void btnTogglePassword_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtPassword->Text == L"Password" && txtPassword->ForeColor == System::Drawing::Color::Silver)
		{
			return;
		}

		isPasswordVisible = !isPasswordVisible;
		txtPassword->UseSystemPasswordChar = !isPasswordVisible;
        btnTogglePassword->Text = isPasswordVisible ? L"🙈" : L"👁";
	}

	private: System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
	{
		String^ username = txtUsername->Text->Trim();
		String^ password = txtPassword->Text;
		lblStatus->Text = L"";

		if (username == L"Username") username = L"";
		if (password == L"Password" && txtPassword->ForeColor == System::Drawing::Color::Silver) password = L"";

		if (String::IsNullOrWhiteSpace(username) || String::IsNullOrWhiteSpace(password))
		{
			lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
			lblStatus->Text = L"Please enter username and password.";
			return;
		}

		if (username->Equals(L"ADMIN", StringComparison::Ordinal))
		{
			if (password->Equals(L"admin123", StringComparison::Ordinal))
			{
				MessageBox::Show(L"Admin verified. Welcome, ADMIN.", L"Access Granted",
					MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
			else
			{
				lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
				lblStatus->Text = L"Invalid admin password.";
			}
			return;
		}

		for each (wchar_t ch in username)
		{
			if (Char::IsLetter(ch) && !Char::IsLower(ch))
			{
				lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
				lblStatus->Text = L"Users must enter username in lowercase letters.";
				return;
			}
		}

		MessageBox::Show(L"User login successful.", L"Login",
			MessageBoxButtons::OK, MessageBoxIcon::Information);
	}

	private: System::Void btnSignup_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MessageBox::Show(L"Signup is for new users. Registration form will open here.", L"Sign Up",
			MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
	};
}
