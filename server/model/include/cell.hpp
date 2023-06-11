#ifndef MODEL_CELL_HPP
#define MODEL_CELL_HPP

#include <random>
#include "coordinates.hpp"

namespace game_model {

class cell {
    const int MAX_CELL_DURABILITY = 20;
    const int MIN_CELL_DURABILITY = 10;

private:
    int m_durability;
    int m_max_durability;
    int m_player_index = -1;
    int m_unit_index = -1;
    coordinates m_coordinates{0, 0};

public:
    cell() {
        int temp_durability =
            rand() % (MAX_CELL_DURABILITY - MIN_CELL_DURABILITY) +
            MIN_CELL_DURABILITY;
        m_durability = temp_durability;
        m_max_durability = temp_durability;
    };

    cell& operator=(cell other){
        m_durability = other.m_durability;
        m_max_durability = other.m_max_durability;
        m_player_index = other.m_player_index;
        m_coordinates = other.m_coordinates;
        m_unit_index = other.m_unit_index;
        return *this;
    }

    [[nodiscard]] int get_player_index() const;
    [[nodiscard]] int get_unit_index() const;
    [[nodiscard]] coordinates get_coordinates() const;
    void set_player_index(int player_index);
    void set_unit_index(int unit_index);
    void set_coordinates(const coordinates &coordinates_new);
    void decrease_cell_durability(int damage);
    void restore_cell_durability();
    [[nodiscard]] bool is_unit_movable(int player_id) const;
    [[nodiscard]] int get_durability() const;
    [[nodiscard]] int get_max_durability() const;
};

}  // namespace game_model

#endif  // MODEL_CELL_HPP
