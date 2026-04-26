#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Collections::Generic;

namespace BloodBank {

	// ─── Data Model ───────────────────────────────────────────────────────────
	public ref class Recipient {
	public:
		String^ name;
		String^ blood;
		int     age;
		String^ contact;
		String^ gender;
		DateTime registeredOn;

		Recipient(String^ n, String^ b, int a, String^ c, String^ g) {
			name = n; blood = b; age = a; contact = c; gender = g;
			registeredOn = DateTime::Now;
		}
	};

	// ─── Rounded-corner Panel helper ──────────────────────────────────────────
	public ref class RoundedPanel : public Panel {
	public:
		int Radius;
		RoundedPanel() { Radius = 16; DoubleBuffered = true; }
	protected:
		virtual void OnPaint(PaintEventArgs^ e) override {
			e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
			Drawing::Rectangle r = ClientRectangle;
			r.Width--; r.Height--;
			GraphicsPath^ path = gcnew GraphicsPath();
			path->AddArc(r.X, r.Y, Radius * 2, Radius * 2, 180, 90);
			path->AddArc(r.Right - Radius * 2, r.Y, Radius * 2, Radius * 2, 270, 90);
			path->AddArc(r.Right - Radius * 2, r.Bottom - Radius * 2, Radius * 2, Radius * 2, 0, 90);
			path->AddArc(r.X, r.Bottom - Radius * 2, Radius * 2, Radius * 2, 90, 90);
			path->CloseFigure();
			e->Graphics->FillPath(gcnew SolidBrush(BackColor), path);
			e->Graphics->DrawPath(gcnew Pen(Color::FromArgb(220, 220, 220)), path);
			Panel::OnPaint(e);
		}
	};

	// ─── Stat Card control ────────────────────────────────────────────────────
	public ref class StatCard : public RoundedPanel {
	private:
		String^ _icon;
		String^ _title;
		Label^ _valLabel;
		Color   _accent;
	public:
		StatCard(String^ icon, String^ title, String^ value, Color accent) {
			_icon = icon; _title = title; _accent = accent;
			Size = Drawing::Size(160, 90);
			BackColor = Color::White;
			Padding = Windows::Forms::Padding(12);

			Label^ iconLbl = gcnew Label();
			iconLbl->Text = icon;
			iconLbl->Font = gcnew Drawing::Font("Segoe UI Emoji", 22);
			iconLbl->AutoSize = true;
			iconLbl->Location = Point(12, 8);
			iconLbl->BackColor = Color::Transparent;

			Label^ titleLbl = gcnew Label();
			titleLbl->Text = title;
			titleLbl->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
			titleLbl->ForeColor = Color::Gray;
			titleLbl->AutoSize = true;
			titleLbl->Location = Point(12, 56);
			titleLbl->BackColor = Color::Transparent;

			_valLabel = gcnew Label();
			_valLabel->Text = value;
			_valLabel->Font = gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold);
			_valLabel->ForeColor = accent;
			_valLabel->AutoSize = true;
			_valLabel->Location = Point(100, 10);
			_valLabel->BackColor = Color::Transparent;

			Controls->Add(iconLbl);
			Controls->Add(titleLbl);
			Controls->Add(_valLabel);
		}
		void SetValue(String^ v) { _valLabel->Text = v; }

