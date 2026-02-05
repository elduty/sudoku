#ifndef ParseUtils_h
#define ParseUtils_h

#include <cstdlib>
#include <string>

namespace sudoku {

inline bool parseUnsigned(const std::string& value, unsigned int& out)
{
    if(value.empty())
        return false;
    char* end = nullptr;
    unsigned long parsed = std::strtoul(value.c_str(), &end, 10);
    if(end == value.c_str() || *end != '\0')
        return false;
    out = static_cast<unsigned int>(parsed);
    return true;
}

} // namespace sudoku

#endif /* ParseUtils_h */
