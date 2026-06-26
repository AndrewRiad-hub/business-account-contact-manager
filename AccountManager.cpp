#include "AccountManager.h"
#include "CsvUtils.h"
#include "InputUtils.h"
#include "StateValidator.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

void AccountManager::rebuildIndexes()
{
    accountIDIndex.clear();
    companyTree.clear();

    for (size_t i = 0; i < accounts.size(); i++)
    {
        accountIDIndex[accounts[i].getAccountID()] = i;
        companyTree.insert(accounts[i].getCompanyName(), accounts[i].getAccountID());
    }
}

Account* AccountManager::findAccountPointer(int accountID)
{
    auto it = accountIDIndex.find(accountID);

    if (it == accountIDIndex.end())
    {
        return nullptr;
    }

    return &accounts[it->second];
}

bool AccountManager::deleteAccountInternal(int accountID, Account& removedAccount)
{
    auto it = accountIDIndex.find(accountID);

    if (it == accountIDIndex.end())
    {
        return false;
    }

    size_t index = it->second;
    removedAccount = accounts[index];
    accounts.erase(accounts.begin() + index);
    rebuildIndexes();

    return true;
}

void AccountManager::saveAccountsToCsv() const
{
    std::ofstream fout(accountsFileName);

    fout << "accountID,companyName,street,city,state,zip,creationDateTime\n";

    for (const Account& account : accounts)
    {
        Address addr = account.getAddress();

        fout << account.getAccountID() << ","
            << csvEscape(account.getCompanyName()) << ","
            << csvEscape(addr.street) << ","
            << csvEscape(addr.city) << ","
            << csvEscape(addr.state) << ","
            << csvEscape(addr.zip) << ","
            << csvEscape(account.getCreationDateTime()) << "\n";
    }
}

void AccountManager::saveContactsToCsv() const
{
    std::ofstream fout(contactsFileName);

    fout << "accountID,contactID,firstName,lastName,phoneNumber,emailAddress,creationDateTime\n";

    for (const Account& account : accounts)
    {
        for (const Contact& contact : account.getContacts())
        {
            fout << account.getAccountID() << ","
                << contact.getContactID() << ","
                << csvEscape(contact.getFirstName()) << ","
                << csvEscape(contact.getLastName()) << ","
                << csvEscape(contact.getPhoneNumber()) << ","
                << csvEscape(contact.getEmailAddress()) << ","
                << csvEscape(contact.getCreationDateTime()) << "\n";
        }
    }
}

