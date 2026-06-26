#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "Account.h"
#include "Action.h"
#include "CompanyBST.h"
#include "CustomHash.h"

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

class AccountManager
{
private:
    std::vector<Account> accounts;
    std::unordered_map<int, size_t, IntHash> accountIDIndex;
    CompanyBST companyTree;
    std::stack<Action> undoHistory;

    int nextAccountID;
    int nextContactID;

    std::string accountsFileName;
    std::string contactsFileName;

    void rebuildIndexes();
    Account* findAccountPointer(int accountID);
    bool deleteAccountInternal(int accountID, Account& removedAccount);

    void saveAccountsToCsv() const;
    void saveContactsToCsv() const;

    void accountSubmenu(int accountID);

    void addContactToAccountByID(int accountID);
    void deleteContactFromAccountByID(int accountID);
    void updateAccountByID(int accountID);
    void updateContactFromAccountByID(int accountID);
    void contactEditSubmenu(Account* account, Contact* contact);

    std::string readValidStateCode();

public:
    AccountManager();

    void loadFromCsv();
    void saveToCsv() const;

    void addAccount();
    void deleteAccount();
    void openAccountByID();
    void updateAccount();

    void addContactToAccount();
    void deleteContactFromAccount();
    void updateContactFromAccount();

    void displayAllAccounts();
    void displayAccountsSortedByCompany();

    void undoLastAction();
    void runBenchmarkTest() const;
};

#endif
