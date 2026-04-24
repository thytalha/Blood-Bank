#pragma once
#include <cmath>
#include "loginForm.h"

namespace BloodBank {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	using namespace System::Reflection; // Required to enable Double Buffering

	public ref class homeForm : public System::Windows::Forms::Form
	{
	public:
		homeForm(void)
		{
			InitializeComponent();

			// Initialize animation angles
			angleO = 0.0f;
			angleA = 2.0f;
			angleB = 4.0f;
			angleAB = 5.5f;

			animTimer = gcnew System::Windows::Forms::Timer();
			animTimer->Interval = 30; // ~33fps
			animTimer->Tick += gcnew System::EventHandler(this, &homeForm::animTimer_Tick);
			animTimer->Start();
		}

	protected:
		~homeForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Panel^ topNavPanel;
	private: System::Windows::Forms::Label^ lblNavContact;
	private: System::Windows::Forms::Label^ lblNavAbout;
	private: System::Windows::Forms::Label^ lblNavLogin;
	private: System::Windows::Forms::Label^ lblNavHome;

	private: System::Windows::Forms::Panel^ leftContentPanel;
	private: System::Windows::Forms::Label^ lblMainHeading;
	private: System::Windows::Forms::Label^ lblSubHeading;
	private: System::Windows::Forms::Button^ btnDashboard;
	private: System::Windows::Forms::Button^ btnLearnMore;
	private: System::Windows::Forms::Label^ lblStatsUnits;
	private: System::Windows::Forms::Label^ lblStatsHospitals;

	private: System::Windows::Forms::Panel^ rightAnimPanel;
	private: System::Windows::Forms::Label^ lblRightWelcome;
	private: System::Windows::Forms::Label^ lblRightDesc;

	private: System::Windows::Forms::Panel^ bottomPanel;
	private: System::Windows::Forms::Panel^ cardManagement;
	private: System::Windows::Forms::Panel^ cardInventory;
	private: System::Windows::Forms::Panel^ cardRequests;

	private: System::Windows::Forms::Timer^ animTimer;
	private: float angleO, angleA, angleB, angleAB;

	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->topNavPanel = (gcnew System::Windows::Forms::Panel());
			this->lblNavContact = (gcnew System::Windows::Forms::Label());
			this->lblNavAbout = (gcnew System::Windows::Forms::Label());
			this->lblNavLogin = (gcnew System::Windows::Forms::Label());
			this->lblNavHome = (gcnew System::Windows::Forms::Label());
			this->leftContentPanel = (gcnew System::Windows::Forms::Panel());
			this->lblStatsHospitals = (gcnew System::Windows::Forms::Label());
			this->lblStatsUnits = (gcnew System::Windows::Forms::Label());
			this->btnLearnMore = (gcnew System::Windows::Forms::Button());
			this->btnDashboard = (gcnew System::Windows::Forms::Button());
			this->lblSubHeading = (gcnew System::Windows::Forms::Label());
			this->lblMainHeading = (gcnew System::Windows::Forms::Label());
			this->rightAnimPanel = (gcnew System::Windows::Forms::Panel());
			this->lblRightDesc = (gcnew System::Windows::Forms::Label());
			this->lblRightWelcome = (gcnew System::Windows::Forms::Label());
			this->bottomPanel = (gcnew System::Windows::Forms::Panel());
			this->cardRequests = (gcnew System::Windows::Forms::Panel());
			this->cardInventory = (gcnew System::Windows::Forms::Panel());
			this->cardManagement = (gcnew System::Windows::Forms::Panel());
			this->topNavPanel->SuspendLayout();
			this->leftContentPanel->SuspendLayout();
			this->rightAnimPanel->SuspendLayout();
			this->bottomPanel->SuspendLayout();
			this->SuspendLayout();

			// topNavPanel
			this->topNavPanel->BackColor = System::Drawing::Color::White;
			this->topNavPanel->Controls->Add(this->lblNavContact);
			this->topNavPanel->Controls->Add(this->lblNavAbout);
			this->topNavPanel->Controls->Add(this->lblNavLogin);
			this->topNavPanel->Controls->Add(this->lblNavHome);
			this->topNavPanel->Dock = System::Windows::Forms::DockStyle::Top;
			this->topNavPanel->Location = System::Drawing::Point(0, 0);
			this->topNavPanel->Name = L"topNavPanel";
			this->topNavPanel->Size = System::Drawing::Size(1200, 80);
			this->topNavPanel->TabIndex = 0;

			// Nav Labels
			System::Drawing::Font^ navFont = gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold);

