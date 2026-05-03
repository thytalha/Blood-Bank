// ================================================================
//  SignupForm.h  —  Blood Bank Management System
//  "Web-in-Desktop" Medical Aesthetic — Full Rewrite
//
//  LAYOUT (1260 x 690 px centred container):
//    • Full-screen  WindowState = Maximized | BorderStyle = None
//    • pnlMain      1260 × 690  — centres on Load + every Resize
//    • pnlLeftCard   820 × 600  — Registration form   (~65 %)
//    • pnlRightCard  390 × 600  — "Why Join Us?" info (~30 %)
//    • btnClose       52 ×  52  — Anchored Top|Right → Application::Exit()
//
//  FIELD → SQL COLUMN MAPPING:
//    txtFullName        → FullName          (nvarchar)
//    txtUsername        → Username          (nvarchar, UNIQUE)
//    txtContactDetails  → ContactDetails    (nvarchar)
//    dtpDOB             → Age              (INT, via CalculateAge)
//    cmbBloodGroup      → BloodGroup        (nvarchar, e.g. "A+")
//    cmbRole            → UserRole          (nvarchar)
//    txtAddress         → [Address]         (nvarchar)
//    txtPassword        → [Password]        (nvarchar)
//    txtConfirmPass     → validation only   (not stored)
//
//  BACKEND:
//    • Database::GetInstance() singleton — unchanged
//    • Blood groups: DISTINCT BloodGroup FROM Users; hardcoded fallback
//    • Age: CalculateAge(DateTime) helper (birthday-aware subtraction)
//    • Parameterised INSERT — SQL-injection safe via SqlCommand^ overload
//    • Database::UsernameExists() — checks for duplicate usernames
//
//  NAVIGATION:
//    • "Create Account"              → INSERT → success dialog → Close()
//    • "Already have an account?"   → this->Close()  (LoginForm shows)
//    • Floating "✕" (top-right)     → Application::Exit()
//
//  WIRING  (LoginForm.h  —  unchanged, same pattern as before):
//    void OnSignupClick(Object^ s, LinkLabelLinkClickedEventArgs^ e)
//    {
//        this->Hide();
//        SignupForm^ sf = gcnew SignupForm();
//        sf->FormClosed += gcnew FormClosedEventHandler(
//                              this, &LoginForm::OnChildClosed);
//        sf->Show();
//    }
// ================================================================

#pragma once
#include "Database.h"

#using <System.dll>
#using <System.Data.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <System.Xml.dll>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Data::SqlClient;

namespace BloodBank {

