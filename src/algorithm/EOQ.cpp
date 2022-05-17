#include <algorithm/EOQ.hpp>
#include <cmath>

namespace MRP_Algorithm
{

EOQ::EOQ(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
{}

void EOQ::calculate()
{
    auto calcQ = [](std::size_t all_demand, std::size_t num_periods, double emision_cost, double hold_cost){
        double Q { std::sqrt( (2.0 * static_cast<double>(all_demand) * emision_cost)/(static_cast<double>(num_periods)*hold_cost))};
        Q = std::ceil(Q);
        return static_cast<std::size_t>(Q);
    };

    auto calc_demand = [](std::vector<std::size_t>& needs, std::vector<std::size_t>& receptions, std::size_t initial_availability, std::size_t security_stock){
        std::int64_t r {};

        for (auto it: needs) r += static_cast<int64_t>(it);
        for (auto it: receptions) r -= static_cast<int64_t>(it);
        r += static_cast<int64_t>(security_stock);
        r -= static_cast<int64_t>(initial_availability);

        r = r>0 ? r : 0;

        return static_cast<std::size_t>(r);
    };

    std::size_t batch_size = calcQ(calc_demand(needs_, receptions_, availability_.at(0), security_stock_), planning_horizon_, emision_cost_, hold_cost_);

    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        std::int64_t net_needs = static_cast<int64_t>(needs_.at(i) - (availability_.at(i) - security_stock_) - receptions_.at(i));

        if (net_needs>0)
        {
            net_needs_.at(i) = static_cast<std::size_t>(net_needs);
            std::size_t num_batchs = net_needs_.at(i) / batch_size;
            if (net_needs_.at(i) % batch_size>0) ++num_batchs;
            ppl_.at(i) = num_batchs*batch_size;
        }
        else
        {
            net_needs_.at(i) = 0;
            ppl_.at(i) = 0;
        }

        availability_.at(i+1) = availability_.at(i) - needs_.at(i) + receptions_.at(i) + ppl_.at(i);
    }
}

} // namespace MRP_Algorithm
