#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cwctype>
using namespace std;

// Constants
const int MAX_ITEMS = 50;
const int IDC_MAIN_LIST = 1001;
const int IDC_PRODUCT_LIST = 1002;
const int IDC_CART_LIST = 1003;
const int IDC_INFO_EDIT = 1004;
const int IDC_ADD_BTN = 2001;
const int IDC_EDIT_BTN = 2002;
const int IDC_DELETE_BTN = 2003;
const int IDC_REPORT_BTN = 2004;
const int IDC_OUTOFSTOCK_BTN = 2005;
const int IDC_EXPORT_BTN = 2006;
const int IDC_FINANCE_BTN = 2007;
const int IDC_SELLER_BTN = 2008;
const int IDC_CLOSE_BTN = 2010;
const int IDC_BUY_BTN = 2011;
const int IDC_CHECKOUT_BTN = 2012;
const int IDC_CUSTOMER_EDIT = 2013;
const int IDC_QUANTITY_EDIT = 2014;
const int IDC_PASSWORD_EDIT = 2015;
const int IDC_EDIT_NAME = 2016;
const int IDC_EDIT_QTY = 2017;
const int IDC_EDIT_PRICE = 2018;
const int IDC_EDIT_UNIT = 2019;
const int IDC_SELLER_NAME = 2020;
const int IDC_SELLER_QTY = 2021;
const int IDC_SELLER_PRICE = 2022;
const int IDC_SELLER_UNIT = 2023;
const int IDC_SEARCH_BTN = 2024;
const int IDC_SEARCH_EDIT = 2025;
const int IDC_SEARCH_RESULT_EDIT = 2026;
const int IDC_PREVIEW_BTN = 2027;  // New preview button
const int IDC_WELCOME_BOX = 2028;  // Welcome box ID

// Global Variables
HINSTANCE hInst;
HWND hMainWnd, hInventoryDlg, hCustomerDlg, hSearchDlg;
COLORREF bgColor = RGB(245, 245, 255); // Light blue background
HFONT hTitleFont, hHeaderFont, hNormalFont, hFixedFont, hWelcomeFont;
bool passwordVerified = false;
HBRUSH hButtonBrush = NULL;  // Brush for button background
HBRUSH hEditBrush = NULL;    // Brush for edit backgrounds
HBRUSH hWelcomeBrush = NULL; // Brush for welcome box

// Product structure
struct Product {
    string name;
    int quantity;
    float price;
    string unit;
};

// Finance structure
struct Finance {
    double totalFinance;
    double financeLimit;

    Finance(double total, double limit) : totalFinance(total), financeLimit(limit) {}

    double getTotalFinance() const { return totalFinance; }
    double getFinanceLimit() const { return financeLimit; }

    void showFinanceStatus(HWND hwnd) {
        stringstream ss;
        ss << "------------- Finance Status -------------\r\n\r\n"
           << left << setw(20) << "Total Finance:" << "Rs. " << fixed << setprecision(2) << totalFinance << "\r\n"
           << left << setw(20) << "Finance Limit:" << "Rs. " << fixed << setprecision(2) << financeLimit << "\r\n"
           << left << setw(20) << "Available Finance:" << "Rs. " << fixed << setprecision(2) << (totalFinance - financeLimit) << "\r\n\r\n"
           << "------------------------------------------";
        SetWindowText(GetDlgItem(hwnd, IDC_INFO_EDIT), ss.str().c_str());
    }

    bool canPurchase(double amount) {
        return (totalFinance - amount) >= financeLimit;
    }

    void deductFinance(double amount) {
        totalFinance -= amount;
    }

    void addFinance(double amount) {
        totalFinance += amount;
    }
};

// Global Data
vector<Product> products;
Finance storeFinance(100000, 20000);
vector<pair<Product, int>> cart; // Product and quantity
double totalBill = 0.0;
int selectedInventoryIndex = -1;

// Helper function to trim whitespace
string trim(string str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

// Function Prototypes
void InitializeApp();
void CreateMainWindow();
void ShowInventoryDialog();
void ShowCustomerDialog();
void ShowSearchDialog();
void UpdateInventoryList(HWND hList);
void UpdateProductList(HWND hList);
void UpdateCartList(HWND hList);
void AddProduct();
void EditProduct();
void DeleteProduct();
void GenerateReport(HWND hwnd);
void ListOutOfStock(HWND hwnd);
void ExportInventory();
void ShowSellerPurchaseDialog();
void AddToCart();
void Checkout();
void PreviewBill();
void ShowMessage(HWND hwnd, const char* message, const char* title = "Info");
HWND CreateList(HWND hParent, int x, int y, int width, int height, int id);
HWND CreateButton(HWND hParent, int x, int y, int width, int height, int id, const char* text);
HWND CreateEdit(HWND hParent, int x, int y, int width, int height, int id, DWORD extraStyle = 0);
HWND CreateLabel(HWND hParent, int x, int y, int width, int height, const char* text);
string toLowerCase(string str);
void ShowEditItemDialog();
void ShowSellerDialog();
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InventoryDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CustomerDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PasswordDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SellerDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditItemDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK SearchDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

// WinMain - Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // Initialize common controls
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icc);

    InitializeApp();
    CreateMainWindow();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up brushes
    if (hButtonBrush) DeleteObject(hButtonBrush);
    if (hEditBrush) DeleteObject(hEditBrush);
    if (hWelcomeBrush) DeleteObject(hWelcomeBrush);

    return (int)msg.wParam;
}

// Initialize application data
void InitializeApp() {
    // Initialize with default products
    products = {
        {"Apple", 50, 120, "Kg"},
        {"Banana", 30, 40, "Dozen"},
        {"Milk", 20, 80, "Ltr"},
        {"Bread", 25, 45, "Kg"},
        {"Eggs", 60, 120, "Dozen"},
        {"Rice", 40, 90, "Kg"},
        {"Sugar", 35, 55, "Kg"},
        {"Salt", 45, 20, "Kg"},
        {"Coffee", 15, 250, "Kg"},
        {"Tea", 20, 150, "Kg"}
    };

    // Create fonts
    hTitleFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

    hHeaderFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

    hNormalFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

    hFixedFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY, FIXED_PITCH | FF_DONTCARE, "Courier New");

    hWelcomeFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

    // Create brushes for UI elements
    hButtonBrush = CreateSolidBrush(RGB(70, 130, 180)); // Steel blue for buttons
    hEditBrush = CreateSolidBrush(RGB(255, 255, 255));   // White for edit backgrounds
    hWelcomeBrush = CreateSolidBrush(RGB(220, 240, 255)); // Light blue for welcome box
}

