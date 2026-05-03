#pragma once
#include "Database.h"

#using <System.dll>
#using <System.Data.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Data::SqlClient;

namespace BloodBank { // Switched to your project's namespace

    // ─────────────────────────────────────────────
    // Helper: Rounded Rectangle path generator
    // ─────────────────────────────────────────────
    ref class RoundedPanel : public Panel {
    public:
        int CornerRadius;
        Color BorderColor;

        RoundedPanel() {
            CornerRadius = 16;
            BorderColor = Color::FromArgb(180, 220, 38, 38);
            this->SetStyle(ControlStyles::UserPaint |
                ControlStyles::AllPaintingInWmPaint |
                ControlStyles::DoubleBuffer, true);
        }

    protected:
        virtual void OnPaint(PaintEventArgs^ e) override {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            Drawing::Rectangle rect = Drawing::Rectangle(
                0, 0, this->Width - 1, this->Height - 1);

            GraphicsPath^ path = GetRoundedPath(rect, CornerRadius);

            // Fill background
            SolidBrush^ fill = gcnew SolidBrush(this->BackColor);
            g->FillPath(fill, path);

            // Draw border
            Pen^ pen = gcnew Pen(BorderColor, 1.2f);
            g->DrawPath(pen, path);

            // Clip children to rounded region
            this->Region = gcnew System::Drawing::Region(path);
            Panel::OnPaint(e);
        }

    private:
        GraphicsPath^ GetRoundedPath(Drawing::Rectangle r, int radius) {
            GraphicsPath^ path = gcnew GraphicsPath();
            int d = radius * 2;
            path->AddArc(r.X, r.Y, d, d, 180, 90);
            path->AddArc(r.Right - d, r.Y, d, d, 270, 90);
            path->AddArc(r.Right - d, r.Bottom - d, d, d, 0, 90);
            path->AddArc(r.X, r.Bottom - d, d, d, 90, 90);
            path->CloseFigure();
            return path;
        }
    };

