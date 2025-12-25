#include "mask.hpp"
#include <regex>

bool is_match(boost::filesystem::path path, const std::vector<std::string>& masks) {
    if (masks.empty()) return true;
    for (const auto& m : masks) {
        std::string r_str = "";
        for(char c : m) {
            if (c == '.') r_str += "\\.";
            else if (c == '*') r_str += ".*";
            else if (c == '?') r_str += ".";
            else r_str += c;
        }
        std::regex pattern(r_str, std::regex_constants::icase);
        if (std::regex_match(path.filename().string(), pattern)) return true;
    }
    return false;
}