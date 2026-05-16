#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<conio.h>
#include<algorithm>
#include<windows.h>
#include<ctime>
using namespace std;

const int MAX = 50;

void setColor(int color) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

string toLowerCase(string str) 
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Abstract class for polymorphism and abstraction ...
class ProductBase 
{
    private: // Set to private for encapsulation ...
        static string names[MAX], productUnit[MAX];
        static int quantity[MAX], itemCount;
        static float price[MAX];
        string storeName;

    public:
        ProductBase(string sName) : storeName(sName) {}
    
        // Copy constructor for deep copy ...
        ProductBase(const ProductBase& other) : storeName(other.getStoreName()) {}
    
        // Assignment operator for deep copy ...
        ProductBase& operator=(const ProductBase& other) 
		{
            if (this != &other) 
			{
                storeName = other.getStoreName();
            }
            return *this;
        }
    
        // Operator overloading: Compare store names ...
        bool operator==(const ProductBase& other) const 
		{
            return toLowerCase(storeName) == toLowerCase(other.getStoreName());
        }
    
        // Getter for storeName
        string getStoreName() const 
		{ 
		    return storeName; 
		}
    
        void showHeader()
        {
        	setColor(3); // Light cyan
        	cout<<"========== WELCOME TO ZAIN KA KHOKHA =========="<<endl;
        	setColor(7); // Reset to default
        	cout<<endl;
		}
    
        void animatedMenu()
        {
            cout<<endl;
            string line1 = "================== STORE MENU =================\n";
            string line2 = "1. Inventory (Admin Only)\n";
            string line3 = "2. Customer Products Menu\n";
            string line4 = "3. Exit\n";
            string fullMenu = line1 + "\n";
            string fullMenu1 = line2 + line3 + line4;
            string fullMenu2 = "\nEnter your choice: ";

            setColor(14);
            for (char c : fullMenu) 
            {
                cout << c;
                Sleep(15);  // 15 ms delay per character
            }
            setColor(15);
            for (char c1 : fullMenu1) 
            {
                cout << c1;
                Sleep(15);  // 15 ms delay per character
            }
            setColor(14);
            for (char c2 : fullMenu2) 
            {
                cout << c2;
                Sleep(15);  // 15 ms delay per character
            }
            setColor(7);
        }

        void displayInventory() 
        {
        	setColor(11); 
            if (itemCount == 0) 
            {
                cout << "Inventory is empty." << endl;
                return;
            }
            cout << endl;

            cout << "----------------------------------------------------------------" << endl;
            cout << "                      Current Inventory                        " << endl;
            cout << "----------------------------------------------------------------" << endl;

            cout << left << setw(17) << "Name";
            cout << setw(12) << "Quantity";
            cout << left << setw(12) << "Unit";
            cout << setw(12) << "Price";
            cout << right << setw(8) <<"Status" << endl;

            cout << "----------------------------------------------------------------" << endl;
            setColor(7);
            setColor(15);

            for (int i = 0; i < itemCount; i++) 
            {
                cout << left << setw(20) << names[i]
                     << setw(10) << quantity[i]
                     << left << setw(12) << getproductUnit(i)
                     << left << setw(10) << price[i]
                     << right << setw(10) <<  (quantity[i] > 0 ? "In stock" : "Out of stock") << endl;
            }
            setColor(7); 
            cout<<endl;
        }
    
        // Overloaded displayInventory ...
        void displayInventory(int minQuantity) 
        {
        	setColor(11); 
            if (itemCount == 0) 
            {
                cout << "Inventory is empty." << endl;
                return;
            }
            cout << endl;

            cout << "----------------------------------------------------------------" << endl;
            cout << "        Current Inventory (Quantity > " << minQuantity << ")        " << endl;
            cout << "----------------------------------------------------------------" << endl;

            cout << left << setw(17) << "Name";
            cout << setw(12) << "Quantity";
            cout << left << setw(12) << "Unit";
            cout << setw(12) << "Price";
            cout << right << setw(8) <<"Status" << endl;

            cout << "----------------------------------------------------------------" << endl;
            setColor(7);
            setColor(15);

            bool found = false;
            for (int i = 0; i < itemCount; i++) 
            {
                if (quantity[i] > minQuantity) 
                {
                    cout << left << setw(20) << names[i]
                         << setw(10) << quantity[i]
                         << left << setw(12) << getproductUnit(i)
                         << left << setw(10) << price[i]
                         << right << setw(10) <<  (quantity[i] > 0 ? "In stock" : "Out of stock") << endl;
                    found = true;
                }
            }
            if (!found) {
                cout << "No items with quantity greater than " << minQuantity << "." << endl;
            }
            setColor(7); 
            cout<<endl;
        }
    
