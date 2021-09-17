#include <ascli/ascli.h>
#include <fmt/format.h>

#include <aerospike/aerospike_batch.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/aerospike_scan.h>
#include <aerospike/as_policy.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>

#include <uv.h>

using namespace ascli;

AsCli::AsCli(std::string host, uint32_t port, std::string user, std::string pass)
    : m_host{std::move(host)}, m_port{port}, m_user{std::move(user)}, m_pass{std::move(pass)}, m_scheduler{}, m_cli{get_menu(&m_scheduler, &m_aerospike)} {}

auto AsCli::get_menu(cli::LoopScheduler* scheduler, aerospike * as) const -> std::unique_ptr<cli::Menu> {
    auto rootMenu = std::make_unique<cli::Menu>("cli");

    rootMenu->Insert(
        "get",
        [as,scheduler](std::ostream& out, std::string ns, std::string set, std::string key) {
            out << "The answer is: " << key << "\n";
            as_error err;
            as_record* p_rec = nullptr;
            as_key akey;
            as_key_init_str(&akey, ns.c_str(), set.c_str(), key.c_str());

            // Read the test record from the database.
            if (aerospike_key_get(as, &err, nullptr, &akey, &p_rec) != AEROSPIKE_OK) {
                std::cerr << "aerospike_key_get() returned " << err.code << " " << err.message << std::endl;
                return;
            }

            // If we didn't get an as_record object back, something's wrong.
            if (p_rec == nullptr) {
                std::cerr << "aerospike_key_get() retrieved null as_record object" << std::endl;
                return;
            }

            // Log the result.
            out << ("record was successfully read from database:") << std::endl;

            // Destroy the as_record object.
            as_record_destroy(p_rec);
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
