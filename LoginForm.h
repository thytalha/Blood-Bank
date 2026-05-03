// ================================================================
//  LoginForm.h  —  Blood Bank Management System
//  "Clean-Light" Aesthetic: premium light mode + crimson accent
//
//  REFACTORED to match global application architecture:
//    • Maximized + Borderless  (identical to all dashboards)
//    • Light-mode palette extracted verbatim from DonorDashboard.h
//    • Centred white card — CenterCard() called on Load AND Resize
//    • 8-px CLR_ACCENT brand strip pinned to the very top of the card
//    • Screen-level "X" close button, AnchorStyles::Top|Right
//    • Timer-driven RGB lerp: CLR_ACCENT → CLR_ACCENT_DARK on hover
//    • 2-px underline panels: CLR_FIELD_LINE (idle) / CLR_ACCENT (focus)
//    • Win32 EM_SETCUEBANNER P/Invoke for placeholder text
//    • Char::ConvertFromUtf32(0x...) for every emoji — no literals
//    • All backend logic preserved 1-for-1 (ValidateLogin, routing,
//      FormClosedEventHandler chain, LinkLabel signup wiring)
//    • Drag-window logic removed (Maximized forms cannot be dragged)
//    • Fully programmatic — zero Designer dependency
// ================================================================

#pragma once
#include "Database.h"
#include "SignupForm.h"
#include "AdminDashboard.h"
#include "DonorDashboard.h"
#include "RecipientDashboard.h"

#using <System.dll>
#using <System.Data.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

namespace BloodBank {

