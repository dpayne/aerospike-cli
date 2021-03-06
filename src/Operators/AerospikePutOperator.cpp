#include <ascli/Operators/AerospikePutOperator.h>
#include <chrono>

using namespace ascli;

namespace {
const std::chrono::seconds k_default_ttl{std::chrono::duration_cast<std::chrono::seconds>(std::chrono::days{1})};
}

AerospikePutOperator::AerospikePutOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikePutOperator::put(data_type dt, const std::string& value) const -> bool {
    as_error err{};
    as_record rec{};
    as_key akey{};

    as_policy_write write_policy{};
    as_policy_write_init(&write_policy);

    // save key
    write_policy.key = as_policy_key::AS_POLICY_KEY_SEND;

    auto opIn = get_operator_in();
    as_key_init_str(&akey, opIn.ns.c_str(), opIn.set.c_str(), opIn.key.c_str());

    initialize_record(dt, (char *) value.data(), value.size(), &rec); // NOLINT

    // Read the test record from the database.
    if (aerospike_key_put(opIn.as, &err, nullptr, &akey, &rec) != AEROSPIKE_OK) {
        std::cerr << "aerospike_key_put() returned " << err.code << " " << static_cast<const char*>(err.message) << std::endl;
        as_record_destroy(&rec);
        return false;
    }

    // Log the result.
    opIn.out << ("record was successfully written to the database") << std::endl;

    // Destroy the as_record object.
    as_record_destroy(&rec);

    return true;
}

auto AerospikePutOperator::initialize_record(data_type dt, char * value, uint64_t value_size, as_record* rec) const -> bool {
    as_record_init(rec, 1);
    rec->ttl = k_default_ttl.count();
    auto opIn = get_operator_in();
    const char* bin_name = opIn.bin.empty() ? nullptr : opIn.bin.data();
    auto val = 0L;
    auto* end = value;
    auto* endResult = end;
    switch (dt) {
        case data_type::bytes:
            as_bytes value_bytes;
            value_bytes.value = (uint8_t*)value;  // NOLINT
            value_bytes.size = value_size;
            value_bytes.type = as_bytes_type::AS_BYTES_BLOB;
            as_record_set_bytes(rec, bin_name, &value_bytes);
            return true;
        case data_type::numeric:
            if (auto [p, ec] = std::from_chars(value, value + value_size, val); ec == std::errc()) {  // NOLINT
                as_record_set_int64(rec, bin_name, val);
                return true;
            } else {
                as_record_destroy(rec);
                std::cerr << "Could not read value with type int" << std::endl;
                return false;
            }
        case data_type::decimal:
            end = value + value_size;  // NOLINT
            endResult = end;                                             // NOLINT
            as_record_set_double(rec, bin_name, std::strtod(value, &endResult));
            return true;
        case data_type::string:
            as_record_set_str(rec, bin_name, value);
            return true;
        default:
            std::cerr << "no valid type found for put operation" << std::endl;
            as_record_destroy(rec);
            return false;
    };

    return true;
}
