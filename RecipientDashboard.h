#pragma once
#include "Database.h"

// ================================================================
//  RecipientDashboard.h  —  Blood Bank Management System
//  "Clean-Light" Aesthetic: premium light mode + crimson accents
//
//  ARCHITECTURE:
//    Mirrors DonorDashboard.h exactly — same Z-order rules,
//    same emoji encoding, same colour palette, same grid style.
//    Role badge uses CLR_CARD_ORANGE (orange dot + "RECIPIENT").
//
//  Z-ORDER RULE (inherited from AdminDashboard v3):
//    BuildSidebar()   → adds pnlSidebar to Controls  [higher Z]
//    BuildRightArea() → adds pnlRight   to Controls  [lower  Z]
//    pnlSidebar->SendToBack() → WinForms docks Left (250px) first,
//    pnlRight fills the remainder.
//
//  INSIDE pnlRight (same rule applies):
//    Content panels added FIRST (Fill) → lower Z → docked second.
//    pnlTopBar added LAST             → higher Z → docked first,
//                                        claiming the top 56 px.
//    Content panels then fill whatever remains below the top bar.
//
//  PANELS:
//    pnlOverview    — Home / metrics
//    pnlRequest     — Request Blood (form card)
//    pnlMyRequests  — My Requests  (DataGridView)
//
//  GRID SCROLLBAR SUPPRESSION:
//    dgvMyRequests lives inside pnlGridBox (Dock::Fill, Padding(32,0,32,32)).
//    pnlMyRequests->AutoScroll = false.
//    dgvMyRequests->Dock = Fill → stretches to fill the padded box.
//    No AnchorStyles::Bottom anywhere → no phantom scroll bar.
//
//  EMOJI ENCODING:
//    Char::ConvertFromUtf32(int) runs at runtime using the CLR's
//    Unicode engine — unaffected by /source-charset or BOM settings.
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

    public ref class RecipientDashboard : public Form
    {

        // ══════════════════════════════════════════════════════════
        //  COLOUR PALETTE  — single source of truth (identical to Donor/Admin)
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
        Color CLR_CARD_ORANGE;   // #E67E22  — recipient role badge colour
        Color CLR_CARD_GREEN;    // #27AE60
        Color CLR_CARD_BLUE;     // #2980B9


        // ══════════════════════════════════════════════════════════
        //  MEMBER FIELDS
        // ══════════════════════════════════════════════════════════

        // ── Identity ─────────────────────────────────────────────
        int     _recipientID;
        String^ _fullName;

        // ── Navigation state ─────────────────────────────────────
        Button^ _activeNavBtn;

        // ── Layout panels ────────────────────────────────────────
        Panel^ pnlSidebar;       // 250 px, DockStyle::Left
        Panel^ pnlTopBar;        // 56  px, DockStyle::Top (inside pnlRight)
        Panel^ pnlOverview;      // content view 1 — DockStyle::Fill
        Panel^ pnlRequest;       // content view 2 — DockStyle::Fill
        Panel^ pnlMyRequests;    // content view 3 — DockStyle::Fill

        // ── Sidebar controls ─────────────────────────────────────
        Button^ btnNavHome;
        Button^ btnNavRequest;
        Button^ btnNavMyRequests;
        Button^ btnLogout;

        // ── Top-bar controls ─────────────────────────────────────
        Button^ btnClose;
        Label^ lblTopBarTitle;

        // ── DataGridView ─────────────────────────────────────────
        DataGridView^ dgvMyRequests;

        // ── Metric card value labels (Overview) ──────────────────
        Label^ lblMetricTotal;
        Label^ lblMetricPending;

        // ── Request form controls ─────────────────────────────────
        ComboBox^ cmbBloodGroup;
        NumericUpDown^ nudQuantity;


        // ══════════════════════════════════════════════════════════
        //  CONSTRUCTOR
        // ══════════════════════════════════════════════════════════
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


        // ══════════════════════════════════════════════════════════
        //  InitializeComponent
        // ══════════════════════════════════════════════════════════
    private:
        void InitializeComponent()
        {
            this->Text = "Recipient Dashboard  \u2014  LifeLine Blood Bank";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;
            this->Load += gcnew EventHandler(this, &RecipientDashboard::OnLoad);

            // ══ Z-ORDER — CRITICAL ADD SEQUENCE (mirrors DonorDashboard) ══
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
                this, &RecipientDashboard::OnSidebarPaint);

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

            // Orange filled circle dot + role text (recipient = orange)
            // U+25CF = BLACK CIRCLE ●
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
            btnNavRequest = MakeNavButton(260);
            btnNavMyRequests = MakeNavButton(306);

            // Char::ConvertFromUtf32 — runtime code-point conversion,
            // immune to MSVC source-encoding mis-transcription.
            // U+1F3E0 = HOUSE BUILDING 🏠
            btnNavHome->Text = " " + Char::ConvertFromUtf32(0x1F3E0) + "  Home";
            // U+1F6A8 = POLICE CARS REVOLVING LIGHT (used as "alert/request") 🚨
            // U+1F489 = SYRINGE 💉
            btnNavRequest->Text = " " + Char::ConvertFromUtf32(0x1F489) + "  Request Blood";
            // U+1F4CB = CLIPBOARD 📋
            btnNavMyRequests->Text = " " + Char::ConvertFromUtf32(0x1F4CB) + "  My Requests";

            pnlSidebar->Controls->Add(btnNavHome);
            pnlSidebar->Controls->Add(btnNavRequest);
            pnlSidebar->Controls->Add(btnNavMyRequests);
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
            btn->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnNavEnter);
            btn->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnNavLeave);
            btn->Click += gcnew EventHandler(this, &RecipientDashboard::OnNavClick);
            return btn;
        }

        // ── ④ Log Out button (DockStyle::Bottom) ──────────────────
        void BuildSidebarLogout()
        {
            btnLogout = gcnew Button();
            // U+1F511 = KEY 🔑
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
            BuildRequestPanel();
            BuildMyRequestsPanel();

            pnlOverview->Dock = DockStyle::Fill;
            pnlRequest->Dock = DockStyle::Fill;
            pnlMyRequests->Dock = DockStyle::Fill;

            // 2. Add content panels FIRST (lower Z → docked last → Fill)
            pnlRight->Controls->Add(pnlMyRequests);
            pnlRight->Controls->Add(pnlRequest);
            pnlRight->Controls->Add(pnlOverview);

            // 3. Add top bar LAST → highest Z → docked first → claims top 56 px
            pnlRight->Controls->Add(pnlTopBar);

            // 4. Wire nav buttons to content panels via Tag
            btnNavHome->Tag = pnlOverview;
            btnNavRequest->Tag = pnlRequest;
            btnNavMyRequests->Tag = pnlMyRequests;

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
                this, &RecipientDashboard::OnTopBarPaint);

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
            // U+1F497 = GROWING HEART 💗
            lblSub->Text = Char::ConvertFromUtf32(0x1F497)
                + "  We're here to help. Request blood quickly and safely.";
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
            lblMetricSection->Text = "YOUR REQUEST SUMMARY";
            lblMetricSection->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lblMetricSection->ForeColor = CLR_MUTED;
            lblMetricSection->AutoSize = true;
            lblMetricSection->Location = Drawing::Point(32, 134);
            lblMetricSection->BackColor = Color::Transparent;

            // ── Two metric cards ─────────────────────────────────
            //    Card 1: Total Requests  (Crimson accent)
            //    Card 2: Pending Requests (Orange accent — recipient theme)
            Panel^ cTotal = MakeMetricCard("TOTAL REQUESTS", CLR_ACCENT, 32, 160);
            Panel^ cPending = MakeMetricCard("PENDING REQUESTS", CLR_CARD_ORANGE, 272, 160);

            lblMetricTotal = GetMetricValueLabel(cTotal);
            lblMetricPending = GetMetricValueLabel(cPending);

            // ── Info tip card ─────────────────────────────────────
            Panel^ pnlTip = gcnew Panel();
            pnlTip->Size = Drawing::Size(462, 72);
            pnlTip->Location = Drawing::Point(32, 340);
            pnlTip->BackColor = ColorTranslator::FromHtml("#FEF9E7");  // warm amber tint
            pnlTip->Paint += gcnew PaintEventHandler(this, &RecipientDashboard::OnCardPaint);

            Panel^ tipBar = gcnew Panel();
            tipBar->Size = Drawing::Size(4, 72);
            tipBar->Location = Drawing::Point(0, 0);
            tipBar->BackColor = CLR_CARD_ORANGE;

            Label^ lblTipHead = gcnew Label();
            // U+2139 = INFORMATION SOURCE ℹ
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


        // ══════════════════════════════════════════════════════════
        //  PANEL 2 — REQUEST BLOOD
        // ══════════════════════════════════════════════════════════
        void BuildRequestPanel()
        {
            pnlRequest = gcnew Panel();
            pnlRequest->BackColor = CLR_BG_FORM;
            pnlRequest->AutoScroll = true;

            // ── Section heading ───────────────────────────────────
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

            // ── White form card ───────────────────────────────────
            Panel^ card = gcnew Panel();
            card->Size = Drawing::Size(480, 320);
            card->Location = Drawing::Point(32, 108);
            card->BackColor = CLR_BG_CARD;
            card->Paint += gcnew PaintEventHandler(this, &RecipientDashboard::OnCardPaint);

            // Orange left accent bar (recipient theme)
            Panel^ bar = gcnew Panel();
            bar->Size = Drawing::Size(4, 320);
            bar->Location = Drawing::Point(0, 0);
            bar->BackColor = CLR_CARD_ORANGE;

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
            nudQuantity->Maximum = 10;      // reasonable upper limit per request
            nudQuantity->Value = 1;
            nudQuantity->BorderStyle = BorderStyle::FixedSingle;

            // ── Informational note ────────────────────────────────
            Label^ lblInfo = gcnew Label();
            // U+2139 = INFORMATION SOURCE ℹ
            lblInfo->Text = Char::ConvertFromUtf32(0x2139)
                + "  Your request will be set to 'Pending' "
                "until reviewed and approved by an admin.";
            lblInfo->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Italic);
            lblInfo->ForeColor = CLR_MUTED;
            lblInfo->Size = Drawing::Size(420, 36);
            lblInfo->Location = Drawing::Point(28, 200);
            lblInfo->BackColor = Color::Transparent;

            // ── Divider ───────────────────────────────────────────
            Panel^ cardDiv = gcnew Panel();
            cardDiv->Size = Drawing::Size(420, 1);
            cardDiv->Location = Drawing::Point(28, 248);
            cardDiv->BackColor = CLR_BORDER;

            // ── Primary "Submit Request" button ───────────────────
            //    Solid crimson fill — primary call-to-action.
            Button^ btnSubmit = gcnew Button();
            // U+1FA78 = DROP OF BLOOD 🩸
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


        // ══════════════════════════════════════════════════════════
        //  PANEL 3 — MY REQUESTS
        // ══════════════════════════════════════════════════════════
        void BuildMyRequestsPanel()
        {
            pnlMyRequests = gcnew Panel();
            pnlMyRequests->BackColor = CLR_BG_FORM;
            pnlMyRequests->AutoScroll = false;  // no infinite scrollbar; grid fills

            // ── Top strip (Refresh button) ─────────────────────────
            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            // U+1F504 = ANTICLOCKWISE DOWNWARDS AND UPWARDS OPEN CIRCLE ARROWS 🔄
            Button^ btnRef = MakeOutlineButton(
                " " + Char::ConvertFromUtf32(0x1F504) + "  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &RecipientDashboard::OnRefreshRequests);
            pnlTop->Controls->Add(btnRef);

            // ── Padding container — grid never escapes the panel ────
            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvMyRequests = MakeStyledGrid();
            dgvMyRequests->Dock = DockStyle::Fill;  // fills perfectly inside the padding
            pnlGridBox->Controls->Add(dgvMyRequests);

            // Add in reverse Z-order: grid box (Fill) first, then top strip (Top)
            pnlMyRequests->Controls->Add(pnlGridBox);
            pnlMyRequests->Controls->Add(pnlTop);
        }


        // ══════════════════════════════════════════════════════════
        //  SHARED WIDGET FACTORIES  (identical style to DonorDashboard)
        // ══════════════════════════════════════════════════════════

        // Metric card: coloured 4-px left bar + large number/text label
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
            lblVal->Tag = "metric_value";    // retrieval marker

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
            btn->MouseEnter += gcnew EventHandler(this, &RecipientDashboard::OnOutlineBtnEnter);
            btn->MouseLeave += gcnew EventHandler(this, &RecipientDashboard::OnOutlineBtnLeave);
            return btn;
        }


        // ══════════════════════════════════════════════════════════
        //  DATA LOADING
        // ══════════════════════════════════════════════════════════

        void LoadAllData()
        {
            LoadOverviewMetrics();
            LoadMyRequests();
        }

        // ── Overview: Total & Pending requests for this recipient ─
        void LoadOverviewMetrics()
        {
            Database^ db = Database::GetInstance();

            // ── Total requests submitted by THIS recipient ────────
            SqlCommand^ cmdTotal = gcnew SqlCommand(
                "SELECT COUNT(*) FROM BloodRequests WHERE UserID = @id");
            cmdTotal->Parameters->AddWithValue("@id", _recipientID);
            Object^ tObj = db->ExecuteScalar(cmdTotal);
            if (lblMetricTotal != nullptr && tObj != nullptr)
                lblMetricTotal->Text = Convert::ToInt32(tObj).ToString();

            // ── Pending requests for THIS recipient ───────────────
            SqlCommand^ cmdPending = gcnew SqlCommand(
                "SELECT COUNT(*) FROM BloodRequests "
                "WHERE  UserID = @id AND RequestStatus = 'Pending'");
            cmdPending->Parameters->AddWithValue("@id", _recipientID);
            Object^ pObj = db->ExecuteScalar(cmdPending);
            if (lblMetricPending != nullptr && pObj != nullptr)
                lblMetricPending->Text = Convert::ToInt32(pObj).ToString();
        }

        // ── My Requests grid: all requests for this recipient ─────
        void LoadMyRequests()
        {
            if (dgvMyRequests == nullptr) return;

            // Parameterised query — SQL-injection safe, scoped to this recipient
            SqlCommand^ cmd = gcnew SqlCommand(
                "SELECT RequestID          AS [Request ID], "
                "       BloodGroup         AS [Blood Group], "
                "       QuantityRequested  AS [Units], "
                "       RequestStatus      AS [Status], "
                "       RequestDate        AS [Request Date] "
                "FROM   BloodRequests "
                "WHERE  UserID = @id "
                "ORDER BY RequestDate DESC, RequestID DESC");
            cmd->Parameters->AddWithValue("@id", _recipientID);

            DataTable^ dt = Database::GetInstance()->FillDataTable(cmd);
            if (dt != nullptr)
                dgvMyRequests->DataSource = dt;
        }


        // ══════════════════════════════════════════════════════════
        //  NAVIGATION HELPERS
        // ══════════════════════════════════════════════════════════

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

        // ── Outline button hover ──────────────────────────────────
        void OnOutlineBtnEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_HOVER_BG;
        }
        void OnOutlineBtnLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_BG_CARD;
        }

        // ── Refresh my requests ───────────────────────────────────
        void OnRefreshRequests(Object^ s, EventArgs^ e)
        {
            LoadMyRequests();
        }

        // ── Submit blood request (parameterised INSERT) ────────────
        void OnSubmitRequest(Object^ s, EventArgs^ e)
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
            // RequestStatus defaults to 'Pending' for admin review
            SqlCommand^ cmd = gcnew SqlCommand(
                "INSERT INTO BloodRequests "
                "       (UserID, BloodGroup, QuantityRequested, RequestDate, RequestStatus) "
                "VALUES (@id, @bg, @qty, CAST(GETDATE() AS DATE), 'Pending')");
            cmd->Parameters->AddWithValue("@id", _recipientID);
            cmd->Parameters->AddWithValue("@bg", bg);
            cmd->Parameters->AddWithValue("@qty", qty);

            int rows = Database::GetInstance()->ExecuteNonQuery(cmd);

            if (rows > 0)
            {
                // Success dialog
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

    };  // ref class RecipientDashboard

}   // namespace BloodBank


