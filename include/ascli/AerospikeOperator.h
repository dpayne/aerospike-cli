#pragma once

#include <string>

#include <aerospike/aerospike.h>

namespace ascli {

/**
 * @brief Aerospike operations
 */
class AerospikeOperator {
   private:

   public:

    auto get( const std::string & ns, const std::string & set_name, const std::string & key, aerospike * as, std::ostream & out) const -> void;
};

}  // namespace ascli