    public ref class SignupForm : public Form
    {

        // ══════════════════════════════════════════════════════════
        //  COLOUR PALETTE — single source of truth
        // ══════════════════════════════════════════════════════════
    private:
        Color CLR_BG_FORM;       // #F8F9FA  soft off-white form backdrop
        Color CLR_BG_CARD;       // #FFFFFF  pure white card surface
        Color CLR_ACCENT;        // #C0392B  crimson  — buttons / bullets
        Color CLR_ACCENT_DARK;   // #A93226  15 % darker — hover state
        Color CLR_TEXT;          // #212529  near-black primary text
        Color CLR_MUTED;         // #6C757D  medium grey labels / hints
        Color CLR_BORDER;        // #E9ECEF  pale grey card border / divider
        Color CLR_FIELD_BORDER;  // #DEE2E6  input box idle border
        Color CLR_HOVER_BG;      // #FADBD8  blush pink  — hover fills
        Color CLR_ERROR;         // #C0392B  same as accent — error text
        Color CLR_SUCCESS;       // #27AE60  green — success feedback
        Color CLR_INFO_BG;       // #FDECEA  very light rose — icon bg / box


        // ══════════════════════════════════════════════════════════
        //  CONTROL HANDLES
        // ══════════════════════════════════════════════════════════

        // ── Layout ───────────────────────────────────────────────
        Panel^ pnlMain;          // centred outer wrapper (both cards)
        Panel^ pnlLeftCard;      // white form card
        Panel^ pnlRightCard;     // info panel card
        Button^ btnClose;         // floating ✕, anchored Top|Right

        // ── Form fields ───────────────────────────────────────────
        TextBox^ txtFullName;
        TextBox^ txtUsername;       // labelled "Email Address" in mockup
        TextBox^ txtContactDetails;
        DateTimePicker^ dtpDOB;
        ComboBox^ cmbBloodGroup;
        ComboBox^ cmbRole;
        TextBox^ txtAddress;
        TextBox^ txtPassword;
        TextBox^ txtConfirmPass;
        CheckBox^ chkTerms;
        Button^ btnSubmit;
        Button^ btnBack;
        Label^ lblStatus;         // inline error / success strip

        // ── Field wrapper panels (border painting + focus glow) ───
        Panel^ pboxFullName;
        Panel^ pboxUsername;
        Panel^ pboxContact;
        Panel^ pboxAddress;
        Panel^ pboxPassword;
        Panel^ pboxConfirmPass;

        // Tracks which wrapper is currently focused for repaint
        Panel^ _focusedBox;


        // ══════════════════════════════════════════════════════════
        //  CONSTRUCTOR
        // ══════════════════════════════════════════════════════════
    public:
        SignupForm()
        {
            // ── Palette ───────────────────────────────────────────
            CLR_BG_FORM = ColorTranslator::FromHtml("#F8F9FA");
            CLR_BG_CARD = ColorTranslator::FromHtml("#FFFFFF");
            CLR_ACCENT = ColorTranslator::FromHtml("#C0392B");
            CLR_ACCENT_DARK = ColorTranslator::FromHtml("#A93226");
            CLR_TEXT = ColorTranslator::FromHtml("#212529");
            CLR_MUTED = ColorTranslator::FromHtml("#6C757D");
            CLR_BORDER = ColorTranslator::FromHtml("#E9ECEF");
            CLR_FIELD_BORDER = ColorTranslator::FromHtml("#DEE2E6");
            CLR_HOVER_BG = ColorTranslator::FromHtml("#FADBD8");
            CLR_ERROR = ColorTranslator::FromHtml("#C0392B");
            CLR_SUCCESS = ColorTranslator::FromHtml("#27AE60");
            CLR_INFO_BG = ColorTranslator::FromHtml("#FDECEA");
            _focusedBox = nullptr;

            InitializeComponent();
        }

    protected:
        ~SignupForm() {}


        // ══════════════════════════════════════════════════════════
        //  InitializeComponent
        // ══════════════════════════════════════════════════════════
    private:
        void InitializeComponent()
        {
            this->Text = "Create Account \u2014 LifeBlood";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;

            this->Load += gcnew EventHandler(this, &SignupForm::OnLoad);
            this->Resize += gcnew EventHandler(this, &SignupForm::OnResize);
            this->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

            // Build Z-order: close button last so it floats above cards
            BuildMainLayout();
            BuildCloseButton();
        }


        // ══════════════════════════════════════════════════════════
        //  CLOSE BUTTON
        //  Anchored Top|Right — always in the screen corner.
        //  Calls Application::Exit() (not Close, which would just
        //  return to LoginForm).
        // ══════════════════════════════════════════════════════════
        void BuildCloseButton()
        {
            btnClose = gcnew Button();
            btnClose->Text = Char::ConvertFromUtf32(0x2715); // ✕
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
            btnClose->Size = Drawing::Size(52, 52);
            btnClose->Location = Drawing::Point(this->Width - 60, 4);
            btnClose->Anchor = static_cast<AnchorStyles>(
                AnchorStyles::Top | AnchorStyles::Right);
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->FlatAppearance->MouseOverBackColor = CLR_HOVER_BG;
            btnClose->BackColor = Color::Transparent;
            btnClose->ForeColor = CLR_MUTED;
            btnClose->TextAlign = ContentAlignment::MiddleCenter;
            btnClose->Cursor = Cursors::Hand;

            btnClose->MouseEnter += gcnew EventHandler(this, &SignupForm::OnCloseEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &SignupForm::OnCloseLeave);
            btnClose->Click += gcnew EventHandler(this, &SignupForm::OnCloseClick);

            this->Controls->Add(btnClose);
        }


        // ══════════════════════════════════════════════════════════
        //  MAIN LAYOUT  (page title + both cards side by side)
        // ══════════════════════════════════════════════════════════
        void BuildMainLayout()
        {
            // Outer wrapper — centred in OnLoad / OnResize
            pnlMain = gcnew Panel();
            pnlMain->BackColor = CLR_BG_FORM;
            pnlMain->Size = Drawing::Size(1260, 690);
            pnlMain->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

            // ── Page title ───────────────────────────────────────
            Label^ lblPageTitle = gcnew Label();
            lblPageTitle->Text = "Create Your Account";
            lblPageTitle->Font = gcnew Drawing::Font(
                "Segoe UI Semibold", 20, FontStyle::Bold);
            lblPageTitle->ForeColor = CLR_ACCENT;
            lblPageTitle->AutoSize = true;
            lblPageTitle->Location = Drawing::Point(0, 0);
            pnlMain->Controls->Add(lblPageTitle);

            Label^ lblPageSub = gcnew Label();
            lblPageSub->Text = "Join LifeBlood and become part of a life-saving community.";
            lblPageSub->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblPageSub->ForeColor = CLR_MUTED;
            lblPageSub->AutoSize = true;
            lblPageSub->Location = Drawing::Point(2, 36);
            pnlMain->Controls->Add(lblPageSub);

            // ── Build both cards then add to wrapper ─────────────
            BuildLeftCard();
            BuildRightCard();

            // Right card must be added before left so left is on top
            pnlMain->Controls->Add(pnlRightCard);
            pnlMain->Controls->Add(pnlLeftCard);
            this->Controls->Add(pnlMain);
        }


        // ══════════════════════════════════════════════════════════
        //  LEFT CARD  (Registration Form)
        //  820 × 600 px — two-column field grid
        // ══════════════════════════════════════════════════════════
        void BuildLeftCard()
        {
            pnlLeftCard = gcnew Panel();
            pnlLeftCard->BackColor = CLR_BG_CARD;
            pnlLeftCard->Size = Drawing::Size(820, 600);
            pnlLeftCard->Location = Drawing::Point(0, 68);
            pnlLeftCard->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnCardPaint);
            pnlLeftCard->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

            // ── Card header: icon badge + title + subtitle ────────
            // Rounded icon badge (drawn via OnIconBadgePaint)
            Panel^ pnlBadge = gcnew Panel();
            pnlBadge->Size = Drawing::Size(46, 46);
            pnlBadge->Location = Drawing::Point(28, 20);
            pnlBadge->BackColor = CLR_INFO_BG;
            pnlBadge->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnIconBadgePaint);
            pnlLeftCard->Controls->Add(pnlBadge);

