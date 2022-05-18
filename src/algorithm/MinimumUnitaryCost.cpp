#include <algorithm/MinimumUnitaryCost.hpp>
#include <cmath>
//#include <cstdio>
#include <limits>

namespace MRP_Algorithm
{

MinimumUnitaryCost::MinimumUnitaryCost(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
{}


void MinimumUnitaryCost::calculate()
{
    auto count_units = [](const std::vector<std::size_t>& ppl,
                                    std::size_t start,
                                    std::size_t end){
        std::size_t num {0};
        for(std::size_t i{start} ; i<=end ; ++i)
            num += ppl.at(i);
        return num;
    };

    // MAKE BATCH TO BATCH
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        ppl_.at(i) = net_needs_.at(i);
        availability_.at(i+1) = calculate_availability(i+1);
    }

    // FIND FIRST LAUNCH
    std::size_t first_launch {0};
    while(first_launch<planning_horizon_ && ppl_.at(first_launch)==0)
        first_launch++;

    // ITERATE OVER PPL AND GROUP THE MIN UNITARY COST
    std::size_t it {first_launch}, head {first_launch};
    std::size_t Q {0}, cp {0};

    double ct_ud_last = std::numeric_limits<double>::max();
    while(head<planning_horizon_ && it<planning_horizon_)
    {
        cp += (it-head)*ppl_.at(it)*hold_cost_;
        Q += ppl_.at(it);

        const double cp_ud {static_cast<double>(cp) / static_cast<double>(Q)};
        const double ce_ud = emision_cost_/static_cast<double>(Q);
        const double ct_ud = cp_ud + ce_ud;
        
        //std::printf("\n%lu->%lu - Q*%lu - %.2f - %.2f = %.2f", head, it, Q, cp_ud, ce_ud, ct_ud);
        
        if ( ct_ud>ct_ud_last ) {
            ppl_.at(head) = Q - ppl_.at(it);
            for (std::size_t p{head+1} ; p<it ; ++p)
                ppl_.at(p) = 0;
            
            // RESET CP and Q*
            cp = 0; Q = 0;
            
            head = it;
            ct_ud_last = std::numeric_limits<double>::max();
            //std::printf("===");
        } else {
            ++it;
            ct_ud_last = ct_ud;
        }
    }
    ppl_.at(head) = Q;
    for (std::size_t p{head+1} ; p<it ; ++p)
        ppl_.at(p) = 0;
    //std::printf("===\n");

    // RECALCULATE NET_NEEDS AND AVAILABILITY WHITH PRECALCULATED PLL
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        availability_.at(i+1) = calculate_availability(i+1);
    }
}

} // namespace MRP_Algorithm
