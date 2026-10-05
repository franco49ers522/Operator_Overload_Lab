// Author: Jareth Franco
// Date: October 5, 2026
// Purpose: Demonstrate overloaded operators, static utilities, and the Rule of Three.

#include "BankAccount.h"
#include "Input.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int findAccount(const std::vector<BankAccount>& accounts, const std::string& number) {
    for (std::size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getAccountNumber() == number) return static_cast<int>(i);
    }
    return -1;
}

// Local copies deliberately retain the account number; they are not added to the list.
void demonstrateCopies(const BankAccount& original) {
    BankAccount copied(original);             // Copy constructor.
    BankAccount assigned;                    // Existing destination object.
    assigned = original;                     // Copy assignment operator.
    copied.setAccountHolderName("Copy constructor demonstration");
    assigned.setAccountHolderName("Copy assignment demonstration");
    if (original.getBalance() > 0.0) copied -= original.getBalance();
    assigned = assigned;                     // Self-assignment must be safe.
    std::cout << "Original (unchanged):\n";
    BankAccount::printAccount(original);
    std::cout << "Independent copy-constructed object:\n";
    BankAccount::printAccount(copied);
    std::cout << "Independent copy-assigned object:\n";
    BankAccount::printAccount(assigned);
    std::cout << "Copies keep the same account number: "
              << ((original == copied && original == assigned) ? "true" : "false")
              << "\nLocal copies are destroyed when this demonstration ends.\n";
}

int main() {
    std::vector<BankAccount> accounts;
    std::string choice;
    std::cout << "Operator Overload Lab - Jareth Franco\n";
    while (true) {
        std::cout << "\nBank Account Management\n"
                  << "1. Create account\n2. List accounts\n3. Deposit (+=)\n"
                  << "4. Withdraw (-=)\n5. Change account holder name\n"
                  << "6. Compare accounts (==, <, >)\n"
                  << "7. Demonstrate deep copies\n0. Exit\n";
        if (!Input::readText("Choose an option: ", choice) || choice == "0") break;
        try {
            if (choice == "1") {
                BankAccount account = BankAccount::createAccountFromInput();
                if (findAccount(accounts, account.getAccountNumber()) != -1) {
                    std::cout << "That account number already exists.\n";
                } else {
                    accounts.push_back(account);
                    std::cout << "Account created.\n";
                }
            } else if (choice == "2") {
                if (accounts.empty()) std::cout << "No accounts available.\n";
                for (const BankAccount& account : accounts) BankAccount::printAccount(account);
            } else if (choice == "3" || choice == "4" || choice == "5" ||
                       choice == "6" || choice == "7") {
                if (accounts.empty()) {
                    std::cout << "No accounts available.\n";
                    continue;
                }
                std::string number;
                if (!Input::readText("Account number: ", number)) break;
                const int index = findAccount(accounts, number);
                if (index == -1) {
                    std::cout << "Account not found.\n";
                    continue;
                }
                BankAccount& account = accounts[index];
                if (choice == "5") {
                    std::string name;
                    if (!Input::readText("New account holder name: ", name)) break;
                    account.setAccountHolderName(name);
                    std::cout << "Name updated.\n";
                } else if (choice == "6") {
                    if (!Input::readText("Second account number: ", number)) break;
                    const int second = findAccount(accounts, number);
                    if (second == -1) {
                        std::cout << "Account not found.\n";
                        continue;
                    }
                    const BankAccount& other = accounts[second];
                    std::cout << "Same account number (==): " << (account == other ? "true" : "false")
                              << "\nFirst balance is less (<): " << (account < other ? "true" : "false")
                              << "\nFirst balance is greater (>): " << (account > other ? "true" : "false") << '\n';
                    BankAccount::printAccount(other);
                } else if (choice == "7") {
                    demonstrateCopies(account);
                } else {
                    double amount;
                    if (!Input::readAmount("Amount: $", amount, false)) break;
                    if (choice == "3") {
                        account += amount;
                        std::cout << "Deposit successful.\n";
                    } else {
                        account -= amount;
                        std::cout << "Withdrawal successful.\n";
                    }
                }
                BankAccount::printAccount(account);
            } else {
                std::cout << "Invalid option. Choose 0 through 7.\n";
            }
        } catch (const std::invalid_argument& error) {
            std::cout << "Transaction rejected: " << error.what() << '\n';
        } catch (const std::runtime_error& error) {
            std::cout << error.what() << '\n';
            break;
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}
