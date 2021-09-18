#include <ascli/ascli.h>
#include <fmt/format.h>

#include <aerospike/as_event.h>
#include <aerospike/as_policy.h>

#include <uv.h>

using namespace ascli;

AsCli::AsCli(std::string host, uint32_t port, std::string user, std::string pass)
    : m_host{std::move(host)},
      m_port{port},
      m_user{std::move(user)},
      m_pass{std::move(pass)},
      m_scheduler{},
      m_cli{get_menu(&m_scheduler, &m_aerospike)} {}

AsCli::~AsCli() {
    if (m_is_aerospike_initialized) {
        aerospike_destroy(&m_aerospike);
    }
}

auto AsCli::get_menu(cli::LoopScheduler* scheduler, aerospike* as) const -> std::unique_ptr<cli::Menu> {
    auto rootMenu = std::make_unique<cli::Menu>("cli");

    rootMenu->Insert(
        "get",
        [as, scheduler, aerospike_operator = &m_aerospike_operator](std::ostream& out, std::string ns, std::string set, std::string key) {
            out << "The answer is: " << key << "\n";
            aerospike_operator->get(ns, set, key, as, out);
        },
        "Get key from aerospike");

    auto subMenu = std::make_unique<cli::Menu>("aql");
    rootMenu->Insert(std::move(subMenu));

    return rootMenu;
}

auto AsCli::start() -> void {
    cli::SetColor();
    // global exit action
    m_cli.ExitAction([&](auto& out) {
        out << "Goodbye and thanks for all the fish.\n";
        m_scheduler.Stop();
    });

    cli::CliLocalTerminalSession localSession(m_cli, m_scheduler, std::cout);

    if (!setup_aerospike(&m_config, &m_aerospike)) {
        std::cerr << "Failed to initialize aerospike";
        return;
    }
    m_is_aerospike_initialized = true;

    m_scheduler.Run();
}

auto AsCli::setup_aerospike(as_config* config, aerospike* as) const -> bool {
    as_event_set_external_loop_capacity(1);  // Tell C client the maximum number of event loops that will be shared.
    auto aeroLoop = as_event_set_external_loop(
        uv_default_loop());  // The number of times this function is called has to match as_event_set_external_loop_capacity()

    // Initialize cluster configuration.
    as_config_init(config);

    if (!as_config_add_hosts(config, m_host.c_str(), m_port)) {
        std::cerr << "Invalid host: " << m_host << std::endl;
        as_event_close_loops();
        return false;
    }

    as_config_set_user(config, m_user.c_str(), m_pass.c_str());

    aerospike_init(as, config);

    as_error err;

    // Connect to the Aerospike database cluster. Assume this is the first thing
    // done after calling example_get_opts(), so it's ok to exit on failure.
    if (aerospike_connect(as, &err) != AEROSPIKE_OK) {
        as_event_close_loops();
        aerospike_destroy(as);
        return false;
    }

    return true;
}
