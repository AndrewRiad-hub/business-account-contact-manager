#ifndef CUSTOMHASH_H
#define CUSTOMHASH_H

#include <cstddef>
#include <string>

struct IntHash
{
    std::size_t operator()(int key) const
    {
        return static_cast<std::size_t>(key * 2654435761u);
    }
};

struct StringHash
{
    std::size_t operator()(const std::string& text) const
    {
        std::size_t hashValue = 0;

        for (char ch : text)
        {
            hashValue = hashValue * 31 + static_cast<unsigned char>(ch);
        }

        return hashValue;
    }
};

#endif
