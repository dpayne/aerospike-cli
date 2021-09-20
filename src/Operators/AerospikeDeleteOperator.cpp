#include <aerospike/aerospike_key.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <ascli/Operators/AerospikeDeleteOperator.h>
#include <iostream>
#include <unordered_map>

using namespace ascli;

AerospikeDeleteOperator::AerospikeDeleteOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikeDeleteOperator::del() const -> bool {
    as_error err{};
    as_key akey{};
    auto opIn = get_operator_in();
    as_key_init_str(&akey, opIn.ns.c_str(), opIn.set.c_str(), opIn.key.c_str());

    if (aerospike_key_remove(opIn.as, &err, nullptr, &akey) != AEROSPIKE_OK) {
        std::cerr << "aerospike_key_remove() returned " << err.code << " " << std::string_view(&err.message[0]) << std::endl;
        return false;
    }

    return true;
}
