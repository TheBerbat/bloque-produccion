#pragma once

#include <algorithm/FixedPeriod.hpp>

namespace MRP_Algorithm
{

class FixedPeriodOptimal : public FixedPeriod

{
public:
    explicit FixedPeriodOptimal(std::size_t planning_horizon,
                                std::size_t security_stock = 0,
                                double emision_cost = 1,
                                double hold_cost = 1);

    void calculate();

private:
    std::size_t period_;
};

} // namespace MRP_Algorithm
