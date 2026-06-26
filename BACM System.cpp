#include "AccountManager.h"
#include "InputUtils.h"

#include <iostream>

void displayMenu()
{
    std::cout << "\n========== Business Account & Contact Manager ==========\n";
    std::cout << "1.  Add account\n";
    std::cout << "2.  Delete account\n";
    std::cout << "3.  Open account by ID\n";
    std::cout << "4.  Edit account company/address fields\n";
    std::cout << "5.  Add contact to account\n";
    std::cout << "6.  Delete contact from account\n";
    std::cout << "7.  Edit contact in account\n";
    std::cout << "8.  Display all accounts\n";
    std::cout << "9.  Display accounts sorted by company name using BST\n";
    std::cout << "10. Undo last add/delete action using stack\n";
    std::cout << "11. Run benchmark test\n";
    std::cout << "12. Manually save CSV files\n";
    std::cout << "13. Reload CSV files\n";
    std::cout << "0.  Exit\n";
    std::cout << "=========================================================\n";
}

int main()
{
    AccountManager manager;
    int choice;

    do
    {
        displayMenu();
        choice = readInt("Choose an option: ");

        switch (choice)
        {
        case 1:
            manager.addAccount();
            break;
        case 2:
            manager.deleteAccount();
            break;
        case 3:
            manager.openAccountByID();
            break;
        case 4:
            manager.updateAccount();
            break;
        case 5:
            manager.addContactToAccount();
            break;
        case 6:
            manager.deleteContactFromAccount();
            break;
        case 7:
            manager.updateContactFromAccount();
            break;
        case 8:
            manager.displayAllAccounts();
            break;
        case 9:
            manager.displayAccountsSortedByCompany();
            break;
        case 10:
            manager.undoLastAction();
            break;
        case 11:
            manager.runBenchmarkTest();
            break;
        case 12:
            manager.saveToCsv();
            std::cout << "CSV files saved.\n";
            break;
        case 13:
            manager.loadFromCsv();
            std::cout << "CSV files reloaded.\n";
            break;
        case 0:
            manager.saveToCsv();
            std::cout << "Data saved. Exiting program.\n";
            break;
        default:
            std::cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
