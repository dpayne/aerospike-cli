#pragma once

#include <aerospike/aerospike_key.h>
#include <aerospike/as_record.h>
#include <aerospike/as_record_iterator.h>
#include <ascli/DataTypes.h>
#include <ascli/Operators/AerospikeOperator.h>
#include <charconv>
#include <iostream>

namespace ascli {
class AerospikePutOperator : public AerospikeOperator {
   private:
   public:
    AerospikePutOperator(AeroOperatorIn operatorIn);

    [[nodiscard]] auto put(data_type dt, const std::string& value) const -> bool;
    auto initialize_record(data_type dt, char * value, uint64_t value_size, as_record* rec) const -> bool;
};
}  // namespace ascli
