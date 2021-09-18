#pragma once

#include <ascli/Operators/AerospikeOperator.h>
#include <aerospike/as_record.h>

namespace ascli {
class AerospikeScanOperator : public AerospikeOperator {
   private:
    static bool scan_callback(const as_val* p_val, void* udata);

   public:
    AerospikeScanOperator(AeroOperatorIn operatorIn);

    auto scan() const -> bool;
};
}  // namespace ascli



