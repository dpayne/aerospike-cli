#pragma once

#include <aerospike/as_record.h>
#include <ascli/Operators/AerospikeOperator.h>

namespace ascli {
class AerospikeDeleteOperator : public AerospikeOperator {
   private:
   public:
    AerospikeDeleteOperator(AeroOperatorIn operatorIn);

    auto del() const -> bool;
};
}  // namespace ascli
