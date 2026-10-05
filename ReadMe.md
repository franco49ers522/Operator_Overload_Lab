# Operator Overload Lab

**Author:** Jareth Franco  
**Date:** October 5, 2026

## Purpose

This project extends my Objects_Classes_Lab bank account program with operator overloading, static utility functions, and the Rule of Three. The menu creates and lists accounts, deposits and withdraws money, changes holder names, compares accounts, and demonstrates independent copies. Accounts exist only while the program runs.

## Files

- `BankAccount.h`: class declaration and public interface.
- `BankAccount.cpp`: resource management, validation, operators, and static utilities.
- `Input.h`: shared, line-based console input validation.
- `main.cpp`: menu and copy demonstration.
- `tests/BankAccountTests.cpp`: automated checks of account behavior.

## Data dictionary

| Private member | Type | Purpose |
| --- | --- | --- |
| accountNumber | std::string | Account identifier; the menu rejects duplicate numbers. |
| accountHolderName | std::string* | Owns a dynamically allocated holder name. Each account has its own allocation. |
| balance | double | Finite, nonnegative account balance. |

## Rule of Three

The previous class stored the holder name by value. This version dynamically allocates it to demonstrate explicit resource ownership. A default pointer copy would make two accounts share one allocation, causing unintended changes and double deletion.

- The copy constructor allocates a new string containing the source holder name and copies the other members.
- Copy assignment checks self-assignment, constructs an independent temporary copy, and swaps its members with the destination. The temporary then frees the old destination allocation. A failed allocation leaves the destination unchanged.
- The destructor deletes the owned string once.

Menu option 7 copy-constructs one account and copy-assigns another. It changes both copied names and, when positive, withdraws the copy-constructed account's balance. The original stays unchanged. Copies keep the original account number and are demonstration objects only; they are not inserted into the account list.

The Rule of Five adds move construction and move assignment to support efficient resource transfers. This lab implements the required Rule of Three; copies remain safe without custom moves. With the Rule of Zero, a production class could use a value member such as std::string and let that member manage its own memory, as the original project did.

## Operators and utilities

| Operation | Behavior |
| --- | --- |
| `BankAccount()` | Creates an Unassigned account with Unknown holder and zero balance. |
| `BankAccount(number, name, initialBalance)` | Validates details and initializes an account. |
| `operator+=(double)` | Deposits a positive finite amount and returns `*this` by reference. |
| `operator-=(double)` | Withdraws a positive finite amount if funds are sufficient; returns `*this` by reference. |
| `operator==(const BankAccount&) const` | Compares account numbers, regardless of balances. |
| `operator<(const BankAccount&) const` | Compares balances using less-than. |
| `operator>(const BankAccount&) const` | Compares balances using greater-than. |
| `static printAccount(const BankAccount&)` | Prints number, holder, and balance to std::cout with two decimal places. |
| `static createAccountFromInput()` | Prompts for number, holder, and initial balance; returns a new account by value. |
| Getters | Return account number, holder name, or balance without modifying the object. |
| `setAccountHolderName(name)` | Updates a nonblank name; returns false for blank input. |
| `deposit(amount)` / `withdraw(amount)` | Retain the original bool-returning interface used internally by the operators. |

Invalid transactions throw std::invalid_argument from the arithmetic operators without changing the balance. The menu catches and displays the error. Zero and negative transactions, nonfinite values, deposit overflow, and overdrafts are rejected. A zero initial balance is allowed. The input helpers reject blank fields and trailing junk such as `25abc`. End of input cancels creation and exits cleanly.

The operators are member functions: the left operand is the current object (`*this`). A non-member binary operator would receive both operands as parameters and use public methods, or friendship if private access were necessary. Static functions have no current object: printAccount receives the account explicitly, and createAccountFromInput constructs one. Both can be called with the class name.

Equality checks identity while ordering checks balance, as required by the assignment. Different account numbers can have equal balances, making all three comparisons false. Balances use double to retain the original interface; this is an educational program rather than a financial ledger.

## Build and run

Use a C++11 or newer compiler from this project folder:

```sh
g++ -std=c++11 -Wall -Wextra -Wpedantic main.cpp BankAccount.cpp -o bank_accounts
```

Run `./bank_accounts` on Linux/macOS or `.\bank_accounts.exe` in Windows PowerShell.

## Menu

1. Create account using the static factory.
2. List accounts using the static print function.
3. Deposit using +=.
4. Withdraw using -=.
5. Change account holder name.
6. Compare two selected accounts using ==, <, and >. Selecting the same account demonstrates equality.
7. Demonstrate copy construction, assignment, self-assignment, and independent data.
0. Exit and release account resources.

## Verification

Build and run the automated checks:

```sh
g++ -std=c++11 -Wall -Wextra -Wpedantic tests/BankAccountTests.cpp BankAccount.cpp -o account_tests
```

Run `./account_tests` or `.\account_tests.exe`. Expected output: `All BankAccount tests passed.`

The project and tests compiled with warnings treated as errors. Tests passed for independent copies, self-assignment, assignment surviving destruction of its source, chained arithmetic and returned references, account-number equality, balance ordering and ties, full withdrawals, overdrafts, invalid amounts, overflow, invalid construction, vector growth, static input validation, formatted printing, and end of input. A scripted menu run also verified creation, deposits, withdrawals, rejection of insufficient funds, renaming, comparisons, deep-copy demonstrations, listing, and exit.
