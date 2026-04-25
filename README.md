# LifeBlood MS – Blood Bank Management System
### Team 04 | FAST-NUCES | CS1004 OOP Project

---

## 📁 Complete Project Structure

```
LifeBloodMMS/
│
├── LifeBloodMMS.sln                  ← Open this in Visual Studio
├── LifeBloodMMS.vcxproj              ← Project file (all sources listed)
│
├── homeForm.h                        ← Landing / splash page (animated)
├── homeForm.cpp                      ← Entry point  +  GoToLogin impl
├── loginForm.h                       ← Login UI + role-based routing
├── loginForm.cpp                     ← Stub TU (avoids linker issues)
│
├── Models/
│   ├── User.h                        ← Abstract base class (OOP)
│   ├── Admin.h                       ← Derived: Admin role
│   ├── Donor.h                       ← Derived: Donor role
│   └── Recipient.h                   ← Derived: Recipient role
│
├── Database/
│   └── DatabaseHelper.h              ← All ADO.NET SQL operations
│
├── Utils/
│   └── UIHelpers.h                   ← Shared drawing / styling helpers
│
└── Forms/
    ├── SignUpForm.h                   ← New user registration
    ├── AdminDashboard.h              ← Admin home + expiry timer
    ├── ManageDonorsForm.h            ← View / edit / delete donors
    ├── ManageInventoryForm.h         ← Blood inventory + unit details
    ├── RequestsListForm.h            ← Admin approve/reject requests
    ├── DonorDashboard.h              ← Donor home + inline profile edit
    ├── DonationForm.h                ← Record a new donation
    ├── RequestForm.h                 ← Submit a blood request
    ├── RecipientDashboard.h          ← Recipient home + profile edit
    └── BloodSearchForm.h             ← Search blood availability
```

---

## ⚙️ Prerequisites

| Tool | Version |
|------|---------|
| Visual Studio | 2022 (v17+) |
| Workload | **Desktop development with C++** |
| Individual component | **.NET Framework 4.7.2 targeting pack** |
| SQL Server | SQL Server Express 2019 / 2022 |
| SSMS | Any recent version |

---

## 🗄️ Step 1 – Set Up the Database

1. Open **SQL Server Management Studio (SSMS)**.
2. Open the file **`SQLQuery1.sql`** (provided separately).
3. Run the entire script (`F5`). It will:
   - Create database `bloobbank_db`
   - Create all 5 tables (`Users`, `BloodInventory`, `Donations`, `BloodUnits`, `BloodRequests`)
   - Insert sample data (1 Admin, 2 Donors, 1 Recipient, sample donations & requests)

### Verify your SQL Server instance name

The connection string in `DatabaseHelper.h` uses:
```
Data Source=.\SQLEXPRESS
```

If your instance has a different name (e.g., `DESKTOP-XYZ\SQLEXPRESS`), update **both** lines at the top of `Database/DatabaseHelper.h`:

```cpp
static String^ ConnStr =
    L"Data Source=.\\SQLEXPRESS;Initial Catalog=bloobbank_db;"
    L"Integrated Security=True;Encrypt=False;TrustServerCertificate=True";

static String^ ConnStrNamed =
    L"Data Source=YOUR_PC_NAME\\SQLEXPRESS;Initial Catalog=bloobbank_db;"
    L"Integrated Security=True;Encrypt=False;TrustServerCertificate=True";
```

---

## 🖥️ Step 2 – Open in Visual Studio 2022

1. Open **`LifeBloodMMS.sln`** in Visual Studio 2022.
2. In Solution Explorer, verify all files appear under their folders.
3. If any `.h` files are missing from the project, right-click the project → **Add → Existing Item** and select the missing file.

---

## 🔧 Step 3 – Configure Project Properties

Right-click the project → **Properties**:

### General
| Setting | Value |
|---------|-------|
| Configuration Type | Application (.exe) |
| Common Language Runtime Support | **Common Language Runtime Support (/clr)** |
| .NET Target Framework Version | **v4.7.2** |
| Character Set | **Unicode** |
| Platform Toolset | v143 (VS 2022) |

### C/C++ → General
- Additional `#using` Directories:  
  `$(SystemRoot)\Microsoft.NET\Framework\v4.0.30319`

### Linker → System
- SubSystem: **Windows (/SUBSYSTEM:WINDOWS)**

---

## ▶️ Step 4 – Build & Run

