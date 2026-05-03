// ================================================================
//  LoginForm.h  —  LifeBlood MMS
//  Logic-Injection Overhaul
//
//  VISUAL ORIGIN : loginForm.h  (Heartbeat / Glass GDI+ right panel,
//                                50/50 split, crimson brand palette)
//  LOGIC  ORIGIN : LoginForm.h  (Role state machine, DB singleton,
//                                role guard, dashboard routing)
//
//  COMPILER CONSTRAINTS OBSERVED
//  ─────────────────────────────────────────────────────────────
//  1. NO LAMBDAS        — every event handler is a named member fn
//  2. INTEGER BOXING    — LoginRole stored in Tag as (int) to avoid
//                         C2440 "cannot convert LoginRole to Object^"
//  3. UNICODE SAFE      — arrows / special chars via
//                         Char::ConvertFromUtf32(codepoint); no raw
//                         Unicode string literals that trigger CP-1252
//
//  ARCHITECTURE
//  ─────────────────────────────────────────────────────────────
//  • WindowState = Maximized | FormBorderStyle = None | DoubleBuffered
//  • Left  Panel (50 %) — white functional login panel
//  • Right Panel (50 %) — original heartbeat / glass GDI+ showcase
//  • enum class LoginRole { Admin, Donor, Recipient }
//  • SwitchRole(LoginRole) updates every dynamic element
//  • _animTimer (~60 fps) drives pill sine-drift + panel Invalidate()
//  • _fadeTimer drives btnLogin RGB hover lerp
//  • Role Guard: DB role must match UI-selected role, else blocked
//
//  DEPENDENCIES (unchanged)
//  ─────────────────────────────────────────────────────────────
//  Database.h · SignupForm.h · AdminDashboard.h
//  DonorDashboard.h · RecipientDashboard.h
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
using namespace System::Drawing::Drawing2D;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

namespace BloodBank {

    // ────────────────────────────────────────────────────────────
    //  Role enum — drives every dynamic UI state
    // ────────────────────────────────────────────────────────────
    enum class LoginRole { Admin, Donor, Recipient };


