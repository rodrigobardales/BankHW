
#ifndef BANK_H
#define BANK_H

#include "structs.h"
#include <iostream>
#include <fstream>

using namespace std;

class bank
{
private:
    bool find_account(int);
    float check_balance(int);
    bool change_balance(int, float);
    void write_history(int, char, float);

public:
    bank();
    void create_account(int, char[], float);
    bool transaction(int, char, float);
    void read_history();
    void view_account(int);

    void assisst_create();
    void assisst_transaction();
};

#endif