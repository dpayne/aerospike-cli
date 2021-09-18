#include <ascli/ascli.h>
#include <ascli/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

#include <cli/cli.h>

auto main(int argc, char** argv) -> int {
    cxxopts::Options options(*argv, "aerospike cli client");

    // clang-format off
    options.add_options()
        ("h,help", "Show help")
        ("o,host", "Server (default 127.0.0.1)", cxxopts::value<std::string>()->default_value("127.0.0.1"))
        ("p,port", "port", cxxopts::value<uint32_t>()->default_value("3000"))
        ("u,user", "user", cxxopts::value<std::string>()->default_value(""))
        ("a,pass", "password", cxxopts::value<std::string>()->default_value(""))
        ("v,version", "Print the current version number");
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result["help"].as<bool>()) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (result["version"].as<bool>()) {
        std::cout << "AsCli, version " << ASCLILIB_VERSION << std::endl;
        return 0;
    }

    auto host = result["host"].as<std::string>();
    auto port = result["port"].as<uint32_t>();
    auto user = result["user"].as<std::string>();
    auto pass = result["pass"].as<std::string>();

    ascli::AsCli ascli(host, port, user, pass);
    ascli.start();

    return 0;
}
