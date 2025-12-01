#include <iostream>
#include <stack>
#include <queue>
#include <string>
using namespace std;

// Stacks & Queue for history
stack<string> transactionStack;
queue<string> operationQueue;

// ===============================
// View Transaction History
// ===============================
void viewTransactionHistory() {
    cout << "\n===== Transaction History =====\n";

    if (transactionStack.empty()) {
        cout << "No transactions available.\n";
        return;
    }

    stack<string> temp = transactionStack;
    while (!temp.empty()) {
        cout << "- " << temp.top() << endl;
        temp.pop();
    }
}

// ===============================
// View Operation Queue
// ===============================
void viewOperationQueue() {
    cout << "\n===== Operation Queue =====\n";

    if (operationQueue.empty()) {
        cout << "No pending operations.\n";
        return;
    }

    queue<string> temp = operationQueue;
    while (!temp.empty()) {
        cout << "- " << temp.front() << endl;
        temp.pop();
    }
}

// ===============================
// MAIN (Only History Module)
// ===============================
int main() {

    // Dummy data for testing (you can remove or keep)
    transactionStack.push("Deposited: 1000 PKR");
    transactionStack.push("Withdrawn: 500 PKR");

    operationQueue.push("Check Balance");
    operationQueue.push("Deposit Request");

    int choice;

    do {
        cout << "\n===== HISTORY MODULE =====\n";
        cout << "1. View Transaction History\n";
        cout << "2. View Operation Queue\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                viewTransactionHistory();
                break;

            case 2:
                viewOperationQueue();
                break;

            case 3:
                cout << "Exiting History Module...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 3);

    return 0;
}