    public ref class LoginForm : public Form
    {

        // ── Win32 cue-banner P/Invoke ────────────────────────────
    private:
        [DllImport("user32.dll", EntryPoint = "SendMessageW",
            CharSet = CharSet::Unicode)]
            static IntPtr SendCue(IntPtr hWnd, UInt32 msg,
                IntPtr wp, String^ lp);






        // ════════════════════════════════════════════════════════
        //  PALETTE
        //  Left panel uses the original form's pure whites and
        //  its exact brand crimson (232, 15, 59).
        //  Right panel keeps the old form's exact gradient stops.
        // ════════════════════════════════════════════════════════
        Color CLR_WHITE;          // #FFFFFF
        Color CLR_TEXT;           // #1A0A0A  near-black

        Color CLR_MUTED;          // #6C757D  secondary text
        Color CLR_HINT;           // #ADB5BD  placeholder / icon

        // Accent — old form's brand crimson family
        Color CLR_ACCENT;         // rgb(232, 15, 59)   primary
        Color CLR_ACCENT_DARK;    // rgb(180, 10, 45)   hover target
        Color CLR_ERROR;          // #E74C3C            error red

        // Role switcher button states
        Color CLR_BTN_ACTIVE_BG;  // = CLR_ACCENT fill when selected
        Color CLR_BTN_IDLE_BG;    // #F0F0F0  unselected bg
        Color CLR_BTN_IDLE_FG;    // #555555  unselected text

        // Input / layout structure
        Color CLR_BORDER;         // #E9ECEF  field outline
        Color CLR_FIELD_BG;       // #F8F8FA  glass-input surface
        Color CLR_HOVER_BG;       // rgb(255,230,235)  close btn hover

        // Right panel — old form's exact gradient stops (preserved 1-for-1)
        Color CLR_RIGHT_TOP;      // rgb(210, 10,  45) Dark Red
        Color CLR_RIGHT_BOT;      // rgb( 60, 20,  35) Deep Burgundy


        // Adjust this number to push the content further right
        int LEFT_MARGIN = 120;

        // Adjust this number to push the content lower down
        int TOP_MARGIN = 60; // Change this to whatever looks best


        // ════════════════════════════════════════════════════════
        //  STATE
        // ════════════════════════════════════════════════════════
        LoginRole currentRole;


        // ════════════════════════════════════════════════════════
        //  CONTROL HANDLES — Left panel
        // ════════════════════════════════════════════════════════
        Panel^ pnlLeft;
        Panel^ pnlRight;

        // Role switcher (3-button segmented control)
        Button^ btnAdmin;
        Button^ btnDonor;
        Button^ btnRecipient;
        Label^ lblRoleHint;    // "Manage inventory, requests…"

        // Glass input boxes
        Panel^ pnlUserBox;
        Panel^ pnlPassBox;
        TextBox^ txtUsername;
        TextBox^ txtPassword;
        Button^ btnReveal;      // eye-icon toggle inside password box


        // CTA
        Button^ btnLogin;

        // Signup row
        LinkLabel^ lnkSignup;

        // Status / error
        Label^ lblError;

        // Screen-level close button (top-right)
        Button^ btnClose;


        // ════════════════════════════════════════════════════════
        //  CONTROL HANDLES — Right panel (dynamic role text)
        // ════════════════════════════════════════════════════════
        Label^ lblPortalChip;    // e.g. "⊙ Admin portal"
        Label^ lblRightHeading;  // hero text (changes per role)
        Label^ lblRightSub;      // subtitle paragraph


        // ════════════════════════════════════════════════════════
        //  ANIMATION — btnLogin hover (RGB lerp)
        // ════════════════════════════════════════════════════════
        Timer^ _fadeTimer;
        float       _fadeT;
        int         _fadeDir;
        const float FADE_STEP = 0.08f;


        // ════════════════════════════════════════════════════════
        //  ANIMATION — right panel (~60 fps)
        //
        //  Single _animTimer drives two visual effects:
        //    A) Floating blood-type pills — sine-wave Y drift
        //    B) Heartbeat / EKG glass card — panel Invalidate()
        //       re-draws every frame so the card is always fresh
        //
        //  Pill layout: label, fractional X, fractional base-Y,
        //  phase offset for staggered sine waves.
        // ════════════════════════════════════════════════════════
        Timer^ _animTimer;
        double          _animTick;        // ever-increasing radian angle

        array<String^>^ _pillLabels;
        array<float>^ _pillX;          // fraction of right-panel width
        array<float>^ _pillBaseY;      // fraction of right-panel height
        array<float>^ _pillPhase;      // per-pill phase offset


        // ════════════════════════════════════════════════════════
        //  CONSTRUCTOR
        // ════════════════════════════════════════════════════════
    public:
        LoginForm()
        {
            // ── Palette initialisation ───────────────────────────
            CLR_WHITE = Color::White;
            CLR_TEXT = ColorTranslator::FromHtml("#1A0A0A");
            CLR_MUTED = ColorTranslator::FromHtml("#6C757D");
            CLR_HINT = ColorTranslator::FromHtml("#ADB5BD");

            // Old form's exact brand crimson
            CLR_ACCENT = Color::FromArgb(232, 15, 59);
            CLR_ACCENT_DARK = Color::FromArgb(180, 10, 45);
            CLR_ERROR = ColorTranslator::FromHtml("#E74C3C");

            CLR_BTN_ACTIVE_BG = CLR_ACCENT;
            CLR_BTN_IDLE_BG = ColorTranslator::FromHtml("#F0F0F0");
            CLR_BTN_IDLE_FG = ColorTranslator::FromHtml("#555555");

            CLR_BORDER = ColorTranslator::FromHtml("#E9ECEF");
            CLR_FIELD_BG = Color::FromArgb(248, 248, 250);
            CLR_HOVER_BG = Color::FromArgb(255, 230, 235);

            // Old form's right panel gradient — preserved exactly
            CLR_RIGHT_TOP = Color::FromArgb(210, 10, 45);
            CLR_RIGHT_BOT = Color::FromArgb(60, 20, 35);


            // ── Pill data ────────────────────────────────────────
            _pillLabels = gcnew array<String^>{ "A+", "O-", "O+", "B+" };
            _pillX = gcnew array<float>{ 0.06f, 0.60f, 0.52f, 0.08f };
            _pillBaseY = gcnew array<float>{ 0.08f, 0.18f, 0.31f, 0.44f };
            _pillPhase = gcnew array<float>{ 0.0f, 1.2f, 2.4f, 3.6f };

            // ── Hover fade timer ─────────────────────────────────
            _fadeTimer = gcnew Timer();
            _fadeTimer->Interval = 12;
            _fadeTimer->Tick += gcnew EventHandler(this, &LoginForm::OnFadeTick);
            _fadeT = 0.0f;
            _fadeDir = 0;

            // ── Heartbeat + pill animation (~60 fps) ─────────────
            _animTimer = gcnew Timer();
            _animTimer->Interval = 16;
            _animTick = 0.0;
            _animTimer->Tick += gcnew EventHandler(this, &LoginForm::OnAnimTick);

            currentRole = LoginRole::Admin;

            InitializeComponent();
        }

    protected:
        ~LoginForm() {}


        // ════════════════════════════════════════════════════════
        //  InitializeComponent
        // ════════════════════════════════════════════════════════
    private:
        void InitializeComponent()
        {
            this->Text = "LifeBlood MS   -   Sign In";
            this->WindowState = FormWindowState::Maximized;
            this->FormBorderStyle =
                System::Windows::Forms::FormBorderStyle::None;
            this->BackColor = CLR_WHITE;
            this->DoubleBuffered = true;
            this->KeyPreview = true;

            this->Load += gcnew EventHandler(this, &LoginForm::OnLoad);
            this->Resize += gcnew EventHandler(this, &LoginForm::OnResize);
            this->KeyDown += gcnew KeyEventHandler(this, &LoginForm::OnFormKey);

            // Build order matters: right panel first (z-order back)
            BuildRightPanel();
            BuildLeftPanel();
            BuildCloseButton();   // floats above both panels

            // Apply default role styling after all controls exist
            SwitchRole(LoginRole::Admin);
        }


        // ════════════════════════════════════════════════════════
        //  LAYOUT HELPERS
        // ════════════════════════════════════════════════════════

        void LayoutPanels()
        {
            int W = this->ClientSize.Width;
            int H = this->ClientSize.Height;
            int half = W / 2;

            pnlLeft->SetBounds(0, 0, half, H);
            pnlRight->SetBounds(half, 0, W - half, H);

            // Keep close button pinned to absolute top-right
            btnClose->Location =
                Drawing::Point(this->ClientSize.Width - 48, 0);
        }


        // ════════════════════════════════════════════════════════
        //  RIGHT PANEL — Original Heartbeat / Glass showcase
        //
        //  Visual origin: loginForm.h (old form), preserved exactly.
        //  Added on top: floating blood-type pills from new form.
        //
        //  Paint layers (back to front)
        //  ─────────────────────────────────────────────────────
        //  1. ForwardDiagonal gradient (CLR_RIGHT_TOP → CLR_RIGHT_BOT)
        //  2. Floating blood-type pills  (sine-wave Y drift)
        //  3. Central glass card         (translucent rounded rect)
        //  4. Heartbeat / EKG icon       (heart outline + EKG polyline)
        //  5. Dynamic role labels        (portal chip, heading, sub-text)
        //  6. Stats footer               (Donors / Units / Hospitals)
        // ════════════════════════════════════════════════════════
        void BuildRightPanel()
        {
            pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::None;
            pnlRight->BackColor = CLR_RIGHT_TOP;

            // Enable double-buffering via reflection (Panel has no public setter)
            pnlRight->GetType()->GetProperty("DoubleBuffered",
                System::Reflection::BindingFlags::Instance |
                System::Reflection::BindingFlags::NonPublic)
                ->SetValue(pnlRight, true);

            // ── Dynamic role text labels ─────────────────────────
            lblPortalChip = BuildRightLabel(
                "  Admin portal", 9, FontStyle::Regular,
                Color::FromArgb(200, 255, 255, 255));
            lblPortalChip->AutoSize = true;
            pnlRight->Controls->Add(lblPortalChip);

            lblRightHeading = BuildRightLabel(
                "", 20, FontStyle::Bold, Color::White);
            lblRightHeading->AutoSize = false;
            lblRightHeading->Size = Drawing::Size(460, 120);
            pnlRight->Controls->Add(lblRightHeading);

            lblRightSub = BuildRightLabel(
                "", 10, FontStyle::Regular,
                Color::FromArgb(210, 255, 255, 255));
            lblRightSub->AutoSize = false;
            lblRightSub->Size = Drawing::Size(420, 70);
            pnlRight->Controls->Add(lblRightSub);

            // ── Stats footer ─────────────────────────────────────
            BuildStatsFooter();

            // ── GDI+ paint ───────────────────────────────────────
            pnlRight->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnRightPaint);

            this->Controls->Add(pnlRight);
        }

        // Factory for a transparent right-panel label
        Label^ BuildRightLabel(String^ text, float size,
            FontStyle style, Color fore)
        {
            Label^ l = gcnew Label();
            l->Text = text;
            l->Font = gcnew Drawing::Font("Segoe UI", size, style);
            l->ForeColor = fore;
            l->BackColor = Color::Transparent;
            l->AutoSize = true;
            return l;
        }

        // Build the three-column stat footer on the right panel
        void BuildStatsFooter()
        {
            // Subtle divider line above the stats
            Panel^ div = gcnew Panel();
            div->BackColor = Color::FromArgb(60, 255, 255, 255);
            div->Size = Drawing::Size(1, 1);   // real size set in PositionRightLabels
            div->Tag = "divider";
            pnlRight->Controls->Add(div);

            array<String^>^ vals = { "12,450", "3,280", "47" };
            array<String^>^ capts = { "DONORS",  "UNITS", "HOSPITALS" };

            for (int i = 0; i < 3; i++)
            {
                Label^ lv = gcnew Label();
                lv->Text = vals[i];
                lv->Font = gcnew Drawing::Font(
                    "Segoe UI", 18, FontStyle::Bold);
                lv->ForeColor = Color::White;
                lv->AutoSize = true;
                lv->BackColor = Color::Transparent;
                lv->Tag = "stat_val_" + i;
                pnlRight->Controls->Add(lv);

                Label^ lc = gcnew Label();
                lc->Text = capts[i];
                lc->Font = gcnew Drawing::Font(
                    "Segoe UI", 7, FontStyle::Bold);
                lc->ForeColor = Color::FromArgb(170, 255, 255, 255);
                lc->AutoSize = true;
                lc->BackColor = Color::Transparent;
                lc->Tag = "stat_cap_" + i;
                pnlRight->Controls->Add(lc);
            }
        }


