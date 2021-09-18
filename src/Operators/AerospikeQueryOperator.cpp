#include <aerospike/aerospike_key.h>
#include <aerospike/aerospike_query.h>
#include <aerospike/as_query.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <ascli/Operators/AerospikeGetOperator.h>
#include <ascli/Operators/AerospikeQueryOperator.h>
#include <iostream>
#include <unordered_map>

using namespace ascli;

AerospikeQueryOperator::AerospikeQueryOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikeQueryOperator::query() const -> bool {
    as_error err{};

    as_policy_query query_policy{};
    as_policy_query_init(&query_policy);

    auto opIn = get_operator_in();

    as_query query;
    as_query_init(&query, opIn.ns.c_str(), opIn.set.c_str());

    if (aerospike_query_foreach(opIn.as, &err, &query_policy, &query, query_callback, &opIn) != AEROSPIKE_OK) {
        as_query_destroy(&query);
        return false;
    }

    as_query_destroy(&query);

    return true;
}

bool AerospikeQueryOperator::query_callback(const as_val* val, void* udata) {
    auto opIn = (AeroOperatorIn*)udata;
    if (val == nullptr) {
        return true;
    }

    auto rec = as_record_fromval(val);

    if (rec == nullptr) {
        return true;
    }

    AerospikeGetOperator::print_record(rec, opIn->bin, opIn->out);
    opIn->out << std::endl;

    return true;
}
