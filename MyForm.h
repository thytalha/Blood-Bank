#pragma once
namespace BloodBank
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			isPasswordVisible = false; // Initialize the password visibility state
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
	private: System::Windows::Forms::Label^ lblLogo;
	private: System::Windows::Forms::Label^ lblBrandName;
	private: System::Windows::Forms::Label^ lblLoginTitle;
	private: System::Windows::Forms::Label^ lblLoginSubTitle;
	private: System::Windows::Forms::Label^ lblUsername;
	private: System::Windows::Forms::Label^ lblUserIcon;
	private: System::Windows::Forms::TextBox^ txtUsername;
	private: System::Windows::Forms::Label^ lblPassword;
	private: System::Windows::Forms::Label^ lblLockIcon;
	private: System::Windows::Forms::TextBox^ txtPassword;
	private: System::Windows::Forms::Button^ btnLogin;
	private: System::Windows::Forms::Label^ lblNoAccount;
	private: System::Windows::Forms::LinkLabel^ linkSignup;
	private: System::Windows::Forms::CheckBox^ chkRememberMe;
	private: System::Windows::Forms::LinkLabel^ linkForgotPassword;
	private: System::Windows::Forms::Label^ lblStatus;
	private: System::Windows::Forms::Label^ lblWelcome;
	private: System::Windows::Forms::Label^ lblWelcomeDesc;
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
			this->lblLogo = (gcnew System::Windows::Forms::Label());
			this->lblBrandName = (gcnew System::Windows::Forms::Label());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->linkSignup = (gcnew System::Windows::Forms::LinkLabel());
			this->lblNoAccount = (gcnew System::Windows::Forms::Label());
			this->btnLogin = (gcnew System::Windows::Forms::Button());
			this->chkRememberMe = (gcnew System::Windows::Forms::CheckBox());
			this->linkForgotPassword = (gcnew System::Windows::Forms::LinkLabel());
			this->btnTogglePassword = (gcnew System::Windows::Forms::Button());
			this->linePassword = (gcnew System::Windows::Forms::Panel());
			this->lineUsername = (gcnew System::Windows::Forms::Panel());
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblLockIcon = (gcnew System::Windows::Forms::Label());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblUserIcon = (gcnew System::Windows::Forms::Label());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
			this->lblLoginSubTitle = (gcnew System::Windows::Forms::Label());
			this->lblLoginTitle = (gcnew System::Windows::Forms::Label());
			this->rightPanel = (gcnew System::Windows::Forms::Panel());
			this->lblWelcome = (gcnew System::Windows::Forms::Label());
			this->lblWelcomeDesc = (gcnew System::Windows::Forms::Label());
			this->leftPanel->SuspendLayout();
			this->rightPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// leftPanel
			// 
			this->leftPanel->BackColor = System::Drawing::Color::White;
			this->leftPanel->Controls->Add(this->lblLogo);
			this->leftPanel->Controls->Add(this->lblBrandName);
			this->leftPanel->Controls->Add(this->lblStatus);
			this->leftPanel->Controls->Add(this->linkSignup);
			this->leftPanel->Controls->Add(this->lblNoAccount);
			this->leftPanel->Controls->Add(this->btnLogin);
			this->leftPanel->Controls->Add(this->chkRememberMe);
			this->leftPanel->Controls->Add(this->linkForgotPassword);
			this->leftPanel->Controls->Add(this->btnTogglePassword);
			this->leftPanel->Controls->Add(this->linePassword);
			this->leftPanel->Controls->Add(this->lineUsername);
			this->leftPanel->Controls->Add(this->txtPassword);
			this->leftPanel->Controls->Add(this->lblLockIcon);
			this->leftPanel->Controls->Add(this->lblPassword);
			this->leftPanel->Controls->Add(this->txtUsername);
			this->leftPanel->Controls->Add(this->lblUserIcon);
			this->leftPanel->Controls->Add(this->lblUsername);
			this->leftPanel->Controls->Add(this->lblLoginSubTitle);
			this->leftPanel->Controls->Add(this->lblLoginTitle);
			this->leftPanel->Dock = System::Windows::Forms::DockStyle::Left;
			this->leftPanel->Location = System::Drawing::Point(0, 0);
			this->leftPanel->Name = L"leftPanel";
			this->leftPanel->Size = System::Drawing::Size(600, 800);
			this->leftPanel->TabIndex = 0;
			this->leftPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::leftPanel_Paint);
			// 
			// lblLogo
			// 
			this->lblLogo->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
				static_cast<System::Int32>(static_cast<System::Byte>(235)));
			this->lblLogo->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 16));
			this->lblLogo->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->lblLogo->Location = System::Drawing::Point(0, 0);
			this->lblLogo->Name = L"lblLogo";
			this->lblLogo->Padding = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->lblLogo->Size = System::Drawing::Size(40, 40);
			this->lblLogo->TabIndex = 0;
			this->lblLogo->Text = L"💧";
			this->lblLogo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lblBrandName
			// 
			this->lblBrandName->AutoSize = true;
			this->lblBrandName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
			this->lblBrandName->ForeColor = System::Drawing::Color::Black;
			this->lblBrandName->Location = System::Drawing::Point(0, 0);
			this->lblBrandName->Name = L"lblBrandName";
			this->lblBrandName->Size = System::Drawing::Size(140, 37);
			this->lblBrandName->TabIndex = 1;
			this->lblBrandName->Text = L"LifeBlood";
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->ForeColor = System::Drawing::Color::Red;
			this->lblStatus->Location = System::Drawing::Point(0, 0);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 16);
			this->lblStatus->TabIndex = 2;
			// 
			// linkSignup
			// 
			this->linkSignup->AutoSize = true;
			this->linkSignup->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->linkSignup->LinkBehavior = System::Windows::Forms::LinkBehavior::NeverUnderline;
			this->linkSignup->LinkColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->linkSignup->Location = System::Drawing::Point(0, 0);
			this->linkSignup->Name = L"linkSignup";
			this->linkSignup->Size = System::Drawing::Size(72, 23);
			this->linkSignup->TabIndex = 3;
			this->linkSignup->TabStop = true;
			this->linkSignup->Text = L"Sign up";
			this->linkSignup->Click += gcnew System::EventHandler(this, &MyForm::btnSignup_Click);
			// 
			// lblNoAccount
			// 
			this->lblNoAccount->AutoSize = true;
			this->lblNoAccount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblNoAccount->ForeColor = System::Drawing::Color::Gray;
			this->lblNoAccount->Location = System::Drawing::Point(0, 0);
			this->lblNoAccount->Name = L"lblNoAccount";
			this->lblNoAccount->Size = System::Drawing::Size(191, 23);
			this->lblNoAccount->TabIndex = 4;
			this->lblNoAccount->Text = L"Don\'t have an account\?";
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->btnLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogin->FlatAppearance->BorderSize = 0;
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
			this->btnLogin->ForeColor = System::Drawing::Color::White;
			this->btnLogin->Location = System::Drawing::Point(0, 0);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(400, 45);
			this->btnLogin->TabIndex = 5;
			this->btnLogin->Text = L"Login to Dashboard";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// chkRememberMe
			// 
			this->chkRememberMe->AutoSize = true;
			this->chkRememberMe->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->chkRememberMe->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->chkRememberMe->Location = System::Drawing::Point(0, 0);
			this->chkRememberMe->Name = L"chkRememberMe";
			this->chkRememberMe->Size = System::Drawing::Size(144, 27);
			this->chkRememberMe->TabIndex = 6;
			this->chkRememberMe->Text = L"Remember me";
			// 
			// linkForgotPassword
			// 
			this->linkForgotPassword->AutoSize = true;
			this->linkForgotPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->linkForgotPassword->LinkBehavior = System::Windows::Forms::LinkBehavior::NeverUnderline;
			this->linkForgotPassword->LinkColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)),
				static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->linkForgotPassword->Location = System::Drawing::Point(0, 0);
			this->linkForgotPassword->Name = L"linkForgotPassword";
			this->linkForgotPassword->Size = System::Drawing::Size(152, 23);
			this->linkForgotPassword->TabIndex = 7;
			this->linkForgotPassword->TabStop = true;
			this->linkForgotPassword->Text = L"Forgot password\?";
			// 
			// btnTogglePassword
			// 
			this->btnTogglePassword->FlatAppearance->BorderSize = 0;
			this->btnTogglePassword->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(238)),
				static_cast<System::Int32>(static_cast<System::Byte>(241)), static_cast<System::Int32>(static_cast<System::Byte>(247)));
			this->btnTogglePassword->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)),
				static_cast<System::Int32>(static_cast<System::Byte>(247)), static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->btnTogglePassword->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnTogglePassword->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 12));
			this->btnTogglePassword->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)),
				static_cast<System::Int32>(static_cast<System::Byte>(160)), static_cast<System::Int32>(static_cast<System::Byte>(175)));
			this->btnTogglePassword->Location = System::Drawing::Point(0, 0);
			this->btnTogglePassword->Name = L"btnTogglePassword";
			this->btnTogglePassword->Size = System::Drawing::Size(30, 30);
			this->btnTogglePassword->TabIndex = 8;
			this->btnTogglePassword->Text = L"";
			this->btnTogglePassword->Click += gcnew System::EventHandler(this, &MyForm::btnTogglePassword_Click);
			// 
			// linePassword
			// 
			this->linePassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->linePassword->Location = System::Drawing::Point(0, 0);
			this->linePassword->Name = L"linePassword";
			this->linePassword->Size = System::Drawing::Size(400, 56);
			this->linePassword->TabIndex = 9;
			// 
			// lineUsername
			// 
			this->lineUsername->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->lineUsername->Location = System::Drawing::Point(0, 0);
			this->lineUsername->Name = L"lineUsername";
			this->lineUsername->Size = System::Drawing::Size(400, 56);
			this->lineUsername->TabIndex = 10;
			// 
			// txtPassword
			// 
			this->txtPassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->txtPassword->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->txtPassword->ForeColor = System::Drawing::Color::Gray;
			this->txtPassword->Location = System::Drawing::Point(0, 0);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->Size = System::Drawing::Size(100, 27);
			this->txtPassword->TabIndex = 11;
			this->txtPassword->Text = L"••••••••";
			this->txtPassword->Enter += gcnew System::EventHandler(this, &MyForm::txtPassword_Enter);
			this->txtPassword->Leave += gcnew System::EventHandler(this, &MyForm::txtPassword_Leave);
			// 
			// lblLockIcon
			// 
			this->lblLockIcon->AutoSize = true;
			this->lblLockIcon->BackColor = System::Drawing::Color::Transparent;
			this->lblLockIcon->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 14));
			this->lblLockIcon->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)), static_cast<System::Int32>(static_cast<System::Byte>(160)),
				static_cast<System::Int32>(static_cast<System::Byte>(175)));
			this->lblLockIcon->Location = System::Drawing::Point(0, 0);
			this->lblLockIcon->Name = L"lblLockIcon";
			this->lblLockIcon->Size = System::Drawing::Size(34, 24);
			this->lblLockIcon->TabIndex = 12;
			this->lblLockIcon->Text = L"";
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblPassword->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->lblPassword->Location = System::Drawing::Point(0, 0);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(85, 23);
			this->lblPassword->TabIndex = 13;
			this->lblPassword->Text = L"Password";
			// 
			// txtUsername
			// 
			this->txtUsername->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->txtUsername->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->txtUsername->ForeColor = System::Drawing::Color::Gray;
			this->txtUsername->Location = System::Drawing::Point(0, 0);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->Size = System::Drawing::Size(100, 27);
			this->txtUsername->TabIndex = 14;
			this->txtUsername->Text = L"Enter your username";
			this->txtUsername->Enter += gcnew System::EventHandler(this, &MyForm::txtUsername_Enter);
			this->txtUsername->Leave += gcnew System::EventHandler(this, &MyForm::txtUsername_Leave);
			// 
			// lblUserIcon
			// 
			this->lblUserIcon->AutoSize = true;
			this->lblUserIcon->BackColor = System::Drawing::Color::Transparent;
			this->lblUserIcon->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 14));
			this->lblUserIcon->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)), static_cast<System::Int32>(static_cast<System::Byte>(160)),
				static_cast<System::Int32>(static_cast<System::Byte>(175)));
			this->lblUserIcon->Location = System::Drawing::Point(0, 0);
			this->lblUserIcon->Name = L"lblUserIcon";
			this->lblUserIcon->Size = System::Drawing::Size(34, 24);
			this->lblUserIcon->TabIndex = 15;
			this->lblUserIcon->Text = L"";
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblUsername->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(64)), static_cast<System::Int32>(static_cast<System::Byte>(64)),
				static_cast<System::Int32>(static_cast<System::Byte>(64)));
			this->lblUsername->Location = System::Drawing::Point(0, 0);
			this->lblUsername->Name = L"lblUsername";
			this->lblUsername->Size = System::Drawing::Size(89, 23);
			this->lblUsername->TabIndex = 16;
			this->lblUsername->Text = L"Username";
			// 
			// lblLoginSubTitle
			// 
			this->lblLoginSubTitle->AutoSize = true;
			this->lblLoginSubTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));
			this->lblLoginSubTitle->ForeColor = System::Drawing::Color::Gray;
			this->lblLoginSubTitle->Location = System::Drawing::Point(0, 0);
			this->lblLoginSubTitle->Name = L"lblLoginSubTitle";
			this->lblLoginSubTitle->Size = System::Drawing::Size(383, 25);
			this->lblLoginSubTitle->TabIndex = 17;
			this->lblLoginSubTitle->Text = L"Enter your account details below to proceed";
			// 
			// lblLoginTitle
			// 
			this->lblLoginTitle->AutoSize = true;
			this->lblLoginTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 28, System::Drawing::FontStyle::Bold));
			this->lblLoginTitle->ForeColor = System::Drawing::Color::Black;
			this->lblLoginTitle->Location = System::Drawing::Point(0, 0);
			this->lblLoginTitle->Name = L"lblLoginTitle";
			this->lblLoginTitle->Size = System::Drawing::Size(511, 62);
			this->lblLoginTitle->TabIndex = 18;
			this->lblLoginTitle->Text = L"Login to your account";
			// 
			// rightPanel
			// 
			this->rightPanel->Controls->Add(this->lblWelcome);
			this->rightPanel->Controls->Add(this->lblWelcomeDesc);
			this->rightPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rightPanel->Location = System::Drawing::Point(600, 0);
			this->rightPanel->Name = L"rightPanel";
			this->rightPanel->Size = System::Drawing::Size(600, 800);
			this->rightPanel->TabIndex = 0;
			this->rightPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::rightPanel_Paint);
			// 
			// lblWelcome
			// 
			this->lblWelcome->BackColor = System::Drawing::Color::Transparent;
			this->lblWelcome->Font = (gcnew System::Drawing::Font(L"Segoe UI Black", 42, System::Drawing::FontStyle::Bold));
			this->lblWelcome->ForeColor = System::Drawing::Color::White;
			this->lblWelcome->Location = System::Drawing::Point(0, 0);
			this->lblWelcome->Name = L"lblWelcome";
			this->lblWelcome->Size = System::Drawing::Size(100, 23);
			this->lblWelcome->TabIndex = 0;
			this->lblWelcome->Text = L"Welcome to\nLifeBlood MMS";
			this->lblWelcome->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lblWelcomeDesc
			// 
			this->lblWelcomeDesc->BackColor = System::Drawing::Color::Transparent;
			this->lblWelcomeDesc->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14));
			this->lblWelcomeDesc->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(230)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
				static_cast<System::Int32>(static_cast<System::Byte>(230)));
			this->lblWelcomeDesc->Location = System::Drawing::Point(0, 0);
			this->lblWelcomeDesc->Name = L"lblWelcomeDesc";
			this->lblWelcomeDesc->Size = System::Drawing::Size(100, 23);
			this->lblWelcomeDesc->TabIndex = 1;
			this->lblWelcomeDesc->Text = L"Streamlining the connection between generous donors\nand critical needs. Every dro"
				L"p counts in saving lives.";
			this->lblWelcomeDesc->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// MyForm
			// 
			this->ClientSize = System::Drawing::Size(1200, 800);
			this->Controls->Add(this->rightPanel);
			this->Controls->Add(this->leftPanel);
			this->Name = L"MyForm";
			this->Text = L"LifeBlood MMS Login";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->leftPanel->ResumeLayout(false);
			this->leftPanel->PerformLayout();
			this->rightPanel->ResumeLayout(false);
			this->ResumeLayout(false);

			// In InitializeComponent(), add these handlers (after controls are created)
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
			this->leftPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
			this->rightPanel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);

			// optional (recommended): labels also receive clicks, not panel background
			this->lblLoginTitle->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
			this->lblLoginSubTitle->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
			this->lblBrandName->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
			this->lblWelcome->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
			this->lblWelcomeDesc->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::ClearFocusOnBackgroundClick);
		}