        virtual void showMenu() = 0; // pure virtual function
    
        // Getters and setters for private static members
        static string getproductUnit(int i) 
		{ 
		    return productUnit[i]; 
		}
        static int getItemCount() 
		{ 
		    return itemCount; 
		}
        static string getName(int i) 
		{ 
		    return names[i]; 
		}
        static int getQuantity(int i) 
		{ 
		    return quantity[i]; 
		}
        static float getPrice(int i) 
		{ 
		    return price[i]; 
		}
        static void reduceQuantity(int i, int qty) 
		{ 
		    quantity[i] -= qty; 
		}
        static void setName(int i, const string& name) 
		{ 
		    if (i < MAX) names[i] = name; 
		}
        static void setQuantity(int i, int qty) 
		{ 
		    if (i < MAX) quantity[i] = qty; 
		}
        static void setPrice(int i, float pr) 
		{ 
		    if (i < MAX) price[i] = pr; 
		}
        static void setProductUnit(int i, const string& unit) 
		{ 
		    if (i < MAX) productUnit[i] = unit; 
		}
        static void incrementItemCount() 
		{ 
		    if (itemCount < MAX) itemCount++; 
		}
        static void decrementItemCount() 
		{ 
		    if (itemCount > 0) itemCount--; 
		}
};

// Initializing static members with 10 default products ...
string ProductBase::names[MAX] = {"Apple", "Banana", "Milk", "Bread", "Eggs",
                                  "Rice", "Sugar", "Salt", "Coffee", "Tea"};
int ProductBase::quantity[MAX] = {50, 30, 20, 25, 60,
                                  40, 35, 45, 15, 20};
float ProductBase::price[MAX] = {120, 40, 80, 45, 120,
                                 90, 55, 20, 250, 150};
string ProductBase::productUnit[MAX]= { "Kg", "Dozen", "Ltr", "Kg", "Dozen",
                                          "Kg", "Kg", "Kg", "Kg", "Kg" };
int ProductBase::itemCount = 10;

class Report
{
    public:
        static void generateInventoryReport()
        {
        	setColor(11);
            cout << endl;
            cout << "-------------- INVENTORY REPORT ---------------" << endl;
            cout << left << setw(18) << "Item Name" << setw(15) << "Price (Rs.)" << setw(10) << "Stock" << endl;
            cout << "-----------------------------------------------" << endl;
            setColor(7);
            int totalItems = ProductBase::getItemCount();
            int outOfStockCount = 0;
            int totalQuantity = 0;
            float totalValue = 0;
            setColor(15);
            for (int i = 0; i < totalItems; i++)
            {
                string name = ProductBase::getName(i);
                float price = ProductBase::getPrice(i);
                int quantity = ProductBase::getQuantity(i);

                totalQuantity += quantity;
                totalValue += price * quantity;

                if (quantity == 0)
                    outOfStockCount++;

                cout << left << setw(20) << name << setw(15) << price << setw(10) << quantity << endl;
            }
            setColor(7);
            setColor(11);
            cout << "-----------------------------------------------" << endl;
            setColor(7);
            cout<<endl;
            setColor(15);
            cout << "Total Unique Items: " << totalItems << endl;
            cout << "Total Quantity of All Items: " << totalQuantity << endl;
            cout << "Out-of-Stock Items: " << outOfStockCount << endl;
            cout<<endl;
            setColor(11);
            cout << "-----------------------------------------------" << endl;
            setColor(7);
        }

        static void listOutOfStockItems()
        {
        	setColor(11);
            cout << endl << "----------- OUT OF STOCK ITEMS -----------" << endl;
            bool found = false;
            setColor(7);
            for (int i = 0; i < ProductBase::getItemCount(); i++)
            {
            	setColor(15);
                if (ProductBase::getQuantity(i) == 0)
                {
                    cout << "- " << ProductBase::getName(i) << endl;
                    found = true;
                }
            }

            if (!found)
            {
            	cout<<endl;
                cout << "No items are currently out of stock." << endl;
                cout<<endl;
            }
            setColor(7);
            setColor(11);
            cout << "------------------------------------------" << endl;
            setColor(7);
        }
        // File Handling .....
    
