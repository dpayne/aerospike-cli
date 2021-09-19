#pragma once

#include <ascli/Operators/AerospikeOperator.h>
#include <aerospike/as_record.h>

namespace ascli {
class AerospikeQueryOperator : public AerospikeOperator {
   private:
    static bool query_callback(const as_val* p_val, void* udata);

   public:
    AerospikeQueryOperator(AeroOperatorIn operatorIn);

    auto query() const -> bool;
};
}  // namespace ascli

