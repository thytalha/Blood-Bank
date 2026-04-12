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
         this->leftPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(248)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
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
			this->leftPanel->TabStop = true;
			this->leftPanel->TabIndex = 0;
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->Font = (gcnew System::Drawing::Font(L"Calisto MT", 12));
			this->lblStatus->ForeColor = System::Drawing::Color::OrangeRed;
            this->lblStatus->Location = System::Drawing::Point(70, 597);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Size = System::Drawing::Size(0, 20);
			this->lblStatus->TabIndex = 9;
			// 
			// btnSignup
			// 
			this->btnSignup->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(221)), static_cast<System::Int32>(static_cast<System::Byte>(229)),
				static_cast<System::Int32>(static_cast<System::Byte>(245)));
			this->btnSignup->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnSignup->FlatAppearance->BorderSize = 0;
			this->btnSignup->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnSignup->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
			this->btnSignup->ForeColor = System::Drawing::Color::FromArgb(0, 0, 0);
			this->btnSignup->Location = System::Drawing::Point(285, 685);
			this->btnSignup->Name = L"btnSignup";
			this->btnSignup->Size = System::Drawing::Size(120, 42);
			this->btnSignup->TabIndex = 8;
			this->btnSignup->Text = L"Sign up";
			this->btnSignup->UseVisualStyleBackColor = false;
			this->btnSignup->Click += gcnew System::EventHandler(this, &MyForm::btnSignup_Click);
			// 
			// lblNoAccount
			// 
			this->lblNoAccount->AutoSize = true;
            this->lblNoAccount->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
			this->lblNoAccount->ForeColor = System::Drawing::Color::FromArgb(0, 0, 0);
			this->lblNoAccount->Location = System::Drawing::Point(70, 693);
			this->lblNoAccount->Name = L"lblNoAccount";
			this->lblNoAccount->Size = System::Drawing::Size(171, 20);
			this->lblNoAccount->TabIndex = 7;
			this->lblNoAccount->Text = L"Don\'t have an account?";
			// 
			// btnLogin
			// 
			this->btnLogin->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(220)), static_cast<System::Int32>(static_cast<System::Byte>(20)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->btnLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnLogin->FlatAppearance->BorderSize = 0;
			this->btnLogin->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->btnLogin->Font = (gcnew System::Drawing::Font(L"Calisto MT", 15));
			this->btnLogin->ForeColor = System::Drawing::Color::White;
			this->btnLogin->Location = System::Drawing::Point(70, 527);
			this->btnLogin->Name = L"btnLogin";
			this->btnLogin->Size = System::Drawing::Size(540, 50);
			this->btnLogin->TabIndex = 6;
			this->btnLogin->Text = L"Login";
			this->btnLogin->UseVisualStyleBackColor = false;
			this->btnLogin->Click += gcnew System::EventHandler(this, &MyForm::btnLogin_Click);
			// 
			// txtPassword
			// 
			this->txtPassword->BackColor = System::Drawing::Color::White;
			this->txtPassword->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtPassword->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
			this->txtPassword->ForeColor = System::Drawing::Color::Silver;
			this->txtPassword->Location = System::Drawing::Point(70, 449);
			this->txtPassword->Name = L"txtPassword";
			this->txtPassword->AutoSize = false;
			this->txtPassword->Multiline = false;
			this->txtPassword->Size = System::Drawing::Size(540, 40);
			this->txtPassword->TabIndex = 5;
			this->txtPassword->Text = L"Password";
			this->txtPassword->Enter += gcnew System::EventHandler(this, &MyForm::txtPassword_Enter);
			this->txtPassword->Leave += gcnew System::EventHandler(this, &MyForm::txtPassword_Leave);
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
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
			this->txtUsername->BackColor = System::Drawing::Color::White;
			this->txtUsername->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtUsername->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
			this->txtUsername->ForeColor = System::Drawing::Color::Silver;
			this->txtUsername->Location = System::Drawing::Point(70, 364);
			this->txtUsername->Name = L"txtUsername";
			this->txtUsername->AutoSize = false;
			this->txtUsername->Multiline = false;
			this->txtUsername->Size = System::Drawing::Size(540, 40);

			this->txtUsername->TabIndex = 3;
			this->txtUsername->Text = L"Username";
			this->txtUsername->Enter += gcnew System::EventHandler(this, &MyForm::txtUsername_Enter);
			this->txtUsername->Leave += gcnew System::EventHandler(this, &MyForm::txtUsername_Leave);
			// 
			// lblUsername
			// 
			this->lblUsername->AutoSize = true;
			this->lblUsername->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
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
			this->lblLoginSubTitle->Font = (gcnew System::Drawing::Font(L"Calisto MT", 13));
			this->lblLoginSubTitle->ForeColor = System::Drawing::Color::FromArgb(0, 0, 0);
			this->lblLoginSubTitle->Location = System::Drawing::Point(70, 225);
			this->lblLoginSubTitle->Name = L"lblLoginSubTitle";
			this->lblLoginSubTitle->Size = System::Drawing::Size(158, 20);
			this->lblLoginSubTitle->TabIndex = 1;
			this->lblLoginSubTitle->Text = L"Enter your account details";
			// 
			// lblLoginTitle
			// 
			this->lblLoginTitle->AutoSize = false;
			this->lblLoginTitle->Font = (gcnew System::Drawing::Font(L"Calisto MT", 36, System::Drawing::FontStyle::Bold));
			this->lblLoginTitle->ForeColor = System::Drawing::Color::FromArgb(0, 0, 0);
			this->lblLoginTitle->Location = System::Drawing::Point(62, 140);
			this->lblLoginTitle->Name = L"lblLoginTitle";
            this->lblLoginTitle->Size = System::Drawing::Size(700, 90);
			this->lblLoginTitle->TabIndex = 0;
			this->lblLoginTitle->Text = L"Login to your account";
			this->lblLoginTitle->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->lblLoginTitle->UseCompatibleTextRendering = true;
			// 
			// rightPanel
			// 
          this->rightPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(186)), static_cast<System::Int32>(static_cast<System::Byte>(30)),
				static_cast<System::Int32>(static_cast<System::Byte>(46)));
			this->rightPanel->Controls->Add(this->lblWelcome);
			this->rightPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rightPanel->Location = System::Drawing::Point(950, 0);
			this->rightPanel->Name = L"rightPanel";
			this->rightPanel->Size = System::Drawing::Size(740, 800);
			this->rightPanel->TabIndex = 1;
			this->rightPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::rightPanel_Paint);
			// 
			// lblWelcome
			// 
			this->lblWelcome->AutoSize = false;
            this->lblWelcome->BackColor = System::Drawing::Color::Transparent;
			this->lblWelcome->Font = (gcnew System::Drawing::Font(L"Calisto MT", 36, System::Drawing::FontStyle::Bold));
			this->lblWelcome->ForeColor = System::Drawing::Color::White;
			this->lblWelcome->Location = System::Drawing::Point(80, 195);
			this->lblWelcome->Name = L"lblWelcome";
            this->lblWelcome->Size = System::Drawing::Size(900, 100);
			this->lblWelcome->TabIndex = 0;
			this->lblWelcome->Text = L"Welcome to LifeBlood MMS";
			this->lblWelcome->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->lblWelcome->UseCompatibleTextRendering = true;
			// 
			// lineUsername
			// 
			this->lineUsername->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(180)), static_cast<System::Int32>(static_cast<System::Byte>(191)),
				static_cast<System::Int32>(static_cast<System::Byte>(216)));
			this->lineUsername->Location = System::Drawing::Point(70, 401);
			this->lineUsername->Name = L"lineUsername";
			this->lineUsername->Size = System::Drawing::Size(540, 2);
			this->lineUsername->TabIndex = 10;
			// 
			// linePassword
			// 
			this->linePassword->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(180)), static_cast<System::Int32>(static_cast<System::Byte>(191)),
				static_cast<System::Int32>(static_cast<System::Byte>(216)));
			this->linePassword->Location = System::Drawing::Point(70, 486);
			this->linePassword->Name = L"linePassword";
			this->linePassword->Size = System::Drawing::Size(540, 2);
			this->linePassword->TabIndex = 11;
			// 
			// btnTogglePassword
			// 
			this->btnTogglePassword->BackColor = System::Drawing::Color::Transparent;
			this->btnTogglePassword->Cursor = System::Windows::Forms::Cursors::Hand;
            this->btnTogglePassword->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(26)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->btnTogglePassword->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(24)), static_cast<System::Int32>(static_cast<System::Byte>(26)),
				static_cast<System::Int32>(static_cast<System::Byte>(36)));
			this->btnTogglePassword->FlatAppearance->BorderSize = 0;
			this->btnTogglePassword->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
         this->btnTogglePassword->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 12));
            this->btnTogglePassword->ForeColor = System::Drawing::Color::FromArgb(72, 86, 118);
           this->btnTogglePassword->Location = System::Drawing::Point(536, 455);
			this->btnTogglePassword->Name = L"btnTogglePassword";
          this->btnTogglePassword->Size = System::Drawing::Size(28, 22);
			this->btnTogglePassword->TabIndex = 12;
          this->btnTogglePassword->TabStop = false;
        this->btnTogglePassword->Text = L"\xE18B";
			this->btnTogglePassword->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->btnTogglePassword->UseVisualStyleBackColor = false;
			this->btnTogglePassword->Click += gcnew System::EventHandler(this, &MyForm::btnTogglePassword_Click);
         this->btnTogglePassword->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::btnTogglePassword_Paint);
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
           this->Shown += gcnew System::EventHandler(this, &MyForm::MyForm_Shown);
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
     int x = System::Math::Max(20, ((leftPanel->ClientSize.Width - fieldWidth) / 2) - 75);

		lblLoginTitle->Left = x;
     lblLoginTitle->Width = fieldWidth + 220;
	 lblLoginSubTitle->Left = x + 3;
		lblUsername->Left = x;
		txtUsername->Left = x;
      lineUsername->Left = x;
		lineUsername->Width = fieldWidth;
		lblPassword->Left = x;
		txtPassword->Left = x;
     linePassword->Left = x;
		linePassword->Width = fieldWidth;
		btnTogglePassword->Left = x + fieldWidth - btnTogglePassword->Width;
      btnTogglePassword->Left = x + fieldWidth - btnTogglePassword->Width - 6;
		btnTogglePassword->Top = txtPassword->Top + ((txtPassword->Height - btnTogglePassword->Height) / 2);
		txtPassword->Width = fieldWidth;
		btnLogin->Left = x;
        btnLogin->Width = fieldWidth;
		lblStatus->Left = x;

		lblNoAccount->Left = x;
        btnSignup->Left = lblNoAccount->Right + 50;
		btnSignup->Top = lblNoAccount->Top - ((btnSignup->Height - lblNoAccount->Height) / 2);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		ArrangeLeftPanelControls();
       this->ActiveControl = nullptr;
		this->Focus();
	}

	private: System::Void MyForm_Shown(System::Object^ sender, System::EventArgs^ e)
	{
		this->BeginInvoke(gcnew MethodInvoker(this, &MyForm::ClearInitialFocus));
	}

	private: System::Void ClearInitialFocus()
	{
		this->ActiveControl = leftPanel;
		leftPanel->Select();
		leftPanel->Focus();
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
         txtUsername->ForeColor = System::Drawing::Color::FromArgb(44, 62, 100);
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
       if (txtPassword->Text == L"Password")
		{
			txtPassword->Text = L"";
         txtPassword->ForeColor = System::Drawing::Color::FromArgb(44, 62, 100);
			txtPassword->UseSystemPasswordChar = !isPasswordVisible;
		}
	}

	private: System::Void txtPassword_Leave(System::Object^ sender, System::EventArgs^ e)
	{
		if (String::IsNullOrWhiteSpace(txtPassword->Text))
		{
			txtPassword->UseSystemPasswordChar = false;
            isPasswordVisible = false;
			txtPassword->Text = L"Password";
           txtPassword->ForeColor = System::Drawing::Color::Silver;
          btnTogglePassword->Text = L"\xE18B";
			btnTogglePassword->Invalidate();
		}
	}

	private: System::Void btnTogglePassword_Click(System::Object^ sender, System::EventArgs^ e)
	{
       if (txtPassword->Text == L"Password")
		{
			return;
		}

		isPasswordVisible = !isPasswordVisible;
		txtPassword->UseSystemPasswordChar = !isPasswordVisible;
        btnTogglePassword->Text = L"\xE18B";
		btnTogglePassword->Invalidate();
	}

	private: System::Void btnTogglePassword_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		if (!isPasswordVisible)
		{
			return;
		}

		e->Graphics->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
        Pen^ slashPen = gcnew Pen(System::Drawing::Color::FromArgb(72, 86, 118), 2.2f);
		e->Graphics->DrawLine(slashPen, 4, btnTogglePassword->Height - 4, btnTogglePassword->Width - 4, 4);
	}

	private: GraphicsPath^ CreateRoundedPath(Rectangle rect, int radius)
	{
		GraphicsPath^ path = gcnew GraphicsPath();
		int d = radius * 2;
		path->AddArc(rect.X, rect.Y, d, d, 180, 90);
		path->AddArc(rect.Right - d, rect.Y, d, d, 270, 90);
		path->AddArc(rect.Right - d, rect.Bottom - d, d, d, 0, 90);
		path->AddArc(rect.X, rect.Bottom - d, d, d, 90, 90);
		path->CloseFigure();
		return path;
	}

	private: GraphicsPath^ CreateRightRoundedPath(Rectangle rect, int radius)
	{
		GraphicsPath^ path = gcnew GraphicsPath();
		int d = radius * 2;
		path->StartFigure();
		path->AddLine(rect.X, rect.Y, rect.Right - d, rect.Y);
		path->AddArc(rect.Right - d, rect.Y, d, d, 270, 90);
		path->AddLine(rect.Right, rect.Y + d, rect.Right, rect.Bottom - d);
		path->AddArc(rect.Right - d, rect.Bottom - d, d, d, 0, 90);
		path->AddLine(rect.Right - d, rect.Bottom, rect.X, rect.Bottom);
		path->AddLine(rect.X, rect.Bottom, rect.X, rect.Y);
		path->CloseFigure();
		return path;
	}

	private: System::Void rightPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;

     Rectangle card = Rectangle(0, 18, rightPanel->Width - 26, rightPanel->Height - 58);
		Rectangle shadowRect = Rectangle(card.X + 10, card.Y + 14, card.Width, card.Height);

       GraphicsPath^ shadowPath = CreateRightRoundedPath(shadowRect, 28);
      SolidBrush^ shadowBrush = gcnew SolidBrush(Color::FromArgb(35, 80, 30, 45));
		e->Graphics->FillPath(shadowBrush, shadowPath);

       GraphicsPath^ cardPath = CreateRightRoundedPath(card, 28);
        LinearGradientBrush^ cardBrush = gcnew LinearGradientBrush(card,
			Color::FromArgb(236, 176, 22, 38),
			Color::FromArgb(228, 120, 10, 24),
			LinearGradientMode::Vertical);
		e->Graphics->FillPath(cardBrush, cardPath);

       Pen^ cardEdge = gcnew Pen(Color::FromArgb(110, 255, 210, 210), 1.4f);
		e->Graphics->DrawPath(cardEdge, cardPath);


		SolidBrush^ bubbleA = gcnew SolidBrush(Color::FromArgb(38, 255, 255, 255));
		SolidBrush^ bubbleB = gcnew SolidBrush(Color::FromArgb(26, 255, 255, 255));
		SolidBrush^ bubbleC = gcnew SolidBrush(Color::FromArgb(18, 255, 255, 255));

		array<Point>^ blob1 = gcnew array<Point>{ Point(card.X - 90, card.Y + 220), Point(card.X + 40, card.Y + 120), Point(card.X + 170, card.Y + 200), Point(card.X + 135, card.Y + 355), Point(card.X - 30, card.Y + 350) };
		array<Point>^ blob2 = gcnew array<Point>{ Point(card.X + 180, card.Y - 20), Point(card.X + 295, card.Y - 10), Point(card.X + 345, card.Y + 85), Point(card.X + 250, card.Y + 170), Point(card.X + 150, card.Y + 115) };
		array<Point>^ blob3 = gcnew array<Point>{ Point(card.Right - 240, card.Y + 40), Point(card.Right - 120, card.Y + 5), Point(card.Right - 20, card.Y + 80), Point(card.Right - 55, card.Y + 220), Point(card.Right - 195, card.Y + 200) };
		array<Point>^ blob4 = gcnew array<Point>{ Point(card.Right - 265, card.Bottom - 155), Point(card.Right - 120, card.Bottom - 220), Point(card.Right + 8, card.Bottom - 120), Point(card.Right - 40, card.Bottom + 5), Point(card.Right - 190, card.Bottom + 12) };
		array<Point>^ blob5 = gcnew array<Point>{ Point(card.X - 80, card.Bottom - 210), Point(card.X + 40, card.Bottom - 255), Point(card.X + 135, card.Bottom - 170), Point(card.X + 90, card.Bottom - 55), Point(card.X - 35, card.Bottom - 80) };
		array<Point>^ blob6 = gcnew array<Point>{ Point(card.X + 300, card.Y + 245), Point(card.X + 430, card.Y + 205), Point(card.X + 515, card.Y + 295), Point(card.X + 480, card.Y + 410), Point(card.X + 340, card.Y + 385) };
		array<Point>^ blob7 = gcnew array<Point>{ Point(card.X + 65, card.Y + 28), Point(card.X + 135, card.Y + 12), Point(card.X + 200, card.Y + 44), Point(card.X + 180, card.Y + 98), Point(card.X + 90, card.Y + 92) };
		array<Point>^ blob8 = gcnew array<Point>{ Point(card.Right - 120, card.Y + 250), Point(card.Right - 32, card.Y + 248), Point(card.Right + 6, card.Y + 320), Point(card.Right - 48, card.Y + 380), Point(card.Right - 130, card.Y + 350) };
		array<Point>^ blob9 = gcnew array<Point>{ Point(card.X + 210, card.Bottom - 120), Point(card.X + 305, card.Bottom - 170), Point(card.X + 405, card.Bottom - 130), Point(card.X + 395, card.Bottom - 48), Point(card.X + 270, card.Bottom - 40) };
		array<Point>^ blob10 = gcnew array<Point>{ Point(card.X + 460, card.Y + 118), Point(card.X + 560, card.Y + 98), Point(card.X + 610, card.Y + 162), Point(card.X + 570, card.Y + 228), Point(card.X + 478, card.Y + 205) };

		e->Graphics->FillClosedCurve(bubbleA, blob1, FillMode::Winding, 0.45f);
		e->Graphics->FillClosedCurve(bubbleB, blob2, FillMode::Winding, 0.45f);
		e->Graphics->FillClosedCurve(bubbleA, blob3, FillMode::Winding, 0.45f);
		e->Graphics->FillClosedCurve(bubbleB, blob4, FillMode::Winding, 0.45f);
		e->Graphics->FillClosedCurve(bubbleB, blob5, FillMode::Winding, 0.45f);
		e->Graphics->FillClosedCurve(bubbleA, blob6, FillMode::Winding, 0.45f);
		e->Graphics->FillClosedCurve(bubbleC, blob7, FillMode::Winding, 0.40f);
		e->Graphics->FillClosedCurve(bubbleC, blob8, FillMode::Winding, 0.40f);
		e->Graphics->FillClosedCurve(bubbleB, blob9, FillMode::Winding, 0.40f);
		e->Graphics->FillClosedCurve(bubbleC, blob10, FillMode::Winding, 0.40f);

		e->Graphics->FillEllipse(bubbleC, card.X + 28, card.Y + 148, 36, 26);
		e->Graphics->FillEllipse(bubbleC, card.X + 420, card.Y + 66, 28, 20);
		e->Graphics->FillEllipse(bubbleC, card.X + 510, card.Bottom - 88, 32, 24);
		e->Graphics->FillEllipse(bubbleC, card.X + 150, card.Bottom - 56, 24, 18);

		delete bubbleA;
		delete bubbleB;
       delete bubbleC;
		delete cardEdge;
		delete cardBrush;
		delete shadowBrush;
		delete cardPath;
		delete shadowPath;
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
