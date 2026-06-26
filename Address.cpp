#include "Address.h"
#include "InputUtils.h"
#include "StateValidator.h"

#include <iostream>

void Address::input()
{
    street = readLine("Street: ");
    city = readLine("City: ");

    while (true)
    {
        state = normalizeStateCode(readLine("State code, such as CA or NY: "));

        if (isValidStateCode(state))
        {
            break;
        }

        std::cout << "Invalid state code. Please enter a valid 2-letter U.S. state code.\n";
    }

    zip = readLine("ZIP: ");
}

void Address::display() const
{
    std::cout << street << ", " << city << ", " << state << " " << zip;
}
