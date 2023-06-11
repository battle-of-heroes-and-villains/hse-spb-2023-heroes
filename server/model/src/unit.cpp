#include "unit.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>
#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"

namespace game_model {

void unit::set_position_value(coordinates coord, coordinates board_size) {
    m_position_cur_value = m_position_base_value *
                           (board_size.get_column() - coord.get_column() - 1);
}

unit::unit(int type) : m_type(type) {
    using namespace boost::property_tree;

    ptree tree;

    read_json("../../server/model/const_game_info/units.json", tree);

    ptree units = tree.get_child("units");
    auto it = units.begin();
    while (type-- > 1) {
        it++;
    }

    m_number = it->second.get<int>("m_number");
    m_max_health = it->second.get<int>("m_max_health");
    m_damage = it->second.get<int>("m_damage");
    m_attack_range = it->second.get<int>("m_attack_range");
    m_movement_range = it->second.get<int>("m_movement_range");
    m_weight = it->second.get<int>("m_weight");
    m_unit_base_value = it->second.get<int>("m_unit_base_value");
    m_number_base_value = it->second.get<int>("m_number_base_value");
    m_position_base_value = it->second.get<int>("m_position_base_value");
    m_health_base_value = it->second.get<int>("m_health_base_value");
    m_health = m_max_health;
}

[[nodiscard]] int unit::get_type() const {
    return m_type;
}

[[nodiscard]] int unit::get_number() const {
    return m_number;
}

[[nodiscard]] int unit::get_health() const {
    return m_health;
}

int unit::get_value() const {
    if (is_dead()) {
        return 0;
    }
    return m_unit_base_value + m_health_base_value * m_health +
           (m_number_base_value + m_position_cur_value) * m_number;
}

int unit::get_movement_range() const {
    return m_movement_range;
}

void unit::decrease_health(int damage) {
    m_health = std::max(0, m_health - damage);
    if (m_health == 0) {
        if (m_number > 0)
            --m_number;
        m_health = m_max_health;
    }
}

int unit::get_damage() const {
    return m_damage * m_number;
}

int unit::get_weight() const {
    return m_weight * m_number;
}

bool unit::is_dead() const {
    return m_number == 0;
}

int unit::get_attack_range() const {
    return m_attack_range;
}

void unit::restore_health() {
    m_health = m_max_health;
}

int unit::get_max_health() const {
    return m_max_health;
}
}  // namespace game_model
