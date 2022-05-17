#include <algorithm/Batch2Batch.hpp>

namespace MRP_Algorithm
{

Batch2Batch::Batch2Batch(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : Batch2BatchWithMin(0, planning_horizon, security_stock, emision_cost, hold_cost)
{}

} // namespace MRP_Algorithm
