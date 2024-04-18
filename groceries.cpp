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

struct LineItem {
  int item_id;
  int qty;
  string description;
  double price;

  double sub_total() const {
    return price * qty;
  }
  
};

struct Payment {
  double amount;
  virtual string print_detail() const = 0;
  virtual ~Payment() {}

};

struct Credit : public Payment {
  string card_number;
  string expiration_date;
  string print_detail() const override {
    return "Paid by Credit Card: " + card_number + ", expires: " + expiration_date + "\n";
  }
};

struct PayPal : public Payment{
  string paypal_id;
  string print_detail() const override{
    return "Paid by PayPal ID: " + paypal_id + "\n";
  }
};

struct WireTransfer : public Payment{
  string bank_id;
  string account_id;
  string print_detail() const override {
    return "Paid by Wire Transfer from Bank ID " + bank_id + ", account #: " + account_id + "\n";
  }
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

  string print_detail() const {
    return "Customer ID #" + to_string(id) + ": " + "\n" + name + ", ph. " + phone + ", email: " + email + "\n" + address + "\n" + city + ", " + state + " " + zip;
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
    return total;
  }
  // ~Order() {
  //   delete payment;

  // }

string print_order(const vector<Customer>& customers) const {
    string order_str = "===========================\n";
    order_str += "Order #" + to_string(order_id) + ", Date: " + order_date + "\n";
    order_str += payment->print_detail() + "\n";

    for (const LineItem& item : line_items) {
        order_str += item.description + ": $" + to_string(item.price) + "\n";
    }

    // Find the customer associated with this order
    for (const Customer& customer : customers) {
        if (customer.id == cust_id) {
            order_str += customer.print_detail() + "\n";
            break;
        }
    }

    order_str += "Total: $" + to_string(total()) + "\n";
    return order_str;
}

  double total() const {
    double total = 0;
    for (const LineItem& item : line_items) {
      total += item.sub_total();
    }
    return total;
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

// Define Order, LineItem, and Payment structs/classes here

void read_orders(const string& filename) {
    ifstream file(filename);
    string line;

    // Read two lines at a time
    while (getline(file, line)) {
        // Read order information
        vector<string> fields = split(line, ',');
        Order order;
        order.cust_id = stoi(fields[0]);
        order.order_id = stoi(fields[1]);
        order.order_date = fields[2];
        cout << "Order ID: " << order.order_id << " information collected" << endl;
        for (int i = 3; i < fields.size(); i += 1) {
            // handle line items
            LineItem line_item;
            line_item.item_id = stoi(fields[i].substr(0, 5));
            line_item.qty = stoi(fields[i].substr(6));
            for (const Item& item : items) {
                if (item.id == line_item.item_id) {
                    line_item.description = item.description;
                    line_item.price = item.price;
                    break;
                }
            }
            order.line_items.push_back(line_item);
            cout << "Item ID: " + to_string(line_item.item_id) + ", Quantity: " + to_string(line_item.qty) << endl;
        }
        cout << "Order ID: " << order.order_id << " line items collected" << endl;

        // Read payment information
        if (getline(file, line)) {
            vector<string> payment_fields = split(line, ',');
            int payment_type = stoi(payment_fields[0]);
            if (payment_type == 1) {
                Credit* credit = new Credit;
                credit->card_number = payment_fields[1];
                credit->expiration_date = payment_fields[2];
                order.payment = credit;
            } else if (payment_type == 2) {
                PayPal* paypal = new PayPal;
                paypal->paypal_id = payment_fields[1];
                order.payment = paypal;
            } else if (payment_type == 3) {
                WireTransfer* wire_transfer = new WireTransfer;
                wire_transfer->bank_id = payment_fields[1];
                wire_transfer->account_id = payment_fields[2];
                order.payment = wire_transfer;
            }
            cout << "Order ID: " << order.order_id << " payment collected" << endl;
        } else {
            cerr << "Error: Incomplete data for order ID " << order.order_id << endl;
            break; // Exit loop if there's an error reading payment information
        }

        orders.push_back(order);
        cout << "Order ID: " << order.order_id << " Finished" << endl;
        //call the ~Order() destructor

    }

    cout << "Total orders: " << orders.size() << endl;
}



int main() {
  read_customers("customers.txt");
  read_items("items.txt");
  read_orders("orders.txt");

  ofstream ofs("order_report.txt");
  int i = 0;
  for (i = 0; i < orders.size(); i++) {
    
    ofs << orders[i].print_order(customers) << endl;
  }
}




//Elephant Code GRAVEYARD


// while (getline(file, line)) {
//     vector<string> fields = split(line, ',');
//     Order order;
//     order.cust_id = stoi(fields[0]);
//     order.order_id = stoi(fields[1]);
//     order.order_date = fields[2];
//     // cout << "Order ID: " << order.order_id << endl;
//     // cout << "Order Date: " << order.order_date << endl;
//     // cout << "Customer ID: " << order.cust_id << endl;
//     for (int i = 3; i < fields.size(); i += 1) {
//       // handle line items, the first 5 digits are the item id, and the last one is the quantity
//       LineItem line_item;
//       line_item.item_id = stoi(fields[i].substr(0, 5));
//       line_item.qty = stoi(fields[i].substr(5));
//       for (const Item& item : items) {
//         if (item.id == line_item.item_id) {
//           line_item.description = item.description;
//           line_item.price = item.price;
//           break;
//         }
//       }
//       order.line_items.push_back(line_item);
      
//       // cout << "Item ID: " + to_string(line_item.item_id) + ", Quantity: " + to_string(line_item.qty) << endl;
      
//     }