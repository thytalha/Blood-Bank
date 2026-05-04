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

    enum class LoginRole { Admin, Donor, Recipient };

    public ref class LoginForm : public Form
    {

    private:
        [DllImport("user32.dll", EntryPoint = "SendMessageW",
            CharSet = CharSet::Unicode)]
            static IntPtr SendCue(IntPtr hWnd, UInt32 msg,
                IntPtr wp, String^ lp);

        Color CLR_WHITE;
        Color CLR_TEXT;

        Color CLR_MUTED;
        Color CLR_HINT;

        Color CLR_ACCENT;
        Color CLR_ACCENT_DARK;
        Color CLR_ERROR;

        Color CLR_BTN_ACTIVE_BG;
        Color CLR_BTN_IDLE_BG;
        Color CLR_BTN_IDLE_FG;

        Color CLR_BORDER;
        Color CLR_FIELD_BG;
        Color CLR_HOVER_BG;

        Color CLR_RIGHT_TOP;
        Color CLR_RIGHT_BOT;

        int LEFT_MARGIN = 120;

        int TOP_MARGIN = 60;

        LoginRole currentRole;

        Panel^ pnlLeft;
        Panel^ pnlRight;

        Button^ btnAdmin;
        Button^ btnDonor;
        Button^ btnRecipient;
        Label^ lblRoleHint;

        Panel^ pnlUserBox;
        Panel^ pnlPassBox;
        TextBox^ txtUsername;
        TextBox^ txtPassword;
        Button^ btnReveal;

        Button^ btnLogin;

        LinkLabel^ lnkSignup;

        Label^ lblError;

        Button^ btnClose;

        Label^ lblPortalChip;
        Label^ lblRightHeading;
        Label^ lblRightSub;

        Timer^ _fadeTimer;
        float       _fadeT;
        int         _fadeDir;
        const float FADE_STEP = 0.08f;

        Timer^ _animTimer;
        double          _animTick;

        array<String^>^ _pillLabels;
        array<float>^ _pillX;
        array<float>^ _pillBaseY;
        array<float>^ _pillPhase;

    public:
        LoginForm()
        {
            CLR_WHITE = Color::White;
            CLR_TEXT = ColorTranslator::FromHtml("#1A0A0A");
            CLR_MUTED = ColorTranslator::FromHtml("#6C757D");
            CLR_HINT = ColorTranslator::FromHtml("#ADB5BD");

            CLR_ACCENT = Color::FromArgb(232, 15, 59);
            CLR_ACCENT_DARK = Color::FromArgb(180, 10, 45);
            CLR_ERROR = ColorTranslator::FromHtml("#E74C3C");

            CLR_BTN_ACTIVE_BG = CLR_ACCENT;
            CLR_BTN_IDLE_BG = ColorTranslator::FromHtml("#F0F0F0");
            CLR_BTN_IDLE_FG = ColorTranslator::FromHtml("#555555");

            CLR_BORDER = ColorTranslator::FromHtml("#E9ECEF");
            CLR_FIELD_BG = Color::FromArgb(248, 248, 250);
            CLR_HOVER_BG = Color::FromArgb(255, 230, 235);

            CLR_RIGHT_TOP = Color::FromArgb(210, 10, 45);
            CLR_RIGHT_BOT = Color::FromArgb(60, 20, 35);

            _pillLabels = gcnew array<String^>{ "A+", "O-", "O+", "B+" };
            _pillX = gcnew array<float>{ 0.06f, 0.60f, 0.52f, 0.08f };
            _pillBaseY = gcnew array<float>{ 0.08f, 0.18f, 0.31f, 0.44f };
            _pillPhase = gcnew array<float>{ 0.0f, 1.2f, 2.4f, 3.6f };

            _fadeTimer = gcnew Timer();
            _fadeTimer->Interval = 12;
            _fadeTimer->Tick += gcnew EventHandler(this, &LoginForm::OnFadeTick);
            _fadeT = 0.0f;
            _fadeDir = 0;

            _animTimer = gcnew Timer();
            _animTimer->Interval = 16;
            _animTick = 0.0;
            _animTimer->Tick += gcnew EventHandler(this, &LoginForm::OnAnimTick);

            currentRole = LoginRole::Admin;

            InitializeComponent();
        }

    protected:
        ~LoginForm() {}

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

            BuildRightPanel();
            BuildLeftPanel();
            BuildCloseButton();

            SwitchRole(LoginRole::Admin);
        }

        void LayoutPanels()
        {
            int W = this->ClientSize.Width;
            int H = this->ClientSize.Height;
            int half = W / 2;

            pnlLeft->SetBounds(0, 0, half, H);
            pnlRight->SetBounds(half, 0, W - half, H);

            btnClose->Location =
                Drawing::Point(this->ClientSize.Width - 48, 0);
        }

        void BuildRightPanel()
        {
            pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::None;
            pnlRight->BackColor = CLR_RIGHT_TOP;

            pnlRight->GetType()->GetProperty("DoubleBuffered",
                System::Reflection::BindingFlags::Instance |
                System::Reflection::BindingFlags::NonPublic)
                ->SetValue(pnlRight, true);

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

            BuildStatsFooter();

            pnlRight->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnRightPaint);

            this->Controls->Add(pnlRight);
        }

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

        void BuildStatsFooter()
        {
            Panel^ div = gcnew Panel();
            div->BackColor = Color::FromArgb(60, 255, 255, 255);
            div->Size = Drawing::Size(1, 1);
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

        void OnRightPaint(Object^ sender, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            g->TextRenderingHint =
                System::Drawing::Text::TextRenderingHint::AntiAliasGridFit;

            int W = pnlRight->Width;
            int H = pnlRight->Height;

            Rectangle rect = pnlRight->ClientRectangle;
            LinearGradientBrush^ bgBrush = gcnew LinearGradientBrush(
                rect,
                CLR_RIGHT_TOP,
                CLR_RIGHT_BOT,
                LinearGradientMode::ForwardDiagonal);
            g->FillRectangle(bgBrush, rect);
            delete bgBrush;

            for (int i = 0; i < _pillLabels->Length; i++)
            {
                double drift = Math::Sin(_animTick + _pillPhase[i]) * 12.0;
                int px = (int)(_pillX[i] * W);
                int py = (int)(_pillBaseY[i] * H) + (int)drift;
                DrawPill(g, _pillLabels[i], px, py);
            }

            SolidBrush^ glassBrush =
                gcnew SolidBrush(Color::FromArgb(30, 255, 255, 255));
            Pen^ glassBorder =
                gcnew Pen(Color::FromArgb(50, 255, 255, 255), 1.5f);

            int CW = Math::Min(200, (int)(W * 0.38));
            int CH = (int)(CW * 1.15);
            int CX = (W - CW) / 2;
            int CY = (int)(H * 0.25) - CH / 2;

            Rectangle cardRect(CX, CY, CW, CH);
            GraphicsPath^ cardPath = RoundedRect(cardRect, 15);
            g->FillPath(glassBrush, cardPath);
            g->DrawPath(glassBorder, cardPath);
            delete glassBrush; delete glassBorder; delete cardPath;

            DrawHeartbeatIcon(g,
                Rectangle(
                    CX + 5,
                    CY + (int)(CH * 0.12f),
                    CW - 10,
                    (int)(CH * 0.75f)));
        }

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

            heart->AddBezier(
                pTop,
                Point(x + static_cast<int>(w * 0.18f), y - 2),
                Point(x - 2, y + static_cast<int>(h * 0.38f)),
                Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)));

            heart->AddBezier(
                Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)),
                Point(x + static_cast<int>(w * 0.34f), y + static_cast<int>(h * 0.74f)),
                Point(x + static_cast<int>(w * 0.43f), y + static_cast<int>(h * 0.88f)),
                pBottom);

            heart->AddBezier(
                pBottom,
                Point(x + static_cast<int>(w * 0.57f), y + static_cast<int>(h * 0.88f)),
                Point(x + static_cast<int>(w * 0.66f), y + static_cast<int>(h * 0.74f)),
                Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)));

            heart->AddBezier(
                Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)),
                Point(x + w + 2, y + static_cast<int>(h * 0.38f)),
                Point(x + static_cast<int>(w * 0.82f), y - 2),
                pTop);

            g->DrawPath(iconPen, heart);

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

        void DrawPill(Graphics^ g, String^ label, int x, int y)
        {
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

            SolidBrush^ glint =
                gcnew SolidBrush(Color::FromArgb(90, 255, 255, 255));
            g->FillEllipse(glint,
                (float)(cx - r * 0.20f), (float)(cy - r * 0.55f),
                (float)(r * 0.28f), (float)(r * 0.38f));

            delete wb; delete glint; delete drop;
        }

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

        void OnAnimTick(Object^ sender, EventArgs^ e)
        {
            _animTick += 0.018;
            if (_animTick > Math::PI * 200) _animTick = 0.0;
            pnlRight->Invalidate();
        }

        void BuildLeftPanel()
        {
            pnlLeft = gcnew Panel();
            pnlLeft->BackColor = CLR_WHITE;

            BuildBrandRow();
            BuildWelcomeChip();
            BuildTitleBlock();
            BuildRoleSwitcher();
            BuildRoleHint();
            BuildUsernameField();
            BuildPasswordField();
            BuildErrorLabel();
            BuildLoginButton();
            BuildSignupRow();

            this->Controls->Add(pnlLeft);
        }

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

        void OnDotPaint(Object^ s, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Panel^ p = safe_cast<Panel^>(s);

            SolidBrush^ rb = gcnew SolidBrush(CLR_ACCENT);
            g->FillEllipse(rb, 0, 0, p->Width - 1, p->Height - 1);
            delete rb;

            DrawBloodDrop(g, p->Width / 2, p->Height / 2 + 1, 5);
        }

        void BuildWelcomeChip()
        {
            Panel^ chip = gcnew Panel();
            chip->Size = Drawing::Size(136, 28);
            chip->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 80);
            chip->BackColor = Color::FromArgb(255, 230, 235);
            chip->Paint += gcnew PaintEventHandler(
                this, &LoginForm::OnChipPaint);
            pnlLeft->Controls->Add(chip);

            Label^ lbl = gcnew Label();
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

        void BuildUsernameField()
        {
            pnlUserBox = BuildInputBox(TOP_MARGIN + 316);
            txtUsername = BuildTextBox(pnlUserBox, "Username", false);
            pnlLeft->Controls->Add(pnlUserBox);
        }

        void BuildPasswordField()
        {
            pnlPassBox = BuildInputBox(TOP_MARGIN + 376);
            txtPassword = BuildTextBox(pnlPassBox, "Password", true);

            btnReveal = gcnew Button();
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

        TextBox^ BuildTextBox(Panel^ box, String^ cue, bool isPassword)
        {
            TextBox^ t = gcnew TextBox();
            t->Size = Drawing::Size(box->Width - 60, 26);
            t->Location = Drawing::Point(14, 13);
            t->BorderStyle = BorderStyle::None;
            t->BackColor = CLR_FIELD_BG;
            t->ForeColor = CLR_TEXT;
            t->Font = gcnew Drawing::Font("Segoe UI", 11);
            t->Tag = cue;

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

        void BuildLoginButton()
        {
            btnLogin = gcnew Button();
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
                System::Windows::Forms::LinkArea(24, 10);
            lnk->Size = Drawing::Size(368, 28);
            lnk->Location = Drawing::Point(LEFT_MARGIN, TOP_MARGIN + 530);
            lnk->TextAlign = ContentAlignment::MiddleCenter;
            lnk->LinkClicked += gcnew LinkLabelLinkClickedEventHandler(
                this, &LoginForm::OnSignupClick);
            pnlLeft->Controls->Add(lnk);
        }

        void BuildCloseButton()
        {
            btnClose = gcnew Button();
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

            this->Controls->Add(btnClose);
            btnClose->BringToFront();
        }

        void SwitchRole(LoginRole newRole)
        {
            currentRole = newRole;

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

            String^ roleHint;
            String^ portalChip;
            String^ heading;
            String^ sub;

            switch (newRole)
            {
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

            case LoginRole::Donor:
                btnDonor->BackColor = CLR_ACCENT;
                btnDonor->ForeColor = Color::White;
                btnDonor->Font = gcnew Drawing::Font(
                    "Segoe UI", 9, FontStyle::Bold);

                roleHint = "Save lives. Track your impact.";
                portalChip = Char::ConvertFromUtf32(0x2665)
                    + "  Donor portal";
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

            case LoginRole::Recipient:
            default:
                btnRecipient->BackColor = CLR_ACCENT;
                btnRecipient->ForeColor = Color::White;
                btnRecipient->Font = gcnew Drawing::Font(
                    "Segoe UI", 9, FontStyle::Bold);

                roleHint = "Request blood when it matters most.";
                portalChip = Char::ConvertFromUtf32(0x229B)
                    + "  Recipient portal";
                heading = "Help is always one request away "
                    + Char::ConvertFromUtf32(0x2014)
                    + " never alone.";
                sub = "Search nearby availability, submit requests"
                    " in seconds, and follow them in real time.";
                btnLogin->Text =
                    "Sign in as Recipient  " + Char::ConvertFromUtf32(0x2192);
                break;
            }

            lblRoleHint->Text = roleHint;

            lblPortalChip->Text = portalChip;
            lblRightHeading->Text = heading;
            lblRightSub->Text = sub;
            PositionRightLabels();

            _fadeT = 0.0f;
            btnLogin->BackColor = CLR_ACCENT;
            btnLogin->Invalidate();

            if (lblError != nullptr)
                lblError->Visible = false;
        }

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
                    c->Location = Drawing::Point(margin + slotW * 0, statY + 36);
                else if (tag == "stat_val_1")
                    c->Location = Drawing::Point(margin + slotW * 1, statY);
                else if (tag == "stat_cap_1")
                    c->Location = Drawing::Point(margin + slotW * 1, statY + 36);
                else if (tag == "stat_val_2")
                    c->Location = Drawing::Point(margin + slotW * 2, statY);
                else if (tag == "stat_cap_2")
                    c->Location = Drawing::Point(margin + slotW * 2, statY + 36);
            }
        }

        void OnLoad(Object^ sender, EventArgs^ e)
        {
            LayoutPanels();
            PositionRightLabels();

            const UInt32 EM_SETCUEBANNER = 0x1501u;
            SendCue(txtUsername->Handle, EM_SETCUEBANNER,
                IntPtr(1), "Email or Username");
            SendCue(txtPassword->Handle, EM_SETCUEBANNER,
                IntPtr(1), "Password");

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

        void OnResize(Object^ sender, EventArgs^ e)
        {
            LayoutPanels();
            PositionRightLabels();
        }

        void OnRoleClick(Object^ sender, EventArgs^ e)
        {
            Button^ b = (Button^)sender;
            LoginRole selRole = (LoginRole)(int)b->Tag;
            SwitchRole(selRole);
        }

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

        void OnClose(Object^ s, EventArgs^ e)
        {
            _animTimer->Stop();
            Application::Exit();
        }

        void OnCloseBtnEnter(Object^ s, EventArgs^ e)
        {
            btnClose->ForeColor = CLR_HOVER_BG;
        }

        void OnCloseBtnLeave(Object^ s, EventArgs^ e)
        {
            btnClose->ForeColor = Color::White;
        }

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

        void OnSignupClick(Object^ sender, LinkLabelLinkClickedEventArgs^ e)
        {
            this->Hide();
            SignupForm^ sf = gcnew SignupForm();
            sf->FormClosed += gcnew FormClosedEventHandler(
                this, &LoginForm::OnChildClosed);
            sf->Show();
        }

        void OnChildClosed(Object^ s, FormClosedEventArgs^ e)
        {
            txtUsername->Clear();
            txtPassword->Clear();
            lblError->Visible = false;
            _animTimer->Start();
            this->Show();
            txtUsername->Focus();
        }

        void ShowError(String^ msg)
        {
            lblError->Text = msg;
            lblError->Visible = true;
        }

        void OnLoginClick(Object^ sender, EventArgs^ e)
        {
            lblError->Visible = false;
            lblError->Text = "";

            String^ uname = txtUsername->Text->Trim();
            String^ pwd = txtPassword->Text;

            if (String::IsNullOrWhiteSpace(uname) ||
                String::IsNullOrWhiteSpace(pwd))
            {
                ShowError(Char::ConvertFromUtf32(0x26A0)
                    + "  Username and password cannot be empty.");
                return;
            }

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
                ShowError(Char::ConvertFromUtf32(0x1F6AB)
                    + "  Unauthorized: You do not have "
                    + selectedRoleStr + " privileges.");
                txtPassword->Clear();
                txtPassword->Focus();
                return;
            }

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
                this->Show();
                _animTimer->Start();
                ShowError("Unknown role '" + dbRole + "'. Contact admin.");
            }
        }

    };

}