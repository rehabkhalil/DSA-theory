#include <iostream>
#include <windows.h>
#include <string>
#include <stack>
#include <queue>
#include <fstream>
#include <cctype>
using namespace std;

// For colors
void setColor(int textColor, int backgroundColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorAttribute = textColor + (backgroundColor << 4);
    SetConsoleTextAttribute(hConsole, colorAttribute);
}

#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_BLUE 9
#define COLOR_PURPLE 13
#define COLOR_BRIGHTYELLOW 14
#define COLOR_YELLOW 6
#define COLOR_AQUA 11
#define BACKGROUND_BLACK 0


// For safe input
template<typename T>
void safeInput(T &var) {
    cin >> var;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        setColor(COLOR_RED, BACKGROUND_BLACK);
        cout << "Invalid input! Enter again: ";
        cin >> var;
    }
    cin.ignore(1000,'\n'); 
}


// for name validation
bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char c : name)
        if (!isalpha(c) && c != ' ')
            return false;
    return true;
}


// for pin validation (4-digit)
bool isValidPin(const string& pin) {
    if (pin.length() != 4) return false;
    for (char c : pin)
        if (!isdigit(c))
            return false;
    return true;
}

// Structure
struct Account {
    int accountNumber;
    string name;
    string pin;
    double balance;
    Account* next;
};

Account* head = NULL;

// Data Structure
stack<string> transactionStack;
queue<string> operationQueue;

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

//save account
void saveAccounts() {
    ofstream file("accounts.txt", ios::trunc); // overwrite file
    if (!file) {
        cerr << "Error opening file for writing!" << endl;
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


//load account
void loadAccounts() {
    ifstream file("accounts.txt");
    if (!file) return;

    Account* last = nullptr;
    string line;

    while (getline(file, line)) {
        Account* acc = new Account;
        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);

        if (pos1 == string::npos || pos2 == string::npos || pos3 == string::npos) {
            cerr << "Invalid line format, skipping: " << line << endl;
            delete acc;
            continue;
        }

        try {
            acc->accountNumber = stoi(line.substr(0, pos1));
            acc->name = line.substr(pos1 + 1, pos2 - pos1 - 1);
            acc->pin = line.substr(pos2 + 1, pos3 - pos2 - 1);
            acc->balance = stod(line.substr(pos3 + 1));
        } catch (...) {
            cerr << "Failed to parse account, skipping: " << line << endl;
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

// Create Account
void createAccount() {
    Account* newAcc = new Account;

    newAcc->accountNumber = generateAccountNumber();
    cout << "\nGenerated Account Number: " << newAcc->accountNumber << endl;

    // Input Name
    do {
        cout << "Enter Name: ";
        getline(cin >> ws, newAcc->name);
        if (!isValidName(newAcc->name)) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Invalid name! Only letters allowed.\n";
        }
    } while (!isValidName(newAcc->name));

    // Input PIN
    do {
        cout << "Set 4-digit PIN: ";
        cin >> newAcc->pin;
        if (!isValidPin(newAcc->pin)) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Invalid PIN! Must be 4 digits.\n";
        }
    } while (!isValidPin(newAcc->pin));

    // Input Initial Balance
    do {
        cout << "Enter Initial Balance (>=0): ";
        safeInput(newAcc->balance);
        if (newAcc->balance < 0) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Balance cannot be negative!\n";
        }
    } while (newAcc->balance < 0);

    // Link to list
    newAcc->next = NULL;
    if (!head) head = newAcc;
    else {
        Account* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newAcc;
    }

    saveAccounts(); // Save after account creation

    setColor(COLOR_GREEN, BACKGROUND_BLACK);
    cout << "? Account Created Successfully!\n";
}

// login
Account* login() {
    int accNo;
    string pinInput;

    cout << "\nEnter Account Number: ";
    setColor(COLOR_GREEN, BACKGROUND_BLACK);
    safeInput(accNo);

    cout << "Enter PIN: ";
    cin >> pinInput;

    Account* temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accNo && temp->pin == pinInput) {
            setColor(COLOR_BLUE, BACKGROUND_BLACK);
            cout << "\n? Login Successful! Welcome " << temp->name << "\n";
            return temp;
        }
        temp = temp->next;
    }

    setColor(COLOR_RED, BACKGROUND_BLACK);
    cout << "\n? Invalid Account Number or PIN!\n";
    return NULL;
}