            // Icon inside badge — U+1F464 👤 person silhouette
            Label^ lblBadgeIcon = gcnew Label();
            lblBadgeIcon->Text = Char::ConvertFromUtf32(0x1F464);
            lblBadgeIcon->Font = gcnew Drawing::Font("Segoe UI Emoji", 18);
            lblBadgeIcon->Size = Drawing::Size(40, 40);
            lblBadgeIcon->Location = Drawing::Point(3, 2);
            lblBadgeIcon->TextAlign = ContentAlignment::MiddleCenter;
            pnlBadge->Controls->Add(lblBadgeIcon);

            Label^ lblCardTitle = gcnew Label();
            lblCardTitle->Text = "Registration Details";
            lblCardTitle->Font = gcnew Drawing::Font(
                "Segoe UI Semibold", 13, FontStyle::Bold);
            lblCardTitle->ForeColor = CLR_TEXT;
            lblCardTitle->AutoSize = true;
            lblCardTitle->Location = Drawing::Point(84, 18);
            pnlLeftCard->Controls->Add(lblCardTitle);

            Label^ lblCardSub = gcnew Label();
            lblCardSub->Text = "All fields are required unless marked optional.";
            lblCardSub->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblCardSub->ForeColor = CLR_MUTED;
            lblCardSub->AutoSize = true;
            lblCardSub->Location = Drawing::Point(84, 44);
            pnlLeftCard->Controls->Add(lblCardSub);

            // Thin horizontal divider beneath the header
            Panel^ divider = gcnew Panel();
            divider->BackColor = CLR_BORDER;
            divider->Size = Drawing::Size(760, 1);
            divider->Location = Drawing::Point(30, 82);
            pnlLeftCard->Controls->Add(divider);

            // ── Two-column field grid ─────────────────────────────
            // x1 = left col   x2 = right col   fw = column width   fh = field height
            const int x1 = 30, x2 = 425, fw = 365, fh = 36;
            int y = 100;   // first row label top

            // ── Row 1: Full Name  |  Username ────
            AddLabel(pnlLeftCard, "Full Name", x1, y);
            AddLabel(pnlLeftCard, "Username", x2, y);
            y += 22;
            pboxFullName = MakeFieldBox(pnlLeftCard, x1, y, fw, fh);
            txtFullName = AddTextBox(pboxFullName, "Talha Pasha", false);
            pboxUsername = MakeFieldBox(pnlLeftCard, x2, y, fw, fh);
            txtUsername = AddTextBox(pboxUsername, "thytalha", false);
            y += fh + 16;

            // ── Row 2: Phone Number  |  Date of Birth ────────────
            AddLabel(pnlLeftCard, "Phone Number", x1, y);
            AddLabel(pnlLeftCard, "Date of Birth", x2, y);
            y += 22;
            pboxContact = MakeFieldBox(pnlLeftCard, x1, y, fw, fh);
            txtContactDetails = AddTextBox(pboxContact, "+92 (301) 2345678", false);
            // DateTimePicker — placed directly (has own border rendering)
            dtpDOB = gcnew DateTimePicker();
            dtpDOB->Format = DateTimePickerFormat::Custom;
            dtpDOB->CustomFormat = "'MM/DD/YYYY'";
            dtpDOB->Font = gcnew Drawing::Font("Segoe UI", 10);
            dtpDOB->Size = Drawing::Size(fw, fh);
            dtpDOB->Location = Drawing::Point(x2, y);
            dtpDOB->MaxDate = DateTime::Today.AddYears(-16); // must be 16+
            dtpDOB->Value = dtpDOB->MaxDate;
            dtpDOB->Tag = false;
            dtpDOB->ValueChanged += gcnew EventHandler(this, &SignupForm::OnDobChanged);
            dtpDOB->DropDown += gcnew EventHandler(this, &SignupForm::OnDobChanged);
            pnlLeftCard->Controls->Add(dtpDOB);
            y += fh + 16;

            // ── Row 3: Blood Group  |  Role ──────────────────────
            AddLabel(pnlLeftCard, "Blood Group", x1, y);
            AddLabel(pnlLeftCard, "Role", x2, y);
            y += 22;
            cmbBloodGroup = MakeComboBox(pnlLeftCard, x1, y, fw, fh);
            // Blood groups populated in OnLoad → LoadBloodGroups()
            cmbRole = MakeComboBox(pnlLeftCard, x2, y, fw, fh);
            cmbRole->Items->AddRange(gcnew array<Object^>{ "Donor", "Recipient" });
            cmbRole->SelectedIndex = 0;
            y += fh + 16;

            // ── Row 4: Address (full width) ───────────────────────
            AddLabel(pnlLeftCard, "Address", x1, y);
            y += 22;
            pboxAddress = MakeFieldBox(pnlLeftCard, x1, y, 760, fh);
            txtAddress = AddTextBox(pboxAddress, "Street, City, State", false);
            y += fh + 16;

