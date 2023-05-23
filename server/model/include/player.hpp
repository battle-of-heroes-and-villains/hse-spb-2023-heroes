#ifndef MODEL_PLAYER_HPP
#define MODEL_PLAYER_HPP

#include <vector>
#include "board.hpp"
#include "unit.hpp"

namespace game_model {

class player {
private:
    std::vector<unit> m_units_list{};
    int m_id;
    int m_mana{10};
    int m_max_mana{10};

public:
    explicit player(int account_id) : m_id(account_id) {
    }

    void set_start_units(int player_index, const board &game_board, const std::vector<int> &troop);
    [[nodiscard]] int get_id() const;
    [[nodiscard]] unit &get_unit(int index);
    [[nodiscard]] const std::vector<unit> & get_units() const;
    void decrease_mana(int cost);
    [[nodiscard]] int get_mana() const;
};

}  // namespace game_model

#endif  // MODEL_PLAYER_HPP
