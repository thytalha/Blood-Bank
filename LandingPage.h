// ================================================================
//  LandingPage.h  —  Blood Bank Management System
//  Animated · Scrollable · Light-Theme Landing Page  (v2)
//
//  ARCHITECTURE:
//    • Maximized + Borderless + AutoScroll=true fullscreen form
//    • Palette  BG=#FFFFFF  ACCENT=#C0392B  TEXT=#212529  MUTED=#6C757D
//    • Vertical Dock-Top stacking (processed in reverse Controls->Add order):
//        topNavPanel (80px)  →  heroPanel (600px)  →  cardsPanel (300px)
//        footerPanel (80px, Dock=Bottom)
//    • Hero splits into leftContentPanel (Dock=Left, 56%) +
//                       rightAnimPanel   (Dock=Fill)
//    • Pill badge, heading, subtext, 2 CTA buttons, stats row on left
//    • GDI+ animation engine on right: dark-crimson gradient box,
//        central hovering white blood drop + 4 orbiting blood-type drops
//    • 3 feature cards in cardsPanel — Y-axis hover-lift (+5px)
//    • Nav links: Home · Login · About · Contact + solid "Sign Up" button
//    • Login / "Login to Dashboard" → LoginForm  (OnChildClosed restores)
//    • "Sign Up" (nav + hero CTA missing; hero only has "Learn More")
//        actually Sign Up nav btn → SignupForm
//    • [✕] Anchor=Top|Right → Application::Exit()
//    • DoubleBuffered=true on form; Reflection double-buffer on rightAnimPanel
//
//  DOCK ORDER (Controls->Add reverse rule):
//    footerPanel(Bottom) → cardsPanel(Top) → heroPanel(Top) → topNavPanel(Top)
//    → btnClose(float)
//
//  Inside heroPanel:
//    rightAnimPanel(Fill) → leftContentPanel(Left)   [Fill added first]
//
//  GDI+ MEMORY: Every Brush / Pen / GraphicsPath deleted before return.
// ================================================================

#pragma once
#include <cmath>
#include "LoginForm.h"
#include "SignupForm.h"

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

namespace BloodBank {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;
    using namespace System::Reflection;

