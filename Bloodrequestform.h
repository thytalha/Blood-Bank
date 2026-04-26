#pragma once
#include "RecipientForm.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Collections::Generic;

// NOTE: RecipientForm.h must be included before this file
// so RoundedPanel and StatCard are already defined.

namespace BloodBank {

	// ─── Request urgency levels ───────────────────────────────────────────────
	public enum class Urgency { Routine, Urgent, Critical };

	// ─── Request status ───────────────────────────────────────────────────────
	public enum class RequestStatus { Pending, Approved, Fulfilled, Cancelled };

	// ─── Data Model ───────────────────────────────────────────────────────────
	public ref class BloodRequest {
	public:
		static int  NextID = 1000;
		int         requestID;
		String^ patientName;
		String^ bloodGroup;
		int         unitsNeeded;
		String^ hospital;
		String^ doctorName;
		String^ contactNumber;
		Urgency     urgencyLevel;
		RequestStatus status;
		String^ notes;
		DateTime    requestedOn;

		BloodRequest(String^ pn, String^ bg, int u, String^ h,
			String^ doc, String^ c, Urgency urg, String^ n) {
			requestID = NextID++;
			patientName = pn;
			bloodGroup = bg;
			unitsNeeded = u;
			hospital = h;
			doctorName = doc;
			contactNumber = c;
			urgencyLevel = urg;
			status = RequestStatus::Pending;
			notes = n;
			requestedOn = DateTime::Now;
		}

		String^ UrgencyText() {
			switch (urgencyLevel) {
			case Urgency::Critical: return "CRITICAL";
			case Urgency::Urgent:   return "URGENT";
			default:                return "Routine";
			}
		}

		String^ StatusText() {
			switch (status) {
			case RequestStatus::Approved:   return "Approved";
			case RequestStatus::Fulfilled:  return "Fulfilled";
			case RequestStatus::Cancelled:  return "Cancelled";
			default:                        return "Pending";
			}
		}
	};

	// ─── Blood Request Form ───────────────────────────────────────────────────
	public ref class BloodRequestForm : public Form
	{
	private:
		List<BloodRequest^>^ requests;

		// Input controls
		TextBox^ txtPatient;
		ComboBox^ comboBlood;
		NumericUpDown^ numUnits;
		TextBox^ txtHospital;
		TextBox^ txtDoctor;
		TextBox^ txtContact;
		ComboBox^ comboUrgency;
		TextBox^ txtNotes;
		TextBox^ txtSearch;

		// Buttons
		Button^ btnSubmit;
		Button^ btnApprove;
		Button^ btnFulfill;
		Button^ btnCancel;
		Button^ btnDelete;
		Button^ btnClear;

		// Display
		ListView^ listView;
		Label^ lblStatus;

		// Stat cards
		StatCard^ cardTotal;
		StatCard^ cardPending;
		StatCard^ cardCritical;
		StatCard^ cardFulfilled;

		// Colours (mirror RecipientForm palette)
		static Color CRIMSON = Color::FromArgb(185, 28, 28);
		static Color CRIMSON_LT = Color::FromArgb(220, 38, 38);
		static Color CRIMSON_BG = Color::FromArgb(254, 242, 242);
		static Color DARK_TEXT = Color::FromArgb(30, 30, 30);
		static Color MID_GRAY = Color::FromArgb(107, 114, 128);
		static Color LIGHT_BG = Color::FromArgb(249, 250, 251);
		static Color ACCENT_GRN = Color::FromArgb(5, 150, 105);
		static Color ACCENT_AMB = Color::FromArgb(217, 119, 6);
		static Color ACCENT_BLU = Color::FromArgb(37, 99, 235);
		static Color BORDER = Color::FromArgb(229, 231, 235);

	public:
		BloodRequestForm(void) {
			requests = gcnew List<BloodRequest^>();
			this->DoubleBuffered = true;
			InitializeComponent();
		}

	private:
		// ── Small helpers ─────────────────────────────────────────────────────

		Label^ MakeFieldLabel(String^ text) {
			Label^ l = gcnew Label();
			l->Text = text;
			l->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold);
			l->ForeColor = MID_GRAY;
			l->AutoSize = true;
			return l;
		}

		TextBox^ MakeTextBox(int width) {
			TextBox^ t = gcnew TextBox();
			t->Font = gcnew Drawing::Font("Segoe UI", 10);
			t->BorderStyle = BorderStyle::FixedSingle;
			t->BackColor = Color::White;
			t->Width = width;
			return t;
		}

