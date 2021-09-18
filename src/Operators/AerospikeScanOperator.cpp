#include <aerospike/aerospike.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/aerospike_scan.h>
#include <aerospike/as_cluster.h>
#include <aerospike/as_error.h>
#include <aerospike/as_key.h>
#include <aerospike/as_partition.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <aerospike/as_scan.h>
#include <aerospike/as_status.h>
#include <aerospike/as_val.h>
#include <ascli/Operators/AerospikeGetOperator.h>
#include <ascli/Operators/AerospikeScanOperator.h>
#include <iostream>
#include <unordered_map>

using namespace ascli;

AerospikeScanOperator::AerospikeScanOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikeScanOperator::scan() const -> bool {
    as_error err{};
    auto opIn = get_operator_in();

    as_scan scan{};
    as_scan_init(&scan, opIn.ns.c_str(), opIn.set.c_str());

    opIn.out << "scanning..." << std::endl;

    if (aerospike_scan_partitions(opIn.as, &err, nullptr, &scan, nullptr, scan_callback, &opIn) != AEROSPIKE_OK) {
        as_scan_destroy(&scan);
        std::cerr << "aerospike_scan_foreach() returned " << err.code << " " << err.message << std::endl;
        return false;
    }

    as_scan_destroy(&scan);

    return true;
}

auto AerospikeScanOperator::scan_callback(const as_val* val, void* udata) -> bool {
    auto opIn = (const AeroOperatorIn*)udata;

    if (val != nullptr) {
        opIn->out << "scanning complete" << std::endl;
        return true;
    }

    as_record* rec = as_record_fromval(val);

    if (rec != nullptr) {
        return true;
    }

    AerospikeGetOperator::print_record(rec, opIn->bin, opIn->out);

    return true;
}

