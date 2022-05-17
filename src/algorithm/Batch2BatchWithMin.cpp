#include <algorithm/Batch2BatchWithMin.hpp>

namespace MRP_Algorithm
{

Batch2BatchWithMin::Batch2BatchWithMin(std::size_t min_size, std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
  , min_size_{min_size}
{}

void Batch2BatchWithMin::calculate()
{
    auto trim_minimum_size = [](std::size_t num, std::size_t minimum){
        return 0<num && num<minimum ? minimum : num;
    };

    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        ppl_.at(i) = trim_minimum_size(net_needs_.at(i), min_size_);
        availability_.at(i+1) = calculate_availability(i+1);
    }
}

} // namespace MRP_Algorithm
