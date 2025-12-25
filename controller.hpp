#pragma once
#include <string>
#include <vector>

void run_bayan(const std::vector<std::string>& paths, 
              const std::vector<std::string>& exceptions, 
              const std::vector<std::string>& masks, 
              size_t block_size, size_t depth, size_t min_size, bool use_crc32);