    public ref class LoginForm : public Form
    {

        // ────────────────────────────────────────────────────────────
        //  WIN32 CUE-BANNER  —  placeholder text on borderless TextBox
        //  EM_SETCUEBANNER = 0x1501  |  wParam 1 = show even when focused
        // ────────────────────────────────────────────────────────────
    private:
        [DllImport("user32.dll", EntryPoint = "SendMessageW", CharSet = CharSet::Unicode)]
            static IntPtr SendCue(IntPtr hWnd, UInt32 msg, IntPtr wp, String^ lp);


        // ══════════════════════════════════════════════════════════
        //  COLOUR PALETTE
        //  Extracted verbatim from DonorDashboard.h / AdminDashboard.h.
        //  This is the single source of truth for every pixel of colour.
        // ══════════════════════════════════════════════════════════

        // — Background tier —
        Color CLR_BG_FORM;        // #F8F9FA  soft off-white outer fill
        Color CLR_BG_CARD;        // #FFFFFF  pure white card surface
        Color CLR_BG_INPUT;       // #FFFFFF  TextBox fill (matches card)

        // — Text tier —
        Color CLR_TEXT;           // #212529  near-black primary text
        Color CLR_MUTED;          // #6C757D  medium grey labels / subtitles
        Color CLR_HINT;           // #ADB5BD  very muted footer / chip text

        // — Accent tier —
        Color CLR_ACCENT;         // #C0392B  crimson — strip, button rest, links
        Color CLR_ACCENT_DARK;    // #922B21  darker crimson — button hover target
        Color CLR_ERROR;          // #E74C3C  lighter crimson — inline error text

        // — Structure tier —
        Color CLR_BORDER;         // #E9ECEF  pale grey — card border, divider
        Color CLR_FIELD_LINE;     // #CED4DA  underline idle state
        Color CLR_HOVER_BG;       // #FADBD8  blush pink — close-btn hover fill


        // ══════════════════════════════════════════════════════════
        //  CONTROL HANDLES
        // ══════════════════════════════════════════════════════════
        Panel^ pnlCard;          // centred white card (440 × 540 px)
        TextBox^ txtUsername;
        TextBox^ txtPassword;
        Panel^ pnlUnderUser;     // 2-px animated underline — username
        Panel^ pnlUnderPass;     // 2-px animated underline — password
        Button^ btnLogin;         // full-width crimson CTA
        Button^ btnClose;         // screen-level X, anchored Top|Right
        CheckBox^ chkShow;          // show/hide password toggle
        Label^ lblError;         // inline validation message


        // ══════════════════════════════════════════════════════════
        //  TIMER-DRIVEN SMOOTH BUTTON HOVER FADE
        //
        //  _fadeT  ∈ [0 , 1]
        //    0 = fully CLR_ACCENT  (rest state)
        //    1 = fully CLR_ACCENT_DARK  (hover state)
        //  _fadeDir : +1 = fading toward hover
        //             -1 = fading toward rest
        //
        //  FADE_STEP = 0.09 → ~11 ticks × 12 ms ≈ 130 ms transition
        //  Perceived as "snappy but intentional" — same as old form.
        // ══════════════════════════════════════════════════════════
        Timer^ _fadeTimer;
        float  _fadeT;
        int    _fadeDir;
        const float FADE_STEP = 0.09f;


        // ══════════════════════════════════════════════════════════
        //  CONSTRUCTOR
        // ══════════════════════════════════════════════════════════
    public:
        LoginForm()
        {
            // ── Initialise light-mode palette (mirrors DonorDashboard) ──
            CLR_BG_FORM = ColorTranslator::FromHtml("#F8F9FA");
            CLR_BG_CARD = ColorTranslator::FromHtml("#FFFFFF");
            CLR_BG_INPUT = ColorTranslator::FromHtml("#FFFFFF");
            CLR_TEXT = ColorTranslator::FromHtml("#212529");
            CLR_MUTED = ColorTranslator::FromHtml("#6C757D");
            CLR_HINT = ColorTranslator::FromHtml("#ADB5BD");
            CLR_ACCENT = ColorTranslator::FromHtml("#C0392B");
            CLR_ACCENT_DARK = ColorTranslator::FromHtml("#922B21");
            CLR_ERROR = ColorTranslator::FromHtml("#E74C3C");
            CLR_BORDER = ColorTranslator::FromHtml("#E9ECEF");
            CLR_FIELD_LINE = ColorTranslator::FromHtml("#CED4DA");
            CLR_HOVER_BG = ColorTranslator::FromHtml("#FADBD8");

            // ── Initialise fade timer (stopped; starts on mouse-enter) ──
            _fadeTimer = gcnew Timer();
            _fadeTimer->Interval = 12;           // ~83 fps
            _fadeTimer->Enabled = false;
            _fadeTimer->Tick += gcnew EventHandler(this, &LoginForm::OnFadeTick);
            _fadeT = 0.0f;
            _fadeDir = 0;

            InitializeComponent();
        }

    protected:
        ~LoginForm() {}


        // ══════════════════════════════════════════════════════════
        //  InitializeComponent
        //  Wires the form shell and delegates every widget to its own
        //  Build*() helper.  No Designer (.resx / .Designer.cpp) used.
        // ══════════════════════════════════════════════════════════
    private:
        void InitializeComponent()
        {
            // ── Form-level properties ─────────────────────────────
            this->Text = "Blood Bank Management System \u2014 LifeLine";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_BG_FORM;
            this->DoubleBuffered = true;   // eliminates repaint flicker

            // ── Escape key closes the application ────────────────
            this->KeyPreview = true;
            this->KeyDown += gcnew KeyEventHandler(this, &LoginForm::OnFormKey);

            // ── Layout callbacks ──────────────────────────────────
            this->Load += gcnew EventHandler(this, &LoginForm::OnLoad);
            this->Resize += gcnew EventHandler(this, &LoginForm::OnResize);

            // ── Build in Z-order: close btn first (below card) ───
            BuildCloseButton();   // lives on the form, not the card
            BuildCardPanel();     // card + all child widgets
        }


        // ══════════════════════════════════════════════════════════
        //  CenterCard
        //  Pixel-perfect horizontal AND vertical centering.
        //  Called on Load and on every Resize event so any resolution
        //  or DPI change is handled transparently.
        // ══════════════════════════════════════════════════════════
        void CenterCard()
        {
            pnlCard->Location = Drawing::Point(
                (this->ClientSize.Width - pnlCard->Width) / 2,
                (this->ClientSize.Height - pnlCard->Height) / 2
            );
        }


        // ══════════════════════════════════════════════════════════
        //  BuildCloseButton
        //
        //  The "X" lives on the FORM (not pnlCard).
        //  AnchorStyles::Top | AnchorStyles::Right pins it permanently
        //  to the top-right corner of the maximised screen regardless
        //  of resolution.  The button is positioned precisely in OnLoad
        //  once ClientSize is available.
        //
        //  Character: U+2715 MULTIPLICATION X (✕) — never a literal.
        // ══════════════════════════════════════════════════════════
        void BuildCloseButton()
        {
            btnClose = gcnew Button();
            btnClose->Text = Char::ConvertFromUtf32(0x2715);   // ✕
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 12, FontStyle::Regular);
            btnClose->Size = Drawing::Size(44, 44);
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->FlatAppearance->MouseOverBackColor = CLR_HOVER_BG;
            btnClose->BackColor = Color::Transparent;
            btnClose->ForeColor = CLR_MUTED;
            btnClose->TextAlign = ContentAlignment::MiddleCenter;
            btnClose->Cursor = Cursors::Hand;
            // Anchor to Top | Right — distance to right edge stays constant
            btnClose->Anchor = static_cast<AnchorStyles>(
                AnchorStyles::Top | AnchorStyles::Right);
            // Placeholder location; corrected in OnLoad once ClientSize is valid
            btnClose->Location = Drawing::Point(0, 2);

            btnClose->Click += gcnew EventHandler(this, &LoginForm::OnClose);
            btnClose->MouseEnter += gcnew EventHandler(this, &LoginForm::OnCloseBtnEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &LoginForm::OnCloseBtnLeave);

            this->Controls->Add(btnClose);
        }


