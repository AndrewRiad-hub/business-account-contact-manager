#include "StateValidator.h"
#include "CustomHash.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>

std::string normalizeStateCode(const std::string& stateCode)
{
    std::string result;

    for (char ch : stateCode)
    {
        if (!std::isspace(static_cast<unsigned char>(ch)))
        {
            result += static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        }
    }

    return result;
}

bool isValidStateCode(const std::string& stateCode)
{
    static const std::unordered_set<std::string, StringHash> validStates =
    {
        "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA",
        "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD",
        "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ",
        "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC",
        "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY"
    };

    return validStates.find(normalizeStateCode(stateCode)) != validStates.end();
}
