#include <algorithm/EOQ.hpp>
#include <cmath>

namespace MRP_Algorithm
{

EOQ::EOQ(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
{}

void EOQ::calculate()
{
    // Q* = sqrt((2*D*S)/(H))
    auto calcQ = [](std::size_t all_demand, std::size_t num_periods, double emision_cost, double hold_cost){
        double Q { std::sqrt( (2.0 * static_cast<double>(all_demand) * emision_cost)/(static_cast<double>(num_periods)*hold_cost))};
        Q = std::ceil(Q);
        return static_cast<std::size_t>(Q);
    };

    auto get_multiple_ceil = [](std::size_t num, std::size_t multiple){
        std::size_t num_multiple = num / multiple;
        if ((num%multiple) > 0) ++num_multiple;
        return num_multiple*num;
    };

    const std::size_t batch_size {calcQ(get_all_demand(), planning_horizon_, emision_cost_, hold_cost_)};

    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        ppl_.at(i) = get_multiple_ceil(net_needs_.at(i), batch_size);
        availability_.at(i+1) = calculate_availability(i+1);
    }
}

} // namespace MRP_Algorithm
