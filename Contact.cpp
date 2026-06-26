#include "Contact.h"
#include "DateTimeUtils.h"

#include <iostream>

Contact::Contact()
    : contactID(0), firstName(""), lastName(""), phoneNumber(""), emailAddress(""),
      creationDateTime(getCurrentDateTime())
{
}

Contact::Contact(int id, std::string first, std::string last, std::string phone, std::string email)
    : contactID(id), firstName(first), lastName(last), phoneNumber(phone), emailAddress(email),
      creationDateTime(getCurrentDateTime())
{
}

Contact::Contact(int id, std::string first, std::string last, std::string phone, std::string email, std::string created)
    : contactID(id), firstName(first), lastName(last), phoneNumber(phone), emailAddress(email),
      creationDateTime(created)
{
}

int Contact::getContactID() const
{
    return contactID;
}

std::string Contact::getFirstName() const
{
    return firstName;
}

std::string Contact::getLastName() const
{
    return lastName;
}

std::string Contact::getPhoneNumber() const
{
    return phoneNumber;
}

std::string Contact::getEmailAddress() const
{
    return emailAddress;
}

std::string Contact::getCreationDateTime() const
{
    return creationDateTime;
}

void Contact::setFirstName(const std::string& first)
{
    firstName = first;
}

void Contact::setLastName(const std::string& last)
{
    lastName = last;
}

void Contact::setPhoneNumber(const std::string& phone)
{
    phoneNumber = phone;
}

void Contact::setEmailAddress(const std::string& email)
{
    emailAddress = email;
}

void Contact::displaySummary() const
{
    std::cout << "ID: " << contactID
              << " | Name: " << firstName << " " << lastName
              << " | Phone: " << phoneNumber
              << " | Email: " << emailAddress
              << " | Created: " << creationDateTime << "\n";
}

void Contact::display() const
{
    std::cout << "\n----------------------------------------\n";
    std::cout << "Contact ID: " << contactID << "\n";
    std::cout << "First Name: " << firstName << "\n";
    std::cout << "Last Name: " << lastName << "\n";
    std::cout << "Phone: " << phoneNumber << "\n";
    std::cout << "Email: " << emailAddress << "\n";
    std::cout << "Created: " << creationDateTime << "\n";
    std::cout << "----------------------------------------\n";
}
