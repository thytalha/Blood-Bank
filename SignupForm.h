#pragma once

namespace BloodBank
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::Collections::Generic;
	using namespace System::Text::RegularExpressions;

	public ref class SignupForm : public Form
	{
	private:
		ref class UserAccount
		{
		public:
			String^ FirstName;
			String^ SecondName;
			String^ Phone;
			String^ Email;
			String^ Username;
			String^ Password;
		};

		static List<UserAccount^>^ users = gcnew List<UserAccount^>();

		Panel^ mainPanel;
		Label^ lblLogo;
		Label^ lblTitle;
		Label^ lblSubTitle;

		Label^ lblFirstName;
		TextBox^ txtFirstName;

		Label^ lblSecondName;
		TextBox^ txtSecondName;

		Label^ lblPhone;
		TextBox^ txtPhone;

		Label^ lblEmail;
		TextBox^ txtEmail;

		Label^ lblUsername;
		TextBox^ txtUsername;

		Label^ lblPassword;
		TextBox^ txtPassword;

		Label^ lblConfirmPassword;
		TextBox^ txtConfirmPassword;

		Button^ btnCreateAccount;
		Button^ btnBack;
		Label^ lblStatus;

	public:
		SignupForm()
		{
			InitializeComponent();
		}

	private:
		void InitializeComponent()
		{
			this->Text = L"LifeBlood MMS - Sign Up";
			this->ClientSize = System::Drawing::Size(700, 850);
			this->StartPosition = FormStartPosition::CenterScreen;
			this->BackColor = Color::FromArgb(248, 249, 252);

			mainPanel = gcnew Panel();
			mainPanel->BackColor = Color::White;
			mainPanel->Location = Point(70, 30);
			mainPanel->Size = System::Drawing::Size(560, 780);

			lblLogo = gcnew Label();
			lblLogo->BackColor = Color::FromArgb(255, 230, 235);
			lblLogo->ForeColor = Color::FromArgb(232, 15, 59);
			lblLogo->Font = gcnew Drawing::Font(L"Segoe UI Symbol", 18, FontStyle::Regular);
			lblLogo->Text = L"💧";
			lblLogo->TextAlign = ContentAlignment::MiddleCenter;
			lblLogo->Location = Point(28, 24);
			lblLogo->Size = System::Drawing::Size(46, 46);

			lblTitle = gcnew Label();
			lblTitle->AutoSize = true;
			lblTitle->Font = gcnew Drawing::Font(L"Calisto MT", 24, FontStyle::Bold);
			lblTitle->ForeColor = Color::Black;
			lblTitle->Text = L"Create your account";
			lblTitle->Location = Point(28, 90);

			lblSubTitle = gcnew Label();
			lblSubTitle->AutoSize = true;
			lblSubTitle->Font = gcnew Drawing::Font(L"Calisto MT", 11, FontStyle::Regular);
			lblSubTitle->ForeColor = Color::Gray;
			lblSubTitle->Text = L"Join LifeBlood Blood Bank Management System";
			lblSubTitle->Location = Point(30, 132);

			int x = 34;
			int w = 490;
			int y = 185;
			int gap = 78;

			lblFirstName = gcnew Label();
			lblFirstName->Text = L"First Name *";
			lblFirstName->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblFirstName->Location = Point(x, y);

			txtFirstName = gcnew TextBox();
			txtFirstName->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtFirstName->Location = Point(x, y + 24);
			txtFirstName->Size = System::Drawing::Size(w, 28);

			y += gap;
			lblSecondName = gcnew Label();
			lblSecondName->Text = L"Second Name (Optional)";
			lblSecondName->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblSecondName->Location = Point(x, y);

			txtSecondName = gcnew TextBox();
			txtSecondName->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtSecondName->Location = Point(x, y + 24);
			txtSecondName->Size = System::Drawing::Size(w, 28);

			y += gap;
			lblPhone = gcnew Label();
			lblPhone->Text = L"Phone Number (+92...) *";
			lblPhone->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblPhone->Location = Point(x, y);

			txtPhone = gcnew TextBox();
			txtPhone->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtPhone->Location = Point(x, y + 24);
			txtPhone->Size = System::Drawing::Size(w, 28);
			txtPhone->Text = L"+92";

			y += gap;
			lblEmail = gcnew Label();
			lblEmail->Text = L"Email *";
			lblEmail->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblEmail->Location = Point(x, y);

			txtEmail = gcnew TextBox();
			txtEmail->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtEmail->Location = Point(x, y + 24);
			txtEmail->Size = System::Drawing::Size(w, 28);

			y += gap;
			lblUsername = gcnew Label();
			lblUsername->Text = L"Username *";
			lblUsername->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblUsername->Location = Point(x, y);

			txtUsername = gcnew TextBox();
			txtUsername->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtUsername->Location = Point(x, y + 24);
			txtUsername->Size = System::Drawing::Size(w, 28);

			y += gap;
			lblPassword = gcnew Label();
			lblPassword->Text = L"Password *";
			lblPassword->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblPassword->Location = Point(x, y);

			txtPassword = gcnew TextBox();
			txtPassword->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtPassword->Location = Point(x, y + 24);
			txtPassword->Size = System::Drawing::Size(w, 28);
			txtPassword->UseSystemPasswordChar = true;

			y += gap;
			lblConfirmPassword = gcnew Label();
			lblConfirmPassword->Text = L"Confirm Password *";
			lblConfirmPassword->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblConfirmPassword->Location = Point(x, y);

			txtConfirmPassword = gcnew TextBox();
			txtConfirmPassword->Font = gcnew Drawing::Font(L"Calisto MT", 11);
			txtConfirmPassword->Location = Point(x, y + 24);
			txtConfirmPassword->Size = System::Drawing::Size(w, 28);
			txtConfirmPassword->UseSystemPasswordChar = true;

			lblStatus = gcnew Label();
			lblStatus->AutoSize = true;
			lblStatus->ForeColor = Color::FromArgb(210, 40, 40);
			lblStatus->Font = gcnew Drawing::Font(L"Calisto MT", 10, FontStyle::Bold);
			lblStatus->Location = Point(x, 700);

			btnCreateAccount = gcnew Button();
			btnCreateAccount->Text = L"Create Account";
			btnCreateAccount->Font = gcnew Drawing::Font(L"Calisto MT", 12, FontStyle::Bold);
			btnCreateAccount->BackColor = Color::FromArgb(232, 15, 59);
			btnCreateAccount->ForeColor = Color::White;
			btnCreateAccount->FlatStyle = FlatStyle::Flat;
			btnCreateAccount->FlatAppearance->BorderSize = 0;
			btnCreateAccount->Location = Point(34, 730);
			btnCreateAccount->Size = System::Drawing::Size(300, 40);
			btnCreateAccount->Click += gcnew EventHandler(this, &SignupForm::btnCreateAccount_Click);

			btnBack = gcnew Button();
			btnBack->Text = L"Back to Login";
			btnBack->Font = gcnew Drawing::Font(L"Calisto MT", 12, FontStyle::Bold);
			btnBack->BackColor = Color::FromArgb(241, 243, 247);
			btnBack->ForeColor = Color::FromArgb(60, 60, 60);
			btnBack->FlatStyle = FlatStyle::Flat;
			btnBack->FlatAppearance->BorderSize = 0;
			btnBack->Location = Point(354, 730);
			btnBack->Size = System::Drawing::Size(170, 40);
			btnBack->Click += gcnew EventHandler(this, &SignupForm::btnBack_Click);

			this->FormClosed += gcnew FormClosedEventHandler(this, &SignupForm::SignupForm_FormClosed);

			mainPanel->Controls->Add(lblLogo);
			mainPanel->Controls->Add(lblTitle);
			mainPanel->Controls->Add(lblSubTitle);
			mainPanel->Controls->Add(lblFirstName);
			mainPanel->Controls->Add(txtFirstName);
			mainPanel->Controls->Add(lblSecondName);
			mainPanel->Controls->Add(txtSecondName);
			mainPanel->Controls->Add(lblPhone);
			mainPanel->Controls->Add(txtPhone);
			mainPanel->Controls->Add(lblEmail);
			mainPanel->Controls->Add(txtEmail);
			mainPanel->Controls->Add(lblUsername);
			mainPanel->Controls->Add(txtUsername);
			mainPanel->Controls->Add(lblPassword);
			mainPanel->Controls->Add(txtPassword);
			mainPanel->Controls->Add(lblConfirmPassword);
			mainPanel->Controls->Add(txtConfirmPassword);
			mainPanel->Controls->Add(lblStatus);
			mainPanel->Controls->Add(btnCreateAccount);
			mainPanel->Controls->Add(btnBack);

			this->Controls->Add(mainPanel);
		}

		bool IsValidPakPhone(String^ phone)
		{
			Regex^ r = gcnew Regex(L"^\\+92(3\\d{9})$");
			return r->IsMatch(phone);
		}

		bool IsValidEmailProvider(String^ email)
		{
			Regex^ syntax = gcnew Regex(L"^[A-Za-z0-9._%+-]+@([A-Za-z0-9-]+\\.)+[A-Za-z]{2,}$");
			if (!syntax->IsMatch(email)) return false;

			array<String^>^ allowed = gcnew array<String^> {
				L"gmail.com", L"outlook.com", L"hotmail.com", L"yahoo.com", L"icloud.com", L"proton.me"
			};

			String^ domain = email->Substring(email->LastIndexOf('@') + 1)->ToLowerInvariant();
			for each (String ^ d in allowed)
			{
				if (domain == d) return true;
			}
			return false;
		}

		bool UsernameExists(String^ username)
		{
			for each (UserAccount ^ u in users)
			{
				if (String::Compare(u->Username, username, true) == 0) return true;
			}
			return false;
		}

		bool EmailExists(String^ email)
		{
			for each (UserAccount ^ u in users)
			{
				if (String::Compare(u->Email, email, true) == 0) return true;
			}
			return false;
		}

		System::Void btnCreateAccount_Click(System::Object^ sender, System::EventArgs^ e)
		{
			String^ first = txtFirstName->Text->Trim();
			String^ second = txtSecondName->Text->Trim();
			String^ phone = txtPhone->Text->Trim();
			String^ email = txtEmail->Text->Trim();
			String^ username = txtUsername->Text->Trim();
			String^ password = txtPassword->Text;
			String^ confirm = txtConfirmPassword->Text;

			if (String::IsNullOrWhiteSpace(first))
			{
				lblStatus->Text = L"First name is required.";
				return;
			}
			if (!IsValidPakPhone(phone))
			{
				lblStatus->Text = L"Use valid Pakistani number format: +923XXXXXXXXX";
				return;
			}
			if (!IsValidEmailProvider(email))
			{
				lblStatus->Text = L"Use a valid email with common provider (gmail/outlook/yahoo/etc).";
				return;
			}
			if (String::IsNullOrWhiteSpace(username))
			{
				lblStatus->Text = L"Username is required.";
				return;
			}
			if (username->Length < 4)
			{
				lblStatus->Text = L"Username must be at least 4 characters.";
				return;
			}
			if (String::IsNullOrWhiteSpace(password) || password->Length < 8)
			{
				lblStatus->Text = L"Password must be at least 8 characters.";
				return;
			}
			if (password != confirm)
			{
				lblStatus->Text = L"Password and confirm password do not match.";
				return;
			}
			if (UsernameExists(username))
			{
				lblStatus->Text = L"Username already exists.";
				return;
			}
			if (EmailExists(email))
			{
				lblStatus->Text = L"Email already registered.";
				return;
			}

			UserAccount^ user = gcnew UserAccount();
			user->FirstName = first;
			user->SecondName = second;
			user->Phone = phone;
			user->Email = email;
			user->Username = username;
			user->Password = password; // demo only; hash in production
			users->Add(user);

			MessageBox::Show(L"Signup successful. You can now login.", L"Success",
				MessageBoxButtons::OK, MessageBoxIcon::Information);

			this->Close();
		}

		System::Void btnBack_Click(System::Object^ sender, System::EventArgs^ e)
		{
			this->Close();
		}

		System::Void SignupForm_FormClosed(System::Object^ sender, FormClosedEventArgs^ e)
		{
			if (this->Owner != nullptr)
			{
				this->Owner->Show();
			}
		}
	};
}