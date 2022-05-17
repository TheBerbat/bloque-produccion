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
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        std::int64_t net_needs = static_cast<int64_t>(needs_.at(i) - (availability_.at(i) - security_stock_) - receptions_.at(i));

        if (net_needs>0)
        {
            net_needs_.at(i) = static_cast<std::size_t>(net_needs);
            ppl_.at(i) = net_needs_.at(i);
        }
        else
        {
            net_needs_.at(i) = 0;
            ppl_.at(i) = 0;
        }

        availability_.at(i+1) = availability_.at(i) - needs_.at(i) + receptions_.at(i) + ppl_.at(i);
    }

    for (std::size_t i{0} ; i<planning_horizon_ ; i+=period_)
    {
        for (std::size_t p{i+1} ; p<i+period_ && p<planning_horizon_ ; ++p)
        {
            ppl_.at(i) += ppl_.at(p);
            ppl_.at(p) = 0;
        }
    }
    
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        std::int64_t net_needs = static_cast<int64_t>(needs_.at(i) - (availability_.at(i) - security_stock_) - receptions_.at(i));

        if (net_needs>0)
            net_needs_.at(i) = static_cast<std::size_t>(net_needs);
        else
            net_needs_.at(i) = 0;
            
        availability_.at(i+1) = availability_.at(i) - needs_.at(i) + receptions_.at(i) + ppl_.at(i);
    }
    
}

} // namespace MRP_Algorithm
