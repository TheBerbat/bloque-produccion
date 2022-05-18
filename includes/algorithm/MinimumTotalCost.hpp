#pragma once

#include <algorithm/BaseMRP.hpp>

namespace MRP_Algorithm
{

class MinimumTotalCost : public BaseMRP

{
public:
    explicit MinimumTotalCost(std::size_t planning_horizon,
                                std::size_t security_stock = 0,
                                double emision_cost = 1,
                                double hold_cost = 1);

    virtual void calculate();
};

} // namespace MRP_Algorithm
