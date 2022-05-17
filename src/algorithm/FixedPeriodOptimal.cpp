#include <algorithm/FixedPeriodOptimal.hpp>
#include <cmath>

namespace MRP_Algorithm
{

FixedPeriodOptimal::FixedPeriodOptimal(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : FixedPeriod(1, planning_horizon, security_stock, emision_cost, hold_cost)
{}

void FixedPeriodOptimal::calculate()
{
    auto calcPeriod = [](std::size_t all_demand, std::size_t num_periods, double emision_cost, double hold_cost){
        double T { static_cast<double>(num_periods)*std::sqrt( (2.0 * emision_cost)/(static_cast<double>(all_demand)*static_cast<double>(num_periods)*hold_cost))};
        T = std::round(T);
        return static_cast<std::size_t>(T);
    };

    std::size_t period = calcPeriod(get_all_demand(), planning_horizon_, emision_cost_, hold_cost_);
    set_period(period);
    FixedPeriod::calculate();
}

} // namespace MRP_Algorithm
