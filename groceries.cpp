#include "split.h" // contains a split function that returns all fields that were separated by some character as a vector of strings
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Item {
  int id;
  string description;
  double price;
};

struct Customer {
  int id;
  string name;
  string address;
  string city;
  string state;
  string zip;
  string phone;
  string email;
};

vector<Item> items;

vector<Customer> customers;

void read_customers(const string& filename) {
  ifstream file(filename);
  string line;
  while (getline(file, line)) {
    vector<string> fields = split(line, ',');
    Customer customer;
    customer.id = stoi(fields[0]);
    customer.name = fields[1];
    customer.address = fields[2];
    customer.city = fields[3];
    customer.state = fields[4];
    customer.zip = fields[5];
    customer.phone = fields[6];
    customer.email = fields[7];
    customers.push_back(customer);
  }
    cout << "Total customers: " << customers.size() << endl;
}

void read_items(const string& filename) {
  ifstream file(filename);
  string line;
  while (getline(file, line)) {
    vector<string> fields = split(line, ',');
    Item item;
    item.id = stoi(fields[0]);
    item.description = fields[1];
    item.price = stod(fields[2]);
    items.push_back(item);
  }
    cout << "Total items: " << items.size() << endl;
}

void one_customer_order() {
  cout << "Enter customer number: ";
  int customer_number;
  try {
    cin >> customer_number;
  } catch (...) {
    cout << "Invalid customer number" << endl;
    return;
  }
  for (const Customer& customer : customers) {
    if (customer.id == customer_number) {
      cout << "Welcome " << customer.name << endl;
      cout << "Enter item number (0 to quit)" << endl;
      double total = 0;
      vector<Item> customer_items;
      while (true) {
        int item_number;
        cin >> item_number;
        if (item_number == 0) {
          cout << "Thank you for shopping with us" << endl;
          cout << "Customer: " << customer.name << endl;
          cout << "Address: " << customer.address << ", " << customer.city << ", " << customer.state << " " << customer.zip << endl;
          cout << "Phone: " << customer.phone << endl;
          cout << "Email: " << customer.email << endl;
          cout << "Items: " << endl;
          for (const Item& item : customer_items) {
            cout << item.description << ": $" << item.price << endl;
          }
          cout << "Total Number of Items: " << customer_items.size() << endl;
          cout << "Total: $" << total << endl;
          return;
        }
        for (int i = 0; i < items.size(); i++) {
          if (items[i].id == item_number) {
            total += items[i].price;
            cout << "Added " << items[i].description << ": $" << items[i].price << endl << endl;
            customer_items.push_back(items[i]);
            //collect garbage
            cin.clear();
            cout << "Enter item number (0 to quit)" << endl;
            break;
          }
          if (i == items.size() - 1) {
            cout << "Item not found" << endl << endl;
            cout << "Enter item number (0 to quit)" << endl;
          }
        }
            
          
        
      }
  }
}
  cout << "Customer not found" << endl;
}



int main() {
  read_customers("customers.txt");
  read_items("items.txt");
  one_customer_order();
}