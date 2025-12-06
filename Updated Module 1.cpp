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

// Generate unique account number
int generateAccountNumber() {
    int maxNum = 1000;
    Account* temp = head;
    while (temp) {
        if (temp->accountNumber > maxNum)
            maxNum = temp->accountNumber;
        temp = temp->next;
    }
    return maxNum + 1;
}

// Validate name (only letters and spaces)
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name)
        if (!isalpha(c) && c != ' ')
            return false;
    return true;
}

// Validate 4-digit PIN
bool isValidPin(const string& pin) {
    if (pin.length() != 4) return false;
    for (char c : pin)
        if (!isdigit(c)) return false;
    return true;
}

// Safe input for numbers
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

// Create new account
void createAccount() {
    Account* newAcc = new Account;
    newAcc->accountNumber = generateAccountNumber();
    cout << "Generated Account Number: " << newAcc->accountNumber << endl;

    do {
        cout << "Enter Name: ";
        getline(cin >> ws, newAcc->name);
        if (!isValidName(newAcc->name))
            cout << "Invalid name! Only letters allowed.\n";
    } while (!isValidName(newAcc->name));

    do {
        cout << "Set 4-digit PIN: ";
        cin >> newAcc->pin;
        if (!isValidPin(newAcc->pin))
            cout << "Invalid PIN! Must be 4 digits.\n";
    } while (!isValidPin(newAcc->pin));

    do {
        cout << "Enter Initial Balance: ";
        safeInput(newAcc->balance);
        if (newAcc->balance < 0)
            cout << "Balance cannot be negative!\n";
    } while (newAcc->balance < 0);

    newAcc->next = nullptr;
    if (!head) head = newAcc;
    else {
        Account* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newAcc;
    }

    saveAccounts();
    cout << "Account Created Successfully!\n";
}

// Login to existing account
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

// View all accounts
void viewAllAccounts() {
    if (!head) {
        cout << "No accounts available.\n";
        return;
    }
    Account* temp = head;
    cout << "\n--- ALL ACCOUNTS ---\n";
    while (temp) {
        cout << "Account Number: " << temp->accountNumber << "\n";
        cout << "Name: " << temp->name << "\n";
        cout << "Balance: " << temp->balance << "\n\n";
        temp = temp->next;
    }
}

// Update account (name or PIN)
void updateAccount() {
    int acc;
    cout << "Enter Account Number to Update: ";
    safeInput(acc);
    Account* temp = head;
    while (temp) {
        if (temp->accountNumber == acc) {
            int choice;
            cout << "1. Update Name\n2. Update PIN\nEnter choice: ";
            safeInput(choice);
            if (choice == 1) {
                do {
                    cout << "Enter New Name: ";
                    getline(cin >> ws, temp->name);
                    if (!isValidName(temp->name))
                        cout << "Invalid name! Only letters allowed.\n";
                } while (!isValidName(temp->name));
                cout << "Name Updated Successfully!\n";
            } else if (choice == 2) {
                do {
                    cout << "Enter New 4-digit PIN: ";
                    cin >> temp->pin;
                    if (!isValidPin(temp->pin))
                        cout << "Invalid PIN! Must be 4 digits.\n";
                } while (!isValidPin(temp->pin));
                cout << "PIN Updated Successfully!\n";
            } else {
                cout << "Invalid choice!\n";
            }
            saveAccounts();
            return;
        }
        temp = temp->next;
    }
    cout << "Account Not Found!\n";
}

// Delete account
void deleteAccount() {
    int acc;
    cout << "Enter Account Number to Delete: ";
    safeInput(acc);
    if (!head) {
        cout << "No accounts available!\n";
        return;
    }

    // Delete first node
    if (head->accountNumber == acc) {
        Account* del = head;
        head = head->next;
        delete del;
        cout << "Account Deleted Successfully!\n";
        saveAccounts();
        return;
    }

    // Delete any other node
    Account* temp = head;
    while (temp->next) {
        if (temp->next->accountNumber == acc) {
            Account* del = temp->next;
            temp->next = temp->next->next;
            delete del;
            cout << "Account Deleted Successfully!\n";
            saveAccounts();
            return;
        }
        temp = temp->next;
    }

    cout << "Account Not Found!\n";
}

// Main function
int main() {
    loadAccounts();
    int choice;
    do {
        cout << "\n===== ACCOUNT MODULE =====\n";
        cout << "1. Create Account\n2. Login\n3. View All Accounts\n4. Update Account\n5. Delete Account\n6. Exit\n";
        cout << "Enter choice: ";
        safeInput(choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: login(); break;
            case 3: viewAllAccounts(); break;
            case 4: updateAccount(); break;
            case 5: deleteAccount(); break;
            case 6: cout << "Exiting Module 1...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