void AccountManager::accountSubmenu(int accountID)
{
    int choice;

    do
    {
        Account* account = findAccountPointer(accountID);

        if (account == nullptr)
        {
            std::cout << "This account no longer exists.\n";
            return;
        }

        std::cout << "\n========== Account Menu: "
            << account->getCompanyName()
            << " [ID " << account->getAccountID() << "] ==========\n";
        std::cout << "1. Display full account\n";
        std::cout << "2. Display contacts only\n";
        std::cout << "3. Add contact to this account\n";
        std::cout << "4. Delete contact from this account\n";
        std::cout << "5. Edit this account's company/address fields\n";
        std::cout << "6. Edit a contact in this account\n";
        std::cout << "0. Return to previous menu/display\n";

        choice = readInt("Choose an option: ");

        switch (choice)
        {
        case 1:
            account->displayFull();
            break;
        case 2:
            account->displayContactsPaged();
            break;
        case 3:
            addContactToAccountByID(accountID);
            break;
        case 4:
            deleteContactFromAccountByID(accountID);
            break;
        case 5:
            updateAccountByID(accountID);
            break;
        case 6:
            updateContactFromAccountByID(accountID);
            break;
        case 0:
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);
}

void AccountManager::addContactToAccountByID(int accountID)
{
    Account* account = findAccountPointer(accountID);

    if (account == nullptr)
    {
        std::cout << "Account not found.\n";
        return;
    }

    int contactID = nextContactID;
    nextContactID++;

    std::cout << "Generated contact ID: " << contactID << "\n";

    std::string first = readLine("Contact first name: ");
    std::string last = readLine("Contact last name: ");
    std::string phone = readLine("Contact phone number: ");
    std::string email = readLine("Contact email address: ");

    Contact contact(contactID, first, last, phone, email);
    account->addContact(contact);

    Action action;
    action.type = ActionType::AddedContact;
    action.accountID = accountID;
    action.contactID = contactID;
    undoHistory.push(action);

    saveToCsv();

    std::cout << "Contact added successfully.\n";
}

void AccountManager::deleteContactFromAccountByID(int accountID)
{
    Account* account = findAccountPointer(accountID);

    if (account == nullptr)
    {
        std::cout << "Account not found.\n";
        return;
    }

    account->displayContacts();

    int contactID = readInt("Enter contact ID to delete: ");
    Contact removedContact;

    if (!account->deleteContactByID(contactID, removedContact))
    {
        std::cout << "Contact not found.\n";
        return;
    }

    Action action;
    action.type = ActionType::DeletedContact;
    action.accountID = accountID;
    action.contactID = contactID;
    action.contact = removedContact;
    undoHistory.push(action);

    saveToCsv();

    std::cout << "Contact deleted successfully.\n";
}

void AccountManager::updateAccountByID(int accountID)
{
    Account* account = findAccountPointer(accountID);

    if (account == nullptr)
    {
        std::cout << "Account not found.\n";
        return;
    }

    int choice;

    do
    {
        Address currentAddress = account->getAddress();

        std::cout << "\n========== Edit Account ==========\n";
        std::cout << "Account ID: " << account->getAccountID() << "\n";
        std::cout << "1. Company Name: " << account->getCompanyName() << "\n";
        std::cout << "2. Street: " << currentAddress.street << "\n";
        std::cout << "3. City: " << currentAddress.city << "\n";
        std::cout << "4. State Code: " << currentAddress.state << "\n";
        std::cout << "5. ZIP: " << currentAddress.zip << "\n";
        std::cout << "6. Replace entire address\n";
        std::cout << "0. Return\n";

        choice = readInt("Choose field to edit: ");

        switch (choice)
        {
        case 1:
            account->setCompanyName(readLine("New company name: "));
            rebuildIndexes();
            saveToCsv();
            std::cout << "Company name updated.\n";
            break;
        case 2:
            account->setStreet(readLine("New street: "));
            saveToCsv();
            std::cout << "Street updated.\n";
            break;
        case 3:
            account->setCity(readLine("New city: "));
            saveToCsv();
            std::cout << "City updated.\n";
            break;
        case 4:
            account->setState(readValidStateCode());
            saveToCsv();
            std::cout << "State code updated.\n";
            break;
        case 5:
            account->setZip(readLine("New ZIP: "));
            saveToCsv();
            std::cout << "ZIP updated.\n";
            break;
        case 6:
        {
            Address newAddress;
            std::cout << "Enter new address information:\n";
            newAddress.input();
            account->setAddress(newAddress);
            saveToCsv();
            std::cout << "Address updated.\n";
            break;
        }
        case 0:
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);
}

void AccountManager::updateContactFromAccountByID(int accountID)
{
    Account* account = findAccountPointer(accountID);

    if (account == nullptr)
    {
        std::cout << "Account not found.\n";
        return;
    }

    account->displayContactsPaged();

    if (account->getContacts().empty())
    {
        return;
    }

    int contactID = readInt("Enter contact ID to edit: ");
    Contact* contact = account->findContactByID(contactID);

    if (contact == nullptr)
    {
        std::cout << "Contact not found.\n";
        return;
    }

    contactEditSubmenu(account, contact);
}

void AccountManager::contactEditSubmenu(Account* account, Contact* contact)
{
    int choice;

    do
    {
        std::cout << "\n========== Edit Contact ==========\n";
        std::cout << "Account: " << account->getCompanyName() << " [ID " << account->getAccountID() << "]\n";
        std::cout << "Contact ID: " << contact->getContactID() << "\n";
        std::cout << "1. First Name: " << contact->getFirstName() << "\n";
        std::cout << "2. Last Name: " << contact->getLastName() << "\n";
        std::cout << "3. Phone Number: " << contact->getPhoneNumber() << "\n";
        std::cout << "4. Email Address: " << contact->getEmailAddress() << "\n";
        std::cout << "0. Return\n";

        choice = readInt("Choose field to edit: ");

        switch (choice)
        {
        case 1:
            contact->setFirstName(readLine("New first name: "));
            saveToCsv();
            std::cout << "First name updated.\n";
            break;
        case 2:
            contact->setLastName(readLine("New last name: "));
            saveToCsv();
            std::cout << "Last name updated.\n";
            break;
        case 3:
            contact->setPhoneNumber(readLine("New phone number: "));
            saveToCsv();
            std::cout << "Phone number updated.\n";
            break;
        case 4:
            contact->setEmailAddress(readLine("New email address: "));
            saveToCsv();
            std::cout << "Email address updated.\n";
            break;
        case 0:
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);
}

std::string AccountManager::readValidStateCode()
{
    while (true)
    {
        std::string state = normalizeStateCode(readLine("State code, such as CA or NY: "));

        if (isValidStateCode(state))
        {
            return state;
        }

        std::cout << "Invalid state code. Please enter a valid 2-letter U.S. state code.\n";
    }
}

AccountManager::AccountManager()
    : nextAccountID(1001), nextContactID(5001),
    accountsFileName("accounts.csv"), contactsFileName("contacts.csv")
{
    loadFromCsv();
}

void AccountManager::loadFromCsv()
{
    accounts.clear();
    accountIDIndex.clear();
    companyTree.clear();

    int maxAccountID = 1000;
    int maxContactID = 5000;

    std::ifstream accountFile(accountsFileName);

    if (accountFile)
    {
        std::string line;
        getline(accountFile, line);

        while (getline(accountFile, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::vector<std::string> fields = parseCsvLine(line);

            if (fields.size() < 7)
            {
                continue;
            }

            try
            {
                int accountID = std::stoi(fields[0]);

                Address addr;
                addr.street = fields[2];
                addr.city = fields[3];
                addr.state = fields[4];
                addr.zip = fields[5];

                Account account(accountID, fields[1], addr, fields[6]);
                accounts.push_back(account);

                if (accountID > maxAccountID)
                {
                    maxAccountID = accountID;
                }
            }
            catch (const std::exception&)
            {
                continue;
            }
        }
    }

    rebuildIndexes();

    std::ifstream contactFile(contactsFileName);

    if (contactFile)
    {
        std::string line;
        getline(contactFile, line);

        while (getline(contactFile, line))
        {
            if (line.empty())
            {
                continue;
            }

            std::vector<std::string> fields = parseCsvLine(line);

            if (fields.size() < 7)
            {
                continue;
            }

            try
            {
                int accountID = std::stoi(fields[0]);
                int contactID = std::stoi(fields[1]);

                Account* account = findAccountPointer(accountID);

                if (account != nullptr)
                {
                    Contact contact(contactID, fields[2], fields[3], fields[4], fields[5], fields[6]);
                    account->addContact(contact);

                    if (contactID > maxContactID)
                    {
                        maxContactID = contactID;
                    }
                }
            }
            catch (const std::exception&)
            {
                continue;
            }
        }
    }

    nextAccountID = maxAccountID + 1;
    nextContactID = maxContactID + 1;
}

void AccountManager::saveToCsv() const
{
    saveAccountsToCsv();
    saveContactsToCsv();
}

void AccountManager::addAccount()
{
    int id = nextAccountID;
    nextAccountID++;

    std::cout << "Generated account ID: " << id << "\n";

    std::string companyName = readLine("Enter company name: ");

    Address address;
    std::cout << "Enter address information:\n";
    address.input();

    Account account(id, companyName, address);
    accounts.push_back(account);
    rebuildIndexes();

    Action action;
    action.type = ActionType::AddedAccount;
    action.accountID = id;
    action.contactID = 0;
    undoHistory.push(action);

    saveToCsv();

    std::cout << "Account added successfully.\n";
}

void AccountManager::deleteAccount()
{
    int id = readInt("Enter account ID to delete: ");
    Account removedAccount;

    if (!deleteAccountInternal(id, removedAccount))
    {
        std::cout << "Account not found.\n";
        return;
    }

    Action action;
    action.type = ActionType::DeletedAccount;
    action.account = removedAccount;
    action.accountID = id;
    action.contactID = 0;
    undoHistory.push(action);

    saveToCsv();

    std::cout << "Account deleted successfully.\n";
}

void AccountManager::openAccountByID()
{
    int id = readInt("Enter account ID to open: ");
    Account* account = findAccountPointer(id);

    if (account == nullptr)
    {
        std::cout << "Account not found.\n";
        return;
    }

    account->displayFull();
    accountSubmenu(id);
}

void AccountManager::updateAccount()
{
    int id = readInt("Enter account ID to edit: ");
    updateAccountByID(id);
}

void AccountManager::addContactToAccount()
{
    int id = readInt("Enter account ID: ");
    addContactToAccountByID(id);
}

void AccountManager::deleteContactFromAccount()
{
    int id = readInt("Enter account ID: ");
    deleteContactFromAccountByID(id);
}

void AccountManager::updateContactFromAccount()
{
    int id = readInt("Enter account ID: ");
    updateContactFromAccountByID(id);
}

void AccountManager::displayAllAccounts()
{
    if (accounts.empty())
    {
        std::cout << "No accounts stored.\n";
        return;
    }

    const int defaultPageSize = 25;
    int pageSize = defaultPageSize;

    std::string answer = readLine("Display accounts per page [press Enter for 25]: ");

    if (!answer.empty())
    {
        try
        {
            int requestedSize = std::stoi(answer);

            if (requestedSize > 0)
            {
                pageSize = requestedSize;
            }
            else
            {
                std::cout << "Invalid page size. Using default of 25.\n";
            }
        }
        catch (...)
        {
            std::cout << "Invalid page size. Using default of 25.\n";
        }
    }

    int currentPage = 0;
    int totalAccounts = static_cast<int>(accounts.size());
    int totalPages = (totalAccounts + pageSize - 1) / pageSize;

    while (true)
    {
        if (totalPages <= 0)
        {
            break;
        }

        if (currentPage < 0 || currentPage >= totalPages)
        {
            currentPage = 0;
        }

        int startIndex = currentPage * pageSize;
        int endIndex = startIndex + pageSize;

        if (endIndex > totalAccounts)
        {
            endIndex = totalAccounts;
        }

        std::cout << "\n========== Accounts Page "
            << currentPage + 1 << " of " << totalPages
            << " ==========\n";
        std::cout << "Showing accounts " << startIndex + 1
            << " through " << endIndex
            << " of " << totalAccounts << ".\n\n";

        for (int i = startIndex; i < endIndex; i++)
        {
            accounts[i].displaySummary();
        }

        std::cout << "\nOptions: [N]ext, [P]revious, [J]ump page, [O]pen account, [Q]uit accounts\n";
        std::string choice = readLine("Choose option: ");

        if (choice == "N" || choice == "n")
        {
            currentPage = (currentPage + 1) % totalPages;
        }
        else if (choice == "P" || choice == "p")
        {
            currentPage = (currentPage - 1 + totalPages) % totalPages;
        }
        else if (choice == "J" || choice == "j")
        {
            int requestedPage = readInt("Enter page number: ");

            if (requestedPage >= 1 && requestedPage <= totalPages)
            {
                currentPage = requestedPage - 1;
            }
            else
            {
                std::cout << "Invalid page number. Valid pages are 1 through " << totalPages << ".\n";
            }
        }
        else if (choice == "O" || choice == "o")
        {
            int accountID = readInt("Enter account ID to open: ");
            Account* account = findAccountPointer(accountID);

            if (account == nullptr)
            {
                std::cout << "Account not found.\n";
            }
            else
            {
                account->displayFull();
                accountSubmenu(accountID);
            }
        }
        else if (choice == "Q" || choice == "q")
        {
            break;
        }
        else
        {
            std::cout << "Invalid option.\n";
        }
    }
}

void AccountManager::displayAccountsSortedByCompany()
{
    if (accounts.empty())
    {
        std::cout << "No accounts stored.\n";
        return;
    }

    std::vector<int> sortedAccountIDs;

    companyTree.inorder([&sortedAccountIDs](const std::string&, int accountID)
        {
            sortedAccountIDs.push_back(accountID);
        });

    const int defaultPageSize = 25;
    int pageSize = defaultPageSize;

    std::string answer = readLine("Display sorted accounts per page [press Enter for 25]: ");

    if (!answer.empty())
    {
        try
        {
            int requestedSize = std::stoi(answer);

            if (requestedSize > 0)
            {
                pageSize = requestedSize;
            }
            else
            {
                std::cout << "Invalid page size. Using default of 25.\n";
            }
        }
        catch (...)
        {
            std::cout << "Invalid page size. Using default of 25.\n";
        }
    }

    int currentPage = 0;
    int totalAccounts = static_cast<int>(sortedAccountIDs.size());
    int totalPages = (totalAccounts + pageSize - 1) / pageSize;

    while (true)
    {
        if (totalPages <= 0)
        {
            break;
        }

        if (currentPage < 0 || currentPage >= totalPages)
        {
            currentPage = 0;
        }

        int startIndex = currentPage * pageSize;
        int endIndex = startIndex + pageSize;

        if (endIndex > totalAccounts)
        {
            endIndex = totalAccounts;
        }

        std::cout << "\n========== Alphabetical Accounts Page "
            << currentPage + 1 << " of " << totalPages
            << " ==========\n";
        std::cout << "Showing sorted accounts " << startIndex + 1
            << " through " << endIndex
            << " of " << totalAccounts << ".\n\n";

        for (int i = startIndex; i < endIndex; i++)
        {
            Account* account = findAccountPointer(sortedAccountIDs[i]);

            if (account != nullptr)
            {
                account->displaySummary();
            }
        }

        std::cout << "\nOptions: [N]ext, [P]revious, [J]ump page, [O]pen account, [Q]uit sorted accounts\n";
        std::string choice = readLine("Choose option: ");

        if (choice == "N" || choice == "n")
        {
            currentPage = (currentPage + 1) % totalPages;
        }
        else if (choice == "P" || choice == "p")
        {
            currentPage = (currentPage - 1 + totalPages) % totalPages;
        }
        else if (choice == "J" || choice == "j")
        {
            int requestedPage = readInt("Enter page number: ");

            if (requestedPage >= 1 && requestedPage <= totalPages)
            {
                currentPage = requestedPage - 1;
            }
            else
            {
                std::cout << "Invalid page number. Valid pages are 1 through " << totalPages << ".\n";
            }
        }
        else if (choice == "O" || choice == "o")
        {
            int accountID = readInt("Enter account ID to open: ");
            Account* account = findAccountPointer(accountID);

            if (account == nullptr)
            {
                std::cout << "Account not found.\n";
            }
            else
            {
                account->displayFull();
                accountSubmenu(accountID);
            }
        }
        else if (choice == "Q" || choice == "q")
        {
            break;
        }
        else
        {
            std::cout << "Invalid option.\n";
        }
    }
}

void AccountManager::undoLastAction()
{
    if (undoHistory.empty())
    {
        std::cout << "No actions to undo.\n";
        return;
    }

    Action action = undoHistory.top();
    undoHistory.pop();

    if (action.type == ActionType::AddedAccount)
    {
        Account removed;
        deleteAccountInternal(action.accountID, removed);
        saveToCsv();
        std::cout << "Undo complete: added account removed.\n";
    }
    else if (action.type == ActionType::DeletedAccount)
    {
        accounts.push_back(action.account);
        rebuildIndexes();
        saveToCsv();
        std::cout << "Undo complete: deleted account restored.\n";
    }
    else if (action.type == ActionType::AddedContact)
    {
        Account* account = findAccountPointer(action.accountID);

        if (account != nullptr)
        {
            Contact removed;
            account->deleteContactByID(action.contactID, removed);
            saveToCsv();
            std::cout << "Undo complete: added contact removed.\n";
        }
        else
        {
            std::cout << "Undo failed: account no longer exists.\n";
        }
    }
    else if (action.type == ActionType::DeletedContact)
    {
        Account* account = findAccountPointer(action.accountID);

        if (account != nullptr)
        {
            account->addContact(action.contact);
            saveToCsv();
            std::cout << "Undo complete: deleted contact restored.\n";
        }
        else
        {
            std::cout << "Undo failed: account no longer exists.\n";
        }
    }
}

void AccountManager::runBenchmarkTest() const
{
    if (accounts.empty())
    {
        std::cout << "No accounts loaded. Add accounts or load CSV files before running benchmarks.\n";
        return;
    }

    const int repetitions = 100000;
    std::vector<int> testIDs;

    for (const Account& account : accounts)
    {
        testIDs.push_back(account.getAccountID());
    }

    long long checksum = 0;

    auto linearStart = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < repetitions; i++)
    {
        int targetID = testIDs[i % testIDs.size()];

        for (const Account& account : accounts)
        {
            if (account.getAccountID() == targetID)
            {
                checksum += account.getAccountID();
                break;
            }
        }
    }

    auto linearEnd = std::chrono::high_resolution_clock::now();

    auto hashStart = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < repetitions; i++)
    {
        int targetID = testIDs[i % testIDs.size()];
        auto it = accountIDIndex.find(targetID);

        if (it != accountIDIndex.end())
        {
            checksum += accounts[it->second].getAccountID();
        }
    }

    auto hashEnd = std::chrono::high_resolution_clock::now();

    auto bstStart = std::chrono::high_resolution_clock::now();

    int traversalRepeats = 1000;

    for (int i = 0; i < traversalRepeats; i++)
    {
        companyTree.inorder([&checksum](const std::string&, int accountID)
            {
                checksum += accountID;
            });
    }

    auto bstEnd = std::chrono::high_resolution_clock::now();

    double linearMs = std::chrono::duration<double, std::milli>(linearEnd - linearStart).count();
    double hashMs = std::chrono::duration<double, std::milli>(hashEnd - hashStart).count();
    double bstMs = std::chrono::duration<double, std::milli>(bstEnd - bstStart).count();

    std::cout << "\n========== Benchmark Test Results ==========\n";
    std::cout << "Accounts loaded: " << accounts.size() << "\n";
    std::cout << "Search repetitions: " << repetitions << "\n";
    std::cout << "BST traversal repetitions: " << traversalRepeats << "\n\n";

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Linear search by Account ID: " << linearMs << " ms\n";
    std::cout << "Hash table lookup by Account ID: " << hashMs << " ms\n";
    std::cout << "BST alphabetical traversal: " << bstMs << " ms\n\n";

    std::cout << "Expected Big-O behavior:\n";
    std::cout << "Linear search: O(n)\n";
    std::cout << "Hash table lookup: O(1) average\n";
    std::cout << "BST inorder traversal: O(n)\n\n";

    std::cout << "Checksum value, used to prevent compiler optimization: " << checksum << "\n";
}