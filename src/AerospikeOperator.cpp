#include <ascli/AerospikeOperator.h>
#include <aerospike/aerospike_key.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <iostream>

using namespace ascli;

auto AerospikeOperator::get( const std::string & ns, const std::string & set_name, const std::string & key, aerospike * as, std::ostream & out) const -> void {
    as_error err;
    as_record* p_rec = nullptr;
    as_key akey;
    as_key_init_str(&akey, ns.c_str(), set_name.c_str(), key.c_str());

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
}
