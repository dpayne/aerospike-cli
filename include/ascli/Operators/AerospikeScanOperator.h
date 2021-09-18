#include <ascli/Operators/AerospikeOperator.h>
#include <aerospike/as_record.h>

namespace ascli {
class AerospikeScanOperator : public AerospikeOperator {
   private:
    static auto scan_callback(const as_val* p_val, void* udata) -> bool;

   public:
    AerospikeScanOperator(AeroOperatorIn operatorIn);

    auto scan() const -> bool;
};
}  // namespace ascli