        // ════════════════════════════════════════════════════════
        //  OnRightPaint  — GDI+ paint for right panel
        //  Combines old form's heartbeat / glass with new form's
        //  floating pills for a richer animated showcase.
        // ════════════════════════════════════════════════════════
        void OnRightPaint(Object^ sender, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            g->TextRenderingHint =
                System::Drawing::Text::TextRenderingHint::AntiAliasGridFit;

            int W = pnlRight->Width;
            int H = pnlRight->Height;

            // ── Layer 1: Diagonal gradient (old form exact colors) ─
            Rectangle rect = pnlRight->ClientRectangle;
            LinearGradientBrush^ bgBrush = gcnew LinearGradientBrush(
                rect,
                CLR_RIGHT_TOP,          // rgb(210,  10,  45) Dark Red
                CLR_RIGHT_BOT,          // rgb( 60,  20,  35) Deep Burgundy
                LinearGradientMode::ForwardDiagonal);
            g->FillRectangle(bgBrush, rect);
            delete bgBrush;

            // ── Layer 2: Floating blood-type pills (sine drift) ────
            for (int i = 0; i < _pillLabels->Length; i++)
            {
                double drift = Math::Sin(_animTick + _pillPhase[i]) * 12.0;
                int px = (int)(_pillX[i] * W);
                int py = (int)(_pillBaseY[i] * H) + (int)drift;
                DrawPill(g, _pillLabels[i], px, py);
            }

            // ── Layer 3: Central glass card (old form style) ───────
            SolidBrush^ glassBrush =
                gcnew SolidBrush(Color::FromArgb(30, 255, 255, 255));
            Pen^ glassBorder =
                gcnew Pen(Color::FromArgb(50, 255, 255, 255), 1.5f);

            int CW = Math::Min(200, (int)(W * 0.38));
            int CH = (int)(CW * 1.15);
            int CX = (W - CW) / 2;
            // Old form used ClientSize.Height * 0.25 for the card's reference Y
            int CY = (int)(H * 0.25) - CH / 2;

            Rectangle cardRect(CX, CY, CW, CH);
            GraphicsPath^ cardPath = RoundedRect(cardRect, 15);
            g->FillPath(glassBrush, cardPath);
            g->DrawPath(glassBorder, cardPath);
            delete glassBrush; delete glassBorder; delete cardPath;

            // ── Layer 4: Heartbeat / EKG icon inside card (old form) ─
            //  Inset 5 px left/right, start 12 % from card top,
            //  occupy 75 % of card height (matches original proportions).
            DrawHeartbeatIcon(g,
                Rectangle(
                    CX + 5,
                    CY + (int)(CH * 0.12f),
                    CW - 10,
                    (int)(CH * 0.75f)));
        }


        // ════════════════════════════════════════════════════════
        //  GDI+ DRAWING PRIMITIVES
        // ════════════════════════════════════════════════════════

