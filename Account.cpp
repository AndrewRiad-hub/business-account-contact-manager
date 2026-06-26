#include "Account.h"
#include "DateTimeUtils.h"
#include "InputUtils.h"

#include <iostream>
#include <string>

Account::Account()
    : accountID(0), companyName(""), creationDateTime(getCurrentDateTime())
{
}

Account::Account(int id, std::string name, Address addr)
    : accountID(id), companyName(name), address(addr), creationDateTime(getCurrentDateTime())
{
}

Account::Account(int id, std::string name, Address addr, std::string created)
    : accountID(id), companyName(name), address(addr), creationDateTime(created)
{
}

int Account::getAccountID() const
{
    return accountID;
}

std::string Account::getCompanyName() const
{
    return companyName;
}

Address Account::getAddress() const
{
    return address;
}

std::string Account::getCreationDateTime() const
{
    return creationDateTime;
}

const std::vector<Contact>& Account::getContacts() const
{
    return contacts;
}

void Account::setCompanyName(const std::string& name)
{
    companyName = name;
}

void Account::setAddress(const Address& addr)
{
    address = addr;
}

void Account::setStreet(const std::string& street)
{
    address.street = street;
}

void Account::setCity(const std::string& city)
{
    address.city = city;
}

void Account::setState(const std::string& state)
{
    address.state = state;
}

void Account::setZip(const std::string& zip)
{
    address.zip = zip;
}

void Account::addContact(const Contact& contact)
{
    contacts.push_back(contact);
}

Contact* Account::findContactByID(int contactID)
{
    for (Contact& contact : contacts)
    {
        if (contact.getContactID() == contactID)
        {
            return &contact;
        }
    }

    return nullptr;
}

const Contact* Account::findContactByID(int contactID) const
{
    for (const Contact& contact : contacts)
    {
        if (contact.getContactID() == contactID)
        {
            return &contact;
        }
    }

    return nullptr;
}

bool Account::deleteContactByID(int contactID, Contact& removedContact)
{
    for (size_t i = 0; i < contacts.size(); i++)
    {
        if (contacts[i].getContactID() == contactID)
        {
            removedContact = contacts[i];
            contacts.erase(contacts.begin() + i);
            return true;
        }
    }

    return false;
}

void Account::displaySummary() const
{
    std::cout << "ID: " << accountID
              << " | Company: " << companyName
              << " | Contacts: " << contacts.size()
              << " | Created: " << creationDateTime << "\n";
}

void Account::displayContacts() const
{
    std::cout << "\nContacts for " << companyName << ":\n";

    if (contacts.empty())
    {
        std::cout << "  No contacts stored.\n";
        return;
    }

    for (const Contact& contact : contacts)
    {
        contact.displaySummary();
    }
}

void Account::displayContactsPaged() const
{
    std::cout << "\nContacts for " << companyName << ":\n";

    if (contacts.empty())
    {
        std::cout << "  No contacts stored.\n";
        return;
    }

    const int defaultPageSize = 25;
    int pageSize = defaultPageSize;

    std::string answer = readLine("Display contacts per page [press Enter for 25]: ");

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
    int totalContacts = static_cast<int>(contacts.size());
    int totalPages = (totalContacts + pageSize - 1) / pageSize;

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

        if (endIndex > totalContacts)
        {
            endIndex = totalContacts;
        }

        std::cout << "\n========== Contacts Page "
                  << currentPage + 1 << " of " << totalPages
                  << " ==========\n";
        std::cout << "Showing contacts " << startIndex + 1
                  << " through " << endIndex
                  << " of " << totalContacts << ".\n\n";

        for (int i = startIndex; i < endIndex; i++)
        {
            contacts[i].displaySummary();
        }

        std::cout << "\nOptions: [N]ext, [P]revious, [J]ump page, [O]pen contact, [Q]uit contacts\n";
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
            int contactID = readInt("Enter contact ID to open: ");
            const Contact* contact = findContactByID(contactID);

            if (contact == nullptr)
            {
                std::cout << "Contact not found.\n";
            }
            else
            {
                contact->display();
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

void Account::displayFull() const
{
    std::cout << "\n----------------------------------------\n";
    std::cout << "Account ID: " << accountID << "\n";
    std::cout << "Company Name: " << companyName << "\n";
    std::cout << "Address: ";
    address.display();
    std::cout << "\nCreated: " << creationDateTime << "\n";
    std::cout << "Contacts: " << contacts.size() << "\n";
    displayContacts();
    std::cout << "----------------------------------------\n";
}