// Create a list view control
HWND CreateList(HWND hParent, int x, int y, int width, int height, int id) {
    HWND hList = CreateWindow(WC_LISTVIEW, "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_SHOWSELALWAYS,
        x, y, width, height, hParent, (HMENU)(INT_PTR)id, hInst, NULL);

    ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    SendMessage(hList, WM_SETFONT, (WPARAM)hNormalFont, TRUE);

    // Set list colors
    ListView_SetBkColor(hList, bgColor);
    ListView_SetTextBkColor(hList, bgColor);
    ListView_SetTextColor(hList, RGB(25, 25, 112)); // Dark blue text

    return hList;
}

// Create a button control
HWND CreateButton(HWND hParent, int x, int y, int width, int height, int id, const char* text) {
    HWND hBtn = CreateWindow("BUTTON", text,
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_CENTER,
        x, y, width, height, hParent, (HMENU)(INT_PTR)id, hInst, NULL);

    SendMessage(hBtn, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
    return hBtn;
}

// Create an edit control
HWND CreateEdit(HWND hParent, int x, int y, int width, int height, int id, DWORD extraStyle) {
    HWND hEdit = CreateWindow("EDIT", "",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | extraStyle,
        x, y, width, height, hParent, (HMENU)(INT_PTR)id, hInst, NULL);

    SendMessage(hEdit, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
    return hEdit;
}

// Create a label control
HWND CreateLabel(HWND hParent, int x, int y, int width, int height, const char* text) {
    HWND hLabel = CreateWindow("STATIC", text,
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        x, y, width, height, hParent, NULL, hInst, NULL);

    SendMessage(hLabel, WM_SETFONT, (WPARAM)hNormalFont, TRUE);
    return hLabel;
}

// Create a welcome box
HWND CreateWelcomeBox(HWND hParent, int x, int y, int width, int height, int id) {
    HWND hBox = CreateWindow("STATIC", "",
        WS_VISIBLE | WS_CHILD | SS_CENTER | SS_SUNKEN,
        x, y, width, height, hParent, (HMENU)(INT_PTR)id, hInst, NULL);

    // Set welcome text
    string welcomeText =
        "========== WELCOME TO ZAIN KA KHOKHA ==========\r\n"
        "Store:     Zain Ka KhoKha\r\n"
        "Manager:   Zain Badmash\r\n"
        "Location:  Ali Town Branch\r\n"
        "===============================================";

    SetWindowText(hBox, welcomeText.c_str());
    SendMessage(hBox, WM_SETFONT, (WPARAM)hWelcomeFont, TRUE);
    return hBox;
}

// Password dialog procedure
LRESULT CALLBACK PasswordDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            CreateLabel(hDlg, 20, 20, 200, 30, "Enter Admin Password:");
            CreateEdit(hDlg, 20, 50, 200, 30, IDC_PASSWORD_EDIT, ES_PASSWORD);
            CreateButton(hDlg, 20, 100, 100, 30, IDOK, "OK");
            CreateButton(hDlg, 130, 100, 100, 30, IDCANCEL, "Cancel");
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    char password[50];
                    GetWindowText(GetDlgItem(hDlg, IDC_PASSWORD_EDIT), password, 50);

                    if (strcmp(password, "123") == 0) {
                        passwordVerified = true;
                        DestroyWindow(hDlg);
                    } else {
                        ShowMessage(hDlg, "Incorrect password!", "Error");
                    }
                    break;
                }
                case IDCANCEL:
                    DestroyWindow(hDlg);
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)hButtonBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkColor(hdc, RGB(255, 255, 255)); // White background
            return (LRESULT)hEditBrush;
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hDlg, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_CLOSE:
            DestroyWindow(hDlg);
            break;
    }
    return DefWindowProc(hDlg, msg, wParam, lParam);
}

// Seller dialog procedure
LRESULT CALLBACK SellerDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hName, hQty, hPrice, hUnit;

    switch (msg) {
        case WM_CREATE: {
            CreateLabel(hDlg, 20, 20, 200, 30, "Product Name:");
            hName = CreateEdit(hDlg, 20, 50, 200, 30, IDC_SELLER_NAME, 0);

            CreateLabel(hDlg, 20, 90, 200, 30, "Quantity:");
            hQty = CreateEdit(hDlg, 20, 120, 200, 30, IDC_SELLER_QTY, ES_NUMBER);

            CreateLabel(hDlg, 20, 160, 200, 30, "Price per Unit:");
            hPrice = CreateEdit(hDlg, 20, 190, 200, 30, IDC_SELLER_PRICE, 0);

            CreateLabel(hDlg, 20, 230, 200, 30, "Unit:");
            hUnit = CreateEdit(hDlg, 20, 260, 200, 30, IDC_SELLER_UNIT, 0);

            CreateButton(hDlg, 20, 310, 100, 30, IDOK, "Add");
            CreateButton(hDlg, 130, 310, 100, 30, IDCANCEL, "Cancel");
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    char name[100], qty[20], price[20], unit[20];
                    GetWindowText(hName, name, 100);
                    GetWindowText(hQty, qty, 20);
                    GetWindowText(hPrice, price, 20);
                    GetWindowText(hUnit, unit, 20);

                    if (strlen(name) == 0 || strlen(qty) == 0 || strlen(price) == 0 || strlen(unit) == 0) {
                        ShowMessage(hDlg, "Please fill all fields", "Error");
                        return 0;
                    }

                    int quantity = atoi(qty);
                    float unitPrice = atof(price);

                    if (quantity <= 0 || unitPrice <= 0) {
                        ShowMessage(hDlg, "Quantity and price must be positive numbers", "Error");
                        return 0;
                    }

                    float totalCost = quantity * unitPrice;

                    if (!storeFinance.canPurchase(totalCost)) {
                        ShowMessage(hDlg, "Insufficient finance balance to make this purchase!", "Error");
                        return 0;
                    }

                    // Deduct finance
                    storeFinance.deductFinance(totalCost);

                    // Check if item exists
                    string nameStr = name;
                    string nameLower = toLowerCase(nameStr);
                    bool found = false;

                    for (auto& product : products) {
                        if (toLowerCase(product.name) == nameLower) {
                            product.quantity += quantity;
                            product.price = unitPrice;
                            product.unit = unit;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        if (products.size() < MAX_ITEMS) {
                            products.push_back({name, quantity, unitPrice, unit});
                        } else {
                            ShowMessage(hDlg, "Inventory is full! Cannot add new item.", "Error");
                            return 0;
                        }
                    }

                    ShowMessage(hInventoryDlg, "Purchase successful! New item added to inventory.", "Success");
                    DestroyWindow(hDlg);
                    break;
                }
                case IDCANCEL:
                    DestroyWindow(hDlg);
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)hButtonBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkColor(hdc, RGB(255, 255, 255)); // White background
            return (LRESULT)hEditBrush;
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hDlg, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_CLOSE:
            DestroyWindow(hDlg);
            break;
    }
    return DefWindowProc(hDlg, msg, wParam, lParam);
}

