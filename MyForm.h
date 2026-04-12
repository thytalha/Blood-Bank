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
			this->txtPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
          this->lblLockIcon = (gcnew System::Windows::Forms::Label());
			this->txtUsername = (gcnew System::Windows::Forms::TextBox());
			this->lblUsername = (gcnew System::Windows::Forms::Label());
           this->lblUserIcon = (gcnew System::Windows::Forms::Label());
			this->lblLoginSubTitle = (gcnew System::Windows::Forms::Label());
			this->lblLoginTitle = (gcnew System::Windows::Forms::Label());
			this->rightPanel = (gcnew System::Windows::Forms::Panel());
			this->lblWelcome = (gcnew System::Windows::Forms::Label());
			this->lblWelcomeDesc = (gcnew System::Windows::Forms::Label());
			this->lineUsername = (gcnew System::Windows::Forms::Panel());
			this->linePassword = (gcnew System::Windows::Forms::Panel());
			this->btnTogglePassword = (gcnew System::Windows::Forms::Button());
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
			// 
			// lblLogo (Placeholder for actual image)
			// 
			this->lblLogo->BackColor = System::Drawing::Color::FromArgb(255, 230, 235);
			this->lblLogo->ForeColor = System::Drawing::Color::FromArgb(232, 15, 59);
            this->lblLogo->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 16));
         this->lblLogo->Padding = System::Windows::Forms::Padding(3, 0, 0, 0);
			this->lblLogo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->lblLogo->Size = System::Drawing::Size(40, 40);
           this->lblLogo->Text = L"\xD83D\xDCA7";
			// 
			// lblBrandName
			// 
			this->lblBrandName->AutoSize = true;
			this->lblBrandName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
			this->lblBrandName->ForeColor = System::Drawing::Color::Black;
			this->lblBrandName->Text = L"LifeBlood";
			// 
			// lblLoginTitle
			// 
			this->lblLoginTitle->AutoSize = true;
			this->lblLoginTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 28, System::Drawing::FontStyle::Bold));
			this->lblLoginTitle->ForeColor = System::Drawing::Color::Black;
			this->lblLoginTitle->Text = L"Login to your account";
			// 
			// lblLoginSubTitle
			// 
			this->lblLoginSubTitle->AutoSize = true;
			this->lblLoginSubTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));
			this->lblLoginSubTitle->ForeColor = System::Drawing::Color::Gray;
			this->lblLoginSubTitle->Text = L"Enter your account details below to proceed";
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblUsername->ForeColor = System::Drawing::Color::FromArgb(64, 64, 64);
			this->lblUsername->Text = L"Username";
         // 
			// lblUserIcon
			// 
			this->lblUserIcon->AutoSize = true;
			this->lblUserIcon->BackColor = System::Drawing::Color::Transparent;
			this->lblUserIcon->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 14));
			this->lblUserIcon->ForeColor = System::Drawing::Color::FromArgb(150, 160, 175);
			this->lblUserIcon->Text = L"\xE77B";
			// 
			// txtUsername
			// 
			this->txtUsername->BorderStyle = System::Windows::Forms::BorderStyle::None;
           this->txtUsername->BackColor = System::Drawing::Color::FromArgb(245, 247, 250);
			this->txtUsername->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->txtUsername->ForeColor = System::Drawing::Color::Gray;
			this->txtUsername->Text = L"Enter your username";
			this->txtUsername->Enter += gcnew System::EventHandler(this, &MyForm::txtUsername_Enter);
			this->txtUsername->Leave += gcnew System::EventHandler(this, &MyForm::txtUsername_Leave);
			// 
			// lineUsername
			// 
          this->lineUsername->BackColor = System::Drawing::Color::FromArgb(245, 247, 250);
         this->lineUsername->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->lineUsername->Size = System::Drawing::Size(400, 56);
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->lblPassword->ForeColor = System::Drawing::Color::FromArgb(64, 64, 64);
			this->lblPassword->Text = L"Password";
			// 
			// txtPassword
			// 
			this->txtPassword->BorderStyle = System::Windows::Forms::BorderStyle::None;
           this->txtPassword->BackColor = System::Drawing::Color::FromArgb(245, 247, 250);
			this->txtPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->txtPassword->ForeColor = System::Drawing::Color::Gray;
			this->txtPassword->Text = L"••••••••";
			this->txtPassword->Enter += gcnew System::EventHandler(this, &MyForm::txtPassword_Enter);
			this->txtPassword->Leave += gcnew System::EventHandler(this, &MyForm::txtPassword_Leave);
         // 
			// lblLockIcon
			// 
			this->lblLockIcon->AutoSize = true;
			this->lblLockIcon->BackColor = System::Drawing::Color::Transparent;
			this->lblLockIcon->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 14));
			this->lblLockIcon->ForeColor = System::Drawing::Color::FromArgb(150, 160, 175);
			this->lblLockIcon->Text = L"\xE72E";
			// 
			// linePassword
			// 
          this->linePassword->BackColor = System::Drawing::Color::FromArgb(245, 247, 250);
         this->linePassword->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->linePassword->Size = System::Drawing::Size(400, 56);
			// 
			// btnTogglePassword
			// 
			this->btnTogglePassword->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnTogglePassword->FlatAppearance->BorderSize = 0;
         this->btnTogglePassword->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(245, 247, 250);
			this->btnTogglePassword->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(238, 241, 247);
			this->btnTogglePassword->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 12));
			this->btnTogglePassword->ForeColor = System::Drawing::Color::FromArgb(150, 160, 175);
          this->btnTogglePassword->Text = L"\xE18B";
			this->btnTogglePassword->Size = System::Drawing::Size(30, 30);
			this->btnTogglePassword->Click += gcnew System::EventHandler(this, &MyForm::btnTogglePassword_Click);
			// 
			// chkRememberMe
			// 
			this->chkRememberMe->AutoSize = true;
			this->chkRememberMe->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->chkRememberMe->ForeColor = System::Drawing::Color::FromArgb(64, 64, 64);
			this->chkRememberMe->Text = L"Remember me";
			// 
			// linkForgotPassword
			// 
			this->linkForgotPassword->AutoSize = true;
			this->linkForgotPassword->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->linkForgotPassword->LinkColor = System::Drawing::Color::FromArgb(232, 15, 59);
			this->linkForgotPassword->LinkBehavior = System::Windows::Forms::LinkBehavior::NeverUnderline;
			this->linkForgotPassword->Text = L"Forgot password?";
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::FromArgb(232, 15, 59);
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLogin->FlatAppearance->BorderSize = 0;
			this->btnLogin->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold));
			this->btnLogin->ForeColor = System::Drawing::Color::White;
			this->btnLogin->Text = L"Login to Dashboard";
			this->btnLogin->Size = System::Drawing::Size(400, 45);
			this->btnLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// lblNoAccount
			// 
			this->lblNoAccount->AutoSize = true;
			this->lblNoAccount->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->lblNoAccount->ForeColor = System::Drawing::Color::Gray;
			this->lblNoAccount->Text = L"Don't have an account?";
			// 
			// linkSignup
			// 
			this->linkSignup->AutoSize = true;
			this->linkSignup->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold));
			this->linkSignup->LinkColor = System::Drawing::Color::FromArgb(232, 15, 59);
			this->linkSignup->LinkBehavior = System::Windows::Forms::LinkBehavior::NeverUnderline;
			this->linkSignup->Text = L"Sign up";
			this->linkSignup->Click += gcnew System::EventHandler(this, &MyForm::btnSignup_Click);
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->ForeColor = System::Drawing::Color::Red;
			// 
			// rightPanel
			// 
			this->rightPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rightPanel->Controls->Add(this->lblWelcome);
			this->rightPanel->Controls->Add(this->lblWelcomeDesc);
			this->rightPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::rightPanel_Paint);
			// 
			// lblWelcome
			// 
			this->lblWelcome->BackColor = System::Drawing::Color::Transparent;
			this->lblWelcome->Font = (gcnew System::Drawing::Font(L"Segoe UI Black", 42, System::Drawing::FontStyle::Bold));
			this->lblWelcome->ForeColor = System::Drawing::Color::White;
			this->lblWelcome->Text = L"Welcome to\nLifeBlood MMS";
			this->lblWelcome->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lblWelcomeDesc
			// 
			this->lblWelcomeDesc->BackColor = System::Drawing::Color::Transparent;
			this->lblWelcomeDesc->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14));
			this->lblWelcomeDesc->ForeColor = System::Drawing::Color::FromArgb(230, 230, 230);
			this->lblWelcomeDesc->Text = L"Streamlining the connection between generous donors\nand critical needs. Every drop counts in saving lives.";
			this->lblWelcomeDesc->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// MyForm
			// 
			this->ClientSize = System::Drawing::Size(1200, 800);
			this->Controls->Add(this->rightPanel);
			this->Controls->Add(this->leftPanel);
			this->Text = L"LifeBlood MMS Login";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
			this->leftPanel->ResumeLayout(false);
			this->leftPanel->PerformLayout();
			this->rightPanel->ResumeLayout(false);
			this->ResumeLayout(false);
		}