			this->lblNavContact->AutoSize = true;
			this->lblNavContact->Font = navFont;
			this->lblNavContact->ForeColor = System::Drawing::Color::Gray;
			this->lblNavContact->Text = L"Contact";
			this->lblNavContact->Cursor = System::Windows::Forms::Cursors::Hand;

			this->lblNavAbout->AutoSize = true;
			this->lblNavAbout->Font = navFont;
			this->lblNavAbout->ForeColor = System::Drawing::Color::Gray;
			this->lblNavAbout->Text = L"About";
			this->lblNavAbout->Cursor = System::Windows::Forms::Cursors::Hand;

			this->lblNavLogin->AutoSize = true;
			this->lblNavLogin->Font = navFont;
			this->lblNavLogin->ForeColor = System::Drawing::Color::Gray;
			this->lblNavLogin->Text = L"Login";
			this->lblNavLogin->Cursor = System::Windows::Forms::Cursors::Hand;
			this->lblNavLogin->Click += gcnew System::EventHandler(this, &homeForm::GoToLogin_Click);

			this->lblNavHome->AutoSize = true;
			this->lblNavHome->Font = navFont;
			this->lblNavHome->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->lblNavHome->Text = L"Home";
			this->lblNavHome->Cursor = System::Windows::Forms::Cursors::Hand;

