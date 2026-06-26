#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>

struct Address
{
    std::string street;
    std::string city;
    std::string state;
    std::string zip;

    void input();
    void display() const;
};

#endif
