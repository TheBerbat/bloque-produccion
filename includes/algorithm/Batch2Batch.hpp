#pragma once

#include <algorithm/Batch2BatchWithMin.hpp>

namespace MRP_Algorithm
{

class Batch2Batch : public Batch2BatchWithMin
{
public:
    explicit Batch2Batch(std::size_t planning_horizon,
                         std::size_t security_stock = 0,
                         double emision_cost = 1,
                         double hold_cost = 1);
};

} // namespace MRP_Algorithm
