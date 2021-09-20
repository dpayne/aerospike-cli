#include <ascli/ascli.h>
#include <fmt/format.h>

#include <aerospike/as_event.h>
#include <aerospike/as_policy.h>
#include <ascli/DataTypes.h>
#include <ascli/Operators/AerospikeDeleteOperator.h>
#include <ascli/Operators/AerospikeGetOperator.h>
#include <ascli/Operators/AerospikeOperator.h>
#include <ascli/Operators/AerospikePutOperator.h>
#include <ascli/Operators/AerospikeQueryOperator.h>
#include <ascli/Operators/AerospikeScanOperator.h>

#include <uv.h>

using namespace ascli;

namespace {
constexpr std::string_view k_error_msg = "\033[1;31merror: \033[0m";
const std::unordered_map<std::string_view, data_type> k_str_to_data_type{
    {"int", data_type::numeric},   {"long", data_type::numeric}, {"double", data_type::decimal}, {"float", data_type::decimal},
    {"string", data_type::string}, {"str", data_type::string},   {"bytes", data_type::bytes},    {"blob", data_type::bytes},
};
}  // namespace

AsCli::AsCli(std::string host, uint32_t port, std::string user, std::string pass)
    : m_host{std::move(host)}, m_port{port}, m_user{std::move(user)}, m_pass{std::move(pass)}, m_cli{get_menu(&m_aerospike)} {}

AsCli::~AsCli() {
    if (m_is_aerospike_initialized) {
        aerospike_destroy(&m_aerospike);
    }
}

auto AsCli::get_menu(aerospike* as) const -> std::unique_ptr<cli::Menu> {
    auto rootMenu = std::make_unique<cli::Menu>(m_host);

    setup_get_ops(as, rootMenu.get());
    setup_put_ops(as, rootMenu.get());
    setup_delete_ops(as, rootMenu.get());
    setup_query_ops(as, rootMenu.get());
    setup_scan_ops(as, rootMenu.get());

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
    as_event_set_external_loop_capacity(1);         // Tell C client the maximum number of event loops that will be shared.
    as_event_set_external_loop(uv_default_loop());  // The number of times this function is called has to match as_event_set_external_loop_capacity()

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

auto AsCli::setup_get_ops(aerospike* as, cli::Menu* menu) const -> void {
    menu->Insert(
        "get",
        [as](std::ostream& out, std::string ns, std::string set, std::string key, std::string bin) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .key = std::move(key), .bin = std::move(bin), .out = out, .as = as};
            AerospikeGetOperator op(std::move(opIn));
            if (op.get()) {
                out << k_error_msg << "could not get value" << std::endl;
            }
        },
        "Get key from aerospike with bin");
    menu->Insert(
        "get",
        [as](std::ostream& out, std::string ns, std::string set, std::string key) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .key = std::move(key), .bin = "", .out = out, .as = as};
            AerospikeGetOperator op(std::move(opIn));
            if (op.get()) {
                out << k_error_msg << "could not get value" << std::endl;
            }
        },
        "Get key from aerospike with bin");
}

auto AsCli::setup_put_ops(aerospike* as, cli::Menu* menu) const -> void {
    menu->Insert(
        "put",
        [as](std::ostream& out, std::string ns, std::string set, std::string key, std::string bin, const std::string& type,
             const std::string& value) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .key = std::move(key), .bin = std::move(bin), .out = out, .as = as};
            AerospikePutOperator op(std::move(opIn));
            auto findPair = k_str_to_data_type.find(type);
            if (findPair == k_str_to_data_type.end()) {
                std::cerr << "Invalid data type: " << type << ", valid types are [int, double, bool, string, bytes]" << std::endl;
            } else {
                if (op.put(findPair->second, value)) {
                    out << k_error_msg << "could not put value" << std::endl;
                }
            }
        },
        "Put key-value into aerospike with bin");
    menu->Insert(
        "put",
        [as](std::ostream& out, std::string ns, std::string set, std::string key, const std::string& type, const std::string& value) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .key = std::move(key), .bin = "default", .out = out, .as = as};
            AerospikePutOperator op(std::move(opIn));
            auto findPair = k_str_to_data_type.find(type);
            if (findPair == k_str_to_data_type.end()) {
                std::cerr << "Invalid data type: " << type << ", valid types are [int, double, bool, string, bytes]" << std::endl;
            } else {
                if (op.put(findPair->second, value)) {
                    out << k_error_msg << "could not put value" << std::endl;
                }
            }
        },
        "Put key-value into aerospike");
}

auto AsCli::setup_delete_ops(aerospike* as, cli::Menu* menu) const -> void {
    menu->Insert(
        "delete",
        [as](std::ostream& out, std::string ns, std::string set, std::string key) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .key = std::move(key), .out = out, .as = as};
            AerospikeDeleteOperator op(std::move(opIn));
            if (op.del()) {
                out << k_error_msg << "could not delete value" << std::endl;
            }
        },
        "delete set in aerospike");
}

auto AsCli::setup_query_ops(aerospike* as, cli::Menu* menu) const -> void {
    menu->Insert(
        "query",
        [as](std::ostream& out, std::string ns, std::string set) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .out = out, .as = as};
            AerospikeQueryOperator op(std::move(opIn));
            if (op.query()) {
                out << k_error_msg << "could not run query" << std::endl;
            }
        },
        "query set in aerospike");
}

auto AsCli::setup_scan_ops(aerospike* as, cli::Menu* menu) const -> void {
    menu->Insert(
        "scan",
        [as](std::ostream& out, std::string ns, std::string set) {
            AeroOperatorIn opIn = {.ns = std::move(ns), .set = std::move(set), .out = out, .as = as};
            AerospikeScanOperator op(std::move(opIn));
            if (op.scan()) {
                out << k_error_msg << "could not run scan" << std::endl;
            }
        },
        "scan set in aerospike");
}
