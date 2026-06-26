#ifndef CSVUTILS_H
#define CSVUTILS_H

#include <string>
#include <vector>

std::string csvEscape(const std::string& value);
std::vector<std::string> parseCsvLine(const std::string& line);

#endif
