#pragma once

#include <aerospike/as_record.h>
#include <ascli/Operators/AerospikeOperator.h>

namespace ascli {
class AerospikeQueryOperator : public AerospikeOperator {
   private:
    static auto query_callback(const as_val* p_val, void* udata) -> bool;

   public:
    AerospikeQueryOperator(AeroOperatorIn operatorIn);

    [[nodiscard]] auto query() const -> bool;
};
}  // namespace ascli