        static void exportInventoryToFile()
        {
            ofstream file("inventory_report.txt", ios::app);
            if (!file.is_open())
            {
                cout << "Error: Could not create file." << endl;
                return;
            }

            // Get current date and time
            time_t now = time(0);
            char dt[20];
            strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));

            file << endl;
            file << "Date: " << dt << endl;
            file << "=============== INVENTORY REPORT ===============" << endl;
            file << left << setw(20) << "Item Name" << setw(15) << "Price (Rs.)" << setw(10) << "Stock" << endl;
            file << "------------------------------------------------" << endl;

            int totalItems = ProductBase::getItemCount();
            int outOfStockCount = 0;
            int totalQuantity = 0;
            float totalValue = 0;

            for (int i = 0; i < totalItems; i++)
            {
                string name = ProductBase::getName(i);
                float price = ProductBase::getPrice(i);
                int quantity = ProductBase::getQuantity(i);

                totalQuantity += quantity;
                totalValue += price * quantity;

                if (quantity == 0)
                    outOfStockCount++;

                file << left << setw(20) << name << setw(15) << price << setw(10) << quantity << endl;
            }

            file << "------------------------------------------------" << endl;
            file << "Total Unique Items: " << totalItems << endl;
            file << "Total Quantity of All Items: " << totalQuantity << endl;
            file << "Out-of-Stock Items: " << outOfStockCount << endl;
            file << "Estimated Inventory Value: Rs. " << totalValue << endl;
            file << "================================================" << endl;

            file.close();
    
            setColor(11);
            cout << endl;
            cout << "---------------------------- Status -----------------------------" <<endl;
            cout << endl;
            setColor(15);
            cout << "Inventory report exported to 'inventory_report.txt' successfully!" << endl;
            cout<<endl;
            setColor(11);
            cout << "-----------------------------------------------------------------" <<endl;
            setColor(7);
        }

};

class Finance
{
    private:
        double totalFinance;
        double financeLimit;

    public:
        Finance(double total, double limit) : totalFinance(total), financeLimit(limit) {}
    
        // Copy constructor for deep copy
        Finance(const Finance& other) : totalFinance(other.totalFinance), financeLimit(other.financeLimit) {}
    
        // Assignment operator for deep copy
        Finance& operator=(const Finance& other) {
            if (this != &other) {
                totalFinance = other.totalFinance;
                financeLimit = other.financeLimit;
            }
            return *this;
        }
    
        // Operator overloading: += for adding funds
        Finance& operator+=(double amount) {
            totalFinance += amount;
            return *this;
        }
    
        double getTotalFinance() const 
		{
		    return totalFinance; 
		}
        double getFinanceLimit() const 
		{ 
		    return financeLimit; 
		}
    
        void showFinanceStatus() const
        {
    	    setColor(11);
    	    cout << endl;
            cout << "------------- Finance Status -------------" << endl;
            setColor(15);
            cout << endl;
            cout << "Total Finance:     Rs. " << totalFinance << endl;
            cout << "Finance Limit:     Rs. " << financeLimit << endl;
            cout << "Available Finance: Rs. " << (totalFinance - financeLimit) << endl;
            cout << endl;
            setColor(11);
            cout << "------------------------------------------" << endl;
            cout << endl;
            setColor(7);
        }

        bool canPurchase(double amount)
        {
            return (totalFinance - amount) >= financeLimit;
        }

        void deductFinance(double amount)
        {
            totalFinance -= amount;
        }
        void addFinance(double amount)
        {
            totalFinance += amount;
        }
};

class Inventory : public ProductBase 
{
    protected:
        string inventoryManager;
        Finance finance;

    public:
        Inventory(string manager, string sName, double totalFinance, double financeLimit) 
            : ProductBase(sName), inventoryManager(manager), finance(totalFinance, financeLimit) {}

        Finance& getFinance() 
		{ 
		    return finance; 
		}

