#include <iostream>
#include <string>
using namespace std;

// ===============================
// Structure for Account (Linked List Node)
// ===============================
struct Account {
    int accountNumber;
    string name;
    string pin;
    double balance;
    Account* next;
};

// Head pointer for Linked List
Account* head = NULL;

// ===============================
// Create Account Function
// ===============================
void createAccount() {
    Account* newAcc = new Account;

    cout << "\nEnter Account Number: ";
    cin >> newAcc->accountNumber;

    cout << "Enter Name: ";
    cin >> newAcc->name;

    cout << "Set PIN: ";
    cin >> newAcc->pin;

    cout << "Enter Initial Balance: ";
    cin >> newAcc->balance;

    newAcc->next = NULL;

    // Insert into Linked List
    if (head == NULL) {
        head = newAcc;
    } else {
        Account* temp = head;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newAcc;
    }

    cout << "\nAccount Created Successfully!\n";
}

// ===============================
// Login / Verify Account
// ===============================
Account* login() {
    int accNo;
    string pinInput;

    cout << "\nEnter Account Number: ";
    cin >> accNo;
    cout << "Enter PIN: ";
    cin >> pinInput;

    Account* temp = head;

    while (temp != NULL) {
        if (temp->accountNumber == accNo && temp->pin == pinInput) {
            cout << "\nLogin Successful! Welcome " << temp->name << "\n";
            return temp;
        }
        temp = temp->next;
    }

    cout << "\nInvalid Account Number or PIN!\n";
    return NULL;
}

// ===============================
// MAIN for Module-1 Only
// ===============================
int main() {
    int choice;
    Account* currentUser = nullptr;

    do {
        cout << "\n===== ACCOUNT MODULE =====\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            createAccount();
            break;

        case 2:
            currentUser = login();
            break;

        case 3:
            cout << "Exiting Account Module...\n";
            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while (choice != 3);

    return 0;
}
