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
using namespace System::Text::RegularExpressions;

namespace BloodBank {

    public ref class SignupForm : public Form
    {
    private:
        Color CLR_BG_FORM;
        Color CLR_BG_CARD;
        Color CLR_ACCENT;
        Color CLR_ACCENT_DARK;
        Color CLR_TEXT;
        Color CLR_MUTED;
        Color CLR_BORDER;
        Color CLR_FIELD_BORDER;
        Color CLR_HOVER_BG;
        Color CLR_ERROR;
        Color CLR_SUCCESS;
        Color CLR_INFO_BG;

        Panel^ pnlMain;
        Panel^ pnlLeftCard;
        Panel^ pnlRightCard;
        Button^ btnClose;

        TextBox^ txtFullName;
        TextBox^ txtUsername;
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
        Label^ lblStatus;

        Panel^ pboxFullName;
        Panel^ pboxUsername;
        Panel^ pboxContact;
        Panel^ pboxAddress;
        Panel^ pboxPassword;
        Panel^ pboxConfirmPass;

        Panel^ _focusedBox;

    public:
        SignupForm()
        {
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

            BuildMainLayout();
            BuildCloseButton();
        }

        void BuildCloseButton()
        {
            btnClose = gcnew Button();
            btnClose->Text = Char::ConvertFromUtf32(0x2715);
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

        void BuildMainLayout()
        {
            pnlMain = gcnew Panel();
            pnlMain->BackColor = CLR_BG_FORM;
            pnlMain->Size = Drawing::Size(1260, 690);
            pnlMain->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

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

            BuildLeftCard();
            BuildRightCard();

            pnlMain->Controls->Add(pnlRightCard);
            pnlMain->Controls->Add(pnlLeftCard);
            this->Controls->Add(pnlMain);
        }

        void BuildLeftCard()
        {
            pnlLeftCard = gcnew Panel();
            pnlLeftCard->BackColor = CLR_BG_CARD;
            pnlLeftCard->Size = Drawing::Size(820, 600);
            pnlLeftCard->Location = Drawing::Point(0, 68);
            pnlLeftCard->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnCardPaint);
            pnlLeftCard->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

            Panel^ pnlBadge = gcnew Panel();
            pnlBadge->Size = Drawing::Size(46, 46);
            pnlBadge->Location = Drawing::Point(28, 20);
            pnlBadge->BackColor = CLR_INFO_BG;
            pnlBadge->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnIconBadgePaint);
            pnlLeftCard->Controls->Add(pnlBadge);

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

            Panel^ divider = gcnew Panel();
            divider->BackColor = CLR_BORDER;
            divider->Size = Drawing::Size(760, 1);
            divider->Location = Drawing::Point(30, 82);
            pnlLeftCard->Controls->Add(divider);

            const int x1 = 30, x2 = 425, fw = 365, fh = 36;
            int y = 100;

            AddLabel(pnlLeftCard, "Full Name", x1, y);
            AddLabel(pnlLeftCard, "Username", x2, y);
            y += 22;
            pboxFullName = MakeFieldBox(pnlLeftCard, x1, y, fw, fh);
            txtFullName = AddTextBox(pboxFullName, "Talha Pasha", false);
            pboxUsername = MakeFieldBox(pnlLeftCard, x2, y, fw, fh);
            txtUsername = AddTextBox(pboxUsername, "thytalha", false);
            y += fh + 16;

            AddLabel(pnlLeftCard, "Phone Number", x1, y);
            AddLabel(pnlLeftCard, "Date of Birth", x2, y);
            y += 22;
            pboxContact = MakeFieldBox(pnlLeftCard, x1, y, fw, fh);
            txtContactDetails = AddTextBox(pboxContact, "+92 (301) 2345678", false);
            dtpDOB = gcnew DateTimePicker();
            dtpDOB->Format = DateTimePickerFormat::Custom;
            dtpDOB->CustomFormat = "'MM/DD/YYYY'";
            dtpDOB->Font = gcnew Drawing::Font("Segoe UI", 10);
            dtpDOB->Size = Drawing::Size(fw, fh);
            dtpDOB->Location = Drawing::Point(x2, y);
            dtpDOB->MaxDate = DateTime::Today.AddYears(-16);
            dtpDOB->Value = dtpDOB->MaxDate;
            dtpDOB->Tag = false;
            dtpDOB->ValueChanged += gcnew EventHandler(this, &SignupForm::OnDobChanged);
            dtpDOB->DropDown += gcnew EventHandler(this, &SignupForm::OnDobChanged);
            pnlLeftCard->Controls->Add(dtpDOB);
            y += fh + 16;

            AddLabel(pnlLeftCard, "Blood Group", x1, y);
            AddLabel(pnlLeftCard, "Role", x2, y);
            y += 22;
            cmbBloodGroup = MakeComboBox(pnlLeftCard, x1, y, fw, fh);
            cmbRole = MakeComboBox(pnlLeftCard, x2, y, fw, fh);
            cmbRole->Items->AddRange(gcnew array<Object^>{ "Donor", "Recipient" });
            cmbRole->SelectedIndex = 0;
            y += fh + 16;

            AddLabel(pnlLeftCard, "Address", x1, y);
            y += 22;
            pboxAddress = MakeFieldBox(pnlLeftCard, x1, y, 760, fh);
            txtAddress = AddTextBox(pboxAddress, "Street, City, State", false);
            y += fh + 16;

            AddLabel(pnlLeftCard, "Password", x1, y);
            AddLabel(pnlLeftCard, "Confirm Password", x2, y);
            y += 22;
            pboxPassword = MakeFieldBox(pnlLeftCard, x1, y, fw, fh);
            txtPassword = AddPasswordBox(pboxPassword, fw, fh);
            pboxConfirmPass = MakeFieldBox(pnlLeftCard, x2, y, fw, fh);
            txtConfirmPass = AddPasswordBox(pboxConfirmPass, fw, fh);
            y += fh + 22;

            chkTerms = gcnew CheckBox();
            chkTerms->Text = "I agree to the terms of service and donation eligibility criteria.";
            chkTerms->Font = gcnew Drawing::Font("Segoe UI", 9);
            chkTerms->ForeColor = CLR_TEXT;
            chkTerms->AutoSize = true;
            chkTerms->Location = Drawing::Point(x1, y);
            pnlLeftCard->Controls->Add(chkTerms);
            y += 30;

            lblStatus = gcnew Label();
            lblStatus->Text = "";
            lblStatus->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblStatus->ForeColor = CLR_ERROR;
            lblStatus->Size = Drawing::Size(760, 20);
            lblStatus->Location = Drawing::Point(x1, y);
            lblStatus->Visible = false;
            pnlLeftCard->Controls->Add(lblStatus);
            y += 24;

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

        void BuildRightCard()
        {
            pnlRightCard = gcnew Panel();
            pnlRightCard->BackColor = CLR_BG_CARD;
            pnlRightCard->Size = Drawing::Size(390, 600);
            pnlRightCard->Location = Drawing::Point(870, 68);
            pnlRightCard->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnCardPaint);
            pnlRightCard->Click += gcnew EventHandler(this, &SignupForm::OnBackgroundClick);

            Label^ lblTitle = gcnew Label();
            lblTitle->Text = "Why Join Us?";
            lblTitle->Font = gcnew Drawing::Font(
                "Segoe UI Semibold", 13, FontStyle::Bold);
            lblTitle->ForeColor = CLR_TEXT;
            lblTitle->AutoSize = true;
            lblTitle->Location = Drawing::Point(24, 28);
            pnlRightCard->Controls->Add(lblTitle);

            array<String^>^ points = gcnew array<String^> {
                "Track your donation history and impact",
                    "Get notified when your blood type is urgently needed",
                    "Easily request blood for yourself or a loved one",
                    "Access hospital partner network nationwide"
            };

            int y = 70;
            for each (String ^ text in points)
            {
                Label^ dot = gcnew Label();
                dot->Text = Char::ConvertFromUtf32(0x2022);
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

        Panel^ MakeFieldBox(Panel^ parent, int x, int y, int w, int h)
        {
            Panel^ box = gcnew Panel();
            box->Size = Drawing::Size(w, h);
            box->Location = Drawing::Point(x, y);
            box->BackColor = Color::White;
            box->Tag = (Object^)false;
            box->Paint += gcnew PaintEventHandler(
                this, &SignupForm::OnFieldBoxPaint);
            parent->Controls->Add(box);
            return box;
        }

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
                Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022) + Char::ConvertFromUtf32(0x2022);
            tb->GotFocus += gcnew EventHandler(this, &SignupForm::OnTbFocus);
            tb->LostFocus += gcnew EventHandler(this, &SignupForm::OnTbBlur);
            box->Controls->Add(tb);
            return tb;
        }

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