	protected:
		virtual void OnPaint(PaintEventArgs^ e) override {
			RoundedPanel::OnPaint(e);
			// Accent stripe on left edge
			e->Graphics->FillRectangle(gcnew SolidBrush(_accent),
				Drawing::Rectangle(0, 16, 4, Height - 32));
		}
	};

	// ─── Main Form ────────────────────────────────────────────────────────────
	public ref class RecipientForm : public Form
	{
	private:
		// Data
		List<Recipient^>^ recipients;

		// Input Controls
		TextBox^ txtName;
		ComboBox^ comboBlood;
		NumericUpDown^ numAge;
		TextBox^ txtContact;
		ComboBox^ comboGender;
		TextBox^ txtSearch;

		// Buttons
		Button^ btnAdd;
		Button^ btnUpdate;
		Button^ btnDelete;
		Button^ btnClear;

		// Display
		ListView^ listView;
		Label^ lblStatus;

		// Stat Cards
		StatCard^ cardTotal;
		StatCard^ cardAPos;
		StatCard^ cardBPos;
		StatCard^ cardOPos;

		// Colors
		static Color CRIMSON = Color::FromArgb(185, 28, 28);
		static Color CRIMSON_LT = Color::FromArgb(220, 38, 38);
		static Color CRIMSON_BG = Color::FromArgb(254, 242, 242);
		static Color DARK_TEXT = Color::FromArgb(30, 30, 30);
		static Color MID_GRAY = Color::FromArgb(107, 114, 128);
		static Color LIGHT_BG = Color::FromArgb(249, 250, 251);
		static Color ACCENT_GRN = Color::FromArgb(5, 150, 105);
		static Color BORDER = Color::FromArgb(229, 231, 235);

	public:
		RecipientForm(void) {
			recipients = gcnew List<Recipient^>();
			this->DoubleBuffered = true;
			InitializeComponent();
		}

	private:

		// ── Styled label for sidebar ──────────────────────────────────────────
		Label^ MakeFieldLabel(String^ text) {
			Label^ l = gcnew Label();
			l->Text = text;
			l->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold);
			l->ForeColor = MID_GRAY;
			l->AutoSize = true;
			return l;
		}

		// ── Styled text-box ───────────────────────────────────────────────────
		TextBox^ MakeTextBox() {
			TextBox^ t = gcnew TextBox();
			t->Font = gcnew Drawing::Font("Segoe UI", 10);
			t->BorderStyle = BorderStyle::FixedSingle;
			t->BackColor = Color::White;
			t->Width = 310;
			t->Height = 34;
			return t;
		}

		// ── Styled combo-box ──────────────────────────────────────────────────
		ComboBox^ MakeComboBox() {
			ComboBox^ c = gcnew ComboBox();
			c->Font = gcnew Drawing::Font("Segoe UI", 10);
			c->DropDownStyle = ComboBoxStyle::DropDownList;
			c->FlatStyle = FlatStyle::Flat;
			c->Width = 310;
			return c;
		}

		// ── Action button ─────────────────────────────────────────────────────
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

		// ── Place a label+control pair in sidebar ─────────────────────────────
		void AddField(Panel^ p, String^ labelText, Control^ ctrl, int% y) {
			Label^ lbl = MakeFieldLabel(labelText);
			lbl->Location = Point(24, y);
			p->Controls->Add(lbl);
			y += 20;
			ctrl->Location = Point(24, y);
			p->Controls->Add(ctrl);
			y += ctrl->Height + 16;
		}

		// ── Main layout ───────────────────────────────────────────────────────
		void InitializeComponent(void)
		{
			this->Text = "LifeBlood — Recipient Management";
			this->WindowState = FormWindowState::Maximized;
			this->BackColor = LIGHT_BG;
			this->Font = gcnew Drawing::Font("Segoe UI", 10);
			this->MinimumSize = Drawing::Size(1100, 700);

			// ════════════════════════════════════════════
			//  HEADER BAR
			// ════════════════════════════════════════════
			Panel^ header = gcnew Panel();
			header->Dock = DockStyle::Top;
			header->Height = 72;
			header->BackColor = CRIMSON;
			header->Paint += gcnew PaintEventHandler(this, &RecipientForm::DrawHeaderGradient);

			// Blood-drop emoji as icon
			Label^ logoMark = gcnew Label();
			logoMark->Text = L"🩸";
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
			sysSubtitle->Text = "Recipient Management System";
			sysSubtitle->Font = gcnew Drawing::Font("Segoe UI", 9);
			sysSubtitle->ForeColor = Color::FromArgb(254, 202, 202);
			sysSubtitle->AutoSize = true;
			sysSubtitle->Location = Point(76, 44);
			sysSubtitle->BackColor = Color::Transparent;

			// Hospital cross decoration
			Label^ crossDeco = gcnew Label();
			crossDeco->Text = L"✚  Blood Donor Network  |  Est. 2025";
			crossDeco->Font = gcnew Drawing::Font("Segoe UI", 9);
			crossDeco->ForeColor = Color::FromArgb(254, 226, 226);
			crossDeco->AutoSize = true;
			crossDeco->BackColor = Color::Transparent;
			// anchor to right — set after form load via event, or set large x:
			crossDeco->Location = Point(750, 28);

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
			statsStrip->Padding = Windows::Forms::Padding(20, 10, 20, 10);

			cardTotal = gcnew StatCard(L"👥", "TOTAL RECIPIENTS", "0", CRIMSON);
			cardTotal->Location = Point(24, 12);

			cardAPos = gcnew StatCard(L"🅰", "A+ RECIPIENTS", "0", Color::FromArgb(220, 38, 38));
			cardAPos->Location = Point(196, 12);

			cardBPos = gcnew StatCard(L"🅱", "B+ RECIPIENTS", "0", Color::FromArgb(37, 99, 235));
			cardBPos->Location = Point(368, 12);

			cardOPos = gcnew StatCard(L"🅾", "O+ RECIPIENTS", "0", ACCENT_GRN);
			cardOPos->Location = Point(540, 12);

			// Tagline on right of stats strip
			Label^ tagline = gcnew Label();
			tagline->Text = L"\"Every drop counts.\nEvery life matters.\"";
			tagline->Font = gcnew Drawing::Font("Georgia", 10, FontStyle::Italic);
			tagline->ForeColor = CRIMSON;
			tagline->AutoSize = true;
			tagline->Location = Point(740, 22);
			tagline->BackColor = Color::Transparent;

			statsStrip->Controls->Add(cardTotal);
			statsStrip->Controls->Add(cardAPos);
			statsStrip->Controls->Add(cardBPos);
			statsStrip->Controls->Add(cardOPos);
			statsStrip->Controls->Add(tagline);

			// ════════════════════════════════════════════
			//  SIDEBAR  (LEFT PANEL)
			// ════════════════════════════════════════════
			Panel^ sidebar = gcnew Panel();
			sidebar->Width = 370;
			sidebar->Dock = DockStyle::Left;
			sidebar->BackColor = Color::White;
			sidebar->Padding = Windows::Forms::Padding(24);

			// Section heading
			Label^ formHeading = gcnew Label();
			formHeading->Text = L"Register Recipient";
			formHeading->Font = gcnew Drawing::Font("Segoe UI", 13, FontStyle::Bold);
			formHeading->ForeColor = CRIMSON;
			formHeading->AutoSize = true;
			formHeading->Location = Point(24, 18);
			sidebar->Controls->Add(formHeading);

			// Divider
			Panel^ divider = gcnew Panel();
			divider->Location = Point(24, 48);
			divider->Size = Drawing::Size(310, 2);
			divider->BackColor = CRIMSON_BG;
			sidebar->Controls->Add(divider);

			// Build form fields
			txtName = MakeTextBox();      txtName->Width = 310;
			comboBlood = MakeComboBox();     comboBlood->Width = 310;
			comboGender = MakeComboBox();    comboGender->Width = 310;
			numAge = gcnew NumericUpDown();
			numAge->Font = gcnew Drawing::Font("Segoe UI", 10);
			numAge->Width = 310; numAge->Minimum = 1; numAge->Maximum = 120;
			numAge->BorderStyle = BorderStyle::FixedSingle;
			txtContact = MakeTextBox();      txtContact->Width = 310;

			comboBlood->Items->AddRange(gcnew array<Object^>{
				"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"});
			comboGender->Items->AddRange(gcnew array<Object^>{
				"Male", "Female", "Other"});

			int y = 62;
			AddField(sidebar, "FULL NAME", txtName, y);
			AddField(sidebar, "BLOOD GROUP", comboBlood, y);
			AddField(sidebar, "GENDER", comboGender, y);
			AddField(sidebar, "AGE", numAge, y);
			AddField(sidebar, "CONTACT NUMBER", txtContact, y);

			// Action buttons
			btnAdd = MakeButton(L"ADD RECIPIENT", CRIMSON, Color::White, 310, 44);
			btnAdd->Location = Point(24, y); y += 52;
			btnAdd->Click += gcnew EventHandler(this, &RecipientForm::AddRecipient);

			Panel^ btnRow = gcnew Panel();
			btnRow->Location = Point(24, y);
			btnRow->Size = Drawing::Size(310, 42);
			btnRow->BackColor = Color::Transparent;

			btnUpdate = MakeButton(L"UPDATE", Color::FromArgb(37, 99, 235), Color::White, 148, 40);
			btnUpdate->Location = Point(0, 0);
			btnUpdate->Click += gcnew EventHandler(this, &RecipientForm::UpdateRecipient);

			btnDelete = MakeButton(L"DELETE", Color::FromArgb(107, 114, 128), Color::White, 80, 40);
			btnDelete->Location = Point(156, 0);
			btnDelete->Click += gcnew EventHandler(this, &RecipientForm::DeleteRecipient);

			btnClear = MakeButton(L"", Color::FromArgb(243, 244, 246), DARK_TEXT, 60, 40);
			btnClear->Location = Point(244, 0);
			btnClear->Click += gcnew EventHandler(this, &RecipientForm::ClearFields_Click);

			btnRow->Controls->Add(btnUpdate);
			btnRow->Controls->Add(btnDelete);
			btnRow->Controls->Add(btnClear);

			sidebar->Controls->Add(btnAdd);
			sidebar->Controls->Add(btnRow);

			// Sidebar footer watermark
			Label^ sideFooter = gcnew Label();
			sideFooter->Text = L"LifeBlood Management System v2.0";
			sideFooter->Font = gcnew Drawing::Font("Segoe UI", 7);
			sideFooter->ForeColor = Color::FromArgb(209, 213, 219);
			sideFooter->AutoSize = true;
			sideFooter->Location = Point(24, y + 60);
			sidebar->Controls->Add(sideFooter);

			// Sidebar right border
			Panel^ sideRight = gcnew Panel();
			sideRight->Dock = DockStyle::Right;
			sideRight->Width = 1;
			sideRight->BackColor = BORDER;
			sidebar->Controls->Add(sideRight);

			// ════════════════════════════════════════════
			//  MAIN CONTENT (LIST + TOOLBAR)
			// ════════════════════════════════════════════
			Panel^ mainContent = gcnew Panel();
			mainContent->Dock = DockStyle::Fill;
			mainContent->BackColor = LIGHT_BG;
			mainContent->Padding = Windows::Forms::Padding(20, 16, 20, 16);

			// ── Toolbar row ─────────────────────────────
			Panel^ toolbar = gcnew Panel();
			toolbar->Dock = DockStyle::Top;
			toolbar->Height = 52;
			toolbar->BackColor = Color::Transparent;

			Label^ listTitle = gcnew Label();
			listTitle->Text = L"Recipient Registry";
			listTitle->Font = gcnew Drawing::Font("Segoe UI", 13, FontStyle::Bold);
			listTitle->ForeColor = DARK_TEXT;
			listTitle->AutoSize = true;
			listTitle->Location = Point(0, 12);

			// Search box
			txtSearch = gcnew TextBox();
			txtSearch->Font = gcnew Drawing::Font("Segoe UI", 10);
			txtSearch->BorderStyle = BorderStyle::FixedSingle;
			txtSearch->BackColor = Color::White;
			txtSearch->Width = 220;
			txtSearch->Location = Point(430, 13);
			txtSearch->Text = "Search recipients...";
			txtSearch->ForeColor = Color::Gray;
			txtSearch->GotFocus += gcnew EventHandler(this, &RecipientForm::SearchGotFocus);
			txtSearch->LostFocus += gcnew EventHandler(this, &RecipientForm::SearchLostFocus);
			txtSearch->TextChanged += gcnew EventHandler(this, &RecipientForm::SearchChanged);

			Label^ searchIcon = gcnew Label();
			searchIcon->Text = L"";
			searchIcon->Font = gcnew Drawing::Font("Segoe UI Emoji", 11);
			searchIcon->AutoSize = true;
			searchIcon->Location = Point(404, 16);
			searchIcon->BackColor = Color::Transparent;

			toolbar->Controls->Add(listTitle);
			toolbar->Controls->Add(searchIcon);
			toolbar->Controls->Add(txtSearch);

			// ── ListView ────────────────────────────────
			listView = gcnew ListView();
			listView->Dock = DockStyle::Fill;
			listView->View = View::Details;
			listView->FullRowSelect = true;
			listView->GridLines = false;
			listView->Font = gcnew Drawing::Font("Segoe UI", 10);
			listView->BackColor = Color::White;
			listView->BorderStyle = BorderStyle::None;
			listView->HideSelection = false;

			// Column headers
			ColumnHeader^ ch0 = gcnew ColumnHeader(); ch0->Text = "  NAME";          ch0->Width = 200;
			ColumnHeader^ ch1 = gcnew ColumnHeader(); ch1->Text = "BLOOD";            ch1->Width = 80;
			ColumnHeader^ ch2 = gcnew ColumnHeader(); ch2->Text = "GENDER";           ch2->Width = 90;
			ColumnHeader^ ch3 = gcnew ColumnHeader(); ch3->Text = "AGE";              ch3->Width = 60;
			ColumnHeader^ ch4 = gcnew ColumnHeader(); ch4->Text = "CONTACT";          ch4->Width = 160;
			ColumnHeader^ ch5 = gcnew ColumnHeader(); ch5->Text = "REGISTERED ON";    ch5->Width = 160;
			listView->Columns->AddRange(gcnew array<ColumnHeader^>{ ch0, ch1, ch2, ch3, ch4, ch5 });
			listView->ColumnWidthChanged += gcnew ColumnWidthChangedEventHandler(this, &RecipientForm::ListView_ColumnWidthChanged);

			// Alternate row color via DrawItem
			listView->OwnerDraw = true;
			listView->DrawColumnHeader += gcnew DrawListViewColumnHeaderEventHandler(
				this, &RecipientForm::DrawHeader);
			listView->DrawItem += gcnew DrawListViewItemEventHandler(
				this, &RecipientForm::DrawItem);
			listView->DrawSubItem += gcnew DrawListViewSubItemEventHandler(
				this, &RecipientForm::DrawSubItem);
			listView->SelectedIndexChanged += gcnew EventHandler(this, &RecipientForm::ListView_SelectionChanged);

			// ── Status bar ──────────────────────────────
			lblStatus = gcnew Label();
			lblStatus->Dock = DockStyle::Bottom;
			lblStatus->Height = 28;
			lblStatus->Font = gcnew Drawing::Font("Segoe UI", 8);
			lblStatus->ForeColor = MID_GRAY;
			lblStatus->BackColor = Color::White;
			lblStatus->TextAlign = ContentAlignment::MiddleLeft;
			lblStatus->Padding = Windows::Forms::Padding(8, 0, 0, 0);
			lblStatus->Text = "Ready  |  LifeBlood Management System";

			mainContent->Controls->Add(listView);
			mainContent->Controls->Add(toolbar);
			mainContent->Controls->Add(lblStatus);

			// ════════════════════════════════════════════
			//  ASSEMBLE
			// ════════════════════════════════════════════
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
			e->Graphics->FillRectangle(gcnew SolidBrush(Color::FromArgb(254, 242, 242)),
				e->Bounds);
			e->Graphics->DrawLine(gcnew Pen(Color::FromArgb(252, 165, 165)),
				e->Bounds.X, e->Bounds.Bottom - 1,
				e->Bounds.Right, e->Bounds.Bottom - 1);
			StringFormat^ sf = gcnew StringFormat();
			sf->Alignment = StringAlignment::Near;
			sf->LineAlignment = StringAlignment::Center;
			e->Graphics->DrawString(e->Header->Text,
				gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold),
				gcnew SolidBrush(CRIMSON),
				RectangleF(e->Bounds.X + 6, (float)e->Bounds.Y,
					(float)e->Bounds.Width, (float)e->Bounds.Height),
				sf);
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

			// Blood group badge
			if (e->ColumnIndex == 1) {
				Drawing::Rectangle badge = Drawing::Rectangle(
					e->Bounds.X + 6, e->Bounds.Y + 4,
					52, e->Bounds.Height - 8);
				e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
				e->Graphics->FillEllipse(gcnew SolidBrush(CRIMSON_BG), badge);
				e->Graphics->DrawEllipse(gcnew Pen(CRIMSON), badge);
				StringFormat^ sf = gcnew StringFormat();
				sf->Alignment = StringAlignment::Center;
				sf->LineAlignment = StringAlignment::Center;
				e->Graphics->DrawString(e->SubItem->Text,
					gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold),
					gcnew SolidBrush(CRIMSON),
					RectangleF((float)badge.X, (float)badge.Y,
						(float)badge.Width, (float)badge.Height), sf);
				return;
			}

			StringFormat^ sf2 = gcnew StringFormat();
			sf2->Alignment = StringAlignment::Near;
			sf2->LineAlignment = StringAlignment::Center;
			Color textColor = (e->ColumnIndex == 0) ? DARK_TEXT : MID_GRAY;
			Drawing::Font^ textFont = (e->ColumnIndex == 0)
				? gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold)
				: gcnew Drawing::Font("Segoe UI", 10);
			e->Graphics->DrawString(e->SubItem->Text, textFont,
				gcnew SolidBrush(textColor),
				RectangleF(e->Bounds.X + 6, (float)e->Bounds.Y,
					e->Bounds.Width - 6, (float)e->Bounds.Height),
				sf2);
		}

		// Prevent column resize from crashing owner-draw
		void ListView_ColumnWidthChanged(Object^ sender, ColumnWidthChangedEventArgs^ e) {
			listView->Invalidate();
		}

		// ─────────────────────────────────────────────────────────────────────
		//  SEARCH
		// ─────────────────────────────────────────────────────────────────────

		void SearchGotFocus(Object^ sender, EventArgs^ e) {
			if (txtSearch->Text == "Search recipients...") {
				txtSearch->Text = "";
				txtSearch->ForeColor = DARK_TEXT;
			}
		}
		void SearchLostFocus(Object^ sender, EventArgs^ e) {
			if (String::IsNullOrWhiteSpace(txtSearch->Text)) {
				txtSearch->Text = "Search recipients...";
				txtSearch->ForeColor = Color::Gray;
			}
		}
		void SearchChanged(Object^ sender, EventArgs^ e) {
			if (txtSearch->Text == "Search recipients...") return;
			RefreshList(txtSearch->Text);
		}

		// ─────────────────────────────────────────────────────────────────────
		//  CRUD OPERATIONS
		// ─────────────────────────────────────────────────────────────────────

		void AddRecipient(Object^ sender, EventArgs^ e) {
			if (String::IsNullOrWhiteSpace(txtName->Text) || comboBlood->SelectedIndex == -1) {
				MessageBox::Show("Full Name and Blood Group are required fields.",
					"Validation Error", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return;
			}
			String^ gender = (comboGender->SelectedIndex >= 0) ? comboGender->Text : "—";
			Recipient^ r = gcnew Recipient(
				txtName->Text->Trim(), comboBlood->Text,
				(int)numAge->Value, txtContact->Text->Trim(), gender);
			recipients->Add(r);
			RefreshList(String::Empty);
			ClearFields();
			UpdateStats();
			lblStatus->Text = String::Format(L"Recipient '{0}' added successfully  |  {1}",
				r->name, DateTime::Now.ToString("HH:mm:ss"));
		}

		void UpdateRecipient(Object^ sender, EventArgs^ e) {
			if (listView->SelectedItems->Count == 0) {
				lblStatus->Text = "Please select a recipient to update.";
				return;
			}
			int idx = listView->SelectedItems[0]->Index;
			recipients[idx]->name = txtName->Text->Trim();
			recipients[idx]->blood = comboBlood->SelectedIndex >= 0 ? comboBlood->Text : recipients[idx]->blood;
			recipients[idx]->age = (int)numAge->Value;
			recipients[idx]->contact = txtContact->Text->Trim();
			recipients[idx]->gender = (comboGender->SelectedIndex >= 0) ? comboGender->Text : recipients[idx]->gender;
			RefreshList(String::Empty);
			UpdateStats();
			lblStatus->Text = String::Format(L" Recipient updated  |  {0}",
				DateTime::Now.ToString("HH:mm:ss"));
		}

		void DeleteRecipient(Object^ sender, EventArgs^ e) {
			if (listView->SelectedItems->Count == 0) {
				lblStatus->Text = "Please select a recipient to delete.";
				return;
			}
			String^ name = recipients[listView->SelectedItems[0]->Index]->name;
			if (MessageBox::Show(
				String::Format("Remove '{0}' from the registry?", name),
				"Confirm Delete", MessageBoxButtons::YesNo, MessageBoxIcon::Question)
				== System::Windows::Forms::DialogResult::Yes) {
				recipients->RemoveAt(listView->SelectedItems[0]->Index);
				RefreshList(String::Empty);
				ClearFields();
				UpdateStats();
				lblStatus->Text = String::Format(L"Recipient '{0}' removed  |  {1}",
					name, DateTime::Now.ToString("HH:mm:ss"));
			}
		}

		void ClearFields_Click(Object^ sender, EventArgs^ e) { ClearFields(); }

		// ─────────────────────────────────────────────────────────────────────
		//  SELECTION → POPULATE FORM
		// ─────────────────────────────────────────────────────────────────────
		void ListView_SelectionChanged(Object^ sender, EventArgs^ e) {
			if (listView->SelectedItems->Count == 0) return;
			int idx = listView->SelectedItems[0]->Index;
			Recipient^ r = recipients[idx];
			txtName->Text = r->name;
			comboBlood->SelectedItem = r->blood;
			numAge->Value = r->age;
			txtContact->Text = r->contact;
			int gi = comboGender->Items->IndexOf(r->gender);
			if (gi >= 0) comboGender->SelectedIndex = gi;
		}

		// ─────────────────────────────────────────────────────────────────────
		//  HELPERS
		// ─────────────────────────────────────────────────────────────────────

		void RefreshList(String^ filter) {
			listView->Items->Clear();
			int rowIdx = 0;
			for each (Recipient ^ r in recipients) {
				bool match = String::IsNullOrEmpty(filter) ||
					r->name->ToLower()->Contains(filter->ToLower()) ||
					r->blood->ToLower()->Contains(filter->ToLower()) ||
					r->contact->Contains(filter);
				if (!match) continue;
				ListViewItem^ item = gcnew ListViewItem("  " + r->name);
				item->SubItems->Add(r->blood);
				item->SubItems->Add(r->gender);
				item->SubItems->Add(r->age.ToString());
				item->SubItems->Add(r->contact);
				item->SubItems->Add(r->registeredOn.ToString("dd MMM yyyy  HH:mm"));
				listView->Items->Add(item);
				rowIdx++;
			}
		}

		void ClearFields() {
			txtName->Clear();
			comboBlood->SelectedIndex = -1;
			comboGender->SelectedIndex = -1;
			numAge->Value = 1;
			txtContact->Clear();
			listView->SelectedItems->Clear();
		}

		void UpdateStats() {
			int total = recipients->Count;
			int aPlus = 0, bPlus = 0, oPlus = 0;
			for each (Recipient ^ r in recipients) {
				if (r->blood == "A+")  aPlus++;
				if (r->blood == "B+")  bPlus++;
				if (r->blood == "O+")  oPlus++;
			}
			cardTotal->SetValue(total.ToString());
			cardAPos->SetValue(aPlus.ToString());
			cardBPos->SetValue(bPlus.ToString());
			cardOPos->SetValue(oPlus.ToString());
			lblStatus->Text = String::Format(
				L"Total: {0} recipients  |  A+: {1}  B+: {2}  O+: {3}",
				total, aPlus, bPlus, oPlus);
		}
	};
}