        void showMenu() override 
        {
            int adminChoice;
            do 
            {
        	    cout<<endl;
        	    cout<<endl;
        	    setColor(3); 
                cout << "=============== Inventory Menu ================" << endl;
                setColor(7);
                cout<<endl;
                setColor(15);
                cout << "1.  Show Inventory" << endl;
                cout << "2.  Edit Item" << endl;
                cout << "3.  Delete Item" << endl;
                cout << "4.  View Inventory Report" << endl;
                cout << "5.  View Out-of-Stock Items" << endl;
                cout << "6.  Export Inventory to File" << endl;
                cout << "7.  View Finance Status" << endl;  
                cout << "8.  Seller Purchase Menu" << endl; 
                cout << "9.  Fetch Bill Details by Customer Name" << endl; 
                cout << "10. Back to Main Menu" << endl;
                cout<<endl;

                while (true) 
		        {
		        	cout << "Enter your choice: ";
                    cin >> adminChoice;

                    // Check if input is valid and in range
                    if (cin.fail() || adminChoice < 1 || adminChoice > 10) 
			        {
                        cin.clear(); // clear the error flag
                        cin.ignore(1000, '\n'); // discard invalid input
                        cout << "Invalid input! Please enter a positive number between 1 and 10." << endl;
                    } 
			        else 
			        {
                        break; // exit the loop
                    }
                }
                setColor(7); 
            
                switch (adminChoice) 
                {
                    case 1: displayInventory(); break;
                    case 2: editItem(); break;
                    case 3: deleteItem(); break;
                    case 4: Report::generateInventoryReport(); break;
                    case 5: Report::listOutOfStockItems(); break;
                    case 6: Report::exportInventoryToFile(); break;
                    case 7: finance.showFinanceStatus(); break;
                    case 8: sellerPurchaseMenu(); break;
                    case 9: fetchBillDetailsByCustomerName(); break;
                    case 10: break;
                    default: cout << "Invalid choice!" << endl;
                }
            } 
            while (adminChoice != 10);
        }

        void fetchBillDetailsByCustomerName() 
        {
            setColor(15);
            string customerName;
            cout << endl;
            cout << "Enter Customer Name to fetch bill details: ";
            cin.ignore();
            getline(cin, customerName);

            string lowerInput = toLowerCase(customerName);

            ifstream file("sample.txt");
            if (!file.is_open()) 
            {
                cout << "Error: Could not open the bill file." << endl;
                return;
            }

            string line;
            bool found = false;

            while (getline(file, line) && !found) 
            {
                string lowerLine = toLowerCase(line);

                if (lowerLine.find("customer name:           " + lowerInput) != string::npos) 
                {
                    found = true;

                    cout << endl;
                    setColor(11);
                    cout << "==================================================" << endl;
                    setColor(15);
                    cout << "                   BILL SUMMARY                   " << endl;
                    setColor(11);
                    cout << "==================================================" << endl;
                    cout << endl;
                    setColor(7);
                    setColor(15);
                    cout << line << endl;

                    // Print the next 5 lines of the bill
                    setColor(15); 
                    for (int i = 0; i < 4; i++) 
                    {
                        if (getline(file, line)) 
                        {
                            cout << line << endl;
                        }
                    }
                    cout<<endl;
                    setColor(11);
                    cout << "==================================================" << endl;
                        setColor(15);
                    cout << "       THANK YOU FOR SHOPPING WITH US!            " << endl;
                    setColor(11);
                    cout << "==================================================" << endl;
                    setColor(7);
                }
            }

            if (!found) 
            {
                cout << "No bill found for customer: " << customerName << endl;
            }

            setColor(7);
            file.close();
        }

