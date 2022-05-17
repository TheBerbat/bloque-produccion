#include <algorithm/FixedPeriod.hpp>
#include <cmath>

namespace MRP_Algorithm
{

FixedPeriod::FixedPeriod(std::size_t period, std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
  , period_{period}
{}

void FixedPeriod::set_period(std::size_t period)
{
    calculated_ = false;
    period_ = period;
}

void FixedPeriod::calculate()
{
    // MAKE BATCH TO BATCH
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        ppl_.at(i) = net_needs_.at(i);
        availability_.at(i+1) = calculate_availability(i+1);
    }

    // BRING FORWARD PPL TO GROUP IN CONSTANT PERIODS
    // AND OPTIMIZE FIRST ORDER LAUNCH
    std::size_t first_launch {0};
    while(first_launch<planning_horizon_ && ppl_.at(first_launch)==0)
        first_launch++;

    std::size_t p{ first_launch };
    for (std::size_t i{first_launch+1} ; i<planning_horizon_ ; ++i)
    {
        if ((i-1)/period_ != i/period_)
        {
            p = period_*(i/period_);
            continue;
        }
        ppl_.at(p) += ppl_.at(i);
        ppl_.at(i) = 0;
    }

    // RECALCULATE NET_NEEDS AND AVAILABILITY WHITH PRECALCULATED PLL
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        availability_.at(i+1) = calculate_availability(i+1);
    }

}

} // namespace MRP_Algorithm
