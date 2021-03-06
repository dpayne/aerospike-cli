#pragma once

#include <aerospike/aerospike.h>
#include <aerospike/as_config.h>
#include <cli/cli.h>
#include <cli/clifilesession.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <string>

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

    as_config m_config{};
    aerospike m_aerospike{};
    bool m_is_aerospike_initialized{false};

    auto get_menu(aerospike* as) const -> std::unique_ptr<cli::Menu>;

    auto setup_get_ops(aerospike* as, cli::Menu* menu) const -> void;

    auto setup_put_ops(aerospike* as, cli::Menu* menu) const -> void;

    auto setup_delete_ops(aerospike* as, cli::Menu* menu) const -> void;

    auto setup_query_ops(aerospike* as, cli::Menu* menu) const -> void;

    auto setup_scan_ops(aerospike* as, cli::Menu* menu) const -> void;

    auto setup_aerospike(as_config* config, aerospike* as) const -> bool;

   public:
    /**
     * @brief Creates a new ascli
     * @param host hostname of the aerospike server
     * @param port port of the aerospike server
     * @param pass password of the aerospike server
     */
    AsCli(std::string host, uint32_t port, std::string user, std::string pass);
    virtual ~AsCli();

    AsCli(AsCli&&) = delete;
    AsCli(const AsCli&) = delete;
    auto operator=(const AsCli&) -> AsCli = delete;
    auto operator=(AsCli&&) -> AsCli = delete;

    /**
     * @brief Starts aerospike cli
     */
    auto start() -> void;
};

}  // namespace ascli