			// rightAnimPanel
			this->rightAnimPanel->Controls->Add(this->lblRightDesc);
			this->rightAnimPanel->Controls->Add(this->lblRightWelcome);
			this->rightAnimPanel->Dock = System::Windows::Forms::DockStyle::Right;
			this->rightAnimPanel->Location = System::Drawing::Point(600, 80);
			this->rightAnimPanel->Name = L"rightAnimPanel";
			this->rightAnimPanel->Size = System::Drawing::Size(600, 500);
			this->rightAnimPanel->TabIndex = 2;
			this->rightAnimPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &homeForm::rightAnimPanel_Paint);

			this->lblRightWelcome->AutoSize = true;
			this->lblRightWelcome->BackColor = System::Drawing::Color::Transparent;
			this->lblRightWelcome->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblRightWelcome->ForeColor = System::Drawing::Color::White;
			this->lblRightWelcome->Text = L"Welcome to LifeBlood MMS";

			this->lblRightDesc->AutoSize = true;
			this->lblRightDesc->BackColor = System::Drawing::Color::Transparent;
			this->lblRightDesc->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));
			this->lblRightDesc->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(240)));
			this->lblRightDesc->Text = L"Streamlining the connection between generous donors and critical needs.";

			// leftContentPanel
			this->leftContentPanel->BackColor = System::Drawing::Color::White;
			this->leftContentPanel->Controls->Add(this->lblStatsHospitals);
			this->leftContentPanel->Controls->Add(this->lblStatsUnits);
			this->leftContentPanel->Controls->Add(this->btnLearnMore);
			this->leftContentPanel->Controls->Add(this->btnDashboard);
			this->leftContentPanel->Controls->Add(this->lblSubHeading);
			this->leftContentPanel->Controls->Add(this->lblMainHeading);
			this->leftContentPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->leftContentPanel->Location = System::Drawing::Point(0, 80);
			this->leftContentPanel->Name = L"leftContentPanel";
			this->leftContentPanel->Size = System::Drawing::Size(600, 500);
			this->leftContentPanel->TabIndex = 1;

			this->lblMainHeading->AutoSize = true;
			this->lblMainHeading->Font = (gcnew System::Drawing::Font(L"Segoe UI", 42, System::Drawing::FontStyle::Bold));
			this->lblMainHeading->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(161)), static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(50)));
			this->lblMainHeading->Text = L"Saving Lives,\nOne Donation at a Time.";

			this->lblSubHeading->AutoSize = true;
			this->lblSubHeading->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12));
			this->lblSubHeading->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)), static_cast<System::Int32>(static_cast<System::Byte>(100)));
			this->lblSubHeading->Text = L"MMS streamlines the connection between generous\ncritical needs. Manage inventory, track donations, and\ns — all in one unified platform built for hospitals,\nand the communities they serve.";

			this->btnDashboard->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->btnDashboard->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnDashboard->FlatAppearance->BorderSize = 0;
			this->btnDashboard->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold));
			this->btnDashboard->ForeColor = System::Drawing::Color::White;
			this->btnDashboard->Size = System::Drawing::Size(150, 45);
			this->btnDashboard->Text = L"Dashboard";
			this->btnDashboard->Cursor = System::Windows::Forms::Cursors::Hand;
			this->btnDashboard->Click += gcnew System::EventHandler(this, &homeForm::GoToLogin_Click);

			this->btnLearnMore->BackColor = System::Drawing::Color::White;
			this->btnLearnMore->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnLearnMore->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(200)), static_cast<System::Int32>(static_cast<System::Byte>(200)), static_cast<System::Int32>(static_cast<System::Byte>(200)));
			this->btnLearnMore->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11, System::Drawing::FontStyle::Bold));
			this->btnLearnMore->ForeColor = System::Drawing::Color::Black;
			this->btnLearnMore->Size = System::Drawing::Size(150, 45);
			this->btnLearnMore->Text = L"Learn More";
			this->btnLearnMore->Cursor = System::Windows::Forms::Cursors::Hand;

			this->lblStatsUnits->AutoSize = true;
			this->lblStatsUnits->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblStatsUnits->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->lblStatsUnits->Text = L"3,280\nUnits in Stock";

			this->lblStatsHospitals->AutoSize = true;
			this->lblStatsHospitals->Font = (gcnew System::Drawing::Font(L"Segoe UI", 20, System::Drawing::FontStyle::Bold));
			this->lblStatsHospitals->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->lblStatsHospitals->Text = L"47\nPartner Hospitals";

			// bottomPanel
			this->bottomPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)), static_cast<System::Int32>(static_cast<System::Byte>(250)), static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->bottomPanel->Controls->Add(this->cardRequests);
			this->bottomPanel->Controls->Add(this->cardInventory);
			this->bottomPanel->Controls->Add(this->cardManagement);
			this->bottomPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->bottomPanel->Location = System::Drawing::Point(0, 580);
			this->bottomPanel->Name = L"bottomPanel";
			this->bottomPanel->Size = System::Drawing::Size(1200, 220);
			this->bottomPanel->TabIndex = 3;

			// Initialize Cards
			SetupCard(this->cardManagement, L"Donor Management", L"Manage donors, track eligibility, and maintain comprehensive health histories.");
			SetupCard(this->cardInventory, L"Safe Inventory", L"Real-time blood unit tracking with automated expiry alerts across all 8 blood groups.");
			SetupCard(this->cardRequests, L"Life-Saving Requests", L"Hospitals and patients request blood; admins approve with a single click — inventory auto-updates.");

			// Form
			this->ClientSize = System::Drawing::Size(1200, 800);
			this->Controls->Add(this->leftContentPanel);
			this->Controls->Add(this->rightAnimPanel);
			this->Controls->Add(this->bottomPanel);
			this->Controls->Add(this->topNavPanel);
			this->Name = L"homeForm";
			this->Text = L"LifeBlood MMS Home";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->BackColor = System::Drawing::Color::White;
			this->Resize += gcnew System::EventHandler(this, &homeForm::homeForm_Resize);
			this->Load += gcnew System::EventHandler(this, &homeForm::homeForm_Load);

			this->topNavPanel->ResumeLayout(false);
			this->topNavPanel->PerformLayout();
			this->leftContentPanel->ResumeLayout(false);
			this->leftContentPanel->PerformLayout();
			this->rightAnimPanel->ResumeLayout(false);
			this->rightAnimPanel->PerformLayout();
			this->bottomPanel->ResumeLayout(false);
			this->ResumeLayout(false);
		}