        void showinventoryManagerDetails() 
        {
            cout << "Store:     " << getStoreName() << endl;
            cout << "Manager:   " << inventoryManager << endl;
        }

    
        bool sellerPurchaseMenu()
        {
	        cout<<endl;
	        setColor(11);
            cout << "---------- Seller Purchase Menu ----------" << endl;
            cout<<endl;
            setColor(7); 
            setColor(15);
            cout << "Enter '0' as product name to finish." << endl;
            setColor(7);
            while (true)
            {
            	setColor(15);
                string sellerItem;
                int qty;
                float unitPrice; 
                string unit;
                cout << "Enter product name from seller: ";
                cin.ignore();
                getline(cin, sellerItem);
                 if (sellerItem == "0") 
                    {
                        cout << "Returning to Main Menu..." << endl;
                        cout << endl;
                        return true;
                    }

                cout << "Enter quantity: ";
                cin >> qty;
                cout << "Enter price per unit: ";
                cin >> unitPrice;
                cout << "Enter New Unit: ";
                cin.ignore();
                getline(cin,unit);
        
                if (cin.fail() || qty < 0 || unitPrice < 0)
                    {
                        cout << "Invalid input. Please enter valid quantity and price." << endl;
                        cin.clear();
                        cin.ignore(10000, '\n');
                        continue;
                    }

                float totalCost = qty * unitPrice;

                if (!finance.canPurchase(totalCost))
                {
                    cout << "Insufficient finance balance to make this purchase!" << endl;
                    continue;
                }

               // Deduct finance
               finance.deductFinance(totalCost);

               // Check if item exists, update stock and price, else add new item
               string sellerItemLower = toLowerCase(sellerItem);
               bool found = false;

               for (int i = 0; i < ProductBase::getItemCount(); i++)
               {
                    if (toLowerCase(ProductBase::getName(i)) == sellerItemLower)
                    {
                        ProductBase::setQuantity(i, ProductBase::getQuantity(i) + qty);
                        ProductBase::setPrice(i, unitPrice);
                        ProductBase::setProductUnit(i, unit);
                        found = true;
                        cout << "Existing item updated in inventory." << endl;
                        break;
                    }
                }

                if (!found)
                {
                    if (ProductBase::getItemCount() < MAX)
                    {
                        ProductBase::setName(ProductBase::getItemCount(), sellerItem);
                        ProductBase::setQuantity(ProductBase::getItemCount(), qty);
                        ProductBase::setPrice(ProductBase::getItemCount(), unitPrice);
                        ProductBase::setProductUnit(ProductBase::getItemCount(), unit);
                        ProductBase::incrementItemCount();
                        cout << "New item added to inventory." << endl;
                    }
                    else
                    {
                        cout << "Inventory is full! Cannot add new item." << endl;
                    }
                }
 
                cout << "Purchase successful! Rs. " << totalCost << " deducted from finance." << endl;
                finance.showFinanceStatus();
                cout << endl;
            }
                return false;
                setColor(7);
        }

        void editItem() 
        {
            string name;
            setColor(11); 
            cout<<endl;
            cout<<"-------------- EDIT MENU --------------"<<endl; 
            cout<<endl;
            setColor(7); 
            setColor(15);
            cout << "Enter Item Name to edit: ";
            cin.ignore();
            getline(cin, name);

            string nameLower = toLowerCase(name);

            for (int i = 0; i < ProductBase::getItemCount(); i++) 
            {
                if (toLowerCase(ProductBase::getName(i)) == nameLower) 
                {
                    cout << "Enter New Name: ";
                    getline(cin, name);
                    ProductBase::setName(i, name);
                    cout << "Enter New Quantity: ";
                    int qty;
                    cin >> qty;
                    ProductBase::setQuantity(i, qty);
                    cout << "Enter New Price: ";
                    float pr;
                    cin >> pr;
                    ProductBase::setPrice(i, pr);
                    cout << "Enter New Unit: ";
                    cin.ignore();
                    string unit;
                    getline(cin, unit);
                    ProductBase::setProductUnit(i, unit);
                    setColor(7);
                    setColor(6);
                    cout << "Item updated successfully!" << endl;
                    setColor(7);
                    setColor(11);
                    cout<<endl;
                    cout<<"---------------------------------------"<<endl; 
                    setColor(7); 
                    return;
                }
            }
            setColor(4); 
            cout << "Item not found!" << endl;
            cout<<endl;
            setColor(11);
            cout<<"---------------------------------------"<<endl; 
            setColor(7); 
            setColor(7); 
        }

        void deleteItem() 
        {
            string name;
            setColor(11);
            cout<<endl;
            cout<<"-------------- DELETE MENU --------------"<<endl; 
            cout<<endl;
            setColor(7); 
            setColor(15);
            cout << "Enter Item Name to delete: ";
            cin.ignore();
            getline(cin, name);

            transform(name.begin(), name.end(), name.begin(), ::tolower);

            for (int i = 0; i < ProductBase::getItemCount(); i++) 
            {
                string temp = ProductBase::getName(i);
                transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

                if (temp == name) 
                {
                    for (int j = i; j < ProductBase::getItemCount() - 1; j++) 
                    {
                        ProductBase::setName(j, ProductBase::getName(j + 1));
                        ProductBase::setQuantity(j, ProductBase::getQuantity(j + 1));
                        ProductBase::setPrice(j, ProductBase::getPrice(j + 1));
                        ProductBase::setProductUnit(j, ProductBase::getproductUnit(j + 1));
                    }
                    ProductBase::decrementItemCount();
                    setColor(7);
                    setColor(12);
                    cout << "Item deleted successfully!" << endl;
                    setColor(11);
                    cout<<endl;
                    cout<<"-----------------------------------------"<<endl; 
                    setColor(7); 
                    return;
                }
            }
            setColor(7);
            setColor(4);
            cout << "Item not found!" << endl;
            cout<<endl;
            setColor(11);
            cout<<"---------------------------------------"<<endl; 
            setColor(7); 
            setColor(7);
        }
};