1. Set configuration to **Debug | x86** (top toolbar).
2. Press **F5** or click **Local Windows Debugger**.
3. The animated **homeForm** splash screen launches.
4. Click **"Go to Dashboard"** or **"Login"** in the nav bar.

---

## 👤 Default Login Credentials

| Role | Username | Password |
|------|----------|----------|
| **Admin** | `talha_admin` | `dummy_hash_123` |
| **Donor** | `adnan_donor` | `dummy_hash_456` |
| **Donor** | `nabeera_donor` | `dummy_hash_789` |
| **Recipient** | `maryam_rec` | `dummy_hash_abc` |

> ⚠️ Passwords are stored as plain text in the sample data for demonstration.  
> In a production system, replace with proper bcrypt/SHA-256 hashing.

---

## 🎯 OOP Concepts Implemented

### Inheritance
```
User (abstract)
├── Admin     → AdminDashboard
├── Donor     → DonorDashboard
└── Recipient → RecipientDashboard
```

### Encapsulation
- All User fields are **private** in `User.h`
- Accessed through **properties** (getters) and setter methods

### Polymorphism
- `display_menu()` is **pure virtual** in `User`
- Overridden in `Admin`, `Donor`, `Recipient`
- Called after login to open the correct dashboard

### Role-Based Access Control (RBAC)
| Feature | Admin | Donor | Recipient |
|---------|-------|-------|-----------|
| Manage donors | ✅ | ❌ | ❌ |
| Approve requests | ✅ | ❌ | ❌ |
| View all inventory | ✅ | ✅ | ✅ |
| Record donation | ❌ | ✅ | ❌ |
| Submit request | ❌ | ✅ | ✅ |
| View own history | ✅ | ✅ | ✅ |

---

## 🩸 Feature Summary

1. **Animated Home Page** – Orbiting blood drop animation with gradient panel
2. **Login** – Validates from DB, routes to correct dashboard by role
3. **Sign Up** – Donor or Recipient registration with validation
4. **Admin Dashboard**
   - Stat cards: Total Donors, Blood Units, Pending Requests
   - Auto expiry check on load (marks expired units, deducts inventory)
   - Quick-action shortcuts
5. **Manage Donors** – Search, edit, delete with confirmation
6. **Inventory Management** – Live summary + individual unit details with add/remove
7. **Request Management** – Approve (deducts inventory, marks units Issued) or Reject
8. **Donor Dashboard** – Donate blood, submit request, view history, edit profile
9. **Recipient Dashboard** – Search blood, submit request, track own requests
10. **Blood Search** – Filter by blood group, colour-coded availability (red = 0 units)
11. **Expiry Tracking** – Auto-detects expired units on every Admin login, alerts Admin

---

## 🎨 UI Design

| Element | Specification |
|---------|--------------|
| Primary colour | `Color::FromArgb(232, 15, 59)` (Crimson) |
| Dark accent | `Color::FromArgb(161, 30, 50)` |
| Background gradient | Crimson red → Dark burgundy |
| Card style | Glassmorphism (transparent + white overlay + rounded corners) |
| Main font | Segoe UI |
| Brand font | Calisto MT |
| Icons | Segoe MDL2 Assets / Unicode emoji |
| Button style | Flat, rounded, hover highlight |

---

## 🐛 Troubleshooting

| Problem | Fix |
|---------|-----|
| `Cannot open database` | Check SQL Server is running; update instance name in `DatabaseHelper.h` |
| `CLR not enabled` error | Project Properties → C/C++ → Common Language Runtime Support → set to `/clr` |
| `System.Data not found` | Add Reference → System.Data in project references |
| Blank form / black window | Ensure `EnableVisualStyles()` is called before `Application::Run()` |
| `display_menu not implemented` | Rebuild solution (Ctrl+Shift+B) to pick up `homeForm.cpp` stubs |
| Build error on `SqlParameter` | Add `#using <System.Data.dll>` at top of the file using it |

---

## 📝 Notes for Submission

- All database operations use **transactions** where data consistency matters (donations, approvals, expiry processing).
- The project uses **no file I/O** — all persistence is through SQL Server via ADO.NET.
- The `DatabaseHelper` class is a static utility class following the **Repository pattern**.
- Expiry check runs automatically 2 seconds after Admin dashboard loads via a `Timer`.

---

*LifeBlood MS | Talha Pasha · Maryam Shafiq · Adnan Ali · Nabeera Saqib*
