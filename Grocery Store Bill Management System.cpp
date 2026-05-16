#include<iostream>
#include<string>
#include<fstream>
using namespace std;
void bill(float totalBill, float discount, float discounted_price, float quantity, int itemCount, string name);
int main() 
{
	// Price Decleration ...
    
    float ghee = 550, rice = 320, sugar = 180, tea = 1500, shampoo = 800, soap = 150,
          toothpaste = 220, biscuit = 750, bread = 450, jam = 350, coffee = 950,
          noodles = 190, snacks = 150, colddrink = 250, ketchup = 450;


    int choise;
    float discount, quantity, discounted_price, totalBill = 0;
    string name;
    cout<<endl;
    
    
    // Showing Menu using an arrays and pointers ...
    
    string menu[3]={"==================================================",
	                "                GROCERY STORE MENU                ",
					"=================================================="};
    string *ptr=menu;
	for (int i=0; i<3; i++)
	{
		cout<<*ptr<<endl;
		ptr++;
	}
	
	string menu1[3]={"   Product","          Quantity","             Price"};
	string *ptr1=menu1;
	for (int i=0; i<3; i++)
	{
		cout<<*ptr1;
		ptr1++;
	}
	cout<<endl;
	
	string menu3[1]={"=================================================="};
	string *ptr3=menu3;
	for (int i=0; i<1; i++)
	{
		cout<<*ptr3;
		ptr3++;
	}
	cout<<endl;
	cout<<endl;
	
	string menu2[16]={"1.  Ghee              1Kg             - Rs. 550",
	                  "2.  Rice              1Kg             - Rs. 320",
					  "3.  Sugar             1Kg             - Rs. 180",
					  "4.  Tea               1Kg             - Rs. 1500",
					  "5.  Shampoo           1000ml          - Rs. 800",
					  "6.  Toothpaste        250ml           - Rs. 220",
					  "7.  Biscuit           1Kg             - Rs. 750",
					  "8.  Bread             1Kg             - Rs. 450",
					  "9.  Jam               1Kg             - Rs. 350",
					  "10. Coffee            1Kg             - Rs. 950",
					  "11. Noodles           4 Packs         - Rs. 350",
					  "12. Snacks            1 Pack          - Rs. 150",
					  "13. Cold Drink        1 litre         - Rs. 250",
					  "14. Ketchup           1Kg             - Rs. 450",
					  "15. Soap              1 Pack          - Rs. 150",
					  "0.  Exit"};
	string *ptr2=menu2;
	for(int i=0; i<16; i++)
	{
		cout<<*ptr2<<endl;
		ptr2++;
	}
	cout<<endl;
    
    
    // Asking for costumer's name ......
    
    cout<<"Enter Costumer's Name: ";
	getline(cin,name);
    cout<<endl;
    
    
    // Using Switch Case inside a Do While loop to choose the products and adding them to the bill ....
    
    int itemCount=0;

    do 
	{
		cout << "Please enter your choice (0 to exit): ";
		cin >> choise;
        
        switch (choise) 
		{
            case 1:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Ghee: Rs. " << ghee*quantity << endl;
                cout<<endl;
                totalBill += ghee*quantity;
                itemCount++;
                break;
            case 2:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Rice: Rs. " << rice*quantity << endl;
                cout<<endl;
                totalBill += rice*quantity;
                itemCount++;
                break;
            case 3:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Sugar: Rs. " << sugar*quantity << endl;
                cout<<endl;
                totalBill += sugar*quantity;
                itemCount++;
                break;
            case 4:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Tea: Rs. " << tea*quantity << endl;
                cout<<endl;
                totalBill += tea*quantity;
                itemCount++;
                break;
            case 5:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Shampoo: Rs. " << shampoo*quantity << endl;
                cout<<endl;
                totalBill += shampoo*quantity;
                itemCount++;
                break;
            case 6:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Toothpaste: Rs. " << toothpaste*quantity << endl;
                cout<<endl;
                totalBill += toothpaste*quantity;
                itemCount++;
                break;
            case 7:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Biscuit: Rs. " << biscuit*quantity << endl;
                cout<<endl;
                totalBill += biscuit*quantity;
                itemCount++;
                break;
            case 8:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Bread: Rs. " << bread*quantity << endl;
                cout<<endl;
                totalBill += bread*quantity;
                itemCount++;
                break;
            case 9:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Jam: Rs. " << jam*quantity << endl;
                cout<<endl;
                totalBill += jam*quantity;
                itemCount++;
                break;
            case 10:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Coffee: Rs. " << coffee*quantity << endl;
                cout<<endl;
                totalBill += coffee*quantity;
                itemCount++;
                break;
            case 11:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Noodles: Rs. " << noodles*quantity << endl;
                cout<<endl;
                totalBill += noodles*quantity;
                itemCount++;
                break;
            case 12:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Snacks: Rs. " << snacks*quantity << endl;
                cout<<endl;
                totalBill += snacks*quantity;
                itemCount++;
                break;
            case 13:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Cold Drink: Rs. " << colddrink*quantity << endl;
                cout<<endl;
                totalBill += colddrink*quantity;
                itemCount++;
                break;
            case 14:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Ketchup: Rs. " << ketchup*quantity << endl;
                cout<<endl;
                totalBill += ketchup*quantity;
                itemCount++;
                break;
            case 15:
            	cout << "Quantity: ";
	            cin>>quantity;
                cout << "You selected Soap: Rs. " << soap*quantity << endl;
                cout<<endl;
                totalBill += soap*quantity;
                itemCount++;
                break;
            case 0:
            	cout<<endl;
                cout << "Exiting the program..." << endl;
                break;
            default:
                cout << "Invalid choice! Please select a valid product number." << endl;
        }

    } 
	while (choise != 0);
    cout<<endl;
    
     
    // Calling the bill function created above to show the bill ...
    
    
	bill(totalBill, discount, discounted_price, quantity, itemCount, name);
	
    return 0;
    
}