		ComboBox^ MakeComboBox(int width) {
			ComboBox^ c = gcnew ComboBox();
			c->Font = gcnew Drawing::Font("Segoe UI", 10);
			c->DropDownStyle = ComboBoxStyle::DropDownList;
			c->FlatStyle = FlatStyle::Flat;
			c->Width = width;
			return c;
		}

		Button^ MakeButton(String^ text, Color bg, Color fg, int w, int h) {
			Button^ b = gcnew Button();
			b->Text = text;
			b->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
			b->FlatStyle = FlatStyle::Flat;
			b->FlatAppearance->BorderSize = 0;
			b->BackColor = bg;
			b->ForeColor = fg;
			b->Size = Drawing::Size(w, h);
			b->Cursor = Cursors::Hand;
			return b;
		}

		void AddField(Panel^ p, String^ labelText, Control^ ctrl, int% y) {
			Label^ lbl = MakeFieldLabel(labelText);
			lbl->Location = Point(24, y);
			p->Controls->Add(lbl);
			y += 20;
			ctrl->Location = Point(24, y);
			p->Controls->Add(ctrl);
			y += ctrl->Height + 14;
		}

		// ── Layout ────────────────────────────────────────────────────────────
		void InitializeComponent(void)
		{
			this->Text = "LifeBlood — Blood Request Management";
			this->WindowState = FormWindowState::Maximized;
			this->BackColor = LIGHT_BG;
			this->Font = gcnew Drawing::Font("Segoe UI", 10);
			this->MinimumSize = Drawing::Size(1150, 720);

			// ════════════════════════════════════════════
			//  HEADER
			// ════════════════════════════════════════════
			Panel^ header = gcnew Panel();
			header->Dock = DockStyle::Top;
			header->Height = 72;
			header->BackColor = CRIMSON;
			header->Paint += gcnew PaintEventHandler(this, &BloodRequestForm::DrawHeaderGradient);

			Label^ logoMark = gcnew Label();
			logoMark->Text = L"💉";
			logoMark->Font = gcnew Drawing::Font("Segoe UI Emoji", 26);
			logoMark->ForeColor = Color::White;
			logoMark->AutoSize = true;
			logoMark->Location = Point(24, 14);
			logoMark->BackColor = Color::Transparent;

			Label^ sysTitle = gcnew Label();
			sysTitle->Text = "LifeBlood";
			sysTitle->Font = gcnew Drawing::Font("Georgia", 22, FontStyle::Bold | FontStyle::Italic);
			sysTitle->ForeColor = Color::White;
			sysTitle->AutoSize = true;
			sysTitle->Location = Point(74, 16);
			sysTitle->BackColor = Color::Transparent;

			Label^ sysSubtitle = gcnew Label();
			sysSubtitle->Text = "Blood Request Management";
			sysSubtitle->Font = gcnew Drawing::Font("Segoe UI", 9);
			sysSubtitle->ForeColor = Color::FromArgb(254, 202, 202);
			sysSubtitle->AutoSize = true;
			sysSubtitle->Location = Point(76, 44);
			sysSubtitle->BackColor = Color::Transparent;

			Label^ crossDeco = gcnew Label();
			crossDeco->Text = L"✚  Emergency  |  Urgent  |  Routine Requests";
			crossDeco->Font = gcnew Drawing::Font("Segoe UI", 9);
			crossDeco->ForeColor = Color::FromArgb(254, 226, 226);
			crossDeco->AutoSize = true;
			crossDeco->BackColor = Color::Transparent;
			crossDeco->Location = Point(730, 28);

			header->Controls->Add(logoMark);
			header->Controls->Add(sysTitle);
			header->Controls->Add(sysSubtitle);
			header->Controls->Add(crossDeco);

			// ════════════════════════════════════════════
			//  STAT CARDS STRIP
			// ════════════════════════════════════════════
			Panel^ statsStrip = gcnew Panel();
			statsStrip->Dock = DockStyle::Top;
			statsStrip->Height = 110;
			statsStrip->BackColor = CRIMSON_BG;

			cardTotal = gcnew StatCard(L"📋", "TOTAL REQUESTS", "0", CRIMSON);
			cardTotal->Location = Point(24, 12);

			cardPending = gcnew StatCard(L"⏳", "PENDING", "0", ACCENT_AMB);
			cardPending->Location = Point(196, 12);

			cardCritical = gcnew StatCard(L"🚨", "CRITICAL", "0", CRIMSON_LT);
			cardCritical->Location = Point(368, 12);

			cardFulfilled = gcnew StatCard(L"✅", "FULFILLED", "0", ACCENT_GRN);
			cardFulfilled->Location = Point(540, 12);

			Label^ tagline = gcnew Label();
			tagline->Text = L"\"Save a life —\nDonate blood today.\"";
			tagline->Font = gcnew Drawing::Font("Georgia", 10, FontStyle::Italic);
			tagline->ForeColor = CRIMSON;
			tagline->AutoSize = true;
			tagline->Location = Point(740, 22);
			tagline->BackColor = Color::Transparent;

			statsStrip->Controls->Add(cardTotal);
			statsStrip->Controls->Add(cardPending);
			statsStrip->Controls->Add(cardCritical);
			statsStrip->Controls->Add(cardFulfilled);
			statsStrip->Controls->Add(tagline);

			// ════════════════════════════════════════════
			//  SIDEBAR
			// ════════════════════════════════════════════
			Panel^ sidebar = gcnew Panel();
			sidebar->Width = 390;
			sidebar->Dock = DockStyle::Left;
			sidebar->BackColor = Color::White;
			sidebar->AutoScroll = true;

			Label^ formHeading = gcnew Label();
			formHeading->Text = L"🩸  New Blood Request";
			formHeading->Font = gcnew Drawing::Font("Segoe UI", 13, FontStyle::Bold);
			formHeading->ForeColor = CRIMSON;
			formHeading->AutoSize = true;
			formHeading->Location = Point(24, 18);
			sidebar->Controls->Add(formHeading);

			Panel^ divider = gcnew Panel();
			divider->Location = Point(24, 48);
			divider->Size = Drawing::Size(330, 2);
			divider->BackColor = CRIMSON_BG;
			sidebar->Controls->Add(divider);

			// Build input controls
			txtPatient = MakeTextBox(330);
			comboBlood = MakeComboBox(330);
			comboBlood->Items->AddRange(gcnew array<Object^>{
				"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"});

			numUnits = gcnew NumericUpDown();
			numUnits->Font = gcnew Drawing::Font("Segoe UI", 10);
			numUnits->Width = 330; numUnits->Minimum = 1; numUnits->Maximum = 50;
			numUnits->BorderStyle = BorderStyle::FixedSingle;

			txtHospital = MakeTextBox(330);
			txtDoctor = MakeTextBox(330);
			txtContact = MakeTextBox(330);

			comboUrgency = MakeComboBox(330);
			comboUrgency->Items->AddRange(gcnew array<Object^>{ "Routine", "Urgent", "Critical" });
			comboUrgency->SelectedIndex = 0;

			txtNotes = gcnew TextBox();
			txtNotes->Font = gcnew Drawing::Font("Segoe UI", 10);
			txtNotes->BorderStyle = BorderStyle::FixedSingle;
			txtNotes->BackColor = Color::White;
			txtNotes->Width = 330;
			txtNotes->Height = 60;
			txtNotes->Multiline = true;
			txtNotes->ScrollBars = ScrollBars::Vertical;

			int y = 62;
			AddField(sidebar, "PATIENT FULL NAME", txtPatient, y);
			AddField(sidebar, "BLOOD GROUP REQUIRED", comboBlood, y);
			AddField(sidebar, "UNITS NEEDED", numUnits, y);
			AddField(sidebar, "HOSPITAL / CLINIC", txtHospital, y);
			AddField(sidebar, "REQUESTING DOCTOR", txtDoctor, y);
			AddField(sidebar, "CONTACT NUMBER", txtContact, y);
			AddField(sidebar, "URGENCY LEVEL", comboUrgency, y);
			AddField(sidebar, "ADDITIONAL NOTES", txtNotes, y);

			// Submit button
			btnSubmit = MakeButton(L"💉  SUBMIT REQUEST", CRIMSON, Color::White, 330, 44);
			btnSubmit->Location = Point(24, y); y += 52;
			btnSubmit->Click += gcnew EventHandler(this, &BloodRequestForm::SubmitRequest);
			sidebar->Controls->Add(btnSubmit);

			// Status action buttons row
			Panel^ statusRow = gcnew Panel();
			statusRow->Location = Point(24, y);
			statusRow->Size = Drawing::Size(330, 42);
			statusRow->BackColor = Color::Transparent;

			btnApprove = MakeButton(L"✔ Approve", ACCENT_GRN, Color::White, 76, 40);
			btnApprove->Location = Point(0, 0);
			btnApprove->Click += gcnew EventHandler(this, &BloodRequestForm::ApproveRequest);

			btnFulfill = MakeButton(L"★ Fulfill", ACCENT_BLU, Color::White, 76, 40);
			btnFulfill->Location = Point(82, 0);
			btnFulfill->Click += gcnew EventHandler(this, &BloodRequestForm::FulfillRequest);

			btnCancel = MakeButton(L"✖ Cancel", ACCENT_AMB, Color::White, 76, 40);
			btnCancel->Location = Point(164, 0);
			btnCancel->Click += gcnew EventHandler(this, &BloodRequestForm::CancelRequest);

			btnDelete = MakeButton(L"🗑", Color::FromArgb(239, 68, 68), Color::White, 42, 40);
			btnDelete->Location = Point(246, 0);
			btnDelete->Click += gcnew EventHandler(this, &BloodRequestForm::DeleteRequest);

			btnClear = MakeButton(L"↺", Color::FromArgb(243, 244, 246), DARK_TEXT, 36, 40);
			btnClear->Location = Point(294, 0);
			btnClear->Click += gcnew EventHandler(this, &BloodRequestForm::ClearFields_Click);

			statusRow->Controls->Add(btnApprove);
			statusRow->Controls->Add(btnFulfill);
			statusRow->Controls->Add(btnCancel);
			statusRow->Controls->Add(btnDelete);
			statusRow->Controls->Add(btnClear);
			sidebar->Controls->Add(statusRow);

			// Sidebar footer
			Label^ sideFooter = gcnew Label();
			sideFooter->Text = L"🏥  LifeBlood Management System v2.0";
			sideFooter->Font = gcnew Drawing::Font("Segoe UI", 7);
			sideFooter->ForeColor = Color::FromArgb(209, 213, 219);
			sideFooter->AutoSize = true;
			sideFooter->Location = Point(24, y + 56);
			sidebar->Controls->Add(sideFooter);

			// Sidebar right border
			Panel^ sideRight = gcnew Panel();
			sideRight->Dock = DockStyle::Right;
			sideRight->Width = 1;
			sideRight->BackColor = BORDER;
			sidebar->Controls->Add(sideRight);

			// ════════════════════════════════════════════
			//  MAIN CONTENT
			// ════════════════════════════════════════════
			Panel^ mainContent = gcnew Panel();
			mainContent->Dock = DockStyle::Fill;
			mainContent->BackColor = LIGHT_BG;
			mainContent->Padding = Windows::Forms::Padding(20, 16, 20, 16);

			// Toolbar
			Panel^ toolbar = gcnew Panel();
			toolbar->Dock = DockStyle::Top;
			toolbar->Height = 52;
			toolbar->BackColor = Color::Transparent;

			Label^ listTitle = gcnew Label();
			listTitle->Text = L"📋  Request Registry";
			listTitle->Font = gcnew Drawing::Font("Segoe UI", 13, FontStyle::Bold);
			listTitle->ForeColor = DARK_TEXT;
			listTitle->AutoSize = true;
			listTitle->Location = Point(0, 12);

			txtSearch = MakeTextBox(220);
			txtSearch->Location = Point(420, 13);
			txtSearch->Text = "Search requests...";
			txtSearch->ForeColor = Color::Gray;
			txtSearch->GotFocus += gcnew EventHandler(this, &BloodRequestForm::SearchGotFocus);
			txtSearch->LostFocus += gcnew EventHandler(this, &BloodRequestForm::SearchLostFocus);
			txtSearch->TextChanged += gcnew EventHandler(this, &BloodRequestForm::SearchChanged);

			Label^ searchIcon = gcnew Label();
			searchIcon->Text = L"🔍";
			searchIcon->Font = gcnew Drawing::Font("Segoe UI Emoji", 11);
			searchIcon->AutoSize = true;
			searchIcon->Location = Point(394, 16);
			searchIcon->BackColor = Color::Transparent;

			toolbar->Controls->Add(listTitle);
			toolbar->Controls->Add(searchIcon);
			toolbar->Controls->Add(txtSearch);

			// ListView
			listView = gcnew ListView();
			listView->Dock = DockStyle::Fill;
			listView->View = View::Details;
			listView->FullRowSelect = true;
			listView->GridLines = false;
			listView->Font = gcnew Drawing::Font("Segoe UI", 10);
			listView->BackColor = Color::White;
			listView->BorderStyle = BorderStyle::None;
			listView->HideSelection = false;
			listView->OwnerDraw = true;

			ColumnHeader^ c0 = gcnew ColumnHeader(); c0->Text = "  REQ #";         c0->Width = 70;
			ColumnHeader^ c1 = gcnew ColumnHeader(); c1->Text = "PATIENT";          c1->Width = 170;
			ColumnHeader^ c2 = gcnew ColumnHeader(); c2->Text = "BLOOD";            c2->Width = 70;
			ColumnHeader^ c3 = gcnew ColumnHeader(); c3->Text = "UNITS";            c3->Width = 60;
			ColumnHeader^ c4 = gcnew ColumnHeader(); c4->Text = "HOSPITAL";         c4->Width = 160;
			ColumnHeader^ c5 = gcnew ColumnHeader(); c5->Text = "DOCTOR";           c5->Width = 140;
			ColumnHeader^ c6 = gcnew ColumnHeader(); c6->Text = "URGENCY";          c6->Width = 90;
			ColumnHeader^ c7 = gcnew ColumnHeader(); c7->Text = "STATUS";           c7->Width = 90;
			ColumnHeader^ c8 = gcnew ColumnHeader(); c8->Text = "REQUESTED ON";     c8->Width = 150;
			listView->Columns->AddRange(gcnew array<ColumnHeader^>{ c0, c1, c2, c3, c4, c5, c6, c7, c8 });

			listView->DrawColumnHeader += gcnew DrawListViewColumnHeaderEventHandler(
				this, &BloodRequestForm::DrawHeader);
			listView->DrawItem += gcnew DrawListViewItemEventHandler(
				this, &BloodRequestForm::DrawItem);
			listView->DrawSubItem += gcnew DrawListViewSubItemEventHandler(
				this, &BloodRequestForm::DrawSubItem);
			listView->SelectedIndexChanged += gcnew EventHandler(
				this, &BloodRequestForm::ListView_SelectionChanged);

			// Status bar
			lblStatus = gcnew Label();
			lblStatus->Dock = DockStyle::Bottom;
			lblStatus->Height = 28;
			lblStatus->Font = gcnew Drawing::Font("Segoe UI", 8);
			lblStatus->ForeColor = MID_GRAY;
			lblStatus->BackColor = Color::White;
			lblStatus->TextAlign = ContentAlignment::MiddleLeft;
			lblStatus->Padding = Windows::Forms::Padding(8, 0, 0, 0);
			lblStatus->Text = "Ready  |  LifeBlood Blood Request System";

			mainContent->Controls->Add(listView);
			mainContent->Controls->Add(toolbar);
			mainContent->Controls->Add(lblStatus);

			// Assemble
			this->Controls->Add(mainContent);
			this->Controls->Add(sidebar);
			this->Controls->Add(statsStrip);
			this->Controls->Add(header);
		}

