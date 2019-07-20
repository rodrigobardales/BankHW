#include <iostream>
#include "bank.h"

int main()
{
    bank bank;
    std::cout << "~~~~~~~~~~~ Bank du Class ~~~~~~~~~~~~~" << std::endl;
    int op;
    while (op != 5)
    {
        cout << "\n\n";
        cout << "Insert option \n";
        cout << "1. Create account \n 2. Transacction \n 3. Print history file \n 4. View account\n 5. Exit\n";
        cin >> op;

        switch (op)
        {
        case 1:
            bank.assisst_create();
            break;
        case 2:
            bank.assisst_transaction();
            break;
        case 3:
            bank.read_history();
            break;
        case 4:
            cout << "Account number: \n";
            int account;
            cin >> account;
            bank.view_account(account);
            break;
        case 5:
            exit(0);
            break;

        default:
            cout << "Wrong option \n";
            break;
        }
    }
}