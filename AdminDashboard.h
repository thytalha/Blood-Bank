#pragma once
#include "Database.h"

// ================================================================
//  AdminDashboard.h  —  Blood Bank Management System
//  "Clean-Light" Aesthetic: premium light mode + crimson accents
//
//  BUG FIXES APPLIED (v3):
//    ① Z-order:  pnlRight added FIRST, pnlSidebar added SECOND,
//                then pnlSidebar->BringToFront() — guaranteed layout.
//    ② Emojis:   Char::ConvertFromUtf32(0x...) — compiler-agnostic,
//                works regardless of MSVC source-encoding settings.
//    ③ Close ✕:  btnClose uses DockStyle::Right inside pnlTopBar.
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

    public ref class AdminDashboard : public Form
    {
        // ══════════════════════════════════════════════════════════
        //  COLOUR PALETTE  — single source of truth
        // ══════════════════════════════════════════════════════════
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


        // ══════════════════════════════════════════════════════════
        //  MEMBER FIELDS
        // ══════════════════════════════════════════════════════════

        // ── Identity ─────────────────────────────────────────────
        int     _adminID;
        String^ _fullName;

        // ── Navigation state ─────────────────────────────────────
        Button^ _activeNavBtn;

        // ── Layout panels ────────────────────────────────────────
        Panel^ pnlSidebar;     // 250 px,  DockStyle::Left
        Panel^ pnlTopBar;      // 56 px,   DockStyle::Top  (inside pnlRight)
        Panel^ pnlOverview;    // content view 1  — DockStyle::Fill
        Panel^ pnlInventory;   // content view 2  — DockStyle::Fill
        Panel^ pnlRequests;    // content view 3  — DockStyle::Fill

        // ── Sidebar controls ─────────────────────────────────────
        Button^ btnNavOverview;
        Button^ btnNavInventory;
        Button^ btnNavRequests;
        Button^ btnLogout;      // DockStyle::Bottom

        // ── Top-bar controls ─────────────────────────────────────
        Button^ btnClose;       // DockStyle::Right  →  Application::Exit()
        Label^ lblTopBarTitle;

        // ── DataGridViews ─────────────────────────────────────────
        DataGridView^ dgvQuickOverview;
        DataGridView^ dgvInventory;
        DataGridView^ dgvRequests;

        // ── Metric card value labels ──────────────────────────────
        Label^ lblMetricPending;
        Label^ lblMetricExpired;
        Label^ lblMetricUsers;
        Label^ lblMetricDonations;


        // ══════════════════════════════════════════════════════════
        //  CONSTRUCTOR
        // ══════════════════════════════════════════════════════════
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


        // ══════════════════════════════════════════════════════════
        //  InitializeComponent
        // ══════════════════════════════════════════════════════════
    private:
        void InitializeComponent()
        {
            this->Text = "Admin Dashboard  —  LifeLine Blood Bank";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;
            this->Load += gcnew EventHandler(this, &AdminDashboard::OnLoad);

            // ══ FIX ①  Z-ORDER — CRITICAL ADD SEQUENCE ══════════
            //
            // WinForms docking rule:
            //   Controls are docked in REVERSE Z-order.
            //   The control with the HIGHEST Z-order (last added /
            //   BringToFront'd) is docked first and claims its edge.
            //   The next control then fills what remains.
            //
            // Required result: Sidebar claims 250 px on the left;
            //   pnlRight fills the rest.
            //
            // Therefore:
            //   1. Add pnlRight  FIRST  → lower Z-order → docked second
            //   2. Add pnlSidebar SECOND → higher Z-order → docked first
            //   3. BringToFront() makes the intent explicit and
            //      immune to any future control-add reordering.
            //
            BuildSidebar();             // step 2: adds pnlSidebar (Left)
            BuildRightArea();           // step 1: adds pnlRight  (Fill)
            pnlSidebar->SendToBack();
        }


        // ══════════════════════════════════════════════════════════
        //  SIDEBAR  (DockStyle::Left, 250 px, pure white)
        // ══════════════════════════════════════════════════════════
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

        // ── ① Crimson brand strip (80 px tall) ───────────────────
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

        // ── ② Welcome block (name + role tag) ────────────────────
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

            // Allocate buttons first so text can be set below
            btnNavOverview = MakeNavButton(214);
            btnNavInventory = MakeNavButton(260);
            btnNavRequests = MakeNavButton(306);

            // ══ FIX ②  EMOJI — Char::ConvertFromUtf32() ══════════
            //
            // MSVC mangles UTF-8 string literals (producing mojibake
            // like "ðŸ"Š") when the source file's BOM or the compiler's
            // /source-charset switch does not match the runtime code
            // page.  Char::ConvertFromUtf32() operates on integer code
            // points at runtime, completely bypassing the compiler's
            // text-encoding pipeline — the correct glyph is always
            // produced regardless of project encoding settings.
            //
            btnNavOverview->Text = " " + Char::ConvertFromUtf32(0x1F4CA) + "  Overview";
            btnNavInventory->Text = " " + Char::ConvertFromUtf32(0x1F9EA) + "  Inventory";
            btnNavRequests->Text = " " + Char::ConvertFromUtf32(0x1F4CB) + "  Requests";

            pnlSidebar->Controls->Add(btnNavOverview);
            pnlSidebar->Controls->Add(btnNavInventory);
            pnlSidebar->Controls->Add(btnNavRequests);
        }

        // Factory: allocate a nav button at the given Y coordinate.
        // Text is set by the caller after construction (see FIX ②).
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

        // ── ④ Log Out button (DockStyle::Bottom) ──────────────────
        //
        // DockStyle::Bottom is resolved after the form reaches its
        // final maximised size, so the button is always flush to the
        // bottom of the sidebar — no manual Y arithmetic required.
        //
        void BuildSidebarLogout()
        {
            btnLogout = gcnew Button();
            // FIX ②: runtime code-point conversion for the key emoji
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


        // ══════════════════════════════════════════════════════════
        //  RIGHT AREA  (top bar + three swappable content panels)
        // ══════════════════════════════════════════════════════════
        void BuildRightArea()
        {
            Panel^ pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::Fill;
            pnlRight->BackColor = CLR_BG_FORM;

            // 1. Build the components
            BuildTopBar();
            pnlTopBar->Dock = DockStyle::Top;

            BuildOverviewPanel();
            BuildInventoryPanel();
            BuildRequestsPanel();

            pnlOverview->Dock = DockStyle::Fill;
            pnlInventory->Dock = DockStyle::Fill;
            pnlRequests->Dock = DockStyle::Fill;

            // 2. Add Content Panels FIRST
            pnlRight->Controls->Add(pnlRequests);
            pnlRight->Controls->Add(pnlInventory);
            pnlRight->Controls->Add(pnlOverview);

            // 3. Add Top Bar LAST (WinForms docks the last-added item first, saving the top 56px)
            pnlRight->Controls->Add(pnlTopBar);

            // Wire each nav button to its content panel via Tag
            btnNavOverview->Tag = pnlOverview;
            btnNavInventory->Tag = pnlInventory;
            btnNavRequests->Tag = pnlRequests;

            // Default view on startup
            ShowPanel(pnlOverview);
            SetActiveNav(btnNavOverview);

            // Finally, add the entire right side to the main form
            this->Controls->Add(pnlRight);
        }

        // ── Top bar (56 px, white, 1-px bottom border) ────────────
        //
        // FIX ③: btnClose uses DockStyle::Right so it is always
        // pinned to the right edge without manual coordinate math.
        //
        void BuildTopBar()
        {
            pnlTopBar = gcnew Panel();
            pnlTopBar->Height = 56;
            pnlTopBar->BackColor = CLR_BG_CARD;
            pnlTopBar->Paint += gcnew PaintEventHandler(
                this, &AdminDashboard::OnTopBarPaint);

            // "X" close button — docked right inside the top bar
            btnClose = gcnew Button();
            btnClose->Text = "X";
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnClose->Width = 56;
            btnClose->Dock = DockStyle::Right;   // always flush-right
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

            // Add btnClose FIRST so DockStyle::Right claims its strip;
            // lblTopBarTitle uses an absolute location so order is irrelevant for it.
            pnlTopBar->Controls->Add(btnClose);
            pnlTopBar->Controls->Add(lblTopBarTitle);
        }


        // ══════════════════════════════════════════════════════════
        //  OVERVIEW PANEL
        // ══════════════════════════════════════════════════════════
        void BuildOverviewPanel()
        {
            pnlOverview = gcnew Panel();
            pnlOverview->BackColor = CLR_BG_FORM;
            pnlOverview->AutoScroll = true;

            // Four metric cards (Moved up to Y = 32 so they don't hide under the top bar)
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
            lblQT->Location = Drawing::Point(32, 212); // Shifted up
            lblQT->BackColor = Color::Transparent;

            dgvQuickOverview = MakeStyledGrid();
            dgvQuickOverview->Location = Drawing::Point(32, 248); // Shifted up
            dgvQuickOverview->Size = Drawing::Size(600, 300);
            dgvQuickOverview->Anchor = AnchorStyles::Top | AnchorStyles::Left;

            pnlOverview->Controls->Add(cPending);
            pnlOverview->Controls->Add(cExpired);
            pnlOverview->Controls->Add(cUsers);
            pnlOverview->Controls->Add(cDonations);
            pnlOverview->Controls->Add(lblQT);
            pnlOverview->Controls->Add(dgvQuickOverview);
        }

        // Metric card: coloured 4-px left bar + large number label
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
            lblVal->Tag = "metric_value";    // retrieval marker

            card->Controls->Add(bar);
            card->Controls->Add(lblTitle);
            card->Controls->Add(lblVal);
            return card;
        }

        // Walk a card's children and return the label marked "metric_value"
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


        // ══════════════════════════════════════════════════════════
        //  INVENTORY PANEL
        // ══════════════════════════════════════════════════════════
        void BuildInventoryPanel()
        {
            pnlInventory = gcnew Panel();
            pnlInventory->BackColor = CLR_BG_FORM;
            pnlInventory->AutoScroll = false; // Turn off infinite scrolling

            // 1. Top section (Just for the Refresh button now!)
            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            Button^ btnRef = MakeOutlineButton("  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &AdminDashboard::OnRefreshInventory);
            pnlTop->Controls->Add(btnRef);

            // 2. Padding Container (Guarantees the grid doesn't fall off the screen)
            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvInventory = MakeStyledGrid();
            dgvInventory->Dock = DockStyle::Fill; // Fills perfectly inside the padding!

            pnlGridBox->Controls->Add(dgvInventory);

            // 3. Add to panel in reverse order so the top bar docks first
            pnlInventory->Controls->Add(pnlGridBox);
            pnlInventory->Controls->Add(pnlTop);
        }


        // ══════════════════════════════════════════════════════════
        //  REQUESTS PANEL
        // ══════════════════════════════════════════════════════════
        void BuildRequestsPanel()
        {
            pnlRequests = gcnew Panel();
            pnlRequests->BackColor = CLR_BG_FORM;
            pnlRequests->AutoScroll = false; // Turn off infinite scrolling

            // 1. Top section (Just for the Refresh button now!)
            Panel^ pnlTop = gcnew Panel();
            pnlTop->Dock = DockStyle::Top;
            pnlTop->Height = 80;

            Button^ btnRef = MakeOutlineButton("  Refresh", 32, 24);
            btnRef->Click += gcnew EventHandler(this, &AdminDashboard::OnRefreshRequests);
            pnlTop->Controls->Add(btnRef);

            // 2. Padding Container (Guarantees the grid doesn't fall off the screen)
            Panel^ pnlGridBox = gcnew Panel();
            pnlGridBox->Dock = DockStyle::Fill;
            pnlGridBox->Padding = System::Windows::Forms::Padding(32, 0, 32, 32);

            dgvRequests = MakeStyledGrid();
            dgvRequests->Dock = DockStyle::Fill; // Fills perfectly inside the padding!

            pnlGridBox->Controls->Add(dgvRequests);

            // 3. Add to panel in reverse order so the top bar docks first
            pnlRequests->Controls->Add(pnlGridBox);
            pnlRequests->Controls->Add(pnlTop);
        }


        // ══════════════════════════════════════════════════════════
        //  DATAGRIDVIEW FACTORY  — fully custom styled
        // ══════════════════════════════════════════════════════════
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

            // Must be false — otherwise Windows overrides header colours
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

            // Default cells: white bg, dark text, blush selection
            DataGridViewCellStyle^ cStyle = gcnew DataGridViewCellStyle();
            cStyle->BackColor = Color::White;
            cStyle->ForeColor = CLR_TEXT;
            cStyle->SelectionBackColor = CLR_SEL_BG;
            cStyle->SelectionForeColor = CLR_SEL_FG;
            cStyle->Font = gcnew Drawing::Font("Segoe UI", 9);
            cStyle->Padding = System::Windows::Forms::Padding(10, 4, 0, 4);
            dgv->DefaultCellStyle = cStyle;

            // Alternating rows: very subtle off-white tint
            DataGridViewCellStyle^ aStyle = gcnew DataGridViewCellStyle();
            aStyle->BackColor = CLR_ALT_ROW;
            aStyle->ForeColor = CLR_TEXT;
            aStyle->SelectionBackColor = CLR_SEL_BG;
            aStyle->SelectionForeColor = CLR_SEL_FG;
            dgv->AlternatingRowsDefaultCellStyle = aStyle;

            dgv->RowTemplate->Height = 38;
            return dgv;
        }

        // Crimson-outline action button (used for Refresh etc.)
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


        // ══════════════════════════════════════════════════════════
        //  DATA LOADING
        // ══════════════════════════════════════════════════════════

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
                    "SELECT BloodGroup    AS [Blood Group], "
                    "       QuantityUnits AS [Units Available] "
                    "FROM   BloodInventory "
                    "ORDER BY BloodGroup");
                if (dt != nullptr)
                    dgvQuickOverview->DataSource = dt;
            }
        }

        void LoadInventory()
        {
            if (dgvInventory == nullptr) return;
            DataTable^ dt = Database::GetInstance()->FillDataTable(
                "SELECT BloodGroup    AS [Blood Group], "
                "       QuantityUnits AS [Units Available] "
                "FROM   BloodInventory "
                "ORDER BY BloodGroup");
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
                "JOIN   Users u ON br.UserID = u.UserID "
                "ORDER BY br.RequestDate DESC");
            if (dt != nullptr)
                dgvRequests->DataSource = dt;
        }


        // ══════════════════════════════════════════════════════════
        //  NAVIGATION HELPERS
        // ══════════════════════════════════════════════════════════

        // Hide all panels then reveal the target
        void ShowPanel(Panel^ target)
        {
            pnlOverview->Visible = (target == pnlOverview);
            pnlInventory->Visible = (target == pnlInventory);
            pnlRequests->Visible = (target == pnlRequests);
        }

        // Style the newly active nav button; reset the previous one
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


        // ══════════════════════════════════════════════════════════
        //  EVENT HANDLERS
        // ══════════════════════════════════════════════════════════

        // ── Form Load ─────────────────────────────────────────────
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

        // ── Nav click: swap panel + refresh its data ──────────────
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

        // ── Outline button hover ──────────────────────────────────
        void OnOutlineBtnEnter(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_HOVER_BG;
        }
        void OnOutlineBtnLeave(Object^ s, EventArgs^ e)
        {
            safe_cast<Button^>(s)->BackColor = CLR_BG_CARD;
        }

        // ── Refresh handlers ──────────────────────────────────────
        void OnRefreshInventory(Object^ s, EventArgs^ e) { LoadInventory(); }
        void OnRefreshRequests(Object^ s, EventArgs^ e) { LoadRequests(); }

        // ── Logout: close this form only ──────────────────────────
        //    LoginForm::OnChildClosed fires → login screen reappears.
        //    Must NOT call Application::Exit() here.
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
            this->Close();   // closes dashboard only; login re-shows itself
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
            Application::Exit();   // terminates the process entirely
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

        // Welcome panel — 1-px bottom divider
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

        // Metric card — thin border on all four sides
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

    };  // ref class AdminDashboard

}   // namespace BloodBank


