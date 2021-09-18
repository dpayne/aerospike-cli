#include <aerospike/aerospike_key.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <ascli/Operators/AerospikeGetOperator.h>
#include <iostream>

using namespace ascli;

AerospikeGetOperator::AerospikeGetOperator(AeroOperatorIn operatorIn) : AerospikeOperator(std::move(operatorIn)) {}

auto AerospikeGetOperator::get() const -> bool {
    as_error err{};
    as_record* rec = nullptr;
    as_key akey{};
    auto opIn = get_operator_in();
    as_key_init_str(&akey, opIn.ns.c_str(), opIn.set.c_str(), opIn.key.c_str());

    // Read the test record from the database.
    if (aerospike_key_get(opIn.as, &err, nullptr, &akey, &rec) != AEROSPIKE_OK) {
        std::cerr << "aerospike_key_get() returned " << err.code << " " << err.message << std::endl;
        return false;
    }

    // If we didn't get an as_record object back, something's wrong.
    if (rec == nullptr) {
        std::cerr << "aerospike_key_get() retrieved null as_record object" << std::endl;
        return false;
    }

    print_record(rec, opIn.out) << std::endl;

    // Destroy the as_record object.
    as_record_destroy(rec);

    return true;
}

auto AerospikeGetOperator::print_record(const as_record* rec, std::ostream& out) -> std::ostream & {
    out << "{"
        << "\"gen\":" << rec->gen << ", "
        << "\"ttl\":" << rec->ttl << ", "
        << "\"bins\":[";
    as_record_iterator it;
    as_record_iterator_init(&it, rec);
    while(as_record_iterator_has_next(&it))
    {
        auto bin = as_record_iterator_next(&it);
        out << as_val_tostring(bin);
    }
    out << "]";
    out << "}";
    as_record_iterator_destroy(&it);

    return out;
}
