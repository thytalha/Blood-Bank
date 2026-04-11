#pragma once

namespace BloodBank
{

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

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
	private: System::Windows::Forms::WebBrowser^ videoBrowser;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
         this->videoBrowser = (gcnew System::Windows::Forms::WebBrowser());
			this->loginPanel = (gcnew System::Windows::Forms::Panel());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->btnSignup = (gcnew System::Windows::Forms::Button());
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->lblTitle = (gcnew System::Windows::Forms::Label());
			this->loginPanel->SuspendLayout();
			this->SuspendLayout();
         // 
			// videoBrowser
			// 
			this->videoBrowser->AllowWebBrowserDrop = false;
			this->videoBrowser->Dock = System::Windows::Forms::DockStyle::Fill;
			this->videoBrowser->IsWebBrowserContextMenuEnabled = false;
			this->videoBrowser->Location = System::Drawing::Point(0, 0);
			this->videoBrowser->MinimumSize = System::Drawing::Size(20, 20);
			this->videoBrowser->Name = L"videoBrowser";
			this->videoBrowser->ScriptErrorsSuppressed = true;
			this->videoBrowser->ScrollBarsEnabled = false;
			this->videoBrowser->Size = System::Drawing::Size(1200, 800);
			this->videoBrowser->TabIndex = 8;
			this->videoBrowser->WebBrowserShortcutsEnabled = false;
			// 
			// loginPanel
			// 
			this->loginPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->loginPanel->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->loginPanel->Controls->Add(this->lblStatus);
			this->loginPanel->Controls->Add(this->btnSignup);
			this->loginPanel->Controls->Add(this->btnLogin);
			this->loginPanel->Controls->Add(this->txtPassword);
			this->loginPanel->Controls->Add(this->lblPassword);
			this->loginPanel->Controls->Add(this->txtUsername);
			this->loginPanel->Controls->Add(this->lblUsername);
			this->loginPanel->Controls->Add(this->lblTitle);
			this->loginPanel->Location = System::Drawing::Point(0, 0);
			this->loginPanel->Name = L"loginPanel";
			this->loginPanel->Size = System::Drawing::Size(600, 500);
			this->loginPanel->TabIndex = 0;
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblStatus->Location = System::Drawing::Point(80, 440);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 23);
			this->lblStatus->TabIndex = 0;
			// 
			// btnSignup
			// 
			this->btnSignup->BackColor = System::Drawing::Color::Transparent;
			this->btnSignup->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSignup->FlatAppearance->BorderColor = System::Drawing::Color::White;
			this->btnSignup->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSignup->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14));
			this->btnSignup->ForeColor = System::Drawing::Color::White;
			this->btnSignup->Location = System::Drawing::Point(310, 370);
			this->btnSignup->Name = L"btnSignup";
			this->btnSignup->Size = System::Drawing::Size(210, 50);
			this->btnSignup->TabIndex = 1;
			this->btnSignup->Text = L"SIGN UP";
			this->btnSignup->UseVisualStyleBackColor = false;
			this->btnSignup->Click += gcnew System::EventHandler(this, &MyForm::btnSignup_Click);
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::Crimson;
			this->btnLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogin->FlatAppearance->BorderSize = 0;
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogin->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14));
			this->btnLogin->ForeColor = System::Drawing::Color::White;
			this->btnLogin->Location = System::Drawing::Point(80, 370);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(210, 50);
			this->btnLogin->TabIndex = 2;
			this->btnLogin->Text = L"LOGIN";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// txtPassword
			// 
			this->txtPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14));
			this->txtPassword->Location = System::Drawing::Point(80, 285);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->PasswordChar = '*';
			this->txtPassword->Size = System::Drawing::Size(440, 39);
			this->txtPassword->TabIndex = 3;
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14));
			this->lblPassword->ForeColor = System::Drawing::Color::LightGray;
			this->lblPassword->Location = System::Drawing::Point(80, 250);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(111, 32);
			this->lblPassword->TabIndex = 4;
			this->lblPassword->Text = L"Password";
			// 
			// txtUsername
			// 
			this->txtUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14));
			this->txtUsername->Location = System::Drawing::Point(80, 185);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(440, 39);
			this->txtUsername->TabIndex = 5;
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14));
			this->lblUsername->ForeColor = System::Drawing::Color::LightGray;
			this->lblUsername->Location = System::Drawing::Point(80, 150);
			this->lblUsername->Name = L"lblUsername";
			this->lblUsername->Size = System::Drawing::Size(121, 32);
			this->lblUsername->TabIndex = 6;
			this->lblUsername->Text = L"Username";
			// 
			// lblTitle
			// 
			this->lblTitle->AutoSize = true;
			this->lblTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI Semibold", 28, System::Drawing::FontStyle::Bold));
			this->lblTitle->ForeColor = System::Drawing::Color::White;
			this->lblTitle->Location = System::Drawing::Point(100, 40);
			this->lblTitle->Name = L"lblTitle";
			this->lblTitle->Size = System::Drawing::Size(306, 62);
			this->lblTitle->TabIndex = 7;
			this->lblTitle->Text = L"Admin Portal";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(5)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
				static_cast<System::Int32>(static_cast<System::Byte>(35)));
			this->ClientSize = System::Drawing::Size(1200, 800);
          this->Controls->Add(this->videoBrowser);
			this->Controls->Add(this->loginPanel);
			this->Name = L"MyForm";
			this->Text = L"Blood Bank Management System";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->loginPanel->ResumeLayout(false);
			this->loginPanel->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void CenterPanel()
	{
		int x = (this->ClientSize.Width - loginPanel->Width) / 2;
		int y = (this->ClientSize.Height - loginPanel->Height) / 2;
		loginPanel->Location = System::Drawing::Point(x, y);
	}

	private: System::Void InitializeVideoBackground()
	{
       String^ videoFile = nullptr;
		array<String^>^ candidates = gcnew array<String^>
		{
			Path::Combine(Application::StartupPath, L"bg_video.mp4"),
			Path::GetFullPath(Path::Combine(Application::StartupPath, L"..\\bg_video.mp4")),
			Path::GetFullPath(Path::Combine(Application::StartupPath, L"..\\..\\bg_video.mp4")),
			Path::GetFullPath(Path::Combine(Application::StartupPath, L"..\\..\\..\\bg_video.mp4"))
		};

		for each (String ^ candidate in candidates)
		{
			if (File::Exists(candidate))
			{
				videoFile = candidate;
				break;
			}
		}

       if (String::IsNullOrEmpty(videoFile))
		{
			lblStatus->ForeColor = System::Drawing::Color::Orange;
          lblStatus->Text = L"Background video not found. Put bg_video.mp4 in Debug/x64 folder or project root.";
			return;
		}

		String^ videoUri = (gcnew Uri(videoFile))->AbsoluteUri;
		String^ html = String::Format(
			L"<html><head><meta http-equiv='X-UA-Compatible' content='IE=edge' />"
			L"<style>html,body{{margin:0;padding:0;overflow:hidden;background:black;height:100%;}}"
			L"video{{position:fixed;right:0;bottom:0;min-width:100%;min-height:100%;object-fit:cover;}}</style>"
			L"</head><body><video autoplay muted loop>"
			L"<source src='{0}' type='video/mp4'></video></body></html>",
			videoUri);

		videoBrowser->DocumentText = html;
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) 
	{
      InitializeVideoBackground();
		CenterPanel();
       loginPanel->BringToFront();
	}

	private: System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e) 
	{
		CenterPanel();
       loginPanel->BringToFront();
	}

	private: System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		if (txtUsername->Text == "admin" && txtPassword->Text == "admin123")
		{
			MessageBox::Show("Admin Verified. Loading Database...", "Access Granted",
				MessageBoxButtons::OK, MessageBoxIcon::Information);
		}
		else
		{
			lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
			lblStatus->Text = "Error: Invalid admin credentials.";
		}
	}

		   // THE UPDATED SIGN UP MESSAGE:
	private: System::Void btnSignup_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		MessageBox::Show("The Sign Up form is not ready yet. It will be ready soon!", "System Update",
			MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
	};
}