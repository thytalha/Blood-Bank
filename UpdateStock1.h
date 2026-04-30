#pragma once

namespace BloodBankSystem {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    // ============================================================
    //  BloodStock CLASS  –  OOP Data Model
    // ============================================================
    public ref class BloodStock {
    public:
        property String^ BloodType;
        property String^ RhFactor;
        property int     UnitsAvailable;
        property int     UnitsAdded;
        property int     UnitsRemoved;
        property DateTime ExpiryDate;
        property String^ DonorID;
        property String^ StorageLocation;
        property String^ Remarks;

        BloodStock() {
            BloodType = "";
            RhFactor = "+";
            UnitsAvailable = 0;
            UnitsAdded = 0;
            UnitsRemoved = 0;
            ExpiryDate = DateTime::Today.AddDays(42);
            DonorID = "";
            StorageLocation = "";
            Remarks = "";
        }

        // Calculate new total
        int GetUpdatedTotal() {
            return UnitsAvailable + UnitsAdded - UnitsRemoved;
        }

        // Validation
        bool IsValid([System::Runtime::InteropServices::Out] String^% error) {
            if (BloodType == "" || BloodType == nullptr) {
                error = "Please select a Blood Type.";
                return false;
            }
            if (UnitsAdded < 0 || UnitsRemoved < 0) {
                error = "Units cannot be negative.";
                return false;
            }
            if (GetUpdatedTotal() < 0) {
                error = "Stock cannot go below zero. Check Units Removed.";
                return false;
            }
            if (ExpiryDate <= DateTime::Today) {
                error = "Expiry date must be in the future.";
                return false;
            }
            error = "";
            return true;
        }
    };

    // ============================================================
    //  UpdateStockForm CLASS
    // ============================================================
    public ref class UpdateStockForm : public System::Windows::Forms::Form {

        // ── Private Members ─────────────────────────────────────────
    private:
        BloodStock^ _stock;

        // Layout
        Panel^ pnlHeader;
        Panel^ pnlBody;        // holds left + right side by side
        Panel^ pnlLeft;        // button sidebar
        Panel^ pnlRight;       // scrollable form area
        Panel^ pnlFooter;
        Panel^ pnlStatus;

        // Header controls
        PictureBox^ picLogo;
        Label^ lblSystemTitle;
        Label^ lblSystemSub;
        Label^ lblDateTime;
        System::Windows::Forms::Timer^ tmrClock;

        // Sidebar buttons
        Button^ btnNew;
        Button^ btnSave;
        Button^ btnDelete;
        Button^ btnPrint;
        Button^ btnRefresh;
        Button^ btnClose;

        // Section: Blood Info
        GroupBox^ grpBloodInfo;
        Label^ lblBloodType;
        ComboBox^ cboBloodType;
        Label^ lblRhFactor;
        ComboBox^ cboRhFactor;
        Label^ lblBloodGroup;
        Label^ lblBloodGroupVal;

        // Section: Stock Details
        GroupBox^ grpStockDetails;
        Label^ lblCurrentStock;
        NumericUpDown^ nudCurrentStock;
        Label^ lblUnitsAdded;
        NumericUpDown^ nudUnitsAdded;
        Label^ lblUnitsRemoved;
        NumericUpDown^ nudUnitsRemoved;
        Label^ lblNewTotal;
        Label^ lblNewTotalVal;

        // Section: Batch Info
        GroupBox^ grpBatchInfo;
        Label^ lblDonorID;
        TextBox^ txtDonorID;
        Label^ lblExpiryDate;
        DateTimePicker^ dtpExpiryDate;
        Label^ lblStorageLoc;
        ComboBox^ cboStorageLoc;
        Label^ lblRemarks;
        TextBox^ txtRemarks;

        // Section: Stock Level Indicator
        GroupBox^ grpIndicator;
        ProgressBar^ pbStockLevel;
        Label^ lblStockStatus;
        Label^ lblCriticalNote;

        // Footer
        Label^ lblFooter;
        Label^ lblVersion;
        Label^ lblStatusBar;

        // Colors (unchanged)
        Color colorCrimson = Color::FromArgb(183, 28, 28);
        Color colorCrimsonDark = Color::FromArgb(127, 0, 0);
        Color colorCrimsonMid = Color::FromArgb(198, 40, 40);
        Color colorCrimsonLt = Color::FromArgb(229, 57, 53);
        Color colorWhite = Color::White;
        Color colorOffWhite = Color::FromArgb(255, 248, 248);
        Color colorLightGray = Color::FromArgb(245, 245, 245);
        Color colorBorder = Color::FromArgb(220, 200, 200);
        Color colorTextDark = Color::FromArgb(30, 0, 0);
        Color colorGold = Color::FromArgb(255, 214, 0);

        // ── Constructor ─────────────────────────────────────────────
    public:
        UpdateStockForm(void) {
            _stock = gcnew BloodStock();
            InitializeComponent();
            StartClock();
            RefreshCalculations();
        }

        // ── Destructor ──────────────────────────────────────────────
    protected:
        ~UpdateStockForm() {
            if (components) delete components;
        }

    private:
        System::ComponentModel::Container^ components;

        // ════════════════════════════════════════════════════════
        //  InitializeComponent
        // ════════════════════════════════════════════════════════
        void InitializeComponent(void) {
            this->components = gcnew System::ComponentModel::Container();
            this->SuspendLayout();

            // ── Form Properties ──────────────────────────────────
            this->Text = L"LifeBlood  |  Update Blood Stock Registry";
            this->Size = System::Drawing::Size(1050, 760);
            this->MinimumSize = System::Drawing::Size(960, 700);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = colorOffWhite;
            this->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.5f);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Sizable;

            // Build in order: header top, footer bottom, body fills middle
            BuildHeader();
            BuildFooter();
            BuildBody();   // body contains left sidebar + right form panel

            this->ResumeLayout(false);
        }

