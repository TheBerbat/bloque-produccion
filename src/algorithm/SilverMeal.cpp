#include <algorithm/SilverMeal.hpp>
#include <cmath>
//#include <cstdio>
#include <limits>

namespace MRP_Algorithm
{

SilverMeal::SilverMeal(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost)
  : BaseMRP(planning_horizon, security_stock, emision_cost, hold_cost)
{}


void SilverMeal::calculate()
{
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

    std::size_t desviation = std::numeric_limits<std::size_t>::max();
    while(head<planning_horizon_ && it<planning_horizon_)
    {
        cp += (it-head)*ppl_.at(it)*hold_cost_;
        Q += ppl_.at(it);

        const double cp_tot {static_cast<double>(cp)};
        const double ce_tot {emision_cost_};
        const double desv_tot {(cp_tot + ce_tot)/static_cast<double>(it-head+1)};
        
        //std::printf("%lu->%lu - Q*%lu - %.2f - %.2f = %.2f\n", head, it, Q, cp_tot, ce_tot, desv_tot);
        
        if ( desv_tot>desviation ) {
            ppl_.at(head) = Q - ppl_.at(it);
            for (std::size_t p{head+1} ; p<it ; ++p)
                ppl_.at(p) = 0;
            
            // RESET CP and Q*
            cp = 0; Q = 0;
            
            head = it;
            desviation = std::numeric_limits<std::size_t>::max();
        } else {
            ++it;
            desviation = desv_tot;
        }
    }
    ppl_.at(head) = Q;
    for (std::size_t p{head+1} ; p<it ; ++p)
        ppl_.at(p) = 0;

    // RECALCULATE NET_NEEDS AND AVAILABILITY WHITH PRECALCULATED PLL
    for (std::size_t i{0} ; i<planning_horizon_ ; ++i)
    {
        net_needs_.at(i) = calculate_net_needs(i);
        availability_.at(i+1) = calculate_availability(i+1);
    }
}

} // namespace MRP_Algorithm
