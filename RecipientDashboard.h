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

    public ref class RecipientDashboard : public Form
    {
    private:
        Color CLR_BG_FORM;
        Color CLR_BG_CARD;
        Color CLR_ACCENT;
        Color CLR_TEXT;
        Color CLR_MUTED;
        Color CLR_BORDER;
        Color CLR_HOVER_BG;
        Color CLR_GRID_LINE;
        Color CLR_SEL_BG;
        Color CLR_SEL_FG;
        Color CLR_ALT_ROW;
        Color CLR_CARD_ORANGE;
        Color CLR_CARD_GREEN;
        Color CLR_CARD_BLUE;

        int     _recipientID;
        String^ _fullName;
        Button^ _activeNavBtn;

        Panel^ pnlSidebar;
        Panel^ pnlTopBar;
        Panel^ pnlOverview;
        Panel^ pnlRequest;
        Panel^ pnlMyRequests;

        Button^ btnNavHome;
        Button^ btnNavRequest;
        Button^ btnNavMyRequests;
        Button^ btnLogout;

        Button^ btnClose;
        Label^ lblTopBarTitle;

        DataGridView^ dgvMyRequests;

        Label^ lblMetricTotal;
        Label^ lblMetricPending;

        ComboBox^ cmbBloodGroup;
        NumericUpDown^ nudQuantity;

    public:
        RecipientDashboard(int recipientID, String^ fullName)
        {
            _recipientID = recipientID;
            _fullName = fullName;
            _activeNavBtn = nullptr;

            CLR_BG_FORM = ColorTranslator::FromHtml("#F8F9FA");
            CLR_BG_CARD = ColorTranslator::FromHtml("#FFFFFF");
            CLR_ACCENT = ColorTranslator::FromHtml("#C0392B");
            CLR_TEXT = ColorTranslator::FromHtml("#212529");
            CLR_MUTED = ColorTranslator::FromHtml("#6C757D");
            CLR_BORDER = ColorTranslator::FromHtml("#E9ECEF");
            CLR_HOVER_BG = ColorTranslator::FromHtml("#FADBD8");
            CLR_GRID_LINE = ColorTranslator::FromHtml("#DEE2E6");
            CLR_SEL_BG = ColorTranslator::FromHtml("#FADBD8");
            CLR_SEL_FG = ColorTranslator::FromHtml("#C0392B");
            CLR_ALT_ROW = ColorTranslator::FromHtml("#FAFAFA");
            CLR_CARD_ORANGE = ColorTranslator::FromHtml("#E67E22");
            CLR_CARD_GREEN = ColorTranslator::FromHtml("#27AE60");
            CLR_CARD_BLUE = ColorTranslator::FromHtml("#2980B9");

            InitializeComponent();
        }

    protected:
        ~RecipientDashboard() {}

    private:
        void InitializeComponent()
        {
            this->Text = "Recipient Dashboard  \u2014  LifeLine Blood Bank";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;
            this->Load += gcnew EventHandler(this, &RecipientDashboard::OnLoad);

            BuildSidebar();
            BuildRightArea();
            pnlSidebar->SendToBack();
        }

        void BuildSidebar()
        {
            pnlSidebar = gcnew Panel();
            pnlSidebar->Width = 250;
            pnlSidebar->Dock = DockStyle::Left;
            pnlSidebar->BackColor = CLR_BG_CARD;
            pnlSidebar->Paint += gcnew PaintEventHandler(
                this, &RecipientDashboard::OnSidebarPaint);

            BuildSidebarBrand();
            BuildSidebarWelcome();
            BuildSidebarNav();
            BuildSidebarLogout();

            this->Controls->Add(pnlSidebar);
        }

        void BuildSidebarBrand()
        {
            Panel^ pnlBrand = gcnew Panel();
            pnlBrand->Size = Drawing::Size(250, 80);
            pnlBrand->Location = Drawing::Point(0, 0);
            pnlBrand->BackColor = CLR_ACCENT;

            Panel^ cv = gcnew Panel();
            cv->Size = Drawing::Size(6, 22);
            cv->Location = Drawing::Point(24, 28);
            cv->BackColor = Color::White;

            Panel^ ch = gcnew Panel();
            ch->Size = Drawing::Size(22, 6);
            ch->Location = Drawing::Point(16, 36);
            ch->BackColor = Color::White;

            Label^ lblBrand = gcnew Label();
            lblBrand->Text = "LIFELINE";
            lblBrand->Font = gcnew Drawing::Font("Segoe UI", 13, FontStyle::Bold);
            lblBrand->ForeColor = Color::White;
            lblBrand->AutoSize = true;
            lblBrand->Location = Drawing::Point(52, 22);
            lblBrand->BackColor = Color::Transparent;

            Label^ lblSub = gcnew Label();
            lblSub->Text = "BLOOD BANK SYSTEM";
            lblSub->Font = gcnew Drawing::Font("Segoe UI", 6, FontStyle::Bold);
            lblSub->ForeColor = ColorTranslator::FromHtml("#F1948A");
            lblSub->AutoSize = true;
            lblSub->Location = Drawing::Point(52, 48);
            lblSub->BackColor = Color::Transparent;

            pnlBrand->Controls->Add(cv);
            pnlBrand->Controls->Add(ch);
            pnlBrand->Controls->Add(lblBrand);
            pnlBrand->Controls->Add(lblSub);
            pnlSidebar->Controls->Add(pnlBrand);
        }

        void BuildSidebarWelcome()
        {
            Panel^ pnlWelcome = gcnew Panel();
            pnlWelcome->Size = Drawing::Size(250, 90);
            pnlWelcome->Location = Drawing::Point(0, 80);
            pnlWelcome->BackColor = CLR_BG_CARD;
            pnlWelcome->Paint += gcnew PaintEventHandler(
                this, &RecipientDashboard::OnDividerPaint);

            Label^ lblHi = gcnew Label();
            lblHi->Text = "Welcome back,";
            lblHi->Font = gcnew Drawing::Font("Segoe UI", 8);
            lblHi->ForeColor = CLR_MUTED;
            lblHi->AutoSize = true;
            lblHi->Location = Drawing::Point(20, 14);
            lblHi->BackColor = Color::Transparent;

            Label^ lblName = gcnew Label();
            lblName->Text = _fullName;
            lblName->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            lblName->ForeColor = CLR_TEXT;
            lblName->AutoSize = true;
            lblName->MaximumSize = Drawing::Size(210, 0);
            lblName->Location = Drawing::Point(20, 32);
            lblName->BackColor = Color::Transparent;

            Label^ lblRole = gcnew Label();
            lblRole->Text = " " + Char::ConvertFromUtf32(0x25CF) + "  RECIPIENT";
            lblRole->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblRole->ForeColor = CLR_CARD_ORANGE;
            lblRole->AutoSize = true;
            lblRole->Location = Drawing::Point(20, 66);
            lblRole->BackColor = Color::Transparent;

            pnlWelcome->Controls->Add(lblHi);
            pnlWelcome->Controls->Add(lblName);
            pnlWelcome->Controls->Add(lblRole);
            pnlSidebar->Controls->Add(pnlWelcome);
        }

        void BuildSidebarNav()
        {
            Label^ lblSection = gcnew Label();
            lblSection->Text = "NAVIGATION";
            lblSection->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblSection->ForeColor = CLR_MUTED;
            lblSection->AutoSize = true;
            lblSection->Location = Drawing::Point(20, 192);
            lblSection->BackColor = Color::Transparent;
            pnlSidebar->Controls->Add(lblSection);

            btnNavHome = MakeNavButton(214);
            btnNavRequest = MakeNavButton(260);
            btnNavMyRequests = MakeNavButton(306);

            btnNavHome->Text = " " + Char::ConvertFromUtf32(0x1F3E0) + "  Home";
            btnNavRequest->Text = " " + Char::ConvertFromUtf32(0x1F489) + "  Request Blood";
            btnNavMyRequests->Text = " " + Char::ConvertFromUtf32(0x1F4CB) + "  My Requests";

            pnlSidebar->Controls->Add(btnNavHome);
            pnlSidebar->Controls->Add(btnNavRequest);
            pnlSidebar->Controls->Add(btnNavMyRequests);
        }

        Button^ MakeNavButton(int y)
        {
            Button^ btn = gcnew Button();
            btn->TextAlign = ContentAlignment::MiddleLeft;
            btn->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Regular);
            btn->ForeColor = CLR_TEXT;
            btn->BackColor = CLR_BG_CARD;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderSize = 0;
            btn->Size = Drawing::Size(250, 44);
            btn->Location = Drawing::Point(0, y);
            btn->Cursor = Cursors::Hand;
            btn->Padding = System::Windows::Forms::Padding(14, 0, 0, 0);
            btn->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnNavEnter);
            btn->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnNavLeave);
            btn->Click += gcnew EventHandler(this, &RecipientDashboard::OnNavClick);
            return btn;
        }

        void BuildSidebarLogout()
        {
            btnLogout = gcnew Button();
            btnLogout->Text = " " + Char::ConvertFromUtf32(0x1F511) + "  Log Out";
            btnLogout->TextAlign = ContentAlignment::MiddleLeft;
            btnLogout->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Regular);
            btnLogout->ForeColor = CLR_MUTED;
            btnLogout->BackColor = CLR_BG_CARD;
            btnLogout->FlatStyle = FlatStyle::Flat;
            btnLogout->FlatAppearance->BorderSize = 0;
            btnLogout->Height = 52;
            btnLogout->Dock = DockStyle::Bottom;
            btnLogout->Cursor = Cursors::Hand;
            btnLogout->Padding = System::Windows::Forms::Padding(14, 0, 0, 0);
            btnLogout->Paint += gcnew PaintEventHandler(this, &RecipientDashboard::OnLogoutPaint);
            btnLogout->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnLogoutEnter);
            btnLogout->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnLogoutLeave);
            btnLogout->Click += gcnew EventHandler(this, &RecipientDashboard::OnLogoutClick);
            pnlSidebar->Controls->Add(btnLogout);
        }

        void BuildRightArea()
        {
            Panel^ pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::Fill;
            pnlRight->BackColor = CLR_BG_FORM;

            BuildTopBar();
            pnlTopBar->Dock = DockStyle::Top;

            BuildOverviewPanel();
            BuildRequestPanel();
            BuildMyRequestsPanel();

            pnlOverview->Dock = DockStyle::Fill;
            pnlRequest->Dock = DockStyle::Fill;
            pnlMyRequests->Dock = DockStyle::Fill;

            pnlRight->Controls->Add(pnlMyRequests);
            pnlRight->Controls->Add(pnlRequest);
            pnlRight->Controls->Add(pnlOverview);

            pnlRight->Controls->Add(pnlTopBar);

            btnNavHome->Tag = pnlOverview;
            btnNavRequest->Tag = pnlRequest;
            btnNavMyRequests->Tag = pnlMyRequests;

            ShowPanel(pnlOverview);
            SetActiveNav(btnNavHome);

            this->Controls->Add(pnlRight);
        }

        void BuildTopBar()
        {
            pnlTopBar = gcnew Panel();
            pnlTopBar->Height = 56;
            pnlTopBar->BackColor = CLR_BG_CARD;
            pnlTopBar->Paint += gcnew PaintEventHandler(
                this, &RecipientDashboard::OnTopBarPaint);

            btnClose = gcnew Button();
            btnClose->Text = "X";
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnClose->Width = 56;
            btnClose->Dock = DockStyle::Right;
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->BackColor = Color::Transparent;
            btnClose->ForeColor = CLR_MUTED;
            btnClose->Cursor = Cursors::Hand;
            btnClose->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnCloseBtnEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnCloseBtnLeave);
            btnClose->Click += gcnew EventHandler(this, &RecipientDashboard::OnCloseClick);

            lblTopBarTitle = gcnew Label();
            lblTopBarTitle->Text = "Home";
            lblTopBarTitle->Font = gcnew Drawing::Font("Segoe UI Light", 16, FontStyle::Regular);
            lblTopBarTitle->ForeColor = CLR_TEXT;
            lblTopBarTitle->AutoSize = true;
            lblTopBarTitle->Location = Drawing::Point(28, 13);
            lblTopBarTitle->BackColor = Color::Transparent;

            pnlTopBar->Controls->Add(btnClose);
            pnlTopBar->Controls->Add(lblTopBarTitle);
        }

        void BuildOverviewPanel()
        {
            pnlOverview = gcnew Panel();
            pnlOverview->BackColor = CLR_BG_FORM;
            pnlOverview->AutoScroll = true;

            Label^ lblHello = gcnew Label();
            lblHello->Text = "Hello, " + _fullName + "!";
            lblHello->Font = gcnew Drawing::Font("Segoe UI Light", 22, FontStyle::Regular);
            lblHello->ForeColor = CLR_TEXT;
            lblHello->AutoSize = true;
            lblHello->Location = Drawing::Point(32, 32);
            lblHello->BackColor = Color::Transparent;

            Label^ lblSub = gcnew Label();
            lblSub->Text = Char::ConvertFromUtf32(0x1F497)
                + "  We're here to help. Request blood quickly and safely.";
            lblSub->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblSub->ForeColor = CLR_MUTED;
            lblSub->AutoSize = true;
            lblSub->Location = Drawing::Point(34, 76);
            lblSub->BackColor = Color::Transparent;

            Panel^ pnlDivider = gcnew Panel();
            pnlDivider->Size = Drawing::Size(500, 1);
            pnlDivider->Location = Drawing::Point(32, 116);
            pnlDivider->BackColor = CLR_BORDER;

            Label^ lblMetricSection = gcnew Label();
            lblMetricSection->Text = "YOUR REQUEST SUMMARY";
            lblMetricSection->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblMetricSection->ForeColor = CLR_MUTED;
            lblMetricSection->AutoSize = true;
            lblMetricSection->Location = Drawing::Point(32, 134);
            lblMetricSection->BackColor = Color::Transparent;

            Panel^ cTotal = MakeMetricCard("TOTAL REQUESTS", CLR_ACCENT, 32, 160);
            Panel^ cPending = MakeMetricCard("PENDING REQUESTS", CLR_CARD_ORANGE, 272, 160);

            lblMetricTotal = GetMetricValueLabel(cTotal);
            lblMetricPending = GetMetricValueLabel(cPending);

            Panel^ pnlTip = gcnew Panel();
            pnlTip->Size = Drawing::Size(462, 72);
            pnlTip->Location = Drawing::Point(32, 340);
            pnlTip->BackColor = ColorTranslator::FromHtml("#FEF9E7");
            pnlTip->Paint += gcnew PaintEventHandler(this, &RecipientDashboard::OnCardPaint);

            Panel^ tipBar = gcnew Panel();
            tipBar->Size = Drawing::Size(4, 72);
            tipBar->Location = Drawing::Point(0, 0);
            tipBar->BackColor = CLR_CARD_ORANGE;

            Label^ lblTipHead = gcnew Label();
            lblTipHead->Text = Char::ConvertFromUtf32(0x2139) + "  How it works";
            lblTipHead->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lblTipHead->ForeColor = CLR_CARD_ORANGE;
            lblTipHead->AutoSize = true;
            lblTipHead->Location = Drawing::Point(20, 12);
            lblTipHead->BackColor = Color::Transparent;

            Label^ lblTipBody = gcnew Label();
            lblTipBody->Text = "Submit a request and an admin will review it. "
                "Check 'My Requests' to track its status.";
            lblTipBody->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Italic);
            lblTipBody->ForeColor = CLR_TEXT;
            lblTipBody->AutoSize = true;
            lblTipBody->Location = Drawing::Point(20, 36);
            lblTipBody->BackColor = Color::Transparent;

            pnlTip->Controls->Add(tipBar);
            pnlTip->Controls->Add(lblTipHead);
            pnlTip->Controls->Add(lblTipBody);

            pnlOverview->Controls->Add(lblHello);
            pnlOverview->Controls->Add(lblSub);
            pnlOverview->Controls->Add(pnlDivider);
            pnlOverview->Controls->Add(lblMetricSection);
            pnlOverview->Controls->Add(cTotal);
            pnlOverview->Controls->Add(cPending);
            pnlOverview->Controls->Add(pnlTip);
        }

        void BuildRequestPanel()
        {
            pnlRequest = gcnew Panel();
            pnlRequest->BackColor = CLR_BG_FORM;
            pnlRequest->AutoScroll = true;

            Label^ lblTitle = gcnew Label();
            lblTitle->Text = "Request Blood";
            lblTitle->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            lblTitle->ForeColor = CLR_TEXT;
            lblTitle->AutoSize = true;
            lblTitle->Location = Drawing::Point(32, 32);
            lblTitle->BackColor = Color::Transparent;

            Label^ lblSub = gcnew Label();
            lblSub->Text = "Select a blood group and quantity, then click Submit Request.";
            lblSub->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblSub->ForeColor = CLR_MUTED;
            lblSub->AutoSize = true;
            lblSub->Location = Drawing::Point(32, 66);
            lblSub->BackColor = Color::Transparent;

            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(480, 320);
            card->Location = Drawing::Point(32, 108);
            card->BackColor = CLR_BG_CARD;
            card->Paint += gcnew PaintEventHandler(this, &RecipientDashboard::OnCardPaint);

            Panel^ bar = gcnew Panel();
            bar->Size = Drawing::Size(4, 320);
            bar->Location = Drawing::Point(0, 0);
            bar->BackColor = CLR_CARD_ORANGE;

            Label^ lblBG = gcnew Label();
            lblBG->Text = "Blood Group";
            lblBG->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lblBG->ForeColor = CLR_MUTED;
            lblBG->AutoSize = true;
            lblBG->Location = Drawing::Point(28, 36);
            lblBG->BackColor = Color::Transparent;

            cmbBloodGroup = gcnew ComboBox();
            cmbBloodGroup->Font = gcnew Drawing::Font("Segoe UI", 11);
            cmbBloodGroup->ForeColor = CLR_TEXT;
            cmbBloodGroup->Size = Drawing::Size(210, 32);
            cmbBloodGroup->Location = Drawing::Point(28, 62);
            cmbBloodGroup->DropDownStyle = ComboBoxStyle::DropDownList;
            cmbBloodGroup->FlatStyle = FlatStyle::Flat;
            cmbBloodGroup->Items->Add("A+");
            cmbBloodGroup->Items->Add("A-");
            cmbBloodGroup->Items->Add("B+");
            cmbBloodGroup->Items->Add("B-");
            cmbBloodGroup->Items->Add("O+");
            cmbBloodGroup->Items->Add("O-");
            cmbBloodGroup->Items->Add("AB+");
            cmbBloodGroup->Items->Add("AB-");
            cmbBloodGroup->SelectedIndex = 0;

            Label^ lblQty = gcnew Label();
            lblQty->Text = "Quantity  (Units)";
            lblQty->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lblQty->ForeColor = CLR_MUTED;
            lblQty->AutoSize = true;
            lblQty->Location = Drawing::Point(28, 118);
            lblQty->BackColor = Color::Transparent;

            nudQuantity = gcnew NumericUpDown();
            nudQuantity->Font = gcnew Drawing::Font("Segoe UI", 11);
            nudQuantity->ForeColor = CLR_TEXT;
            nudQuantity->Size = Drawing::Size(120, 32);
            nudQuantity->Location = Drawing::Point(28, 144);
            nudQuantity->Minimum = 1;
            nudQuantity->Maximum = 10;
            nudQuantity->Value = 1;
            nudQuantity->BorderStyle = BorderStyle::FixedSingle;

            Label^ lblInfo = gcnew Label();
            lblInfo->Text = Char::ConvertFromUtf32(0x2139)
                + "  Your request will be set to 'Pending' "
                "until reviewed and approved by an admin.";
            lblInfo->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Italic);
            lblInfo->ForeColor = CLR_MUTED;
            lblInfo->Size = Drawing::Size(420, 36);
            lblInfo->Location = Drawing::Point(28, 200);
            lblInfo->BackColor = Color::Transparent;

            Panel^ cardDiv = gcnew Panel();
            cardDiv->Size = Drawing::Size(420, 1);
            cardDiv->Location = Drawing::Point(28, 248);
            cardDiv->BackColor = CLR_BORDER;

            Button^ btnSubmit = gcnew Button();
            btnSubmit->Text = Char::ConvertFromUtf32(0x1FA78) + "  Submit Request";
            btnSubmit->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnSubmit->Size = Drawing::Size(210, 52);
            btnSubmit->Location = Drawing::Point(28, 258);
            btnSubmit->BackColor = CLR_ACCENT;
            btnSubmit->ForeColor = Color::White;
            btnSubmit->FlatStyle = FlatStyle::Flat;
            btnSubmit->FlatAppearance->BorderSize = 0;
            btnSubmit->Cursor = Cursors::Hand;
            btnSubmit->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnSubmitEnter);
            btnSubmit->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnSubmitLeave);
            btnSubmit->Click += gcnew EventHandler(this, &RecipientDashboard::OnSubmitRequest);

            card->Controls->Add(bar);
            card->Controls->Add(lblBG);
            card->Controls->Add(cmbBloodGroup);
            card->Controls->Add(lblQty);
            card->Controls->Add(nudQuantity);
            card->Controls->Add(lblInfo);
            card->Controls->Add(cardDiv);
            card->Controls->Add(btnSubmit);

            pnlRequest->Controls->Add(lblTitle);
            pnlRequest->Controls->Add(lblSub);
            pnlRequest->Controls->Add(card);
        }

        void BuildMyRequestsPanel()
        {
            pnlMyRequests = gcnew Panel();
            pnlMyRequests->BackColor = CLR_BG_FORM;
            pnlMyRequests->AutoScroll = false;

            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            Button^ btnRef = MakeOutlineButton(
                " " + Char::ConvertFromUtf32(0x1F504) + "  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &RecipientDashboard::OnRefreshRequests);
            pnlTop->Controls->Add(btnRef);

            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvMyRequests = MakeStyledGrid();
            dgvMyRequests->Dock = DockStyle::Fill;
            pnlGridBox->Controls->Add(dgvMyRequests);

            pnlMyRequests->Controls->Add(pnlGridBox);
            pnlMyRequests->Controls->Add(pnlTop);
        }

        Panel^ MakeMetricCard(String^ title, Color accent, int x, int y)
        {
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(222, 152);
            card->Location = Drawing::Point(x, y);
            card->BackColor = CLR_BG_CARD;
            card->Paint += gcnew PaintEventHandler(this, &RecipientDashboard::OnCardPaint);

            Panel^ bar = gcnew Panel();
            bar->Size = Drawing::Size(4, 152);
            bar->Location = Drawing::Point(0, 0);
            bar->BackColor = accent;

            Label^ lblTitle = gcnew Label();
            lblTitle->Text = title;
            lblTitle->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblTitle->ForeColor = CLR_MUTED;
            lblTitle->AutoSize = true;
            lblTitle->Location = Drawing::Point(20, 20);
            lblTitle->BackColor = Color::Transparent;

            Label^ lblVal = gcnew Label();
            lblVal->Text = "-";
            lblVal->Font = gcnew Drawing::Font("Segoe UI Light", 40, FontStyle::Regular);
            lblVal->ForeColor = accent;
            lblVal->AutoSize = true;
            lblVal->Location = Drawing::Point(18, 48);
            lblVal->BackColor = Color::Transparent;
            lblVal->Tag = "metric_value";

            card->Controls->Add(bar);
            card->Controls->Add(lblTitle);
            card->Controls->Add(lblVal);
            return card;
        }

        Label^ GetMetricValueLabel(Panel^ card)
        {
            for each (Control ^ c in card->Controls)
            {
                Label^ lbl = dynamic_cast<Label^>(c);
                if (lbl != nullptr &&
                    lbl->Tag != nullptr &&
                    lbl->Tag->ToString() == "metric_value")
                    return lbl;
            }
            return nullptr;
        }

        DataGridView^ MakeStyledGrid()
        {
            DataGridView^ dgv = gcnew DataGridView();
            dgv->ReadOnly = true;
            dgv->AllowUserToAddRows = false;
            dgv->AllowUserToDeleteRows = false;
            dgv->AllowUserToResizeRows = false;
            dgv->MultiSelect = false;
            dgv->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            dgv->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            dgv->RowHeadersVisible = false;

            dgv->EnableHeadersVisualStyles = false;
            dgv->BackgroundColor = Color::White;
            dgv->BorderStyle = BorderStyle::None;
            dgv->CellBorderStyle = DataGridViewCellBorderStyle::SingleHorizontal;
            dgv->GridColor = CLR_GRID_LINE;

            DataGridViewCellStyle^ hStyle = gcnew DataGridViewCellStyle();
            hStyle->BackColor = CLR_ACCENT;
            hStyle->ForeColor = Color::White;
            hStyle->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            hStyle->Padding = System::Windows::Forms::Padding(10, 0, 0, 0);
            hStyle->SelectionBackColor = CLR_ACCENT;
            hStyle->SelectionForeColor = Color::White;
            dgv->ColumnHeadersDefaultCellStyle = hStyle;
            dgv->ColumnHeadersHeight = 42;
            dgv->ColumnHeadersHeightSizeMode =
                DataGridViewColumnHeadersHeightSizeMode::DisableResizing;

            DataGridViewCellStyle^ cStyle = gcnew DataGridViewCellStyle();
            cStyle->BackColor = Color::White;
            cStyle->ForeColor = CLR_TEXT;
            cStyle->SelectionBackColor = CLR_SEL_BG;
            cStyle->SelectionForeColor = CLR_SEL_FG;
            cStyle->Font = gcnew Drawing::Font("Segoe UI", 9);
            cStyle->Padding = System::Windows::Forms::Padding(10, 4, 0, 4);
            dgv->DefaultCellStyle = cStyle;

            DataGridViewCellStyle^ aStyle = gcnew DataGridViewCellStyle();
            aStyle->BackColor = CLR_ALT_ROW;
            aStyle->ForeColor = CLR_TEXT;
            aStyle->SelectionBackColor = CLR_SEL_BG;
            aStyle->SelectionForeColor = CLR_SEL_FG;
            dgv->AlternatingRowsDefaultCellStyle = aStyle;

            dgv->RowTemplate->Height = 38;
            return dgv;
        }

        Button^ MakeOutlineButton(String^ text, int x, int y)
        {
            Button^ btn = gcnew Button();
            btn->Text = text;
            btn->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            btn->Size = Drawing::Size(148, 36);
            btn->Location = Drawing::Point(x, y);
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderColor = CLR_ACCENT;
            btn->FlatAppearance->BorderSize = 1;
            btn->BackColor = CLR_BG_CARD;
            btn->ForeColor = CLR_ACCENT;
            btn->Cursor = Cursors::Hand;
            btn->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnOutlineBtnEnter);
            btn->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnOutlineBtnLeave);
            return btn;
        }

        void LoadAllData()
        {
            LoadOverviewMetrics();
            LoadMyRequests();
        }

        void LoadOverviewMetrics()
        {
            Database^ db = Database::GetInstance();

            SqlCommand^ cmdTotal = gcnew SqlCommand(
                "SELECT COUNT(*) FROM BloodRequests WHERE RecipientID = @id");
            cmdTotal->Parameters->AddWithValue("@id", _recipientID);
            Object^ tObj = db->ExecuteScalar(cmdTotal);
            if (lblMetricTotal != nullptr && tObj != nullptr)
                lblMetricTotal->Text = Convert::ToInt32(tObj).ToString();

            SqlCommand^ cmdPending = gcnew SqlCommand(
                "SELECT COUNT(*) FROM BloodRequests "
                "WHERE  RecipientID = @id AND RequestStatus = 'Pending'");
            cmdPending->Parameters->AddWithValue("@id", _recipientID);
            Object^ pObj = db->ExecuteScalar(cmdPending);
            if (lblMetricPending != nullptr && pObj != nullptr)
                lblMetricPending->Text = Convert::ToInt32(pObj).ToString();
        }

        void LoadMyRequests()
        {
            if (dgvMyRequests == nullptr) return;

            SqlCommand^ cmd = gcnew SqlCommand(
                "SELECT RequestID          AS [Request ID], "
                "       BloodGroup         AS [Blood Group], "
                "       QuantityRequested  AS [Units], "
                "       RequestStatus      AS [Status], "
                "       RequestDate        AS [Request Date] "
                "FROM   BloodRequests "
                "WHERE  RecipientID = @id "
                "ORDER BY RequestDate DESC, RequestID DESC");
            cmd->Parameters->AddWithValue("@id", _recipientID);

            DataTable^ dt = Database::GetInstance()->FillDataTable(cmd);
            if (dt != nullptr)
                dgvMyRequests->DataSource = dt;
        }

        void ShowPanel(Panel^ target)
        {
            pnlOverview->Visible = (target == pnlOverview);
            pnlRequest->Visible = (target == pnlRequest);
            pnlMyRequests->Visible = (target == pnlMyRequests);
        }

        void SetActiveNav(Button^ btn)
        {
            if (_activeNavBtn != nullptr)
            {
                _activeNavBtn->BackColor = CLR_BG_CARD;
                _activeNavBtn->ForeColor = CLR_TEXT;
                _activeNavBtn->Font =
                    gcnew Drawing::Font("Segoe UI", 10, FontStyle::Regular);
            }
            _activeNavBtn = btn;
            btn->BackColor = CLR_HOVER_BG;
            btn->ForeColor = CLR_ACCENT;
            btn->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
        }

        void OnLoad(Object^ sender, EventArgs^ e)
        {
            LoadAllData();
        }

        void OnNavEnter(Object^ s, EventArgs^ e)
        {
            Button^ btn = safe_cast<Button^>(s);
            if (btn != _activeNavBtn)
            {
                btn->BackColor = CLR_HOVER_BG;
                btn->ForeColor = CLR_ACCENT;
            }
        }
        void OnNavLeave(Object^ s, EventArgs^ e)
        {
            Button^ btn = safe_cast<Button^>(s);
            if (btn != _activeNavBtn)
            {
                btn->BackColor = CLR_BG_CARD;
                btn->ForeColor = CLR_TEXT;
            }
        }

        void OnNavClick(Object^ s, EventArgs^ e)
        {
            Button^ btn = safe_cast<Button^>(s);
            Panel^ target = dynamic_cast<Panel^>(btn->Tag);
            if (target == nullptr) return;

            SetActiveNav(btn);
            ShowPanel(target);

            if (target == pnlOverview)
            {
                lblTopBarTitle->Text = "Home";
                LoadOverviewMetrics();
            }
            else if (target == pnlRequest)
            {
                lblTopBarTitle->Text = "Request Blood";
            }
            else if (target == pnlMyRequests)
            {
                lblTopBarTitle->Text = "My Requests";
                LoadMyRequests();
            }
        }

        void OnOutlineBtnEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_HOVER_BG;
        }
        void OnOutlineBtnLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_BG_CARD;
        }

        void OnRefreshRequests(Object^ s, EventArgs^ e)
        {
            LoadMyRequests();
        }

        void OnSubmitRequest(Object^ s, EventArgs^ e)
        {
            if (cmbBloodGroup->SelectedItem == nullptr)
            {
                MessageBox::Show(
                    "Please select a blood group.",
                    "Validation",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Warning);
                return;
            }

            String^ bg = cmbBloodGroup->SelectedItem->ToString();
            int     qty = Convert::ToInt32(nudQuantity->Value);

            SqlCommand^ cmd = gcnew SqlCommand(
                "INSERT INTO BloodRequests "
                "       (RecipientID, BloodGroup, QuantityRequested, RequestDate, RequestStatus) "
                "VALUES (@id, @bg, @qty, CAST(GETDATE() AS DATE), 'Pending')");
            cmd->Parameters->AddWithValue("@id", _recipientID);
            cmd->Parameters->AddWithValue("@bg", bg);
            cmd->Parameters->AddWithValue("@qty", qty);

            int rows = Database::GetInstance()->ExecuteNonQuery(cmd);

            if (rows > 0)
            {
                MessageBox::Show(
                    "Blood request submitted successfully!\n\n"
                    "Blood Group  :  " + bg + "\n"
                    "Units        :  " + qty.ToString() + "\n"
                    "Status       :  Pending\n\n"
                    "An admin will review your request shortly. "
                    + Char::ConvertFromUtf32(0x1F497),
                    "Request Submitted",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information);

                cmbBloodGroup->SelectedIndex = 0;
                nudQuantity->Value = 1;

                LoadOverviewMetrics();
            }
        }

        void OnSubmitEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor =
                ColorTranslator::FromHtml("#A93226");
        }
        void OnSubmitLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_ACCENT;
        }

        void OnLogoutEnter(Object^ s, EventArgs^ e)
        {
            btnLogout->BackColor = CLR_HOVER_BG;
            btnLogout->ForeColor = CLR_ACCENT;
        }
        void OnLogoutLeave(Object^ s, EventArgs^ e)
        {
            btnLogout->BackColor = CLR_BG_CARD;
            btnLogout->ForeColor = CLR_MUTED;
        }
        void OnLogoutClick(Object^ s, EventArgs^ e)
        {
            this->Close();
        }

        void OnCloseBtnEnter(Object^ s, EventArgs^ e)
        {
            btnClose->ForeColor = CLR_ACCENT;
            btnClose->BackColor = CLR_HOVER_BG;
        }
        void OnCloseBtnLeave(Object^ s, EventArgs^ e)
        {
            btnClose->ForeColor = CLR_MUTED;
            btnClose->BackColor = Color::Transparent;
        }
        void OnCloseClick(Object^ s, EventArgs^ e)
        {
            Application::Exit();
        }

        void OnSidebarPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, p->Width - 1, 0, p->Width - 1, p->Height);
            delete pen;
        }

        void OnDividerPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, 16, p->Height - 1, p->Width - 16, p->Height - 1);
            delete pen;
        }

        void OnTopBarPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, 0, p->Height - 1, p->Width, p->Height - 1);
            delete pen;
        }

        void OnCardPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }

        void OnLogoutPaint(Object^ s, PaintEventArgs^ e)
        {
            Button^ b = safe_cast<Button^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, 0, 0, b->Width, 0);
            delete pen;
        }
    };
}