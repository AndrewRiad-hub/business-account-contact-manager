#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "Address.h"
#include "Contact.h"

#include <string>
#include <vector>

class Account
{
private:
    int accountID;
    std::string companyName;
    Address address;
    std::string creationDateTime;
    std::vector<Contact> contacts;

public:
    Account();
    Account(int id, std::string name, Address addr);
    Account(int id, std::string name, Address addr, std::string created);

    int getAccountID() const;
    std::string getCompanyName() const;
    Address getAddress() const;
    std::string getCreationDateTime() const;
    const std::vector<Contact>& getContacts() const;

    void setCompanyName(const std::string& name);
    void setAddress(const Address& addr);
    void setStreet(const std::string& street);
    void setCity(const std::string& city);
    void setState(const std::string& state);
    void setZip(const std::string& zip);

    void addContact(const Contact& contact);
    Contact* findContactByID(int contactID);
    const Contact* findContactByID(int contactID) const;
    bool deleteContactByID(int contactID, Contact& removedContact);

    void displaySummary() const;
    void displayContacts() const;
    void displayContactsPaged() const;
    void displayFull() const;
};

#endif
