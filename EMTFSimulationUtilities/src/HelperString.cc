#include "L1TMuonSimulations/EMTFSimulationUtilities/interface/HelperString.h"

std::vector<std::string> split(std::string const & s, char c)
{
    std::vector<std::string> result;
    const char* str = s.c_str();
    do {
        const char *begin = str;
        while(*str != c && *str)
            str++;
        result.push_back(std::string(begin, str));
    } while (0 != *str++);
    return result;
}

std::string replace_all(std::string s, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = s.find(search, pos)) != std::string::npos) {
        s.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return s;
}
