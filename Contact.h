#ifndef CONTACT_H
#define CONTACT_H

#include <string>

class Contact
{
private:
    int contactID;
    std::string firstName;
    std::string lastName;
    std::string phoneNumber;
    std::string emailAddress;
    std::string creationDateTime;

public:
    Contact();
    Contact(int id, std::string first, std::string last, std::string phone, std::string email);
    Contact(int id, std::string first, std::string last, std::string phone, std::string email, std::string created);

    int getContactID() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getPhoneNumber() const;
    std::string getEmailAddress() const;
    std::string getCreationDateTime() const;

    void setFirstName(const std::string& first);
    void setLastName(const std::string& last);
    void setPhoneNumber(const std::string& phone);
    void setEmailAddress(const std::string& email);

    void displaySummary() const;
    void display() const;
};

#endif
