#pragma once
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <memory>

class FileHasher {
public:
    FileHasher(boost::filesystem::path path, size_t block_size, bool use_crc32);
    
    // Возвращает хэш блока по индексу. Если файл короче - возвращает пустую строку.
    std::string get_block_hash(size_t index);
    std::string get_path_string() const;

private:
    boost::filesystem::path path_;
    size_t block_size_;
    bool use_crc32_;
    std::ifstream file_;
    std::vector<std::string> cached_hashes_;
    bool eof_reached_ = false;

    bool read_next_block();
};