// Edit item dialog procedure
LRESULT CALLBACK EditItemDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hName, hQty, hPrice, hUnit;

    switch (msg) {
        case WM_CREATE: {
            CreateLabel(hDlg, 20, 20, 200, 30, "Product Name:");
            hName = CreateEdit(hDlg, 20, 50, 200, 30, IDC_EDIT_NAME, 0);
            SetWindowText(hName, products[selectedInventoryIndex].name.c_str());

            CreateLabel(hDlg, 20, 90, 200, 30, "Quantity:");
            hQty = CreateEdit(hDlg, 20, 120, 200, 30, IDC_EDIT_QTY, ES_NUMBER);
            char qty[20];
            sprintf(qty, "%d", products[selectedInventoryIndex].quantity);
            SetWindowText(hQty, qty);

            CreateLabel(hDlg, 20, 160, 200, 30, "Price:");
            hPrice = CreateEdit(hDlg, 20, 190, 200, 30, IDC_EDIT_PRICE, 0);
            char price[20];
            sprintf(price, "%.2f", products[selectedInventoryIndex].price);
            SetWindowText(hPrice, price);

            CreateLabel(hDlg, 20, 230, 200, 30, "Unit:");
            hUnit = CreateEdit(hDlg, 20, 260, 200, 30, IDC_EDIT_UNIT, 0);
            SetWindowText(hUnit, products[selectedInventoryIndex].unit.c_str());

            CreateButton(hDlg, 20, 310, 100, 30, IDOK, "Update");
            CreateButton(hDlg, 130, 310, 100, 30, IDCANCEL, "Cancel");
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    char name[100], qty[20], price[20], unit[20];
                    GetWindowText(hName, name, 100);
                    GetWindowText(hQty, qty, 20);
                    GetWindowText(hPrice, price, 20);
                    GetWindowText(hUnit, unit, 20);

                    if (strlen(name) == 0 || strlen(qty) == 0 || strlen(price) == 0 || strlen(unit) == 0) {
                        ShowMessage(hDlg, "Please fill all fields", "Error");
                        return 0;
                    }

                    int quantity = atoi(qty);
                    float unitPrice = atof(price);

                    if (quantity < 0 || unitPrice < 0) {
                        ShowMessage(hDlg, "Quantity and price must be non-negative numbers", "Error");
                        return 0;
                    }

                    products[selectedInventoryIndex].name = name;
                    products[selectedInventoryIndex].quantity = quantity;
                    products[selectedInventoryIndex].price = unitPrice;
                    products[selectedInventoryIndex].unit = unit;

                    ShowMessage(hInventoryDlg, "Item updated successfully!", "Success");
                    DestroyWindow(hDlg);
                    break;
                }
                case IDCANCEL:
                    DestroyWindow(hDlg);
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkMode(hdc, TRANSPARENT);
            return (LRESULT)hButtonBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkColor(hdc, RGB(255, 255, 255)); // White background
            return (LRESULT)hEditBrush;
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hDlg, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_CLOSE:
            DestroyWindow(hDlg);
            break;
    }
    return DefWindowProc(hDlg, msg, wParam, lParam);
}

