// Author: Jareth Franco
// Date: October 5, 2026
// Purpose: Declare an owning bank account with deep copies and overloaded operators.

#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>

class BankAccount {
private:
    std::string accountNumber;
    std::string* accountHolderName; // Owned allocation; never shared by copies.
    double balance;

public:
    BankAccount();
    BankAccount(const std::string& number, const std::string& name,
                double initialBalance);
    BankAccount(const BankAccount& other);
    BankAccount& operator=(const BankAccount& other);
    ~BankAccount();

    BankAccount& operator+=(double amount);
    BankAccount& operator-=(double amount);
    bool operator==(const BankAccount& other) const;
    bool operator<(const BankAccount& other) const;
    bool operator>(const BankAccount& other) const;

    static void printAccount(const BankAccount& account);
    static BankAccount createAccountFromInput();

    std::string getAccountNumber() const;
    std::string getAccountHolderName() const;
    double getBalance() const;
    bool setAccountHolderName(const std::string& name);
    bool deposit(double amount);
    bool withdraw(double amount);
};

#endif
