#include <algorithm/Batch2BatchWithMin.hpp>

namespace MRP_Algorithm
{

Batch2BatchWithMin::Batch2BatchWithMin(std::size_t min_size, std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
  , min_size_{min_size}
{}

void Batch2BatchWithMin::calculate()
{
    if (calculated_)
        return;
    calculated_=true;

    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        std::int64_t net_needs = static_cast<int64_t>(needs_.at(i) - (availability_.at(i) - security_stock_) - receptions_.at(i));
        net_needs_.at(i) = net_needs<=0 ? 0 : static_cast<std::size_t>(net_needs);

        ppl_.at(i) = 0<net_needs_.at(i) && net_needs_.at(i)<min_size_ ? min_size_ : net_needs_.at(i);

        availability_.at(i+1) = availability_.at(i) - needs_.at(i) + receptions_.at(i) + ppl_.at(i);
    }
}

} // namespace MRP_Algorithm