        // ════════════════════════════════════════════════════════
        //  HEADER PANEL  (full width, docked top — UNCHANGED)
        // ════════════════════════════════════════════════════════
        void BuildHeader() {
            pnlHeader = gcnew Panel();
            pnlHeader->Dock = DockStyle::Top;
            pnlHeader->Height = 90;
            pnlHeader->BackColor = colorCrimsonDark;
            pnlHeader->Paint += gcnew PaintEventHandler(this, &UpdateStockForm::pnlHeader_Paint);

            picLogo = gcnew PictureBox();
            picLogo->Size = System::Drawing::Size(60, 60);
            picLogo->Location = System::Drawing::Point(20, 15);
            picLogo->BackColor = Color::Transparent;
            picLogo->Paint += gcnew PaintEventHandler(this, &UpdateStockForm::picLogo_Paint);

            lblSystemTitle = gcnew Label();
            lblSystemTitle->Text = L"LifeBlood";
            lblSystemTitle->Font = gcnew System::Drawing::Font(L"Georgia", 26, FontStyle::Bold);
            lblSystemTitle->ForeColor = colorWhite;
            lblSystemTitle->BackColor = Color::Transparent;
            lblSystemTitle->Location = System::Drawing::Point(90, 10);
            lblSystemTitle->AutoSize = true;

            lblSystemSub = gcnew Label();
            lblSystemSub->Text = L"Blood Bank Management System  •  Stock Registry";
            lblSystemSub->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.5f, FontStyle::Regular);
            lblSystemSub->ForeColor = Color::FromArgb(255, 200, 200);
            lblSystemSub->BackColor = Color::Transparent;
            lblSystemSub->Location = System::Drawing::Point(92, 52);
            lblSystemSub->AutoSize = true;