class Store : public Inventory 
{
    protected:
        string location;

    public:
        Store(string loc, string manager, string sName, float totalFinance, float financeLimit) 
            : Inventory(manager, sName, totalFinance, financeLimit), location(loc)
        {
        	showHeader();
            displayStoreHeader();
        }
    
        void showHeader()
        {
        	setColor(3);
        	cout<<"========== WELCOME TO ZAIN KA KHOKHA =========="<<endl;
        	setColor(7);
        	cout<<endl;
	}

        void displayStoreHeader() 
        {
        	setColor(15); 
        	showinventoryManagerDetails();
            cout << "Location:  " << location << endl;
            cout<<endl;
            setColor(7); 
            setColor(3); 
            cout<<"==============================================="<<endl;
            cout << endl;
            setColor(7); 
        }

        void passwordProtect() 
        {
            setColor(6); 
            string password;
            cout << "Enter admin password: ";
    
            while (true) 
            {
    	        setColor(6);
                cin >> password;

                if (password == "123") 
                {
                    break; // correct password, exit loop
                } 
                else 
                {
        	        setColor(12); 

                    cout << "Incorrect password!" << endl;
                    cout << endl;
                    setColor(6);
                    cout << "Enter admin password: ";
                    setColor(7); 
                }
            }
        }

        void showMenu() override 
        {
            passwordProtect();
            Inventory::showMenu();
        }   
};

class Customer : public ProductBase 
{
	private:
        Finance& finance;
    public:
        Customer(string sName, Finance& storeFinance) : ProductBase(sName), finance(storeFinance) {}

        void showMenu() override 
        {
            customerBuy();
        }

