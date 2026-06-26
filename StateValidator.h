#ifndef STATEVALIDATOR_H
#define STATEVALIDATOR_H

#include <string>

std::string normalizeStateCode(const std::string& stateCode);
bool isValidStateCode(const std::string& stateCode);

#endif