// Customer search dialog procedure
LRESULT CALLBACK SearchDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hSearchEdit, hResultEdit;

    switch (msg) {
        case WM_CREATE: {
            CreateLabel(hDlg, 20, 20, 200, 30, "Customer Name:");
            hSearchEdit = CreateEdit(hDlg, 20, 50, 300, 30, IDC_SEARCH_EDIT, 0);
            CreateButton(hDlg, 330, 50, 100, 30, IDC_SEARCH_BTN, "Search");
            CreateButton(hDlg, 440, 50, 100, 30, IDC_CLOSE_BTN, "Close");

            hResultEdit = CreateEdit(hDlg, 20, 90, 520, 300, IDC_SEARCH_RESULT_EDIT,
                                    ES_MULTILINE | ES_READONLY | WS_VSCROLL | WS_HSCROLL);
            SendMessage(hResultEdit, WM_SETFONT, (WPARAM)hFixedFont, TRUE);
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_SEARCH_BTN: {
                    char searchName[100];
                    GetWindowText(hSearchEdit, searchName, 100);

                    if (strlen(searchName) == 0) {
                        SetWindowText(hResultEdit, "Please enter a customer name to search.");
                        break;
                    }

                    ifstream file("bills.txt");
                    if (!file) {
                        SetWindowText(hResultEdit, "No bills found or error opening file.");
                        break;
                    }

                    stringstream ss;
                    string line;
                    bool found = false;
                    bool readingBill = false;
                    string currentBill;
                    string searchNameLower = toLowerCase(string(searchName));

                    while (getline(file, line)) {
                        // Check for customer name line
                        if (line.find("Customer Name:") != string::npos) {
                            // Extract customer name from the line
                            string billName = line.substr(line.find(":") + 2); // Skip colon and space
                            string billNameLower = toLowerCase(trim(billName));

                            // If we're already reading a bill and found a new customer, finish previous bill
                            if (readingBill) {
                                if (billNameLower.find(searchNameLower) != string::npos) {
                                    ss << currentBill << "\r\n\r\n";
                                }
                                readingBill = false;
                                currentBill = "";
                            }

                            // If this customer matches our search
                            if (billNameLower.find(searchNameLower) != string::npos) {
                                found = true;
                                readingBill = true;
                                currentBill = line + "\r\n";
                            }
                        }
                        else if (readingBill) {
                            currentBill += line + "\r\n";

                            // Check for end of bill (empty line)
                            if (line.empty()) {
                                readingBill = false;
                                ss << currentBill << "\r\n";
                                currentBill = "";
                            }
                        }
                    }

                    // Handle last bill in file
                    if (readingBill) {
                        ss << currentBill << "\r\n";
                    }

                    if (!found) {
                        ss << "No bills found for customer: " << searchName;
                    }

                    SetWindowText(hResultEdit, ss.str().c_str());
                    break;
                }
                case IDC_CLOSE_BTN:
                    DestroyWindow(hDlg);
                    hSearchDlg = NULL;
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND hwndCtl = (HWND)lParam;
            int ctrlId = GetDlgCtrlID(hwndCtl);

            if (ctrlId == IDC_SEARCH_RESULT_EDIT) {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkColor(hdc, RGB(255, 255, 255)); // White background
                return (LRESULT)hEditBrush;
            } else {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)hButtonBrush;
            }
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkColor(hdc, RGB(255, 255, 255)); // White background
            return (LRESULT)hEditBrush;
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hDlg, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_CLOSE:
            DestroyWindow(hDlg);
            hSearchDlg = NULL;
            break;

        default:
            return DefWindowProc(hDlg, msg, wParam, lParam);
    }
    return 0;
}

// Main window procedure
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Create title
            HWND hTitle = CreateWindow("STATIC", "ZAIN KA KHOKHA",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                10, 20, 780, 40, hwnd, NULL, hInst, NULL);
            SendMessage(hTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

            // Create welcome box
            CreateWelcomeBox(hwnd, 50, 80, 700, 150, IDC_WELCOME_BOX);

            // Create buttons in a vertical layout
            CreateButton(hwnd, 300, 250, 200, 40, IDC_ADD_BTN, "1) Inventory Management");
            CreateButton(hwnd, 300, 300, 200, 40, IDC_EDIT_BTN, "2) Customer Purchase");
            CreateButton(hwnd, 300, 350, 200, 40, IDC_DELETE_BTN, "3) Exit");
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_ADD_BTN:
                    ShowInventoryDialog();
                    break;
                case IDC_EDIT_BTN:
                    ShowCustomerDialog();
                    break;
                case IDC_DELETE_BTN:
                    PostQuitMessage(0);
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND hwndCtl = (HWND)lParam;
            int ctrlId = GetDlgCtrlID(hwndCtl);

            if (ctrlId == IDC_WELCOME_BOX) {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkColor(hdc, RGB(220, 240, 255)); // Light blue background
                return (LRESULT)hWelcomeBrush;
            } else {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)CreateSolidBrush(bgColor);
            }
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// Inventory dialog procedure - MODIFIED BUTTON WIDTHS
LRESULT CALLBACK InventoryDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hList, hInfoEdit;

    switch (msg) {
        case WM_CREATE: {
            // Create controls with wider buttons
            CreateButton(hDlg, 10, 10, 120, 30, IDC_EDIT_BTN, "Edit Item");
            CreateButton(hDlg, 140, 10, 120, 30, IDC_DELETE_BTN, "Delete Item");
            CreateButton(hDlg, 270, 10, 120, 30, IDC_REPORT_BTN, "View Report");
            CreateButton(hDlg, 400, 10, 120, 30, IDC_OUTOFSTOCK_BTN, "Out of Stock");
            CreateButton(hDlg, 530, 10, 100, 30, IDC_EXPORT_BTN, "Export");
            CreateButton(hDlg, 640, 10, 100, 30, IDC_FINANCE_BTN, "Finance");
            CreateButton(hDlg, 750, 10, 140, 30, IDC_SELLER_BTN, "Seller Purchase");
            CreateButton(hDlg, 10, 50, 100, 30, IDC_SEARCH_BTN, "Search"); // Moved below
            CreateButton(hDlg, 810, 550, 80, 30, IDC_CLOSE_BTN, "Close");

            hList = CreateList(hDlg, 10, 90, 880, 350, IDC_MAIN_LIST); // Larger list, adjusted position
            hInfoEdit = CreateEdit(hDlg, 10, 450, 880, 90, IDC_INFO_EDIT, ES_MULTILINE | ES_READONLY | WS_VSCROLL); // Adjusted position
            SendMessage(hInfoEdit, WM_SETFONT, (WPARAM)hFixedFont, TRUE);

            // Set up list columns
            LVCOLUMN lvc;
            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

            lvc.iSubItem = 0;
            lvc.pszText = "Name";
            lvc.cx = 250; // Wider columns
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hList, 0, &lvc);

            lvc.iSubItem = 1;
            lvc.pszText = "Quantity";
            lvc.cx = 100;
            lvc.fmt = LVCFMT_RIGHT;
            ListView_InsertColumn(hList, 1, &lvc);

            lvc.iSubItem = 2;
            lvc.pszText = "Unit";
            lvc.cx = 100;
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hList, 2, &lvc);

            lvc.iSubItem = 3;
            lvc.pszText = "Price";
            lvc.cx = 100;
            lvc.fmt = LVCFMT_RIGHT;
            ListView_InsertColumn(hList, 3, &lvc);

            lvc.iSubItem = 4;
            lvc.pszText = "Status";
            lvc.cx = 150; // Wider status column
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hList, 4, &lvc);

            UpdateInventoryList(hList);
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_EDIT_BTN: {
                    int index = ListView_GetNextItem(hList, -1, LVNI_SELECTED);
                    if (index == -1) {
                        ShowMessage(hDlg, "Please select an item to edit.");
                        break;
                    }
                    selectedInventoryIndex = index;

                    // Create edit dialog
                    HWND hEditDlg = CreateWindowEx(0, "EditItemDialogClass", "Edit Item",
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
                        hDlg, NULL, hInst, NULL);

                    // Run modal loop for edit dialog
                    MSG msg;
                    while (IsWindow(hEditDlg) && GetMessage(&msg, NULL, 0, 0)) {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }

                    UpdateInventoryList(hList);  // Refresh AFTER dialog closes
                    break;
                }
                case IDC_DELETE_BTN:
                    DeleteProduct();
                    UpdateInventoryList(hList);
                    break;
                case IDC_REPORT_BTN:
                    GenerateReport(hDlg);
                    break;
                case IDC_OUTOFSTOCK_BTN:
                    ListOutOfStock(hDlg);
                    break;
                case IDC_EXPORT_BTN:
                    ExportInventory();
                    break;
                case IDC_FINANCE_BTN:
                    storeFinance.showFinanceStatus(hDlg);
                    break;
                case IDC_SELLER_BTN: {
                    // Create seller dialog
                    HWND hSellerDlg = CreateWindowEx(0, "SellerDialogClass", "Seller Purchase",
                        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
                        CW_USEDEFAULT, CW_USEDEFAULT, 300, 400,
                        hDlg, NULL, hInst, NULL);

                    // Run modal loop for seller dialog
                    MSG msg;
                    while (IsWindow(hSellerDlg) && GetMessage(&msg, NULL, 0, 0)) {
                        TranslateMessage(&msg);
                        DispatchMessage(&msg);
                    }

                    UpdateInventoryList(hList);  // Refresh AFTER dialog closes
                    break;
                }
                case IDC_SEARCH_BTN:
                    ShowSearchDialog();
                    break;
                case IDC_CLOSE_BTN:
                    DestroyWindow(hDlg);
                    hInventoryDlg = NULL;
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND hwndCtl = (HWND)lParam;
            int ctrlId = GetDlgCtrlID(hwndCtl);

            if (ctrlId == IDC_INFO_EDIT) {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkColor(hdc, RGB(255, 255, 255)); // White background
                return (LRESULT)hEditBrush;
            } else {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)hButtonBrush;
            }
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkColor(hdc, RGB(255, 255, 255)); // White background
            return (LRESULT)hEditBrush;
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hDlg, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_CLOSE:
            DestroyWindow(hDlg);
            hInventoryDlg = NULL;
            break;

        default:
            return DefWindowProc(hDlg, msg, wParam, lParam);
    }
    return 0;
}

