#include "bank.h"

bank::bank() {}

void bank::create_account(int account, char name[25], float balance)
{
    ofstream accountsdat("cuentas.dat", ios::out | ios::app | ios::binary);

    if (!accountsdat)
    {
        cout << "Error with file \n";
        return;
    }

    accounts new_account;
    new_account.accountnumber = account;
    strcpy(new_account.name, name);
    new_account.balance = balance;

    accountsdat.seekp(0, ios::end);
    accountsdat.write(reinterpret_cast<const char *>(&new_account), sizeof(accounts));

    accountsdat.close();
}

bool bank::find_account(int searching)
{
    ifstream accountsdat("cuentas.dat", ios::in | ios::binary);

    if (!accountsdat)
    {
        cout << "Error with file \n";
        return false;
    }

    accounts reading;

    accountsdat.seekg(0, ios::beg);
    accountsdat.read(reinterpret_cast<char *>(&reading), sizeof(accounts));

    while (!accountsdat.eof())
    {
        if (reading.accountnumber == searching)
        {
            accountsdat.close();
            return true;
        }
        accountsdat.read(reinterpret_cast<char *>(&reading), sizeof(accounts));
    }
    accountsdat.close();
    return false;
}

void bank::write_history(int account, char transaction, float amount)
{
    ofstream historyfile("history.dat", ios::out | ios::app | ios::binary);

    if (!historyfile)
    {
        cout << "Error with file \n";
        return;
    }

    history writing;
    writing.accountnumber = account;
    writing.transaction = transaction;
    writing.amount = amount;

    historyfile.seekp(0, ios::end);
    historyfile.write(reinterpret_cast<const char *>(&writing), sizeof(history));

    historyfile.close();
}

void bank::read_history()
{
    ifstream historyfile("history.dat", ios::in | ios::binary);

    if (!historyfile)
    {
        cout << "Error with file \n";
        return;
    }

    history temp;

    historyfile.seekg(0, ios::beg);
    historyfile.read(reinterpret_cast<char *>(&temp), sizeof(history));

    while (!historyfile.eof())
    {
        cout << "Account: " << temp.accountnumber << " Transaction: "
             << temp.transaction << " amount: " << temp.amount << "\n";
        historyfile.read(reinterpret_cast<char *>(&temp), sizeof(history));
    }
    historyfile.close();
}

float bank::check_balance(int account_number)
{
    ifstream accountfiles("cuentas.dat", ios::in | ios::binary);

    accounts tmp;
    accountfiles.seekg(0, ios::beg);
    accountfiles.read(reinterpret_cast<char *>(&tmp), sizeof(accounts));

    while (!accountfiles.eof())
    {
        if (tmp.accountnumber == account_number)
        {
            accountfiles.close();
            return tmp.balance;
        }
        accountfiles.read(reinterpret_cast<char *>(&tmp), sizeof(accounts));
    }
    accountfiles.close();
    return -1;
}

bool bank::change_balance(int account_number, float amount)
{

    fstream accountsfile("cuentas.dat", ios::in | ios::out | ios::binary);
    if (!accountsfile)
    {
        cout << "Error with file \n";
        return false;
    }
    accounts reading;
    accountsfile.seekg(0, ios::beg);
    accountsfile.read(reinterpret_cast<char *>(&reading), sizeof(accounts));

    while (!accountsfile.eof())
    {
        if (reading.accountnumber == account_number)
        {
            reading.balance = reading.balance + amount;
            int current_pos = accountsfile.tellp();
            accountsfile.seekp(current_pos - sizeof(accounts), ios::beg);
            accountsfile.write(reinterpret_cast<char *>(&reading), sizeof(accounts));
            accountsfile.close();
            return true;
        }
        accountsfile.read(reinterpret_cast<char *>(&reading), sizeof(accounts));
    }
    accountsfile.close();
    return false;
}

bool bank::transaction(int account, char tran, float amount)
{
    if (find_account(account) == false)
    {
        cout << "Account not found! \n";
        return false;
    }

    switch (tran)
    {
    case 'd':
        //deposito
        if (change_balance(account, amount) == true)
        {
            write_history(account, 'd', amount);
            cout << "Transaction accomplished \n";
        }
        else
        {
            cout << "Error with transacction \n";
        }
        break;
    case 'r':
        //retiro
        if (check_balance(account) > amount)
        {
            if (change_balance(account, amount * -1) == true)
            {
                write_history(account, 'r', amount * -1);
                cout << "Transaction accomplished \n";
            }
            else
            {
                cout << "Not enough balance \n";
            }
        }
        break;
    case 'p':
        //pago cuenta a cuenta
        cout << "Paying to? \n";
        int pagando;
        cin >> pagando;
        if (find_account(pagando) == true)
        {
            if (check_balance(account) > amount)
            {
                float sub = amount * -1;
                if (change_balance(account, sub) == true && change_balance(pagando, amount) == true)
                {
                    write_history(account, 'r', sub);
                    write_history(pagando, 'd', amount);
                    cout << "Transaction accomplished \n";
                }
            }
        }
        break;

    default:
        cout << "Invalid option \n";
        break;
    }
}

void bank::view_account(int account)
{
    if (find_account(account == false))
    {
        cout << "Non existing account \n";
        return;
    }
    ifstream accountfiles("cuentas.dat", ios::in | ios::binary);

    accounts tmp;
    accountfiles.seekg(0, ios::beg);
    accountfiles.read(reinterpret_cast<char *>(&tmp), sizeof(accounts));

    while (!accountfiles.eof())
    {
        if (tmp.accountnumber == account)
        {
            cout << "Name: " << tmp.name << " balance: " << tmp.balance << endl;
            accountfiles.close();
            return;
        }
        accountfiles.read(reinterpret_cast<char *>(&tmp), sizeof(accounts));
    }
    accountfiles.close();
}

void bank::assisst_create()
{
    int account;
    char name[25];
    cout << "Insert account number \n";
    cin >> account;
    cout << "Insert name \n";
    cin >> name;
    create_account(account, name, 0);
}

void bank::assisst_transaction()
{
    int account;
    cout << "Account number \n";
    cin >> account;
    char transacction;
    cout << "Transaction type \n r - retiro d - deposito p - pago cuenta a cuenta \n";
    cin >> transacction;
    float amount;
    cout << "Inserte amount of transaction \n";
    cin >> amount;
    transaction(account, transacction, amount);
}