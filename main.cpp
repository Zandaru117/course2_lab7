#include <iostream>
#include <boost/program_options.hpp>
#include "controller.hpp"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        po::options_description desc("Bayan - duplicate file finder");
        desc.add_options()
            ("help,h", "Help message")
            ("path,p", po::value<std::vector<std::string>>()->multitoken(), "Include paths")
            ("exclude,e", po::value<std::vector<std::string>>()->multitoken(), "Exclude paths")
            ("depth,d", po::value<size_t>()->default_value(0), "Scan depth (0=current)")
            ("min-size,s", po::value<size_t>()->default_value(1), "Min file size")
            ("mask,m", po::value<std::vector<std::string>>()->multitoken(), "File masks")
            ("block-size,b", po::value<size_t>()->default_value(1024), "Block size S")
            ("hash,a", po::value<std::string>()->default_value("md5"), "Hash (md5/crc32)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help") || !vm.count("path")) {
            std::cout << desc << "\n";
            return 0;
        }

        run_bayan(
            vm["path"].as<std::vector<std::string>>(),
            vm.count("exclude") ? vm["exclude"].as<std::vector<std::string>>() : std::vector<std::string>{},
            vm.count("mask") ? vm["mask"].as<std::vector<std::string>>() : std::vector<std::string>{},
            vm["block-size"].as<size_t>(),
            vm["depth"].as<size_t>(),
            vm["min-size"].as<size_t>(),
            vm["hash"].as<std::string>() == "crc32"
        );

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}