// Customer dialog procedure
LRESULT CALLBACK CustomerDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hProductList, hCartList, hInfoEdit, hCustomerEdit, hQuantityEdit;

    switch (msg) {
        case WM_CREATE: {
            // Create controls
            CreateLabel(hDlg, 10, 15, 100, 20, "Customer Name:");
            hCustomerEdit = CreateEdit(hDlg, 120, 10, 300, 25, IDC_CUSTOMER_EDIT, 0); // Wider customer edit

            hProductList = CreateList(hDlg, 10, 50, 400, 300, IDC_PRODUCT_LIST); // Slightly wider
            hCartList = CreateList(hDlg, 420, 50, 400, 300, IDC_CART_LIST); // Slightly wider

            CreateLabel(hDlg, 10, 360, 100, 20, "Quantity:");
            hQuantityEdit = CreateEdit(hDlg, 120, 360, 100, 25, IDC_QUANTITY_EDIT, ES_NUMBER);

            // Wider buttons for better text fit
            CreateButton(hDlg, 240, 360, 120, 30, IDC_BUY_BTN, "Add to Cart");
            CreateButton(hDlg, 370, 360, 120, 30, IDC_PREVIEW_BTN, "Preview Bill");
            CreateButton(hDlg, 600, 360, 120, 30, IDC_CHECKOUT_BTN, "Checkout");
            CreateButton(hDlg, 760, 550, 80, 30, IDC_CLOSE_BTN, "Close");

            hInfoEdit = CreateEdit(hDlg, 10, 400, 830, 140, IDC_INFO_EDIT, ES_MULTILINE | ES_READONLY | WS_VSCROLL);
            SendMessage(hInfoEdit, WM_SETFONT, (WPARAM)hFixedFont, TRUE);  // Fixed font for proper alignment

            // Set up product list columns
            LVCOLUMN lvc;
            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

            lvc.iSubItem = 0;
            lvc.pszText = "Name";
            lvc.cx = 180; // Wider
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hProductList, 0, &lvc);

            lvc.iSubItem = 1;
            lvc.pszText = "Unit";
            lvc.cx = 80;
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hProductList, 1, &lvc);

            lvc.iSubItem = 2;
            lvc.pszText = "Price";
            lvc.cx = 80;
            lvc.fmt = LVCFMT_RIGHT;
            ListView_InsertColumn(hProductList, 2, &lvc);

            // Set up cart list columns
            lvc.iSubItem = 0;
            lvc.pszText = "Name";
            lvc.cx = 180; // Wider
            lvc.fmt = LVCFMT_LEFT;
            ListView_InsertColumn(hCartList, 0, &lvc);

            lvc.iSubItem = 1;
            lvc.pszText = "Qty";
            lvc.cx = 50;
            lvc.fmt = LVCFMT_RIGHT;
            ListView_InsertColumn(hCartList, 1, &lvc);

            lvc.iSubItem = 2;
            lvc.pszText = "Unit Price";
            lvc.cx = 90; // Wider
            lvc.fmt = LVCFMT_RIGHT;
            ListView_InsertColumn(hCartList, 2, &lvc);

            lvc.iSubItem = 3;
            lvc.pszText = "Total";
            lvc.cx = 90; // Wider
            lvc.fmt = LVCFMT_RIGHT;
            ListView_InsertColumn(hCartList, 3, &lvc);

            UpdateProductList(hProductList);
            break;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDC_BUY_BTN:
                    AddToCart();
                    UpdateProductList(hProductList);
                    UpdateCartList(hCartList);
                    break;
                case IDC_PREVIEW_BTN:  // New button handler
                    PreviewBill();
                    break;
                case IDC_CHECKOUT_BTN:
                    Checkout();
                    UpdateProductList(hProductList);
                    UpdateCartList(hCartList);
                    break;
                case IDC_CLOSE_BTN:
                    DestroyWindow(hDlg);
                    hCustomerDlg = NULL;
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND hwndCtl = (HWND)lParam;
            int ctrlId = GetDlgCtrlID(hwndCtl);

            if (ctrlId == IDC_INFO_EDIT) {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkColor(hdc, RGB(255, 255, 255)); // White background
                return (LRESULT)hEditBrush;
            } else {
                SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
                SetBkMode(hdc, TRANSPARENT);
                return (LRESULT)hButtonBrush;
            }
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(25, 25, 112)); // Dark blue text
            SetBkColor(hdc, RGB(255, 255, 255)); // White background
            return (LRESULT)hEditBrush;
        }

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            SetBkColor(hdc, RGB(70, 130, 180)); // Steel blue background
            return (LRESULT)hButtonBrush;
        }

        case WM_ERASEBKGND: {
            RECT rect;
            GetClientRect(hDlg, &rect);
            HBRUSH hBrush = CreateSolidBrush(bgColor);
            FillRect((HDC)wParam, &rect, hBrush);
            DeleteObject(hBrush);
            return 1;
        }

        case WM_CLOSE:
            DestroyWindow(hDlg);
            hCustomerDlg = NULL;
            break;

        default:
            return DefWindowProc(hDlg, msg, wParam, lParam);
    }
    return 0;
}

