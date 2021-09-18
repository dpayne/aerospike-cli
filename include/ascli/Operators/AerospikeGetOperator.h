#include <ascli/Operators/AerospikeOperator.h>
#include <aerospike/as_record.h>

namespace ascli {
class AerospikeGetOperator : public AerospikeOperator {
   private:
   public:
    AerospikeGetOperator(AeroOperatorIn operatorIn);

    auto get() const -> bool;
    static auto print_record(const as_record * rec, const std::string & bin_name, std::ostream &out) -> std::ostream &;
    static auto print_bin(const as_bin * bin, std::ostream &out) -> std::ostream &;
    static auto print_aerospike_type(as_val_t type) -> std::string;
};
}  // namespace ascli