        // ── Heartbeat + EKG icon  (source: loginForm.h, exact copy) ─
        void DrawHeartbeatIcon(Graphics^ g, Rectangle bounds)
        {
            g->SmoothingMode = SmoothingMode::AntiAlias;

            Pen^ iconPen = gcnew Pen(Color::FromArgb(255, 205, 214), 4.4f);
            iconPen->LineJoin = LineJoin::Round;
            iconPen->StartCap = LineCap::Round;
            iconPen->EndCap = LineCap::Round;

            GraphicsPath^ heart = gcnew GraphicsPath();
            int w = bounds.Width;
            int h = bounds.Height;
            int x = bounds.X;
            int y = bounds.Y;

            Point pTop = Point(x + w / 2, y + static_cast<int>(h * 0.28f));
            Point pBottom = Point(x + w / 2, y + h - 4);

            heart->StartFigure();

            // Left lobe
            heart->AddBezier(
                pTop,
                Point(x + static_cast<int>(w * 0.18f), y - 2),
                Point(x - 2, y + static_cast<int>(h * 0.38f)),
                Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)));

            // Left lower
            heart->AddBezier(
                Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)),
                Point(x + static_cast<int>(w * 0.34f), y + static_cast<int>(h * 0.74f)),
                Point(x + static_cast<int>(w * 0.43f), y + static_cast<int>(h * 0.88f)),
                pBottom);

            // Right lower
            heart->AddBezier(
                pBottom,
                Point(x + static_cast<int>(w * 0.57f), y + static_cast<int>(h * 0.88f)),
                Point(x + static_cast<int>(w * 0.66f), y + static_cast<int>(h * 0.74f)),
                Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)));

            // Right lobe
            heart->AddBezier(
                Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)),
                Point(x + w + 2, y + static_cast<int>(h * 0.38f)),
                Point(x + static_cast<int>(w * 0.82f), y - 2),
                pTop);

            g->DrawPath(iconPen, heart);

            // EKG / pulse line through the heart
            array<Point>^ ekgPoints = {
                Point(x + static_cast<int>(w * 0.16f), y + static_cast<int>(h * 0.52f)),
                Point(x + static_cast<int>(w * 0.35f), y + static_cast<int>(h * 0.52f)),
                Point(x + static_cast<int>(w * 0.44f), y + static_cast<int>(h * 0.38f)),
                Point(x + static_cast<int>(w * 0.53f), y + static_cast<int>(h * 0.70f)),
                Point(x + static_cast<int>(w * 0.64f), y + static_cast<int>(h * 0.50f)),
                Point(x + static_cast<int>(w * 0.83f), y + static_cast<int>(h * 0.50f))
            };
            g->DrawLines(iconPen, ekgPoints);

            delete heart;
            delete iconPen;
        }


        // ── Floating blood-type pill  (source: LoginForm.h) ─────────
        void DrawPill(Graphics^ g, String^ label, int x, int y)
        {
            // UNICODE SAFE: blood-drop via codepoint, not a raw literal
            String^ text = Char::ConvertFromUtf32(0x1FA78) + " " + label;

            Drawing::Font^ f = gcnew Drawing::Font(
                "Segoe UI Emoji", 9, FontStyle::Bold);
            SizeF sz = g->MeasureString(text, f);

            int pw = (int)sz.Width + 20;
            int ph = (int)sz.Height + 10;

            SolidBrush^ bg =
                gcnew SolidBrush(Color::FromArgb(45, 255, 255, 255));
            Pen^ border =
                gcnew Pen(Color::FromArgb(80, 255, 255, 255), 1.0f);

            Drawing::Rectangle pr(x, y, pw, ph);
            GraphicsPath^ pp = RoundedRect(pr, ph / 2);
            g->FillPath(bg, pp);
            g->DrawPath(border, pp);
            delete bg; delete border; delete pp;

            SolidBrush^ fg =
                gcnew SolidBrush(Color::FromArgb(230, 255, 255, 255));
            g->DrawString(text, f, fg, (float)(x + 10), (float)(y + 5));
            delete fg; delete f;
        }


        // ── Blood-drop shape  (for brand badge dot) ──────────────────
        void DrawBloodDrop(Graphics^ g, int cx, int cy, int r)
        {
            SolidBrush^ wb =
                gcnew SolidBrush(Color::FromArgb(220, 255, 255, 255));

            int top = cy - r;
            int bottom = cy + (int)(r * 1.3);
            int left = cx - (int)(r * 0.72);
            int right = cx + (int)(r * 0.72);
            int mid = cy + (int)(r * 0.35);

            GraphicsPath^ drop = gcnew GraphicsPath();
            drop->AddLine(
                PointF((float)cx, (float)top),
                PointF((float)right, (float)mid));
            drop->AddArc(
                (float)left, (float)mid,
                (float)(right - left), (float)(bottom - mid),
                0, 180);
            drop->AddLine(
                PointF((float)left, (float)mid),
                PointF((float)cx, (float)top));
            drop->CloseFigure();

            g->FillPath(wb, drop);

            // Specular glint
            SolidBrush^ glint =
                gcnew SolidBrush(Color::FromArgb(90, 255, 255, 255));
            g->FillEllipse(glint,
                (float)(cx - r * 0.20f), (float)(cy - r * 0.55f),
                (float)(r * 0.28f), (float)(r * 0.38f));

            delete wb; delete glint; delete drop;
        }


        // ── Rounded-corner GraphicsPath (shared by all controls) ─────
        GraphicsPath^ RoundedRect(Drawing::Rectangle r, int radius)
        {
            GraphicsPath^ path = gcnew GraphicsPath();
            int d = radius * 2;
            path->AddArc(r.X, r.Y, d, d, 180, 90);
            path->AddArc(r.Right - d, r.Y, d, d, 270, 90);
            path->AddArc(r.Right - d, r.Bottom - d, d, d, 0, 90);
            path->AddArc(r.X, r.Bottom - d, d, d, 90, 90);
            path->CloseFigure();
            return path;
        }


        // ── Animation tick: advance angle, force right-panel repaint ─
        void OnAnimTick(Object^ sender, EventArgs^ e)
        {
            _animTick += 0.018;                        // radians per frame
            if (_animTick > Math::PI * 200) _animTick = 0.0;
            pnlRight->Invalidate();
        }


        // ════════════════════════════════════════════════════════
        //  LEFT PANEL — White functional login area
        //  Build order matches visual top-to-bottom reading order.
        // ════════════════════════════════════════════════════════
        void BuildLeftPanel()
        {
            pnlLeft = gcnew Panel();
            pnlLeft->BackColor = CLR_WHITE;

            BuildBrandRow();       //  LifeBlood logo + wordmark
            BuildWelcomeChip();    //  "✦ Welcome back" pill chip
            BuildTitleBlock();     //  "Sign in to LifeBlood MMS"
            BuildRoleSwitcher();   //  [Admin] [Donor] [Recipient]
            BuildRoleHint();       //  context sentence below switcher
            BuildUsernameField();  //  glass rounded input
            BuildPasswordField();  //  glass rounded input + reveal btn
            BuildErrorLabel();     //  hidden until ShowError() called
            BuildLoginButton();    //  CTA — "Sign in as Admin →"
            BuildSignupRow();      //  "Don't have an account? Create one"
            // NOTE: Social (Google / Microsoft) buttons intentionally omitted
            //       per UI requirements.

            this->Controls->Add(pnlLeft);
        }


        // ── Brand row: red dot + "LifeBlood" wordmark ────────────────
        void BuildBrandRow()
        {
            Panel^ dot = gcnew Panel();
            dot->Size = Drawing::Size(28, 28);
            dot->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 28);
            dot->BackColor = CLR_ACCENT;
            dot->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnDotPaint);
            pnlLeft->Controls->Add(dot);

            Label^ brand = gcnew Label();
            brand->Text = "LifeBlood";
            brand->Font = gcnew Drawing::Font("Segoe UI", 11, FontStyle::Bold);
            brand->ForeColor = CLR_TEXT;
            brand->AutoSize = true;
            brand->Location = Drawing::Point(LEFT_MARGIN + 34, TOP_MARGIN + 32);
            brand->BackColor = Color::Transparent;
            pnlLeft->Controls->Add(brand);
        }

        // Circular red badge with a tiny white blood-drop inside
        void OnDotPaint(Object^ s, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Panel^ p = safe_cast<Panel^>(s);

            SolidBrush^ rb = gcnew SolidBrush(CLR_ACCENT);
            g->FillEllipse(rb, 0, 0, p->Width - 1, p->Height - 1);
            delete rb;

            // White micro drop centred in the badge
            DrawBloodDrop(g, p->Width / 2, p->Height / 2 + 1, 5);
        }


        // ── "✦ Welcome back" chip ─────────────────────────────────────
        void BuildWelcomeChip()
        {
            Panel^ chip = gcnew Panel();
            chip->Size = Drawing::Size(136, 28);
            chip->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 80);
            chip->BackColor = Color::FromArgb(255, 230, 235);  // matches old form
            chip->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnChipPaint);
            pnlLeft->Controls->Add(chip);

            Label^ lbl = gcnew Label();
            // UNICODE SAFE: four-pointed star via codepoint
            lbl->Text = Char::ConvertFromUtf32(0x2726) + "  Welcome back";
            lbl->Font = gcnew Drawing::Font("Segoe UI", 8, FontStyle::Regular);
            lbl->ForeColor = CLR_ACCENT;
            lbl->AutoSize = true;
            lbl->Location = Drawing::Point(8, 6);
            lbl->BackColor = Color::Transparent;
            chip->Controls->Add(lbl);
        }

        void OnChipPaint(Object^ s, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Panel^ p = safe_cast<Panel^>(s);
            Drawing::Rectangle r(0, 0, p->Width - 1, p->Height - 1);
            GraphicsPath^ path = RoundedRect(r, 12);
            SolidBrush^ bg =
                gcnew SolidBrush(Color::FromArgb(255, 230, 235));
            g->FillPath(bg, path);
            Pen^ pen = gcnew Pen(
                Color::FromArgb(80, CLR_ACCENT.R, CLR_ACCENT.G, CLR_ACCENT.B), 1);
            g->DrawPath(pen, path);
            delete bg; delete pen; delete path;
        }


        // ── Title block ───────────────────────────────────────────────
        void BuildTitleBlock()
        {
            Label^ title = gcnew Label();
            title->Text = "Sign in to\nLifeBlood MS";
            title->Font = gcnew Drawing::Font(
                "Segoe UI", 22, FontStyle::Bold);
            title->ForeColor = CLR_TEXT;
            title->AutoSize = true;
            title->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 120);
            title->BackColor = Color::Transparent;

            Label^ sub = gcnew Label();
            sub->Text = "Choose your role to continue to the right experience.";
            sub->Font = gcnew Drawing::Font("Segoe UI", 9);
            sub->ForeColor = CLR_MUTED;
            sub->AutoSize = true;
            sub->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 200);
            sub->BackColor = Color::Transparent;

            pnlLeft->Controls->Add(title);
            pnlLeft->Controls->Add(sub);
        }


        // ════════════════════════════════════════════════════════
        //  Role Switcher — 3 segmented buttons
        //
        //  INTEGER BOXING CONTRACT
        //  MakeRoleButton stores   b->Tag = (int)role;
        //  OnRoleClick   unboxes   (LoginRole)(int)b->Tag;
        //  Both casts are explicit C-style to prevent C2440.
        //
        //  Active  : CLR_ACCENT fill, white text, bold
        //  Inactive: CLR_BTN_IDLE_BG, dark text, regular
        // ════════════════════════════════════════════════════════
        void BuildRoleSwitcher()
        {
            Panel^ row = gcnew Panel();
            row->Size = Drawing::Size(368, 58);
            row->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 222);
            row->BackColor = CLR_BTN_IDLE_BG;
            row->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnSwitcherRowPaint);
            pnlLeft->Controls->Add(row);

            int BW = 368 / 3;

            // UNICODE SAFE: circled dot / heart / circled asterisk
            btnAdmin = MakeRoleButton(
                Char::ConvertFromUtf32(0x1F464) + "  Admin",
                LoginRole::Admin, 0, BW, row);

            btnDonor = MakeRoleButton(
                Char::ConvertFromUtf32(0x2665) + "  Donor",
                LoginRole::Donor, BW, BW, row);

            btnRecipient = MakeRoleButton(
                Char::ConvertFromUtf32(0x229B) + "  Recipient",
                LoginRole::Recipient, BW * 2, 368 - BW * 2, row);

            row->Controls->Add(btnAdmin);
            row->Controls->Add(btnDonor);
            row->Controls->Add(btnRecipient);
        }

        Button^ MakeRoleButton(String^ text, LoginRole role,
            int x, int w, Panel^ parent)
        {
            Button^ b = gcnew Button();
            b->Text = text;
            b->Size = Drawing::Size(w, 58);
            b->Location = Drawing::Point(x, 0);
            b->FlatStyle = FlatStyle::Flat;
            b->FlatAppearance->BorderSize = 0;
            b->BackColor = CLR_BTN_IDLE_BG;
            b->ForeColor = CLR_BTN_IDLE_FG;
            b->Font = gcnew Drawing::Font("Segoe UI", 9, FontStyle::Regular);
            b->Cursor = Cursors::Hand;

            // INTEGER BOXING: cast enum to int — avoids C2440
            b->Tag = (int)role;

            b->Click += gcnew EventHandler(this, &LoginForm::OnRoleClick);
            return b;
        }

        void OnSwitcherRowPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Drawing::Rectangle r(0, 0, p->Width - 1, p->Height - 1);
            GraphicsPath^ path = RoundedRect(r, 12);
            SolidBrush^ bg = gcnew SolidBrush(CLR_BTN_IDLE_BG);
            g->FillPath(bg, path);
            delete bg; delete path;
        }


        // ── Role hint text (below switcher) ──────────────────────────
        void BuildRoleHint()
        {
            lblRoleHint = gcnew Label();
            lblRoleHint->Text = "";
            lblRoleHint->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblRoleHint->ForeColor = CLR_MUTED;
            lblRoleHint->AutoSize = true;
            lblRoleHint->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 288);
            lblRoleHint->BackColor = Color::Transparent;
            pnlLeft->Controls->Add(lblRoleHint);
        }


        // ── Username glass input ──────────────────────────────────────
        void BuildUsernameField()
        {
            pnlUserBox = BuildInputBox(TOP_MARGIN + 316);
            txtUsername = BuildTextBox(pnlUserBox, "Username", false);
            pnlLeft->Controls->Add(pnlUserBox);
        }


        // ── Password glass input + eye-reveal toggle ──────────────────
        void BuildPasswordField()
        {
            pnlPassBox = BuildInputBox(TOP_MARGIN + 376);
            txtPassword = BuildTextBox(pnlPassBox, "Password", true);

            btnReveal = gcnew Button();
            // UNICODE SAFE: eye emoji via codepoint
            btnReveal->Text = Char::ConvertFromUtf32(0x1F441);
            btnReveal->Size = Drawing::Size(36, 36);
            btnReveal->Location =
                Drawing::Point(pnlPassBox->Width - 42, 7);
            btnReveal->FlatStyle = FlatStyle::Flat;
            btnReveal->FlatAppearance->BorderSize = 0;
            btnReveal->BackColor = Color::Transparent;
            btnReveal->ForeColor = CLR_HINT;
            btnReveal->Cursor = Cursors::Hand;
            btnReveal->Font = gcnew Drawing::Font("Segoe UI", 10);
            btnReveal->Click += gcnew EventHandler(
                this, &LoginForm::OnRevealClick);
            pnlPassBox->Controls->Add(btnReveal);

            pnlLeft->Controls->Add(pnlPassBox);
        }

        // Shared factory for a rounded glass input container
        Panel^ BuildInputBox(int y)
        {
            Panel^ box = gcnew Panel();
            box->Size = Drawing::Size(368, 52);
            box->Location = Drawing::Point(LEFT_MARGIN, y);
            box->BackColor = CLR_FIELD_BG;
            box->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnInputBoxPaint);
            return box;
        }

        // Shared factory for a borderless TextBox inside a glass box
        TextBox^ BuildTextBox(Panel^ box, String^ cue, bool isPassword)
        {
            TextBox^ t = gcnew TextBox();
            t->Size = Drawing::Size(box->Width - 60, 26);
            t->Location = Drawing::Point(14, 13);
            t->BorderStyle = BorderStyle::None;
            t->BackColor = CLR_FIELD_BG;
            t->ForeColor = CLR_TEXT;
            t->Font = gcnew Drawing::Font("Segoe UI", 11);
            t->Tag = cue;   // cue text stored for P/Invoke on Load

            if (isPassword)
            {
                t->PasswordChar = L'*';
                t->KeyDown += gcnew KeyEventHandler(
                    this, &LoginForm::OnPassKey);
            }
            else
            {
                t->KeyDown += gcnew KeyEventHandler(
                    this, &LoginForm::OnUserKey);
            }

            box->Controls->Add(t);
            return t;
        }

        void OnInputBoxPaint(Object^ s, PaintEventArgs^ e)
        {
            Panel^ p = safe_cast<Panel^>(s);
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Drawing::Rectangle r(0, 0, p->Width - 1, p->Height - 1);
            GraphicsPath^ path = RoundedRect(r, 12);
            SolidBrush^ bg = gcnew SolidBrush(CLR_FIELD_BG);
            g->FillPath(bg, path);
            Pen^ border = gcnew Pen(CLR_BORDER, 1.2f);
            g->DrawPath(border, path);
            delete bg; delete border; delete path;
        }


        // ── Error / status label ──────────────────────────────────────
        void BuildErrorLabel()
        {
            lblError = gcnew Label();
            lblError->Text = "";
            lblError->Font = gcnew Drawing::Font("Segoe UI", 9);
            lblError->ForeColor = CLR_ERROR;
            lblError->Size = Drawing::Size(368, 22);
            lblError->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 438);
            lblError->BackColor = Color::Transparent;
            lblError->Visible = false;
            pnlLeft->Controls->Add(lblError);
        }


        // ── CTA Login button ──────────────────────────────────────────
        void BuildLoginButton()
        {
            btnLogin = gcnew Button();
            // UNICODE SAFE: right arrow via codepoint
            btnLogin->Text = "Sign in as Admin  " + Char::ConvertFromUtf32(0x2192);
            btnLogin->Size = Drawing::Size(368, 52);
            btnLogin->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 468);
            btnLogin->FlatStyle = FlatStyle::Flat;
            btnLogin->FlatAppearance->BorderSize = 0;
            btnLogin->BackColor = CLR_ACCENT;
            btnLogin->ForeColor = Color::White;
            btnLogin->Font = gcnew Drawing::Font(
                "Segoe UI", 11, FontStyle::Bold);
            btnLogin->Cursor = Cursors::Hand;

            btnLogin->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnLoginBtnPaint);
            btnLogin->MouseEnter += gcnew EventHandler(
                this, &LoginForm::OnBtnEnter);
            btnLogin->MouseLeave += gcnew EventHandler(
                this, &LoginForm::OnBtnLeave);
            btnLogin->Click += gcnew EventHandler(
                this, &LoginForm::OnLoginClick);

            pnlLeft->Controls->Add(btnLogin);
        }

        void OnLoginBtnPaint(Object^ s, PaintEventArgs^ e)
        {
            // Rounded fill — GDI+ overrides default square rendering
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Button^ b = safe_cast<Button^>(s);

            Drawing::Rectangle r(0, 0, b->Width, b->Height);
            GraphicsPath^ path = RoundedRect(r, 12);
            SolidBrush^ bg = gcnew SolidBrush(b->BackColor);
            g->FillPath(bg, path);
            delete bg;

            StringFormat^ sf = gcnew StringFormat();
            sf->Alignment = StringAlignment::Center;
            sf->LineAlignment = StringAlignment::Center;
            SolidBrush^ fg = gcnew SolidBrush(Color::White);
            g->DrawString(b->Text, b->Font, fg,
                RectangleF(0, 0, (float)b->Width, (float)b->Height), sf);
            delete fg; delete sf; delete path;
        }


        // ── Signup row ────────────────────────────────────────────────
        void BuildSignupRow()
        {
            LinkLabel^ lnk = gcnew LinkLabel();
            lnk->Text = "Don't have an account?  Create one";
            lnk->Font = gcnew Drawing::Font("Segoe UI", 9);
            lnk->ForeColor = CLR_MUTED;
            lnk->BackColor = Color::Transparent;
            lnk->LinkColor = CLR_ACCENT;
            lnk->VisitedLinkColor = CLR_ACCENT;
            lnk->ActiveLinkColor = CLR_ERROR;
            lnk->LinkBehavior = LinkBehavior::HoverUnderline;
            lnk->LinkArea =
                System::Windows::Forms::LinkArea(24, 10);   // "Create one"
            lnk->Size = Drawing::Size(368, 28);
            lnk->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 530);
            lnk->TextAlign = ContentAlignment::MiddleCenter;
            lnk->LinkClicked += gcnew LinkLabelLinkClickedEventHandler(
                this, &LoginForm::OnSignupClick);
            pnlLeft->Controls->Add(lnk);
        }


        // ════════════════════════════════════════════════════════
        //  CLOSE BUTTON — screen-level ✕ (top-right corner)
        //  Floats above both panels; calls Application::Exit().
        // ════════════════════════════════════════════════════════
        void BuildCloseButton()
        {
            btnClose = gcnew Button();
            // UNICODE SAFE: multiplication sign as close glyph
            btnClose->Text = Char::ConvertFromUtf32(0x2715);
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 12);
            btnClose->Size = Drawing::Size(48, 48);
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->FlatAppearance->MouseOverBackColor = Color::Red;
            btnClose->BackColor = Color::Transparent;
            btnClose->ForeColor = Color::Black;
            btnClose->Cursor = Cursors::Hand;
            btnClose->Anchor =
                static_cast<AnchorStyles>(
                    AnchorStyles::Top | AnchorStyles::Right);
            btnClose->Location = Drawing::Point(this->ClientSize.Width - btnClose->Width, 0);
            btnClose->Click += gcnew EventHandler(this, &LoginForm::OnClose);
            btnClose->MouseEnter += gcnew EventHandler(this, &LoginForm::OnCloseBtnEnter);
            btnClose->MouseLeave += gcnew EventHandler(this, &LoginForm::OnCloseBtnLeave);

            this->Controls->Add(btnClose);   // add to Form, not to a panel
            btnClose->BringToFront();
        }


        // ════════════════════════════════════════════════════════
        //  SwitchRole  — CORE STATE MACHINE
        //
        //  The single source of truth for every dynamic element.
        //  Updates in order:
        //    1. Reset all three role buttons to idle state
        //    2. Apply active styling to the selected button
        //    3. Update left-panel helper text (lblRoleHint)
        //    4. Update right-panel dynamic labels + repaint
        //    5. Update btnLogin text with correct role + arrow
        //    6. Reset hover-fade and clear any visible error
        //
        //  UNICODE SAFE: arrows and special chars use codepoints.
        //  NO LAMBDAS: explicit per-button statements instead.
        // ════════════════════════════════════════════════════════
        void SwitchRole(LoginRole newRole)
        {
            currentRole = newRole;

            // ── Step 1: Reset all buttons to idle ───────────────
            btnAdmin->BackColor = CLR_BTN_IDLE_BG;
            btnAdmin->ForeColor = CLR_BTN_IDLE_FG;
            btnAdmin->Font = gcnew Drawing::Font(
                "Segoe UI", 9, FontStyle::Regular);

            btnDonor->BackColor = CLR_BTN_IDLE_BG;
            btnDonor->ForeColor = CLR_BTN_IDLE_FG;
            btnDonor->Font = gcnew Drawing::Font(
                "Segoe UI", 9, FontStyle::Regular);

            btnRecipient->BackColor = CLR_BTN_IDLE_BG;
            btnRecipient->ForeColor = CLR_BTN_IDLE_FG;
            btnRecipient->Font = gcnew Drawing::Font(
                "Segoe UI", 9, FontStyle::Regular);

            // ── Step 2-5: Role-specific content ─────────────────
            String^ roleHint;
            String^ portalChip;
            String^ heading;
            String^ sub;

            switch (newRole)
            {
                // ────────────────────────────────────────────────────
            case LoginRole::Admin:
                btnAdmin->BackColor = CLR_ACCENT;
                btnAdmin->ForeColor = Color::White;
                btnAdmin->Font = gcnew Drawing::Font(
                    "Segoe UI", 9, FontStyle::Bold);

                roleHint = "Manage inventory, requests & reports";
                portalChip = Char::ConvertFromUtf32(0x2299)
                    + "  Admin portal";
                heading = "Coordinate the lifeline of every"
                    " hospital you serve.";
                sub = "Track inventory across 8 blood groups, approve"
                    " urgent requests, and prevent expiry with"
                    " smart alerts.";
                btnLogin->Text =
                    "Sign in as Admin  " + Char::ConvertFromUtf32(0x2192);
                break;

                // ────────────────────────────────────────────────────
            case LoginRole::Donor:
                btnDonor->BackColor = CLR_ACCENT;
                btnDonor->ForeColor = Color::White;
                btnDonor->Font = gcnew Drawing::Font(
                    "Segoe UI", 9, FontStyle::Bold);

                roleHint = "Save lives. Track your impact.";
                portalChip = Char::ConvertFromUtf32(0x2665)
                    + "  Donor portal";
                // UNICODE SAFE: right-single-quotation mark via codepoint
                heading = "Every drop you give writes a chapter in"
                    " someone"
                    + Char::ConvertFromUtf32(0x2019)
                    + "s survival.";
                sub = "Log donations, watch lives saved climb,"
                    " and get notified when your blood is"
                    " urgently needed.";
                btnLogin->Text =
                    "Sign in as Donor  " + Char::ConvertFromUtf32(0x2192);
                break;

                // ────────────────────────────────────────────────────
            case LoginRole::Recipient:
            default:
                btnRecipient->BackColor = CLR_ACCENT;
                btnRecipient->ForeColor = Color::White;
                btnRecipient->Font = gcnew Drawing::Font(
                    "Segoe UI", 9, FontStyle::Bold);

                roleHint = "Request blood when it matters most.";
                portalChip = Char::ConvertFromUtf32(0x229B)
                    + "  Recipient portal";
                // UNICODE SAFE: em-dash via codepoint
                heading = "Help is always one request away "
                    + Char::ConvertFromUtf32(0x2014)
                    + " never alone.";
                sub = "Search nearby availability, submit requests"
                    " in seconds, and follow them in real time.";
                btnLogin->Text =
                    "Sign in as Recipient  " + Char::ConvertFromUtf32(0x2192);
                break;
            }

            // ── Update left panel ────────────────────────────────
            lblRoleHint->Text = roleHint;

            // ── Update right panel ───────────────────────────────
            lblPortalChip->Text = portalChip;
            lblRightHeading->Text = heading;
            lblRightSub->Text = sub;
            PositionRightLabels();

            // ── Step 6: Reset hover-fade & clear error ───────────
            _fadeT = 0.0f;
            btnLogin->BackColor = CLR_ACCENT;
            btnLogin->Invalidate();

            if (lblError != nullptr)
                lblError->Visible = false;
        }


        // ════════════════════════════════════════════════════════
        //  PositionRightLabels
        //  All right-panel labels are positioned as fractions of
        //  pnlRight's live dimensions so the layout scales
        //  correctly when the window is resized.
        //  Called by SwitchRole() and OnResize().
        // ════════════════════════════════════════════════════════
        void PositionRightLabels()
        {
            if (pnlRight == nullptr) return;

            int W = pnlRight->Width;
            int H = pnlRight->Height;
            int margin = (int)(W * 0.08);

            lblPortalChip->Location =
                Drawing::Point(margin, (int)(H * 0.62));

            lblRightHeading->Size =
                Drawing::Size(W - margin * 2, 120);
            lblRightHeading->Location =
                Drawing::Point(margin, (int)(H * 0.62) + 32);

            lblRightSub->Size =
                Drawing::Size(W - margin * 2, 70);
            lblRightSub->Location =
                Drawing::Point(margin, (int)(H * 0.62) + 164);

            int statY = H - 70;
            int divY = H - 90;
            int slotW = (W - margin * 2) / 3;

            // Divider line + stat labels — located by Tag strings.
            // "for each" loop is the no-lambda-safe iteration form.
            for each (Control ^ c in pnlRight->Controls)
            {
                if (c->Tag == nullptr) continue;

                String^ tag = c->Tag->ToString();

                if (tag == "divider")
                {
                    c->Size = Drawing::Size(W - margin * 2, 1);
                    c->Location = Drawing::Point(margin, divY);
                }
                else if (tag == "stat_val_0")
                    c->Location = Drawing::Point(margin + slotW * 0, statY);
                else if (tag == "stat_cap_0")
                    c->Location = Drawing::Point(margin + slotW * 0, statY + 36); // Increased from 28
                else if (tag == "stat_val_1")
                    c->Location = Drawing::Point(margin + slotW * 1, statY);
                else if (tag == "stat_cap_1")
                    c->Location = Drawing::Point(margin + slotW * 1, statY + 36); // Increased from 28
                else if (tag == "stat_val_2")
                    c->Location = Drawing::Point(margin + slotW * 2, statY);
                else if (tag == "stat_cap_2")
                    c->Location = Drawing::Point(margin + slotW * 2, statY + 36); // Increased from 28
            }
        }


        // ════════════════════════════════════════════════════════
        //  EVENT HANDLERS
        // ════════════════════════════════════════════════════════

        // ── Form Load ────────────────────────────────────────────
        void OnLoad(Object^ sender, EventArgs^ e)
        {
            LayoutPanels();
            PositionRightLabels();

            // Win32 cue banners (grey placeholder text in TextBoxes)
            const UInt32 EM_SETCUEBANNER = 0x1501u;
            SendCue(txtUsername->Handle, EM_SETCUEBANNER,
                IntPtr(1), "Email or Username");
            SendCue(txtPassword->Handle, EM_SETCUEBANNER,
                IntPtr(1), "Password");

            // DB startup health-check
            if (!Database::GetInstance()->TestConnection())
            {
                MessageBox::Show(
                    "Cannot reach the SQL Server database.\n\n"
                    "Verify that TALHA\\SQLEXPRESS is running and "
                    "'db_bloodbank' exists.",
                    "LifeBlood MS — Database Unreachable",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Error);
            }

            _animTimer->Start();
            txtUsername->Focus();
        }


        // ── Form Resize ───────────────────────────────────────────
        void OnResize(Object^ sender, EventArgs^ e)
        {
            LayoutPanels();
            PositionRightLabels();
        }


        // ── Role button click ─────────────────────────────────────
        //
        //  INTEGER UNBOXING:
        //    Tag was stored as (int)role in MakeRoleButton.
        //    Unbox as: (LoginRole)(int)b->Tag
        //    The explicit C-style double-cast prevents C2440.
        // ─────────────────────────────────────────────────────────
        void OnRoleClick(Object^ sender, EventArgs^ e)
        {
            Button^ b = (Button^)sender;
            LoginRole selRole = (LoginRole)(int)b->Tag;
            SwitchRole(selRole);
        }


        // ── btnLogin hover fade (RGB lerp) ────────────────────────
        Color LerpColor(Color a, Color b, float t)
        {
            int r = Math::Max(0, Math::Min(255,
                (int)(a.R + (b.R - a.R) * t)));
            int gr = Math::Max(0, Math::Min(255,
                (int)(a.G + (b.G - a.G) * t)));
            int bl = Math::Max(0, Math::Min(255,
                (int)(a.B + (b.B - a.B) * t)));
            return Color::FromArgb(255, r, gr, bl);
        }

        void OnFadeTick(Object^ sender, EventArgs^ e)
        {
            _fadeT += _fadeDir * FADE_STEP;
            if (_fadeT >= 1.0f) { _fadeT = 1.0f; _fadeTimer->Stop(); }
            else if (_fadeT <= 0.0f) { _fadeT = 0.0f; _fadeTimer->Stop(); }
            btnLogin->BackColor = LerpColor(CLR_ACCENT, CLR_ACCENT_DARK, _fadeT);
            btnLogin->Invalidate();
        }

        void OnBtnEnter(Object^ sender, EventArgs^ e)
        {
            _fadeDir = +1;
            _fadeTimer->Start();
        }

        void OnBtnLeave(Object^ sender, EventArgs^ e)
        {
            _fadeDir = -1;
            _fadeTimer->Start();
        }


        // ── Password reveal toggle ────────────────────────────────
        void OnRevealClick(Object^ sender, EventArgs^ e)
        {
            if (txtPassword->PasswordChar == L'\0')
            {
                txtPassword->PasswordChar = L'*';
                btnReveal->ForeColor = CLR_HINT;
            }
            else
            {
                txtPassword->PasswordChar = L'\0';
                btnReveal->ForeColor = CLR_ACCENT;
            }
        }


        // ── Close button ──────────────────────────────────────────
        void OnClose(Object^ s, EventArgs^ e)
        {
            _animTimer->Stop();
            Application::Exit();
        }

        void OnCloseBtnEnter(Object^ s, EventArgs^ e)
        {
            btnClose->ForeColor = CLR_HOVER_BG;  // warm pink tint on hover
        }

        void OnCloseBtnLeave(Object^ s, EventArgs^ e)
        {
            btnClose->ForeColor = Color::White;
        }


        // ── Remember-me colour tint ───────────────────────────────
        // (declared with BuildRememberRow above; kept near its control)


        // ── Keyboard shortcuts ────────────────────────────────────
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


        // ── Signup navigation ─────────────────────────────────────
        void OnSignupClick(Object^ sender, LinkLabelLinkClickedEventArgs^ e)
        {
            this->Hide();
            SignupForm^ sf = gcnew SignupForm();
            sf->FormClosed += gcnew FormClosedEventHandler(
                this, &LoginForm::OnChildClosed);
            sf->Show();
        }


        // ── Restore login form when any child window closes ───────
        void OnChildClosed(Object^ s, FormClosedEventArgs^ e)
        {
            txtUsername->Clear();
            txtPassword->Clear();
            lblError->Visible = false;
            _animTimer->Start();
            this->Show();
            txtUsername->Focus();
        }


        // ── Display an error message ──────────────────────────────
        void ShowError(String^ msg)
        {
            lblError->Text = msg;
            lblError->Visible = true;
        }


        // ════════════════════════════════════════════════════════
        //  OnLoginClick  — AUTHENTICATION + ROLE GUARD
        //
        //  SECURITY CONTRACT
        //  ─────────────────────────────────────────────────────
        //  Database::GetInstance()->ValidateLogin() returns the
        //  role stored in the DB for the authenticated user.
        //  That role MUST exactly match the role the user selected
        //  in the UI switcher.  Mismatch → login blocked with:
        //    "Unauthorized: You do not have <role> privileges."
        //
        //  This prevents a Donor account from escalating to Admin
        //  by simply clicking a different role button.
        // ════════════════════════════════════════════════════════
        void OnLoginClick(Object^ sender, EventArgs^ e)
        {
            // Reset any previous error
            lblError->Visible = false;
            lblError->Text = "";

            String^ uname = txtUsername->Text->Trim();
            String^ pwd = txtPassword->Text;

            // ── Guard: empty fields ──────────────────────────────
            if (String::IsNullOrWhiteSpace(uname) ||
                String::IsNullOrWhiteSpace(pwd))
            {
                // UNICODE SAFE: warning sign via codepoint
                ShowError(Char::ConvertFromUtf32(0x26A0)
                    + "  Username and password cannot be empty.");
                return;
            }

            // ── Database authentication (singleton pattern) ──────
            String^ dbRole = nullptr;
            String^ fullName = nullptr;
            int     userID = 0;

            bool ok = Database::GetInstance()->ValidateLogin(
                uname, pwd, dbRole, userID, fullName);

            if (!ok)
            {
                ShowError(Char::ConvertFromUtf32(0x26A0)
                    + "  Invalid username or password. Please try again.");
                txtPassword->Clear();
                txtPassword->Focus();
                return;
            }

            // ── Role Guard ───────────────────────────────────────
            String^ selectedRoleStr;
            switch (currentRole)
            {
            case LoginRole::Admin:     selectedRoleStr = "Admin";     break;
            case LoginRole::Donor:     selectedRoleStr = "Donor";     break;
            case LoginRole::Recipient: selectedRoleStr = "Recipient"; break;
            default:                   selectedRoleStr = "";           break;
            }

            if (!dbRole->Equals(selectedRoleStr,
                StringComparison::OrdinalIgnoreCase))
            {
                // UNICODE SAFE: no-entry sign via codepoint
                ShowError(Char::ConvertFromUtf32(0x1F6AB)
                    + "  Unauthorized: You do not have "
                    + selectedRoleStr + " privileges.");
                txtPassword->Clear();
                txtPassword->Focus();
                return;
            }

            // ── Route to correct dashboard ───────────────────────
            _animTimer->Stop();
            this->Hide();

            if (dbRole->Equals("Admin", StringComparison::OrdinalIgnoreCase))
            {
                AdminDashboard^ d = gcnew AdminDashboard(userID, fullName);
                d->FormClosed += gcnew FormClosedEventHandler(
                    this, &LoginForm::OnChildClosed);
                d->Show();
            }
            else if (dbRole->Equals("Donor", StringComparison::OrdinalIgnoreCase))
            {
                DonorDashboard^ d = gcnew DonorDashboard(userID, fullName);
                d->FormClosed += gcnew FormClosedEventHandler(
                    this, &LoginForm::OnChildClosed);
                d->Show();
            }
            else if (dbRole->Equals("Recipient", StringComparison::OrdinalIgnoreCase))
            {
                RecipientDashboard^ rd =
                    gcnew RecipientDashboard(userID, fullName);
                rd->FormClosed += gcnew FormClosedEventHandler(
                    this, &LoginForm::OnChildClosed);
                rd->Show();
            }
            else
            {
                // Unknown DB role — should never reach here after the guard
                this->Show();
                _animTimer->Start();
                ShowError("Unknown role '" + dbRole + "'. Contact admin.");
            }
        }

    };  // ref class LoginForm

}  // namespace BloodBank


