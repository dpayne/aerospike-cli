#pragma once

#include <string>
#include <aerospike/aerospike.h>
#include <aerospike/as_config.h>
#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>

namespace ascli {

/**
 * @brief Aerospike cli
 */
class AsCli {
   private:
    std::string m_host;
    uint32_t m_port{};
    std::string m_user;
    std::string m_pass;

    cli::LoopScheduler m_scheduler;
    cli::Cli m_cli;

    as_config m_config;
    aerospike m_aerospike;

    auto get_menu(cli::LoopScheduler* scheduler, aerospike * as) const -> std::unique_ptr<cli::Menu>;

    auto setup_aerospike(as_config * config, aerospike * as) const -> bool;

   public:
    /**
     * @brief Creates a new ascli
     * @param host hostname of the aerospike server
     * @param port port of the aerospike server
     * @param pass password of the aerospike server
     */
    AsCli(std::string host, uint32_t port, std::string user, std::string pass);

    /**
     * @brief Starts aerospike cli
     */
    auto start() -> void;
};

}  // namespace ascli