#pragma endregion

	private: System::Void ArrangeLeftPanelControls()
	{
		int fieldWidth = 400;

		lblLogo->Location = System::Drawing::Point(100, 50);
		lblBrandName->Location = System::Drawing::Point(150, 55);
		GraphicsPath^ logoBox = GetRoundedRect(Rectangle(0, 0, lblLogo->Width - 1, lblLogo->Height - 1), 10);
		lblLogo->Region = gcnew System::Drawing::Region(logoBox);
		delete logoBox;

		lblLoginTitle->Location = System::Drawing::Point(95, 150);
		lblLoginSubTitle->Location = System::Drawing::Point(100, 205);

		lblUsername->Location = System::Drawing::Point(100, 270);
		lineUsername->Location = System::Drawing::Point(100, 295);
		lineUsername->Size = System::Drawing::Size(fieldWidth, 56);
		lblUserIcon->Location = System::Drawing::Point(16, 17);
		txtUsername->Location = System::Drawing::Point(52, 17);
		txtUsername->Width = fieldWidth - 68;

		lblPassword->Location = System::Drawing::Point(100, 375);
		linePassword->Location = System::Drawing::Point(100, 400);
		linePassword->Size = System::Drawing::Size(fieldWidth, 56);
		lblLockIcon->Location = System::Drawing::Point(16, 17);
		txtPassword->Location = System::Drawing::Point(52, 17);
		txtPassword->Width = fieldWidth - 100;
		btnTogglePassword->Location = System::Drawing::Point(fieldWidth - 38, 13);

		GraphicsPath^ userBox = GetRoundedRect(Rectangle(0, 0, lineUsername->Width - 1, lineUsername->Height - 1), 16);
		lineUsername->Region = gcnew System::Drawing::Region(userBox);
		delete userBox;
		GraphicsPath^ passBox = GetRoundedRect(Rectangle(0, 0, linePassword->Width - 1, linePassword->Height - 1), 16);
		linePassword->Region = gcnew System::Drawing::Region(passBox);
		delete passBox;

		lineUsername->SendToBack();
		linePassword->SendToBack();
		lineUsername->BringToFront();
		linePassword->BringToFront();

		chkRememberMe->Location = System::Drawing::Point(100, 490);
		linkForgotPassword->Location = System::Drawing::Point(100 + fieldWidth - linkForgotPassword->Width, 490);

		btnLogin->Location = System::Drawing::Point(100, 530);
		btnLogin->Width = fieldWidth;
		GraphicsPath^ loginBox = GetRoundedRect(Rectangle(0, 0, btnLogin->Width - 1, btnLogin->Height - 1), 14);
		btnLogin->Region = gcnew System::Drawing::Region(loginBox);
		delete loginBox;

		lblNoAccount->Location = System::Drawing::Point(100 + 100, 590);
		linkSignup->Location = System::Drawing::Point(lblNoAccount->Right + 5, 590);

		lblStatus->Location = System::Drawing::Point(100, 620);
	}

	private: System::Void ArrangeRightPanelControls()
	{
		lblWelcome->Width = rightPanel->Width;
		lblWelcome->Height = 150;
		lblWelcome->Location = System::Drawing::Point(0, (rightPanel->Height / 2) - 80);

		lblWelcomeDesc->Width = rightPanel->Width;
		lblWelcomeDesc->Height = 80;
		lblWelcomeDesc->Location = System::Drawing::Point(0, lblWelcome->Bottom + 10);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtUsername->Parent != lineUsername)
		{
			leftPanel->Controls->Remove(lblUserIcon);
			leftPanel->Controls->Remove(txtUsername);
			lineUsername->Controls->Add(lblUserIcon);
			lineUsername->Controls->Add(txtUsername);
		}

		if (txtPassword->Parent != linePassword)
		{
			leftPanel->Controls->Remove(lblLockIcon);
			leftPanel->Controls->Remove(txtPassword);
			leftPanel->Controls->Remove(btnTogglePassword);
			linePassword->Controls->Add(lblLockIcon);
			linePassword->Controls->Add(txtPassword);
			linePassword->Controls->Add(btnTogglePassword);
		}

		ArrangeLeftPanelControls();
		ArrangeRightPanelControls();
		this->ActiveControl = nullptr;
	}

	private: System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e)
	{
		leftPanel->Width = this->Width / 2; // Split 50/50 exactly like the image
		ArrangeLeftPanelControls();
		ArrangeRightPanelControls();
		rightPanel->Invalidate(); // Force redraw of custom graphics
	}

	private: System::Void txtUsername_Enter(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtUsername->Text == L"Enter your username") {
			txtUsername->Text = L"";
			txtUsername->ForeColor = System::Drawing::Color::Black;
		}
	}

	private: System::Void txtUsername_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(txtUsername->Text)) {
			txtUsername->Text = L"Enter your username";
		 txtUsername->ForeColor = System::Drawing::Color::Gray;
		}
	}

	private: System::Void txtPassword_Enter(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtPassword->Text == L"••••••••") {
			txtPassword->Text = L"";
			txtPassword->ForeColor = System::Drawing::Color::Black;
			txtPassword->UseSystemPasswordChar = !isPasswordVisible;
		}
	}

	private: System::Void txtPassword_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(txtPassword->Text)) {
			isPasswordVisible = false;
			txtPassword->UseSystemPasswordChar = false;
			txtPassword->Text = L"••••••••";
			txtPassword->ForeColor = System::Drawing::Color::Gray;
			btnTogglePassword->Text = L"\xE18B";
		}
	}

	private: System::Void btnTogglePassword_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtPassword->Text != L"••••••••") {
			isPasswordVisible = !isPasswordVisible;
			txtPassword->UseSystemPasswordChar = !isPasswordVisible;
			btnTogglePassword->Text = isPasswordVisible ? L"\xED1A" : L"\xE18B";
		}
	}

		   // Helper to draw rounded rectangles for the glassmorphism effect
	private: GraphicsPath^ GetRoundedRect(Rectangle bounds, int radius)
	{
		GraphicsPath^ path = gcnew GraphicsPath();
		int diameter = radius * 2;
		Rectangle arc = Rectangle(bounds.Location, System::Drawing::Size(diameter, diameter));
		path->AddArc(arc, 180, 90);
		arc.X = bounds.Right - diameter;
		path->AddArc(arc, 270, 90);
		arc.Y = bounds.Bottom - diameter;
		path->AddArc(arc, 0, 90);
		arc.X = bounds.Left;
		path->AddArc(arc, 90, 90);
		path->CloseFigure();
		return path;
	}

	private: System::Void DrawHeartbeatIcon(Graphics^ g, Rectangle bounds)
	{
		g->SmoothingMode = SmoothingMode::AntiAlias;

		Pen^ iconPen = gcnew Pen(Color::FromArgb(255, 205, 214), 4.4f);
		iconPen->LineJoin = LineJoin::Round;
		iconPen->StartCap = LineCap::Round;
		iconPen->EndCap = LineCap::Round;

		GraphicsPath^ heart = gcnew GraphicsPath();
		int w = bounds.Width;
		int h = bounds.Height;
		int x = bounds.X;
		int y = bounds.Y;

		Point pTop = Point(x + w / 2, y + static_cast<int>(h * 0.28f));
		Point pBottom = Point(x + w / 2, y + h - 4);

		heart->StartFigure();
		heart->AddBezier(
			pTop,
			Point(x + static_cast<int>(w * 0.18f), y - 2),
			Point(x - 2, y + static_cast<int>(h * 0.38f)),
			Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)));
		heart->AddBezier(
			Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)),
			Point(x + static_cast<int>(w * 0.34f), y + static_cast<int>(h * 0.74f)),
			Point(x + static_cast<int>(w * 0.43f), y + static_cast<int>(h * 0.88f)),
			pBottom);
		heart->AddBezier(
			pBottom,
			Point(x + static_cast<int>(w * 0.57f), y + static_cast<int>(h * 0.88f)),
			Point(x + static_cast<int>(w * 0.66f), y + static_cast<int>(h * 0.74f)),
			Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)));
		heart->AddBezier(
			Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)),
			Point(x + w + 2, y + static_cast<int>(h * 0.38f)),
			Point(x + static_cast<int>(w * 0.82f), y - 2),
			pTop);

		g->DrawPath(iconPen, heart);

		array<Point>^ ekgPoints = {
			Point(x + static_cast<int>(w * 0.16f), y + static_cast<int>(h * 0.52f)),
			Point(x + static_cast<int>(w * 0.35f), y + static_cast<int>(h * 0.52f)),
			Point(x + static_cast<int>(w * 0.44f), y + static_cast<int>(h * 0.38f)),
			Point(x + static_cast<int>(w * 0.53f), y + static_cast<int>(h * 0.70f)),
			Point(x + static_cast<int>(w * 0.64f), y + static_cast<int>(h * 0.50f)),
			Point(x + static_cast<int>(w * 0.83f), y + static_cast<int>(h * 0.50f))
		};
		g->DrawLines(iconPen, ekgPoints);

		delete heart;
		delete iconPen;
	}

	private: System::Void rightPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;

		// Main Background Gradient
		Rectangle rect = rightPanel->ClientRectangle;
		LinearGradientBrush^ bgBrush = gcnew LinearGradientBrush(rect,
			Color::FromArgb(210, 10, 45),   // Top Left Dark Red
			Color::FromArgb(60, 20, 35),    // Bottom Right Deep Burgundy
			LinearGradientMode::ForwardDiagonal);
		e->Graphics->FillRectangle(bgBrush, rect);

		// Translucent card style
		SolidBrush^ glassBrush = gcnew SolidBrush(Color::FromArgb(30, 255, 255, 255));
		Pen^ glassBorder = gcnew Pen(Color::FromArgb(50, 255, 255, 255), 1);

		// Center Top Icon Card (Heart) only
		Rectangle card2 = Rectangle(rightPanel->Width / 2 - 40, 200, 80, 80);
		GraphicsPath^ path2 = GetRoundedRect(card2, 15);
		e->Graphics->FillPath(glassBrush, path2);
		e->Graphics->DrawPath(glassBorder, path2);
		DrawHeartbeatIcon(e->Graphics, Rectangle(card2.X + 5, card2.Y + 10, 70, 60));

		// Cleanup
		delete bgBrush;
		delete glassBrush;
		delete glassBorder;
		delete path2;
	}

	private: System::Void btnLogin_Click(System::Object^ sender, System::EventArgs^ e)
	{
		// Basic validation logic
		if (String::IsNullOrWhiteSpace(txtUsername->Text) || txtUsername->Text == L"Enter your username" ||
			String::IsNullOrWhiteSpace(txtPassword->Text) || txtPassword->Text == L"••••••••")
		{
			lblStatus->Text = L"Please enter valid credentials.";
			return;
		}

		lblStatus->Text = L"";
		MessageBox::Show(L"Login attempting...", L"System", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}

	private: System::Void btnSignup_Click(System::Object^ sender, System::EventArgs^ e)
	{
		MessageBox::Show(L"Redirecting to registration...", L"Sign Up", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
	private: System::Void leftPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}
	private: System::Void ClearFocusOnBackgroundClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		this->ActiveControl = nullptr;
	}
	};
}
// hello