#pragma once

#include <algorithm/BaseMRP.hpp>

namespace MRP_Algorithm
{

class Batch2BatchWithMin : public BaseMRP
{
public:
    explicit Batch2BatchWithMin(std::size_t size_min,
                                std::size_t planning_horizon,
                                std::size_t security_stock = 0,
                                double emision_cost = 1,
                                double hold_cost = 1);
    void calculate();
private:
    std::size_t min_size_{0};
};

} // namespace MRP_Algorithm