// Create the main application window
void CreateMainWindow() {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(bgColor);
    wc.lpszClassName = "GroceryStoreClass";

    RegisterClassEx(&wc);

    wc.lpfnWndProc = InventoryDlgProc;
    wc.lpszClassName = "InventoryDialogClass";
    wc.hbrBackground = CreateSolidBrush(bgColor);
    RegisterClassEx(&wc);

    wc.lpfnWndProc = CustomerDlgProc;
    wc.lpszClassName = "CustomerDialogClass";
    wc.hbrBackground = CreateSolidBrush(bgColor);
    RegisterClassEx(&wc);

    wc.lpfnWndProc = PasswordDlgProc;
    wc.lpszClassName = "PasswordDialogClass";
    wc.hbrBackground = CreateSolidBrush(bgColor);
    RegisterClassEx(&wc);

    wc.lpfnWndProc = SellerDlgProc;
    wc.lpszClassName = "SellerDialogClass";
    wc.hbrBackground = CreateSolidBrush(bgColor);
    RegisterClassEx(&wc);

    wc.lpfnWndProc = EditItemDlgProc;
    wc.lpszClassName = "EditItemDialogClass";
    wc.hbrBackground = CreateSolidBrush(bgColor);
    RegisterClassEx(&wc);

    wc.lpfnWndProc = SearchDlgProc;
    wc.lpszClassName = "SearchDialogClass";
    wc.hbrBackground = CreateSolidBrush(bgColor);
    RegisterClassEx(&wc);

    hMainWnd = CreateWindowEx(0, "GroceryStoreClass", "Zain Ka Khokha - Grocery Store Management",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInst, NULL);

    ShowWindow(hMainWnd, SW_SHOW);
    UpdateWindow(hMainWnd);
}

// Show Inventory Management Dialog with password check
void ShowInventoryDialog() {
    if (hInventoryDlg) return;

    passwordVerified = false;

    // Create password window
    HWND hPwdWnd = CreateWindowEx(0, "PasswordDialogClass", "Password",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        hMainWnd, NULL, hInst, NULL);

    if (!hPwdWnd) {
        return;
    }

    ShowWindow(hPwdWnd, SW_SHOW);
    UpdateWindow(hPwdWnd);

    // Run modal loop for password window
    MSG msg;
    while (IsWindow(hPwdWnd) && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (!passwordVerified) {
        return;
    }

    // Create inventory window with increased width
    hInventoryDlg = CreateWindowEx(0, "InventoryDialogClass", "Inventory Management",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 600, // Wider window
        hMainWnd, NULL, hInst, NULL);
}

// Show Customer Purchase Dialog
void ShowCustomerDialog() {
    if (hCustomerDlg) return;

    hCustomerDlg = CreateWindowEx(0, "CustomerDialogClass", "Customer Purchase",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 850, 600,
        hMainWnd, NULL, hInst, NULL);
}

// Show Customer Search Dialog
void ShowSearchDialog() {
    if (hSearchDlg) return;

    hSearchDlg = CreateWindowEx(0, "SearchDialogClass", "Search Customer Bills",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 560, 450,
        hMainWnd, NULL, hInst, NULL);
}

// Update inventory list view
void UpdateInventoryList(HWND hList) {
    ListView_DeleteAllItems(hList);

    for (size_t i = 0; i < products.size(); i++) {
        LVITEM lvi = {0};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = (LPSTR)products[i].name.c_str();
        ListView_InsertItem(hList, &lvi);

        char buffer[50];
        sprintf(buffer, "%d", products[i].quantity);
        ListView_SetItemText(hList, i, 1, buffer);

        ListView_SetItemText(hList, i, 2, (LPSTR)products[i].unit.c_str());

        sprintf(buffer, "%.2f", products[i].price);
        ListView_SetItemText(hList, i, 3, buffer);

        ListView_SetItemText(hList, i, 4, (LPSTR)(products[i].quantity > 0 ? "In Stock" : "Out of Stock"));
    }
}

// Update product list for customer - FIXED INDEXING ISSUE
void UpdateProductList(HWND hList) {
    ListView_DeleteAllItems(hList);

    int listIndex = 0; // Track the actual position in the list view
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].quantity > 0) {
            LVITEM lvi = {0};
            lvi.mask = LVIF_TEXT;
            lvi.iItem = listIndex; // Use sequential index for list items
            lvi.iSubItem = 0;
            lvi.pszText = (LPSTR)products[i].name.c_str();
            ListView_InsertItem(hList, &lvi);

            ListView_SetItemText(hList, listIndex, 1, (LPSTR)products[i].unit.c_str());

            char buffer[50];
            sprintf(buffer, "%.2f", products[i].price);
            ListView_SetItemText(hList, listIndex, 2, buffer);

            listIndex++; // Only increment for displayed items
        }
    }
}

