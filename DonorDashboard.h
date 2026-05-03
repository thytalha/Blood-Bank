#pragma once
#include "Database.h"

// ================================================================
//  DonorDashboard.h  —  Blood Bank Management System
//  "Clean-Light" Aesthetic: premium light mode + crimson accents
//
//  ARCHITECTURE:
//    Mirrors AdminDashboard.h exactly — same Z-order rules,
//    same emoji encoding, same colour palette, same grid style.
//
//  Z-ORDER RULE (inherited from AdminDashboard v3):
//    BuildSidebar()   → adds pnlSidebar to Controls  [higher Z]
//    BuildRightArea() → adds pnlRight   to Controls  [lower  Z]
//    pnlSidebar->SendToBack() → WinForms docks Left (250px) first,
//    pnlRight fills the remainder.
//
//  PANELS:
//    pnlOverview  — Home / metrics
//    pnlDonate    — Log a new donation (form card)
//    pnlHistory   — Past donations (DataGridView)
// ================================================================

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

    public ref class DonorDashboard : public Form
    {

        // ══════════════════════════════════════════════════════════
        //  COLOUR PALETTE  — single source of truth (identical to Admin)
        // ══════════════════════════════════════════════════════════
    private:
        Color CLR_BG_FORM;       // #F8F9FA  soft off-white
        Color CLR_BG_CARD;       // #FFFFFF  pure white
        Color CLR_ACCENT;        // #C0392B  crimson
        Color CLR_TEXT;          // #212529  near-black
        Color CLR_MUTED;         // #6C757D  medium gray
        Color CLR_BORDER;        // #E9ECEF  pale gray
        Color CLR_HOVER_BG;      // #FADBD8  blush pink
        Color CLR_GRID_LINE;     // #DEE2E6
        Color CLR_SEL_BG;        // #FADBD8
        Color CLR_SEL_FG;        // #C0392B
        Color CLR_ALT_ROW;       // #FAFAFA
        Color CLR_CARD_ORANGE;   // #E67E22
        Color CLR_CARD_GREEN;    // #27AE60
        Color CLR_CARD_BLUE;     // #2980B9


        // ══════════════════════════════════════════════════════════
        //  MEMBER FIELDS
        // ══════════════════════════════════════════════════════════

        // ── Identity ─────────────────────────────────────────────
        int     _donorID;
        String^ _fullName;

        // ── Navigation state ─────────────────────────────────────
        Button^ _activeNavBtn;

        // ── Layout panels ────────────────────────────────────────
        Panel^ pnlSidebar;     // 250 px, DockStyle::Left
        Panel^ pnlTopBar;      // 56  px, DockStyle::Top (inside pnlRight)
        Panel^ pnlOverview;    // content view 1 — DockStyle::Fill
        Panel^ pnlDonate;      // content view 2 — DockStyle::Fill
        Panel^ pnlHistory;     // content view 3 — DockStyle::Fill

        // ── Sidebar controls ─────────────────────────────────────
        Button^ btnNavHome;
        Button^ btnNavDonate;
        Button^ btnNavHistory;
        Button^ btnLogout;

        // ── Top-bar controls ─────────────────────────────────────
        Button^ btnClose;
        Label^ lblTopBarTitle;

        // ── DataGridView ─────────────────────────────────────────
        DataGridView^ dgvHistory;

        // ── Metric card value labels (Overview) ──────────────────
        Label^ lblMetricTotalDonations;
        Label^ lblMetricLastDate;

        // ── Donate form controls ──────────────────────────────────
        ComboBox^ cmbBloodGroup;
        NumericUpDown^ nudQuantity;


        // ══════════════════════════════════════════════════════════
        //  CONSTRUCTOR
        // ══════════════════════════════════════════════════════════
    public:
        DonorDashboard(int donorID, String^ fullName)
        {
            _donorID = donorID;
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
        ~DonorDashboard() {}


        // ══════════════════════════════════════════════════════════
        //  InitializeComponent
        // ══════════════════════════════════════════════════════════
    private:
        void InitializeComponent()
        {
            this->Text = "Donor Dashboard  \u2014  LifeLine Blood Bank";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;
            this->Load += gcnew EventHandler(this, &DonorDashboard::OnLoad);

            // ══ Z-ORDER — CRITICAL ADD SEQUENCE (mirrors AdminDashboard) ══
            //
            //  WinForms docks controls in REVERSE Z-order.
            //  The control with the HIGHEST Z-order (last added /
            //  BringToFront'd) is docked first and claims its edge.
            //
            //  1. BuildSidebar()   → Controls->Add(pnlSidebar)  [higher Z]
            //  2. BuildRightArea() → Controls->Add(pnlRight)    [lower  Z]
            //  3. pnlSidebar->SendToBack() → makes pnlRight Fill first,
            //     then pnlSidebar claims Left 250 px.
            //
            BuildSidebar();         // adds pnlSidebar to this->Controls
            BuildRightArea();       // adds pnlRight   to this->Controls
            pnlSidebar->SendToBack();
        }


        // ══════════════════════════════════════════════════════════
        //  SIDEBAR
        // ══════════════════════════════════════════════════════════
        void BuildSidebar()
        {
            pnlSidebar = gcnew Panel();
            pnlSidebar->Width = 250;
            pnlSidebar->Dock = DockStyle::Left;
            pnlSidebar->BackColor = CLR_BG_CARD;
            pnlSidebar->Paint += gcnew PaintEventHandler(
                this, &DonorDashboard::OnSidebarPaint);

            BuildSidebarBrand();
            BuildSidebarWelcome();
            BuildSidebarNav();
            BuildSidebarLogout();

            this->Controls->Add(pnlSidebar);
        }

        // ── ① Crimson brand strip (80 px) ────────────────────────
        void BuildSidebarBrand()
        {
            Panel^ pnlBrand = gcnew Panel();
            pnlBrand->Size = Drawing::Size(250, 80);
            pnlBrand->Location = Drawing::Point(0, 0);
            pnlBrand->BackColor = CLR_ACCENT;

            Panel^ cv = gcnew Panel();          // cross — vertical bar
            cv->Size = Drawing::Size(6, 22);
            cv->Location = Drawing::Point(24, 28);
            cv->BackColor = Color::White;

            Panel^ ch = gcnew Panel();          // cross — horizontal bar
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

        // ── ② Welcome block (name + role badge) ──────────────────
        void BuildSidebarWelcome()
        {
            Panel^ pnlWelcome = gcnew Panel();
            pnlWelcome->Size = Drawing::Size(250, 90);
            pnlWelcome->Location = Drawing::Point(0, 80);
            pnlWelcome->BackColor = CLR_BG_CARD;
            pnlWelcome->Paint += gcnew PaintEventHandler(
                this, &DonorDashboard::OnDividerPaint);

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

            // Green filled circle dot + role text (donor = green)
            Label^ lblRole = gcnew Label();
            lblRole->Text = " " + Char::ConvertFromUtf32(0x25CF) + "  DONOR";
            lblRole->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblRole->ForeColor = CLR_CARD_GREEN;
            lblRole->AutoSize = true;
            lblRole->Location = Drawing::Point(20, 66);
            lblRole->BackColor = Color::Transparent;

            pnlWelcome->Controls->Add(lblHi);
            pnlWelcome->Controls->Add(lblName);
            pnlWelcome->Controls->Add(lblRole);
            pnlSidebar->Controls->Add(pnlWelcome);
        }

        // ── ③ Navigation buttons ──────────────────────────────────
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
            btnNavDonate = MakeNavButton(260);
            btnNavHistory = MakeNavButton(306);

            // Char::ConvertFromUtf32 — runtime code-point conversion,
            // immune to MSVC source-encoding mis-transcription.
            btnNavHome->Text = " " + Char::ConvertFromUtf32(0x1F3E0) + "  Home";
            btnNavDonate->Text = " " + Char::ConvertFromUtf32(0x1F489) + "  Donate Blood";
            btnNavHistory->Text = " " + Char::ConvertFromUtf32(0x1F4CB) + "  My History";

            pnlSidebar->Controls->Add(btnNavHome);
            pnlSidebar->Controls->Add(btnNavDonate);
            pnlSidebar->Controls->Add(btnNavHistory);
        }

        // Factory: nav button at a given Y coordinate
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
            btn->MouseEnter += gcnew EventHandler(this, &DonorDashboard::OnNavEnter);
            btn->MouseLeave += gcnew EventHandler(this, &DonorDashboard::OnNavLeave);
            btn->Click += gcnew EventHandler(this, &DonorDashboard::OnNavClick);
            return btn;
        }

        // ── ④ Log Out button (DockStyle::Bottom) ──────────────────
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
            btnLogout->Paint += gcnew PaintEventHandler(this, &DonorDashboard::OnLogoutPaint);
            btnLogout->MouseEnter += gcnew EventHandler(this, &DonorDashboard::OnLogoutEnter);
            btnLogout->MouseLeave += gcnew EventHandler(this, &DonorDashboard::OnLogoutLeave);
            btnLogout->Click += gcnew EventHandler(this, &DonorDashboard::OnLogoutClick);
            pnlSidebar->Controls->Add(btnLogout);
        }


        // ══════════════════════════════════════════════════════════
        //  RIGHT AREA  (top bar + three swappable content panels)
        // ══════════════════════════════════════════════════════════
        void BuildRightArea()
        {
            Panel^ pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::Fill;
            pnlRight->BackColor = CLR_BG_FORM;

            // 1. Build all components
            BuildTopBar();
            pnlTopBar->Dock = DockStyle::Top;

            BuildOverviewPanel();
            BuildDonatePanel();
            BuildHistoryPanel();

            pnlOverview->Dock = DockStyle::Fill;
            pnlDonate->Dock = DockStyle::Fill;
            pnlHistory->Dock = DockStyle::Fill;

            // 2. Add content panels FIRST (lower Z → docked last → Fill)
            pnlRight->Controls->Add(pnlHistory);
            pnlRight->Controls->Add(pnlDonate);
            pnlRight->Controls->Add(pnlOverview);

            // 3. Add top bar LAST → highest Z → docked first → claims top 56 px
            pnlRight->Controls->Add(pnlTopBar);

            // 4. Wire nav buttons to content panels via Tag
            btnNavHome->Tag = pnlOverview;
            btnNavDonate->Tag = pnlDonate;
            btnNavHistory->Tag = pnlHistory;

            // 5. Default view
            ShowPanel(pnlOverview);
            SetActiveNav(btnNavHome);

            this->Controls->Add(pnlRight);
        }

        // ── Top bar (56 px, white, 1-px bottom border) ────────────
        void BuildTopBar()
        {
            pnlTopBar = gcnew Panel();
            pnlTopBar->Height = 56;
            pnlTopBar->BackColor = CLR_BG_CARD;
            pnlTopBar->Paint += gcnew PaintEventHandler(
                this, &DonorDashboard::OnTopBarPaint);

            // "X" close — DockStyle::Right, always flush to right edge
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
            btnClose->MouseEnter += gcnew EventHandler(this, &DonorDashboard::OnCloseBtnEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &DonorDashboard::OnCloseBtnLeave);
            btnClose->Click += gcnew EventHandler(this, &DonorDashboard::OnCloseClick);

            lblTopBarTitle = gcnew Label();
            lblTopBarTitle->Text = "Home";
            lblTopBarTitle->Font = gcnew Drawing::Font("Segoe UI Light", 16, FontStyle::Regular);
            lblTopBarTitle->ForeColor = CLR_TEXT;
            lblTopBarTitle->AutoSize = true;
            lblTopBarTitle->Location = Drawing::Point(28, 13);
            lblTopBarTitle->BackColor = Color::Transparent;

            // btnClose added FIRST so DockStyle::Right claims its 56 px strip
            pnlTopBar->Controls->Add(btnClose);
            pnlTopBar->Controls->Add(lblTopBarTitle);
        }


        // ══════════════════════════════════════════════════════════
        //  PANEL 1 — OVERVIEW
        // ══════════════════════════════════════════════════════════
        void BuildOverviewPanel()
        {
            pnlOverview = gcnew Panel();
            pnlOverview->BackColor = CLR_BG_FORM;
            pnlOverview->AutoScroll = true;

            // ── Hero welcome section ──────────────────────────────
            Label^ lblHello = gcnew Label();
            lblHello->Text = "Hello, " + _fullName + "!";
            lblHello->Font = gcnew Drawing::Font("Segoe UI Light", 22, FontStyle::Regular);
            lblHello->ForeColor = CLR_TEXT;
            lblHello->AutoSize = true;
            lblHello->Location = Drawing::Point(32, 32);
            lblHello->BackColor = Color::Transparent;

            Label^ lblSub = gcnew Label();
            // U+1F496 = SPARKLING HEART
            lblSub->Text = Char::ConvertFromUtf32(0x1F496)
                + "  Your generosity saves lives. Thank you for being a donor.";
            lblSub->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblSub->ForeColor = CLR_MUTED;
            lblSub->AutoSize = true;
            lblSub->Location = Drawing::Point(34, 76);
            lblSub->BackColor = Color::Transparent;

            // ── Divider line ─────────────────────────────────────
            Panel^ pnlDivider = gcnew Panel();
            pnlDivider->Size = Drawing::Size(500, 1);
            pnlDivider->Location = Drawing::Point(32, 116);
            pnlDivider->BackColor = CLR_BORDER;

            // ── Section label ────────────────────────────────────
            Label^ lblMetricSection = gcnew Label();
            lblMetricSection->Text = "YOUR DONATION SUMMARY";
            lblMetricSection->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblMetricSection->ForeColor = CLR_MUTED;
            lblMetricSection->AutoSize = true;
            lblMetricSection->Location = Drawing::Point(32, 134);
            lblMetricSection->BackColor = Color::Transparent;

            // ── Two metric cards ─────────────────────────────────
            Panel^ cTotal = MakeMetricCard("TOTAL DONATIONS", CLR_ACCENT, 32, 160);
            Panel^ cLast = MakeMetricCard("LAST DONATION DATE", CLR_CARD_BLUE, 272, 160);

            lblMetricTotalDonations = GetMetricValueLabel(cTotal);
            lblMetricLastDate = GetMetricValueLabel(cLast);

            // Date text is longer — use a smaller font so it fits inside the card
            if (lblMetricLastDate != nullptr)
            {
                lblMetricLastDate->Font = gcnew Drawing::Font("Segoe UI Light", 18, FontStyle::Regular);
                lblMetricLastDate->Location = Drawing::Point(18, 52);
            }

            // ── Quick-tip card ───────────────────────────────────
            Panel^ pnlTip = gcnew Panel();
            pnlTip->Size = Drawing::Size(462, 72);
            pnlTip->Location = Drawing::Point(32, 340);
            pnlTip->BackColor = ColorTranslator::FromHtml("#EBF5FB");   // light blue tint
            pnlTip->Paint += gcnew PaintEventHandler(this, &DonorDashboard::OnCardPaint);

            Panel^ tipBar = gcnew Panel();
            tipBar->Size = Drawing::Size(4, 72);
            tipBar->Location = Drawing::Point(0, 0);
            tipBar->BackColor = CLR_CARD_BLUE;

            Label^ lblTipHead = gcnew Label();
            lblTipHead->Text = Char::ConvertFromUtf32(0x2139) + "  Did you know?";
            lblTipHead->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Bold);
            lblTipHead->ForeColor = CLR_CARD_BLUE;
            lblTipHead->AutoSize = true;
            lblTipHead->Location = Drawing::Point(20, 12);
            lblTipHead->BackColor = Color::Transparent;

            Label^ lblTipBody = gcnew Label();
            lblTipBody->Text = "A single donation can save up to 3 lives. "
                "You can donate again after 56 days.";
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
            pnlOverview->Controls->Add(cLast);
            pnlOverview->Controls->Add(pnlTip);
        }


        // ══════════════════════════════════════════════════════════
        //  PANEL 2 — DONATE BLOOD
        // ══════════════════════════════════════════════════════════
        void BuildDonatePanel()
        {
            pnlDonate = gcnew Panel();
            pnlDonate->BackColor = CLR_BG_FORM;
            pnlDonate->AutoScroll = true;

            // ── Section heading ───────────────────────────────────
            Label^ lblTitle = gcnew Label();
            lblTitle->Text = "Log a New Donation";
            lblTitle->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            lblTitle->ForeColor = CLR_TEXT;
            lblTitle->AutoSize = true;
            lblTitle->Location = Drawing::Point(32, 32);
            lblTitle->BackColor = Color::Transparent;

            Label^ lblSub = gcnew Label();
            lblSub->Text = "Fill in the details below and click Submit to record your contribution.";
            lblSub->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblSub->ForeColor = CLR_MUTED;
            lblSub->AutoSize = true;
            lblSub->Location = Drawing::Point(32, 66);
            lblSub->BackColor = Color::Transparent;

            // ── White form card ───────────────────────────────────
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(480, 340);
            card->Location = Drawing::Point(32, 108);
            card->BackColor = CLR_BG_CARD;
            card->Paint += gcnew PaintEventHandler(this, &DonorDashboard::OnCardPaint);

            // Crimson left accent bar (mirrors metric card pattern)
            Panel^ bar = gcnew Panel();
            bar->Size = Drawing::Size(4, 340);
            bar->Location = Drawing::Point(0, 0);
            bar->BackColor = CLR_ACCENT;

            // ── Blood Group row ───────────────────────────────────
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
            // All 8 ABO blood groups
            cmbBloodGroup->Items->Add("A+");
            cmbBloodGroup->Items->Add("A-");
            cmbBloodGroup->Items->Add("B+");
            cmbBloodGroup->Items->Add("B-");
            cmbBloodGroup->Items->Add("O+");
            cmbBloodGroup->Items->Add("O-");
            cmbBloodGroup->Items->Add("AB+");
            cmbBloodGroup->Items->Add("AB-");
            cmbBloodGroup->SelectedIndex = 0;

            // ── Quantity row ──────────────────────────────────────
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
            nudQuantity->Maximum = 5;       // safe upper limit per donation
            nudQuantity->Value = 1;
            nudQuantity->BorderStyle = BorderStyle::FixedSingle;

            // ── Informational note ────────────────────────────────
            Label^ lblInfo = gcnew Label();
            // U+2139 = information source ℹ
            lblInfo->Text = Char::ConvertFromUtf32(0x2139)
                + "  Donation date is set to today. "
                "Expiry is automatically set 42 days from now.";
            lblInfo->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Italic);
            lblInfo->ForeColor = CLR_MUTED;
            lblInfo->Size = Drawing::Size(420, 36);
            lblInfo->Location = Drawing::Point(28, 210);
            lblInfo->BackColor = Color::Transparent;

            // ── Divider ───────────────────────────────────────────
            Panel^ cardDiv = gcnew Panel();
            cardDiv->Size = Drawing::Size(420, 1);
            cardDiv->Location = Drawing::Point(28, 256);
            cardDiv->BackColor = CLR_BORDER;

            // ── Primary "Submit Donation" button ──────────────────
            //    Solid crimson fill — this is the primary call-to-action.
            Button^ btnSubmit = gcnew Button();
            // U+2764 = HEAVY BLACK HEART ❤
            btnSubmit->Text = Char::ConvertFromUtf32(0x2764) + "  Submit Donation";
            btnSubmit->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnSubmit->Size = Drawing::Size(210, 52);
            btnSubmit->Location = Drawing::Point(28, 272);
            btnSubmit->BackColor = CLR_ACCENT;
            btnSubmit->ForeColor = Color::White;
            btnSubmit->FlatStyle = FlatStyle::Flat;
            btnSubmit->FlatAppearance->BorderSize = 0;
            btnSubmit->Cursor = Cursors::Hand;
            btnSubmit->MouseEnter += gcnew EventHandler(this, &DonorDashboard::OnSubmitEnter);
            btnSubmit->MouseLeave += gcnew EventHandler(this, &DonorDashboard::OnSubmitLeave);
            btnSubmit->Click += gcnew EventHandler(this, &DonorDashboard::OnSubmitDonation);

            card->Controls->Add(bar);
            card->Controls->Add(lblBG);
            card->Controls->Add(cmbBloodGroup);
            card->Controls->Add(lblQty);
            card->Controls->Add(nudQuantity);
            card->Controls->Add(lblInfo);
            card->Controls->Add(cardDiv);
            card->Controls->Add(btnSubmit);

            pnlDonate->Controls->Add(lblTitle);
            pnlDonate->Controls->Add(lblSub);
            pnlDonate->Controls->Add(card);
        }


        // ══════════════════════════════════════════════════════════
        //  PANEL 3 — MY HISTORY
        // ══════════════════════════════════════════════════════════
        void BuildHistoryPanel()
        {
            pnlHistory = gcnew Panel();
            pnlHistory->BackColor = CLR_BG_FORM;
            pnlHistory->AutoScroll = false;   // no infinite scrollbar; grid fills

            // ── Top strip (Refresh button) ─────────────────────────
            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            // U+1F504 = ANTICLOCKWISE DOWNWARDS AND UPWARDS OPEN CIRCLE ARROWS
            Button^ btnRef = MakeOutlineButton(
                " " + Char::ConvertFromUtf32(0x1F504) + "  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &DonorDashboard::OnRefreshHistory);
            pnlTop->Controls->Add(btnRef);

            // ── Padding container — grid never escapes the panel ────
            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvHistory = MakeStyledGrid();
            dgvHistory->Dock = DockStyle::Fill;   // fills perfectly inside the padding
            pnlGridBox->Controls->Add(dgvHistory);

            // Add in reverse Z-order: grid box (Fill) first, then top strip (Top)
            pnlHistory->Controls->Add(pnlGridBox);
            pnlHistory->Controls->Add(pnlTop);
        }


        // ══════════════════════════════════════════════════════════
        //  SHARED WIDGET FACTORIES  (identical style to AdminDashboard)
        // ══════════════════════════════════════════════════════════

        // Metric card: coloured 4-px left bar + large number/text label
        Panel^ MakeMetricCard(String^ title, Color accent, int x, int y)
        {
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(222, 152);
            card->Location = Drawing::Point(x, y);
            card->BackColor = CLR_BG_CARD;
            card->Paint += gcnew PaintEventHandler(this, &DonorDashboard::OnCardPaint);

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
            lblVal->Tag = "metric_value";   // retrieval marker

            card->Controls->Add(bar);
            card->Controls->Add(lblTitle);
            card->Controls->Add(lblVal);
            return card;
        }

        // Walk a card's children and return the label tagged "metric_value"
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

        // Fully styled DataGridView — crimson headers, blush selection
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

            // MUST be false — otherwise Windows overrides header colours
            dgv->EnableHeadersVisualStyles = false;
            dgv->BackgroundColor = Color::White;
            dgv->BorderStyle = BorderStyle::None;
            dgv->CellBorderStyle = DataGridViewCellBorderStyle::SingleHorizontal;
            dgv->GridColor = CLR_GRID_LINE;

            // Column headers: crimson bg, white bold text
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

            // Default cells
            DataGridViewCellStyle^ cStyle = gcnew DataGridViewCellStyle();
            cStyle->BackColor = Color::White;
            cStyle->ForeColor = CLR_TEXT;
            cStyle->SelectionBackColor = CLR_SEL_BG;
            cStyle->SelectionForeColor = CLR_SEL_FG;
            cStyle->Font = gcnew Drawing::Font("Segoe UI", 9);
            cStyle->Padding = System::Windows::Forms::Padding(10, 4, 0, 4);
            dgv->DefaultCellStyle = cStyle;

            // Alternating rows
            DataGridViewCellStyle^ aStyle = gcnew DataGridViewCellStyle();
            aStyle->BackColor = CLR_ALT_ROW;
            aStyle->ForeColor = CLR_TEXT;
            aStyle->SelectionBackColor = CLR_SEL_BG;
            aStyle->SelectionForeColor = CLR_SEL_FG;
            dgv->AlternatingRowsDefaultCellStyle = aStyle;

            dgv->RowTemplate->Height = 38;
            return dgv;
        }

        // Crimson-outline action button (Refresh, etc.)
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
            btn->MouseEnter += gcnew EventHandler(this, &DonorDashboard::OnOutlineBtnEnter);
            btn->MouseLeave += gcnew EventHandler(this, &DonorDashboard::OnOutlineBtnLeave);
            return btn;
        }


        // ══════════════════════════════════════════════════════════
        //  DATA LOADING
        // ══════════════════════════════════════════════════════════

        void LoadAllData()
        {
            LoadOverviewMetrics();
            LoadHistory();
        }

        void LoadOverviewMetrics()
        {
            Database^ db = Database::GetInstance();

            // ── Total donations for THIS donor ────────────────────
            SqlCommand^ cmdCount = gcnew SqlCommand(
                "SELECT COUNT(*) FROM Donations WHERE DonorID = @id");
            cmdCount->Parameters->AddWithValue("@id", _donorID);
            Object^ cObj = db->ExecuteScalar(cmdCount);
            if (lblMetricTotalDonations != nullptr && cObj != nullptr)
                lblMetricTotalDonations->Text = Convert::ToInt32(cObj).ToString();

            // ── Date of most recent donation ─────────────────────
            SqlCommand^ cmdDate = gcnew SqlCommand(
                "SELECT MAX(DonationDate) FROM Donations WHERE DonorID = @id");
            cmdDate->Parameters->AddWithValue("@id", _donorID);
            Object^ dObj = db->ExecuteScalar(cmdDate);
            if (lblMetricLastDate != nullptr)
            {
                if (dObj != nullptr && dObj != DBNull::Value)
                    lblMetricLastDate->Text =
                    Convert::ToDateTime(dObj).ToString("dd MMM yyyy");
                else
                    lblMetricLastDate->Text = "N / A";
            }
        }

        void LoadHistory()
        {
            if (dgvHistory == nullptr) return;

            // Parameterised query — SQL-injection safe, scoped to this donor
            SqlCommand^ cmd = gcnew SqlCommand(
                "SELECT DonationID       AS [ID], "
                "       BloodGroup       AS [Blood Group], "
                "       Quantity         AS [Units], "
                "       DonationDate     AS [Donation Date], "
                "       ExpiryDate       AS [Expiry Date], "
                "       CASE WHEN ExpiryDate < CAST(GETDATE() AS DATE) "
                "            THEN 'Expired' "
                "            ELSE 'Valid' "
                "       END              AS [Status] "
                "FROM   Donations "
                "WHERE  DonorID = @id "
                "ORDER BY DonationDate DESC");
            cmd->Parameters->AddWithValue("@id", _donorID);

            DataTable^ dt = Database::GetInstance()->FillDataTable(cmd);
            if (dt != nullptr)
                dgvHistory->DataSource = dt;
        }


        // ══════════════════════════════════════════════════════════
        //  NAVIGATION HELPERS
        // ══════════════════════════════════════════════════════════

        void ShowPanel(Panel^ target)
        {
            pnlOverview->Visible = (target == pnlOverview);
            pnlDonate->Visible = (target == pnlDonate);
            pnlHistory->Visible = (target == pnlHistory);
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
            btn->Font =
                gcnew Drawing::Font("Segoe UI", 10, FontStyle::Bold);
        }


        // ══════════════════════════════════════════════════════════
        //  EVENT HANDLERS
        // ══════════════════════════════════════════════════════════

        // ── Form load ─────────────────────────────────────────────
        void OnLoad(Object^ sender, EventArgs^ e)
        {
            LoadAllData();
        }

        // ── Nav hover ─────────────────────────────────────────────
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

        // ── Nav click: swap panel + update top bar title ──────────
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
            else if (target == pnlDonate)
            {
                lblTopBarTitle->Text = "Donate Blood";
            }
            else if (target == pnlHistory)
            {
                lblTopBarTitle->Text = "My History";
                LoadHistory();
            }
        }

        // ── Outline button hover ──────────────────────────────────
        void OnOutlineBtnEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_HOVER_BG;
        }
        void OnOutlineBtnLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_BG_CARD;
        }

        // ── Refresh history ───────────────────────────────────────
        void OnRefreshHistory(Object^ s, EventArgs^ e)
        {
            LoadHistory();
        }

        // ── Submit donation (parameterised INSERT) ─────────────────
        void OnSubmitDonation(Object^ s, EventArgs^ e)
        {
            // Guard — ComboBox should always have a selection, but be safe
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

            // Parameterised INSERT — prevents SQL injection
            // ExpiryDate = today + 42 days (standard whole-blood shelf life)
            SqlCommand^ cmd = gcnew SqlCommand(
                "INSERT INTO Donations "
                "       (DonorID, BloodGroup, Quantity, DonationDate, ExpiryDate) "
                "VALUES (@donorID, @bg, @qty, "
                "        CAST(GETDATE() AS DATE), "
                "        CAST(DATEADD(day, 42, GETDATE()) AS DATE))");
            cmd->Parameters->AddWithValue("@donorID", _donorID);
            cmd->Parameters->AddWithValue("@bg", bg);
            cmd->Parameters->AddWithValue("@qty", qty);

            int rows = Database::GetInstance()->ExecuteNonQuery(cmd);

            if (rows > 0)
            {
                // Success dialog
                String^ expiryStr =
                    DateTime::Now.AddDays(42).ToString("dd MMM yyyy");

                MessageBox::Show(
                    "Donation recorded successfully!\n\n"
                    "Blood Group  :  " + bg + "\n"
                    "Units        :  " + qty.ToString() + "\n"
                    "Expires On   :  " + expiryStr + "\n\n"
                    "Thank you for saving lives " + Char::ConvertFromUtf32(0x2764),
                    "Donation Submitted",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information);

                // Reset form controls to defaults
                cmbBloodGroup->SelectedIndex = 0;
                nudQuantity->Value = 1;

                // Silently refresh the Home metrics so numbers stay current
                LoadOverviewMetrics();
            }
            // If rows <= 0 the Database singleton already showed an error dialog
        }

        // ── Submit button hover (darker crimson on hover) ──────────
        void OnSubmitEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor =
                ColorTranslator::FromHtml("#A93226");   // 15% darker crimson
        }
        void OnSubmitLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_ACCENT;
        }

        // ── Logout: close dashboard → LoginForm resurfaces ─────────
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
            this->Close();   // closes dashboard only; LoginForm::OnChildClosed fires
        }

        // ── Close (X): terminate the entire application ───────────
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


        // ══════════════════════════════════════════════════════════
        //  CUSTOM PAINT HANDLERS
        // ══════════════════════════════════════════════════════════

        // Sidebar — 1-px right-edge separator
        void OnSidebarPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, p->Width - 1, 0, p->Width - 1, p->Height);
            delete pen;
        }

        // Welcome block — 1-px bottom divider
        void OnDividerPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, 16, p->Height - 1, p->Width - 16, p->Height - 1);
            delete pen;
        }

        // Top bar — 1-px bottom border
        void OnTopBarPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, 0, p->Height - 1, p->Width, p->Height - 1);
            delete pen;
        }

        // Cards — thin border on all four sides
        void OnCardPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawRectangle(pen, 0, 0, p->Width - 1, p->Height - 1);
            delete pen;
        }

        // Logout button — 1-px top separator
        void OnLogoutPaint(Object^ s, PaintEventArgs^ e)
        {
            Button^ b = safe_cast<Button^>(s);
            Pen^ pen = gcnew Pen(CLR_BORDER, 1);
            e->Graphics->DrawLine(pen, 0, 0, b->Width, 0);
            delete pen;
        }

    };  // ref class DonorDashboard

}   // namespace BloodBank