// Update Account
void updateAccount() {
    int acc;
    cout << "\nEnter Account Number to Update: ";
    setColor(COLOR_GREEN, BACKGROUND_BLACK);
    safeInput(acc);

    Account* temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == acc) {
            int ch;
            cout << "\n1. Update Name\n2. Update PIN\nEnter choice: ";
            safeInput(ch);

            if (ch == 1) {
                do {
                    cout << "Enter New Name: ";
                    getline(cin >> ws, temp->name);
                    if (!isValidName(temp->name)) {
                        setColor(COLOR_RED, BACKGROUND_BLACK);
                        cout << "Invalid name! Only letters allowed.\n";
                    }
                } while (!isValidName(temp->name));
                setColor(COLOR_GREEN, BACKGROUND_BLACK);
                cout << "? Name Updated Successfully!\n";
            } else if (ch == 2) {
                do {
                    cout << "Enter New 4-digit PIN: ";
                    cin >> temp->pin;
                    if (!isValidPin(temp->pin)) {
                        setColor(COLOR_RED, BACKGROUND_BLACK);
                        cout << "Invalid PIN! Must be 4 digits.\n";
                    }
                } while (!isValidPin(temp->pin));
                setColor(COLOR_BLUE, BACKGROUND_BLACK);
                cout << "? PIN Updated Successfully!\n";
            } else {
                setColor(COLOR_RED, BACKGROUND_BLACK);
                cout << "Invalid Option!\n";
            }
            return;
        }
        temp = temp->next;
    }
    setColor(COLOR_RED, BACKGROUND_BLACK);
    cout << "? Account Not Found!\n";
    saveAccounts();
}


void deleteAccount() {
    int acc;
    cout << "\nEnter Account Number to Delete: ";
    safeInput(acc);

    if (!head) {
        cout << "No accounts available!\n";
        return;
    }

    // Case 1: Delete first node
    if (head->accountNumber == acc) {
        Account* del = head;
        head = head->next;
        delete del;
        cout << "Account Deleted Successfully!\n";
        saveAccounts();     
        return;
    }

    // Case 2: Delete any other node
    Account* temp = head;
    while (temp->next != NULL) {
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

// View All Account
void viewAllAccounts() {
    cout << "\n===== ALL ACCOUNTS =====\n";
    if (!head) {
        setColor(COLOR_RED, BACKGROUND_BLACK);
        cout << "No accounts available.\n";
        return;
    }
    Account* temp = head;
    while (temp) {
        setColor(COLOR_GREEN, BACKGROUND_BLACK);
        cout << "\nAccount Number: " << temp->accountNumber;
        cout << "\nName: " << temp->name;
        cout << "\nBalance: " << temp->balance << " PKR\n";
        temp = temp->next;
    }
}

// Transaction
void checkBalance(Account* acc) {
    operationQueue.push("Check Balance");
    setColor(COLOR_AQUA, BACKGROUND_BLACK);
    cout << "\nCurrent Balance: " << acc->balance << " PKR\n";
}

void depositMoney(Account* acc) {
    double amount;
    do {
        cout << "\nEnter Amount to Deposit: ";
        safeInput(amount);
        if (amount <= 0) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Amount must be greater than 0!\n";
        }
    } while (amount <= 0);

    acc->balance += amount;
    operationQueue.push("Deposit");
    transactionStack.push("Deposited: " + to_string(amount) + " PKR");
    setColor(COLOR_AQUA, BACKGROUND_BLACK);
    cout << "? Deposit Successful! New Balance: " << acc->balance << " PKR\n";
    saveAccounts();
}

void withdrawMoney(Account* acc) {
    double amount;
    do {
        cout << "\nEnter Amount to Withdraw: ";
        safeInput(amount);
        if (amount <= 0) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Amount must be greater than 0!\n";
        } else if (amount > acc->balance) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Insufficient balance!\n";
            amount = -1; 
        }
    } while (amount <= 0);

    acc->balance -= amount;
    operationQueue.push("Withdraw");
    transactionStack.push("Withdrawn: " + to_string(amount) + " PKR");
    setColor(COLOR_AQUA, BACKGROUND_BLACK);
    cout << "? Withdrawal Successful! Remaining Balance: " << acc->balance << " PKR\n";
    saveAccounts();
}

