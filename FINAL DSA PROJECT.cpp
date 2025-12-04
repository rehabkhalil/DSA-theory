#include <iostream>
#include <windows.h>
#include <string>
#include <stack>
#include <queue>
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


// Create Account
void createAccount() {
    Account* newAcc = new Account;

    do {
        cout << "\nEnter Account Number (positive integer): ";
        setColor(COLOR_GREEN, BACKGROUND_BLACK);
        safeInput(newAcc->accountNumber);
    } while (newAcc->accountNumber <= 0);

    do {
        cout << "Enter Name: ";
        getline(cin >> ws, newAcc->name);
        if (!isValidName(newAcc->name)) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Invalid name! Only letters allowed.\n";
        }
    } while (!isValidName(newAcc->name));

    do {
        cout << "Set 4-digit PIN: ";
        cin >> newAcc->pin;
        if (!isValidPin(newAcc->pin)) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Invalid PIN! Must be 4 digits.\n";
        }
    } while (!isValidPin(newAcc->pin));

    do {
        cout << "Enter Initial Balance (>=0): ";
        safeInput(newAcc->balance);
        if (newAcc->balance < 0) {
            setColor(COLOR_RED, BACKGROUND_BLACK);
            cout << "Balance cannot be negative!\n";
        }
    } while (newAcc->balance < 0);

    newAcc->next = NULL;
    if (head == NULL) head = newAcc;
    else {
        Account* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newAcc;
    }

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
}


// Delete Account
void deleteAccount() {
    int acc;
    cout << "\nEnter Account Number to Delete: ";
    setColor(COLOR_GREEN, BACKGROUND_BLACK);
    safeInput(acc);

    if (!head) {
        setColor(COLOR_RED, BACKGROUND_BLACK);
        cout << "No accounts available!\n";
        return;
    }

    if (head->accountNumber == acc) {
        Account* del = head;
        head = head->next;
        delete del;
        setColor(COLOR_BLUE, BACKGROUND_BLACK);
        cout << "? Account Deleted Successfully!\n";
        return;
    }

    Account* temp = head;
    while (temp->next != NULL) {
        if (temp->next->accountNumber == acc) {
            Account* del = temp->next;
            temp->next = temp->next->next;
            delete del;
            setColor(COLOR_BLUE, BACKGROUND_BLACK);
            cout << "? Account Deleted Successfully!\n";
            return;
        }
        temp = temp->next;
    }

    setColor(COLOR_RED, BACKGROUND_BLACK);
    cout << "? Account Not Found!\n";
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
            amount = -1; // force repeat
        }
    } while (amount <= 0);

    acc->balance -= amount;
    operationQueue.push("Withdraw");
    transactionStack.push("Withdrawn: " + to_string(amount) + " PKR");
    setColor(COLOR_AQUA, BACKGROUND_BLACK);
    cout << "? Withdrawal Successful! Remaining Balance: " << acc->balance << " PKR\n";
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
