<div align="center">

# 🛒 Grocery Store Bill Management System
### C++ · GUI · OOP · File Handling

A desktop-based Grocery Store Billing application built in **C++ with a graphical user interface**, designed to streamline billing operations, manage product inventory, and generate customer receipts efficiently.

---

</div>

## 🧩 Overview

This **Grocery Store Bill Management System** provides a complete point-of-sale experience through a clean GUI. It allows store operators to manage products, process customer purchases, apply discounts, and generate itemized bills — all within a user-friendly desktop interface built in C++.

---

## ✨ Features

### 🧾 Billing
- Add multiple items to a customer's cart
- Auto-calculate subtotal, tax, and grand total
- Apply discount codes or percentage-based discounts
- Generate and print itemized customer receipts
- Unique bill/invoice ID for every transaction

### 📦 Product Management
- Add, edit, and delete grocery items
- Set item name, category, price, and quantity
- Real-time stock availability check during billing

### 👤 Customer Management
- Enter customer name and contact details
- Maintain purchase history per customer
- View previous bills and transactions

### 🗄️ Data Persistence
- All products, bills, and customer records saved to file
- Data loads automatically on application startup
- No data loss between sessions

---

## 🧠 Core Concepts

| Area | Details |
|---|---|
| **OOP** | Classes & Objects, Inheritance, Encapsulation, Abstraction |
| **GUI** | Desktop interface with forms, buttons, and input fields |
| **File Handling** | Read/write product, customer, and billing records |
| **Data Structures** | Arrays / Linked Lists for cart and inventory management |

---

## 🏗️ System Architecture

```
GroceryBillingSystem
│
├── Product Module     → Add, Edit, Delete, Search items
├── Billing Module     → Cart management, discount, invoice generation
├── Customer Module    → Customer records, purchase history
└── Storage Layer      → File-based persistence (load on start, save on exit)
```

---

## 🖥️ GUI Highlights

- Clean desktop interface with intuitive navigation
- Form-based input for products and customer details
- Real-time bill preview as items are added
- Receipt generation with formatted output
- Responsive layout designed for store operators

---

## ⚙️ Getting Started

### Requirements
- Visual Studio / Qt Creator / Code::Blocks
- C++ Compiler (GCC / MSVC)
- GUI framework libraries configured (Qt / WinForms)

### Steps
```bash
1. Clone the repository
2. Open the project in your IDE
3. Configure GUI framework/library paths
4. Build and compile the project
5. Run the executable
```

---

## 📁 Project Structure

```
grocery-billing-system/
│
├── src/
│   ├── main.cpp
│   ├── Product.cpp / Product.h
│   ├── Bill.cpp / Bill.h
│   ├── Customer.cpp / Customer.h
│   └── FileHandler.cpp / FileHandler.h
│
├── data/
│   ├── products.txt
│   ├── customers.txt
│   └── bills.txt
│
└── README.md
```

---

## 🚀 Roadmap

- [ ] Barcode scanner integration
- [ ] Database backend (SQL Server / MySQL)
- [ ] Sales analytics and reporting dashboard
- [ ] Multi-cashier / user login support
- [ ] Export bills as PDF

---

## 👨‍💻 Developer

**Muhammad Saad Akhtar**
BS Software Engineering — Superior University Lahore
*Passionate about C++, OOP, and Desktop Application Development*

---

<div align="center">

📜 *This project is for educational purposes. Free to use and modify.*

</div>