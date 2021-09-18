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

    as_policy_scan scan_policy{};
    as_policy_scan_init(&scan_policy);

    auto opIn = get_operator_in();

    as_scan scan;
    as_scan_init(&scan, opIn.ns.c_str(), opIn.set.c_str());

    as_partition_filter pf;
    as_partition_filter_set_all(&pf);

    auto status = aerospike_scan_partitions(opIn.as, &err, nullptr, &scan, &pf, scan_callback, &opIn);
    if (status != AEROSPIKE_OK) {
        as_scan_destroy(&scan);
        return false;
    }

    as_scan_destroy(&scan);

    return true;
}

bool AerospikeScanOperator::scan_callback(const as_val* val, void* udata) {
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