            lblDateTime = gcnew Label();
            lblDateTime->Text = DateTime::Now.ToString(L"dddd, dd MMM yyyy   HH:mm:ss");
            lblDateTime->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.0f);
            lblDateTime->ForeColor = Color::FromArgb(255, 210, 210);
            lblDateTime->BackColor = Color::Transparent;
            lblDateTime->AutoSize = true;
            lblDateTime->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            lblDateTime->Location = System::Drawing::Point(750, 35);

            pnlHeader->Controls->Add(picLogo);
            pnlHeader->Controls->Add(lblSystemTitle);
            pnlHeader->Controls->Add(lblSystemSub);
            pnlHeader->Controls->Add(lblDateTime);

            this->Controls->Add(pnlHeader);
        }

        // Header gradient paint (UNCHANGED)
        void pnlHeader_Paint(Object^ sender, PaintEventArgs^ e) {
            Panel^ p = safe_cast<Panel^>(sender);
            Drawing2D::LinearGradientBrush^ brush =
                gcnew Drawing2D::LinearGradientBrush(
                    p->ClientRectangle,
                    Color::FromArgb(127, 0, 0),
                    Color::FromArgb(183, 28, 28),
                    Drawing2D::LinearGradientMode::Horizontal);
            e->Graphics->FillRectangle(brush, p->ClientRectangle);

            Pen^ goldPen = gcnew Pen(colorGold, 3);
            e->Graphics->DrawLine(goldPen, 0, p->Height - 3, p->Width, p->Height - 3);
            delete goldPen;
            delete brush;
        }

        // Blood-drop icon paint (UNCHANGED)
        void picLogo_Paint(Object^ sender, PaintEventArgs^ e) {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = Drawing2D::SmoothingMode::AntiAlias;

            array<PointF>^ pts = {
                PointF(30, 5), PointF(50, 30), PointF(50, 42),
                PointF(30, 56), PointF(10, 42), PointF(10, 30)
            };
            Drawing2D::GraphicsPath^ path = gcnew Drawing2D::GraphicsPath();
            path->AddArc(10, 30, 40, 26, 0, 180);
            path->AddLine(10, 43, 30, 5);
            path->AddLine(30, 5, 50, 43);

            SolidBrush^ rb = gcnew SolidBrush(Color::FromArgb(220, 255, 255, 255));
            g->FillPath(rb, path);
            Pen^ wp = gcnew Pen(Color::FromArgb(180, 255, 255, 255), 2);
            g->DrawPath(wp, path);

            Pen^ cp = gcnew Pen(colorCrimsonDark, 3);
            g->DrawLine(cp, 30, 32, 30, 48);
            g->DrawLine(cp, 22, 40, 38, 40);

            delete rb; delete wp; delete cp; delete path;
        }

        // ════════════════════════════════════════════════════════
        //  FOOTER (UNCHANGED — built before body so DockStyle works)
        // ════════════════════════════════════════════════════════
        void BuildFooter() {
            pnlStatus = gcnew Panel();
            pnlStatus->Dock = DockStyle::Bottom;
            pnlStatus->Height = 18;
            pnlStatus->BackColor = Color::FromArgb(230, 215, 215);

            lblStatusBar = gcnew Label();
            lblStatusBar->Text = L"Ready";
            lblStatusBar->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.0f);
            lblStatusBar->ForeColor = Color::FromArgb(80, 0, 0);
            lblStatusBar->BackColor = Color::Transparent;
            lblStatusBar->Location = System::Drawing::Point(10, 5);
            lblStatusBar->AutoSize = true;

            pnlStatus->Controls->Add(lblStatusBar);

            pnlFooter = gcnew Panel();
            pnlFooter->Dock = DockStyle::Bottom;
            pnlFooter->Height = 26;
            pnlFooter->BackColor = colorCrimsonDark;

            lblFooter = gcnew Label();
            lblFooter->Text = L"LifeBlood Blood Bank Management System  •  Stock Registry Module";
            lblFooter->ForeColor = Color::FromArgb(255, 200, 200);
            lblFooter->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.5f);
            lblFooter->BackColor = Color::Transparent;
            lblFooter->Location = System::Drawing::Point(10, 10);
            lblFooter->AutoSize = true;

            lblVersion = gcnew Label();
            lblVersion->Text = L"v2.0.1";
            lblVersion->ForeColor = Color::FromArgb(200, 200, 200);
            lblVersion->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.0f);
            lblVersion->BackColor = Color::Transparent;
            lblVersion->Anchor = AnchorStyles::Top | AnchorStyles::Right;
            lblVersion->Location = System::Drawing::Point(970, 10);
            lblVersion->AutoSize = true;

            pnlFooter->Controls->Add(lblFooter);
            pnlFooter->Controls->Add(lblVersion);

            // Add footer controls (bottom-up order matters for DockStyle)
            this->Controls->Add(pnlStatus);
            this->Controls->Add(pnlFooter);
        }

        // ════════════════════════════════════════════════════════
        //  BODY  — Left Sidebar + Right Form Panel
        // ════════════════════════════════════════════════════════
        void BuildBody() {
            // Outer body panel fills remaining space between header and footer
            pnlBody = gcnew Panel();
            pnlBody->Dock = DockStyle::Fill;
            pnlBody->BackColor = colorOffWhite;

            BuildLeftSidebar();   // must be added first with DockStyle::Left
            BuildRightPanel();    // fills remaining space

            this->Controls->Add(pnlBody);
        }

        // ════════════════════════════════════════════════════════
        //  LEFT SIDEBAR  — vertical button strip
        // ════════════════════════════════════════════════════════
        void BuildLeftSidebar() {
            pnlLeft = gcnew Panel();
            pnlLeft->Dock = DockStyle::Left;
            pnlLeft->Width = 140;
            pnlLeft->BackColor = Color::FromArgb(240, 230, 230);
            pnlLeft->Paint += gcnew PaintEventHandler(this, &UpdateStockForm::pnlLeft_Paint);

            // Place buttons vertically, same style as original toolbar buttons
            int y = 200;
            int btnH = 40;
            int gap = 15;

            btnNew = MakeSidebarButton(L"⊕  New", 20, y, colorCrimsonMid, colorWhite); y += btnH + gap;
            btnSave = MakeSidebarButton(L"💾  Save", 20, y, colorCrimson, colorWhite); y += btnH + gap;
            btnDelete = MakeSidebarButton(L"🗑  Delete", 20, y, Color::FromArgb(150, 20, 20), colorWhite); y += btnH + gap;
            btnPrint = MakeSidebarButton(L"🖨  Print", 20, y, Color::FromArgb(80, 80, 80), colorWhite); y += btnH + gap;
            btnRefresh = MakeSidebarButton(L"↺  Refresh", 20, y, Color::FromArgb(60, 100, 60), colorWhite); y += btnH + gap;
            btnClose = MakeSidebarButton(L"✕  Close", 20, y, Color::FromArgb(60, 60, 60), colorWhite);

            // Wire events (UNCHANGED)
            btnNew->Click += gcnew EventHandler(this, &UpdateStockForm::btnNew_Click);
            btnSave->Click += gcnew EventHandler(this, &UpdateStockForm::btnSave_Click);
            btnDelete->Click += gcnew EventHandler(this, &UpdateStockForm::btnDelete_Click);
            btnPrint->Click += gcnew EventHandler(this, &UpdateStockForm::btnPrint_Click);
            btnRefresh->Click += gcnew EventHandler(this, &UpdateStockForm::btnRefresh_Click);
            btnClose->Click += gcnew EventHandler(this, &UpdateStockForm::btnClose_Click);

            pnlLeft->Controls->Add(btnNew);
            pnlLeft->Controls->Add(btnSave);
            pnlLeft->Controls->Add(btnDelete);
            pnlLeft->Controls->Add(btnPrint);
            pnlLeft->Controls->Add(btnRefresh);
            pnlLeft->Controls->Add(btnClose);

            pnlBody->Controls->Add(pnlLeft);
        }

        // Sidebar button — same visual style as original toolbar buttons, width fits panel
        Button^ MakeSidebarButton(String^ text, int x, int y, Color bg, Color fg) {
            Button^ btn = gcnew Button();
            btn->Text = text;
            btn->Size = System::Drawing::Size(100, 36);
            btn->Location = System::Drawing::Point(x, y);
            btn->BackColor = bg;
            btn->ForeColor = fg;
            btn->FlatStyle = FlatStyle::Flat;
            btn->FlatAppearance->BorderColor = Color::FromArgb(80, 0, 0);
            btn->FlatAppearance->BorderSize = 1;
            btn->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.5f, FontStyle::Bold);
            btn->Cursor = Cursors::Hand;
            return btn;
        }

        // Left sidebar right-border paint
        void pnlLeft_Paint(Object^ sender, PaintEventArgs^ e) {
            Panel^ p = safe_cast<Panel^>(sender);
            Pen^ borderPen = gcnew Pen(colorBorder, 1);
            e->Graphics->DrawLine(borderPen, p->Width - 1, 0, p->Width - 1, p->Height);
            delete borderPen;
        }

        // ════════════════════════════════════════════════════════
        //  RIGHT PANEL  — scrollable form content (UNCHANGED groups)
        // ════════════════════════════════════════════════════════
        void BuildRightPanel() {
            pnlRight = gcnew Panel();
            pnlRight->Dock = DockStyle::Fill;
            pnlRight->BackColor = colorOffWhite;
            pnlRight->AutoScroll = true;
            pnlRight->Padding = System::Windows::Forms::Padding(10, 12, 10, 12);

            BuildBloodInfoGroup();
            BuildStockDetailsGroup();
            BuildBatchInfoGroup();
            BuildStockIndicatorGroup();

            pnlBody->Controls->Add(pnlRight);
        }

        // ── Group: Blood Information (UNCHANGED internals) ────────
        void BuildBloodInfoGroup() {
            grpBloodInfo = MakeGroupBox(L"  🩸  Blood Type Information", 200, 100, 900, 110);

            lblBloodType = MakeLabel(L"Blood Type:", 120, 32);
            cboBloodType = gcnew ComboBox();
            cboBloodType->Items->AddRange(gcnew array<Object^>{L"A", L"B", L"AB", L"O"});
            cboBloodType->DropDownStyle = ComboBoxStyle::DropDownList;
            cboBloodType->Size = System::Drawing::Size(100, 28);
            cboBloodType->Location = System::Drawing::Point(110, 30);
            cboBloodType->Font = gcnew System::Drawing::Font(L"Segoe UI", 12, FontStyle::Bold);
            cboBloodType->ForeColor = colorCrimson;
            cboBloodType->SelectedIndex = -1;
            cboBloodType->SelectedIndexChanged += gcnew EventHandler(this, &UpdateStockForm::OnBloodTypeChanged);

            lblRhFactor = MakeLabel(L"Rh Factor:", 230, 32);
            cboRhFactor = gcnew ComboBox();
            cboRhFactor->Items->AddRange(gcnew array<Object^>{L"+  (Positive)", L"−  (Negative)"});
            cboRhFactor->DropDownStyle = ComboBoxStyle::DropDownList;
            cboRhFactor->Size = System::Drawing::Size(160, 28);
            cboRhFactor->Location = System::Drawing::Point(320, 30);
            cboRhFactor->SelectedIndex = 0;
            cboRhFactor->SelectedIndexChanged += gcnew EventHandler(this, &UpdateStockForm::OnBloodTypeChanged);

            lblBloodGroup = MakeLabel(L"Blood Group:", 710, 32);
            lblBloodGroupVal = gcnew Label();
            lblBloodGroupVal->Text = L"—";
            lblBloodGroupVal->Font = gcnew System::Drawing::Font(L"Georgia", 22, FontStyle::Bold);
            lblBloodGroupVal->ForeColor = colorCrimson;
            lblBloodGroupVal->BackColor = Color::Transparent;
            lblBloodGroupVal->Location = System::Drawing::Point(620, 20);
            lblBloodGroupVal->AutoSize = true;

            Label^ lblNote = MakeLabel(L"ℹ  Universal donor: O−   |   Universal recipient: AB+", 20, 72);
            lblNote->ForeColor = Color::FromArgb(150, 50, 50);
            lblNote->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.5f, FontStyle::Italic);

            grpBloodInfo->Controls->Add(lblBloodType);
            grpBloodInfo->Controls->Add(cboBloodType);
            grpBloodInfo->Controls->Add(lblRhFactor);
            grpBloodInfo->Controls->Add(cboRhFactor);
            grpBloodInfo->Controls->Add(lblBloodGroup);
            grpBloodInfo->Controls->Add(lblBloodGroupVal);
            grpBloodInfo->Controls->Add(lblNote);
            pnlRight->Controls->Add(grpBloodInfo);
        }

        // ── Group: Stock Details (UNCHANGED internals) ────────────
        void BuildStockDetailsGroup() {
            grpStockDetails = MakeGroupBox(L"  📦  Stock Update Details", 200, 220, 900, 130);

            lblCurrentStock = MakeLabel(L"Current Units in Stock:", 20, 35);
            nudCurrentStock = MakeNUD(180, 30, 0, 9999, 0);
            nudCurrentStock->ValueChanged += gcnew EventHandler(this, &UpdateStockForm::OnStockValueChanged);

            Label^ lblCurrentUnit = MakeLabel(L"units", 290, 35);
            lblCurrentUnit->ForeColor = Color::Gray;

            lblUnitsAdded = MakeLabel(L"Units to Add  (+):", 340, 35);
            nudUnitsAdded = MakeNUD(490, 30, 0, 9999, 0);
            nudUnitsAdded->BackColor = Color::FromArgb(232, 255, 232);
            nudUnitsAdded->ForeColor = Color::FromArgb(0, 100, 0);
            nudUnitsAdded->ValueChanged += gcnew EventHandler(this, &UpdateStockForm::OnStockValueChanged);

            Label^ lblAddUnit = MakeLabel(L"units", 598, 35);
            lblAddUnit->ForeColor = Color::FromArgb(0, 100, 0);

            lblUnitsRemoved = MakeLabel(L"Units to Remove  (−):", 20, 80);
            nudUnitsRemoved = MakeNUD(180, 75, 0, 9999, 0);
            nudUnitsRemoved->BackColor = Color::FromArgb(255, 235, 235);
            nudUnitsRemoved->ForeColor = colorCrimson;
            nudUnitsRemoved->ValueChanged += gcnew EventHandler(this, &UpdateStockForm::OnStockValueChanged);

            Label^ lblRemUnit = MakeLabel(L"units", 290, 80);
            lblRemUnit->ForeColor = colorCrimson;

            Label^ lblDivider = gcnew Label();
            lblDivider->BorderStyle = BorderStyle::Fixed3D;
            lblDivider->Size = System::Drawing::Size(1, 90);
            lblDivider->Location = System::Drawing::Point(680, 20);
            lblDivider->BackColor = colorBorder;

            lblNewTotal = MakeLabel(L"Updated Total:", 700, 35);
            lblNewTotal->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.5f, FontStyle::Bold);

            lblNewTotalVal = gcnew Label();
            lblNewTotalVal->Text = L"0";
            lblNewTotalVal->Font = gcnew System::Drawing::Font(L"Georgia", 32, FontStyle::Bold);
            lblNewTotalVal->ForeColor = colorCrimson;
            lblNewTotalVal->BackColor = Color::Transparent;
            lblNewTotalVal->Location = System::Drawing::Point(700, 55);
            lblNewTotalVal->AutoSize = true;

            Label^ lblTotalUnit = MakeLabel(L"units", 780, 100);
            lblTotalUnit->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.5f);
            lblTotalUnit->ForeColor = Color::Gray;

            grpStockDetails->Controls->Add(lblCurrentStock);
            grpStockDetails->Controls->Add(nudCurrentStock);
            grpStockDetails->Controls->Add(lblCurrentUnit);
            grpStockDetails->Controls->Add(lblUnitsAdded);
            grpStockDetails->Controls->Add(nudUnitsAdded);
            grpStockDetails->Controls->Add(lblAddUnit);
            grpStockDetails->Controls->Add(lblUnitsRemoved);
            grpStockDetails->Controls->Add(nudUnitsRemoved);
            grpStockDetails->Controls->Add(lblRemUnit);
            grpStockDetails->Controls->Add(lblDivider);
            grpStockDetails->Controls->Add(lblNewTotal);
            grpStockDetails->Controls->Add(lblNewTotalVal);
            grpStockDetails->Controls->Add(lblTotalUnit);
            pnlRight->Controls->Add(grpStockDetails);
        }

        // ── Group: Batch / Donor Info (UNCHANGED internals) ───────
        void BuildBatchInfoGroup() {
            grpBatchInfo = MakeGroupBox(L"  🏥  Batch & Donor Information", 200, 360, 900, 130);

            lblDonorID = MakeLabel(L"Donor / Batch ID:", 20, 35);
            txtDonorID = MakeTextBox(180, 30, 200);


            lblExpiryDate = MakeLabel(L"Expiry Date:", 410, 35);
            dtpExpiryDate = gcnew DateTimePicker();
            dtpExpiryDate->Format = DateTimePickerFormat::Short;
            dtpExpiryDate->Size = System::Drawing::Size(160, 26);
            dtpExpiryDate->Location = System::Drawing::Point(510, 30);
            dtpExpiryDate->MinDate = DateTime::Today.AddDays(1);
            dtpExpiryDate->Value = DateTime::Today.AddDays(42);

            lblStorageLoc = MakeLabel(L"Storage Location:", 20, 80);
            cboStorageLoc = gcnew ComboBox();
            cboStorageLoc->Items->AddRange(gcnew array<Object^>{
                L"Refrigerator A – Unit 1",
                    L"Refrigerator A – Unit 2",
                    L"Refrigerator B – Unit 1",
                    L"Refrigerator B – Unit 2",
                    L"Freezer Bank – Section 1",
                    L"Freezer Bank – Section 2",
                    L"Mobile Unit Storage",
                    L"Emergency Reserve Bay"
            });
            cboStorageLoc->DropDownStyle = ComboBoxStyle::DropDownList;
            cboStorageLoc->Size = System::Drawing::Size(300, 28);
            cboStorageLoc->Location = System::Drawing::Point(180, 75);

            lblRemarks = MakeLabel(L"Remarks / Notes:", 500, 80);
            txtRemarks = gcnew TextBox();
            txtRemarks->Multiline = true;
            txtRemarks->Size = System::Drawing::Size(330, 55);
            txtRemarks->Location = System::Drawing::Point(500, 75);

            txtRemarks->BorderStyle = BorderStyle::FixedSingle;
            txtRemarks->ScrollBars = ScrollBars::Vertical;

            grpBatchInfo->Controls->Add(lblDonorID);
            grpBatchInfo->Controls->Add(txtDonorID);
            grpBatchInfo->Controls->Add(lblExpiryDate);
            grpBatchInfo->Controls->Add(dtpExpiryDate);
            grpBatchInfo->Controls->Add(lblStorageLoc);
            grpBatchInfo->Controls->Add(cboStorageLoc);
            grpBatchInfo->Controls->Add(lblRemarks);
            grpBatchInfo->Controls->Add(txtRemarks);
            pnlRight->Controls->Add(grpBatchInfo);
        }

        // ── Group: Stock Level Indicator (UNCHANGED internals) ────
        void BuildStockIndicatorGroup() {
            grpIndicator = MakeGroupBox(L"  📊  Stock Level Indicator", 200, 500, 900, 95);

            Label^ lblLow = MakeLabel(L"Critical", 20, 48);
            lblLow->ForeColor = Color::DarkRed;

            pbStockLevel = gcnew ProgressBar();
            pbStockLevel->Size = System::Drawing::Size(660, 28);
            pbStockLevel->Location = System::Drawing::Point(80, 40);
            pbStockLevel->Minimum = 0;
            pbStockLevel->Maximum = 200;
            pbStockLevel->Value = 0;
            pbStockLevel->Style = ProgressBarStyle::Continuous;

            Label^ lblHigh = MakeLabel(L"Full", 752, 48);
            lblHigh->ForeColor = Color::DarkGreen;

            lblStockStatus = gcnew Label();
            lblStockStatus->Text = L"● No data";
            lblStockStatus->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.5f, FontStyle::Bold);
            lblStockStatus->ForeColor = Color::Gray;
            lblStockStatus->BackColor = Color::Transparent;
            lblStockStatus->Location = System::Drawing::Point(20, 18);
            lblStockStatus->AutoSize = true;

            lblCriticalNote = gcnew Label();
            lblCriticalNote->Text = L"";
            lblCriticalNote->Font = gcnew System::Drawing::Font(L"Segoe UI", 8.5f, FontStyle::Italic);
            lblCriticalNote->ForeColor = colorCrimson;
            lblCriticalNote->BackColor = Color::Transparent;
            lblCriticalNote->Location = System::Drawing::Point(380, 18);
            lblCriticalNote->AutoSize = true;

            grpIndicator->Controls->Add(lblLow);
            grpIndicator->Controls->Add(pbStockLevel);
            grpIndicator->Controls->Add(lblHigh);
            grpIndicator->Controls->Add(lblStockStatus);
            grpIndicator->Controls->Add(lblCriticalNote);
            pnlRight->Controls->Add(grpIndicator);
        }

        // ════════════════════════════════════════════════════════
        //  CLOCK TIMER (UNCHANGED)
        // ════════════════════════════════════════════════════════
        void StartClock() {
            tmrClock = gcnew System::Windows::Forms::Timer(this->components);
            tmrClock->Interval = 1000;
            tmrClock->Tick += gcnew EventHandler(this, &UpdateStockForm::tmrClock_Tick);
            tmrClock->Start();
        }

        void tmrClock_Tick(Object^ sender, EventArgs^ e) {
            lblDateTime->Text = DateTime::Now.ToString(L"dddd, dd MMM yyyy   HH:mm:ss");
        }

        // ════════════════════════════════════════════════════════
        //  HELPER BUILDERS (UNCHANGED)
        // ════════════════════════════════════════════════════════
        GroupBox^ MakeGroupBox(String^ title, int x, int y, int w, int h) {
            GroupBox^ gb = gcnew GroupBox();
            gb->Text = title;
            gb->Size = System::Drawing::Size(w, h);
            gb->Location = System::Drawing::Point(x, y);
            gb->Font = gcnew System::Drawing::Font(L"Segoe UI", 9.5f, FontStyle::Bold);
            gb->ForeColor = colorCrimson;
            gb->BackColor = colorWhite;
            gb->Padding = System::Windows::Forms::Padding(8);
            return gb;
        }

        Label^ MakeLabel(String^ text, int x, int y) {
            Label^ lbl = gcnew Label();
            lbl->Text = text;
            lbl->Location = System::Drawing::Point(x, y);
            lbl->AutoSize = true;
            lbl->ForeColor = colorTextDark;
            return lbl;
        }

        NumericUpDown^ MakeNUD(int x, int y, int min, int max, int val) {
            NumericUpDown^ nud = gcnew NumericUpDown();
            nud->Size = System::Drawing::Size(100, 26);
            nud->Location = System::Drawing::Point(x, y);
            nud->Minimum = min;
            nud->Maximum = max;
            nud->Value = val;
            nud->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
            return nud;
        }

        TextBox^ MakeTextBox(int x, int y, int w) {
            TextBox^ tb = gcnew TextBox();
            tb->Size = System::Drawing::Size(w, 26);
            tb->Location = System::Drawing::Point(x, y);
            tb->BorderStyle = BorderStyle::FixedSingle;
            return tb;
        }

        // ════════════════════════════════════════════════════════
        //  BUSINESS LOGIC (UNCHANGED)
        // ════════════════════════════════════════════════════════
        void RefreshCalculations() {
            int current = (int)nudCurrentStock->Value;
            int added = (int)nudUnitsAdded->Value;
            int removed = (int)nudUnitsRemoved->Value;
            int newTotal = current + added - removed;

            if (newTotal < 0) newTotal = 0;

            lblNewTotalVal->Text = newTotal.ToString();

            if (newTotal == 0)
                lblNewTotalVal->ForeColor = Color::DarkGray;
            else if (newTotal < 20)
                lblNewTotalVal->ForeColor = Color::DarkRed;
            else if (newTotal < 50)
                lblNewTotalVal->ForeColor = Color::OrangeRed;
            else
                lblNewTotalVal->ForeColor = Color::FromArgb(0, 120, 0);

            int pbVal = Math::Min(newTotal, 200);
            pbStockLevel->Value = pbVal;

            if (newTotal == 0) {
                lblStockStatus->Text = L"● OUT OF STOCK";
                lblStockStatus->ForeColor = Color::DarkRed;
                lblCriticalNote->Text = L"⚠ Immediate replenishment required!";
            }
            else if (newTotal < 20) {
                lblStockStatus->Text = L"● CRITICAL — Low Stock";
                lblStockStatus->ForeColor = Color::DarkRed;
                lblCriticalNote->Text = L"⚠ Alert: Below safe threshold (20 units)";
            }
            else if (newTotal < 50) {
                lblStockStatus->Text = L"● MODERATE — Adequate";
                lblStockStatus->ForeColor = Color::OrangeRed;
                lblCriticalNote->Text = L"Consider scheduling a donation drive soon.";
            }
            else {
                lblStockStatus->Text = L"● GOOD — Stock Sufficient";
                lblStockStatus->ForeColor = Color::FromArgb(0, 120, 0);
                lblCriticalNote->Text = L"";
            }

            SetStatus(L"Total updated: " + newTotal.ToString() + L" units calculated.");
        }

        void UpdateBloodGroupLabel() {
            if (cboBloodType->SelectedIndex >= 0 && cboRhFactor->SelectedIndex >= 0) {
                String^ type = cboBloodType->SelectedItem->ToString();
                String^ rh = (cboRhFactor->SelectedIndex == 0) ? L"+" : L"−";
                lblBloodGroupVal->Text = type + rh;
            }
            else {
                lblBloodGroupVal->Text = L"—";
            }
        }

        void SetStatus(String^ msg) {
            if (lblStatusBar != nullptr)
                lblStatusBar->Text = L"[" + DateTime::Now.ToString(L"HH:mm:ss") + L"]  " + msg;
        }

        // ════════════════════════════════════════════════════════
        //  POPULATE MODEL (UNCHANGED)
        // ════════════════════════════════════════════════════════
        void PopulateModel() {
            if (cboBloodType->SelectedIndex >= 0)
                _stock->BloodType = cboBloodType->SelectedItem->ToString();
            _stock->RhFactor = (cboRhFactor->SelectedIndex == 0) ? L"+" : L"-";
            _stock->UnitsAvailable = (int)nudCurrentStock->Value;
            _stock->UnitsAdded = (int)nudUnitsAdded->Value;
            _stock->UnitsRemoved = (int)nudUnitsRemoved->Value;
            _stock->ExpiryDate = dtpExpiryDate->Value;
            _stock->DonorID = txtDonorID->Text->Trim();
            if (cboStorageLoc->SelectedIndex >= 0)
                _stock->StorageLocation = cboStorageLoc->SelectedItem->ToString();
            _stock->Remarks = txtRemarks->Text->Trim();
        }

        // ════════════════════════════════════════════════════════
        //  CLEAR / RESET FORM (UNCHANGED)
        // ════════════════════════════════════════════════════════
        void ClearForm() {
            cboBloodType->SelectedIndex = -1;
            cboRhFactor->SelectedIndex = 0;
            nudCurrentStock->Value = 0;
            nudUnitsAdded->Value = 0;
            nudUnitsRemoved->Value = 0;
            dtpExpiryDate->Value = DateTime::Today.AddDays(42);
            txtDonorID->Clear();
            cboStorageLoc->SelectedIndex = -1;
            txtRemarks->Clear();
            lblBloodGroupVal->Text = L"—";
            RefreshCalculations();
            SetStatus(L"Form cleared – ready for new entry.");
        }

        // ════════════════════════════════════════════════════════
        //  EVENT HANDLERS (UNCHANGED)
        // ════════════════════════════════════════════════════════
        void OnStockValueChanged(Object^ sender, EventArgs^ e) {
            RefreshCalculations();
        }

        void OnBloodTypeChanged(Object^ sender, EventArgs^ e) {
            UpdateBloodGroupLabel();
        }

        void btnNew_Click(Object^ sender, EventArgs^ e) {
            if (MessageBox::Show(
                L"Clear the form and start a new stock entry?",
                L"New Entry",
                MessageBoxButtons::YesNo,
                MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
                _stock = gcnew BloodStock();
                ClearForm();
            }
        }

        void btnSave_Click(Object^ sender, EventArgs^ e) {
            PopulateModel();
            String^ error;
            if (!_stock->IsValid(error)) {
                MessageBox::Show(error, L"Validation Error",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                SetStatus(L"Save failed – " + error);
                return;
            }
            // TODO: Call your data access layer / database here
            MessageBox::Show(
                L"Blood stock updated successfully!\n\n"
                L"Blood Group : " + _stock->BloodType + _stock->RhFactor + L"\n"
                L"New Total   : " + _stock->GetUpdatedTotal().ToString() + L" units\n"
                L"Expiry Date : " + _stock->ExpiryDate.ToShortDateString(),
                L"Stock Updated",
                MessageBoxButtons::OK,
                MessageBoxIcon::Information);
            SetStatus(L"Stock saved – " + _stock->BloodType + _stock->RhFactor +
                L" | Total: " + _stock->GetUpdatedTotal().ToString() + L" units.");
        }

        void btnDelete_Click(Object^ sender, EventArgs^ e) {
            if (MessageBox::Show(
                L"Are you sure you want to DELETE this stock record?\nThis action cannot be undone.",
                L"Confirm Delete",
                MessageBoxButtons::YesNo,
                MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes) {
                // TODO: Call your data access layer / database delete here
                ClearForm();
                SetStatus(L"Record deleted.");
            }
        }

        void btnPrint_Click(Object^ sender, EventArgs^ e) {
            MessageBox::Show(L"Print functionality will be connected to your report generator.",
                L"Print", MessageBoxButtons::OK, MessageBoxIcon::Information);
            SetStatus(L"Print requested.");
        }

        void btnRefresh_Click(Object^ sender, EventArgs^ e) {
            RefreshCalculations();
            SetStatus(L"Refreshed.");
        }

        void btnClose_Click(Object^ sender, EventArgs^ e) {
            this->Close();
        }
    };
}