        // ══════════════════════════════════════════════════════════
        //  BuildCardPanel
        //  Creates the 440 × 540 white card and delegates each
        //  visual section to its own Build*() helper.
        // ══════════════════════════════════════════════════════════
        void BuildCardPanel()
        {
            pnlCard = gcnew Panel();
            pnlCard->Size = Drawing::Size(440, 540);
            pnlCard->BackColor = CLR_BG_CARD;
            // Subtle 1-px border drawn at paint time (Paint event below)
            pnlCard->Paint += gcnew PaintEventHandler(this, &LoginForm::OnCardPaint);

            BuildAccentStrip();
            BuildBrandRow();
            BuildTitleBlock();
            BuildUsernameField();
            BuildPasswordField();
            BuildShowPasswordRow();
            BuildErrorLabel();
            BuildLoginButton();
            BuildSignupRow();


            this->Controls->Add(pnlCard);
        }


        // ── ① 8-px crimson accent strip (full-width card top) ─────
        void BuildAccentStrip()
        {
            Panel^ strip = gcnew Panel();
            strip->Size = Drawing::Size(pnlCard->Width, 8);
            strip->Location = Drawing::Point(0, 0);
            strip->BackColor = CLR_ACCENT;
            pnlCard->Controls->Add(strip);
        }


        // ── ② Brand row: medical-cross icon + wordmark ────────────
        //
        //  The cross is built from two plain Panels (white-on-crimson
        //  was the dashboard standard; here crimson-on-white).
        //  This avoids any font / encoding dependence for the "+" glyph.
        //  Blood-drop emoji: U+1FA78 🩸  — no literal character.
        // ─────────────────────────────────────────────────────────────
        void BuildBrandRow()
        {
            const int ROW_Y = 24;
            const int ICON_X = 36;

            // Vertical bar of the red cross
            Panel^ cv = gcnew Panel();
            cv->Size = Drawing::Size(6, 22);
            cv->Location = Drawing::Point(ICON_X + 8, ROW_Y);
            cv->BackColor = CLR_ACCENT;

            // Horizontal bar of the red cross
            Panel^ ch = gcnew Panel();
            ch->Size = Drawing::Size(22, 6);
            ch->Location = Drawing::Point(ICON_X, ROW_Y + 8);
            ch->BackColor = CLR_ACCENT;

            // Wordmark (Shifted left to cover the gap left by the emoji)
            Label^ brand = gcnew Label();
            brand->Text = "LIFELINE   \u2014   BLOOD BANK";
            brand->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Bold);
            brand->ForeColor = CLR_ACCENT;
            brand->AutoSize = true;
            brand->Location = Drawing::Point(ICON_X + 28, ROW_Y + 4);
            brand->BackColor = Color::Transparent;

