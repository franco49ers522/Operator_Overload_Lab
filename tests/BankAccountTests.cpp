// Author: Jareth Franco
// Date: October 5, 2026
// Purpose: Check ownership, operators, validation, and static console utilities.
#include "../BankAccount.h"
#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <vector>

int main() {
    BankAccount a("001", "Jareth Franco", 100.0);
    BankAccount copy(a);
    copy.setAccountHolderName("Changed copy");
    copy += 25.0;
    assert(a.getAccountHolderName() == "Jareth Franco" && a.getBalance() == 100.0);
    BankAccount assigned("002", "Old holder", 5.0);
    assert(&(assigned = a) == &assigned);
    assigned.setAccountHolderName("Changed assignment");
    assert(a.getAccountHolderName() == "Jareth Franco");
    assigned = assigned;
    assert(assigned.getAccountHolderName() == "Changed assignment");
    {
        BankAccount temporary("003", "Temporary", 50.0);
        assigned = temporary;
    }
    assert(assigned.getAccountHolderName() == "Temporary" && assigned.getBalance() == 50.0);
    assert(&(a += 25.0) == &a);
    assert(&(a -= 25.0) == &a);
    (a += 10.0) += 15.0;
    assert(a.getBalance() == 125.0);
    assert(a == copy);
    const BankAccount equalBalance("004", "Different number", 125.0);
    assert(!(a == equalBalance) && !(a < equalBalance) && !(a > equalBalance));
    assert(assigned < a && a > assigned && !(a < assigned) && !(assigned > a));
    const double badAmounts[] = {0.0, -1.0, std::numeric_limits<double>::infinity(),
                                 std::numeric_limits<double>::quiet_NaN()};
    for (double amount : badAmounts) {
        bool rejected = false;
        try { a += amount; } catch (const std::invalid_argument&) { rejected = true; }
        assert(rejected && a.getBalance() == 125.0);
        rejected = false;
        try { a -= amount; } catch (const std::invalid_argument&) { rejected = true; }
        assert(rejected && a.getBalance() == 125.0);
    }
    bool rejected = false;
    try { a -= 126.0; } catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected && a.getBalance() == 125.0);
    a -= 125.0;
    assert(a.getBalance() == 0.0);
    BankAccount large("005", "Large", std::numeric_limits<double>::max());
    rejected = false;
    try { large += std::numeric_limits<double>::max(); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected && large.getBalance() == std::numeric_limits<double>::max());
    rejected = false;
    try { BankAccount invalid("006", " ", 0.0); }
    catch (const std::invalid_argument&) { rejected = true; }
    assert(rejected && !a.setAccountHolderName("\t"));
    std::vector<BankAccount> accounts;
    for (int i = 0; i < 1000; ++i) accounts.push_back(a);
    accounts[0].setAccountHolderName("Only first");
    assert(accounts[999].getAccountHolderName() == "Jareth Franco");

    std::ostringstream output;
    auto* oldOutput = std::cout.rdbuf(output.rdbuf());
    std::istringstream input("\n007\nJareth Franco\n25abc\n-1\n50.25\n");
    auto* oldInput = std::cin.rdbuf(input.rdbuf());
    BankAccount created = BankAccount::createAccountFromInput();
    BankAccount::printAccount(created);
    assert(created.getAccountNumber() == "007" && created.getBalance() == 50.25);
    assert(output.str().find("Account: 007 | Holder: Jareth Franco | Balance: $50.25") != std::string::npos);
    std::istringstream ended("");
    std::cin.rdbuf(ended.rdbuf());
    rejected = false;
    try { BankAccount::createAccountFromInput(); }
    catch (const std::runtime_error&) { rejected = true; }
    assert(rejected);
    std::cin.rdbuf(oldInput);
    std::cin.clear();
    std::cout.rdbuf(oldOutput);
    std::cout << "All BankAccount tests passed.\n";
}