            // ── Row 5: Password  |  Confirm Password ─────────────
            AddLabel(pnlLeftCard, "Password", x1, y);
            AddLabel(pnlLeftCard, "Confirm Password", x2, y);
            y += 22;
            pboxPassword = MakeFieldBox(pnlLeftCard, x1, y, fw, fh);
            txtPassword = AddPasswordBox(pboxPassword, fw, fh);
            pboxConfirmPass = MakeFieldBox(pnlLeftCard, x2, y, fw, fh);
            txtConfirmPass = AddPasswordBox(pboxConfirmPass, fw, fh);
            y += fh + 22;

            // ── Terms checkbox ────────────────────────────────────
            chkTerms = gcnew CheckBox();
            chkTerms->Text = "I agree to the terms of service and donation eligibility criteria.";
            chkTerms->Font = gcnew Drawing::Font("Segoe UI", 9);
            chkTerms->ForeColor = CLR_TEXT;
            chkTerms->AutoSize = true;
            chkTerms->Location = Drawing::Point(x1, y);
            pnlLeftCard->Controls->Add(chkTerms);
            y += 30;

            // ── Inline status label (error / success) ─────────────
            lblStatus = gcnew Label();
            lblStatus->Text = "";
            lblStatus->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblStatus->ForeColor = CLR_ERROR;
            lblStatus->Size = Drawing::Size(760, 20);
            lblStatus->Location = Drawing::Point(x1, y);
            lblStatus->Visible = false;
            pnlLeftCard->Controls->Add(lblStatus);
            y += 24;

            // ── Action buttons ────────────────────────────────────
            // Primary: "Create Account" — solid crimson
            btnSubmit = gcnew Button();
            btnSubmit->Text = "Create Account";
            btnSubmit->Font = gcnew Drawing::Font(
                "Segoe UI Semibold", 10, FontStyle::Bold);
            btnSubmit->Size = Drawing::Size(170, 44);
            btnSubmit->Location = Drawing::Point(x1, y);
            btnSubmit->FlatStyle = FlatStyle::Flat;
            btnSubmit->FlatAppearance->BorderSize = 0;
            btnSubmit->BackColor = CLR_ACCENT;
            btnSubmit->ForeColor = Color::White;
            btnSubmit->Cursor = Cursors::Hand;
            btnSubmit->MouseEnter += gcnew EventHandler(
                this, &SignupForm::OnSubmitEnter);
            btnSubmit->MouseLeave += gcnew EventHandler(
                this, &SignupForm::OnSubmitLeave);
            btnSubmit->Click += gcnew EventHandler(
                this, &SignupForm::OnSubmitClick);
            pnlLeftCard->Controls->Add(btnSubmit);