// ════════════════════════════════════════════════════════════════
//  ENTRY POINT WIRING  (main.cpp — unchanged)
//
//  [STAThread]
//  int main(array<String^>^ args)
//  {
//      Application::EnableVisualStyles();
//      Application::SetCompatibleTextRenderingDefault(false);
//      Application::Run(gcnew BloodBank::LoginForm());
//      return 0;
//  }
// ════════════════════════════════════════════════════════════════
//
//  WHAT CHANGED vs. loginForm.h (old form)
//  ──────────────────────────────────────────────────────────────
//  REMOVED
//    • SqlConnection / connectionString field & ValidateUser()
//    • Hard-coded 1200×800 fixed window size
//    • "Continue with Google" / "Continue with Microsoft" buttons
//    • "OR" social-login divider
//    • Placeholder-text simulation (••••••••, gray forecolor trick)
//
//  ADDED
//    • enum class LoginRole { Admin, Donor, Recipient }
//    • SwitchRole(LoginRole) state machine
//    • 3-button role switcher with active / idle GDI+ painting
//    • Role hint label (context sentence below switcher)
//    • Database::GetInstance()->ValidateLogin() — singleton pattern
//    • Role Guard: DB role != UI role → blocked with error
//    • FormWindowState::Maximized + FormBorderStyle::None
//    • LayoutPanels() / OnResize() — responsive 50/50 split
//    • Floating blood-type pills (A+, O-, O+, B+) with sine drift
//    • _animTimer (~60 fps) driving heartbeat + pill animation
//    • _fadeTimer driving btnLogin hover RGB lerp
//    • Dynamic right-panel role text (lblPortalChip / heading / sub)
//    • Stats footer (12,450 Donors / 3,280 Units / 47 Hospitals)
//    • Win32 SendCue / EM_SETCUEBANNER P/Invoke for placeholder text
//    • btnClose (✕) — top-right, calls Application::Exit()
//    • Dashboard routing with FormClosed chain for all three roles
//
//  PRESERVED 1-FOR-1 FROM OLD FORM
//    • Heartbeat / EKG icon (DrawHeartbeatIcon — exact Bezier code)
//    • Right-panel gradient colors (210,10,45 → 60,20,35 diagonal)
//    • Central glass card dimensions and translucency values
//    • CLR_ACCENT = rgb(232, 15, 59) brand crimson
//    • lblLogo background tint = rgb(255, 230, 235)
//    • 50/50 left / right split layout
// ════════════════════════════════════════════════════════════════