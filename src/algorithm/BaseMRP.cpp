#include <algorithm/BaseMRP.hpp>

namespace MRP_Algorithm
{

BaseMRP::BaseMRP(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
    : needs_(planning_horizon)
    , receptions_(planning_horizon)
    , availability_(planning_horizon+1)
    , net_needs_(planning_horizon)
    , ppl_(planning_horizon)
    , planning_horizon_{planning_horizon}
    , period_ship_{0}
    , calculated_{false}
    , emision_cost_{emision_cost}
    , hold_cost_{hold_cost}
    , security_stock_{security_stock}
{
    availability_.at(0) = security_stock;
}

void BaseMRP::set_need(std::size_t period, std::size_t count)
{
    calculated_=false;
    needs_.at(period) = count;
}
void BaseMRP::insert_need(std::size_t period, std::size_t count)
{
    calculated_=false;
    needs_.at(period) += count;
}

void BaseMRP::set_receptions(std::size_t period, std::size_t count)
{
    calculated_=false;
    receptions_.at(period) = count;
}
void BaseMRP::insert_receptions(std::size_t period, std::size_t count)
{
    calculated_=false;
    receptions_.at(period) += count;
}

void BaseMRP::set_initial_availability(std::size_t availability)
{
    calculated_=false;
    availability_[0] = availability;
}

void BaseMRP::set_costs(double emision_cost, double hold_cost)
{
    calculated_=false;
    emision_cost_ = emision_cost;
    hold_cost_    = hold_cost;
}
void BaseMRP::set_ship_period(std::size_t period)
{
    period_ship_ = period;
}
std::size_t BaseMRP::get_ship_period() const
{
    return period_ship_;
}

std::vector<std::size_t> BaseMRP::get_needs()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    return needs_;
}
std::vector<std::size_t> BaseMRP::get_availability()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    return availability_;
}
std::vector<std::size_t> BaseMRP::get_receptions()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    return receptions_;
}
std::vector<std::size_t> BaseMRP::get_net_needs()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    return net_needs_;
}
std::vector<std::size_t> BaseMRP::get_ppl()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    return ppl_;
}
std::vector<std::size_t> BaseMRP::get_lppl()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    std::vector<std::size_t> lppl(planning_horizon_+period_ship_);
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
        lppl.at(i) = ppl_.at(i);
    for(std::size_t i{planning_horizon_} ; i<planning_horizon_+period_ship_ ; ++i)
        lppl.at(i) = 0;
    return lppl;
}

std::size_t BaseMRP::get_all_demand() const
{
    std::int64_t r {};

    // ALL_NEED - ALL_RECEPTIONS - INITIAL_STOCK(WITHOUT SECURITY STOCK)
    for (auto it: needs_) r += static_cast<int64_t>(it);
    for (auto it: receptions_) r -= static_cast<int64_t>(it);
    r += static_cast<int64_t>(security_stock_);
    r -= static_cast<int64_t>(availability_.at(0));

    // TRIM MINIMUM TO 0
    r = r>0 ? r : 0;

    return static_cast<std::size_t>(r);
}

std::size_t BaseMRP::calculate_net_needs(std::size_t idx) const
{
    auto trim_to_size_t = [](std::int64_t num){
        return num>0 ? static_cast<std::size_t>(num) : 0;
    };
    
    std::int64_t net_needs = static_cast<int64_t>(needs_.at(idx)) - 
                           ( static_cast<int64_t>(availability_.at(idx)) -  
                             static_cast<int64_t>(security_stock_) ) - 
                             static_cast<int64_t>(receptions_.at(idx));

    return trim_to_size_t(net_needs);
}

std::size_t BaseMRP::calculate_availability(std::size_t idx) const
{
    auto trim_to_size_t = [](std::int64_t num){
        return num>0 ? static_cast<std::size_t>(num) : 0;
    };
    
    std::int64_t availability = static_cast<int64_t>(availability_.at(idx-1)) - 
                                static_cast<int64_t>(needs_.at(idx-1)) + 
                                static_cast<int64_t>(receptions_.at(idx-1)) + 
                                static_cast<int64_t>(ppl_.at(idx-1));

    return trim_to_size_t(availability);
}

double BaseMRP::get_hold_costs(bool include_security_stock)
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    double cost {0.0};
    for (auto it {availability_.begin()} ; it!=availability_.end()-1 ; ++it)
    {
        double net_availability {static_cast<double>(*it)};
        if (!include_security_stock)
        {
            if (net_availability <= static_cast<double>(security_stock_))
                net_availability = 0.0;
            else
                net_availability -= static_cast<double>(security_stock_);
        }

        cost += hold_cost_ * net_availability;
    }

    return cost;
}
double BaseMRP::get_emision_costs()
{
    if(!calculated_)
    {
        this->calculate();
        calculated_ = true;
    }
    double cost {0.0};
    for (auto it {ppl_.begin()} ; it!=ppl_.end() ; ++it)
        if (*it >0)
        cost += emision_cost_;

    return cost;
}

double BaseMRP::get_total_costs()
{
    return get_hold_costs() + get_emision_costs();
}

} // namespace MRP_Algorithm
