#pragma once

#include <string>

#include <aerospike/aerospike.h>

namespace ascli {

struct AeroOperatorIn_s {
    std::string ns;
    std::string set;
    std::string key;
    std::string bin;

    std::ostream& out;
    aerospike* as;
};

using AeroOperatorIn = struct AeroOperatorIn_s;

/**
 * @brief Aerospike operations
 */
class AerospikeOperator {
   private:
    AeroOperatorIn m_operator_in;

   protected:
    [[nodiscard]] auto get_operator_in() const -> const AeroOperatorIn& { return m_operator_in; }

   public:
    AerospikeOperator(AeroOperatorIn operatorIn);
};

}  // namespace ascli