            // Secondary: "Already have an account?" — outlined
            btnBack = gcnew Button();
            btnBack->Text = "Already have an account? Login";
            btnBack->Font = gcnew Drawing::Font("Segoe UI", 10);
            btnBack->Size = Drawing::Size(240, 44);
            btnBack->Location = Drawing::Point(x1 + 180, y);
            btnBack->FlatStyle = FlatStyle::Flat;
            btnBack->FlatAppearance->BorderSize = 1;
            btnBack->FlatAppearance->BorderColor = CLR_ACCENT;
            btnBack->FlatAppearance->MouseOverBackColor = CLR_HOVER_BG;
            btnBack->BackColor = CLR_BG_CARD;
            btnBack->ForeColor = CLR_ACCENT;
            btnBack->Cursor = Cursors::Hand;
            btnBack->Click += gcnew EventHandler(
                this, &SignupForm::OnBackClick);
            pnlLeftCard->Controls->Add(btnBack);
        }


        // ══════════════════════════════════════════════════════════
        //  RIGHT CARD  ("Why Join Us?" info panel)
        //  390 × 600 px
        // ══════════════════════════════════════════════════════════
        void BuildRightCard()
        {
            pnlRightCard = gcnew Panel();
            pnlRightCard->BackColor = CLR_BG_CARD;
            pnlRightCard->Size = Drawing::Size(390, 600);
            pnlRightCard->Location = Drawing::Point(870, 68);
            pnlRightCard->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnCardPaint);
            pnlRightCard->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

            // Section title
            Label^ lblTitle = gcnew Label();
            lblTitle->Text = "Why Join Us?";
            lblTitle->Font = gcnew Drawing::Font(
                "Segoe UI Semibold", 13, FontStyle::Bold);
            lblTitle->ForeColor = CLR_TEXT;
            lblTitle->AutoSize = true;
            lblTitle->Location = Drawing::Point(24, 28);
            pnlRightCard->Controls->Add(lblTitle);

            // Bullet point entries
            array<String^>^ points = gcnew array<String^> {
                "Track your donation history and impact",
                    "Get notified when your blood type is urgently needed",
                    "Easily request blood for yourself or a loved one",
                    "Access hospital partner network nationwide"
            };

            int y = 70;
            for each (String ^ text in points)
            {
                // Crimson bullet dot  U+2022 •
                Label^ dot = gcnew Label();
                dot->Text = Char::ConvertFromUtf32(0x2022); // •
                dot->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Bold);
                dot->ForeColor = CLR_ACCENT;
                dot->Size = Drawing::Size(18, 24);
                dot->Location = Drawing::Point(20, y + 2);
                dot->TextAlign = ContentAlignment::MiddleCenter;
                pnlRightCard->Controls->Add(dot);

                Label^ lbl = gcnew Label();
                lbl->Text = text;
                lbl->Font = gcnew Drawing::Font("Segoe UI", 10);
                lbl->ForeColor = CLR_TEXT;
                lbl->Size = Drawing::Size(316, 40);
                lbl->Location = Drawing::Point(42, y);
                pnlRightCard->Controls->Add(lbl);

                y += 46;
            }

            // ── "Did you know?" info box ──────────────────────────
            Panel^ pnlInfo = gcnew Panel();
            pnlInfo->BackColor = CLR_INFO_BG;
            pnlInfo->Size = Drawing::Size(338, 82);
            pnlInfo->Location = Drawing::Point(24, y + 12);
            pnlInfo->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnInfoBoxPaint);
            pnlRightCard->Controls->Add(pnlInfo);

            Label^ lblDYKTitle = gcnew Label();
            lblDYKTitle->Text = "Did you know?";
            lblDYKTitle->Font = gcnew Drawing::Font(
                "Segoe UI Semibold", 10, FontStyle::Bold);
            lblDYKTitle->ForeColor = CLR_ACCENT;
            lblDYKTitle->AutoSize = true;
            lblDYKTitle->Location = Drawing::Point(14, 12);
            pnlInfo->Controls->Add(lblDYKTitle);

            Label^ lblDYKText = gcnew Label();
            lblDYKText->Text = "A single blood donation can save up to three lives.";
            lblDYKText->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblDYKText->ForeColor = CLR_TEXT;
            lblDYKText->Size = Drawing::Size(310, 38);
            lblDYKText->Location = Drawing::Point(14, 36);
            pnlInfo->Controls->Add(lblDYKText);
        }


        // ══════════════════════════════════════════════════════════
        //  FIELD FACTORY HELPERS
        // ══════════════════════════════════════════════════════════

        // ── Shared field label (sits above every input) ───────────
        void AddLabel(Panel^ parent, String^ text, int x, int y)
        {
            Label^ lbl = gcnew Label();
            lbl->Text = text;
            lbl->Font = gcnew Drawing::Font("Segoe UI", 9);
            lbl->ForeColor = CLR_TEXT;
            lbl->AutoSize = true;
            lbl->Location = Drawing::Point(x, y);
            parent->Controls->Add(lbl);
        }

        // ── Border wrapper panel for TextBox controls ─────────────
        //    OnFieldBoxPaint draws a 1-px border (crimson when focused).
        Panel^ MakeFieldBox(Panel^ parent, int x, int y, int w, int h)
        {
            Panel^ box = gcnew Panel();
            box->Size = Drawing::Size(w, h);
            box->Location = Drawing::Point(x, y);
            box->BackColor = Color::White;
            box->Tag = (Object^)false; // false = not focused
            box->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnFieldBoxPaint);
            parent->Controls->Add(box);
            return box;
        }

        // ── Plain text input with placeholder simulation ──────────
        //    Placeholder text is stored in TextBox::Tag.
        //    GotFocus clears it; LostFocus restores it if still empty.
        TextBox^ AddTextBox(Panel^ box, String^ placeholder, bool isPass)
        {
            TextBox^ tb = gcnew TextBox();
            tb->BorderStyle = BorderStyle::None;
            tb->Font = gcnew Drawing::Font("Segoe UI", 10);
            tb->BackColor = Color::White;
            tb->Size = Drawing::Size(box->Width - 14, 22);
            tb->Location = Drawing::Point(7, (box->Height - 22) / 2);
            tb->Tag = placeholder;
            tb->Text = placeholder;
            tb->ForeColor = CLR_MUTED;
            tb->GotFocus += gcnew EventHandler(this, &SignupForm::OnTbFocus);
            tb->LostFocus += gcnew EventHandler(this, &SignupForm::OnTbBlur);
            box->Controls->Add(tb);
            return tb;
        }

        // ── Password input — no placeholder (label above is clear) ──
        TextBox^ AddPasswordBox(Panel^ box, int w, int h)
        {
            TextBox^ tb = gcnew TextBox();
            tb->BorderStyle = BorderStyle::None;
            tb->Font = gcnew Drawing::Font("Segoe UI", 10);
            tb->BackColor = Color::White;
            tb->ForeColor = CLR_MUTED;
            tb->UseSystemPasswordChar = false;
            tb->Size = Drawing::Size(w - 14, 22);
            tb->Location = Drawing::Point(7, (h - 22) / 2);
            tb->Tag = "......";
			tb->Text = Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) +
				Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022); // ••••••••
            tb->GotFocus += gcnew EventHandler(this, &SignupForm::OnTbFocus);
            tb->LostFocus += gcnew EventHandler(this, &SignupForm::OnTbBlur);
            box->Controls->Add(tb);
            return tb;
        }

        // ── Drop-down selector ────────────────────────────────────
        ComboBox^ MakeComboBox(Panel^ parent, int x, int y, int w, int h)
        {
            ComboBox^ cmb = gcnew ComboBox();
            cmb->FlatStyle = FlatStyle::Flat;
            cmb->DropDownStyle = ComboBoxStyle::DropDownList;
            cmb->Font = gcnew Drawing::Font("Segoe UI", 10);
            cmb->ForeColor = CLR_TEXT;
            cmb->BackColor = Color::White;
            cmb->Size = Drawing::Size(w, h);
            cmb->Location = Drawing::Point(x, y);
            parent->Controls->Add(cmb);
            return cmb;
        }


        // ══════════════════════════════════════════════════════════
        //  CENTERING LOGIC
        //  Called on Load and every Resize — keeps pnlMain dead-centre
        //  at any resolution.
        // ══════════════════════════════════════════════════════════
        void CenterMain()
        {
            pnlMain->Location = Drawing::Point(
                (this->ClientSize.Width - pnlMain->Width) / 2,
                (this->ClientSize.Height - pnlMain->Height) / 2
            );
        }


        // ══════════════════════════════════════════════════════════
        //  PAINT HANDLERS
        // ══════════════════════════════════════════════════════════

        // 1-px CLR_BORDER outline around a card panel
        void OnCardPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }

        // Field wrapper — 1-px idle border / 2-px crimson focused border
        void OnFieldBoxPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            bool  focused = (p == _focusedBox);
            Color bclr = focused ? CLR_ACCENT : CLR_FIELD_BORDER;
            float bw = focused ? 2.0f : 1.0f;
            Pen^ pen = gcnew Pen(bclr, bw);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }

        // Icon badge — filled circle in CLR_INFO_BG tint
        void OnIconBadgePaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            SolidBrush^ br = gcnew SolidBrush(CLR_INFO_BG);
            e->Graphics->SmoothingMode =
                Drawing::Drawing2D::SmoothingMode::AntiAlias;
            e->Graphics->FillEllipse(br, 0, 0, p->Width - 1, p->Height - 1);
            delete br;
        }

        // "Did you know?" box — light rose border
        void OnInfoBoxPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(ColorTranslator::FromHtml("#F5C6C6"), 1);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }


        // ══════════════════════════════════════════════════════════
        //  FOCUS MANAGEMENT  (placeholder reveal + border glow)
        // ══════════════════════════════════════════════════════════
        void OnTbFocus(Object^ s, EventArgs^ e)
        {
            TextBox^ tb = safe_cast<TextBox^>(s);
            String^ placeholder = tb->Tag ? tb->Tag->ToString() : "";

            // Clear placeholder text on focus (non-password fields only)
            if (!String::IsNullOrEmpty(placeholder) && tb->Text == placeholder)
            {
                tb->Text = "";
                tb->ForeColor = CLR_TEXT;
                if (placeholder == "......")
                    tb->UseSystemPasswordChar = true;
            }

            // Highlight the wrapper panel border
            Panel^ box = safe_cast<Panel^>(tb->Parent);
            _focusedBox = box;
            box->Invalidate();
        }

        void OnTbBlur(Object^ s, EventArgs^ e)
        {
            TextBox^ tb = safe_cast<TextBox^>(s);
            String^ placeholder = tb->Tag ? tb->Tag->ToString() : "";

            // Restore placeholder if field is still empty
            if (!String::IsNullOrEmpty(placeholder) &&
                String::IsNullOrEmpty(tb->Text))
            {
                tb->Text = placeholder;
                tb->ForeColor = CLR_MUTED;
                if (placeholder == "......")
                    tb->UseSystemPasswordChar = false;
            }

            Panel^ box = safe_cast<Panel^>(tb->Parent);
            if (_focusedBox == box) _focusedBox = nullptr;
            box->Invalidate();
        }


        // ══════════════════════════════════════════════════════════
        //  DOB PLACEHOLDER HANDLING
        // ══════════════════════════════════════════════════════════
        void OnDobChanged(Object^ s, EventArgs^ e)
        {
            if (dtpDOB->CustomFormat != "MM/dd/yyyy")
                dtpDOB->CustomFormat = "MM/dd/yyyy";
            dtpDOB->Tag = true;
        }


        // ══════════════════════════════════════════════════════════
        //  EVENT HANDLERS
        // ══════════════════════════════════════════════════════════
        void OnBackgroundClick(Object^ s, EventArgs^ e)
        {
            this->ActiveControl = nullptr;
        }

        void OnLoad(Object^ s, EventArgs^ e)
        {
            CenterMain();
            LoadBloodGroups();
            this->BeginInvoke(gcnew Action(this, &SignupForm::ClearInitialFocus));
        }

        void ClearInitialFocus()
        {
            this->ActiveControl = nullptr;
        }

        void OnResize(Object^ s, EventArgs^ e) { CenterMain(); }

        // Floating ✕ button — exits the entire application
        void OnCloseClick(Object^ s, EventArgs^ e) { Application::Exit(); }
        void OnCloseEnter(Object^ s, EventArgs^ e) { btnClose->ForeColor = CLR_ERROR; }
        void OnCloseLeave(Object^ s, EventArgs^ e) { btnClose->ForeColor = CLR_MUTED; }

        // "Create Account" hover — darkens background
        void OnSubmitEnter(Object^ s, EventArgs^ e) { btnSubmit->BackColor = CLR_ACCENT_DARK; }
        void OnSubmitLeave(Object^ s, EventArgs^ e) { btnSubmit->BackColor = CLR_ACCENT; }

        // "Already have an account?" — returns to LoginForm
        void OnBackClick(Object^ s, EventArgs^ e) { this->Close(); }


        // ══════════════════════════════════════════════════════════
        //  BLOOD GROUP LOADER
        //
        //  Strategy 1: SELECT DISTINCT BloodGroup FROM Users — reads
        //    whatever groups are already in the database so the list
        //    is always in sync with actual data.
        //  Strategy 2: If the query returns no rows (fresh DB) or
        //    fails for any reason, fall back to the canonical 8-type
        //    hardcoded list.
        //
        //  NOTE: Users.BloodGroup stores the group name string directly
        //    (e.g. "A+"), NOT a foreign-key integer, as confirmed by the
        //    live db_bloodbank schema screenshot. The INSERT also uses
        //    the string value.
        // ══════════════════════════════════════════════════════════
        void LoadBloodGroups()
        {
            bool loaded = false;
            try
            {
                SqlDataReader^ rdr = Database::GetInstance()->ExecuteReader(
                    "SELECT DISTINCT BloodGroup "
                    "FROM   Users "
                    "WHERE  BloodGroup IS NOT NULL AND BloodGroup <> '' "
                    "ORDER  BY BloodGroup");

                if (rdr != nullptr)
                {
                    while (rdr->Read())
                    {
                        cmbBloodGroup->Items->Add(rdr[0]->ToString());
                        loaded = true;
                    }
                    rdr->Close();
                }
            }
            catch (Exception^)
            {
                // Silent — fall through to hardcoded fallback below
            }

            if (!loaded)
                PopulateFallbackBloodGroups();

            if (cmbBloodGroup->Items->Count > 0)
                cmbBloodGroup->SelectedIndex = 0;
        }

        void PopulateFallbackBloodGroups()
        {
            array<String^>^ groups = gcnew array<String^> {
                "A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"
            };
            for each (String ^ g in groups)
                cmbBloodGroup->Items->Add(g);
        }


        // ══════════════════════════════════════════════════════════
        //  AGE HELPER
        //  Birthday-aware age calculation in full calendar years.
        //  Subtracts 1 if the birthday this year hasn't occurred yet.
        // ══════════════════════════════════════════════════════════
        int CalculateAge(DateTime dob)
        {
            DateTime today = DateTime::Today;
            int age = today.Year - dob.Year;
            // Adjust if the current date is before this year's birthday
            if (dob.Date > today.AddYears(-age))
                age--;
            return age;
        }


        // ══════════════════════════════════════════════════════════
        //  GetFieldText
        //  Returns the actual typed value, stripping the placeholder
        //  (which is stored in TextBox::Tag).
        // ══════════════════════════════════════════════════════════
        String^ GetFieldText(TextBox^ tb)
        {
            String^ placeholder = tb->Tag ? tb->Tag->ToString() : "";
            String^ val = tb->Text->Trim();
            return (val == placeholder) ? "" : val;
        }


        // ══════════════════════════════════════════════════════════
        //  ShowStatus
        //  Surfaces an inline message below the Terms checkbox.
        //  isError = true  → CLR_ERROR (crimson)
        //  isError = false → CLR_SUCCESS (green)
        // ══════════════════════════════════════════════════════════
        void ShowStatus(String^ msg, bool isError)
        {
            lblStatus->Text = msg;
            lblStatus->ForeColor = isError ? CLR_ERROR : CLR_SUCCESS;
            lblStatus->Visible = true;
        }


        // ══════════════════════════════════════════════════════════
        //  SUBMIT HANDLER  ("Create Account")
        //
        //  Pipeline:
        //    1. Gather & trim all inputs (placeholder-aware)
        //    2. Client-side validation  (required fields, age, passwords)
        //    3. Duplicate username check via Database::UsernameExists()
        //    4. Parameterised INSERT INTO Users (all 8 columns)
        //    5. Success dialog → this->Close()  (LoginForm resurfaces)
        // ══════════════════════════════════════════════════════════
        void OnSubmitClick(Object^ s, EventArgs^ e)
        {
            lblStatus->Visible = false;

            // ── 1. Gather inputs ──────────────────────────────────
            String^ fullName = GetFieldText(txtFullName);
            String^ username = GetFieldText(txtUsername);
            String^ contact = GetFieldText(txtContactDetails);
            String^ address = GetFieldText(txtAddress);
            String^ password = txtPassword->Text;     // no placeholder to strip
            String^ confirm = txtConfirmPass->Text;

            String^ bloodGroup = (cmbBloodGroup->SelectedItem != nullptr)
                ? cmbBloodGroup->SelectedItem->ToString()
                : "";
            String^ role = (cmbRole->SelectedItem != nullptr)
                ? cmbRole->SelectedItem->ToString()
                : "Donor";

            int age = CalculateAge(dtpDOB->Value);

            // ── 2. Validation ─────────────────────────────────────
            if (String::IsNullOrWhiteSpace(fullName))
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Full Name is required.", true);
                txtFullName->Focus();
                return;
            }
            if (String::IsNullOrWhiteSpace(username))
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Email / Username is required.", true);
                txtUsername->Focus();
                return;
            }
            if (String::IsNullOrWhiteSpace(contact))
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Phone Number is required.", true);
                txtContactDetails->Focus();
                return;
            }
            if (dtpDOB->Tag == nullptr || !safe_cast<bool>(dtpDOB->Tag))
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Please select a Date of Birth.", true);
                dtpDOB->Focus();
                return;
            }
            if (age < 16)
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  You must be at least 16 years old to register.", true);
                return;
            }
            if (String::IsNullOrEmpty(bloodGroup))
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Please select a Blood Group.", true);
                cmbBloodGroup->Focus();
                return;
            }
            if (String::IsNullOrWhiteSpace(password) || password->Length < 4)
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Password must be at least 4 characters.", true);
                txtPassword->Focus();
                return;
            }
            if (password != confirm)
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Passwords do not match. Please re-enter.", true);
                txtConfirmPass->Clear();
                txtConfirmPass->Focus();
                return;
            }
            if (!chkTerms->Checked)
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  Please agree to the terms of service.", true);
                return;
            }

            // ── 3. Duplicate username check ───────────────────────
            if (Database::GetInstance()->UsernameExists(username))
            {
                ShowStatus(Char::ConvertFromUtf32(0x26A0) +
                    "  That username is already taken.", true);
                MessageBox::Show(
                    "The username \"" + username + "\" is already registered.\n\n"
                    "Please choose a different username and try again.",
                    "Username Unavailable",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Warning);
                return;
            }

            // ── 4. Parameterised INSERT INTO Users ────────────────
            //
            //  Column mapping (db_bloodbank schema, confirmed via screenshot):
            //    Username       → @uname   (unique login name / email)
            //    [Password]     → @pwd     (plain-text; hash recommended later)
            //    UserRole       → @role    ("Donor" | "Recipient")
            //    FullName       → @fn
            //    ContactDetails → @cd      (phone number)
            //    Age            → @age     (INT, birthday-aware calculation)
            //    [Address]      → @addr
            //    BloodGroup     → @bg      (string, e.g. "A+")
            //
            SqlCommand^ cmd = gcnew SqlCommand(
                "INSERT INTO Users "
                "       (Username, [Password], Role, FullName, "
                "        ContactDetails, Age, [Address], BloodGroup) "
                "VALUES (@uname, @pwd, @role, @fn, @cd, @age, @addr, @bg)");

            cmd->Parameters->AddWithValue("@uname", username);
            cmd->Parameters->AddWithValue("@pwd", password);
            cmd->Parameters->AddWithValue("@role", role);
            cmd->Parameters->AddWithValue("@fn", fullName);
            cmd->Parameters->AddWithValue("@cd", contact);
            cmd->Parameters->AddWithValue("@age", age);
            cmd->Parameters->AddWithValue("@addr",
                String::IsNullOrWhiteSpace(address) ? "" : address);
            cmd->Parameters->AddWithValue("@bg", bloodGroup);

            int rows = Database::GetInstance()->ExecuteNonQuery(cmd);

            // ── 5. Post-insert feedback ───────────────────────────
            if (rows > 0)
            {
                // Role-specific emoji greeting
                String^ roleEmoji = (role == "Donor")
                    ? Char::ConvertFromUtf32(0x1F489)   // 💉 syringe
                    : Char::ConvertFromUtf32(0x1F3E5);  // 🏥 hospital

                MessageBox::Show(
                    roleEmoji + "  Welcome to LifeBlood, " + fullName + "!\n\n"
                    "Your account has been created successfully.\n"
                    "Role         :  " + role + "\n"
                    "Username  :  " + username + "\n"
                    "Blood Group :  " + bloodGroup + "\n"
                    "Age           :  " + age.ToString() + "\n\n"
                    "You can now log in with your credentials.\n"
                    "Thank you for joining the LifeBlood community "
                    + Char::ConvertFromUtf32(0x2764),     // ❤
                    "Registration Successful",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information);

                // Close returns to LoginForm (OnChildClosed fires automatically)
                this->Close();
            }
            // If rows <= 0, Database singleton already displayed an error dialog.
        }

    };  // ref class SignupForm

}   // namespace BloodBank


