#include <ascli/Operators/AerospikeOperator.h>
#include <aerospike/as_record.h>

namespace ascli {
class AerospikeGetOperator : public AerospikeOperator {
   private:
   public:
    AerospikeGetOperator(AeroOperatorIn operatorIn);

    auto get() const -> bool;
    static auto print_record(const as_record * rec, std::ostream &out) -> std::ostream &;
};
}  // namespace ascli
