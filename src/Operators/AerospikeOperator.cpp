#include <ascli/Operators/AerospikeOperator.h>

using namespace ascli;

AerospikeOperator::AerospikeOperator(AeroOperatorIn operatorIn) : m_operator_in{std::move(operatorIn)} {}
