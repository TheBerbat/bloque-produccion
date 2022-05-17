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

} // namespace MRP_Algorithm
