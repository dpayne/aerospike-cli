#pragma once

#include <aerospike/as_record.h>
#include <ascli/Operators/AerospikeOperator.h>

namespace ascli {
class AerospikeGetOperator : public AerospikeOperator {
   private:
   public:
    AerospikeGetOperator(AeroOperatorIn operatorIn);

    [[nodiscard]] auto get() const -> bool;
    static auto print_record(const as_record* rec, const std::string& bin_name, std::ostream& out) -> std::ostream&;
    static auto print_bin(const as_bin* bin, std::ostream& out) -> std::ostream&;
    static auto print_aerospike_type(as_val_t type) -> std::string;
    static auto val_to_string(as_bin_value* value) -> std::string;
};
}  // namespace ascli
