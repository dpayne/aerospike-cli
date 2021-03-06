#include <aerospike/aerospike_key.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <ascli/Operators/AerospikeGetOperator.h>
#include <ascli/base64.h>
#include <iostream>
#include <unordered_map>

using namespace ascli;

AerospikeGetOperator::AerospikeGetOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikeGetOperator::print_aerospike_type(as_val_t type) -> std::string {
    switch (type) {
        case AS_UNKNOWN:
            return "unknown";
        case AS_NIL:
            return "nil";
        case AS_BOOLEAN:
            return "boolean";
        case AS_INTEGER:
            return "numeric";
        case AS_STRING:
            return "string";
        case AS_BYTES:
            return "bytes";
        case AS_DOUBLE:
            return "decimal";
        case AS_LIST:
            return "list";
        case AS_MAP:
            return "MAP";
        case AS_REC:
            return "rec";
        case AS_PAIR:
            return "pair";
        case AS_GEOJSON:
            return "geojson";
        default:
            return "unsupported";
    }
}

auto AerospikeGetOperator::get() const -> bool {
    as_error err{};
    as_record* rec = nullptr;
    as_key akey{};
    auto opIn = get_operator_in();
    as_key_init_str(&akey, opIn.ns.c_str(), opIn.set.c_str(), opIn.key.c_str());

    // Read the test record from the database.
    if (aerospike_key_get(opIn.as, &err, nullptr, &akey, &rec) != AEROSPIKE_OK) {
        std::cerr << "aerospike_key_get() returned " << err.code << " " << static_cast<const char*>(err.message) << std::endl;
        return false;
    }

    // If we didn't get an as_record object back, something's wrong.
    if (rec == nullptr) {
        std::cerr << "aerospike_key_get() retrieved null as_record object" << std::endl;
        return false;
    }

    print_record(rec, opIn.bin, opIn.out) << std::endl;

    // Destroy the as_record object.
    as_record_destroy(rec);

    return true;
}

auto AerospikeGetOperator::print_record(const as_record* rec, const std::string& bin_name, std::ostream& out) -> std::ostream& {
    out << "{";
    if (rec->key.valuep != nullptr) {
        auto key_val_as_str = as_val_tostring(rec->key.valuep);  // NOLINT
        out << "\"key\":" << key_val_as_str << ", ";
        free(key_val_as_str);  // NOLINT
    }

    out << "\"gen\":" << rec->gen << ", "
        << "\"ttl\":" << rec->ttl << ", "
        << "\"bins\":[";
    as_record_iterator it;
    as_record_iterator_init(&it, rec);
    std::string prefix = "";
    while (as_record_iterator_has_next(&it)) {
        auto* bin = as_record_iterator_next(&it);
        if (!bin_name.empty()) {
            if (strncmp(static_cast<const char*>(bin->name), bin_name.c_str(), bin_name.size()) != 0) {
                continue;
            }
        }
        if (bin != nullptr) {
            out << prefix;
            print_bin(bin, out);
            prefix = ", ";
        }
    }
    out << "]";
    out << "}";
    as_record_iterator_destroy(&it);

    return out;
}

auto AerospikeGetOperator::val_to_string(as_bin_value* value) -> std::string {
    auto* val = reinterpret_cast<as_val*>(value);
    std::string str;
    if (val->type == AS_BYTES) {
        auto bin_str = std::string{reinterpret_cast<const char*>(value->bytes.value), value->bytes.size};
        str = "\"";
        str.append(macaron::Base64::Encode(bin_str)).append("\"");
    } else if (val->type == AS_STRING) {
        auto bin_str = std::string{reinterpret_cast<const char*>(value->string.value), value->string.len};
        str = "\"";
        str.append(bin_str).append("\"");
    } else {
        auto* val_str = as_val_tostring(val);
        str = std::string{val_str};

        free(val_str);  // NOLINT
    }

    return str;
}

auto AerospikeGetOperator::print_bin(const as_bin* bin, std::ostream& out) -> std::ostream& {
    auto* value = as_bin_get_value(bin);
    if (value == nullptr) {
        return out;
    }
    auto* val = reinterpret_cast<as_val*>(value);
    auto val_str = val_to_string(value);

    out << "{"
        << R"("name": ")" << static_cast<const char*>(bin->name) << R"(", )";
    out << R"("type": ")" << print_aerospike_type(val->type) << R"(", )";
    out << R"("value": )" << val_str;
    out << "}";

    return out;
}