    public ref class LandingPage : public System::Windows::Forms::Form
    {
        // ══════════════════════════════════════════════════════════════════
        //  PUBLIC  —  Constructor / Destructor
        // ══════════════════════════════════════════════════════════════════
    public:
        LandingPage(void)
        {
            // ── Palette (single source of truth) ──────────────────────
            CLR_BG_FORM = Color::White;
            CLR_BG_CARD = Color::White;
            CLR_ACCENT = Color::FromArgb(0xC0, 0x39, 0x2B);
            CLR_TEXT = Color::FromArgb(0x21, 0x25, 0x29);
            CLR_MUTED = Color::FromArgb(0x6C, 0x75, 0x7D);
            CLR_PINK_LIGHT = Color::FromArgb(0xFF, 0xEB, 0xEE);

            // ── Animation state ────────────────────────────────────────
            angleO = 0.0f;
            angleA = 2.0f;
            angleB = 4.0f;
            angleAB = 5.5f;

            cardRestY = 22;

            InitializeComponent();

            // ── 33 fps animation clock ─────────────────────────────────
            animTimer = gcnew System::Windows::Forms::Timer();
            animTimer->Interval = 30;
            animTimer->Tick += gcnew EventHandler(this, &LandingPage::OnAnimTick);
            animTimer->Start();
        }

    protected:
        ~LandingPage()
        {
            if (components) delete components;
        }

        // ══════════════════════════════════════════════════════════════════
        //  PRIVATE FIELDS
        // ══════════════════════════════════════════════════════════════════
    private:

        // ── Colour Palette ─────────────────────────────────────────────
        Color CLR_BG_FORM;
        Color CLR_BG_CARD;
        Color CLR_ACCENT;
        Color CLR_TEXT;
        Color CLR_MUTED;
        Color CLR_PINK_LIGHT;

        // ── Right-anim inset margins ────────────────────────────────────
        //    The dark hero box is inset from rightAnimPanel edges by these.
        int RP_TOP = 20;
        int RP_BOTTOM = 20;
        int RP_RIGHT = 20;

        // ── Card hover baseline (updated by PositionControls) ───────────
        int cardRestY;

        // ── Section 1 – Header ──────────────────────────────────────────
        Panel^ topNavPanel;
        Panel^ logoBadge;
        Label^ lblBrand;
        Button^ btnNavHome;
        Button^ btnNavLogin;
        Button^ btnNavAbout;
        Button^ btnNavContact;
        Button^ btnSignUpNav;       // solid crimson "Sign Up" in nav

        // ── Section 2 – Hero ────────────────────────────────────────────
        Panel^ heroPanel;          // Dock=Top wrapper, H=600
        Panel^ leftContentPanel;   // Dock=Left, 56% of hero width
        Panel^ rightAnimPanel;     // Dock=Fill, GDI+ animation
        Panel^ pillBadgePanel;     // "♥ Every drop counts" pill
        Label^ lblPillText;
        Label^ lblHeading;
        Label^ lblSubText;
        Button^ btnLoginDash;       // Solid crimson → LoginForm
        Button^ btnLearnMore;       // Outline ghost
        Panel^ statsPanel;         // Stats row (donors / units / hospitals)
        Label^ lblStat1Num;    Label^ lblStat1Desc;
        Label^ lblStat2Num;    Label^ lblStat2Desc;
        Label^ lblStat3Num;    Label^ lblStat3Desc;
        Label^ lblRightTitle;      // White text inside dark box
        Label^ lblRightDesc;

        // ── Section 3 – Cards ───────────────────────────────────────────
        Panel^ cardsPanel;
        Panel^ cardManagement;
        Panel^ cardInventory;
        Panel^ cardRequests;
        Label^ lblDescMgmt;        // Stored for MaximumSize updates
        Label^ lblDescInv;
        Label^ lblDescReq;

        // ── Section 4 – Footer ──────────────────────────────────────────
        Panel^ footerPanel;
        Label^ lblFooterBrand;
        Label^ lblFooterCopy;

        // ── Close button ────────────────────────────────────────────────
        Button^ btnClose;

        // ── GDI+ Animation ─────────────────────────────────────────────
        System::Windows::Forms::Timer^ animTimer;
        float angleO;
        float angleA;
        float angleB;
        float angleAB;

        // ── Designer container ──────────────────────────────────────────
        System::ComponentModel::IContainer^ components;


        // ══════════════════════════════════════════════════════════════════
        //  InitializeComponent
        // ══════════════════════════════════════════════════════════════════
#pragma region Windows Form Designer generated code

        void InitializeComponent(void)
        {
            components = gcnew System::ComponentModel::Container();

            // ── Allocate all controls ──────────────────────────────────
            topNavPanel = gcnew Panel();
            logoBadge = gcnew Panel();
            lblBrand = gcnew Label();
            btnNavHome = gcnew Button();
            btnNavLogin = gcnew Button();
            btnNavAbout = gcnew Button();
            btnNavContact = gcnew Button();
            btnSignUpNav = gcnew Button();

            heroPanel = gcnew Panel();
            leftContentPanel = gcnew Panel();
            rightAnimPanel = gcnew Panel();
            pillBadgePanel = gcnew Panel();
            lblPillText = gcnew Label();
            lblHeading = gcnew Label();
            lblSubText = gcnew Label();
            btnLoginDash = gcnew Button();
            btnLearnMore = gcnew Button();
            statsPanel = gcnew Panel();
            lblStat1Num = gcnew Label();  lblStat1Desc = gcnew Label();
            lblStat2Num = gcnew Label();  lblStat2Desc = gcnew Label();
            lblStat3Num = gcnew Label();  lblStat3Desc = gcnew Label();
            lblRightTitle = gcnew Label();
            lblRightDesc = gcnew Label();

            cardsPanel = gcnew Panel();
            cardManagement = gcnew Panel();
            cardInventory = gcnew Panel();
            cardRequests = gcnew Panel();

            footerPanel = gcnew Panel();
            lblFooterBrand = gcnew Label();
            lblFooterCopy = gcnew Label();

            btnClose = gcnew Button();

            // ── Suspend layouts to prevent intermediate redraws ────────
            this->SuspendLayout();
            topNavPanel->SuspendLayout();
            heroPanel->SuspendLayout();
            leftContentPanel->SuspendLayout();
            rightAnimPanel->SuspendLayout();
            cardsPanel->SuspendLayout();
            footerPanel->SuspendLayout();
            statsPanel->SuspendLayout();
            pillBadgePanel->SuspendLayout();

            // ────────────────────────────────────────────────────────────
            //  FORM
            // ────────────────────────────────────────────────────────────
            this->Text = L"LifeBlood \u2014 Blood Bank Management System";
            this->BackColor = CLR_BG_FORM;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->WindowState = FormWindowState::Maximized;
            this->StartPosition = FormStartPosition::CenterScreen;
            this->DoubleBuffered = true;
            this->AutoScroll = true;   // enables scrollbar when content overflows
            this->Load += gcnew EventHandler(this, &LandingPage::OnFormLoad);
            this->Resize += gcnew EventHandler(this, &LandingPage::OnFormResize);

            // ────────────────────────────────────────────────────────────
            //  SECTION 1 · HEADER  (Dock=Top, H=80)
            //  White bar · logo · brand · nav text links · Sign Up btn
            // ────────────────────────────────────────────────────────────
            topNavPanel->Dock = DockStyle::Top;
            topNavPanel->Height = 80;
            topNavPanel->BackColor = CLR_BG_CARD;
            topNavPanel->Paint += gcnew PaintEventHandler(this, &LandingPage::topNavPanel_Paint);

            // ── Logo badge (48×48 crimson-drop icon) ────────────────────
            logoBadge->Size = System::Drawing::Size(48, 48);
            logoBadge->Location = System::Drawing::Point(28, 16);
            logoBadge->BackColor = Color::Transparent;
            logoBadge->Paint += gcnew PaintEventHandler(this, &LandingPage::logoBadge_Paint);

            // ── Brand wordmark ───────────────────────────────────────────
            lblBrand->AutoSize = true;
            lblBrand->BackColor = Color::Transparent;
            lblBrand->Font = gcnew Drawing::Font(L"Segoe UI", 17, FontStyle::Bold);
            lblBrand->ForeColor = CLR_TEXT;
            lblBrand->Text = L"LifeBlood";
            lblBrand->Location = System::Drawing::Point(86, 26);

            // ── Nav text links (positions computed in PositionControls) ─
            SetupNavLink(btnNavHome, L"Home", 72);
            SetupNavLink(btnNavAbout, L"About", 64);
            SetupNavLink(btnNavContact, L"Contact", 80);
            SetupNavLink(btnNavLogin, L"Login", 64);
            // Wire Login nav link to LoginForm
            btnNavLogin->Click += gcnew EventHandler(this, &LandingPage::btnLoginDash_Click);

            // ── "Sign Up" solid button ────────────────────────────────────
            btnSignUpNav->Text = L"Sign Up";
            btnSignUpNav->Size = System::Drawing::Size(108, 42);
            btnSignUpNav->FlatStyle = FlatStyle::Flat;
            btnSignUpNav->FlatAppearance->BorderSize = 0;
            btnSignUpNav->BackColor = CLR_ACCENT;
            btnSignUpNav->ForeColor = Color::White;
            btnSignUpNav->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            btnSignUpNav->Cursor = Cursors::Hand;
            btnSignUpNav->TabStop = false;
            btnSignUpNav->Paint += gcnew PaintEventHandler(this, &LandingPage::btnSolid_Paint);
            btnSignUpNav->MouseEnter += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
            btnSignUpNav->MouseLeave += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
            btnSignUpNav->Click += gcnew EventHandler(this, &LandingPage::btnSignUp_Click);

            topNavPanel->Controls->Add(logoBadge);
            topNavPanel->Controls->Add(lblBrand);
            topNavPanel->Controls->Add(btnNavHome);
            topNavPanel->Controls->Add(btnNavAbout);
            topNavPanel->Controls->Add(btnNavContact);
            topNavPanel->Controls->Add(btnNavLogin);
            topNavPanel->Controls->Add(btnSignUpNav);

            // ────────────────────────────────────────────────────────────
            //  SECTION 2 · HERO  (Dock=Top, H=600)
            //  Left (56%): pill badge + heading + sub + CTAs + stats
            //  Right (Fill): dark-crimson animated box
            // ────────────────────────────────────────────────────────────
            heroPanel->Dock = DockStyle::Top;
            heroPanel->Height = 600;
            heroPanel->BackColor = CLR_BG_FORM;

            // ── Left content panel (Dock=Left, width set in PositionControls)
            leftContentPanel->Dock = DockStyle::Left;
            leftContentPanel->Width = 600;          // placeholder; overridden
            leftContentPanel->BackColor = CLR_BG_FORM;

            // Pill badge: "♥ Every drop counts"
            pillBadgePanel->Size = System::Drawing::Size(234, 38);
            pillBadgePanel->BackColor = Color::Transparent;
            pillBadgePanel->Location = System::Drawing::Point(60, 72);  // overridden
            pillBadgePanel->Paint += gcnew PaintEventHandler(this, &LandingPage::pillBadge_Paint);

            lblPillText->AutoSize = true;
            lblPillText->BackColor = Color::Transparent;
            lblPillText->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
            lblPillText->ForeColor = CLR_ACCENT;
            lblPillText->Text = L"\u2665  Every drop counts";
            lblPillText->Location = System::Drawing::Point(18, 9);

            // Massive bold heading
            lblHeading->AutoSize = false;
            lblHeading->Font = gcnew Drawing::Font(L"Segoe UI", 36, FontStyle::Bold);
            lblHeading->ForeColor = CLR_TEXT;
            lblHeading->Text = L"Saving Lives,\nOne Donation\nat a Time.";
            lblHeading->Size = System::Drawing::Size(520, 180);
            lblHeading->Location = System::Drawing::Point(60, 128);    // overridden

            // Muted description
            lblSubText->AutoSize = false;
            lblSubText->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Regular);
            lblSubText->ForeColor = CLR_MUTED;
            lblSubText->Text = L"A comprehensive platform to manage donors,\nblood inventory, and life-saving requests.";
            lblSubText->Size = System::Drawing::Size(460, 58);
            lblSubText->Location = System::Drawing::Point(60, 322);    // overridden

            // CTA 1: "Login to Dashboard"  (solid crimson)
            btnLoginDash->Text = L"Login to Dashboard";
            btnLoginDash->Size = System::Drawing::Size(202, 52);
            btnLoginDash->FlatStyle = FlatStyle::Flat;
            btnLoginDash->FlatAppearance->BorderSize = 0;
            btnLoginDash->BackColor = CLR_ACCENT;
            btnLoginDash->ForeColor = Color::White;
            btnLoginDash->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            btnLoginDash->Cursor = Cursors::Hand;
            btnLoginDash->TabStop = false;
            btnLoginDash->Paint += gcnew PaintEventHandler(this, &LandingPage::btnSolid_Paint);
            btnLoginDash->MouseEnter += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
            btnLoginDash->MouseLeave += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
            btnLoginDash->Click += gcnew EventHandler(this, &LandingPage::btnLoginDash_Click);

            // CTA 2: "Learn More"  (outline ghost)
            btnLearnMore->Text = L"Learn More";
            btnLearnMore->Size = System::Drawing::Size(148, 52);
            btnLearnMore->FlatStyle = FlatStyle::Flat;
            btnLearnMore->FlatAppearance->BorderSize = 0;
            btnLearnMore->BackColor = Color::Transparent;
            btnLearnMore->ForeColor = CLR_ACCENT;
            btnLearnMore->Font = gcnew Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            btnLearnMore->Cursor = Cursors::Hand;
            btnLearnMore->TabStop = false;
            btnLearnMore->Paint += gcnew PaintEventHandler(this, &LandingPage::btnOutline_Paint);
            btnLearnMore->MouseEnter += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
            btnLearnMore->MouseLeave += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
            // btnLearnMore: no navigation in this milestone — visually interactive only

            // Stats row panel (transparent; separators drawn by Paint)
            statsPanel->Size = System::Drawing::Size(480, 70);
            statsPanel->Location = System::Drawing::Point(60, 490);    // overridden
            statsPanel->BackColor = Color::Transparent;
            statsPanel->Paint += gcnew PaintEventHandler(this, &LandingPage::statsPanel_Paint);

            // Stat 1 — Donors
            lblStat1Num->AutoSize = true;
            lblStat1Num->BackColor = Color::Transparent;
            lblStat1Num->Font = gcnew Drawing::Font(L"Segoe UI", 17, FontStyle::Bold);
            lblStat1Num->ForeColor = CLR_TEXT;
            lblStat1Num->Text = L"12,450";
            lblStat1Num->Location = System::Drawing::Point(0, 2);
            lblStat1Desc->AutoSize = true;
            lblStat1Desc->BackColor = Color::Transparent;
            lblStat1Desc->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Regular);
            lblStat1Desc->ForeColor = CLR_MUTED;
            lblStat1Desc->Text = L"Donors";
            lblStat1Desc->Location = System::Drawing::Point(0, 38);