// Update cart list
void UpdateCartList(HWND hList) {
    ListView_DeleteAllItems(hList);

    totalBill = 0.0;
    for (size_t i = 0; i < cart.size(); i++) {
        LVITEM lvi = {0};
        lvi.mask = LVIF_TEXT;
        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = (LPSTR)cart[i].first.name.c_str();
        ListView_InsertItem(hList, &lvi);

        char buffer[50];
        sprintf(buffer, "%d", cart[i].second);
        ListView_SetItemText(hList, i, 1, buffer);

        sprintf(buffer, "%.2f", cart[i].first.price);
        ListView_SetItemText(hList, i, 2, buffer);

        double total = cart[i].first.price * cart[i].second;
        totalBill += total;
        sprintf(buffer, "%.2f", total);
        ListView_SetItemText(hList, i, 3, buffer);
    }

    // Update total bill display
    char billText[100];
    sprintf(billText, "Total Bill: Rs. %.2f", totalBill);
    SetWindowText(GetDlgItem(hCustomerDlg, IDC_INFO_EDIT), billText);
}

// Delete product from inventory
void DeleteProduct() {
    HWND hList = GetDlgItem(hInventoryDlg, IDC_MAIN_LIST);
    int index = ListView_GetNextItem(hList, -1, LVNI_SELECTED);

    if (index == -1) {
        ShowMessage(hInventoryDlg, "Please select an item to delete.");
        return;
    }

    products.erase(products.begin() + index);
    ShowMessage(hInventoryDlg, "Item deleted successfully!");
}

// Generate inventory report with detailed statistics
void GenerateReport(HWND hwnd) {
    stringstream report;
    report << "-------------- INVENTORY REPORT ---------------\r\n\r\n";
    report << left << setw(25) << "Item Name"
           << setw(10) << "Qty"
           << setw(10) << "Unit"
           << setw(10) << "Price"
           << setw(15) << "Status" << "\r\n";
    report << "------------------------------------------------\r\n";

    int outOfStockCount = 0;
    int totalQuantity = 0;
    float totalValue = 0;

    for (size_t i = 0; i < products.size(); i++) {
        string status = (products[i].quantity > 0) ? "In Stock" : "Out of Stock";
        if (products[i].quantity == 0) outOfStockCount++;

        report << left << setw(25) << products[i].name
               << setw(10) << products[i].quantity
               << setw(10) << products[i].unit
               << setw(10) << fixed << setprecision(2) << products[i].price
               << setw(15) << status << "\r\n";

        totalQuantity += products[i].quantity;
        totalValue += products[i].price * products[i].quantity;
    }

    report << "------------------------------------------------\r\n\r\n";
    report << left << setw(25) << "Total Unique Items:" << products.size() << "\r\n";
    report << left << setw(25) << "Total Quantity:" << totalQuantity << "\r\n";
    report << left << setw(25) << "Out-of-Stock Items:" << outOfStockCount << "\r\n";
    report << left << setw(25) << "Inventory Value:" << "Rs. " << fixed << setprecision(2) << totalValue << "\r\n";

    SetWindowText(GetDlgItem(hwnd, IDC_INFO_EDIT), report.str().c_str());
}

// List out of stock items
void ListOutOfStock(HWND hwnd) {
    stringstream report;
    report << "----------- OUT OF STOCK ITEMS -----------\r\n\r\n";

    bool found = false;
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].quantity == 0) {
            report << "- " << products[i].name << "\r\n";
            found = true;
        }
    }

    if (!found) {
        report << "No items are currently out of stock.\r\n";
    }

    report << "\r\n------------------------------------------";
    SetWindowText(GetDlgItem(hwnd, IDC_INFO_EDIT), report.str().c_str());
}

// Export inventory to file with timestamp
void ExportInventory() {
    ofstream file("inventory_report.txt");
    if (!file) {
        ShowMessage(hInventoryDlg, "Error creating file!");
        return;
    }

    // Get current date and time
    time_t now = time(0);
    char dt[20];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));

    file << "Date: " << dt << endl;
    file << "=============== INVENTORY REPORT ===============" << endl;
    file << left << setw(20) << "Item Name" << setw(10) << "Qty" << setw(10) << "Unit"
         << setw(10) << "Price" << setw(15) << "Status" << endl;
    file << "------------------------------------------------" << endl;

    int outOfStockCount = 0;
    int totalQuantity = 0;
    float totalValue = 0;

    for (size_t i = 0; i < products.size(); i++) {
        string status = (products[i].quantity > 0) ? "In Stock" : "Out of Stock";
        if (products[i].quantity == 0) outOfStockCount++;

        file << left << setw(20) << products[i].name
             << setw(10) << products[i].quantity
             << setw(10) << products[i].unit
             << setw(10) << fixed << setprecision(2) << products[i].price
             << setw(15) << status << endl;

        totalQuantity += products[i].quantity;
        totalValue += products[i].price * products[i].quantity;
    }

    file << "------------------------------------------------" << endl;
    file << "Total Unique Items: " << products.size() << endl;
    file << "Total Quantity of All Items: " << totalQuantity << endl;
    file << "Out-of-Stock Items: " << outOfStockCount << endl;
    file << "Estimated Inventory Value: Rs. " << fixed << setprecision(2) << totalValue << endl;
    file.close();

    ShowMessage(hInventoryDlg, "Inventory report exported to 'inventory_report.txt' successfully!");
}

