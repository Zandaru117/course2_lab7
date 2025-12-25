#include "Scanner.hpp"
#include "mask.hpp"
#include <boost/filesystem.hpp>
#include <unordered_set>

namespace fs = boost::filesystem;

Scanner::Scanner(const std::vector<std::string>& paths, 
                const std::vector<std::string>& exceptions, 
                const std::vector<std::string>& masks, 
                size_t depth, size_t min_size) {

    // 1. Приводим пути исключений к каноническому виду для точного сравнения
    std::unordered_set<std::string> excl_set;
    for(auto& e : exceptions) {
        try {
            // weakly_canonical работает даже если путь еще не существует, 
            // при этом исправляет слеши и точки (./ ../)
            excl_set.insert(fs::weakly_canonical(fs::absolute(e)).string());
        } catch (...) {
            continue;
        }
    }

    for (const auto& p_str : paths) {
        fs::path start_p;
        try {
            start_p = fs::weakly_canonical(fs::absolute(p_str));
        } catch (...) {
            continue;
        }

        if (!fs::exists(start_p)) continue;

        // Если сам стартовый путь находится в исключениях — пропускаем его полностью
        if (excl_set.count(start_p.string())) continue;

        fs::recursive_directory_iterator it(start_p), end;
        while (it != end) {
            try {
                // Канонизируем текущий путь для сравнения с excl_set
                fs::path current_p = fs::weakly_canonical(it->path());
                
                // 2. Проверка исключений
                if (excl_set.count(current_p.string())) {
                    it.disable_recursion_pending();
                    it++;
                    continue;
                }

                // 3. Проверка глубины
                if (static_cast<size_t>(it.depth()) > depth) {
                    it.disable_recursion_pending();
                    it++;
                    continue;
                }

                // 4. Проверка файла
                if (fs::is_regular_file(it->status())) {
                    if (fs::file_size(current_p) >= min_size && is_match(current_p, masks)) {
                        files_.insert(current_p);
                    }
                }
            } catch (const fs::filesystem_error&) {
                // Пропускаем файлы/папки, к которым нет доступа
                it.disable_recursion_pending();
            }
            
            it++;
        }
    }
}

std::set<fs::path> Scanner::get_files() const { return files_; }