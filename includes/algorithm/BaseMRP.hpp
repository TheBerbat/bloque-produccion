#pragma once

#include <cstdint>
#include <vector>

namespace MRP_Algorithm
{

class BaseMRP
{
public:
    explicit BaseMRP(std::size_t planning_horizon, std::size_t security_stock, double emision_cost, double hold_cost);

    void set_need(std::size_t period, std::size_t count);
    void insert_need(std::size_t period, std::size_t count);

    void set_receptions(std::size_t period, std::size_t count);
    void insert_receptions(std::size_t period, std::size_t count);

    void set_initial_availability(std::size_t availability);

    void set_costs(double emision_cost_, double hold_cost_);
    void set_ship_period(std::size_t period);
    std::size_t get_ship_period() const;

    virtual void calculate() = 0;

protected:
    std::size_t calculate_net_needs(std::size_t idx) const;
    std::size_t calculate_availability(std::size_t idx) const;

public:
    std::vector<std::size_t> get_needs();
    std::vector<std::size_t> get_availability();
    std::vector<std::size_t> get_receptions();
    std::vector<std::size_t> get_net_needs();
    std::vector<std::size_t> get_ppl();
    std::vector<std::size_t> get_lppl();

    std::size_t get_all_demand() const;

    double get_hold_costs(bool include_security_stock = false);
    double get_emision_costs();
    double get_total_costs();

protected:
    std::vector<std::size_t> needs_{};
    std::vector<std::size_t> receptions_{};

    std::vector<std::size_t> availability_{};
    std::vector<std::size_t> net_needs_{};
    std::vector<std::size_t> ppl_{};

    std::size_t planning_horizon_{0};
    std::size_t period_ship_{0};
    bool calculated_{false};

    double emision_cost_{0.0};
    double hold_cost_{0.0};
    std::size_t security_stock_{0};
};

} // namespace MRP_Algorithm
