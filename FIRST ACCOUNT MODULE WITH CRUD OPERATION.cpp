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
// Create Account (C)
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
// Search / Read Account (R)
// ===============================
void searchAccount() {
    int acc;
    cout << "\nEnter Account Number to Search: ";
    cin >> acc;

    Account* temp = head;

    while (temp != NULL) {
        if (temp->accountNumber == acc) {
            cout << "\nAccount Found!\n";
            cout << "Name: " << temp->name << endl;
            cout << "PIN: " << temp->pin << endl;
            cout << "Balance: " << temp->balance << endl;
            return;
        }
        temp = temp->next;
    }

    cout << "Account Not Found!\n";
}

// ===============================
// Update Account (U)
// ===============================
void updateAccount() {
    int acc;
    cout << "\nEnter Account Number to Update: ";
    cin >> acc;

    Account* temp = head;

    while (temp != NULL) {
        if (temp->accountNumber == acc) {

            int updateChoice;
            cout << "\nWhat do you want to update?\n";
            cout << "1. Name\n";
            cout << "2. PIN\n";
            cout << "3. Balance\n";
            cout << "Enter choice: ";
            cin >> updateChoice;

            switch (updateChoice) {
                case 1:
                    cout << "Enter New Name: ";
                    cin >> temp->name;
                    break;

                case 2:
                    cout << "Enter New PIN: ";
                    cin >> temp->pin;
                    break;

                case 3:
                    cout << "Enter New Balance: ";
                    cin >> temp->balance;
                    break;

                default:
                    cout << "Invalid choice!\n";
            }

            cout << "Account Updated Successfully!\n";
            return;
        }
        temp = temp->next;
    }

    cout << "Account Not Found!\n";
}

// ===============================
// Delete Account (D)
// ===============================
void deleteAccount() {
    int acc;
    cout << "\nEnter Account Number to Delete: ";
    cin >> acc;

    if (head == NULL) {
        cout << "No accounts in system!\n";
        return;
    }

    // If first node is to be deleted
    if (head->accountNumber == acc) {
        Account* del = head;
        head = head->next;
        delete del;
        cout << "Account Deleted Successfully!\n";
        return;
    }

    Account* temp = head;
    Account* prev = NULL;

    while (temp != NULL) {
        if (temp->accountNumber == acc) {
            prev->next = temp->next;
            delete temp;
            cout << "Account Deleted Successfully!\n";
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    cout << "Account Not Found!\n";
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
        cout << "\n===== ACCOUNT MODULE (CRUD) =====\n";
        cout << "1. Create Account\n";
        cout << "2. Search Account\n";
        cout << "3. Update Account\n";
        cout << "4. Delete Account\n";
        cout << "5. Login\n";
        cout << "6. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            createAccount();
            break;

        case 2:
            searchAccount();
            break;

        case 3:
            updateAccount();
            break;

        case 4:
            deleteAccount();
            break;

        case 5:
            currentUser = login();
            break;

        case 6:
            cout << "Exiting Account Module...\n";
            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while (choice != 6);

    return 0;
}