// Function for calculating bill ...

void bill(float totalBill, float discount, float discounted_price, float quantity, int itemCount, string name)
{
	cout << "==================================================" << endl;
    cout << "                   BILL SUMMARY                   " << endl;
    cout << "==================================================" << endl;
    cout<<endl;
	
	
	// Calculating Discount for bill ....
	
	
	if(totalBill>500 && totalBill<=1000)
    {
    	discount=totalBill*0.05;
    	cout<<"5% Discount Given!"<<endl;
	}
	else if(totalBill>1000 && totalBill<=1700)
	{
		discount=totalBill*0.1;
		cout<<"10% Discount Given!"<<endl;
	}
	else if(totalBill>1700 && totalBill<=2700)
	{
		discount=totalBill*0.15;
		cout<<"15% Discount Given!"<<endl;
	}
    else if(totalBill>2700 && totalBill<=4000)
    {
    	discount=totalBill*0.25;
		cout<<"25% Discount Given!"<<endl;
	}
	else if(totalBill>4000)
	{
		discount=totalBill*0.3;
		cout<<"30% Discount Given!"<<endl;
	}
	else
	{
		cout<<"No Discount Given!"<<endl;
	}
	
	
	// Showing the bill .........................
	
	
	discounted_price=totalBill-discount;
	
	cout<<"Costumer Name:                   "<<name<<endl;
	cout << "Number of Items Bought:          " << itemCount << endl;
	cout << "Total Price :                Rs. " << totalBill << endl;
    cout << "Discout :                    Rs. "<< discount << endl;
    cout << "Total Bill :                 Rs. " << discounted_price << endl;
    cout<<endl;
    cout << "==================================================" << endl;
    cout << "         THANK YOU FOR SHOPPING WITH US!          " << endl;
    cout << "==================================================" << endl;
    
    
    // Using File Handling (ofstream) to save the bill onto an enternal file ......
    
    
    ofstream bill;
    bill.open("sample.txt", ios::app);
    bill << "==================================================" << endl;
    bill << "                   BILL SUMMARY                   " << endl;
    bill << "==================================================" << endl;
    bill << endl;
    if(totalBill>500 && totalBill<=1000)
    {
    	discount=totalBill*0.05;
    	bill<<"5% Discount Given!"<<endl;
	}
	else if(totalBill>1000 && totalBill<=1700)
	{
		discount=totalBill*0.1;
		bill<<"10% Discount Given!"<<endl;
	}
	else if(totalBill>1700 && totalBill<=2700)
	{
		discount=totalBill*0.15;
		bill<<"15% Discount Given!"<<endl;
	}
    else if(totalBill>2700 && totalBill<=4000)
    {
    	discount=totalBill*0.25;
		bill<<"25% Discount Given!"<<endl;
	}
	else if(totalBill>4000)
	{
		discount=totalBill*0.3;
		bill<<"30% Discount Given!"<<endl;
	}
	else
	{
		bill<<"No Discount Given!"<<endl;
	}
	
	
	bill << "Costumer Name:                "<<name<<endl;
	bill << "Number of Items Bought:       " << itemCount << endl;
    bill << "Total Price :             Rs. " << totalBill << endl;
    bill << "Discout :                 Rs. "<< discount << endl;
    bill << "Total Bill :              Rs. " << discounted_price << endl;
    bill<<endl;
    bill << "==================================================" << endl;
    bill << "         THANK YOU FOR SHOPPING WITH US!          " << endl;
    bill << "==================================================" << endl;
    bill.close();
    
}

//  Made by Muhammad Saad Akhtar.