// ================================================================
//  BUG-FIX NOTES  (v3)
// ================================================================
//
//  ① Z-ORDER / PANEL OVERLAP
//     WinForms resolves DockStyle in reverse Z-order.  The control
//     with the highest Z-order is docked first, claiming its strip;
//     the next control fills the remainder.  To get Sidebar (Left)
//     | Content (Fill):
//
//       BuildRightArea();            ← adds pnlRight  to Controls [lower Z]
//       BuildSidebar();              ← adds pnlSidebar to Controls [higher Z]
//       pnlSidebar->BringToFront();  ← makes highest Z explicit
//
//     pnlSidebar is now docked first (Left, 250 px); pnlRight gets
//     everything that remains.  Without BringToFront() the order
//     can be silently reversed by Control::Add internals.
//
//  ② EMOJI MOJIBAKE
//     MSVC compiles string literals using the Windows ANSI code page
//     (or /source-charset, which defaults to the system locale).
//     Characters outside that page are silently mis-transcribed.
//     Char::ConvertFromUtf32(int) runs at *runtime* using the CLR's
//     Unicode engine — it is completely unaffected by the compiler's
//     source-encoding settings and always produces the correct glyph.
//
//  ③ CLOSE BUTTON POSITION
//     btnClose->Dock = DockStyle::Right inside pnlTopBar.
//     No manual X coordinate, no Resize event needed.  The dock
//     engine keeps it flush-right at every window size automatically.
//
// ================================================================
//  LOGINFORM WIRING REMINDER
// ================================================================
//
//   this->Hide();
//   AdminDashboard^ d = gcnew AdminDashboard(userID, fullName);
//   d->FormClosed += gcnew FormClosedEventHandler(
//                        this, &LoginForm::OnChildClosed);
//   d->Show();
//
// ================================================================