            // Stat 2 — Units in Stock
            lblStat2Num->AutoSize = true;
            lblStat2Num->BackColor = Color::Transparent;
            lblStat2Num->Font = gcnew Drawing::Font(L"Segoe UI", 17, FontStyle::Bold);
            lblStat2Num->ForeColor = CLR_TEXT;
            lblStat2Num->Text = L"3,280";
            lblStat2Num->Location = System::Drawing::Point(162, 2);
            lblStat2Desc->AutoSize = true;
            lblStat2Desc->BackColor = Color::Transparent;
            lblStat2Desc->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Regular);
            lblStat2Desc->ForeColor = CLR_MUTED;
            lblStat2Desc->Text = L"Units in Stock";
            lblStat2Desc->Location = System::Drawing::Point(162, 38);

            // Stat 3 — Partner Hospitals
            lblStat3Num->AutoSize = true;
            lblStat3Num->BackColor = Color::Transparent;
            lblStat3Num->Font = gcnew Drawing::Font(L"Segoe UI", 17, FontStyle::Bold);
            lblStat3Num->ForeColor = CLR_TEXT;
            lblStat3Num->Text = L"47";
            lblStat3Num->Location = System::Drawing::Point(324, 2);
            lblStat3Desc->AutoSize = true;
            lblStat3Desc->BackColor = Color::Transparent;
            lblStat3Desc->Font = gcnew Drawing::Font(L"Segoe UI", 9, FontStyle::Regular);
            lblStat3Desc->ForeColor = CLR_MUTED;
            lblStat3Desc->Text = L"Partner Hospitals";
            lblStat3Desc->Location = System::Drawing::Point(324, 38);

            statsPanel->Controls->Add(lblStat1Num);   statsPanel->Controls->Add(lblStat1Desc);
            statsPanel->Controls->Add(lblStat2Num);   statsPanel->Controls->Add(lblStat2Desc);
            statsPanel->Controls->Add(lblStat3Num);   statsPanel->Controls->Add(lblStat3Desc);

            pillBadgePanel->Controls->Add(lblPillText);

            leftContentPanel->Controls->Add(pillBadgePanel);
            leftContentPanel->Controls->Add(lblHeading);
            leftContentPanel->Controls->Add(lblSubText);
            leftContentPanel->Controls->Add(btnLoginDash);
            leftContentPanel->Controls->Add(btnLearnMore);
            leftContentPanel->Controls->Add(statsPanel);

            // ── Right animation panel (Dock=Fill inside heroPanel) ──────
            rightAnimPanel->Dock = DockStyle::Fill;
            rightAnimPanel->BackColor = Color::Transparent;
            rightAnimPanel->Paint += gcnew PaintEventHandler(this, &LandingPage::rightAnimPanel_Paint);

            // Welcome text (bottom of the dark hero box)
            lblRightTitle->AutoSize = true;
            lblRightTitle->BackColor = Color::Transparent;
            lblRightTitle->Font = gcnew Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
            lblRightTitle->ForeColor = Color::White;
            lblRightTitle->Text = L"Welcome to LifeBlood MMS";
            lblRightTitle->Location = System::Drawing::Point(40, 430);     // overridden

            lblRightDesc->AutoSize = true;
            lblRightDesc->BackColor = Color::Transparent;
            lblRightDesc->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
            lblRightDesc->ForeColor = Color::FromArgb(210, 255, 255, 255);
            lblRightDesc->Text = L"Streamlining the connection between\ngenerous donors and critical needs.";
            lblRightDesc->Location = System::Drawing::Point(40, 464);     // overridden

            rightAnimPanel->Controls->Add(lblRightTitle);
            rightAnimPanel->Controls->Add(lblRightDesc);

            // ── Add to heroPanel: Fill first, Left last (reverse dock) ───
            heroPanel->Controls->Add(rightAnimPanel);       // Dock=Fill  → processed last
            heroPanel->Controls->Add(leftContentPanel);     // Dock=Left  → processed first

            // ────────────────────────────────────────────────────────────
            //  SECTION 3 · CARDS  (Dock=Top, H=300)
            //  Three white cards with icon · title · description · hover
            // ────────────────────────────────────────────────────────────
            cardsPanel->Dock = DockStyle::Top;
            cardsPanel->Height = 300;
            cardsPanel->BackColor = CLR_BG_FORM;
            cardsPanel->Paint += gcnew PaintEventHandler(this, &LandingPage::cardsPanel_Paint);

            // Emoji icon strings (supplementary-plane need ConvertFromUtf32)
            String^ iconPeople = Char::ConvertFromUtf32(0x1F465);   // 👥
            String^ iconShield = Char::ConvertFromUtf32(0x1F6E1);   // 🛡
            String^ iconHeart = L"\u2764";                          // ❤

            lblDescMgmt = SetupCard(cardManagement, iconPeople,
                L"Donor Management",
                L"Manage donors, track eligibility, and\nmaintain comprehensive health histories.");
            lblDescInv = SetupCard(cardInventory, iconShield,
                L"Safe Inventory",
                L"Real-time blood unit tracking with\nautomated expiry alerts across all 8 groups.");
            lblDescReq = SetupCard(cardRequests, iconHeart,
                L"Life-Saving Requests",
                L"Hospitals request blood; admins approve\nwith a single click. Inventory auto-updates.");

            // Add in reverse for correct left-to-right Z-order
            cardsPanel->Controls->Add(cardRequests);
            cardsPanel->Controls->Add(cardInventory);
            cardsPanel->Controls->Add(cardManagement);

            // ────────────────────────────────────────────────────────────
            //  SECTION 4 · FOOTER  (Dock=Bottom, H=80)
            //  Brand mark left · copyright text right
            // ────────────────────────────────────────────────────────────
            footerPanel->Dock = DockStyle::Bottom;
            footerPanel->Height = 80;
            footerPanel->BackColor = CLR_BG_CARD;
            footerPanel->Paint += gcnew PaintEventHandler(this, &LandingPage::footerPanel_Paint);

            lblFooterBrand->AutoSize = true;
            lblFooterBrand->BackColor = Color::Transparent;
            lblFooterBrand->Font = gcnew Drawing::Font(L"Segoe UI", 16, FontStyle::Bold);
            lblFooterBrand->ForeColor = CLR_TEXT;
            lblFooterBrand->Text = L"\u2665 LifeBlood";          // ♥ LifeBlood
            lblFooterBrand->Location = System::Drawing::Point(40, 28);    // overridden

            lblFooterCopy->AutoSize = true;
            lblFooterCopy->BackColor = Color::Transparent;
            lblFooterCopy->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
            lblFooterCopy->ForeColor = CLR_MUTED;
            lblFooterCopy->Text = L"\u00A9 2026 LifeBlood MMS. Every drop counts.";
            lblFooterCopy->Location = System::Drawing::Point(40, 28);    // overridden

            footerPanel->Controls->Add(lblFooterBrand);
            footerPanel->Controls->Add(lblFooterCopy);

            // ────────────────────────────────────────────────────────────
            //  CLOSE BUTTON  [✕]  (floating, Anchor=Top|Right)
            // ────────────────────────────────────────────────────────────
            btnClose->Text = Char::ConvertFromUtf32(0x2715);
            btnClose->Size = System::Drawing::Size(44, 44);
            btnClose->Anchor = static_cast<AnchorStyles>(AnchorStyles::Top | AnchorStyles::Right);
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->BackColor = Color::Transparent;
            btnClose->ForeColor = CLR_MUTED;
            btnClose->Font = gcnew Drawing::Font(L"Segoe UI", 13, FontStyle::Regular);
            btnClose->Cursor = Cursors::Hand;
            btnClose->TabStop = false;
            btnClose->Click += gcnew EventHandler(this, &LandingPage::btnClose_Click);
            btnClose->MouseEnter += gcnew EventHandler(this, &LandingPage::btnClose_MouseEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &LandingPage::btnClose_MouseLeave);

            // ────────────────────────────────────────────────────────────
            //  ADD CONTROLS TO FORM  (reverse dock order)
            //  Dock engine processes last-added first, so:
            //    topNavPanel (added last) → processed first → top of form
            //    footerPanel (added first) → processed last → Dock=Bottom
            // ────────────────────────────────────────────────────────────
            this->Controls->Add(footerPanel);    // Dock=Bottom (processed last by engine)
            this->Controls->Add(cardsPanel);     // Dock=Top  (3rd section)
            this->Controls->Add(heroPanel);      // Dock=Top  (2nd section)
            this->Controls->Add(topNavPanel);    // Dock=Top  (1st, processed first → very top)
            this->Controls->Add(btnClose);       // floating over everything

            // ── Resume layouts ─────────────────────────────────────────
            statsPanel->ResumeLayout(false);
            statsPanel->PerformLayout();
            pillBadgePanel->ResumeLayout(false);
            pillBadgePanel->PerformLayout();
            leftContentPanel->ResumeLayout(false);
            leftContentPanel->PerformLayout();
            rightAnimPanel->ResumeLayout(false);
            rightAnimPanel->PerformLayout();
            heroPanel->ResumeLayout(false);
            cardsPanel->ResumeLayout(false);
            topNavPanel->ResumeLayout(false);
            topNavPanel->PerformLayout();
            footerPanel->ResumeLayout(false);
            footerPanel->PerformLayout();
            this->ResumeLayout(false);
        }

