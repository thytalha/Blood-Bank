# 🩸 LifeBlood MS — Blood Bank Management System (C++/WinForms + SQL Server)

A **Windows Forms-based Blood Bank Management System** developed using **C++ (C++/CLI)** and **OOP concepts**, featuring a clean, user-friendly GUI for **user authentication**, **donor management**, **blood inventory**, **donations**, and **blood requests**.

> Semester project built to apply **object-oriented programming** principles in a real-world scenario.

---

## ✨ Highlights (What you can do)

- 🔐 **Role-based Login** (segmented UI switcher):
  - **Admin**
  - **Donor**
  - **Recipient**
- 🧑‍⚕️ **Admin Dashboard** *(manage the system)*
  - Monitor / manage inventory and requests (based on system design)
- 🩸 **Donor Dashboard**
  - **Log a New Donation**
  - View **My Donation History**
- 🙋 **Recipient Flow**
  - Create/manage **Blood Requests** (via DB + role architecture)
- 🗄️ **SQL Server Database** included (`BloodBank_DB.sql`)
  - Users, Inventory, Donations, Requests + seed data

---

## 🧠 Roles & User Journey (Interactive)

<details>
  <summary><b>👑 Admin</b> — manages inventory & system operations</summary>

- Login as **Admin**
- Access admin dashboard
- Monitor blood stock, requests, and system users *(depending on implemented UI panels)*

</details>

<details>
  <summary><b>🩸 Donor</b> — donate blood & track history</summary>

- Login as **Donor**
- Open **Donor Dashboard**
- Use **“Log a New Donation”** panel
- View **“My History”** in a grid view

</details>

<details>
  <summary><b>🧑‍ Recipient</b> — request blood</summary>

- Login as **Recipient**
- Create/manage blood requests (stored in DB table `BloodRequests`)
- Track request status *(depending on UI implementation)*

</details>

---

## 🗃️ Database Schema (SQL Server)

This project uses a SQL Server database named:

- **`db_bloodbank`**

Core tables (created in `BloodBank_DB.sql`):

- `Users` — stores login + profile + role  
- `BloodGroups` — lookup for 8 universal blood types  
- `BloodInventory` — stock count per blood group  
- `Donations` — donor donation logs  
- `BloodRequests` — recipient requests  

<details>
  <summary><b>📌 Seeded Accounts (from script)</b> — click to view</summary>

The SQL script seeds multiple users. Example seeded admins include:

- `talha / fast2026` (Role: Admin)

> You can view/edit these in `BloodBank_DB.sql` (recommended: change credentials before sharing publicly).

</details>

---

## 🧰 Tech Stack

- **Language:** C++ (C++/CLI)
- **UI:** Windows Forms (.NET)
- **Database:** Microsoft SQL Server (T-SQL)
- **Data Access:** ADO.NET (`SqlCommand`, `ExecuteScalar`, etc.)

---

## 🚀 Setup & Run (Step-by-step)

### 1) Clone the repository
```bash
git clone https://github.com/thytalha/Blood-Bank.git
cd Blood-Bank
```

### 2) Create the Database
1. Open **SQL Server Management Studio (SSMS)**
2. Open and run:
   - `BloodBank_DB.sql`
3. Confirm tables exist:
   - `Users`, `Donations`, `BloodRequests`, `BloodInventory`

> The script creates `db_bloodbank` if it doesn’t exist.

### 3) Configure Connection String (if needed)
The app uses a `Database.h` helper. If your SQL Server instance name differs, update the connection settings in the database layer accordingly.

### 4) Build & Run
- Open the project in **Visual Studio** (with **C++/CLI** support)
- Build the solution
- Run the app → the **Login Form** appears first

---

## 🖥️ UI Notes (From the Code)

- The login screen uses a **role enum** (`Admin`, `Donor`, `Recipient`) that drives dynamic UI state.
- Donor UI includes panels like:
  - **Overview**
  - **Donate Blood**
  - **My History**
- The UI follows a consistent “clean-light + crimson accent” aesthetic.

---


## 🔎 Project Structure (Typical)

```text
Blood-Bank/
│── BloodBank_DB.sql
│── Database.h
│── LoginForm.h
│── DonorDashboard.h
│── AdminDashboard.h
│── RecipientDashboard.h
└── README.md
```

---


## 🛡️ Security Note
This repository includes seeded usernames/passwords inside the SQL script for demo/testing.

If you plan to publish or deploy:
- change seeded credentials
- avoid storing plaintext passwords (use hashing)

---

## 👤 Author
**Talha** — [thytalha](https://github.com/thytalha)

If you found this project helpful, consider starring ⭐ the repo.