        void customerBuy() 
        {
            if (ProductBase::getItemCount() == 0) 
            {
                cout << "No products available." << endl;
                return;
            }

            float total = 0, qty;
            string productName, customerName;
            int count = 0;

            cout << endl;
            setColor(15);
            cout << "Enter Customer Name: ";
            cin.ignore();
            getline(cin, customerName);
            cout << endl;
            setColor(7);

            while (true) 
            {
            	setColor(11);
            	cout << "==================================================" << endl;
            	cout << "                GROCERY STORE MENU                " << endl;
            	cout << "==================================================" << endl;
            	setColor(7);
            	setColor(15);
                cout << left << setw(19) << "Name";
                cout << left << setw(14) << "Unit";
                cout << right << setw(10) << "Price" << endl;
                setColor(7);
                setColor(11);
                cout << "==================================================" << endl;
                setColor(7);
                setColor(15);
                for (int i = 0; i < ProductBase::getItemCount(); i++) 
                {
                    if (ProductBase::getQuantity(i) > 0) 
                    {
                        cout << left << setw(20) << ProductBase::getName(i)
                             << left << setw(12) << ProductBase::getproductUnit(i)
                             << right << setw(10) << "Rs. " << ProductBase::getPrice(i) << endl;
                    }
                }
            
                cout << endl;
                cout << "Enter Product Name to buy (0 to exit): ";
                getline(cin, productName);
                setColor(7);

                if (productName == "0")
                break;

                bool found = false;
                string productNameLower = toLowerCase(productName);

                for (int i = 0; i < ProductBase::getItemCount(); i++) 
                {
                    string currentNameLower = toLowerCase(ProductBase::getName(i));

                    if (currentNameLower == productNameLower && ProductBase::getQuantity(i) > 0) 
                    {
                        cout << "Enter Quantity(in unit) for " << ProductBase::getName(i) << ": ";
                        cin >> qty;
                        cin.ignore();

                        if (qty > ProductBase::getQuantity(i)) 
                        {
                            cout << "Not enough stock!" << endl;
                            found = true;
                            break;
                        }
                        cout << "You Selected " << qty << ProductBase::getproductUnit(i) <<" " << productName << " : Rs. " << qty * ProductBase::getPrice(i) << endl;

                        total += ProductBase::getPrice(i) * qty;
                        ProductBase::reduceQuantity(i, qty);
                        count++;
                        cout << "Added to cart." << endl;
                        cout << endl;
                        found = true;
                        break;
                    }
                }

                if (!found) 
                {
                    cout << "Product not found or out of stock." << endl;
                }
            }

            if (count == 0) 
            {
                cout << endl;
                cout << "No items purchased." << endl;
                ofstream file1("sample.txt", ios::app);
                file1 << endl;
                // Get current date and time
                time_t now = time(0);
                char dt[20];
                strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
                file1 << "Date: " << dt << endl;
                file1 << "No items purchased." << endl;
                file1 << endl;
                file1.close();
                return;
            }

            float discount = 0, finalPrice = total;
    
            if (total >= 500 && total <= 1000) 
            {
                discount = total * 0.05;
            }
            else if (total > 1000 && total <= 2000)
            {
                discount = total * 0.10;
            }
            else if (total > 2000 && total <= 3000) 
            {
            discount = total * 0.15;
            }
            else if (total > 3000 && total <= 4500)
            {
                discount = total * 0.20;
            }
            else if (total > 4500) 
            {
                discount = total * 0.30;
            }
    
            finalPrice = total - discount;
            finance += finalPrice; // Using operator overloading ...
    
            // Get current date and time
            time_t now = time(0);
            char dt[20];
            strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
            setColor(11);
            cout << endl;
            cout << "==================================================" << endl;
            setColor(15);
            cout << "                   BILL SUMMARY                   " << endl;
            setColor(11);
            cout << "==================================================" << endl;
            cout<<endl;
            setColor(15);
            cout << "Date & Time:             " << dt << endl;
            cout << "Customer Name:           " << customerName << endl;
            cout << "Items Bought:            " << count << endl;
            cout << "Total Price:         Rs. " << total << endl;
            cout << "Discount:            Rs. " << discount << endl;
            cout << "Final Bill:          Rs. " << finalPrice << endl;
            cout << endl;
            setColor(11);
            cout << "==================================================" << endl;
            setColor(15);
            cout << "       THANK YOU FOR SHOPPING WITH US!            " << endl;
            setColor(11);
            cout << "==================================================" << endl;
            setColor(7);
    
            ofstream file("sample.txt", ios::app);
            file << endl;
            file << "==================================================" << endl;
            file << "                   BILL SUMMARY                   " << endl;
            file << "==================================================" << endl;
            file << endl;
            file << "Date & Time:             " << dt << endl;
            file << "Customer Name:           " << customerName << endl;
            file << "Items Bought:            " << count << endl;
            file << "Total Price:         Rs. " << total << endl;
            file << "Discount:            Rs. " << discount << endl;
            file << "Final Bill:          Rs. " << finalPrice << endl;
            file << endl;
            file << "==================================================" << endl;
            file << "       THANK YOU FOR SHOPPING WITH US!            " << endl;
            file << "==================================================" << endl;
            file.close();
        }
    
