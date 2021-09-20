#pragma once

#include <aerospike/as_record.h>
#include <ascli/Operators/AerospikeOperator.h>

namespace ascli {
class AerospikeScanOperator : public AerospikeOperator {
   private:
    static auto scan_callback(const as_val* p_val, void* udata) -> bool;

   public:
    AerospikeScanOperator(AeroOperatorIn operatorIn);

    [[nodiscard]] auto scan() const -> bool;
};
}  // namespace ascli
