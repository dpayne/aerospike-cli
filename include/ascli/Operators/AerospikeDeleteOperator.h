#pragma once

#include <ascli/Operators/AerospikeOperator.h>
#include <aerospike/as_record.h>

namespace ascli {
class AerospikeDeleteOperator : public AerospikeOperator {
   private:
   public:
    AerospikeDeleteOperator(AeroOperatorIn operatorIn);

    auto del() const -> bool;
};
}  // namespace ascli

