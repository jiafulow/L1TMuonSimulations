#ifndef EMTFSimulationUtilities_HelperString_h_
#define EMTFSimulationUtilities_HelperString_h_

#include <string>
#include <vector>

inline bool startswith(std::string const & s, std::string const & prefix) {
    return std::equal(prefix.begin(), prefix.end(), s.begin());
}

inline bool endswith(std::string const & s, std::string const & suffix) {
    if (suffix.size() > s.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

// See http://stackoverflow.com/a/53878
std::vector<std::string> split(std::string const & s, char c = ' ');

std::string replace_all(std::string s, const std::string& search, const std::string& replace);

#endif