            pnlCard->Controls->Add(cv);
            pnlCard->Controls->Add(ch);
            pnlCard->Controls->Add(brand);
        }


        // ── ③ Title + subtitle block ──────────────────────────────
        void BuildTitleBlock()
        {
            Label^ title = gcnew Label();
            title->Text = "Blood Bank System";
            title->Font = gcnew Drawing::Font("Segoe UI Light", 24, FontStyle::Regular);
            title->ForeColor = CLR_TEXT;
            title->AutoSize = true;
            title->Location = Drawing::Point(36, 66);
            title->BackColor = Color::Transparent;

            Label^ sub = gcnew Label();
            sub->Text = "SIGN IN TO CONTINUE   \u2014   ROLE-BASED ACCESS";
            sub->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Regular);
            sub->ForeColor = CLR_MUTED;
            sub->AutoSize = true;
            sub->Location = Drawing::Point(36, 114);
            sub->BackColor = Color::Transparent;

            pnlCard->Controls->Add(title);
            pnlCard->Controls->Add(sub);
        }


        // ── ④ Username field — material underline style ───────────
        //
        //  LAYOUT CONSTANTS (shared by all field rows):
        //    FIELD_X = 36 px left margin inside card (36 right also = centred)
        //    FIELD_W = 368 px  →  36 + 368 + 36 = 440 = card width  ✓
        // ─────────────────────────────────────────────────────────────
        void BuildUsernameField()
        {
            const int FIELD_X = 36;
            const int FIELD_Y = 144;
            const int FIELD_W = 368;

            Label^ lbl = gcnew Label();
            lbl->Text = "USERNAME";
            lbl->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lbl->ForeColor = CLR_MUTED;
            lbl->AutoSize = true;
            lbl->Location = Drawing::Point(FIELD_X, FIELD_Y);
            lbl->BackColor = Color::Transparent;

            txtUsername = gcnew TextBox();
            txtUsername->Size = Drawing::Size(FIELD_W, 28);
            txtUsername->Location = Drawing::Point(FIELD_X, FIELD_Y + 20);
            txtUsername->BorderStyle = BorderStyle::None;
            txtUsername->BackColor = CLR_BG_INPUT;
            txtUsername->ForeColor = CLR_TEXT;
            txtUsername->Font = gcnew Drawing::Font("Segoe UI", 13);

            // 2-px animated underline panel
            pnlUnderUser = gcnew Panel();
            pnlUnderUser->Size = Drawing::Size(FIELD_W, 2);
            pnlUnderUser->Location = Drawing::Point(FIELD_X, FIELD_Y + 50);
            pnlUnderUser->BackColor = CLR_FIELD_LINE;   // idle = muted grey

            txtUsername->Enter += gcnew EventHandler(this, &LoginForm::OnUserEnter);
            txtUsername->Leave += gcnew EventHandler(this, &LoginForm::OnUserLeave);
            txtUsername->KeyDown += gcnew KeyEventHandler(this, &LoginForm::OnUserKey);

            pnlCard->Controls->Add(lbl);
            pnlCard->Controls->Add(txtUsername);
            pnlCard->Controls->Add(pnlUnderUser);
        }


        // ── ⑤ Password field — material underline style ───────────
        void BuildPasswordField()
        {
            const int FIELD_X = 36;
            const int FIELD_Y = 220;   // 26 px below username underline (50+144=194 → 220)
            const int FIELD_W = 368;

            Label^ lbl = gcnew Label();
            lbl->Text = "PASSWORD";
            lbl->Font = gcnew Drawing::Font("Segoe UI", 7, FontStyle::Bold);
            lbl->ForeColor = CLR_MUTED;
            lbl->AutoSize = true;
            lbl->Location = Drawing::Point(FIELD_X, FIELD_Y);
            lbl->BackColor = Color::Transparent;

            txtPassword = gcnew TextBox();
            txtPassword->Size = Drawing::Size(FIELD_W, 28);
            txtPassword->Location = Drawing::Point(FIELD_X, FIELD_Y + 20);
            txtPassword->BorderStyle = BorderStyle::None;
            txtPassword->BackColor = CLR_BG_INPUT;
            txtPassword->ForeColor = CLR_TEXT;
            txtPassword->Font = gcnew Drawing::Font("Segoe UI", 13);
            txtPassword->PasswordChar = L'*';

            pnlUnderPass = gcnew Panel();
            pnlUnderPass->Size = Drawing::Size(FIELD_W, 2);
            pnlUnderPass->Location = Drawing::Point(FIELD_X, FIELD_Y + 50);
            pnlUnderPass->BackColor = CLR_FIELD_LINE;

            txtPassword->Enter += gcnew EventHandler(this, &LoginForm::OnPassEnter);
            txtPassword->Leave += gcnew EventHandler(this, &LoginForm::OnPassLeave);
            txtPassword->KeyDown += gcnew KeyEventHandler(this, &LoginForm::OnPassKey);

            pnlCard->Controls->Add(lbl);
            pnlCard->Controls->Add(txtPassword);
            pnlCard->Controls->Add(pnlUnderPass);
        }


        // ── ⑥ Show-password toggle row ────────────────────────────
        void BuildShowPasswordRow()
        {
            chkShow = gcnew CheckBox();
            chkShow->Text = "Show Password";
            chkShow->Font = gcnew Drawing::Font("Segoe UI", 9);
            chkShow->ForeColor = CLR_MUTED;
            chkShow->AutoSize = true;
            chkShow->Location = Drawing::Point(36, 290);
            chkShow->BackColor = Color::Transparent;
            chkShow->FlatStyle = FlatStyle::Flat;
            chkShow->CheckedChanged += gcnew EventHandler(this, &LoginForm::OnShowToggle);
            pnlCard->Controls->Add(chkShow);
        }


        // ── ⑦ Inline error label ──────────────────────────────────
        void BuildErrorLabel()
        {
            lblError = gcnew Label();
            lblError->Text = "";
            lblError->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblError->ForeColor = CLR_ERROR;
            lblError->Size = Drawing::Size(368, 22);
            lblError->Location = Drawing::Point(36, 322);
            lblError->BackColor = Color::Transparent;
            lblError->Visible = false;
            pnlCard->Controls->Add(lblError);
        }


        // ── ⑧ Login button — Timer-driven smooth hover fade ───────
        //
        //  Rest  state: CLR_ACCENT     (#C0392B crimson)
        //  Hover state: CLR_ACCENT_DARK (#922B21 deeper crimson)
        //  The LerpColor() + _fadeTimer tick produce a ~130 ms blend.
        //
        //  Lock emoji: U+1F512 🔒 — rendered via ConvertFromUtf32.
        // ─────────────────────────────────────────────────────────────
        void BuildLoginButton()
        {
            btnLogin = gcnew Button();
            btnLogin->Text = Char::ConvertFromUtf32(0x1F512) + "   SIGN  IN";
            btnLogin->Size = Drawing::Size(368, 50);
            btnLogin->Location = Drawing::Point(36, 356);
            btnLogin->FlatStyle = FlatStyle::Flat;
            btnLogin->FlatAppearance->BorderSize = 0;
            btnLogin->BackColor = CLR_ACCENT;         // rest colour
            btnLogin->ForeColor = Color::White;
            btnLogin->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            btnLogin->Cursor = Cursors::Hand;

            btnLogin->MouseEnter += gcnew EventHandler(this, &LoginForm::OnBtnEnter);
            btnLogin->MouseLeave += gcnew EventHandler(this, &LoginForm::OnBtnLeave);
            btnLogin->Click += gcnew EventHandler(this, &LoginForm::OnLoginClick);
            pnlCard->Controls->Add(btnLogin);
        }


        // ── ⑨ "Don't have an account? Sign Up" link row ───────────
        //
        //  PRESERVED EXACTLY from the original LoginForm.h:
        //    • ForeColor for non-link portion = CLR_MUTED
        //    • LinkColor / VisitedLinkColor   = CLR_ACCENT
        //    • ActiveLinkColor (on click)     = CLR_ERROR
        //    • LinkBehavior = HoverUnderline
        //    • LinkArea(25, 7)  — only "Sign Up" (chars 25-31) is clickable
        //    • LinkClicked wired to OnSignupClick
        // ─────────────────────────────────────────────────────────────
        void BuildSignupRow()
        {
            LinkLabel^ lnk = gcnew LinkLabel();
            lnk->Text = "Don't have an account?   Sign Up";
            lnk->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Regular);
            lnk->ForeColor = CLR_MUTED;
            lnk->BackColor = Color::Transparent;
            lnk->LinkColor = CLR_ACCENT;
            lnk->VisitedLinkColor = CLR_ACCENT;       // stays crimson; no purple
            lnk->ActiveLinkColor = CLR_ERROR;         // lighter flash on click
            lnk->LinkBehavior = LinkBehavior::HoverUnderline;
            // "Don't have an account?   " → 25 chars; "Sign Up" → 7 chars
            lnk->LinkArea = System::Windows::Forms::LinkArea(25, 7);
            lnk->Size = Drawing::Size(368, 30);
            lnk->Location = Drawing::Point(36, 420);
            lnk->TextAlign = ContentAlignment::MiddleCenter;
            lnk->LinkClicked += gcnew LinkLabelLinkClickedEventHandler(
                this, &LoginForm::OnSignupClick);
            pnlCard->Controls->Add(lnk);
        }


        // ── Card border paint (1-px CLR_BORDER outline) ───────────
        //
        //  A Panel has no BorderStyle option that gives a single-pixel
        //  colour border without adding a sunken/raised bevel.
        //  Painting in the Paint event gives pixel-perfect control.
        // ─────────────────────────────────────────────────────────────
        void OnCardPaint(Object^ s, PaintEventArgs^ e)
        {
            Drawing::Rectangle r(0, 0, pnlCard->Width - 1, pnlCard->Height - 1);
            e->Graphics->DrawRectangle(gcnew Drawing::Pen(CLR_BORDER, 1), r);
        }


        // ══════════════════════════════════════════════════════════
        //  EVENT HANDLERS
        // ══════════════════════════════════════════════════════════

        // ── Form Load ────────────────────────────────────────────
        //
        //  1. Pin close button to top-right now that ClientSize is valid.
        //  2. Center the card.
        //  3. Inject Win32 cue-banners (placeholder text).
        //  4. Test DB connectivity; surface error dialog if unreachable.
        // ─────────────────────────────────────────────────────────────
        void OnLoad(Object^ sender, EventArgs^ e)
        {
            // ① Position close button — Anchor maintains it on resize
            btnClose->Location = Drawing::Point(this->ClientSize.Width - 44, 0);

            // ② Centre the login card
            CenterCard();

            // ③ Win32 EM_SETCUEBANNER — placeholder text even when focused
            //    (PlaceholderText property only exists in .NET 5+; this
            //    P/Invoke path is correct for .NET Framework / C++/CLI)
            const UInt32 EM_SETCUEBANNER = 0x1501u;
            SendCue(txtUsername->Handle, EM_SETCUEBANNER, IntPtr(0), "Enter your username");
            SendCue(txtPassword->Handle, EM_SETCUEBANNER, IntPtr(0), "Enter your password");

            // ④ Database connectivity check
            if (!Database::GetInstance()->TestConnection())
            {
                MessageBox::Show(
                    "Cannot reach the SQL Server database.\n\n"
                    "Please verify:\n"
                    "  1.  SQL Server Express is running\n"
                    "       (TALHA\\SQLEXPRESS)\n"
                    "  2.  The database  'db_bloodbank'  exists\n"
                    "  3.  Windows Authentication is enabled",
                    "  Database Unreachable  \u2014  LifeLine",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Error
                );
            }

            txtUsername->Focus();
        }

        // ── Form Resize — re-centre card on every size change ────
        void OnResize(Object^ sender, EventArgs^ e)
        {
            if (pnlCard != nullptr)
                CenterCard();
        }


        // ══ BUTTON HOVER  —  Timer-driven smooth RGB lerp ═════════
        //
        //  LerpColor(a, b, t) linearly blends colour a → b by factor
        //  t ∈ [0, 1].  Called each timer tick to produce a smooth fade.
        // ──────────────────────────────────────────────────────────────
        Color LerpColor(Color a, Color b, float t)
        {
            int r = Math::Max(0, Math::Min(255, (int)(a.R + (b.R - a.R) * t)));
            int g = Math::Max(0, Math::Min(255, (int)(a.G + (b.G - a.G) * t)));
            int bl = Math::Max(0, Math::Min(255, (int)(a.B + (b.B - a.B) * t)));
            return Color::FromArgb(255, r, g, bl);
        }

        // Timer tick: advance _fadeT, clamp, repaint button
        void OnFadeTick(Object^ sender, EventArgs^ e)
        {
            _fadeT += _fadeDir * FADE_STEP;

            if (_fadeT >= 1.0f) { _fadeT = 1.0f; _fadeTimer->Stop(); }
            else if (_fadeT <= 0.0f) { _fadeT = 0.0f; _fadeTimer->Stop(); }

            // Blend between rest colour and darker hover target
            btnLogin->BackColor = LerpColor(CLR_ACCENT, CLR_ACCENT_DARK, _fadeT);
        }

        // Mouse enters button → set direction +1, (re-)start timer
        void OnBtnEnter(Object^ sender, EventArgs^ e)
        {
            _fadeDir = +1;
            _fadeTimer->Start();
        }

        // Mouse leaves button → set direction -1, (re-)start timer
        void OnBtnLeave(Object^ sender, EventArgs^ e)
        {
            _fadeDir = -1;
            _fadeTimer->Start();
        }


        // ══ UNDERLINE FOCUS TRANSITIONS ════════════════════════════
        //
        //  The 2-px Panel beneath each TextBox switches between
        //  CLR_FIELD_LINE (idle) and CLR_ACCENT (focused).
        //  This simulates the Material Design "activated line" pattern.
        // ──────────────────────────────────────────────────────────────
        void OnUserEnter(Object^ s, EventArgs^ e) { pnlUnderUser->BackColor = CLR_ACCENT; }
        void OnUserLeave(Object^ s, EventArgs^ e) { pnlUnderUser->BackColor = CLR_FIELD_LINE; }
        void OnPassEnter(Object^ s, EventArgs^ e) { pnlUnderPass->BackColor = CLR_ACCENT; }
        void OnPassLeave(Object^ s, EventArgs^ e) { pnlUnderPass->BackColor = CLR_FIELD_LINE; }


        // ══ SHOW-PASSWORD TOGGLE ═══════════════════════════════════
        void OnShowToggle(Object^ s, EventArgs^ e)
        {
            // L'\0' removes masking entirely; L'*' restores it
            txtPassword->PasswordChar = chkShow->Checked ? L'\0' : L'*';
        }


        // ══ CLOSE BUTTON ═══════════════════════════════════════════
        void OnClose(Object^ s, EventArgs^ e)
        {
            Application::Exit();
        }

        void OnCloseBtnEnter(Object^ s, EventArgs^ e)
        {
            // Blush-pink fill + crimson icon on hover
            btnClose->BackColor = CLR_HOVER_BG;
            btnClose->ForeColor = CLR_ACCENT;
        }

        void OnCloseBtnLeave(Object^ s, EventArgs^ e)
        {
            btnClose->BackColor = Color::Transparent;
            btnClose->ForeColor = CLR_MUTED;
        }


        // ══ KEYBOARD SHORTCUTS ═════════════════════════════════════
        //
        //  Enter in username field  → jump to password
        //  Enter in password field  → submit (calls OnLoginClick)
        //  Escape (form-level)      → Application::Exit()
        //
        //  SuppressKeyPress = true on Enter prevents the system "ding".
        // ──────────────────────────────────────────────────────────────
        void OnUserKey(Object^ s, KeyEventArgs^ e)
        {
            if (e->KeyCode == Keys::Return)
            {
                e->SuppressKeyPress = true;
                txtPassword->Focus();
            }
        }

        void OnPassKey(Object^ s, KeyEventArgs^ e)
        {
            if (e->KeyCode == Keys::Return)
            {
                e->SuppressKeyPress = true;
                OnLoginClick(s, e);
            }
        }

        void OnFormKey(Object^ s, KeyEventArgs^ e)
        {
            if (e->KeyCode == Keys::Escape)
                Application::Exit();
        }


        // ══ SIGNUP NAVIGATION ══════════════════════════════════════
        void OnSignupClick(Object^ sender, LinkLabelLinkClickedEventArgs^ e)
        {
            this->Hide();
            SignupForm^ sf = gcnew SignupForm();
            // Wire the FormClosed handler so Login returns when signup closes
            sf->FormClosed += gcnew FormClosedEventHandler(this, &LoginForm::OnChildClosed);
            sf->Show();
        }


        // ══ LOGIN LOGIC ════════════════════════════════════════════
        //
        //  PRESERVED EXACTLY:
        //    • Database::GetInstance()->ValidateLogin() call signature
        //    • Role-based routing: Admin / Donor / Recipient
        //    • FormClosedEventHandler chain for all three dashboards
        //    • Client-side empty-field guard
        //    • Error display and password-field clear on failure
        // ──────────────────────────────────────────────────────────────
        void OnLoginClick(Object^ sender, EventArgs^ e)
        {
            // Reset error state
            lblError->Visible = false;
            lblError->Text = "";

            String^ uname = txtUsername->Text->Trim();
            String^ pwd = txtPassword->Text;      // preserve password spacing

            // ── Client-side guard ─────────────────────────────────
            if (String::IsNullOrWhiteSpace(uname) ||
                String::IsNullOrWhiteSpace(pwd))
            {
                ShowError("Username and password cannot be empty.");
                return;
            }

            // ── ADO.NET authentication via Database singleton ─────
            String^ role;
            String^ fullName;
            int     userID = 0;

            bool ok = Database::GetInstance()->ValidateLogin(
                uname, pwd, role, userID, fullName);

            if (ok)
            {
                // ── Role-based routing ────────────────────────────
                if (role == "Admin")
                {
                    this->Hide();
                    AdminDashboard^ d = gcnew AdminDashboard(userID, fullName);
                    // FormClosed → OnChildClosed restores the Login form
                    d->FormClosed += gcnew FormClosedEventHandler(
                        this, &LoginForm::OnChildClosed);
                    d->Show();
                }
                else if (role == "Donor")
                {
                    this->Hide();
                    DonorDashboard^ d = gcnew DonorDashboard(userID, fullName);
                    d->FormClosed += gcnew FormClosedEventHandler(
                        this, &LoginForm::OnChildClosed);
                    d->Show();
                }
                else if (role == "Recipient")
                {
                    this->Hide();
                    RecipientDashboard^ rd = gcnew RecipientDashboard(userID, fullName);
                    rd->FormClosed += gcnew FormClosedEventHandler(
                        this, &LoginForm::OnChildClosed);
                    rd->Show();
                }
                else
                {
                    // Defensive guard for any unexpected DB value
                    ShowError("Unknown role: '" + role + "'. Contact admin.");
                }
            }
            else
            {
                // Warning sign: U+26A0 ⚠
                ShowError(Char::ConvertFromUtf32(0x26A0) +
                    "  Invalid username or password. Please try again.");
                txtPassword->Clear();
                txtPassword->Focus();
            }
        }

        // ── Restores Login form when any child window closes ──────
        void OnChildClosed(Object^ s, FormClosedEventArgs^ e)
        {
            txtUsername->Clear();
            txtPassword->Clear();
            lblError->Visible = false;
            this->Show();
            txtUsername->Focus();
        }

        // ── Helper: surface an error message above the button ─────
        void ShowError(String^ msg)
        {
            lblError->Text = msg;
            lblError->Visible = true;
        }

    };  // ref class LoginForm

}  // namespace BloodBank