// ================================================================
//  LOGINFORM WIRING (add to LoginForm after role check)
// ================================================================
//
//   else if (role == "Recipient")
//   {
//       this->Hide();
//       RecipientDashboard^ rd = gcnew RecipientDashboard(userID, fullName);
//       rd->FormClosed += gcnew FormClosedEventHandler(
//                             this, &LoginForm::OnChildClosed);
//       rd->Show();
//   }
//
// ================================================================
//  ARCHITECTURE NOTES
// ================================================================
//
//  Z-ORDER (mirrors DonorDashboard / AdminDashboard v3 exactly)
//  ─────────────────────────────────────────────────────────────
//  BuildSidebar()   → this->Controls->Add(pnlSidebar)  [high Z]
//  BuildRightArea() → this->Controls->Add(pnlRight)    [low  Z]
//  pnlSidebar->SendToBack()
//    → WinForms docks pnlRight (Fill) first, consuming all space.
//    → Then docks pnlSidebar (Left) on top, claiming 250 px.
//
//  INSIDE pnlRight (same rule applies):
//    Content panels added FIRST (Fill) → lower Z → docked second.
//    pnlTopBar added LAST             → higher Z → docked first,
//                                        claiming the top 56 px.
//    Content panels then fill whatever remains below the top bar.
//
//  GRID SCROLLBAR SUPPRESSION
//  ─────────────────────────────────────────────────────────────
//  dgvMyRequests lives inside pnlGridBox (Dock::Fill, Padding(32,0,32,32)).
//  pnlMyRequests->AutoScroll = false.
//  dgvMyRequests->Dock = Fill → stretches to fill the padded box.
//  No AnchorStyles::Bottom anywhere → no phantom scroll bar.
//
//  EMOJI ENCODING
//  ─────────────────────────────────────────────────────────────
//  Char::ConvertFromUtf32(int) runs at runtime using the CLR's
//  Unicode engine — unaffected by /source-charset or BOM settings.
//  0x25CF = ●  BLACK CIRCLE         (role badge dot)
//  0x1F3E0 = 🏠 HOUSE BUILDING      (Home nav)
//  0x1F489 = 💉 SYRINGE              (Request Blood nav)
//  0x1F4CB = 📋 CLIPBOARD            (My Requests nav)
//  0x1F511 = 🔑 KEY                  (Logout)
//  0x1F504 = 🔄 ARROWS (Refresh)
//  0x1FA78 = 🩸 DROP OF BLOOD        (Submit button)
//  0x1F497 = 💗 GROWING HEART        (overview subtitle + success msg)
//  0x2139  = ℹ  INFORMATION SOURCE   (tip card + form note)
//
// ================================================================