		// ─────────────────────────────────────────────────────────────────────
		//  PAINT EVENTS
		// ─────────────────────────────────────────────────────────────────────

		void DrawHeaderGradient(Object^ sender, PaintEventArgs^ e) {
			Panel^ p = safe_cast<Panel^>(sender);
			LinearGradientBrush^ gb = gcnew LinearGradientBrush(
				p->ClientRectangle,
				Color::FromArgb(185, 28, 28),
				Color::FromArgb(127, 29, 29),
				LinearGradientMode::Horizontal);
			e->Graphics->FillRectangle(gb, p->ClientRectangle);
		}

		void DrawHeader(Object^ sender, DrawListViewColumnHeaderEventArgs^ e) {
			e->Graphics->FillRectangle(gcnew SolidBrush(Color::FromArgb(254, 242, 242)), e->Bounds);
			e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(252, 165, 165)),
				e->Bounds.X, e->Bounds.Bottom - 1, e->Bounds.Right, e->Bounds.Bottom - 1);
			StringFormat^ sf = gcnew StringFormat();
			sf->Alignment = StringAlignment::Near;
			sf->LineAlignment = StringAlignment::Center;
			e->Graphics->DrawString(e->Header->Text,
				gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold),
				gcnew SolidBrush(CRIMSON),
				RectangleF(e->Bounds.X + 6, (float)e->Bounds.Y,
					(float)e->Bounds.Width, (float)e->Bounds.Height), sf);
		}

		void DrawItem(Object^ sender, DrawListViewItemEventArgs^ e) {
			Color bg = (e->ItemIndex % 2 == 0) ? Color::White : Color::FromArgb(253, 246, 246);
			if (e->Item->Selected) bg = Color::FromArgb(254, 226, 226);
			e->Graphics->FillRectangle(gcnew SolidBrush(bg), e->Bounds);
		}

		void DrawSubItem(Object^ sender, DrawListViewSubItemEventArgs^ e) {
			Color bg = (e->ItemIndex % 2 == 0) ? Color::White : Color::FromArgb(253, 246, 246);
			if (e->Item->Selected) bg = Color::FromArgb(254, 226, 226);
			e->Graphics->FillRectangle(gcnew SolidBrush(bg), e->Bounds);

			StringFormat^ sf = gcnew StringFormat();
			sf->Alignment = StringAlignment::Near;
			sf->LineAlignment = StringAlignment::Center;
			RectangleF rc = RectangleF(e->Bounds.X + 6, (float)e->Bounds.Y,
				e->Bounds.Width - 6.0f, (float)e->Bounds.Height);

			// Blood group badge (col 2)
			if (e->ColumnIndex == 2) {
				Drawing::Rectangle badge = Drawing::Rectangle(
					e->Bounds.X + 4, e->Bounds.Y + 4, 52, e->Bounds.Height - 8);
				e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
				e->Graphics->FillEllipse(gcnew SolidBrush(CRIMSON_BG), badge);
				e->Graphics->DrawEllipse(gcnew Pen(CRIMSON), badge);
				StringFormat^ c = gcnew StringFormat();
				c->Alignment = StringAlignment::Center;
				c->LineAlignment = StringAlignment::Center;
				e->Graphics->DrawString(e->SubItem->Text,
					gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold),
					gcnew SolidBrush(CRIMSON),
					RectangleF((float)badge.X, (float)badge.Y,
						(float)badge.Width, (float)badge.Height), c);
				return;
			}

			// Urgency badge (col 6)
			if (e->ColumnIndex == 6) {
				String^ txt = e->SubItem->Text;
				Color badgeBg, badgeFg;
				if (txt == "CRITICAL") {
					badgeBg = Color::FromArgb(254, 226, 226); badgeFg = CRIMSON;
				}
				else if (txt == "URGENT") {
					badgeBg = Color::FromArgb(254, 243, 199); badgeFg = ACCENT_AMB;
				}
				else {
					badgeBg = Color::FromArgb(209, 250, 229); badgeFg = ACCENT_GRN;
				}
				Drawing::Rectangle badge = Drawing::Rectangle(
					e->Bounds.X + 4, e->Bounds.Y + 5, e->Bounds.Width - 10, e->Bounds.Height - 10);
				e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
				GraphicsPath^ gp = gcnew GraphicsPath();
				int r = 8;
				gp->AddArc(badge.X, badge.Y, r * 2, r * 2, 180, 90);
				gp->AddArc(badge.Right - r * 2, badge.Y, r * 2, r * 2, 270, 90);
				gp->AddArc(badge.Right - r * 2, badge.Bottom - r * 2, r * 2, r * 2, 0, 90);
				gp->AddArc(badge.X, badge.Bottom - r * 2, r * 2, r * 2, 90, 90);
				gp->CloseFigure();
				e->Graphics->FillPath(gcnew SolidBrush(badgeBg), gp);
				StringFormat^ c = gcnew StringFormat();
				c->Alignment = StringAlignment::Center;
				c->LineAlignment = StringAlignment::Center;
				e->Graphics->DrawString(txt,
					gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold),
					gcnew SolidBrush(badgeFg),
					RectangleF((float)badge.X, (float)badge.Y,
						(float)badge.Width, (float)badge.Height), c);
				return;
			}

			// Status badge (col 7)
			if (e->ColumnIndex == 7) {
				String^ txt = e->SubItem->Text;
				Color badgeBg, badgeFg;
				if (txt == "Fulfilled") {
					badgeBg = Color::FromArgb(209, 250, 229); badgeFg = ACCENT_GRN;
				}
				else if (txt == "Approved") {
					badgeBg = Color::FromArgb(219, 234, 254); badgeFg = ACCENT_BLU;
				}
				else if (txt == "Cancelled") {
					badgeBg = Color::FromArgb(243, 244, 246); badgeFg = MID_GRAY;
				}
				else {
					badgeBg = Color::FromArgb(254, 243, 199); badgeFg = ACCENT_AMB;
				}
				Drawing::Rectangle badge = Drawing::Rectangle(
					e->Bounds.X + 4, e->Bounds.Y + 5, e->Bounds.Width - 10, e->Bounds.Height - 10);
				e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
				GraphicsPath^ gp = gcnew GraphicsPath();
				int r = 8;
				gp->AddArc(badge.X, badge.Y, r * 2, r * 2, 180, 90);
				gp->AddArc(badge.Right - r * 2, badge.Y, r * 2, r * 2, 270, 90);
				gp->AddArc(badge.Right - r * 2, badge.Bottom - r * 2, r * 2, r * 2, 0, 90);
				gp->AddArc(badge.X, badge.Bottom - r * 2, r * 2, r * 2, 90, 90);
				gp->CloseFigure();
				e->Graphics->FillPath(gcnew SolidBrush(badgeBg), gp);
				StringFormat^ c = gcnew StringFormat();
				c->Alignment = StringAlignment::Center;
				c->LineAlignment = StringAlignment::Center;
				e->Graphics->DrawString(txt,
					gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold),
					gcnew SolidBrush(badgeFg),
					RectangleF((float)badge.X, (float)badge.Y,
						(float)badge.Width, (float)badge.Height), c);
				return;
			}

			// Default text rendering
			Color textColor = (e->ColumnIndex == 1) ? DARK_TEXT : MID_GRAY;
			Drawing::Font^ textFont = (e->ColumnIndex == 1)
				? gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold)
				: gcnew Drawing::Font("Segoe UI", 10);
			e->Graphics->DrawString(e->SubItem->Text, textFont,
				gcnew SolidBrush(textColor), rc, sf);
		}

		// ─────────────────────────────────────────────────────────────────────
		//  SEARCH
		// ─────────────────────────────────────────────────────────────────────
		void SearchGotFocus(Object^ sender, EventArgs^ e) {
			if (txtSearch->Text == "Search requests...") {
				txtSearch->Text = "";
				txtSearch->ForeColor = DARK_TEXT;
			}
		}
		void SearchLostFocus(Object^ sender, EventArgs^ e) {
			if (String::IsNullOrWhiteSpace(txtSearch->Text)) {
				txtSearch->Text = "Search requests...";
				txtSearch->ForeColor = Color::Gray;
			}
		}
		void SearchChanged(Object^ sender, EventArgs^ e) {
			if (txtSearch->Text == "Search requests...") return;
			RefreshList(txtSearch->Text);
		}

		// ─────────────────────────────────────────────────────────────────────
		//  CRUD & STATUS ACTIONS
		// ─────────────────────────────────────────────────────────────────────
		void SubmitRequest(Object^ sender, EventArgs^ e) {
			if (String::IsNullOrWhiteSpace(txtPatient->Text) || comboBlood->SelectedIndex == -1) {
				MessageBox::Show("Patient Name and Blood Group are required.",
					"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			if (String::IsNullOrWhiteSpace(txtHospital->Text)) {
				MessageBox::Show("Hospital / Clinic name is required.",
					"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}

			Urgency urg = Urgency::Routine;
			if (comboUrgency->SelectedIndex == 1) urg = Urgency::Urgent;
			if (comboUrgency->SelectedIndex == 2) urg = Urgency::Critical;

			BloodRequest^ req = gcnew BloodRequest(
				txtPatient->Text->Trim(),
				comboBlood->Text,
				(int)numUnits->Value,
				txtHospital->Text->Trim(),
				txtDoctor->Text->Trim(),
				txtContact->Text->Trim(),
				urg,
				txtNotes->Text->Trim());

			requests->Add(req);
			RefreshList(String::Empty);
			ClearFields();
			UpdateStats();
			lblStatus->Text = String::Format(
				L"✔  Request #{0} submitted for '{1}'  |  {2}",
				req->requestID, req->patientName, DateTime::Now.ToString("HH:mm:ss"));
		}

		void ApproveRequest(Object^ sender, EventArgs^ e) {
			ChangeStatus(RequestStatus::Approved, "approved");
		}

		void FulfillRequest(Object^ sender, EventArgs^ e) {
			ChangeStatus(RequestStatus::Fulfilled, "fulfilled");
		}

		void CancelRequest(Object^ sender, EventArgs^ e) {
			ChangeStatus(RequestStatus::Cancelled, "cancelled");
		}

		void ChangeStatus(RequestStatus newStatus, String^ verb) {
			if (listView->SelectedItems->Count == 0) {
				lblStatus->Text = "⚠  Please select a request first.";
				return;
			}
			int idx = listView->SelectedItems[0]->Tag != nullptr
				? safe_cast<int>(listView->SelectedItems[0]->Tag)
				: listView->SelectedItems[0]->Index;
			// Find by request ID stored in Tag
			int reqID = safe_cast<int>(listView->SelectedItems[0]->Tag);
			for each (BloodRequest ^ r in requests) {
				if (r->requestID == reqID) {
					r->status = newStatus;
					break;
				}
			}
			RefreshList(String::Empty);
			UpdateStats();
			lblStatus->Text = String::Format(L"✔  Request {0}  |  {1}",
				verb, DateTime::Now.ToString("HH:mm:ss"));
		}

		void DeleteRequest(Object^ sender, EventArgs^ e) {
			if (listView->SelectedItems->Count == 0) {
				lblStatus->Text = "⚠  Please select a request to delete.";
				return;
			}
			int reqID = safe_cast<int>(listView->SelectedItems[0]->Tag);
			String^ patName = "";
			BloodRequest^ toRemove = nullptr;
			for each (BloodRequest ^ r in requests) {
				if (r->requestID == reqID) { toRemove = r; patName = r->patientName; break; }
			}
			if (toRemove == nullptr) return;
			if (MessageBox::Show(
				String::Format("Delete request #{0} for '{1}'?", reqID, patName),
				"Confirm Delete", MessageBoxButtons::YesNo, MessageBoxIcon::Question)
				== System::Windows::Forms::DialogResult::Yes) {
				requests->Remove(toRemove);
				RefreshList(String::Empty);
				ClearFields();
				UpdateStats();
				lblStatus->Text = String::Format(L"🗑  Request #{0} deleted  |  {1}",
					reqID, DateTime::Now.ToString("HH:mm:ss"));
			}
		}

		void ClearFields_Click(Object^ sender, EventArgs^ e) { ClearFields(); }

		// ─────────────────────────────────────────────────────────────────────
		//  SELECTION → POPULATE FORM
		// ─────────────────────────────────────────────────────────────────────
		void ListView_SelectionChanged(Object^ sender, EventArgs^ e) {
			if (listView->SelectedItems->Count == 0) return;
			int reqID = safe_cast<int>(listView->SelectedItems[0]->Tag);
			for each (BloodRequest ^ r in requests) {
				if (r->requestID != reqID) continue;
				txtPatient->Text = r->patientName;
				comboBlood->SelectedItem = r->bloodGroup;
				numUnits->Value = r->unitsNeeded;
				txtHospital->Text = r->hospital;
				txtDoctor->Text = r->doctorName;
				txtContact->Text = r->contactNumber;
				comboUrgency->SelectedIndex = (int)r->urgencyLevel;
				txtNotes->Text = r->notes;
				break;
			}
		}

		// ─────────────────────────────────────────────────────────────────────
		//  HELPERS
		// ─────────────────────────────────────────────────────────────────────
		void RefreshList(String^ filter) {
			listView->Items->Clear();
			for each (BloodRequest ^ r in requests) {
				bool match = String::IsNullOrEmpty(filter)
					|| r->patientName->ToLower()->Contains(filter->ToLower())
					|| r->bloodGroup->ToLower()->Contains(filter->ToLower())
					|| r->hospital->ToLower()->Contains(filter->ToLower())
					|| r->requestID.ToString()->Contains(filter);
				if (!match) continue;

				ListViewItem^ item = gcnew ListViewItem("  #" + r->requestID.ToString());
				item->Tag = r->requestID;   // store ID for safe lookup
				item->SubItems->Add(r->patientName);
				item->SubItems->Add(r->bloodGroup);
				item->SubItems->Add(r->unitsNeeded.ToString() + " u");
				item->SubItems->Add(r->hospital);
				item->SubItems->Add(String::IsNullOrEmpty(r->doctorName) ? "—" : r->doctorName);
				item->SubItems->Add(r->UrgencyText());
				item->SubItems->Add(r->StatusText());
				item->SubItems->Add(r->requestedOn.ToString("dd MMM yyyy  HH:mm"));
				listView->Items->Add(item);
			}
		}

		void ClearFields() {
			txtPatient->Clear();
			comboBlood->SelectedIndex = -1;
			numUnits->Value = 1;
			txtHospital->Clear();
			txtDoctor->Clear();
			txtContact->Clear();
			comboUrgency->SelectedIndex = 0;
			txtNotes->Clear();
			listView->SelectedItems->Clear();
		}

		void UpdateStats() {
			int total = requests->Count;
			int pending = 0, critical = 0, fulfilled = 0;
			for each (BloodRequest ^ r in requests) {
				if (r->status == RequestStatus::Pending)   pending++;
				if (r->urgencyLevel == Urgency::Critical)     critical++;
				if (r->status == RequestStatus::Fulfilled) fulfilled++;
			}
			cardTotal->SetValue(total.ToString());
			cardPending->SetValue(pending.ToString());
			cardCritical->SetValue(critical.ToString());
			cardFulfilled->SetValue(fulfilled.ToString());
			lblStatus->Text = String::Format(
				L"📊  Total: {0}  |  Pending: {1}  |  Critical: {2}  |  Fulfilled: {3}",
				total, pending, critical, fulfilled);
		}
	};
}