// History Module
void viewTransactionHistory() {
    cout << "\n===== Transaction History =====\n";
    if (transactionStack.empty()) {
        setColor(COLOR_RED, BACKGROUND_BLACK);
        cout << "No transactions available.\n";
        return;
    }
    stack<string> tempStack = transactionStack;
    setColor(COLOR_PURPLE, BACKGROUND_BLACK);
    while (!tempStack.empty()) {
        cout << "- " << tempStack.top() << endl;
        tempStack.pop();
    }
}

void viewOperationQueue() {
    cout << "\n===== Operation Queue =====\n";
    if (operationQueue.empty()) {
        setColor(COLOR_RED, BACKGROUND_BLACK);
        cout << "No pending operations.\n";
        return;
    }
    queue<string> tempQueue = operationQueue;
    setColor(COLOR_PURPLE, BACKGROUND_BLACK);
    while (!tempQueue.empty()) {
        cout << "- " << tempQueue.front() << endl;
        tempQueue.pop();
    }
}


// main function
int main() {
	loadAccounts();
    int mainChoice;
    Account* currentUser = nullptr;

    do {
        setColor(COLOR_RED, BACKGROUND_BLACK);
        cout << "\n===== ATM MANAGEMENT SYSTEM =====\n";
        setColor(COLOR_BRIGHTYELLOW, BACKGROUND_BLACK);
        cout << "1. Account Module\n2. Transaction Module\n3. History Module\n4. Exit\n";
        cout << "Select a Module: ";
        safeInput(mainChoice);

        switch (mainChoice) {
            case 1: {
                int choice;
                setColor(COLOR_RED, BACKGROUND_BLACK);
                cout << "\n===== ACCOUNT MODULE =====\n";
                setColor(COLOR_GREEN, BACKGROUND_BLACK);
                cout << "1. Create Account\n2. Login\n3. Update Account\n4. Delete Account\n5. View All Accounts\n";
                cout << "Enter choice: ";
                safeInput(choice);

                if (choice == 1) createAccount();
                else if (choice == 2) currentUser = login();
                else if (choice == 3) updateAccount();
                else if (choice == 4) deleteAccount();
                else if (choice == 5) viewAllAccounts();
                else { setColor(COLOR_RED, BACKGROUND_BLACK); cout << "Invalid choice!\n"; }
                break;
            }
            case 2: {
                if (!currentUser) {
                    setColor(COLOR_YELLOW, BACKGROUND_BLACK);
                    cout << "? Please login first!\n";
                    break;
                }
                int choice;
                setColor(COLOR_RED, BACKGROUND_BLACK);
                cout << "\n===== TRANSACTION MODULE =====\n";
                setColor(COLOR_AQUA, BACKGROUND_BLACK);
                cout << "1. Check Balance\n2. Deposit Money\n3. Withdraw Money\n";
                cout << "Enter choice: ";
                safeInput(choice);

                if (choice == 1) checkBalance(currentUser);
                else if (choice == 2) depositMoney(currentUser);
                else if (choice == 3) withdrawMoney(currentUser);
                else { setColor(COLOR_RED, BACKGROUND_BLACK); cout << "Invalid choice!\n"; }
                break;
            }
            case 3: {
                int choice;
                setColor(COLOR_PURPLE, BACKGROUND_BLACK);
                cout << "\n===== HISTORY MODULE =====\n";
                cout << "1. View Transaction History\n2. View Operation Queue\n";
                cout << "Enter choice: ";
                safeInput(choice);

                if (choice == 1) viewTransactionHistory();
                else if (choice == 2) viewOperationQueue();
                else { setColor(COLOR_RED, BACKGROUND_BLACK); cout << "Invalid choice!\n"; }
                break;
            }
            case 4:
                setColor(COLOR_YELLOW, BACKGROUND_BLACK);
                cout << "\nThank you for using the ATM!\n";
                break;
            default:
                setColor(COLOR_RED, BACKGROUND_BLACK);
                cout << "Invalid Module choice!\n";
        }
    } while (mainChoice != 4);

    return 0;
}