#pragma once

#include <algorithm/BaseMRP.hpp>

namespace MRP_Algorithm
{

class FixedPeriod : public BaseMRP

{
public:
    explicit FixedPeriod(std::size_t period,
                         std::size_t planning_horizon,
                         std::size_t security_stock = 0,
                         double emision_cost = 1,
                         double hold_cost = 1);

    void set_period(std::size_t period);

    virtual void calculate();

private:
    std::size_t period_;
};

} // namespace MRP_Algorithm
