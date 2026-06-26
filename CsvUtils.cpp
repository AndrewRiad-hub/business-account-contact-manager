#include "CsvUtils.h"

std::string csvEscape(const std::string& value)
{
    bool needsQuotes = false;

    for (char ch : value)
    {
        if (ch == ',' || ch == '"' || ch == '\n' || ch == '\r')
        {
            needsQuotes = true;
            break;
        }
    }

    if (!needsQuotes)
    {
        return value;
    }

    std::string escaped = "\"";

    for (char ch : value)
    {
        if (ch == '"')
        {
            escaped += "\"\"";
        }
        else
        {
            escaped += ch;
        }
    }

    escaped += "\"";
    return escaped;
}

std::vector<std::string> parseCsvLine(const std::string& line)
{
    std::vector<std::string> fields;
    std::string current;
    bool insideQuotes = false;

    for (size_t i = 0; i < line.size(); i++)
    {
        char ch = line[i];

        if (insideQuotes)
        {
            if (ch == '"')
            {
                if (i + 1 < line.size() && line[i + 1] == '"')
                {
                    current += '"';
                    i++;
                }
                else
                {
                    insideQuotes = false;
                }
            }
            else
            {
                current += ch;
            }
        }
        else
        {
            if (ch == '"')
            {
                insideQuotes = true;
            }
            else if (ch == ',')
            {
                fields.push_back(current);
                current.clear();
            }
            else
            {
                current += ch;
            }
        }
    }

    fields.push_back(current);
    return fields;
}
