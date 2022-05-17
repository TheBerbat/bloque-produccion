#include <algorithm/FixedPeriodOptimal.hpp>
#include <cmath>

namespace MRP_Algorithm
{

FixedPeriodOptimal::FixedPeriodOptimal(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : FixedPeriod(planning_horizon, security_stock, emision_cost, hold_cost)
{}

void FixedPeriodOptimal::calculate()
{
    auto calcPeriod = [](std::size_t all_demand, std::size_t num_periods, double emision_cost, double hold_cost){
        double T { static_cast<double>(num_periods)*std::sqrt( (2.0 * emision_cost)/(static_cast<double>(all_demand)*static_cast<double>(num_periods)*hold_cost))};
        T = std::round(T);
        return static_cast<std::size_t>(T);
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

    std::size_t period = calcPeriod(calc_demand(needs_, receptions_, availability_.at(0), security_stock_), planning_horizon_, emision_cost_, hold_cost_);
    set_period(period);
    FixedPeriod::calculate();
}

} // namespace MRP_Algorithm
