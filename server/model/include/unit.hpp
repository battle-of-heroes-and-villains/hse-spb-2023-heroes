#ifndef MODEL_UNIT_HPP
#define MODEL_UNIT_HPP

#include <utility>
#include "coordinates.hpp"

namespace game_model {

class unit {
private:
    int m_type{1};
    int m_number{5};
    int m_health{10};
    int m_max_health{10};
    int m_damage{1};
    int m_attack_range{1};
    int m_movement_range{2};
    int m_weight{1};
    int m_unit_base_value{31};
    int m_number_base_value{5};
    int m_position_base_value{1};
    int m_position_cur_value{0};
    int m_health_base_value{2};

public:
    explicit unit(int type);

    [[nodiscard]] int get_type() const;
    [[nodiscard]] int get_number() const;
    [[nodiscard]] int get_health() const;
    [[nodiscard]] int get_max_health() const;
    [[nodiscard]] int get_attack_range() const;
    [[nodiscard]] int get_movement_range() const;
    [[nodiscard]] int get_damage() const;
    [[nodiscard]] int get_weight() const;
    [[nodiscard]] int get_value() const;
    [[nodiscard]] bool is_dead() const;
    void decrease_health(int damage);
    void restore_health();
    void set_position_value(coordinates coord, coordinates board_size);
};

}  // namespace game_model

#endif  // MODEL_UNIT_HPP