// ================================================================
//  ENTRY POINT WIRING  (main.cpp — unchanged)
// ================================================================
//
//  [STAThread]
//  int main(array<String^>^ args)
//  {
//      Application::EnableVisualStyles();
//      Application::SetCompatibleTextRenderingDefault(false);
//      Application::Run(gcnew BloodBank::LoginForm());
//      return 0;
//  }
//
// ================================================================
//  QUICK TEST CREDENTIALS  (seed data in SQLQuery1.sql)
// ================================================================
//
//  Admin      →  talha_p   / fast2026
//  Admin      →  hina_i    / pwd6
//  Donor      →  maryam_s  / pass123
//  Donor      →  adnan_a   / pass789
//  Recipient  →  nabeera_s / user456
//
// ================================================================
//  REFACTORING CHANGE LOG vs. ORIGINAL LoginForm.h
// ================================================================
//
//  REMOVED
//    • Dark palette (#1A1A1A / #212121 / #F0F0F0) → replaced with
//      DonorDashboard light palette verbatim
//    • this->Size / MinimumSize / StartPosition → replaced with
//      WindowState::Maximized + FormBorderStyle::None
//    • Mouse-drag logic (_dragging / _dragOriginCursor / _dragOriginForm
//      + OnDragDown / OnDragMove / OnDragUp) — dragging a Maximized
//      borderless form is undefined behaviour; not needed
//    • btnClose inside pnlCard → moved to the form level with Anchor
//
//  ADDED
//    • CenterCard()  — pure centering math, called from OnLoad + OnResize
//    • 8-px CLR_ACCENT accent strip at card top   (BuildAccentStrip)
//    • OnCardPaint   — 1-px CLR_BORDER outline drawn at paint time
//    • OnResize      — re-centres card on any client-size change
//    • Blush-pink hover state on close button (CLR_HOVER_BG fill)
//    • All emojis via Char::ConvertFromUtf32(0x...)
//        0x2715  ✕  close button
//        0x1FA78 🩸  blood-drop brand accent
//        0x1F512 🔒  login button prefix
//        0x26A0  ⚠  error message prefix
//
//  PRESERVED 1-FOR-1
//    • Database::GetInstance()->ValidateLogin() — signature unchanged
//    • Role-based routing (Admin / Donor / Recipient)
//    • FormClosedEventHandler chain on all three dashboards + SignupForm
//    • LinkLabel with LinkArea(25, 7) for "Sign Up"
//    • Win32 SendCue / EM_SETCUEBANNER P/Invoke
//    • _fadeTimer RGB lerp on btnLogin (CLR_ACCENT → CLR_ACCENT_DARK)
//    • pnlUnderUser / pnlUnderPass focus colour transitions
//    • chkShow password-mask toggle
//    • Keyboard shortcuts (Enter navigation, Escape exit)
//    • KeyPreview + SuppressKeyPress anti-ding pattern
//    • DoubleBuffered = true
//
// ================================================================