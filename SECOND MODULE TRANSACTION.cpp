#include <iostream>
#include <string>
#include <stack>
#include <queue>
using namespace std;

//  Data Structures
stack<string> transactionStack;
queue<string> operationQueue;

// For Safe Input
template<typename T>
void safeInput(T &var) {
    cin >> var;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Enter again: ";
        cin >> var;
    }
}

struct Account {
    int accountNumber;
    string name;
    string pin;
    double balance;
};

// Check Balance
void checkBalance(Account* acc) {
    operationQueue.push("Check Balance");
    cout << "\nCurrent Balance: " << acc->balance << " PKR\n";
}

// Deposit Money
void depositMoney(Account* acc) {
    double amount;
    cout << "\nEnter Amount to Deposit: ";
    safeInput(amount);

    if (amount <= 0) {
        cout << "Invalid amount! Must be greater than 0.\n";
        return;
    }

    acc->balance += amount;
    operationQueue.push("Deposit");
    transactionStack.push("Deposited: " + to_string(amount) + " PKR");

    cout << "Deposit Successful! New Balance: " << acc->balance << " PKR\n";
}
// Withdraw Money
void withdrawMoney(Account* acc) {
    double amount;
    cout << "\nEnter Amount to Withdraw: ";
    safeInput(amount);

    if (amount <= 0) {
        cout << "Invalid amount! Must be greater than 0.\n";
        return;
    }

    if (amount > acc->balance) {
        cout << "Insufficient Balance!\n";
        return;
    }

    acc->balance -= amount;
    operationQueue.push("Withdraw");
    transactionStack.push("Withdrawn: " + to_string(amount) + " PKR");

    cout << "Withdrawal Successful! Remaining Balance: " << acc->balance << " PKR\n";
}


//Main function
int main() {
    //Random module for testing transaction
    Account user = {12345, "TestUser", "0000", 5000.0};

    int choice;
    do {
        cout << "\n===== TRANSACTION MODULE =====\n";
        cout << "1. Check Balance\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        safeInput(choice);

        switch (choice) {
            case 1: checkBalance(&user); break;
            case 2: depositMoney(&user); break;
            case 3: withdrawMoney(&user); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