#pragma endregion

	private: System::Void homeForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		// === FIX: Eliminate flickering on the right panel using Reflection ===
		PropertyInfo^ propInfo = System::Windows::Forms::Panel::typeid->GetProperty(
			L"DoubleBuffered",
			BindingFlags::Instance | BindingFlags::NonPublic
		);
		if (propInfo != nullptr)
		{
			propInfo->SetValue(this->rightAnimPanel, true, nullptr);
		}

		PositionControls();
	}

	private: System::Void homeForm_Resize(System::Object^ sender, System::EventArgs^ e)
	{
		PositionControls();
	}

	private: System::Void PositionControls()
	{
		if (this->ClientSize.Width == 0 || this->ClientSize.Height == 0) return;

		int W = this->ClientSize.Width;
		int H = this->ClientSize.Height;

		// Top Nav
		int navRightEdge = W - 50;
		lblNavContact->Location = System::Drawing::Point(navRightEdge - lblNavContact->Width, 30);
		lblNavAbout->Location = System::Drawing::Point(lblNavContact->Left - lblNavAbout->Width - 30, 30);
		lblNavLogin->Location = System::Drawing::Point(lblNavAbout->Left - lblNavLogin->Width - 30, 30);
		lblNavHome->Location = System::Drawing::Point(lblNavLogin->Left - lblNavHome->Width - 30, 30);

		// Layout Splitting
		rightAnimPanel->Width = static_cast<int>(W * 0.45);

		// Right Panel Text
		lblRightWelcome->Location = System::Drawing::Point(50, rightAnimPanel->Height - 120);
		lblRightDesc->Location = System::Drawing::Point(50, rightAnimPanel->Height - 70);

		// Left Content Panel Formatting
		int leftCx = 50;
		int contentStartY = static_cast<int>(H * 0.15);
		lblMainHeading->Location = System::Drawing::Point(leftCx, contentStartY);
		lblSubHeading->Location = System::Drawing::Point(leftCx + 5, lblMainHeading->Bottom + 20);

		btnDashboard->Location = System::Drawing::Point(leftCx + 5, lblSubHeading->Bottom + 30);
		btnLearnMore->Location = System::Drawing::Point(btnDashboard->Right + 20, lblSubHeading->Bottom + 30);
		RoundControl(btnDashboard, 10);
		RoundControl(btnLearnMore, 10);

		lblStatsUnits->Location = System::Drawing::Point(leftCx + 5, btnDashboard->Bottom + 50);
		lblStatsUnits->Font = gcnew System::Drawing::Font(L"Segoe UI", 12);
		lblStatsHospitals->Location = System::Drawing::Point(lblStatsUnits->Right + 80, btnDashboard->Bottom + 50);
		lblStatsHospitals->Font = gcnew System::Drawing::Font(L"Segoe UI", 12);

		// Bottom Cards
		int cardWidth = (W - 200) / 3;
		int cardSpacing = 50;
		cardManagement->Size = System::Drawing::Size(cardWidth, 160);
		cardInventory->Size = System::Drawing::Size(cardWidth, 160);
		cardRequests->Size = System::Drawing::Size(cardWidth, 160);

		// Ensure Y position defaults to 30 for hover logic
		cardManagement->Location = System::Drawing::Point(50, 30);
		cardInventory->Location = System::Drawing::Point(cardManagement->Right + cardSpacing, 30);
		cardRequests->Location = System::Drawing::Point(cardInventory->Right + cardSpacing, 30);

		RoundControl(cardManagement, 15);
		RoundControl(cardInventory, 15);
		RoundControl(cardRequests, 15);

		// Adjust custom rounded shape for right panel
		GraphicsPath^ rightBox = gcnew GraphicsPath();
		rightBox->AddArc(Rectangle(0, 0, 80, 80), 180, 90);
		rightBox->AddLine(40, 0, rightAnimPanel->Width, 0);
		rightBox->AddLine(rightAnimPanel->Width, 0, rightAnimPanel->Width, rightAnimPanel->Height);
		rightBox->AddArc(Rectangle(0, rightAnimPanel->Height - 80, 80, 80), 90, 90);
		rightBox->CloseFigure();
		rightAnimPanel->Region = gcnew System::Drawing::Region(rightBox);
	}

	private: System::Void SetupCard(Panel^ pnl, String^ title, String^ desc)
	{
     pnl->BackColor = System::Drawing::Color::Transparent;
		pnl->Cursor = System::Windows::Forms::Cursors::Hand;
		pnl->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &homeForm::Card_Paint);

		Label^ lblTitle = gcnew Label();
		lblTitle->Text = title;
        lblTitle->BackColor = System::Drawing::Color::Transparent;
		lblTitle->Font = gcnew System::Drawing::Font(L"Calisto MT", 14, System::Drawing::FontStyle::Bold);
		lblTitle->Location = System::Drawing::Point(20, 60);
		lblTitle->AutoSize = true;

		Label^ lblDesc = gcnew Label();
		lblDesc->Text = desc;
       lblDesc->BackColor = System::Drawing::Color::Transparent;
		lblDesc->Font = gcnew System::Drawing::Font(L"Calisto MT", 10);
		lblDesc->ForeColor = System::Drawing::Color::Gray;
		lblDesc->Location = System::Drawing::Point(20, 90);
		lblDesc->Size = System::Drawing::Size(280, 50);

		pnl->Controls->Add(lblTitle);
		pnl->Controls->Add(lblDesc);

		// Attach consistent event handlers to panel AND its children
		pnl->MouseEnter += gcnew System::EventHandler(this, &homeForm::Card_MouseEnter);
		pnl->MouseLeave += gcnew System::EventHandler(this, &homeForm::Card_MouseLeave);
		lblTitle->MouseEnter += gcnew System::EventHandler(this, &homeForm::Card_MouseEnter);
		lblTitle->MouseLeave += gcnew System::EventHandler(this, &homeForm::Card_MouseLeave);
		lblDesc->MouseEnter += gcnew System::EventHandler(this, &homeForm::Card_MouseEnter);
		lblDesc->MouseLeave += gcnew System::EventHandler(this, &homeForm::Card_MouseLeave);
	}

		   // === FIX: Stable Hover Logic ===
	private: System::Void Card_MouseEnter(System::Object^ sender, System::EventArgs^ e) {
		Control^ ctrl = safe_cast<Control^>(sender);
		// Find the parent panel if a label triggered the event
		Panel^ pnl = (ctrl->GetType() == Panel::typeid) ? safe_cast<Panel^>(ctrl) : safe_cast<Panel^>(ctrl->Parent);

		// Fixed movement check instead of continuous subtraction
		if (pnl->Top == 30) {
			pnl->Top = 25;
           pnl->Invalidate();
		}
	}

	private: System::Void Card_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
		Control^ ctrl = safe_cast<Control^>(sender);
		Panel^ pnl = (ctrl->GetType() == Panel::typeid) ? safe_cast<Panel^>(ctrl) : safe_cast<Panel^>(ctrl->Parent);

		// Only reset position if the mouse actually left the entire panel bounds
		Point mousePos = pnl->PointToClient(Control::MousePosition);
		if (!pnl->ClientRectangle.Contains(mousePos)) {
			pnl->Top = 30;
         pnl->Invalidate();
		}
	}

	private: System::Void Card_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		Panel^ pnl = safe_cast<Panel^>(sender);
		e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;

		Rectangle rect = Rectangle(0, 0, pnl->Width - 1, pnl->Height - 1);
		if (rect.Width <= 0 || rect.Height <= 0) return;

		GraphicsPath^ path = gcnew GraphicsPath();
		int radius = 15;
		int d = radius * 2;
		Rectangle arc = Rectangle(rect.X, rect.Y, d, d);
		path->AddArc(arc, 180, 90);
		arc.X = rect.Right - d;
		path->AddArc(arc, 270, 90);
		arc.Y = rect.Bottom - d;
		path->AddArc(arc, 0, 90);
		arc.X = rect.X;
		path->AddArc(arc, 90, 90);
		path->CloseFigure();

		Rectangle shadowRect = rect;
		shadowRect.Offset(0, 2);
		GraphicsPath^ shadowPath = gcnew GraphicsPath();
		Rectangle shadowArc = Rectangle(shadowRect.X, shadowRect.Y, d, d);
		shadowPath->AddArc(shadowArc, 180, 90);
		shadowArc.X = shadowRect.Right - d;
		shadowPath->AddArc(shadowArc, 270, 90);
		shadowArc.Y = shadowRect.Bottom - d;
		shadowPath->AddArc(shadowArc, 0, 90);
		shadowArc.X = shadowRect.X;
		shadowPath->AddArc(shadowArc, 90, 90);
		shadowPath->CloseFigure();

		SolidBrush^ shadowBrush = gcnew SolidBrush(Color::FromArgb(20, 0, 0, 0));
		e->Graphics->FillPath(shadowBrush, shadowPath);

		bool isHover = (pnl->Top == 25);
		Color glassTop = isHover ? Color::FromArgb(210, 255, 255, 255) : Color::FromArgb(185, 255, 255, 255);
		Color glassBottom = isHover ? Color::FromArgb(150, 255, 255, 255) : Color::FromArgb(120, 255, 255, 255);
		LinearGradientBrush^ glassBrush = gcnew LinearGradientBrush(rect, glassTop, glassBottom, LinearGradientMode::Vertical);
		e->Graphics->FillPath(glassBrush, path);

		Pen^ borderPen = gcnew Pen(Color::FromArgb(150, 255, 255, 255), 1.4f);
		e->Graphics->DrawPath(borderPen, path);

		Pen^ highlightPen = gcnew Pen(Color::FromArgb(115, 255, 255, 255), 1.0f);
		e->Graphics->DrawLine(highlightPen, rect.X + 16, rect.Y + 10, rect.Right - 16, rect.Y + 10);

		delete highlightPen;
		delete borderPen;
		delete glassBrush;
		delete shadowBrush;
		delete shadowPath;
		delete path;
	}

	private: System::Void RoundControl(Control^ ctrl, int radius)
	{
		GraphicsPath^ path = gcnew GraphicsPath();
		int diameter = radius * 2;
		Rectangle arc = Rectangle(0, 0, diameter, diameter);
		path->AddArc(arc, 180, 90);
		arc.X = ctrl->Width - diameter;
		path->AddArc(arc, 270, 90);
		arc.Y = ctrl->Height - diameter;
		path->AddArc(arc, 0, 90);
		arc.X = 0;
		path->AddArc(arc, 90, 90);
		path->CloseFigure();
		ctrl->Region = gcnew System::Drawing::Region(path);
	}

		   // === ANIMATION ENGINE ===
	private: System::Void animTimer_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		// Increment angles to create orbiting effect
		angleO -= 0.02f;
		angleA -= 0.025f;
		angleB -= 0.015f;
		angleAB -= 0.018f;

		rightAnimPanel->Invalidate(); // Trigger repaint smoothly now that DoubleBuffered is ON
	}

	private: System::Void rightAnimPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
	{
		e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;

		// Gradient Background
		Rectangle rect = rightAnimPanel->ClientRectangle;
		if (rect.Width <= 0 || rect.Height <= 0) return; // Crash prevention on minimize

		LinearGradientBrush^ bgBrush = gcnew LinearGradientBrush(rect,
			Color::FromArgb(210, 10, 45),
			Color::FromArgb(60, 20, 35),
			LinearGradientMode::ForwardDiagonal);
		e->Graphics->FillRectangle(bgBrush, rect);
		delete bgBrush;

		int cx = rightAnimPanel->Width / 2;
		int cy = rightAnimPanel->Height / 2 - 40;

		// 1. Draw Main Big Blood Drop
		DrawBloodDrop(e->Graphics, cx, cy, 140, Color::White, 4.0f);

		// 2. Draw Orbiting Blood Types
		int orbitRadius = 180;

		DrawOrbitingDrop(e->Graphics, cx, cy, orbitRadius, angleO, L"O-");
		DrawOrbitingDrop(e->Graphics, cx, cy, orbitRadius + 40, angleA, L"A+");
		DrawOrbitingDrop(e->Graphics, cx, cy, orbitRadius - 20, angleB, L"B+");
		DrawOrbitingDrop(e->Graphics, cx, cy, orbitRadius + 70, angleAB, L"AB+");
	}

	private: System::Void DrawBloodDrop(Graphics^ g, int cx, int cy, int size, Color color, float thickness)
	{
		Pen^ pen = gcnew Pen(color, thickness);
		GraphicsPath^ path = gcnew GraphicsPath();

		int topY = cy - size;
		int bottomY = cy + (size / 2);

		path->StartFigure();
		// Curved drop shape using Bezier
		path->AddBezier(
			Point(cx, topY),
			Point(cx + (static_cast<int>(size / 1.5)), cy),
			Point(cx + (static_cast<int>(size / 1.5)), bottomY),
			Point(cx, bottomY + (size / 3))
		);
		path->AddBezier(
			Point(cx, bottomY + (size / 3)),
			Point(cx - (static_cast<int>(size / 1.5)), bottomY),
			Point(cx - (static_cast<int>(size / 1.5)), cy),
			Point(cx, topY)
		);
		path->CloseFigure();
		g->DrawPath(pen, path);
		delete pen;
		delete path;
	}

	private: System::Void DrawOrbitingDrop(Graphics^ g, int cx, int cy, int radius, float angle, String^ text)
	{
		int x = cx + static_cast<int>(radius * std::cos(angle));
		int y = cy + static_cast<int>(radius * std::sin(angle));

		// Draw small drop
		DrawBloodDrop(g, x, y, 20, Color::FromArgb(200, 255, 255, 255), 2.0f);

		// Draw Text next to it
		System::Drawing::Font^ textFont = gcnew System::Drawing::Font(L"Calisto MT", 10, System::Drawing::FontStyle::Bold);
		SolidBrush^ b = gcnew SolidBrush(Color::White);
		g->DrawString(text, textFont, b, x + 20, y - 10);

		delete textFont;
		delete b;
	}

	private: System::Void GoToLogin_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->Hide();
		BloodBank::loginForm^ login = gcnew BloodBank::loginForm();
		login->ShowDialog();
		this->Show();
	}
	};
}