#pragma endregion


        // ══════════════════════════════════════════════════════════════════
        //  FORM EVENTS
        // ══════════════════════════════════════════════════════════════════

    private: System::Void OnFormLoad(Object^ sender, EventArgs^ e)
    {
        // Enable double-buffered back-buffer on rightAnimPanel via Reflection.
        // Panel::DoubleBuffered is Protected — cannot set it directly.
        PropertyInfo^ pi = Panel::typeid->GetProperty(
            L"DoubleBuffered",
            BindingFlags::Instance | BindingFlags::NonPublic);
        if (pi != nullptr)
            pi->SetValue(rightAnimPanel, true, nullptr);

        PositionControls();
        btnClose->BringToFront();
    }

    private: System::Void OnFormResize(Object^ sender, EventArgs^ e)
    {
        PositionControls();
        btnClose->BringToFront();
    }


           // ══════════════════════════════════════════════════════════════════
           //  RESPONSIVE LAYOUT ENGINE
           //  Called on Load and every Resize.
           //  All pixel positions derive from panel sizes → scales to any resolution.
           // ══════════════════════════════════════════════════════════════════

    private: System::Void PositionControls()
    {
        int W = this->ClientSize.Width;
        int H = this->ClientSize.Height;
        if (W <= 0 || H <= 0) return;

        // ── Close button: one-time init; Anchor keeps it pinned on resize ─
        btnClose->Location = System::Drawing::Point(W - btnClose->Width - 2, 2);

        // ── Section 1: Nav links and Sign Up button ───────────────────────
        {
            int navH = topNavPanel->Height;   // 80
            int btnH = btnSignUpNav->Height;  // 42

            // Sign Up: right edge with extra space for the close button
            int closeBuffer = btnClose->Width + 16;
            btnSignUpNav->Location = System::Drawing::Point(W - btnSignUpNav->Width - 28 - closeBuffer,
                (navH - btnH) / 2);
            RoundControl(btnSignUpNav, btnSignUpNav->Height / 2);

            // Text links: lay out right-to-left before Sign Up
            array<Button^>^ links = gcnew array<Button^>{ btnNavContact, btnNavAbout, btnNavLogin, btnNavHome };
            int rightEdge = btnSignUpNav->Left - 12;
            for each (Button ^ b in links)
            {
                rightEdge -= b->Width;
                b->Location = System::Drawing::Point(rightEdge, (navH - b->Height) / 2);
                rightEdge -= 4;
            }
        }

        // ── Section 2: Hero split ─────────────────────────────────────────
        {
            int hW = heroPanel->Width;
            int hH = heroPanel->Height;   // 600
            if (hW <= 0 || hH <= 0) return;

            // ── Left panel: 56% of hero width ─────────────────────────────
            int leftW = static_cast<int>(hW * 0.56f);
            leftContentPanel->Width = leftW;
            heroPanel->PerformLayout();   // recalculate Dock=Left + Dock=Fill

            int padL = Math::Max(50, static_cast<int>(leftW * 0.10f));
            int midY = hH / 2;  // 300

            // Pill badge (centred vertically above heading)
            pillBadgePanel->Location = System::Drawing::Point(padL, midY - 218);

            // Heading: immediately below pill
            int hdgW = Math::Min(520, leftW - padL - 20);
            lblHeading->Size = System::Drawing::Size(hdgW, 182);
            lblHeading->Location = System::Drawing::Point(padL, pillBadgePanel->Bottom + 14);

            // Subtext: below heading
            int stW = Math::Min(460, leftW - padL - 20);
            lblSubText->Size = System::Drawing::Size(stW, 58);
            lblSubText->Location = System::Drawing::Point(padL, lblHeading->Bottom + 14);

            // CTA buttons: side by side below subtext
            btnLoginDash->Location = System::Drawing::Point(padL, lblSubText->Bottom + 26);
            btnLearnMore->Location = System::Drawing::Point(
                btnLoginDash->Right + 16, btnLoginDash->Top);
            RoundControl(btnLoginDash, btnLoginDash->Height / 2);
            RoundControl(btnLearnMore, btnLearnMore->Height / 2);

            // Stats row: below CTA buttons
            int spW = Math::Min(480, leftW - padL - 20);
            statsPanel->Size = System::Drawing::Size(spW, 70);
            statsPanel->Location = System::Drawing::Point(padL, btnLoginDash->Bottom + 24);

            // Responsive stat column positions
            int col = spW / 3;
            lblStat1Num->Location = System::Drawing::Point(0, 2);
            lblStat1Desc->Location = System::Drawing::Point(0, 38);
            lblStat2Num->Location = System::Drawing::Point(col, 2);
            lblStat2Desc->Location = System::Drawing::Point(col, 38);
            lblStat3Num->Location = System::Drawing::Point(col * 2, 2);
            lblStat3Desc->Location = System::Drawing::Point(col * 2, 38);
            statsPanel->Invalidate();

            // ── Right panel: rebuild rounded clip region and text positions ──
            Rectangle box = GetRightPanelBoxRect();
            if (box.Width > 10 && box.Height > 10)
            {
                // Welcome text near the bottom of the dark hero box
                lblRightTitle->MaximumSize = System::Drawing::Size(box.Width - 80, 0);
                lblRightTitle->Location = System::Drawing::Point(box.X + 40, box.Bottom - 110);
                lblRightDesc->MaximumSize = System::Drawing::Size(box.Width - 80, 0);
                lblRightDesc->Location = System::Drawing::Point(box.X + 40, box.Bottom - 64);

                // Rounded clip region (radius = 36) applied to the panel
                int cr = 36, d = cr * 2;
                GraphicsPath^ rgn = gcnew GraphicsPath();
                rgn->AddArc(Rectangle(box.X, box.Y, d, d), 180, 90);
                rgn->AddArc(Rectangle(box.Right - d, box.Y, d, d), 270, 90);
                rgn->AddArc(Rectangle(box.Right - d, box.Bottom - d, d, d), 0, 90);
                rgn->AddArc(Rectangle(box.X, box.Bottom - d, d, d), 90, 90);
                rgn->CloseFigure();
                rightAnimPanel->Region = gcnew System::Drawing::Region(rgn);
                delete rgn;
            }
        }

        // ── Section 3: Feature cards ─────────────────────────────────────
        {
            int bW = cardsPanel->Width;
            int bH = cardsPanel->Height;   // 300
            if (bW <= 0 || bH <= 0) return;

            const int CARD_H = 196;
            const int CARD_GAP = 28;
            int cardW = Math::Max(220, (bW - 80 - CARD_GAP * 2) / 3);
            int totalW = cardW * 3 + CARD_GAP * 2;
            int startX = (bW - totalW) / 2;
            cardRestY = (bH - CARD_H) / 2;   // vertical centre; hover logic reads this

            cardManagement->Size = System::Drawing::Size(cardW, CARD_H);
            cardInventory->Size = System::Drawing::Size(cardW, CARD_H);
            cardRequests->Size = System::Drawing::Size(cardW, CARD_H);

            cardManagement->Location = System::Drawing::Point(startX, cardRestY);
            cardInventory->Location = System::Drawing::Point(startX + cardW + CARD_GAP, cardRestY);
            cardRequests->Location = System::Drawing::Point(startX + 2 * (cardW + CARD_GAP), cardRestY);

            int descW = cardW - 40;
            if (lblDescMgmt != nullptr) lblDescMgmt->MaximumSize = System::Drawing::Size(descW, 0);
            if (lblDescInv != nullptr) lblDescInv->MaximumSize = System::Drawing::Size(descW, 0);
            if (lblDescReq != nullptr) lblDescReq->MaximumSize = System::Drawing::Size(descW, 0);

            RoundControl(cardManagement, 14);
            RoundControl(cardInventory, 14);
            RoundControl(cardRequests, 14);
        }

        // ── Section 4: Footer labels ─────────────────────────────────────
        {
            int fW = footerPanel->Width;
            int fH = footerPanel->Height;   // 80
            int midFY = (fH - lblFooterBrand->PreferredHeight) / 2;
            lblFooterBrand->Location = System::Drawing::Point(40, midFY);
            // Footer copyright auto-sizes; position once after PreferredWidth is known
            lblFooterCopy->Location = System::Drawing::Point(
                fW - lblFooterCopy->PreferredWidth - 40,
                (fH - lblFooterCopy->PreferredHeight) / 2);
        }

        rightAnimPanel->Invalidate();
        cardsPanel->Invalidate();
    }


           // ── Inset box rectangle within rightAnimPanel ─────────────────────
           //    Provides breathing room around the dark hero box so the light
           //    form background shows in the gap — creating the "floating" look.
    private: Rectangle GetRightPanelBoxRect()
    {
        int pW = rightAnimPanel->Width;
        int pH = rightAnimPanel->Height;
        int bW = Math::Max(1, pW - RP_RIGHT);
        int bH = Math::Max(1, pH - RP_TOP - RP_BOTTOM);
        return Rectangle(0, RP_TOP, bW, bH);
    }


           // ══════════════════════════════════════════════════════════════════
           //  ANIMATION ENGINE  (preserved from homeForm.h)
           // ══════════════════════════════════════════════════════════════════

           // ── Timer tick: advance orbit angles then request repaint ─────────
    private: System::Void OnAnimTick(Object^ sender, EventArgs^ e)
    {
        angleO -= 0.020f;
        angleA -= 0.025f;
        angleB -= 0.015f;
        angleAB -= 0.018f;
        rightAnimPanel->Invalidate();
    }

           // ── Right-panel paint: gradient + vignette + GDI+ animation ─────
    private: System::Void rightAnimPanel_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        Rectangle box = GetRightPanelBoxRect();
        if (box.Width <= 0 || box.Height <= 0) return;

        // Dark crimson diagonal gradient background
        LinearGradientBrush^ bgBrush = gcnew LinearGradientBrush(
            box,
            Color::FromArgb(210, 10, 45),
            Color::FromArgb(60, 20, 35),
            LinearGradientMode::ForwardDiagonal);
        g->FillRectangle(bgBrush, box);
        delete bgBrush;

        // Radial vignette overlay for depth
        int vcx = box.X + box.Width / 2;
        int vcy = box.Y + box.Height / 2;
        int vr = Math::Max(box.Width, box.Height);
        GraphicsPath^ ringPath = gcnew GraphicsPath();
        ringPath->AddEllipse(vcx - vr, vcy - vr, vr * 2, vr * 2);
        PathGradientBrush^ vignette = gcnew PathGradientBrush(ringPath);
        vignette->CenterColor = Color::Transparent;
        vignette->SurroundColors = gcnew array<Color> { Color::FromArgb(70, 0, 0, 0) };
        g->FillEllipse(vignette, vcx - vr, vcy - vr, vr * 2, vr * 2);
        delete vignette;
        delete ringPath;

        // Animation geometry derived from box size
        int cx = box.X + box.Width / 2;
        int cy = box.Y + box.Height / 2 - 28;
        int baseSize = Math::Min(box.Width, box.Height);
        int mainSize = Math::Max(72, baseSize / 4);
        int orbitRadius = Math::Max(110, baseSize / 3);

        // 1. Large central blood drop emoji with white outline
        DrawOutlinedBloodEmoji(g, cx, cy, mainSize, Color::FromArgb(210, 170, 35, 50), Color::White);

        // 2. Four orbiting blood-type drops at different radii and speeds
        DrawOrbitingDrop(g, cx, cy, orbitRadius, angleO, L"O\u2212");   // O-
        DrawOrbitingDrop(g, cx, cy, orbitRadius + 28, angleA, L"A+");
        DrawOrbitingDrop(g, cx, cy, orbitRadius - 18, angleB, L"B+");
        DrawOrbitingDrop(g, cx, cy, orbitRadius + 44, angleAB, L"AB+");
    }

           // ── Orbiting blood-type drop + label ─────────────────────────────
    private: System::Void DrawOrbitingDrop(Graphics^ g, int cx, int cy,
        int radius, float angle, String^ text)
    {
        int x = cx + static_cast<int>(radius * std::cos(angle));
        int y = cy + static_cast<int>(radius * std::sin(angle));

        String^ dropEmoji = Char::ConvertFromUtf32(0x1FA78);
        Drawing::Font^ emojiFont = gcnew Drawing::Font(L"Segoe UI Emoji", 12, FontStyle::Regular);
        Drawing::Font^ textFont = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
        System::Drawing::Size emojiSize = TextRenderer::MeasureText(dropEmoji, emojiFont);
        System::Drawing::Size textSize = TextRenderer::MeasureText(text, textFont);

        int padX = 12;
        int padY = 6;
        int pillW = emojiSize.Width + textSize.Width + padX * 2 - 4;
        int pillH = Math::Max(emojiSize.Height, textSize.Height) + padY * 2 - 2;
        Rectangle pillRect = Rectangle(x - pillW / 2, y - pillH / 2, pillW, pillH);

        GraphicsPath^ pillPath = CreateRoundedPath(pillRect, pillH / 2);
        SolidBrush^ pillFill = gcnew SolidBrush(Color::FromArgb(70, 255, 255, 255));
        Pen^ pillBorder = gcnew Pen(Color::FromArgb(120, 255, 255, 255), 1.0f);
        g->FillPath(pillFill, pillPath);
        g->DrawPath(pillBorder, pillPath);
        delete pillBorder;
        delete pillFill;
        delete pillPath;

        int emojiX = pillRect.X + padX - 2;
        int emojiY = pillRect.Y + (pillRect.Height - emojiSize.Height) / 2;
        Rectangle emojiRect = Rectangle(emojiX, emojiY, emojiSize.Width, emojiSize.Height);
        TextRenderer::DrawText(g, dropEmoji, emojiFont, emojiRect, Color::White,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);

        int textX = emojiRect.Right + 4;
        int textY = pillRect.Y + (pillRect.Height - textSize.Height) / 2;
        Rectangle textRect = Rectangle(textX, textY, textSize.Width, textSize.Height);
        TextRenderer::DrawText(g, text, textFont, textRect, Color::White,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);

        delete textFont;
        delete emojiFont;
    }

           // ── Main blood drop emoji with outline ─────────────────────────-
    private: System::Void DrawOutlinedBloodEmoji(Graphics^ g, int cx, int cy, int size, Color fillColor, Color outlineColor)
    {
        String^ dropEmoji = Char::ConvertFromUtf32(0x1FA78);
        Drawing::Font^ font = gcnew Drawing::Font(L"Segoe UI Emoji", static_cast<float>(size), FontStyle::Regular);
        System::Drawing::Size sz = TextRenderer::MeasureText(dropEmoji, font);
        Rectangle rect = Rectangle(cx - sz.Width / 2, cy - sz.Height / 2, sz.Width, sz.Height);

        for (int ox = -2; ox <= 2; ox++)
        {
            for (int oy = -2; oy <= 2; oy++)
            {
                if (ox == 0 && oy == 0) continue;
                Rectangle outlineRect = Rectangle(rect.X + ox, rect.Y + oy, rect.Width, rect.Height);
                TextRenderer::DrawText(g, dropEmoji, font, outlineRect, outlineColor,
                    TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);
            }
        }
        TextRenderer::DrawText(g, dropEmoji, font, rect, fillColor,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);
        delete font;
    }


           // ══════════════════════════════════════════════════════════════════
           //  PAINT HANDLERS
           // ══════════════════════════════════════════════════════════════════

           // ── Top nav: 1 px bottom separator ───────────────────────────────
    private: System::Void topNavPanel_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Pen^ sep = gcnew Pen(Color::FromArgb(220, 222, 226), 1.0f);
        e->Graphics->DrawLine(sep, 0, pnl->Height - 1, pnl->Width, pnl->Height - 1);
        delete sep;
    }

           // ── Cards section: 1 px top separator ────────────────────────────
    private: System::Void cardsPanel_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Pen^ sep = gcnew Pen(Color::FromArgb(220, 222, 226), 1.0f);
        e->Graphics->DrawLine(sep, 0, 0, pnl->Width, 0);
        delete sep;
    }

           // ── Footer: 1 px top separator ────────────────────────────────────
    private: System::Void footerPanel_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Pen^ sep = gcnew Pen(Color::FromArgb(220, 222, 226), 1.0f);
        e->Graphics->DrawLine(sep, 0, 0, pnl->Width, 0);
        delete sep;
    }

           // ── Logo badge: rounded square + filled crimson blood drop ────────
    private: System::Void logoBadge_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        Rectangle rect = Rectangle(0, 0, pnl->Width - 1, pnl->Height - 1);

        // Pale crimson badge background
        GraphicsPath^ bgPath = CreateRoundedPath(rect, 10);
        SolidBrush^ bgBrush = gcnew SolidBrush(Color::FromArgb(245, 232, 236));
        g->FillPath(bgBrush, bgPath);
        delete bgBrush; delete bgPath;

        String^ dropEmoji = Char::ConvertFromUtf32(0x1FA78);
        Drawing::Font^ dropFont = gcnew Drawing::Font(L"Segoe UI Emoji", 14, FontStyle::Regular);
        System::Drawing::Size sz = TextRenderer::MeasureText(dropEmoji, dropFont);
        Rectangle textRect = Rectangle((pnl->Width - sz.Width) / 2 + 2, (pnl->Height - sz.Height) / 2 + 1, sz.Width, sz.Height);
        TextRenderer::DrawText(g, dropEmoji, dropFont, textRect, CLR_ACCENT,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);
        delete dropFont;
    }

           // ── Pill badge: fully-rounded rect with light-pink fill ───────────
    private: System::Void pillBadge_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        Rectangle     rect = Rectangle(0, 0, pnl->Width - 1, pnl->Height - 1);
        int           r = rect.Height / 2;     // fully rounded → pill shape
        GraphicsPath^ path = CreateRoundedPath(rect, r);

        SolidBrush^ bg = gcnew SolidBrush(CLR_PINK_LIGHT);
        g->FillPath(bg, path);
        delete bg;

        Pen^ border = gcnew Pen(Color::FromArgb(80, CLR_ACCENT.R, CLR_ACCENT.G, CLR_ACCENT.B), 1.0f);
        g->DrawPath(border, path);
        delete border; delete path;
    }

           // ── Stats row: vertical dividers between the three stat columns ───
    private: System::Void statsPanel_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Graphics^ g = e->Graphics;

        if (pnl->Width < 3) return;
        int col = pnl->Width / 3;
        Pen^ sep = gcnew Pen(Color::FromArgb(208, 210, 215), 1.0f);
        g->DrawLine(sep, col - 8, 6, col - 8, pnl->Height - 6);
        g->DrawLine(sep, col * 2 - 8, 6, col * 2 - 8, pnl->Height - 6);
        delete sep;
    }

           // ── Nav text link: CLR_TEXT text, accent + underline on hover ─────
    private: System::Void btnNavText_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        bool  isHover = btn->ClientRectangle.Contains(btn->PointToClient(Control::MousePosition));
        Color textColor = isHover ? CLR_ACCENT : CLR_TEXT;

        TextRenderer::DrawText(g, btn->Text, btn->Font, btn->ClientRectangle,
            textColor,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);

        if (isHover)
        {
            System::Drawing::Size sz = TextRenderer::MeasureText(btn->Text, btn->Font);
            int ulX = (btn->Width - sz.Width) / 2;
            int ulY = btn->Height - 5;
            SolidBrush^ ul = gcnew SolidBrush(CLR_ACCENT);
            g->FillRectangle(ul, ulX, ulY, sz.Width, 2);
            delete ul;
        }
    }

           // ── Solid crimson button: rounded rect fill + top-shine + text ────
    private: System::Void btnSolid_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        g->PixelOffsetMode = PixelOffsetMode::HighQuality;
        Rectangle rect = Rectangle(1, 1, btn->Width - 3, btn->Height - 3);
        if (rect.Width <= 0 || rect.Height <= 0) return;

        bool isHover = btn->ClientRectangle.Contains(btn->PointToClient(Control::MousePosition));

        int radius = rect.Height / 2;
        GraphicsPath^ path = CreateRoundedPath(rect, radius);

        // Slightly darker on hover
        Color fillColor = isHover
            ? Color::FromArgb(0xA8, 0x31, 0x25)
            : CLR_ACCENT;
        SolidBrush^ fill = gcnew SolidBrush(fillColor);
        g->FillPath(fill, path);
        delete fill;

        // Subtle top-half shine highlight
        Rectangle     shineRect = Rectangle(rect.X + 1, rect.Y + 1,
            rect.Width - 2, rect.Height / 2);
        GraphicsPath^ shinePath = CreateRoundedPath(shineRect, 9);
        SolidBrush^ shine = gcnew SolidBrush(Color::FromArgb(28, 255, 255, 255));
        g->FillPath(shine, shinePath);
        delete shine; delete shinePath;

        TextRenderer::DrawText(g, btn->Text, btn->Font, rect,
            Color::White,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);

        delete path;
    }

           // ── Outline ghost button: accent border only, light tint on hover ─
    private: System::Void btnOutline_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Button^ btn = safe_cast<Button^>(sender);
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        Rectangle rect = Rectangle(0, 0, btn->Width - 1, btn->Height - 1);
        if (rect.Width <= 0 || rect.Height <= 0) return;

        bool isHover = btn->ClientRectangle.Contains(btn->PointToClient(Control::MousePosition));

        int radius = rect.Height / 2;
        GraphicsPath^ path = CreateRoundedPath(rect, radius);

        if (isHover)
        {
            SolidBrush^ hoverFill = gcnew SolidBrush(
                Color::FromArgb(14, CLR_ACCENT.R, CLR_ACCENT.G, CLR_ACCENT.B));
            g->FillPath(hoverFill, path);
            delete hoverFill;
        }

        Pen^ border = gcnew Pen(CLR_ACCENT, 1.6f);
        g->DrawPath(border, path);
        delete border;

        TextRenderer::DrawText(g, btn->Text, btn->Font, rect,
            CLR_ACCENT,
            TextFormatFlags::HorizontalCenter | TextFormatFlags::VerticalCenter);

        delete path;
    }

           // ── Feature card: white fill + accent stripe + stacked shadow ─────
           //    isHover is inferred from pnl->Top vs cardRestY (avoids extra state).
    private: System::Void Card_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Panel^ pnl = safe_cast<Panel^>(sender);
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        Rectangle rect = Rectangle(0, 0, pnl->Width - 1, pnl->Height - 1);
        if (rect.Width <= 0 || rect.Height <= 0) return;

        bool isHover = (pnl->Top == cardRestY - 5);

        // Multi-layer soft drop shadow
        int shadowDepth = isHover ? 6 : 3;
        for (int i = shadowDepth; i >= 1; i--)
        {
            int           alpha = Math::Max(4, 20 - i * 3);
            SolidBrush^ sb = gcnew SolidBrush(Color::FromArgb(alpha, 0, 0, 0));
            Rectangle     sRect = Rectangle(rect.X + i, rect.Y + i, rect.Width - i, rect.Height - i);
            GraphicsPath^ sp = CreateRoundedPath(sRect, 14);
            g->FillPath(sb, sp);
            delete sp; delete sb;
        }

        // White card fill
        GraphicsPath^ path = CreateRoundedPath(rect, 14);
        SolidBrush^ fill = gcnew SolidBrush(CLR_BG_CARD);
        g->FillPath(fill, path);
        delete fill;

        // 4 px crimson accent stripe at top
        SolidBrush^ stripe = gcnew SolidBrush(CLR_ACCENT);
        g->FillRectangle(stripe, Rectangle(rect.X, rect.Y, rect.Width, 4));
        delete stripe;

        // Border: accent tint on hover, light gray at rest
        Color borderColor = isHover
            ? Color::FromArgb(110, CLR_ACCENT.R, CLR_ACCENT.G, CLR_ACCENT.B)
            : Color::FromArgb(210, 212, 215);
        Pen^ borderPen = gcnew Pen(borderColor, 1.0f);
        g->DrawPath(borderPen, path);
        delete borderPen; delete path;
    }


           // ══════════════════════════════════════════════════════════════════
           //  CARD SETUP + HOVER ANIMATION
           // ══════════════════════════════════════════════════════════════════

           // ── Build one feature card; returns desc Label for width updates ──
    private: Label^ SetupCard(Panel^ pnl, String^ icon, String^ title, String^ desc)
    {
        pnl->BackColor = Color::Transparent;
        pnl->Cursor = Cursors::Hand;
        pnl->Paint += gcnew PaintEventHandler(this, &LandingPage::Card_Paint);

        // Icon (emoji rendered via Segoe UI Emoji)
        Label^ lblIcon = gcnew Label();
        lblIcon->Text = icon;
        lblIcon->BackColor = Color::Transparent;
        lblIcon->Font = gcnew Drawing::Font(L"Segoe UI Emoji", 22, FontStyle::Regular);
        lblIcon->ForeColor = CLR_ACCENT;
        lblIcon->Location = System::Drawing::Point(20, 18);
        lblIcon->AutoSize = true;

        // Title
        Label^ lblTitle = gcnew Label();
        lblTitle->Text = title;
        lblTitle->BackColor = Color::Transparent;
        lblTitle->Font = gcnew Drawing::Font(L"Segoe UI", 13, FontStyle::Bold);
        lblTitle->ForeColor = CLR_TEXT;
        lblTitle->Location = System::Drawing::Point(20, 60);
        lblTitle->AutoSize = true;

        // Description
        Label^ lblDesc = gcnew Label();
        lblDesc->Text = desc;
        lblDesc->BackColor = Color::Transparent;
        lblDesc->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
        lblDesc->ForeColor = CLR_MUTED;
        lblDesc->Location = System::Drawing::Point(20, 98);
        lblDesc->AutoSize = true;
        lblDesc->MaximumSize = System::Drawing::Size(240, 0);

        pnl->Controls->Add(lblIcon);
        pnl->Controls->Add(lblTitle);
        pnl->Controls->Add(lblDesc);

        // Propagate hover events from every child label → parent panel
        pnl->MouseEnter += gcnew EventHandler(this, &LandingPage::Card_MouseEnter);
        pnl->MouseLeave += gcnew EventHandler(this, &LandingPage::Card_MouseLeave);
        lblIcon->MouseEnter += gcnew EventHandler(this, &LandingPage::Card_MouseEnter);
        lblIcon->MouseLeave += gcnew EventHandler(this, &LandingPage::Card_MouseLeave);
        lblTitle->MouseEnter += gcnew EventHandler(this, &LandingPage::Card_MouseEnter);
        lblTitle->MouseLeave += gcnew EventHandler(this, &LandingPage::Card_MouseLeave);
        lblDesc->MouseEnter += gcnew EventHandler(this, &LandingPage::Card_MouseEnter);
        lblDesc->MouseLeave += gcnew EventHandler(this, &LandingPage::Card_MouseLeave);

        return lblDesc;
    }

           // ── Card lift: move up 5 px from rest position ────────────────────
    private: System::Void Card_MouseEnter(Object^ sender, EventArgs^ e)
    {
        Control^ ctrl = safe_cast<Control^>(sender);
        Panel^ pnl = (ctrl->GetType() == Panel::typeid)
            ? safe_cast<Panel^>(ctrl)
            : safe_cast<Panel^>(ctrl->Parent);
        if (pnl->Top == cardRestY)
        {
            pnl->Top = cardRestY - 5;
            pnl->Invalidate();
        }
    }

           // ── Card restore: return to rest only when mouse leaves the panel ─
    private: System::Void Card_MouseLeave(Object^ sender, EventArgs^ e)
    {
        Control^ ctrl = safe_cast<Control^>(sender);
        Panel^ pnl = (ctrl->GetType() == Panel::typeid)
            ? safe_cast<Panel^>(ctrl)
            : safe_cast<Panel^>(ctrl->Parent);
        Point mousePos = pnl->PointToClient(Control::MousePosition);
        if (!pnl->ClientRectangle.Contains(mousePos))
        {
            pnl->Top = cardRestY;
            pnl->Invalidate();
        }
    }

           // ── Generic button invalidate on hover (re-triggers custom Paint) ─
    private: System::Void BtnInvalidate(Object^ sender, EventArgs^ e)
    {
        safe_cast<Button^>(sender)->Invalidate();
    }


           // ══════════════════════════════════════════════════════════════════
           //  SHARED GDI+ HELPERS
           // ══════════════════════════════════════════════════════════════════

           // ── Build a rounded-rectangle GraphicsPath.
           //    CALLER must delete the returned object. ─────────────────────
    private: GraphicsPath^ CreateRoundedPath(Rectangle rect, int radius)
    {
        GraphicsPath^ path = gcnew GraphicsPath();
        int d = radius * 2;
        path->AddArc(Rectangle(rect.X, rect.Y, d, d), 180, 90);
        path->AddArc(Rectangle(rect.Right - d, rect.Y, d, d), 270, 90);
        path->AddArc(Rectangle(rect.Right - d, rect.Bottom - d, d, d), 0, 90);
        path->AddArc(Rectangle(rect.X, rect.Bottom - d, d, d), 90, 90);
        path->CloseFigure();
        return path;
    }

           // ── Apply a rounded-rectangle Region to any Control ──────────────
    private: System::Void RoundControl(Control^ ctrl, int radius)
    {
        GraphicsPath^ path = CreateRoundedPath(
            Rectangle(0, 0, ctrl->Width, ctrl->Height), radius);
        ctrl->Region = gcnew System::Drawing::Region(path);
        delete path;
    }

           // ── Configure a flat nav text-link button ─────────────────────────
    private: System::Void SetupNavLink(Button^ btn, String^ text, int width)
    {
        btn->Text = text;
        btn->Size = System::Drawing::Size(width, 44);
        btn->FlatStyle = FlatStyle::Flat;
        btn->FlatAppearance->BorderSize = 0;
        btn->BackColor = Color::Transparent;
        btn->ForeColor = CLR_TEXT;
        btn->Font = gcnew Drawing::Font(L"Segoe UI", 10, FontStyle::Regular);
        btn->Cursor = Cursors::Hand;
        btn->TabStop = false;
        btn->Paint += gcnew PaintEventHandler(this, &LandingPage::btnNavText_Paint);
        btn->MouseEnter += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
        btn->MouseLeave += gcnew EventHandler(this, &LandingPage::BtnInvalidate);
    }


           // ══════════════════════════════════════════════════════════════════
           //  NAVIGATION  —  child form wiring
           // ══════════════════════════════════════════════════════════════════

           // ── "Login to Dashboard" + "Login" nav link ───────────────────────
    private: System::Void btnLoginDash_Click(Object^ sender, EventArgs^ e)
    {
        this->Hide();
        LoginForm^ lf = gcnew LoginForm();
        lf->FormClosed += gcnew FormClosedEventHandler(this, &LandingPage::OnChildClosed);
        lf->Show();
    }

           // ── "Sign Up" nav button ──────────────────────────────────────────
    private: System::Void btnSignUp_Click(Object^ sender, EventArgs^ e)
    {
        this->Hide();
        SignupForm^ sf = gcnew SignupForm();
        sf->FormClosed += gcnew FormClosedEventHandler(this, &LandingPage::OnChildClosed);
        sf->Show();
    }

           // ── Child form closed: restore this landing page ───────────────────
           //    Fires for both LoginForm and SignupForm regardless of close reason.
    private: System::Void OnChildClosed(Object^ sender, FormClosedEventArgs^ e)
    {
        this->Show();
    }


           // ══════════════════════════════════════════════════════════════════
           //  CLOSE BUTTON
           // ══════════════════════════════════════════════════════════════════

    private: System::Void btnClose_Click(Object^ sender, EventArgs^ e)
    {
        animTimer->Stop();
        Application::Exit();
    }

    private: System::Void btnClose_MouseEnter(Object^ sender, EventArgs^ e)
    {
        btnClose->ForeColor = CLR_ACCENT;   // crimson tint signals danger
    }

    private: System::Void btnClose_MouseLeave(Object^ sender, EventArgs^ e)
    {
        btnClose->ForeColor = CLR_MUTED;
    }

    };  // end class LandingPage
}     // end namespace BloodBank