// Add to cart
void AddToCart() {
    HWND hList = GetDlgItem(hCustomerDlg, IDC_PRODUCT_LIST);
    int index = ListView_GetNextItem(hList, -1, LVNI_SELECTED);

    if (index == -1) {
        ShowMessage(hCustomerDlg, "Please select a product to add to cart.");
        return;
    }

    // Find actual product index from the list view index
    int actualIndex = -1;
    int listIndex = 0;
    for (size_t i = 0; i < products.size(); i++) {
        if (products[i].quantity > 0) {
            if (listIndex == index) {
                actualIndex = i;
                break;
            }
            listIndex++;
        }
    }

    if (actualIndex == -1) {
        ShowMessage(hCustomerDlg, "Invalid product selection.");
        return;
    }

    char qtyText[20];
    GetWindowText(GetDlgItem(hCustomerDlg, IDC_QUANTITY_EDIT), qtyText, 20);
    int qty = atoi(qtyText);

    if (qty <= 0) {
        ShowMessage(hCustomerDlg, "Please enter a valid quantity.");
        return;
    }

    if (qty > products[actualIndex].quantity) {
        ShowMessage(hCustomerDlg, "Not enough stock available!");
        return;
    }

    // Add to cart
    bool found = false;
    for (auto& item : cart) {
        if (item.first.name == products[actualIndex].name) {
            item.second += qty;
            found = true;
            break;
        }
    }

    if (!found) {
        cart.push_back({products[actualIndex], qty});
    }

    // Update product quantity
    products[actualIndex].quantity -= qty;

    // If quantity becomes zero, remove from customer menu
    if (products[actualIndex].quantity == 0) {
        ShowMessage(hCustomerDlg, "Item added to cart successfully! This item is now out of stock.");
    } else {
        ShowMessage(hCustomerDlg, "Item added to cart successfully!");
    }
}

// Preview bill before checkout
void PreviewBill() {
    if (cart.empty()) {
        ShowMessage(hCustomerDlg, "Your cart is empty!");
        return;
    }

    char name[100];
    GetWindowText(GetDlgItem(hCustomerDlg, IDC_CUSTOMER_EDIT), name, 100);
    if (strlen(name) == 0) {
        ShowMessage(hCustomerDlg, "Please enter customer name.");
        return;
    }

    // Calculate total
    double total = 0.0;
    for (const auto& item : cart) {
        total += item.first.price * item.second;
    }

    // Calculate discount
    double discount = 0.0;
    if (total >= 500 && total <= 1000) {
        discount = total * 0.05;
    } else if (total > 1000 && total <= 2000) {
        discount = total * 0.10;
    } else if (total > 2000) {
        discount = total * 0.15;
    }

    double finalPrice = total - discount;

    // Get current date and time
    time_t now = time(0);
    char dt[100];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Generate bill preview
    stringstream preview;
    preview << "==================================================\r\n";
    preview << "                  BILL PREVIEW                   \r\n";
    preview << "==================================================\r\n\r\n";
    preview << "Date & Time:            " << dt << "\r\n";
    preview << "Customer Name:          " << name << "\r\n";
    preview << "Items Purchased:        " << cart.size() << "\r\n";
    preview << "Total Price:         Rs. " << fixed << setprecision(2) << total << "\r\n";
    preview << "Discount:            Rs. " << fixed << setprecision(2) << discount << "\r\n";
    preview << "Final Bill:          Rs. " << fixed << setprecision(2) << finalPrice << "\r\n\r\n";
    preview << "==================================================\r\n";
    preview << "     Confirm checkout to complete purchase!      \r\n";
    preview << "==================================================";

    SetWindowText(GetDlgItem(hCustomerDlg, IDC_INFO_EDIT), preview.str().c_str());
}

// Checkout with bill generation and saving
void Checkout() {
    if (cart.empty()) {
        ShowMessage(hCustomerDlg, "Your cart is empty!");
        return;
    }

    char name[100];
    GetWindowText(GetDlgItem(hCustomerDlg, IDC_CUSTOMER_EDIT), name, 100);
    if (strlen(name) == 0) {
        ShowMessage(hCustomerDlg, "Please enter customer name.");
        return;
    }

    // Calculate discount
    double discount = 0.0;
    if (totalBill >= 500 && totalBill <= 1000) {
        discount = totalBill * 0.05;
    } else if (totalBill > 1000 && totalBill <= 2000) {
        discount = totalBill * 0.10;
    } else if (totalBill > 2000) {
        discount = totalBill * 0.15;
    }

    double finalPrice = totalBill - discount;

    // Add to finance
    storeFinance.addFinance(finalPrice);

    // Get current date and time
    time_t now = time(0);
    char dt[100];
    strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Generate bill
    stringstream bill;
    bill << "==================================================\r\n";
    bill << "                   BILL SUMMARY                   \r\n";
    bill << "==================================================\r\n\r\n";
    bill << "Date & Time:            " << dt << "\r\n";
    bill << "Customer Name:          " << name << "\r\n";
    bill << "Items Purchased:        " << cart.size() << "\r\n";
    bill << "Total Price:         Rs. " << fixed << setprecision(2) << totalBill << "\r\n";
    bill << "Discount:            Rs. " << fixed << setprecision(2) << discount << "\r\n";
    bill << "Final Bill:          Rs. " << fixed << setprecision(2) << finalPrice << "\r\n\r\n";
    bill << "==================================================\r\n";
    bill << "       THANK YOU FOR SHOPPING WITH US!            \r\n";
    bill << "==================================================";

    SetWindowText(GetDlgItem(hCustomerDlg, IDC_INFO_EDIT), bill.str().c_str());

    // Save to file
    ofstream file("bills.txt", ios::app);
    if (file) {
        file << "Customer Name: " << name << "\n";
        file << bill.str() << "\n\n";
        file.close();
    }

    // Clear cart
    cart.clear();
    totalBill = 0.0;
}

// Show message dialog
void ShowMessage(HWND hwnd, const char* message, const char* title) {
    MessageBox(hwnd, message, title, MB_OK | MB_ICONINFORMATION);
}

// Convert string to lowercase
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return tolower(c);
    });
    return str;
}