#include "controller.hpp"
#include "scanner.hpp"
#include "hash.hpp"
#include <map>
#include <iostream>

namespace fs = boost::filesystem;
typedef std::shared_ptr<FileHasher> HasherPtr;

void process_group(std::vector<HasherPtr> group, size_t block_idx) {
    if (group.size() < 2) return;

    std::map<std::string, std::vector<HasherPtr>> sub_groups;
    for (auto& h : group) {
        sub_groups[h->get_block_hash(block_idx)].push_back(h);
    }

    for (auto& [hash, members] : sub_groups) {
        if (members.size() < 2) continue;

        if (hash == "") { // EOF reached for all files in this subgroup
            for (const auto& m : members) std::cout << m->get_path_string() << "\n";
            std::cout << std::endl;
        } else {
            process_group(members, block_idx + 1);
        }
    }
}

void run_bayan(const std::vector<std::string>& paths, 
              const std::vector<std::string>& exceptions, 
              const std::vector<std::string>& masks, 
              size_t block_size, size_t depth, size_t min_size, bool use_crc32) {
    
    Scanner scanner(paths, exceptions, masks, depth, min_size);
    auto files = scanner.get_files();

    std::map<uintmax_t, std::vector<HasherPtr>> size_groups;
    for (const auto& p : files) {
        size_groups[fs::file_size(p)].push_back(std::make_shared<FileHasher>(p, block_size, use_crc32));
    }

    for (auto& [size, members] : size_groups) {
        if (members.size() > 1) {
            process_group(members, 0);
        }
    }
}