// ================================================================
//  SQL SCHEMA ALIGNMENT (db_bloodbank — confirmed via screenshot)
// ================================================================
//
//  TABLE: Users
//  ─────────────────────────────────────────────────────────────
//  Column          Type        Source
//  ─────────────── ─────────── ────────────────────────────────
//  UserID          INT IDENTITY auto-generated (not inserted)
//  Username        NVARCHAR    txtUsername     (labelled Email)
//  [Password]      NVARCHAR    txtPassword
//  UserRole        NVARCHAR    cmbRole         ("Donor"/"Recipient")
//  FullName        NVARCHAR    txtFullName
//  ContactDetails  NVARCHAR    txtContactDetails
//  Age             INT         CalculateAge(dtpDOB.Value)
//  [Address]       NVARCHAR    txtAddress
//  BloodGroup      NVARCHAR    cmbBloodGroup   (stored as "A+", etc.)
//
//  Note: Admin accounts are NOT creatable via public signup — by
//  design. Seed admins directly in the database.
//
// ================================================================
//  EMOJI CODEPOINTS
// ================================================================
//
//  U+2715   ✕  Close button
//  U+1F464  👤  Person silhouette — card header badge
//  U+2022   •  Bullet dot — right card list
//  U+26A0   ⚠  Warning — inline validation messages
//  U+1F489  💉  Syringe — Donor success greeting
//  U+1F3E5  🏥  Hospital — Recipient success greeting
//  U+2764   ❤  Heart — success dialog footer
//
// ================================================================
//  LAYOUT DIMENSIONS (at design target: 1366 × 768 and above)
// ================================================================
//
//  pnlMain       1260 × 690   centred on form
//  pnlLeftCard    820 × 600   at (0,   68) in pnlMain
//  pnlRightCard   390 × 600   at (870, 68) in pnlMain
//  btnClose        52 ×  52   anchored Top|Right on form
//
// ================================================================