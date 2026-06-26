#include "InputUtils.h"

#include <iostream>
#include <limits>

std::string readLine(const std::string& prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int readInt(const std::string& prompt)
{
    int value;

    while (true)
    {
        std::cout << prompt;
        std::cin >> value;

        if (!std::cin.fail())
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number.\n";
    }
}
