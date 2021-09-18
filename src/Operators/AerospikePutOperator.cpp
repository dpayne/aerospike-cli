#include <ascli/Operators/AerospikePutOperator.h>
#include <chrono>

using namespace ascli;

namespace
{
    const std::chrono::seconds k_default_ttl{std::chrono::duration_cast<std::chrono::seconds>(std::chrono::days{1})};
}

AerospikePutOperator::AerospikePutOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikePutOperator::put(data_type dt, const std::string& value) const -> bool {
    as_error err{};
    as_record rec{};
    as_key akey{};
    as_policy_write write_policy{};
    as_policy_write_init(&write_policy);
    auto opIn = get_operator_in();
    as_key_init_str(&akey, opIn.ns.c_str(), opIn.set.c_str(), opIn.key.c_str());

    initialize_record(dt, value, &rec);

    // Read the test record from the database.
    if (aerospike_key_put(opIn.as, &err, nullptr, &akey, &rec) != AEROSPIKE_OK) {
        std::cerr << "aerospike_key_put() returned " << err.code << " " << err.message << std::endl;
        as_record_destroy(&rec);
        return false;
    }

    // Log the result.
    opIn.out << ("record was successfully written to the database") << std::endl;

    // Destroy the as_record object.
    as_record_destroy(&rec);

    return true;
}

auto AerospikePutOperator::initialize_record(data_type dt, const std::string& value, as_record* rec) const -> bool {
    as_record_init(rec, 1);
    rec->ttl = k_default_ttl.count();
    auto opIn = get_operator_in();
    auto bin_name = opIn.bin.empty() ? nullptr : opIn.bin.data();
    auto val = 0L;
    char *end{};
    char *endResult = end;
    switch (dt) {
        case data_type::bytes:
            as_bytes value_bytes;
            value_bytes.value = (uint8_t*)value.data();
            value_bytes.size = value.size();
            value_bytes.type = as_bytes_type::AS_BYTES_BLOB;
            as_record_set_bytes(rec, bin_name, &value_bytes);
                return true;
        case data_type::numeric:
            if (auto [p, ec] = std::from_chars(value.data(), value.data() + value.size(), val); ec == std::errc()) {
                as_record_set_int64(rec, bin_name, val);
                return true;
            } else {
                as_record_destroy(rec);
                std::cerr << "Could not read value with type int" << std::endl;
                return false;
            }
        case data_type::decimal:
            end = (char*)(value.data() + value.size());
            endResult = end;
            as_record_set_double(rec, bin_name, std::strtod(value.data(), &endResult));
            return true;
        case data_type::string:
            as_record_set_str(rec, bin_name, value.c_str());
            return true;
        default:
            std::cerr << "no valid type found for put operation" << std::endl;
            as_record_destroy(rec);
            return false;
    };

    return true;
}
