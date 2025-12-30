#include "hash.hpp"
#include <boost/crc.hpp>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>

FileHasher::FileHasher(boost::filesystem::path path, size_t block_size, bool use_crc32)
    : path_(path), block_size_(block_size), use_crc32_(use_crc32) {}

std::string FileHasher::get_path_string() const {
    return path_.string();
}

std::string FileHasher::get_block_hash(size_t index) {
    while (index >= cached_hashes_.size() && !eof_reached_) {
        if (!read_next_block()) break;
    }
    if (index < cached_hashes_.size()) return cached_hashes_[index];
    return ""; 
}

bool FileHasher::read_next_block() {
    if (!file_.is_open()) {
        file_.open(path_, std::ios::binary);
        if (!file_) { eof_reached_ = true; return false; }
    }

    std::vector<char> buffer(block_size_, 0); // Память пол один блок
    file_.read(buffer.data(), block_size_); // Считывает S байт
    std::streamsize bytes_read = file_.gcount();

    if (bytes_read <= 0) { eof_reached_ = true; return false; }

    if (use_crc32_) {
        boost::crc_32_type crc;
        crc.process_bytes(buffer.data(), block_size_);
        cached_hashes_.push_back(std::to_string(crc.checksum()));
    } else {
        unsigned char md[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(buffer.data()), block_size_, md);
        std::stringstream ss;
        for(int i = 0; i < MD5_DIGEST_LENGTH; i++) ss << std::hex << std::setw(2) << std::setfill('0') << (int)md[i];
        cached_hashes_.push_back(ss.str());
    }
    return true;
}