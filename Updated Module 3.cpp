#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <string>
using namespace std;

// Account structure (minimal for login)
struct Account {
    int accountNumber;
    string name;
    string pin;
    double balance;
    Account* next;
};

Account* head = nullptr;

// Data structures for history
stack<string> transactionStack;
queue<string> operationQueue;

// Safe input
template<typename T>
void safeInput(T &var) {
    cin >> var;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Enter again: ";
        cin >> var;
    }
    cin.ignore(1000, '\n');
}

// Load accounts from file
void loadAccounts() {
    ifstream file("accounts.txt");
    if (!file) return;

    string line;
    Account* last = nullptr;

    while (getline(file, line)) {
        Account* acc = new Account;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) {
            delete acc;
            continue;
        }

        try {
            acc->accountNumber = stoi(line.substr(0, pos1));
            acc->name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            acc->pin = line.substr(pos2 + 1, pos3 - pos2 - 1);
            acc->balance = stod(line.substr(pos3 + 1));
        } catch (...) {
            delete acc;
            continue;
        }

        acc->next = nullptr;
        if (!head) head = acc;
        else last->next = acc;
        last = acc;
    }
    file.close();
}

// Login
Account* login() {
    int accNo;
    string pinInput;
    cout << "Enter Account Number: ";
    safeInput(accNo);
    cout << "Enter PIN: ";
    cin >> pinInput;

    Account* temp = head;
    while (temp) {
        if (temp->accountNumber == accNo && temp->pin == pinInput) {
            cout << "Login Successful! Welcome " << temp->name << "\n";
            return temp;
        }
        temp = temp->next;
    }
    cout << "Invalid Account Number or PIN!\n";
    return nullptr;
}

// Log operations manually (simulate previous module)
void simulateOperations() {
    operationQueue.push("Check Balance");
    transactionStack.push("Checked Balance");

    operationQueue.push("Deposit");
    transactionStack.push("Deposited: 500 PKR");

    operationQueue.push("Withdraw");
    transactionStack.push("Withdrawn: 200 PKR");
}

// View transaction history (stack)
void viewTransactionHistory() {
    cout << "\n===== Transaction History =====\n";
    if (transactionStack.empty()) {
        cout << "No transactions available.\n";
        return;
    }
    stack<string> tempStack = transactionStack;
    while (!tempStack.empty()) {
        cout << "- " << tempStack.top() << "\n";
        tempStack.pop();
    }
}

// View operation queue
void viewOperationQueue() {
    cout << "\n===== Operation Queue =====\n";
    if (operationQueue.empty()) {
        cout << "No pending operations.\n";
        return;
    }
    queue<string> tempQueue = operationQueue;
    while (!tempQueue.empty()) {
        cout << "- " << tempQueue.front() << "\n";
        tempQueue.pop();
    }
}

// Main function
int main() {
    loadAccounts();
    Account* currentUser = login();
    if (!currentUser) return 0;

    // For demo purpose, simulate some operations
    simulateOperations();

    int choice;
    do {
        cout << "\n===== HISTORY MODULE =====\n";
        cout << "1. View Transaction History\n2. View Operation Queue\n3. Exit\n";
        cout << "Enter choice: ";
        safeInput(choice);

        switch (choice) {
            case 1: viewTransactionHistory(); break;
            case 2: viewOperationQueue(); break;
            case 3: cout << "Exiting History Module...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);

    return 0;
}
