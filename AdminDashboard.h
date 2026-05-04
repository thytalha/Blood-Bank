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

    public ref class AdminDashboard : public Form
    {
        //  COLOUR PALETTE  — single source of truth

    private:
        Color CLR_BG_FORM;       // #F8F9FA  soft off-white  — main background
        Color CLR_BG_CARD;       // #FFFFFF  pure white      — sidebar & panels
        Color CLR_ACCENT;        // #C0392B  striking crimson
        Color CLR_TEXT;          // #212529  near-black      — body text
        Color CLR_MUTED;         // #6C757D  medium gray     — subtitles / icons
        Color CLR_BORDER;        // #E9ECEF  pale gray       — panel separators
        Color CLR_HOVER_BG;      // #FADBD8  blush pink      — nav hover fill
        Color CLR_GRID_LINE;     // #DEE2E6  DataGridView row lines
        Color CLR_SEL_BG;        // #FADBD8  grid selection bg
        Color CLR_SEL_FG;        // #C0392B  grid selection text
        Color CLR_ALT_ROW;       // #FAFAFA  alternating row tint
        Color CLR_CARD_ORANGE;   // #E67E22  expired-units accent
        Color CLR_CARD_GREEN;    // #27AE60  users count accent
        Color CLR_CARD_BLUE;     // #2980B9  donations accent


        //  MEMBER FIELDS

        int     _adminID;
        String^ _fullName;

        // Navigation state
        Button^ _activeNavBtn;

        // Layout panels
        Panel^ pnlSidebar;     // 250 px,  DockStyle::Left
        Panel^ pnlTopBar;      // 56 px,   DockStyle::Top  (inside pnlRight)
        Panel^ pnlOverview;    // content view 1  — DockStyle::Fill
        Panel^ pnlInventory;   // content view 2  — DockStyle::Fill
        Panel^ pnlRequests;    // content view 3  — DockStyle::Fill

        // Sidebar controls
        Button^ btnNavOverview;
        Button^ btnNavInventory;
        Button^ btnNavRequests;
        Button^ btnLogout;      // DockStyle::Bottom

        // Top-bar controls
        Button^ btnClose;       // DockStyle::Right  →  Application::Exit()
        Label^ lblTopBarTitle;

        // DataGridViews
        DataGridView^ dgvQuickOverview;
        DataGridView^ dgvInventory;
        DataGridView^ dgvRequests;

        // Metric card value labels
        Label^ lblMetricPending;
        Label^ lblMetricExpired;
        Label^ lblMetricUsers;
        Label^ lblMetricDonations;


        // CONSTRUCTOR


    public:
        AdminDashboard(int adminID, String^ fullName)
        {
            _adminID = adminID;
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
        ~AdminDashboard() {}


        // InitializeComponent


    private:
        void InitializeComponent()
        {
            this->Text = "Admin Dashboard   -   LifeLine Blood Bank";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;
            this->Load += gcnew EventHandler(this, &AdminDashboard::OnLoad);

            
            BuildSidebar();             // step 2: adds pnlSidebar (Left)
            BuildRightArea();           // step 1: adds pnlRight  (Fill)
            pnlSidebar->SendToBack();
        }


        // SIDEBAR  (DockStyle::Left, 250 px, pure white)


        void BuildSidebar()
        {
            pnlSidebar = gcnew Panel();
            pnlSidebar->Width = 250;
            pnlSidebar->Dock = DockStyle::Left;
            pnlSidebar->BackColor = CLR_BG_CARD;
            pnlSidebar->Paint += gcnew PaintEventHandler(
                this, &AdminDashboard::OnSidebarPaint);

            BuildSidebarBrand();
            BuildSidebarWelcome();
            BuildSidebarNav();
            BuildSidebarLogout();

            this->Controls->Add(pnlSidebar);
        }

        // Crimson brand strip (80 px tall)
        void BuildSidebarBrand()
        {
            Panel^ pnlBrand = gcnew Panel();
            pnlBrand->Size = Drawing::Size(250, 80);
            pnlBrand->Location = Drawing::Point(0, 0);
            pnlBrand->BackColor = CLR_ACCENT;

            Panel^ cv = gcnew Panel();   // cross — vertical bar
            cv->Size = Drawing::Size(6, 22);
            cv->Location = Drawing::Point(24, 28);
            cv->BackColor = Color::White;

            Panel^ ch = gcnew Panel();   // cross — horizontal bar
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

        // Welcome block (name + role tag)
        void BuildSidebarWelcome()
        {
            Panel^ pnlWelcome = gcnew Panel();
            pnlWelcome->Size = Drawing::Size(250, 90);
            pnlWelcome->Location = Drawing::Point(0, 80);
            pnlWelcome->BackColor = CLR_BG_CARD;
            pnlWelcome->Paint += gcnew PaintEventHandler(
                this, &AdminDashboard::OnDividerPaint);

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
            lblRole->Text = " " + Char::ConvertFromUtf32(0x25CF) + "  ADMINISTRATOR";
            lblRole->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblRole->ForeColor = CLR_ACCENT;
            lblRole->AutoSize = true;
            lblRole->Location = Drawing::Point(20, 66);
            lblRole->BackColor = Color::Transparent;

            pnlWelcome->Controls->Add(lblHi);
            pnlWelcome->Controls->Add(lblName);
            pnlWelcome->Controls->Add(lblRole);
            pnlSidebar->Controls->Add(pnlWelcome);
        }

        // Navigation buttons
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

            // Allocate buttons first so text can be set below
            btnNavOverview = MakeNavButton(214);
            btnNavInventory = MakeNavButton(260);
            btnNavRequests = MakeNavButton(306);


            btnNavOverview->Text = " " + Char::ConvertFromUtf32(0x1F4CA) + "  Overview";
            btnNavInventory->Text = " " + Char::ConvertFromUtf32(0x1F9EA) + "  Inventory";
            btnNavRequests->Text = " " + Char::ConvertFromUtf32(0x1F4CB) + "  Requests";

            pnlSidebar->Controls->Add(btnNavOverview);
            pnlSidebar->Controls->Add(btnNavInventory);
            pnlSidebar->Controls->Add(btnNavRequests);
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
            btn->MouseEnter += gcnew EventHandler(this, &AdminDashboard::OnNavEnter);
            btn->MouseLeave += gcnew EventHandler(this, &AdminDashboard::OnNavLeave);
            btn->Click += gcnew EventHandler(this, &AdminDashboard::OnNavClick);
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
            btnLogout->Dock = DockStyle::Bottom;   // always bottom
            btnLogout->Cursor = Cursors::Hand;
            btnLogout->Padding = System::Windows::Forms::Padding(14, 0, 0, 0);
            btnLogout->Paint += gcnew PaintEventHandler(
                this, &AdminDashboard::OnLogoutPaint);
            btnLogout->MouseEnter += gcnew EventHandler(this, &AdminDashboard::OnLogoutEnter);
            btnLogout->MouseLeave += gcnew EventHandler(this, &AdminDashboard::OnLogoutLeave);
            btnLogout->Click += gcnew EventHandler(this, &AdminDashboard::OnLogoutClick);
            pnlSidebar->Controls->Add(btnLogout);
        }


        //  RIGHT AREA  (top bar + three swappable content panels)

        void BuildRightArea()
        {
            Panel^ pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::Fill;
            pnlRight->BackColor = CLR_BG_FORM;

            BuildTopBar();
            pnlTopBar->Dock = DockStyle::Top;

            BuildOverviewPanel();
            BuildInventoryPanel();
            BuildRequestsPanel();

            pnlOverview->Dock = DockStyle::Fill;
            pnlInventory->Dock = DockStyle::Fill;
            pnlRequests->Dock = DockStyle::Fill;

            pnlRight->Controls->Add(pnlRequests);
            pnlRight->Controls->Add(pnlInventory);
            pnlRight->Controls->Add(pnlOverview);

            pnlRight->Controls->Add(pnlTopBar);

            btnNavOverview->Tag = pnlOverview;
            btnNavInventory->Tag = pnlInventory;
            btnNavRequests->Tag = pnlRequests;

            ShowPanel(pnlOverview);
            SetActiveNav(btnNavOverview);

            this->Controls->Add(pnlRight);
        }

        void BuildTopBar()
        {
            pnlTopBar = gcnew Panel();
            pnlTopBar->Height = 56;
            pnlTopBar->BackColor = CLR_BG_CARD;
            pnlTopBar->Paint += gcnew PaintEventHandler(
                this, &AdminDashboard::OnTopBarPaint);

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
            btnClose->MouseEnter += gcnew EventHandler(this, &AdminDashboard::OnCloseBtnEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &AdminDashboard::OnCloseBtnLeave);
            btnClose->Click += gcnew EventHandler(this, &AdminDashboard::OnCloseClick);

            lblTopBarTitle = gcnew Label();
            lblTopBarTitle->Text = "Overview";
            lblTopBarTitle->Font = gcnew Drawing::Font("Segoe UI Light", 16, FontStyle::Regular);
            lblTopBarTitle->ForeColor = CLR_TEXT;
            lblTopBarTitle->AutoSize = true;
            lblTopBarTitle->Location = Drawing::Point(28, 13);
            lblTopBarTitle->BackColor = Color::Transparent;

            pnlTopBar->Controls->Add(btnClose);
            pnlTopBar->Controls->Add(lblTopBarTitle);
        }


        //  OVERVIEW PANEL

        void BuildOverviewPanel()
        {
            pnlOverview = gcnew Panel();
            pnlOverview->BackColor = CLR_BG_FORM;
            pnlOverview->AutoScroll = true;

            Panel^ cPending = MakeMetricCard("PENDING REQUESTS", CLR_ACCENT, 32, 32);
            Panel^ cExpired = MakeMetricCard("EXPIRED UNITS", CLR_CARD_ORANGE, 272, 32);
            Panel^ cUsers = MakeMetricCard("TOTAL USERS", CLR_CARD_GREEN, 512, 32);
            Panel^ cDonations = MakeMetricCard("TOTAL DONATIONS", CLR_CARD_BLUE, 752, 32);

            lblMetricPending = GetMetricValueLabel(cPending);
            lblMetricExpired = GetMetricValueLabel(cExpired);
            lblMetricUsers = GetMetricValueLabel(cUsers);
            lblMetricDonations = GetMetricValueLabel(cDonations);

            Label^ lblQT = gcnew Label();
            lblQT->Text = "Blood Inventory   -   Quick View";
            lblQT->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            lblQT->ForeColor = CLR_TEXT;
            lblQT->AutoSize = true;
            lblQT->Location = Drawing::Point(32, 212);
            lblQT->BackColor = Color::Transparent;

            dgvQuickOverview = MakeStyledGrid();
            dgvQuickOverview->Location = Drawing::Point(32, 248);
            dgvQuickOverview->Size = Drawing::Size(600, 300);
            dgvQuickOverview->Anchor = AnchorStyles::Top | AnchorStyles::Left;

            pnlOverview->Controls->Add(cPending);
            pnlOverview->Controls->Add(cExpired);
            pnlOverview->Controls->Add(cUsers);
            pnlOverview->Controls->Add(cDonations);
            pnlOverview->Controls->Add(lblQT);
            pnlOverview->Controls->Add(dgvQuickOverview);
        }

        Panel^ MakeMetricCard(String^ title, Color accent, int x, int y)
        {
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(222, 152);
            card->Location = Drawing::Point(x, y);
            card->BackColor = CLR_BG_CARD;
            card->Paint += gcnew PaintEventHandler(
                this, &AdminDashboard::OnCardPaint);

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


        //  INVENTORY PANEL

        void BuildInventoryPanel()
        {
            pnlInventory = gcnew Panel();
            pnlInventory->BackColor = CLR_BG_FORM;
            pnlInventory->AutoScroll = false;

            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            Button^ btnRef = MakeOutlineButton("  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &AdminDashboard::OnRefreshInventory);
            pnlTop->Controls->Add(btnRef);

            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvInventory = MakeStyledGrid();
            dgvInventory->Dock = DockStyle::Fill;

            pnlGridBox->Controls->Add(dgvInventory);

            pnlInventory->Controls->Add(pnlGridBox);
            pnlInventory->Controls->Add(pnlTop);
        }


        //  REQUESTS PANEL

        void BuildRequestsPanel()
        {
            pnlRequests = gcnew Panel();
            pnlRequests->BackColor = CLR_BG_FORM;
            pnlRequests->AutoScroll = false;

            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            Button^ btnRef = MakeOutlineButton("  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &AdminDashboard::OnRefreshRequests);
            pnlTop->Controls->Add(btnRef);

            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvRequests = MakeStyledGrid();
            dgvRequests->Dock = DockStyle::Fill;
            dgvRequests->CellContentClick += gcnew DataGridViewCellEventHandler(this, &AdminDashboard::OnRequestActionClick);
            dgvRequests->CellFormatting += gcnew DataGridViewCellFormattingEventHandler(this, &AdminDashboard::OnRequestCellFormatting);


            pnlGridBox->Controls->Add(dgvRequests);

            pnlRequests->Controls->Add(pnlGridBox);
            pnlRequests->Controls->Add(pnlTop);
        }


        //  DATAGRIDVIEW FACTORY  — fully custom styled

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
            btn->Size = Drawing::Size(120, 36);
            btn->Location = Drawing::Point(x, y);
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderColor = CLR_ACCENT;
            btn->FlatAppearance->BorderSize = 1;
            btn->BackColor = CLR_BG_CARD;
            btn->ForeColor = CLR_ACCENT;
            btn->Cursor = Cursors::Hand;
            btn->MouseEnter += gcnew EventHandler(this, &AdminDashboard::OnOutlineBtnEnter);
            btn->MouseLeave += gcnew EventHandler(this, &AdminDashboard::OnOutlineBtnLeave);
            return btn;
        }


        //  DATA LOADING

        void LoadAllData()
        {
            LoadOverviewMetrics();
            LoadInventory();
            LoadRequests();
        }

        void LoadOverviewMetrics()
        {
            Database^ db = Database::GetInstance();

            if (lblMetricPending != nullptr)
                lblMetricPending->Text = db->CountPendingRequests().ToString();

            if (lblMetricExpired != nullptr)
                lblMetricExpired->Text = db->CountExpiredUnits().ToString();

            Object^ uObj = db->ExecuteScalar("SELECT COUNT(*) FROM Users");
            if (lblMetricUsers != nullptr && uObj != nullptr)
                lblMetricUsers->Text = Convert::ToInt32(uObj).ToString();

            Object^ dObj = db->ExecuteScalar("SELECT COUNT(*) FROM Donations");
            if (lblMetricDonations != nullptr && dObj != nullptr)
                lblMetricDonations->Text = Convert::ToInt32(dObj).ToString();

            if (dgvQuickOverview != nullptr)
            {
                DataTable^ dt = db->FillDataTable(
                    "SELECT bg.GroupName AS [Blood Group], "
                    "       ISNULL(SUM(d.Quantity), 0) AS [Units Available] "
                    "FROM BloodGroups bg "
                    "LEFT JOIN Donations d ON bg.GroupName = d.BloodGroup "
                    "   AND d.ExpiryDate >= CAST(GETDATE() AS DATE) "
                    "GROUP BY bg.GroupName "
                    "ORDER BY bg.GroupName");
                if (dt != nullptr)
                    dgvQuickOverview->DataSource = dt;
            }
        }

        void LoadInventory()
        {
            if (dgvInventory == nullptr) return;

            DataTable^ dt = Database::GetInstance()->FillDataTable(
                "SELECT bg.GroupName AS [Blood Group], "
                "       ISNULL(SUM(d.Quantity), 0) AS [Units Available] "
                "FROM BloodGroups bg "
                "LEFT JOIN Donations d ON bg.GroupName = d.BloodGroup "
                "   AND d.ExpiryDate >= CAST(GETDATE() AS DATE) "
                "GROUP BY bg.GroupName "
                "ORDER BY bg.GroupName");

            if (dt != nullptr)
                dgvInventory->DataSource = dt;
        }

        void LoadRequests()
        {
            if (dgvRequests == nullptr) return;

            DataTable^ dt = Database::GetInstance()->FillDataTable(
                "SELECT br.RequestID         AS [ID], "
                "       u.FullName           AS [Recipient Name], "
                "       br.BloodGroup        AS [Blood Group], "
                "       br.QuantityRequested AS [Qty], "
                "       br.RequestStatus     AS [Status], "
                "       br.RequestDate       AS [Date] "
                "FROM   BloodRequests br "
                "JOIN   Users u ON br.RecipientID = u.UserID "
                "WHERE  br.RequestStatus = 'Pending' "
                "ORDER BY CASE WHEN br.RequestStatus = 'Pending' THEN 0 ELSE 1 END, br.RequestDate DESC");

            if (dt != nullptr)
            {
                dgvRequests->DataSource = dt;

                if (!dgvRequests->Columns->Contains("btnApprove"))
                {
                    DataGridViewButtonColumn^ btnApprove = gcnew DataGridViewButtonColumn();
                    btnApprove->Name = "btnApprove";
                    btnApprove->HeaderText = "Action";
                    btnApprove->Text = "Approve";
                    btnApprove->UseColumnTextForButtonValue = true;
                    btnApprove->FlatStyle = FlatStyle::Flat;
                    btnApprove->DefaultCellStyle->ForeColor = Color::Green;
                    dgvRequests->Columns->Add(btnApprove);
                }

                if (!dgvRequests->Columns->Contains("btnReject"))
                {
                    DataGridViewButtonColumn^ btnReject = gcnew DataGridViewButtonColumn();
                    btnReject->Name = "btnReject";
                    btnReject->HeaderText = ""; 
                    btnReject->Text = "Reject";
                    btnReject->UseColumnTextForButtonValue = true;
                    btnReject->FlatStyle = FlatStyle::Flat;
                    btnReject->DefaultCellStyle->ForeColor = Color::Red;
                    dgvRequests->Columns->Add(btnReject);
                }
            }
        }


        //  NAVIGATION HELPERS

        void ShowPanel(Panel^ target)
        {
            pnlOverview->Visible = (target == pnlOverview);
            pnlInventory->Visible = (target == pnlInventory);
            pnlRequests->Visible = (target == pnlRequests);
        }

        void SetActiveNav(Button^ btn)
        {
            if (_activeNavBtn != nullptr)
            {
                _activeNavBtn->BackColor = CLR_BG_CARD;
                _activeNavBtn->ForeColor = CLR_TEXT;
                _activeNavBtn->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Regular);
            }
            _activeNavBtn = btn;
            btn->BackColor = CLR_HOVER_BG;
            btn->ForeColor = CLR_ACCENT;
            btn->Font = gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
        }


        //  EVENT HANDLERS

        // Form Load
        void OnLoad(Object^ sender, EventArgs^ e)
        {
            LoadAllData();
        }


        void OnRequestActionClick(Object^ sender, DataGridViewCellEventArgs^ e)
        {
            if (e->RowIndex < 0) return;

            String^ colName = dgvRequests->Columns[e->ColumnIndex]->Name;

            if (colName == "btnApprove" || colName == "btnReject")
            {
                int requestID = Convert::ToInt32(dgvRequests->Rows[e->RowIndex]->Cells["ID"]->Value);
                String^ currentStatus = dgvRequests->Rows[e->RowIndex]->Cells["Status"]->Value->ToString();
                String^ bloodGroup = dgvRequests->Rows[e->RowIndex]->Cells["Blood Group"]->Value->ToString();
                int qtyRequested = Convert::ToInt32(dgvRequests->Rows[e->RowIndex]->Cells["Qty"]->Value);

                if (currentStatus != "Pending")
                {
                    MessageBox::Show("This request has already been " + currentStatus + " and cannot be changed.",
                        "Action Not Allowed", MessageBoxButtons::OK, MessageBoxIcon::Information);
                    return;
                }

                String^ newStatus = (colName == "btnApprove") ? "Approved" : "Rejected";
                Database^ db = Database::GetInstance();

                if (newStatus == "Approved")
                {
                    String^ compGroups;
                    if (bloodGroup == "A+")       compGroups = "'A+', 'A-', 'O+', 'O-'";
                    else if (bloodGroup == "A-")  compGroups = "'A-', 'O-'";
                    else if (bloodGroup == "B+")  compGroups = "'B+', 'B-', 'O+', 'O-'";
                    else if (bloodGroup == "B-")  compGroups = "'B-', 'O-'";
                    else if (bloodGroup == "AB+") compGroups = "'AB+', 'AB-', 'A+', 'A-', 'B+', 'B-', 'O+', 'O-'";
                    else if (bloodGroup == "AB-") compGroups = "'AB-', 'A-', 'B-', 'O-'";
                    else if (bloodGroup == "O+")  compGroups = "'O+', 'O-'";
                    else if (bloodGroup == "O-")  compGroups = "'O-'";
                    else                          compGroups = "'" + bloodGroup + "'";

                    String^ checkQuery = String::Format(
                        "SELECT ISNULL(SUM(Quantity), 0) FROM Donations "
                        "WHERE BloodGroup IN ({0}) AND ExpiryDate >= CAST(GETDATE() AS DATE) AND Quantity > 0",
                        compGroups);

                    Object^ stockObj = db->ExecuteScalar(checkQuery);
                    int availableStock = (stockObj != nullptr && stockObj != DBNull::Value) ? Convert::ToInt32(stockObj) : 0;

                    if (availableStock < qtyRequested)
                    {
                        MessageBox::Show("Insufficient Inventory!\n\nYou do not have enough " + bloodGroup + " (or any compatible substitutes) to fulfill a request of " + qtyRequested + " units.",
                            "Approval Failed", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                        return;
                    }

                    System::Windows::Forms::DialogResult res = MessageBox::Show(
                        "Approve this request? This will deduct " + qtyRequested + " units of " + bloodGroup + " (or a compatible substitute) from your inventory.",
                        "Confirm Approval", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

                    if (res != System::Windows::Forms::DialogResult::Yes) return;

                    String^ getBatchesQuery = String::Format(
                        "SELECT DonationID, Quantity, BloodGroup FROM Donations "
                        "WHERE BloodGroup IN ({0}) AND ExpiryDate >= CAST(GETDATE() AS DATE) AND Quantity > 0 "
                        "ORDER BY CASE WHEN BloodGroup = '{1}' THEN 0 ELSE 1 END, ExpiryDate ASC",
                        compGroups, bloodGroup);

                    DataTable^ dtBatches = db->FillDataTable(getBatchesQuery);
                    int remainingToDeduct = qtyRequested;

                    for (int i = 0; i < dtBatches->Rows->Count && remainingToDeduct > 0; i++)
                    {
                        int donationID = Convert::ToInt32(dtBatches->Rows[i]["DonationID"]);
                        int batchQty = Convert::ToInt32(dtBatches->Rows[i]["Quantity"]);

                        int deductAmount = (batchQty >= remainingToDeduct) ? remainingToDeduct : batchQty;

                        SqlCommand^ updateStockCmd = gcnew SqlCommand(
                            "UPDATE Donations SET Quantity = Quantity - @deduct WHERE DonationID = @did");
                        updateStockCmd->Parameters->AddWithValue("@deduct", deductAmount);
                        updateStockCmd->Parameters->AddWithValue("@did", donationID);
                        db->ExecuteNonQuery(updateStockCmd);

                        remainingToDeduct -= deductAmount;
                    }
                }
                else
                {
                    System::Windows::Forms::DialogResult res = MessageBox::Show(
                        "Are you sure you want to mark this request as Rejected?",
                        "Confirm Rejection", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

                    if (res != System::Windows::Forms::DialogResult::Yes) return;
                }

                SqlCommand^ cmd = gcnew SqlCommand(
                    "UPDATE BloodRequests SET RequestStatus = @status WHERE RequestID = @id");
                cmd->Parameters->AddWithValue("@status", newStatus);
                cmd->Parameters->AddWithValue("@id", requestID);

                int rows = db->ExecuteNonQuery(cmd);

                if (rows > 0)
                {
                    MessageBox::Show("Request successfully " + newStatus + "!", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);

                    LoadRequests();
                    LoadInventory();
                    LoadOverviewMetrics();
                }
            }
        }

        void OnRequestCellFormatting(Object^ sender, DataGridViewCellFormattingEventArgs^ e)
        {
            if (e->RowIndex >= 0 && e->ColumnIndex >= 0)
            {
                String^ colName = dgvRequests->Columns[e->ColumnIndex]->Name;
                if (colName == "btnApprove")
                {
                    e->CellStyle->ForeColor = Color::Green;
                }
                else if (colName == "btnReject")
                {
                    e->CellStyle->ForeColor = Color::Red;
                }
            }
        }




        // Nav hover
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

        // Nav click: swap panel + refresh its data
        void OnNavClick(Object^ s, EventArgs^ e)
        {
            Button^ btn = safe_cast<Button^>(s);
            Panel^ target = dynamic_cast<Panel^>(btn->Tag);
            if (target == nullptr) return;

            SetActiveNav(btn);
            ShowPanel(target);

            if (target == pnlOverview)
            {
                lblTopBarTitle->Text = "Overview";
                LoadOverviewMetrics();
            }
            else if (target == pnlInventory)
            {
                lblTopBarTitle->Text = "Blood Inventory";
                LoadInventory();
            }
            else if (target == pnlRequests)
            {
                lblTopBarTitle->Text = "Blood Requests";
                LoadRequests();
            }
        }

        // Outline button hover
        void OnOutlineBtnEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_HOVER_BG;
        }
        void OnOutlineBtnLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_BG_CARD;
        }

        // Refresh handlers
        void OnRefreshInventory(Object^ s, EventArgs^ e) { LoadInventory(); }
        void OnRefreshRequests(Object^ s, EventArgs^ e) { LoadRequests(); }

        // Logout: close this form only
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


        //  CUSTOM PAINT HANDLERS

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