#pragma endregion

	private: System::Void ArrangeLeftPanelControls()
	{
		int xStart = 100; // Left alignment margin based on the image
		int yStart = 150;
		int fieldWidth = 400;

		lblLogo->Location = System::Drawing::Point(xStart, 50);
		lblBrandName->Location = System::Drawing::Point(xStart + 50, 55);
		GraphicsPath^ logoBox = GetRoundedRect(Rectangle(0, 0, lblLogo->Width - 1, lblLogo->Height - 1), 10);
		lblLogo->Region = gcnew System::Drawing::Region(logoBox);
		delete logoBox;

		lblLoginTitle->Location = System::Drawing::Point(xStart - 5, yStart);
		lblLoginSubTitle->Location = System::Drawing::Point(xStart, yStart + 55);

		lblUsername->Location = System::Drawing::Point(xStart, yStart + 120);
      lineUsername->Location = System::Drawing::Point(xStart, yStart + 145);
		lineUsername->Size = System::Drawing::Size(fieldWidth, 56);
     lblUserIcon->Location = System::Drawing::Point(16, 17);
		txtUsername->Location = System::Drawing::Point(52, 17);
		txtUsername->Width = fieldWidth - 68;

      lblPassword->Location = System::Drawing::Point(xStart, yStart + 225);
		linePassword->Location = System::Drawing::Point(xStart, yStart + 250);
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

        chkRememberMe->Location = System::Drawing::Point(xStart, yStart + 340);
		linkForgotPassword->Location = System::Drawing::Point(xStart + fieldWidth - linkForgotPassword->Width, yStart + 340);

		btnLogin->Location = System::Drawing::Point(xStart, yStart + 380);
		btnLogin->Width = fieldWidth;
		GraphicsPath^ loginBox = GetRoundedRect(Rectangle(0, 0, btnLogin->Width - 1, btnLogin->Height - 1), 14);
		btnLogin->Region = gcnew System::Drawing::Region(loginBox);
		delete loginBox;

		lblNoAccount->Location = System::Drawing::Point(xStart + 100, yStart + 440);
		linkSignup->Location = System::Drawing::Point(lblNoAccount->Right + 5, yStart + 440);

		lblStatus->Location = System::Drawing::Point(xStart, yStart + 470);
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

		// Drawing the Translucent "Glass" Cards
		SolidBrush^ glassBrush = gcnew SolidBrush(Color::FromArgb(30, 255, 255, 255)); // Very faint white
		Pen^ glassBorder = gcnew Pen(Color::FromArgb(50, 255, 255, 255), 1);         // Subtle border

		// 1. Top Left Card (Live Status)
		Rectangle card1 = Rectangle(50, 150, 200, 80);
		GraphicsPath^ path1 = GetRoundedRect(card1, 15);
		e->Graphics->FillPath(glassBrush, path1);
		e->Graphics->DrawPath(glassBorder, path1);

      System::Drawing::Font^ f1Small = gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Bold);
		System::Drawing::Font^ f1Large = gcnew System::Drawing::Font(L"Segoe UI Black", 14, System::Drawing::FontStyle::Bold);
		e->Graphics->DrawString(L"LIVE STATUS", f1Small, Brushes::WhiteSmoke, card1.X + 58, card1.Y + 20);
		e->Graphics->DrawString(L"System Active", f1Large, Brushes::White, card1.X + 58, card1.Y + 35);

		// 2. Center Top Icon Card (Heart)
		Rectangle card2 = Rectangle(rightPanel->Width / 2 - 40, 200, 80, 80);
		GraphicsPath^ path2 = GetRoundedRect(card2, 15);
		e->Graphics->FillPath(glassBrush, path2);
		e->Graphics->DrawPath(glassBorder, path2);

		// 3. Bottom Right Card (Total Donations)
		Rectangle card3 = Rectangle(rightPanel->Width - 280, rightPanel->Height - 150, 220, 80);
		GraphicsPath^ path3 = GetRoundedRect(card3, 15);
		e->Graphics->FillPath(glassBrush, path3);
		e->Graphics->DrawPath(glassBorder, path3);
		SolidBrush^ donationBubble = gcnew SolidBrush(Color::FromArgb(105, 178, 34, 74));
		e->Graphics->FillEllipse(donationBubble, card3.X + 18, card3.Y + 14, 52, 52);
		System::Drawing::Font^ dropFont = gcnew System::Drawing::Font(L"Segoe UI Symbol", 16);
		SolidBrush^ dropBrush = gcnew SolidBrush(Color::FromArgb(248, 255, 194, 214));
		e->Graphics->DrawString(L"\xD83D\xDCA7", dropFont, dropBrush, static_cast<float>(card3.X + 30), static_cast<float>(card3.Y + 25));

		e->Graphics->DrawString(L"TOTAL DONATIONS", f1Small, Brushes::WhiteSmoke, card3.X + 78, card3.Y + 20);
		e->Graphics->DrawString(L"12,450", f1Large, Brushes::White, card3.X + 80, card3.Y + 42);
		System::Drawing::Font^ unitsFont = gcnew System::Drawing::Font(L"Segoe UI Black", 12, System::Drawing::FontStyle::Bold);
		SolidBrush^ unitsBrush = gcnew SolidBrush(Color::FromArgb(245, 86, 120));
		e->Graphics->DrawString(L"units", unitsFont, unitsBrush, static_cast<float>(card3.X + 145), static_cast<float>(card3.Y + 45));

		// Cleanup GDI Objects
		delete bgBrush; delete glassBrush; delete glassBorder;
		delete path1; delete path2; delete path3;
     delete donationBubble; delete dropFont; delete dropBrush;
		delete unitsFont; delete unitsBrush;
		delete f1Small; delete f1Large;
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
	};
}