#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <set>

class Scanner {
public:
    Scanner(const std::vector<std::string>& paths, 
            const std::vector<std::string>& exceptions, 
            const std::vector<std::string>& masks, 
            size_t depth, size_t min_size);
            
    std::set<boost::filesystem::path> get_files() const;

private:
    std::set<boost::filesystem::path> files_;
};