        // Overloaded customerBuy for predefined purchase list using C-style array
        void customerBuy(pair<string, float> purchaseList[], int listSize, const string& customerName) 
        {
            if (ProductBase::getItemCount() == 0) 
            {
                cout << "No products available." << endl;
                return;
            }

            float total = 0;
            int count = 0;

            cout << endl;
            setColor(15);
            cout << "Processing purchase for Customer: " << customerName << endl;
            cout << endl;
            setColor(7);

            for (int j = 0; j < listSize; j++) 
            {
                string productName = purchaseList[j].first;
                float qty = purchaseList[j].second;
                bool found = false;
                string productNameLower = toLowerCase(productName);

                for (int i = 0; i < ProductBase::getItemCount(); i++) 
                {
                    string currentNameLower = toLowerCase(ProductBase::getName(i));

                    if (currentNameLower == productNameLower && ProductBase::getQuantity(i) > 0) 
                    {
                        if (qty > ProductBase::getQuantity(i)) 
                        {
                            cout << "Not enough stock for " << ProductBase::getName(i) << "!" << endl;
                            found = true;
                            break;
                        }
                        cout << "Selected " << qty << " " << ProductBase::getproductUnit(i) << " " << productName << " : Rs. " << qty * ProductBase::getPrice(i) << endl;

                        total += ProductBase::getPrice(i) * qty;
                        ProductBase::reduceQuantity(i, qty);
                        count++;
                        cout << "Added to cart." << endl;
                        cout << endl;
                        found = true;
                        break;
                    }
                }

                if (!found) 
                {
                    cout << "Product " << productName << " not found or out of stock." << endl;
                }
            }

            if (count == 0) 
            {
                cout << endl;
                cout << "No items purchased." << endl;
                ofstream file1("sample.txt", ios::app);
                file1 << endl;
                time_t now = time(0);
                char dt[20];
                strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
                file1 << "Date: " << dt << endl;
                file1 << "No items purchased." << endl;
                file1 << endl;
                file1.close();
                return;
            }

            float discount = 0, finalPrice = total;
    
            if (total >= 500 && total <= 1000) 
            {
                discount = total * 0.05;
            }
            else if (total > 1000 && total <= 2000)
            {
                discount = total * 0.10;
            }
            else if (total > 2000 && total <= 3000) 
            {
                discount = total * 0.15;
            }
            else if (total > 3000 && total <= 4500)
            {
                discount = total * 0.20;
            }
            else if (total > 4500) 
            {
                discount = total * 0.30;
            }
    
            finance += finalPrice;
            finalPrice = total - discount;
    
            time_t now = time(0);
            char dt[20];
            strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
            setColor(11);
            cout << endl;
            cout << "==================================================" << endl;
            setColor(15);
            cout << "                   BILL SUMMARY                   " << endl;
            setColor(11);
            cout << "==================================================" << endl;
            cout<<endl;
            setColor(15);
            cout << "Date & Time:             " << dt << endl;
            cout << "Customer Name:           " << customerName << endl;
            cout << "Items Bought:            " << count << endl;
            cout << "Total Price:         Rs. " << total << endl;
            cout << "Discount:            Rs. " << discount << endl;
            cout << "Final Bill:          Rs. " << finalPrice << endl;
            cout << endl;
            setColor(11);
            cout << "==================================================" << endl;
            setColor(15);
            cout << "       THANK YOU FOR SHOPPING WITH US!            " << endl;
            setColor(11);
            cout << "==================================================" << endl;
            setColor(7);
    
            ofstream file("sample.txt", ios::app);
            file << endl;
            file << "==================================================" << endl;
            file << "                   BILL SUMMARY                   " << endl;
            file << "==================================================" << endl;
            file << endl;
            file << "Date & Time:             " << dt << endl;
            file << "Customer Name:           " << customerName << endl;
            file << "Items Bought:            " << count << endl;
            file << "Total Price:         Rs. " << total << endl;
            file << "Discount:            Rs. " << discount << endl;
            file << "Final Bill:          Rs. " << finalPrice << endl;
            file << endl;
            file << "==================================================" << endl;
            file << "       THANK YOU FOR SHOPPING WITH US!            " << endl;
            file << "==================================================" << endl;
            file.close();
        }        
};

int main() 
{
	// Using Polymorphism ....
	
    ProductBase* store = new Store("Ali Town Branch", "Zain Badmash", "Zain Ka KhoKha", 100000, 20000);

    ProductBase* cus = new Customer("Zain Ka KhoKha", dynamic_cast<Store*>(store)->getFinance()); // Pass store's Finance object

    int choice;

    do 
    {
    	store->animatedMenu();
    	                      
    	setColor(14);
        while (true) 
		{
            cin >> choice;

            // Check if input is valid and in range
            if (cin.fail() || choice < 1 || choice > 3) 
			{
                cin.clear(); // clear the error flag
                cin.ignore(1000, '\n'); // discard invalid input
                setColor(12); 
                cout << "Invalid input! Please enter a positive number between 1 and 3." << endl;
                setColor(7); 
                setColor(14); 
                cout << "\nEnter your choice: ";
            } 
			else 
			{
                break; // exit the loop
            }
        }
        setColor(7);

        switch (choice) 
        {
            case 1:
                store->showMenu();
                break;
            case 2:
                cus->showMenu();
                break;
            case 3:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } 
    while (choice != 3);

    delete store;
    delete cus;

    getch();
    return 0;
}
// Made by Muhammad Saad Akhtar.