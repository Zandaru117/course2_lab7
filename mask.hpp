#pragma once
#include <boost/filesystem.hpp>
#include <string>
#include <vector>

bool is_match(boost::filesystem::path path, const std::vector<std::string>& masks);