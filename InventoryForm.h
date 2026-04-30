#pragma once
#include "MyForm.h"   // for GetRoundedRect / DrawHeartbeatIcon if shared; or keep self-contained

namespace BloodBank {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;

	public ref class InventoryForm : public System::Windows::Forms::Form
	{
	public:
		InventoryForm(void)
		{
			InitializeComponent();
		}

	protected:
		~InventoryForm()
		{
			if (components) delete components;
		}

		// ── Controls ────────────────────────────────────────────────────────────────
	private:
		// Panels
		System::Windows::Forms::Panel^ leftPanel;
		System::Windows::Forms::Panel^ rightPanel;

		// Left-panel branding
		System::Windows::Forms::Label^ lblLogo;
		System::Windows::Forms::Label^ lblBrandName;

		// Left-panel page header
		System::Windows::Forms::Label^ lblPageTitle;
		System::Windows::Forms::Label^ lblPageSubTitle;

		// Summary cards (left panel)
		System::Windows::Forms::Panel^ cardTotalUnits;
		System::Windows::Forms::Label^ lblCardTotalTitle;
		System::Windows::Forms::Label^ lblCardTotalValue;

		System::Windows::Forms::Panel^ cardCritical;
		System::Windows::Forms::Label^ lblCardCriticalTitle;
		System::Windows::Forms::Label^ lblCardCriticalValue;

		System::Windows::Forms::Panel^ cardExpiringSoon;
		System::Windows::Forms::Label^ lblCardExpiringTitle;
		System::Windows::Forms::Label^ lblCardExpiringValue;

		// Right-panel toolbar
		System::Windows::Forms::Panel^ toolbarPanel;
		System::Windows::Forms::Label^ lblSectionTitle;
		System::Windows::Forms::Button^ btnAdd;
		System::Windows::Forms::Button^ btnDelete;
		System::Windows::Forms::Button^ btnRefresh;
		System::Windows::Forms::TextBox^ txtSearch;
		System::Windows::Forms::Label^ lblSearchIcon;
		System::Windows::Forms::Panel^ searchBox;

		// Grid
		System::Windows::Forms::DataGridView^ gridInventory;

		// Status
		System::Windows::Forms::Label^ lblStatus;

		System::ComponentModel::Container^ components;

		// ── InitializeComponent ─────────────────────────────────────────────────────
#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
			this->leftPanel = (gcnew System::Windows::Forms::Panel());
			this->lblLogo = (gcnew System::Windows::Forms::Label());
			this->lblBrandName = (gcnew System::Windows::Forms::Label());
			this->lblPageTitle = (gcnew System::Windows::Forms::Label());
			this->lblPageSubTitle = (gcnew System::Windows::Forms::Label());
			this->cardTotalUnits = (gcnew System::Windows::Forms::Panel());
			this->cardCritical = (gcnew System::Windows::Forms::Panel());
			this->cardExpiringSoon = (gcnew System::Windows::Forms::Panel());
			this->rightPanel = (gcnew System::Windows::Forms::Panel());
			this->gridInventory = (gcnew System::Windows::Forms::DataGridView());
			this->lblStatus = (gcnew System::Windows::Forms::Label());
			this->toolbarPanel = (gcnew System::Windows::Forms::Panel());
			this->lblSectionTitle = (gcnew System::Windows::Forms::Label());
			this->btnAdd = (gcnew System::Windows::Forms::Button());
			this->btnDelete = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->searchBox = (gcnew System::Windows::Forms::Panel());
			this->lblSearchIcon = (gcnew System::Windows::Forms::Label());
			this->txtSearch = (gcnew System::Windows::Forms::TextBox());
			this->lblCardTotalTitle = (gcnew System::Windows::Forms::Label());
			this->lblCardTotalValue = (gcnew System::Windows::Forms::Label());
			this->lblCardCriticalTitle = (gcnew System::Windows::Forms::Label());
			this->lblCardCriticalValue = (gcnew System::Windows::Forms::Label());
			this->lblCardExpiringTitle = (gcnew System::Windows::Forms::Label());
			this->lblCardExpiringValue = (gcnew System::Windows::Forms::Label());
			this->leftPanel->SuspendLayout();
			this->rightPanel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gridInventory))->BeginInit();
			this->toolbarPanel->SuspendLayout();
			this->searchBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// leftPanel
			// 
			this->leftPanel->BackColor = System::Drawing::Color::White;
			this->leftPanel->Controls->Add(this->lblLogo);
			this->leftPanel->Controls->Add(this->lblBrandName);
			this->leftPanel->Controls->Add(this->lblPageTitle);
			this->leftPanel->Controls->Add(this->lblPageSubTitle);
			this->leftPanel->Controls->Add(this->cardTotalUnits);
			this->leftPanel->Controls->Add(this->cardCritical);
			this->leftPanel->Controls->Add(this->cardExpiringSoon);
			this->leftPanel->Dock = System::Windows::Forms::DockStyle::Left;
			this->leftPanel->Location = System::Drawing::Point(0, 0);
			this->leftPanel->Name = L"leftPanel";
			this->leftPanel->Size = System::Drawing::Size(420, 750);
			this->leftPanel->TabIndex = 1;
			this->leftPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &InventoryForm::leftPanel_Paint);
			// 
			// lblLogo
			// 
			this->lblLogo->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
				static_cast<System::Int32>(static_cast<System::Byte>(235)));
			this->lblLogo->Font = (gcnew System::Drawing::Font(L"Segoe UI Symbol", 16));
			this->lblLogo->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->lblLogo->Location = System::Drawing::Point(0, 0);
			this->lblLogo->Name = L"lblLogo";
			this->lblLogo->Size = System::Drawing::Size(40, 40);
			this->lblLogo->TabIndex = 0;
			this->lblLogo->Text = L"💧";
			this->lblLogo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lblBrandName
			// 
			this->lblBrandName->AutoSize = true;
			this->lblBrandName->Font = (gcnew System::Drawing::Font(L"Segoe UI", 16, System::Drawing::FontStyle::Bold));
			this->lblBrandName->ForeColor = System::Drawing::Color::Black;
			this->lblBrandName->Location = System::Drawing::Point(0, 0);
			this->lblBrandName->Name = L"lblBrandName";
			this->lblBrandName->Size = System::Drawing::Size(164, 45);
			this->lblBrandName->TabIndex = 1;
			this->lblBrandName->Text = L"LifeBlood";
			// 
			// lblPageTitle
			// 
			this->lblPageTitle->AutoSize = true;
			this->lblPageTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 26, System::Drawing::FontStyle::Bold));
			this->lblPageTitle->ForeColor = System::Drawing::Color::Black;
			this->lblPageTitle->Location = System::Drawing::Point(0, 0);
			this->lblPageTitle->Name = L"lblPageTitle";
			this->lblPageTitle->Size = System::Drawing::Size(424, 70);
			this->lblPageTitle->TabIndex = 2;
			this->lblPageTitle->Text = L"Blood Inventory";
			// 
			// lblPageSubTitle
			// 
			this->lblPageSubTitle->AutoSize = true;
			this->lblPageSubTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11));
			this->lblPageSubTitle->ForeColor = System::Drawing::Color::Gray;
			this->lblPageSubTitle->Location = System::Drawing::Point(0, 0);
			this->lblPageSubTitle->Name = L"lblPageSubTitle";
			this->lblPageSubTitle->Size = System::Drawing::Size(391, 30);
			this->lblPageSubTitle->TabIndex = 3;
			this->lblPageSubTitle->Text = L"Manage and track available blood units";
			// 
			// cardTotalUnits
			// 
			this->cardTotalUnits->Location = System::Drawing::Point(0, 0);
			this->cardTotalUnits->Name = L"cardTotalUnits";
			this->cardTotalUnits->Size = System::Drawing::Size(200, 100);
			this->cardTotalUnits->TabIndex = 4;
			// 
			// cardCritical
			// 
			this->cardCritical->Location = System::Drawing::Point(0, 0);
			this->cardCritical->Name = L"cardCritical";
			this->cardCritical->Size = System::Drawing::Size(200, 100);
			this->cardCritical->TabIndex = 5;
			// 
			// cardExpiringSoon
			// 
			this->cardExpiringSoon->Location = System::Drawing::Point(0, 0);
			this->cardExpiringSoon->Name = L"cardExpiringSoon";
			this->cardExpiringSoon->Size = System::Drawing::Size(200, 100);
			this->cardExpiringSoon->TabIndex = 6;
			// 
			// rightPanel
			// 
			this->rightPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)), static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->rightPanel->Controls->Add(this->gridInventory);
			this->rightPanel->Controls->Add(this->lblStatus);
			this->rightPanel->Controls->Add(this->toolbarPanel);
			this->rightPanel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rightPanel->Location = System::Drawing::Point(420, 0);
			this->rightPanel->Name = L"rightPanel";
			this->rightPanel->Size = System::Drawing::Size(780, 750);
			this->rightPanel->TabIndex = 0;
			this->rightPanel->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &InventoryForm::rightPanel_Paint);
			// 
			// gridInventory
			// 
			this->gridInventory->AllowUserToAddRows = false;
			dataGridViewCellStyle1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(252)),
				static_cast<System::Int32>(static_cast<System::Byte>(248)), static_cast<System::Int32>(static_cast<System::Byte>(249)));
			this->gridInventory->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
			this->gridInventory->BackgroundColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(250)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)), static_cast<System::Int32>(static_cast<System::Byte>(252)));
			this->gridInventory->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->gridInventory->CellBorderStyle = System::Windows::Forms::DataGridViewCellBorderStyle::SingleHorizontal;
			dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)),
				static_cast<System::Int32>(static_cast<System::Byte>(15)), static_cast<System::Int32>(static_cast<System::Byte>(59)));
			dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			dataGridViewCellStyle2->ForeColor = System::Drawing::Color::White;
			dataGridViewCellStyle2->Padding = System::Windows::Forms::Padding(10, 0, 0, 0);
			dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
			dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
			dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->gridInventory->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
			this->gridInventory->ColumnHeadersHeight = 44;
			dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
			dataGridViewCellStyle3->BackColor = System::Drawing::Color::White;
			dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			dataGridViewCellStyle3->ForeColor = System::Drawing::SystemColors::ControlText;
			dataGridViewCellStyle3->Padding = System::Windows::Forms::Padding(10, 0, 0, 0);
			dataGridViewCellStyle3->SelectionBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(220)), static_cast<System::Int32>(static_cast<System::Byte>(228)));
			dataGridViewCellStyle3->SelectionForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(180)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
			dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
			this->gridInventory->DefaultCellStyle = dataGridViewCellStyle3;
			this->gridInventory->Dock = System::Windows::Forms::DockStyle::Fill;
			this->gridInventory->EnableHeadersVisualStyles = false;
			this->gridInventory->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->gridInventory->GridColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(235)), static_cast<System::Int32>(static_cast<System::Byte>(237)),
				static_cast<System::Int32>(static_cast<System::Byte>(242)));
			this->gridInventory->Location = System::Drawing::Point(0, 70);
			this->gridInventory->Name = L"gridInventory";
			this->gridInventory->RowHeadersVisible = false;
			this->gridInventory->RowHeadersWidth = 62;
			this->gridInventory->RowTemplate->Height = 44;
			this->gridInventory->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->gridInventory->Size = System::Drawing::Size(780, 651);
			this->gridInventory->TabIndex = 0;
			// 
			// lblStatus
			// 
			this->lblStatus->AutoSize = true;
			this->lblStatus->BackColor = System::Drawing::Color::White;
			this->lblStatus->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->lblStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->lblStatus->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(232)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
				static_cast<System::Int32>(static_cast<System::Byte>(59)));
			this->lblStatus->Location = System::Drawing::Point(0, 721);
			this->lblStatus->Name = L"lblStatus";
			this->lblStatus->Padding = System::Windows::Forms::Padding(20, 4, 0, 0);
			this->lblStatus->Size = System::Drawing::Size(20, 29);
			this->lblStatus->TabIndex = 1;
			// 
			// toolbarPanel
			// 
			this->toolbarPanel->BackColor = System::Drawing::Color::White;
			this->toolbarPanel->Controls->Add(this->lblSectionTitle);
			this->toolbarPanel->Controls->Add(this->btnAdd);
			this->toolbarPanel->Controls->Add(this->btnDelete);
			this->toolbarPanel->Controls->Add(this->btnRefresh);
			this->toolbarPanel->Controls->Add(this->searchBox);
			this->toolbarPanel->Dock = System::Windows::Forms::DockStyle::Top;
			this->toolbarPanel->Location = System::Drawing::Point(0, 0);
			this->toolbarPanel->Name = L"toolbarPanel";
			this->toolbarPanel->Padding = System::Windows::Forms::Padding(0, 0, 0, 1);
			this->toolbarPanel->Size = System::Drawing::Size(780, 70);
			this->toolbarPanel->TabIndex = 2;
			// 
			// lblSectionTitle
			// 
			this->lblSectionTitle->AutoSize = true;
			this->lblSectionTitle->Font = (gcnew System::Drawing::Font(L"Segoe UI", 13, System::Drawing::FontStyle::Bold));
			this->lblSectionTitle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(40)),
				static_cast<System::Int32>(static_cast<System::Byte>(40)));
			this->lblSectionTitle->Location = System::Drawing::Point(20, 22);
			this->lblSectionTitle->Name = L"lblSectionTitle";
			this->lblSectionTitle->Size = System::Drawing::Size(236, 36);
			this->lblSectionTitle->TabIndex = 0;
			this->lblSectionTitle->Text = L"Inventory Records";
			// 
			// btnAdd
			// 
			this->btnAdd->Location = System::Drawing::Point(0, 0);
			this->btnAdd->Name = L"btnAdd";
			this->btnAdd->Size = System::Drawing::Size(75, 23);
			this->btnAdd->TabIndex = 1;
			this->btnAdd->Click += gcnew System::EventHandler(this, &InventoryForm::btnAdd_Click);
			// 
			// btnDelete
			// 
			this->btnDelete->Location = System::Drawing::Point(0, 0);
			this->btnDelete->Name = L"btnDelete";
			this->btnDelete->Size = System::Drawing::Size(75, 23);
			this->btnDelete->TabIndex = 2;
			this->btnDelete->Click += gcnew System::EventHandler(this, &InventoryForm::btnDelete_Click);
			// 
			// btnRefresh
			// 
			this->btnRefresh->Location = System::Drawing::Point(0, 0);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(75, 23);
			this->btnRefresh->TabIndex = 3;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &InventoryForm::btnRefresh_Click);
			// 
			// searchBox
			// 
			this->searchBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->searchBox->Controls->Add(this->lblSearchIcon);
			this->searchBox->Controls->Add(this->txtSearch);
			this->searchBox->Location = System::Drawing::Point(0, 0);
			this->searchBox->Name = L"searchBox";
			this->searchBox->Size = System::Drawing::Size(200, 36);
			this->searchBox->TabIndex = 4;
			// 
			// lblSearchIcon
			// 
			this->lblSearchIcon->AutoSize = true;
			this->lblSearchIcon->Font = (gcnew System::Drawing::Font(L"Segoe MDL2 Assets", 11));
			this->lblSearchIcon->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(150)), static_cast<System::Int32>(static_cast<System::Byte>(160)),
				static_cast<System::Int32>(static_cast<System::Byte>(175)));
			this->lblSearchIcon->Location = System::Drawing::Point(10, 8);
			this->lblSearchIcon->Name = L"lblSearchIcon";
			this->lblSearchIcon->Size = System::Drawing::Size(32, 22);
			this->lblSearchIcon->TabIndex = 0;
			this->lblSearchIcon->Text = L"";
			// 
			// txtSearch
			// 
			this->txtSearch->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(247)),
				static_cast<System::Int32>(static_cast<System::Byte>(250)));
			this->txtSearch->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->txtSearch->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->txtSearch->ForeColor = System::Drawing::Color::Gray;
			this->txtSearch->Location = System::Drawing::Point(34, 9);
			this->txtSearch->Name = L"txtSearch";
			this->txtSearch->Size = System::Drawing::Size(155, 27);
			this->txtSearch->TabIndex = 1;
			this->txtSearch->Text = L"Search...";
			this->txtSearch->Enter += gcnew System::EventHandler(this, &InventoryForm::txtSearch_Enter);
			this->txtSearch->Leave += gcnew System::EventHandler(this, &InventoryForm::txtSearch_Leave);
			// 
			// lblCardTotalTitle
			// 
			this->lblCardTotalTitle->Location = System::Drawing::Point(0, 0);
			this->lblCardTotalTitle->Name = L"lblCardTotalTitle";
			this->lblCardTotalTitle->Size = System::Drawing::Size(100, 23);
			this->lblCardTotalTitle->TabIndex = 0;
			// 
			// lblCardTotalValue
			// 
			this->lblCardTotalValue->Location = System::Drawing::Point(0, 0);
			this->lblCardTotalValue->Name = L"lblCardTotalValue";
			this->lblCardTotalValue->Size = System::Drawing::Size(100, 23);
			this->lblCardTotalValue->TabIndex = 0;
			// 
			// lblCardCriticalTitle
			// 
			this->lblCardCriticalTitle->Location = System::Drawing::Point(0, 0);
			this->lblCardCriticalTitle->Name = L"lblCardCriticalTitle";
			this->lblCardCriticalTitle->Size = System::Drawing::Size(100, 23);
			this->lblCardCriticalTitle->TabIndex = 0;
			// 
			// lblCardCriticalValue
			// 
			this->lblCardCriticalValue->Location = System::Drawing::Point(0, 0);
			this->lblCardCriticalValue->Name = L"lblCardCriticalValue";
			this->lblCardCriticalValue->Size = System::Drawing::Size(100, 23);
			this->lblCardCriticalValue->TabIndex = 0;
			// 
			// lblCardExpiringTitle
			// 
			this->lblCardExpiringTitle->Location = System::Drawing::Point(0, 0);
			this->lblCardExpiringTitle->Name = L"lblCardExpiringTitle";
			this->lblCardExpiringTitle->Size = System::Drawing::Size(100, 23);
			this->lblCardExpiringTitle->TabIndex = 0;
			// 
			// lblCardExpiringValue
			// 
			this->lblCardExpiringValue->Location = System::Drawing::Point(0, 0);
			this->lblCardExpiringValue->Name = L"lblCardExpiringValue";
			this->lblCardExpiringValue->Size = System::Drawing::Size(100, 23);
			this->lblCardExpiringValue->TabIndex = 0;
			// 
			// InventoryForm
			// 
			this->ClientSize = System::Drawing::Size(1200, 750);
			this->Controls->Add(this->rightPanel);
			this->Controls->Add(this->leftPanel);
			this->Name = L"InventoryForm";
			this->Text = L"LifeBlood MMS – Inventory";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &InventoryForm::InventoryForm_Load);
			this->Resize += gcnew System::EventHandler(this, &InventoryForm::InventoryForm_Resize);
			this->leftPanel->ResumeLayout(false);
			this->leftPanel->PerformLayout();
			this->rightPanel->ResumeLayout(false);
			this->rightPanel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->gridInventory))->EndInit();
			this->toolbarPanel->ResumeLayout(false);
			this->toolbarPanel->PerformLayout();
			this->searchBox->ResumeLayout(false);
			this->searchBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

		// ── Helpers ─────────────────────────────────────────────────────────────────
	private:

		// Build a glass-tinted stat card on the left panel
		void SetupCard(Panel^ card, Label^ title, Label^ value,
			String^ titleText, String^ valueText, Color bg)
		{
			card->BackColor = bg;
			card->Size = System::Drawing::Size(340, 72);
			card->Cursor = System::Windows::Forms::Cursors::Hand;

			title->AutoSize = true;
			title->Font = gcnew System::Drawing::Font(L"Segoe UI", 8, System::Drawing::FontStyle::Bold);
			title->ForeColor = System::Drawing::Color::FromArgb(200, 232, 15, 59);
			title->BackColor = System::Drawing::Color::Transparent;
			title->Text = titleText;
			title->Location = System::Drawing::Point(16, 14);

			value->AutoSize = true;
			value->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold);
			value->ForeColor = System::Drawing::Color::FromArgb(232, 15, 59);
			value->BackColor = System::Drawing::Color::Transparent;
			value->Text = valueText;
			value->Location = System::Drawing::Point(16, 34);

			card->Controls->Add(title);
			card->Controls->Add(value);
			card->Paint += gcnew System::Windows::Forms::PaintEventHandler(
				this, &InventoryForm::card_Paint);
		}
		void SetupGridColumns()
		{
			gridInventory->Columns->Clear();

			gridInventory->Columns->Add("BloodGroup", "Blood Group");
			gridInventory->Columns->Add("Units", "Units");
			gridInventory->Columns->Add("Threshold", "Threshold");
			gridInventory->Columns->Add("Status", "Status");
			gridInventory->Columns->Add("Collected", "Collected Date");
			gridInventory->Columns->Add("Expiry", "Expiry Date");
			gridInventory->Columns->Add("Location", "Storage Location");
		}

		// Style a toolbar action button
		void SetupActionButton(Button^ btn, String^ text, Color bg)
		{
			btn->Text = text;
			btn->Height = 36;
			btn->Width = 120;
			btn->BackColor = bg;
			btn->ForeColor = System::Drawing::Color::White;
			btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			btn->FlatAppearance->BorderSize = 0;
			btn->Font = gcnew System::Drawing::Font(L"Segoe UI", 9, System::Drawing::FontStyle::Bold);
			btn->Cursor = System::Windows::Forms::Cursors::Hand;
			btn->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		}

		// Add an auto-sizing grid column (width -1 => fill)
		void AddColumn(DataGridView^ grid, String^ key, String^ header, int width)
		{
			DataGridViewTextBoxColumn^ col = gcnew DataGridViewTextBoxColumn();
			col->Name = key;
			col->HeaderText = header;
			if (width < 0)
				col->AutoSizeMode = DataGridViewAutoSizeColumnMode::Fill;
			else
				col->Width = width;
			grid->Columns->Add(col);
		}

		// Rounded rect utility (same as login form)
		GraphicsPath^ GetRoundedRect(Rectangle bounds, int radius)
		{
			GraphicsPath^ path = gcnew GraphicsPath();
			int d = radius * 2;
			Rectangle arc = Rectangle(bounds.Location, System::Drawing::Size(d, d));
			path->AddArc(arc, 180, 90);
			arc.X = bounds.Right - d;
			path->AddArc(arc, 270, 90);
			arc.Y = bounds.Bottom - d;
			path->AddArc(arc, 0, 90);
			arc.X = bounds.Left;
			path->AddArc(arc, 90, 90);
			path->CloseFigure();
			return path;
		}

		// Draw the same heartbeat / EKG icon as the login form
		void DrawHeartbeatIcon(Graphics^ g, Rectangle bounds)
		{
			g->SmoothingMode = SmoothingMode::AntiAlias;
			Pen^ iconPen = gcnew Pen(Color::FromArgb(255, 205, 214), 4.4f);
			iconPen->LineJoin = LineJoin::Round;
			iconPen->StartCap = LineCap::Round;
			iconPen->EndCap = LineCap::Round;

			int w = bounds.Width, h = bounds.Height, x = bounds.X, y = bounds.Y;
			Point pTop = Point(x + w / 2, y + static_cast<int>(h * 0.28f));
			Point pBottom = Point(x + w / 2, y + h - 4);

			GraphicsPath^ heart = gcnew GraphicsPath();
			heart->StartFigure();
			heart->AddBezier(pTop,
				Point(x + static_cast<int>(w * 0.18f), y - 2),
				Point(x - 2, y + static_cast<int>(h * 0.38f)),
				Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)));
			heart->AddBezier(
				Point(x + static_cast<int>(w * 0.24f), y + static_cast<int>(h * 0.60f)),
				Point(x + static_cast<int>(w * 0.34f), y + static_cast<int>(h * 0.74f)),
				Point(x + static_cast<int>(w * 0.43f), y + static_cast<int>(h * 0.88f)),
				pBottom);
			heart->AddBezier(pBottom,
				Point(x + static_cast<int>(w * 0.57f), y + static_cast<int>(h * 0.88f)),
				Point(x + static_cast<int>(w * 0.66f), y + static_cast<int>(h * 0.74f)),
				Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)));
			heart->AddBezier(
				Point(x + static_cast<int>(w * 0.76f), y + static_cast<int>(h * 0.60f)),
				Point(x + w + 2, y + static_cast<int>(h * 0.38f)),
				Point(x + static_cast<int>(w * 0.82f), y - 2),
				pTop);
			g->DrawPath(iconPen, heart);

			array<Point>^ ekg = {
				Point(x + static_cast<int>(w * 0.16f), y + static_cast<int>(h * 0.52f)),
				Point(x + static_cast<int>(w * 0.35f), y + static_cast<int>(h * 0.52f)),
				Point(x + static_cast<int>(w * 0.44f), y + static_cast<int>(h * 0.38f)),
				Point(x + static_cast<int>(w * 0.53f), y + static_cast<int>(h * 0.70f)),
				Point(x + static_cast<int>(w * 0.64f), y + static_cast<int>(h * 0.50f)),
				Point(x + static_cast<int>(w * 0.83f), y + static_cast<int>(h * 0.50f))
			};
			g->DrawLines(iconPen, ekg);

			delete heart;
			delete iconPen;
		}

		// ── Layout ──────────────────────────────────────────────────────────────────
	private:

		void ArrangeLeftPanel()
		{
			int lw = leftPanel->Width;

			// Branding row
			lblLogo->Location = System::Drawing::Point(40, 38);
			lblBrandName->Location = System::Drawing::Point(90, 43);

			GraphicsPath^ logoBox = GetRoundedRect(
				Rectangle(0, 0, lblLogo->Width - 1, lblLogo->Height - 1), 10);
			lblLogo->Region = gcnew System::Drawing::Region(logoBox);
			delete logoBox;

			// Page title block
			lblPageTitle->Location = System::Drawing::Point(40, 130);
			lblPageSubTitle->Location = System::Drawing::Point(40, 185);
			// Small divider line under subtitle
			Panel^ divider = gcnew Panel();
			divider->BackColor = Color::FromArgb(220, 255, 255, 255);
			divider->Size = System::Drawing::Size(320, 2);
			divider->Location = System::Drawing::Point(40, 220);
			leftPanel->Controls->Add(divider);
			// Summary cards (stacked, centered)
			int cardX = (lw - 340) / 2;
			cardTotalUnits->Location = System::Drawing::Point(cardX, 250);
			cardCritical->Location = System::Drawing::Point(cardX, 330);
			cardExpiringSoon->Location = System::Drawing::Point(cardX, 410);
			// Blood group mini summary panel




			// Apply rounded corners to cards
			for each (Panel ^ c in gcnew array<Panel^>{ cardTotalUnits, cardCritical, cardExpiringSoon })
			{
				GraphicsPath^ r = GetRoundedRect(Rectangle(0, 0, c->Width - 1, c->Height - 1), 14);
				c->Region = gcnew System::Drawing::Region(r);
				delete r;
			}
		}

		void ArrangeToolbar()
		{
			int tw = toolbarPanel->Width;

			// Search box — far right
			searchBox->Location = System::Drawing::Point(tw - 220, 17);
			GraphicsPath^ sb = GetRoundedRect(
				Rectangle(0, 0, searchBox->Width - 1, searchBox->Height - 1), 10);
			searchBox->Region = gcnew System::Drawing::Region(sb);
			delete sb;

			// Action buttons — right of title, left of search
			int bx = tw - 220 - (3 * 128) - 10;
			btnAdd->Location = System::Drawing::Point(bx, 17);
			btnDelete->Location = System::Drawing::Point(bx + 128, 17);
			btnRefresh->Location = System::Drawing::Point(bx + 256, 17);

			// Round all buttons
			for each (Button ^ b in gcnew array<Button^>{ btnAdd, btnDelete, btnRefresh })
			{
				GraphicsPath^ br = GetRoundedRect(Rectangle(0, 0, b->Width - 1, b->Height - 1), 10);
				b->Region = gcnew System::Drawing::Region(br);
				delete br;
			}
		}

		// ── Events ──────────────────────────────────────────────────────────────────
	private:

		System::Void InventoryForm_Load(System::Object^ sender, System::EventArgs^ e)
		{

			// 1. set layout first
			leftPanel->Width = this->Width / 3;

			// 2. ADD THIS (IMPORTANT STEP)
			SetupGridColumns();   // <<< ADD THIS LINE HERE

			// 3. then arrange UI
			ArrangeLeftPanel();
			ArrangeToolbar();

			// 4. now load data safely
			LoadData();

			this->ActiveControl = nullptr;
		}



		System::Void InventoryForm_Resize(System::Object^ sender, System::EventArgs^ e)
		{
			leftPanel->Width = this->Width / 3;
			ArrangeLeftPanel();
			ArrangeToolbar();
			leftPanel->Invalidate();
			rightPanel->Invalidate();
		}
		;

		System::Void leftPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
		{
			e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
			Rectangle rect = leftPanel->ClientRectangle;

			LinearGradientBrush^ bg = gcnew LinearGradientBrush(rect,
				Color::FromArgb(210, 10, 45),
				Color::FromArgb(60, 20, 35),
				LinearGradientMode::ForwardDiagonal);
			e->Graphics->FillRectangle(bg, rect);

			// Heart icon card (decorative, same as login)
			SolidBrush^ glass = gcnew SolidBrush(Color::FromArgb(30, 255, 255, 255));
			Pen^ border = gcnew Pen(Color::FromArgb(50, 255, 255, 255), 1);

			int cx = leftPanel->Width / 2;
			Rectangle iconCard = Rectangle(cx - 40, leftPanel->Height - 160, 80, 80);
			GraphicsPath^ ip = GetRoundedRect(iconCard, 15);
			e->Graphics->FillPath(glass, ip);
			e->Graphics->DrawPath(border, ip);
			DrawHeartbeatIcon(e->Graphics,
				Rectangle(iconCard.X + 5, iconCard.Y + 10, 70, 60));

			// Override label colours so they show on the dark bg
			// (Labels set BackColor=Transparent; ForeColor set below in Load/Resize)
			delete bg; delete glass; delete border; delete ip;
		}
		// Override label colours after the panel paints (called once in Load)
		System::Void FixLeftLabelColors()
		{
			Color white = System::Drawing::Color::White;
			Color offWhite = System::Drawing::Color::FromArgb(220, 220, 220);

			lblLogo->BackColor = System::Drawing::Color::FromArgb(255, 230, 235);
			lblLogo->ForeColor = System::Drawing::Color::FromArgb(232, 15, 59);
			lblBrandName->ForeColor = white;
			lblBrandName->BackColor = System::Drawing::Color::Transparent;
			lblPageTitle->ForeColor = white;
			lblPageTitle->BackColor = System::Drawing::Color::Transparent;
			lblPageSubTitle->ForeColor = offWhite;
			lblPageSubTitle->BackColor = System::Drawing::Color::Transparent;

			// Card labels
			for each (Label ^ l in gcnew array<Label^>{
				lblCardTotalTitle, lblCardTotalValue,
					lblCardCriticalTitle, lblCardCriticalValue,
					lblCardExpiringTitle, lblCardExpiringValue})
			{
				l->BackColor = System::Drawing::Color::Transparent;
			}
			// Card panels – semi-transparent white tint over the gradient
			for each (Panel ^ c in gcnew array<Panel^>{
				cardTotalUnits, cardCritical, cardExpiringSoon})
			{
				c->BackColor = System::Drawing::Color::FromArgb(35, 255, 255, 255);
			}
			// Card titles
			for each (Label ^ l in gcnew array<Label^>{
				lblCardTotalTitle, lblCardCriticalTitle, lblCardExpiringTitle})
			{
				l->ForeColor = System::Drawing::Color::FromArgb(200, 255, 200, 205);
				l->Font = gcnew System::Drawing::Font(L"Segoe UI", 8, System::Drawing::FontStyle::Bold);
			}
			// Card values
			for each (Label ^ l in gcnew array<Label^>{
				lblCardTotalValue, lblCardCriticalValue, lblCardExpiringValue})
			{
				l->ForeColor = System::Drawing::Color::White;
				l->Font = gcnew System::Drawing::Font(L"Segoe UI", 18, System::Drawing::FontStyle::Bold);
			}
		}

		// ── Paint: right panel (white – just draw a subtle bottom border on toolbar)
		System::Void rightPanel_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
		{
			// Draw separator under toolbar
			int y = toolbarPanel->Bottom;
			Pen^ sep = gcnew Pen(Color::FromArgb(225, 227, 232), 1);
			e->Graphics->DrawLine(sep, 0, y, rightPanel->Width, y);
			delete sep;
		}

		// ── Card Paint: rounded glass tint ─────────────────────────────────────
		System::Void card_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
		{
			Panel^ card = safe_cast<Panel^>(sender);
			e->Graphics->SmoothingMode = SmoothingMode::AntiAlias;
			GraphicsPath^ path = GetRoundedRect(Rectangle(0, 0, card->Width - 1, card->Height - 1), 14);
			SolidBrush^ fill = gcnew SolidBrush(Color::FromArgb(35, 255, 255, 255));
			Pen^ border = gcnew Pen(Color::FromArgb(60, 255, 255, 255), 1);
			e->Graphics->FillPath(fill, path);
			e->Graphics->DrawPath(border, path);
			delete path; delete fill; delete border;
		}

		// ── Search placeholder ─────────────────────────────────────────────────
		System::Void txtSearch_Enter(System::Object^ sender, System::EventArgs^ e)
		{
			if (txtSearch->Text == L"Search...") {
				txtSearch->Text = L"";
				txtSearch->ForeColor = System::Drawing::Color::Black;
			}
		}

		System::Void txtSearch_Leave(System::Object^ sender, System::EventArgs^ e)
		{
			if (String::IsNullOrWhiteSpace(txtSearch->Text)) {
				txtSearch->Text = L"Search...";
				txtSearch->ForeColor = System::Drawing::Color::Gray;
			}
		}

		// ── Button handlers ────────────────────────────────────────────────────
		System::Void btnAdd_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// TODO: open Add Entry dialog
			MessageBox::Show(L"Open Add Entry dialog here.", L"Add", MessageBoxButtons::OK, MessageBoxIcon::Information);
		}

		System::Void btnDelete_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (gridInventory->SelectedRows->Count == 0) {
				lblStatus->Text = L"Select a row to delete.";
				return;
			}
			if (MessageBox::Show(L"Delete selected record?", L"Confirm",
				MessageBoxButtons::YesNo, MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::Yes)
			{
				gridInventory->Rows->RemoveAt(gridInventory->SelectedRows[0]->Index);
				lblStatus->Text = L"Record deleted.";
			}
		}

		System::Void btnRefresh_Click(System::Object^ sender, System::EventArgs^ e)
		{
			gridInventory->Rows->Clear();
			LoadData();
			lblStatus->Text = L"Data refreshed.";
		}

		// ── Data ────────────────────────────────────────────────────────────────────
	private:
		System::Void LoadData()
		{
			// Fix left panel styling
			FixLeftLabelColors();
			ArrangeLeftPanel();
			ArrangeToolbar();

			gridInventory->Rows->Clear();

			// =========================
			// SAFE C++/CLI ARRAY (FIXED)
			// =========================
			array<array<System::Object^>^>^ rows = gcnew array<array<System::Object^>^>(8);

			// A+
			rows[0] = gcnew array<System::Object^>(7);
			rows[0][0] = L"A+";
			rows[0][1] = L"312";
			rows[0][2] = L"50";
			rows[0][3] = L"✔ Adequate";
			rows[0][4] = L"24 Apr 2026";
			rows[0][5] = L"15 Jun 2026";
			rows[0][6] = L"Ward B – Fridge 3";

			// A-
			rows[1] = gcnew array<System::Object^>(7);
			rows[1][0] = L"A-";
			rows[1][1] = L"45";
			rows[1][2] = L"30";
			rows[1][3] = L"⚠ Low";
			rows[1][4] = L"24 Apr 2026";
			rows[1][5] = L"02 May 2026";
			rows[1][6] = L"Ward B – Fridge 1";

			// B+
			rows[2] = gcnew array<System::Object^>(7);
			rows[2][0] = L"B+";
			rows[2][1] = L"188";
			rows[2][2] = L"50";
			rows[2][3] = L"✔ Adequate";
			rows[2][4] = L"23 Apr 2026";
			rows[2][5] = L"20 Jun 2026";
			rows[2][6] = L"Ward C – Fridge 2";

			// B-
			rows[3] = gcnew array<System::Object^>(7);
			rows[3][0] = L"B-";
			rows[3][1] = L"12";
			rows[3][2] = L"20";
			rows[3][3] = L"🔴 Critical";
			rows[3][4] = L"22 Apr 2026";
			rows[3][5] = L"28 Apr 2026";
			rows[3][6] = L"Ward C – Fridge 2";

			// O+
			rows[4] = gcnew array<System::Object^>(7);
			rows[4][0] = L"O+";
			rows[4][1] = L"420";
			rows[4][2] = L"100";
			rows[4][3] = L"✔ Adequate";
			rows[4][4] = L"23 Apr 2026";
			rows[4][5] = L"25 Jun 2026";
			rows[4][6] = L"Main Storage";

			// O-
			rows[5] = gcnew array<System::Object^>(7);
			rows[5][0] = L"O-";
			rows[5][1] = L"28";
			rows[5][2] = L"20";
			rows[5][3] = L"🔴 Critical";
			rows[5][4] = L"22 Apr 2026";
			rows[5][5] = L"28 Apr 2026";
			rows[5][6] = L"Emergency Unit";

			// AB+
			rows[6] = gcnew array<System::Object^>(7);
			rows[6][0] = L"AB+";
			rows[6][1] = L"95";
			rows[6][2] = L"40";
			rows[6][3] = L"✔ Adequate";
			rows[6][4] = L"21 Apr 2026";
			rows[6][5] = L"18 Jun 2026";
			rows[6][6] = L"Ward A";

			// AB-
			rows[7] = gcnew array<System::Object^>(7);
			rows[7][0] = L"AB-";
			rows[7][1] = L"10";
			rows[7][2] = L"15";
			rows[7][3] = L"⚠ Low";
			rows[7][4] = L"20 Apr 2026";
			rows[7][5] = L"10 May 2026";
			rows[7][6] = L"Emergency Storage";

			// =========================
			// ADD TO GRID
			// =========================
			for each (array<System::Object^> ^ row in rows)
			{
				gridInventory->Rows->Add(row);
			}

			// =========================
			// STATUS COLOR FIX (SAFE)
			// =========================
			for each (DataGridViewRow ^ row in gridInventory->Rows)
			{
				if (row->Cells[3]->Value == nullptr)
					continue;

				String^ status = row->Cells[3]->Value->ToString();

				if (status->Contains(L"Critical"))
					row->Cells[3]->Style->ForeColor = System::Drawing::Color::FromArgb(200, 0, 30);
				else if (status->Contains(L"Low"))
					row->Cells[3]->Style->ForeColor = System::Drawing::Color::FromArgb(180, 100, 0);
				else
					row->Cells[3]->Style->ForeColor = System::Drawing::Color::FromArgb(0, 140, 70);
			}

			lblStatus->Text = L"";
		}
	};
}
