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

  string print_detail() {
    return "Customer IT #" + to_string(id) + ": " + "\n" + name + ", ph. " + phone + ", email: " + email + "\n" + address + "\n" + city + ", " + state + " " + zip;
  }
};

struct Order {

  int order_id;
  string order_date;
  int cust_id;
  vector<LineItem> line_items;
  Payment* payment;

  double total() {
    int total = 0;
    for (const LineItem& item : line_items) {
      total += item.sub_total();
    }
  }
  ~Order() {
    delete payment;
  }

  string print_order() {

    string order_str = "===========================";
    order_str += "Order #" + to_string(order_id) + ", Date: " + to_string(order_date) + "\n";
    order_str += payment->print_detail() + "\n";
    order_str += customers[cust_id].print_detail() + "\n";

    for (const LineItem& item : line_items) {
      order_str += item.description + ": $" + to_string(item.price) + "\n";
    }
    order_str += "Total: $" + to_string(total()) + "\n";
    return order_str;
  }
};

struct LineItem {
  int item_id;
  int qty;
  double sub_total() {
    return qty * items[item_id].price;
  
  }
};

struct Payment {
  double amount;
  string print_detail(int payment_type) {
    string payment_str = "Payment: $" + to_string(amount) + "\n";
    if (payment_type == 1) {
      payment_str += ((Credit*)this)->print_detail();
    } else if (payment_type == 2) {
      payment_str += ((PayPal*)this)->print_detail();
    } else if (payment_type == 3) {
      payment_str += ((WireTransfer*)this)->print_detail();

  }
  return payment_str;
  }

};

struct Credit {
  string card_number;
  string expiration;
  string print_detail() {
    return "Paid by Credit card: " + card_number + ", exp. " + expiration + "\n";
  }
};

struct PayPal {
  string paypal_id;
  string print_detail() {
    return "Paid by PayPal ID: " + paypal_id + "\n";
  
}
};

struct WireTransfer {
  string bank_id;
  string account_id;
  string print_detail() {
    return "Paid by Wire Transfer from Bank ID " + bank_id + ", account #: " + account_id + "\n";
  }
};


vector<Item> items;

vector<Customer> customers;

vector<Order> orders;

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

void read_orders(const string& filename) {
  ifstream file(filename);
  string line;
  while (getline(file, line)) {
    vector<string> fields = split(line, ',');
    Order order;
    order.order_id = stoi(fields[1]);
    order.order_date = fields[2];
    order.cust_id = stoi(fields[0]);
    for (int i = 3; i < fields.size(); i += 1) {
      // Handle the line item, the int before the - is the item id and the int after the - is the quantity
      vector<string> line_item = split(fields[i], '-');
      LineItem item;
      item.item_id = stoi(line_item[0]);
      item.qty = stoi(line_item[1]);
      
    }
    orders.push_back(order);
  }
  cout << "Total orders: " << orders.size() << endl;
}

int main() {
  read_customers("customers.txt");
  read_items("items.txt");
  read_orders("orders.txt");

  ofstream ofs("order_report.txt");
  for (const auto& order : orders) {
    ofs << order.print_order() << endl;
  }
}