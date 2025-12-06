#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// Account structure
struct Account {
    int accountNumber;
    string name;
    string pin;
    double balance;
    Account* next;
};

Account* head = nullptr;

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

// Save accounts to file
void saveAccounts() {
    ofstream file("accounts.txt", ios::trunc);
    if (!file) {
        cerr << "Error opening file!\n";
        return;
    }
    Account* temp = head;
    while (temp) {
        file << temp->accountNumber << "|"
             << temp->name << "|"
             << temp->pin << "|"
             << temp->balance << "\n";
        temp = temp->next;
    }
    file.close();
}

// Login existing account
Account* login() {
    int accNo;
    string pinInput;
    cout << "Enter Account Number for login: ";
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

// Check balance
void checkBalance(Account* acc) {
    cout << "Current Balance: " << acc->balance << "\n";
}

// Deposit money
void depositMoney(Account* acc) {
    double amount;
    do {
        cout << "Enter Amount to Deposit: ";
        safeInput(amount);
        if (amount <= 0) cout << "Amount must be greater than 0!\n";
    } while (amount <= 0);

    acc->balance += amount;
    cout << "Deposit Successful! New Balance: " << acc->balance << "\n";
    saveAccounts();
}

// Withdraw money
void withdrawMoney(Account* acc) {
    double amount;
    do {
        cout << "Enter Amount to Withdraw: ";
        safeInput(amount);
        if (amount <= 0) cout << "Amount must be greater than 0!\n";
        else if (amount > acc->balance) {
            cout << "Insufficient balance!\n";
            amount = -1;
        }
    } while (amount <= 0);

    acc->balance -= amount;
    cout << "Withdrawal Successful! Remaining Balance: " << acc->balance << "\n";
    saveAccounts();
}

// Main function
int main() {
    loadAccounts();
    Account* currentUser = login();
    if (!currentUser) return 0;

    int choice;
    do {
        cout << "\n===== TRANSACTION MODULE =====\n";
        cout << "1. Check Balance\n2. Deposit Money\n3. Withdraw Money\n4. Exit\n";
        cout << "Enter choice: ";
        safeInput(choice);

        switch (choice) {
            case 1: checkBalance(currentUser); break;
            case 2: depositMoney(currentUser); break;
            case 3: withdrawMoney(currentUser); break;
            case 4: cout << "Exiting Transaction Module...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