        void CenterMain()
        {
            pnlMain->Location = Drawing::Point(
                (this->ClientSize.Width - pnlMain->Width) / 2,
                (this->ClientSize.Height - pnlMain->Height) / 2
            );
        }

        void OnCardPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }

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

        void OnIconBadgePaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            SolidBrush^ br = gcnew SolidBrush(CLR_INFO_BG);
            e->Graphics->SmoothingMode =
                Drawing::Drawing2D::SmoothingMode::AntiAlias;
            e->Graphics->FillEllipse(br, 0, 0, p->Width - 1, p->Height - 1);
            delete br;
        }

        void OnInfoBoxPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(ColorTranslator::FromHtml("#F5C6C6"), 1);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }

        void OnTbFocus(Object^ s, EventArgs^ e)
        {
            TextBox^ tb = safe_cast<TextBox^>(s);
            String^ placeholder = tb->Tag ? tb->Tag->ToString() : "";

            if (!String::IsNullOrEmpty(placeholder) && tb->Text == placeholder)
            {
                tb->Text = "";
                tb->ForeColor = CLR_TEXT;
                if (placeholder == "......")
                    tb->UseSystemPasswordChar = true;
            }

            Panel^ box = safe_cast<Panel^>(tb->Parent);
            _focusedBox = box;
            box->Invalidate();
        }

        void OnTbBlur(Object^ s, EventArgs^ e)
        {
            TextBox^ tb = safe_cast<TextBox^>(s);
            String^ placeholder = tb->Tag ? tb->Tag->ToString() : "";

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

        void OnDobChanged(Object^ s, EventArgs^ e)
        {
            if (dtpDOB->CustomFormat != "MM/dd/yyyy")
                dtpDOB->CustomFormat = "MM/dd/yyyy";
            dtpDOB->Tag = true;
        }

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

        void OnCloseClick(Object^ s, EventArgs^ e) { Application::Exit(); }
        void OnCloseEnter(Object^ s, EventArgs^ e) { btnClose->ForeColor = CLR_ERROR; }
        void OnCloseLeave(Object^ s, EventArgs^ e) { btnClose->ForeColor = CLR_MUTED; }

        void OnSubmitEnter(Object^ s, EventArgs^ e) { btnSubmit->BackColor = CLR_ACCENT_DARK; }
        void OnSubmitLeave(Object^ s, EventArgs^ e) { btnSubmit->BackColor = CLR_ACCENT; }

        void OnBackClick(Object^ s, EventArgs^ e) { this->Close(); }

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

        int CalculateAge(DateTime dob)
        {
            DateTime today = DateTime::Today;
            int age = today.Year - dob.Year;
            if (dob.Date > today.AddYears(-age))
                age--;
            return age;
        }

        String^ GetFieldText(TextBox^ tb)
        {
            String^ placeholder = tb->Tag ? tb->Tag->ToString() : "";
            String^ val = tb->Text->Trim();
            return (val == placeholder) ? "" : val;
        }

        void ShowStatus(String^ msg, bool isError)
        {
            lblStatus->Text = msg;
            lblStatus->ForeColor = isError ? CLR_ERROR : CLR_SUCCESS;
            lblStatus->Visible = true;
        }

        void OnSubmitClick(Object^ s, EventArgs^ e)
        {
            lblStatus->Visible = false;

            String^ fullName = GetFieldText(txtFullName);
            String^ username = GetFieldText(txtUsername);
            String^ contact = GetFieldText(txtContactDetails);
            String^ address = GetFieldText(txtAddress);
            String^ pwdPlaceholder = txtPassword->Tag ? txtPassword->Tag->ToString() : "";
            String^ cfmPlaceholder = txtConfirmPass->Tag ? txtConfirmPass->Tag->ToString() : "";
            String^ password = (txtPassword->Text == pwdPlaceholder) ? "" : txtPassword->Text;
            String^ confirm = (txtConfirmPass->Text == cfmPlaceholder) ? "" : txtConfirmPass->Text;

            String^ bloodGroup = (cmbBloodGroup->SelectedItem != nullptr)
                ? cmbBloodGroup->SelectedItem->ToString()
                : "";
            String^ role = (cmbRole->SelectedItem != nullptr)
                ? cmbRole->SelectedItem->ToString()
                : "Donor";

            int age = CalculateAge(dtpDOB->Value);

            String^ warn = Char::ConvertFromUtf32(0x26A0) + "  ";

            if (String::IsNullOrWhiteSpace(fullName))
            {
                ShowStatus(warn + "Full Name is required.", true);
                txtFullName->Focus();
                return;
            }
            if (fullName->Length < 2 || fullName->Length > 50)
            {
                ShowStatus(warn + "Full Name must be between 2 and 50 characters.", true);
                txtFullName->Focus();
                return;
            }
            {
                Regex^ rxName = gcnew Regex("^[A-Za-z\\s'\\-]{2,50}$");
                if (!rxName->IsMatch(fullName))
                {
                    ShowStatus(warn +
                        "Full Name may only contain letters, spaces, "
                        "apostrophes, and hyphens.", true);
                    txtFullName->Focus();
                    return;
                }
            }

            if (String::IsNullOrWhiteSpace(username))
            {
                ShowStatus(warn + "Username is required.", true);
                txtUsername->Focus();
                return;
            }
            if (username->Length < 3)
            {
                ShowStatus(warn + "Username must be at least 3 characters long.", true);
                txtUsername->Focus();
                return;
            }
            if (username->Length > 30)
            {
                ShowStatus(warn + "Username must not exceed 30 characters.", true);
                txtUsername->Focus();
                return;
            }
            {
                Regex^ rxUsername = gcnew Regex(
                    "^[a-zA-Z0-9][a-zA-Z0-9._\\-]{2,29}$");
                if (!rxUsername->IsMatch(username))
                {
                    ShowStatus(warn +
                        "Username must start with a letter or digit and may "
                        "only contain letters, digits, underscores ( _ ), "
                        "hyphens ( - ), and dots ( . ).", true);
                    txtUsername->Focus();
                    return;
                }
            }

            if (String::IsNullOrWhiteSpace(contact))
            {
                ShowStatus(warn + "Phone Number is required.", true);
                txtContactDetails->Focus();
                return;
            }
            {
                Regex^ rxPhoneFmt = gcnew Regex(
                    "^\\+?[\\d\\s\\-\\(\\)]+$");
                if (!rxPhoneFmt->IsMatch(contact))
                {
                    ShowStatus(warn +
                        "Phone Number may only contain digits, spaces, "
                        "dashes, parentheses, and an optional leading '+'.", true);
                    txtContactDetails->Focus();
                    return;
                }
                Regex^ rxDigits = gcnew Regex("\\d");
                int digitCount = rxDigits->Matches(contact)->Count;
                if (digitCount < 10 || digitCount > 15)
                {
                    ShowStatus(warn +
                        "Phone Number must contain between 10 and 15 digits.", true);
                    txtContactDetails->Focus();
                    return;
                }
            }

            if (dtpDOB->Tag == nullptr || !safe_cast<bool>(dtpDOB->Tag))
            {
                ShowStatus(warn + "Please select a Date of Birth.", true);
                dtpDOB->Focus();
                return;
            }

            if (role == "Donor")
            {
                if (age < 16)
                {
                    ShowStatus(warn +
                        "Donors must be at least 16 years old.", true);
                    return;
                }
                if (age > 65)
                {
                    ShowStatus(warn +
                        "Donors must be 65 years old or younger.", true);
                    return;
                }
            }
            else if (role == "Recipient")
            {
                if (age <= 0)
                {
                    ShowStatus(warn +
                        "Please enter a valid Date of Birth for the Recipient.", true);
                    dtpDOB->Focus();
                    return;
                }
            }

            if (String::IsNullOrEmpty(bloodGroup))
            {
                ShowStatus(warn + "Please select a Blood Group.", true);
                cmbBloodGroup->Focus();
                return;
            }

            if (String::IsNullOrWhiteSpace(password))
            {
                ShowStatus(warn + "Password is required.", true);
                txtPassword->Focus();
                return;
            }
            if (password->Length < 8)
            {
                ShowStatus(warn + "Password must be at least 8 characters long.", true);
                txtPassword->Focus();
                return;
            }
            {
                Regex^ rxUpper = gcnew Regex("[A-Z]");
                Regex^ rxLower = gcnew Regex("[a-z]");
                Regex^ rxDigit = gcnew Regex("[0-9]");
                Regex^ rxSpecial = gcnew Regex(
                    "[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?`~]");

                if (!rxUpper->IsMatch(password))
                {
                    ShowStatus(warn +
                        "Password must contain at least one uppercase letter.", true);
                    txtPassword->Focus();
                    return;
                }
                if (!rxLower->IsMatch(password))
                {
                    ShowStatus(warn +
                        "Password must contain at least one lowercase letter.", true);
                    txtPassword->Focus();
                    return;
                }
                if (!rxDigit->IsMatch(password))
                {
                    ShowStatus(warn +
                        "Password must contain at least one digit (0-9).", true);
                    txtPassword->Focus();
                    return;
                }
                if (!rxSpecial->IsMatch(password))
                {
                    ShowStatus(warn +
                        "Password must contain at least one special character "
                        "(!@#$%^&* etc.).", true);
                    txtPassword->Focus();
                    return;
                }
            }

            if (password != confirm)
            {
                ShowStatus(warn + "Passwords do not match. Please re-enter.", true);
                txtConfirmPass->Clear();
                txtConfirmPass->Focus();
                return;
            }

            if (!String::IsNullOrWhiteSpace(address) && address->Length >= 255)
            {
                ShowStatus(warn +
                    "Address must be fewer than 255 characters.", true);
                txtAddress->Focus();
                return;
            }

            if (!chkTerms->Checked)
            {
                ShowStatus(warn + "Please agree to the terms of service.", true);
                return;
            }

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

            if (rows > 0)
            {
                String^ roleEmoji = (role == "Donor")
                    ? Char::ConvertFromUtf32(0x1F489)
                    : Char::ConvertFromUtf32(0x1F3E5);

                MessageBox::Show(
                    roleEmoji + "  Welcome to LifeBlood, " + fullName + "!\n\n"
                    "Your account has been created successfully.\n"
                    "Role         :  " + role + "\n"
                    "Username  :  " + username + "\n"
                    "Blood Group :  " + bloodGroup + "\n"
                    "Age           :  " + age.ToString() + "\n\n"
                    "You can now log in with your credentials.\n"
                    "Thank you for joining the LifeBlood community "
                    + Char::ConvertFromUtf32(0x2764),
                    "Registration Successful",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information);

                this->Close();
            }
        }

    };

}