    // ─────────────────────────────────────────────
    // Main About Form
    // ─────────────────────────────────────────────
    public ref class AboutForm : public Form {

        // ── Controls ──────────────────────────────
        Label^ lblTitle;
        Label^ lblSubtitle;
        RoundedPanel^ pnlMission;
        Panel^ pnlBanner;
        Label^ lblMissionHeading;
        Label^ lblMissionText;
        Label^ lblDonorStat;

        // Value cards
        RoundedPanel^ pnlPrecision;
        RoundedPanel^ pnlCompassion;
        RoundedPanel^ pnlSpeed;

        // Team section
        RoundedPanel^ pnlTeam;

        // Close Button
        Button^ btnClose;

        // Colors
        Color burgundy = Color::FromArgb(127, 0, 36);
        Color lightBurgundy = Color::FromArgb(190, 30, 60);
        Color cardBg = Color::White;
        Color pageBg = Color::FromArgb(248, 246, 246);
        Color textDark = Color::FromArgb(50, 20, 20);
        Color textGray = Color::FromArgb(110, 100, 100);

    public:
        AboutForm() {
            this->Text = "About LifeBlood";
            this->Size = Drawing::Size(1200, 780);
            this->BackColor = pageBg;
            this->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;

            InitializeTitle();
            InitializeCloseButton();
            InitializeMissionCard();
            InitializeValueCards();
            InitializeTeamSection();

            LoadDonorCount();  // Uses your Database.h!
        }

    private:

        // ── CLOSE BUTTON ──────────────────────────────
        void InitializeCloseButton() {
            btnClose = gcnew Button();
            btnClose->Text = Char::ConvertFromUtf32(0x2715); // ✕
            btnClose->Font = gcnew Drawing::Font("Segoe UI", 14, FontStyle::Bold);
            btnClose->Size = Drawing::Size(50, 50);
            btnClose->Location = Point(this->Width - 60, 10);
            btnClose->FlatStyle = FlatStyle::Flat;
            btnClose->FlatAppearance->BorderSize = 0;
            btnClose->ForeColor = textGray;
            btnClose->Cursor = Cursors::Hand;
            btnClose->Click += gcnew EventHandler(this, &AboutForm::OnCloseClick);

            this->Controls->Add(btnClose);
        }

        void OnCloseClick(Object^ sender, EventArgs^ e) {
            this->Close(); // Closes the popup and returns to landing page
        }

        // ── TITLE ─────────────────────────────────
        void InitializeTitle() {
            lblTitle = gcnew Label();
            lblTitle->Text = "About LifeBlood MS";
            lblTitle->Font = gcnew Drawing::Font("Segoe UI Semibold", 20, FontStyle::Bold);
            lblTitle->ForeColor = burgundy;
            lblTitle->Location = Point(30, 28);
            lblTitle->AutoSize = true;

            lblSubtitle = gcnew Label();
            lblSubtitle->Text = "Our mission, values, and the team behind the platform.";
            lblSubtitle->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblSubtitle->ForeColor = textGray;
            lblSubtitle->Location = Point(32, 62);
            lblSubtitle->AutoSize = true;

            this->Controls->Add(lblTitle);
            this->Controls->Add(lblSubtitle);
        }

        // ── MISSION CARD ──────────────────────────
        void InitializeMissionCard() {
            pnlMission = gcnew RoundedPanel();
            pnlMission->BackColor = cardBg;
            pnlMission->BorderColor = Color::FromArgb(230, 210, 210);
            pnlMission->CornerRadius = 18;
            pnlMission->Location = Point(28, 100);
            pnlMission->Size = Drawing::Size(1140, 230);

            // Left: text
            lblMissionHeading = gcnew Label();
            lblMissionHeading->Text = "Our Mission";
            lblMissionHeading->Font = gcnew Drawing::Font("Segoe UI Semibold", 12, FontStyle::Bold);
            lblMissionHeading->ForeColor = burgundy;
            lblMissionHeading->Location = Point(28, 38);
            lblMissionHeading->AutoSize = true;

            lblMissionText = gcnew Label();
            lblMissionText->Text =
                "LifeBlood MS exists to eliminate the gap between those who give and those who need.\n"
                "Every year, thousands of lives are lost to blood shortages that better coordination\n"
                "could prevent. We built this platform to give hospitals, donors, and administrators\n"
                "a single, reliable source of truth for life-saving blood inventory.";
            lblMissionText->Font = gcnew Drawing::Font("Segoe UI", 10);
            lblMissionText->ForeColor = textDark;
            lblMissionText->Location = Point(28, 72);
            lblMissionText->Size = Drawing::Size(620, 130);

            // SQL-fed stat badge
            lblDonorStat = gcnew Label();
            lblDonorStat->Text = "Total Donors: Loading...";
            lblDonorStat->Font = gcnew Drawing::Font("Segoe UI Semibold", 10, FontStyle::Bold);
            lblDonorStat->ForeColor = burgundy;
            lblDonorStat->Location = Point(28, 175);
            lblDonorStat->AutoSize = true;

            // Right: burgundy banner panel (painted via event)
            pnlBanner = gcnew Panel();
            pnlBanner->Location = Point(710, 12);
            pnlBanner->Size = Drawing::Size(415, 205);
            pnlBanner->Paint += gcnew PaintEventHandler(this, &AboutForm::PaintBanner);

            pnlMission->Controls->Add(lblMissionHeading);
            pnlMission->Controls->Add(lblMissionText);
            pnlMission->Controls->Add(lblDonorStat);
            pnlMission->Controls->Add(pnlBanner);
            this->Controls->Add(pnlMission);
        }

        // ── BANNER PAINT (gradient + heart icon) ──
        void PaintBanner(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;

            Panel^ p = safe_cast<Panel^>(sender);
            Drawing::Rectangle r(0, 0, p->Width, p->Height);

            // Rounded clip
            GraphicsPath^ clip = GetRoundedRect(r, 14);
            g->SetClip(clip);

            // Gradient: dark → lighter burgundy
            LinearGradientBrush^ grad = gcnew LinearGradientBrush(
                r,
                Color::FromArgb(100, 0, 20),
                Color::FromArgb(180, 30, 60),
                LinearGradientMode::Horizontal);
            g->FillPath(grad, clip);

            // Draw a heart outline (Bezier approximation)
            DrawHeartIcon(g, p->Width / 2 - 38, p->Height / 2 - 42, 80);
        }

        void DrawHeartIcon(Graphics^ g, int x, int y, int size) {
            Pen^ pen = gcnew Pen(Color::FromArgb(200, 255, 255, 255), 3.5f);
            pen->LineJoin = LineJoin::Round;

            // Two bezier halves of a heart
            array<PointF>^ left = {
                PointF(x + size / 2.0f, y + size * 0.35f),
                PointF(x + size * 0.2f, y),
                PointF(x - size * 0.1f, y + size * 0.55f),
                PointF(x + size / 2.0f, y + size)
            };
            array<PointF>^ right = {
                PointF(x + size / 2.0f, y + size * 0.35f),
                PointF(x + size * 0.8f, y),
                PointF(x + size * 1.1f, y + size * 0.55f),
                PointF(x + size / 2.0f, y + size)
            };
            g->DrawBeziers(pen, left);
            g->DrawBeziers(pen, right);
        }

        // ── VALUE CARDS (Precision, Compassion, Speed)
        void InitializeValueCards() {
            pnlPrecision = MakeValueCard(Char::ConvertFromUtf32(0x2299) + "  Precision",
                "Real-time inventory, automated expiry\nalerts, and audit-grade activity logs.",
                28, 352);

            pnlCompassion = MakeValueCard(Char::ConvertFromUtf32(0x2661) + "  Compassion",
                "We put donors and patients first --\nevery feature is designed with empathy.",
                392, 352);

            pnlSpeed = MakeValueCard(Char::ConvertFromUtf32(0x26A1) + "  Speed",
                "Urgent requests are prioritized with\ninstant notifications to matching donors.",
                756, 352);

            this->Controls->Add(pnlPrecision);
            this->Controls->Add(pnlCompassion);
            this->Controls->Add(pnlSpeed);
        }

        RoundedPanel^ MakeValueCard(String^ title, String^ body, int x, int y) {
            RoundedPanel^ card = gcnew RoundedPanel();
            card->BackColor = cardBg;
            card->BorderColor = Color::FromArgb(220, 200, 200);
            card->CornerRadius = 16;
            card->Location = Point(x, y);
            card->Size = Drawing::Size(352, 170);

            // Icon bubble
            Panel^ iconBubble = gcnew Panel();
            iconBubble->BackColor = Color::FromArgb(255, 240, 240);
            iconBubble->Size = Drawing::Size(44, 44);
            iconBubble->Location = Point(20, 20);
            iconBubble->Paint += gcnew PaintEventHandler(
                this, &AboutForm::PaintIconBubble);

            Label^ lblIcon = gcnew Label();
            lblIcon->Text = title->Substring(0, 2);  // Grab emoji
            lblIcon->Font = gcnew Drawing::Font("Segoe UI Emoji", 14);
            lblIcon->ForeColor = burgundy;
            lblIcon->TextAlign = ContentAlignment::MiddleCenter;
            lblIcon->Dock = DockStyle::Fill;
            iconBubble->Controls->Add(lblIcon);

            Label^ lblTitle = gcnew Label();
            lblTitle->Text = title->Substring(2)->Trim();
            lblTitle->Font = gcnew Drawing::Font("Segoe UI Semibold", 11, FontStyle::Bold);
            lblTitle->ForeColor = burgundy;
            lblTitle->Location = Point(18, 76);
            lblTitle->AutoSize = true;

            Label^ lblBody = gcnew Label();
            lblBody->Text = body;
            lblBody->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            lblBody->ForeColor = textGray;
            lblBody->Location = Point(18, 108);
            lblBody->Size = Drawing::Size(318, 50);

            card->Controls->Add(iconBubble);
            card->Controls->Add(lblTitle);
            card->Controls->Add(lblBody);
            return card;
        }

        void PaintIconBubble(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Panel^ p = safe_cast<Panel^>(sender);
            GraphicsPath^ path = GetRoundedRect(
                Drawing::Rectangle(0, 0, p->Width, p->Height), 12);
            g->FillPath(gcnew SolidBrush(Color::FromArgb(255, 235, 235)), path);
            p->Region = gcnew System::Drawing::Region(path);
        }

        // ── TEAM SECTION ──────────────────────────
        void InitializeTeamSection() {
            pnlTeam = gcnew RoundedPanel();
            pnlTeam->BackColor = cardBg;
            pnlTeam->BorderColor = Color::FromArgb(220, 200, 200);
            pnlTeam->CornerRadius = 18;
            pnlTeam->Location = Point(28, 545);
            pnlTeam->Size = Drawing::Size(1140, 185);

            Label^ lblTeamHead = gcnew Label();
            lblTeamHead->Text = "Our Team";
            lblTeamHead->Font = gcnew Drawing::Font("Segoe UI Semibold", 12, FontStyle::Bold);
            lblTeamHead->ForeColor = burgundy;
            lblTeamHead->Location = Point(28, 18);
            lblTeamHead->AutoSize = true;
            pnlTeam->Controls->Add(lblTeamHead);

            // Team member data
            array<String^>^ names = { "Talha Pasha", "Maryam Shafiq",   "Nabeera Saqib", "Adnan Ali" };
            array<String^>^ roles = { "Team Leader",  "Developer","Developer",      "Developer" };
            array<String^>^ initials = { "T", "M", "N", "A" };

            int startX = 28;
            for (int i = 0; i < 4; i++) {
                Panel^ memberCard = MakeTeamCard(
                    names[i], roles[i], initials[i], startX, 50);
                pnlTeam->Controls->Add(memberCard);
                startX += 268;
            }

            this->Controls->Add(pnlTeam);
        }

        Panel^ MakeTeamCard(String^ name, String^ role, String^ initial, int x, int y) {
            Panel^ card = gcnew Panel();
            card->BackColor = Color::FromArgb(252, 248, 248);
            card->Size = Drawing::Size(248, 115);
            card->Location = Point(x, y);
            card->Paint += gcnew PaintEventHandler(this, &AboutForm::PaintTeamCard);

            // Avatar circle - REMOVED LAMBDA
            Panel^ avatar = gcnew Panel();
            avatar->Size = Drawing::Size(52, 52);
            avatar->Location = Point(14, 14);
            avatar->BackColor = Color::FromArgb(252, 248, 248);
            avatar->Tag = initial; // Secretly store the letter here!
            avatar->Paint += gcnew PaintEventHandler(this, &AboutForm::PaintTeamAvatar);
            card->Controls->Add(avatar);

            Label^ lblName = gcnew Label();
            lblName->Text = name;
            lblName->Font = gcnew Drawing::Font("Segoe UI Semibold", 10.5f, FontStyle::Bold);
            lblName->ForeColor = textDark;
            lblName->Location = Point(74, 20);
            lblName->AutoSize = true;
            card->Controls->Add(lblName);

            Label^ lblRole = gcnew Label();
            lblRole->Text = role;
            lblRole->Font = gcnew Drawing::Font("Segoe UI", 9.5f);
            lblRole->ForeColor = (role == "Team Leader") ? burgundy : textGray;
            lblRole->Location = Point(74, 44);
            lblRole->AutoSize = true;
            card->Controls->Add(lblRole);

            return card;
        }

        void PaintTeamAvatar(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Panel^ av = safe_cast<Panel^>(sender);

            // Retrieve the initial letter we stored in the Tag property
            String^ initial = safe_cast<String^>(av->Tag);

            // Circle fill
            SolidBrush^ brush = gcnew SolidBrush(burgundy);
            g->FillEllipse(brush, 0, 0, av->Width - 2, av->Height - 2);
            delete brush;

            // Initial letter
            StringFormat^ sf = gcnew StringFormat();
            sf->Alignment = StringAlignment::Center;
            sf->LineAlignment = StringAlignment::Center;

            Drawing::Font^ font = gcnew Drawing::Font("Segoe UI Semibold", 18, FontStyle::Bold);

            // Note: Explicitly casted to float to fix the "possible loss of data" warnings
            Drawing::RectangleF rect = Drawing::RectangleF(0.0f, 0.0f, (float)(av->Width - 2), (float)(av->Height - 2));

            g->DrawString(initial, font, Brushes::White, rect, sf);

            delete sf;
            delete font;
        }

        void PaintTeamCard(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = SmoothingMode::AntiAlias;
            Panel^ p = safe_cast<Panel^>(sender);
            GraphicsPath^ path = GetRoundedRect(
                Drawing::Rectangle(0, 0, p->Width - 1, p->Height - 1), 12);
            g->FillPath(gcnew SolidBrush(Color::FromArgb(252, 248, 248)), path);
            g->DrawPath(gcnew Pen(Color::FromArgb(220, 200, 200), 1.0f), path);
            p->Region = gcnew System::Drawing::Region(path);
        }

        // ── UTILITY: Rounded Rectangle Path ───────
        GraphicsPath^ GetRoundedRect(Drawing::Rectangle r, int radius) {
            int d = radius * 2;
            GraphicsPath^ path = gcnew GraphicsPath();
            path->AddArc(r.X, r.Y, d, d, 180, 90);
            path->AddArc(r.Right - d, r.Y, d, d, 270, 90);
            path->AddArc(r.Right - d, r.Bottom - d, d, d, 0, 90);
            path->AddArc(r.X, r.Bottom - d, d, d, 90, 90);
            path->CloseFigure();
            return path;
        }

        // ── SQL: Load Total Donor Count using your Database.h ────────────
        void LoadDonorCount() {
            try {
                // Corrected to match your actual schema:
                SqlCommand^ cmd = gcnew SqlCommand("SELECT COUNT(*) FROM Users WHERE Role = 'Donor'");

                // Using your existing Singleton instance
                Object^ result = Database::GetInstance()->ExecuteScalar(cmd);

                if (result != nullptr && result != DBNull::Value) {
                    int count = Convert::ToInt32(result);
                    lblDonorStat->Text = String::Format(Char::ConvertFromUtf32(0x2764) + "  Total Registered Donors: {0}", count);
                    lblDonorStat->ForeColor = burgundy;
                }
                else {
                    lblDonorStat->Text = "Total Donors: 0";
                }
            }
            catch (Exception^) {
                lblDonorStat->Text = "Stats currently unavailable.";
            }
        }
    };
}