#ifndef SPELL_LIST_HPP
#define SPELL_LIST_HPP

#include <vector>
#include "spell.hpp"

namespace const_game_info {
const std::vector<game_model::spell> SPELL_LIST = {
    game_model::spell(
        0,
        game_model::spell::type::CELL,
        "break\n cell",
        "destroy the\nselected \ncell",
        5,
        [](game_model::cell &cell) {
            cell.decrease_cell_durability(cell.get_durability());
        },
        [](game_model::unit &unit) { return; },
        [](const game_model::cell &cell, int player_id) {
            return cell.get_durability() > 0;
        },
        [](const game_model::unit &unit) { return true; }
    ),
    game_model::spell(
        1,
        game_model::spell::type::CELL,
        "heal\ncell",
        "restore cell's\ndurability\nto maximum",
        5,
        [](game_model::cell &cell) { cell.restore_cell_durability(); },
        [](game_model::unit &unit) { return; },
        [](const game_model::cell &cell, int player_id) {
            return cell.get_durability() < cell.get_max_durability();
        },
        [](const game_model::unit &unit) { return true; }
    ),
    game_model::spell(
        2,
        game_model::spell::type::UNIT,
        "attack\n unit ",
        "deal 10\ndamage\nto unit",
        5,
        [](game_model::cell &cell) { return; },
        [](game_model::unit &unit) { unit.decrease_health(10); },
        [](const game_model::cell &cell, int player_id) {
            return cell.get_player_index() != -1 &&
                   cell.get_player_index() != player_id;
        },
        [](const game_model::unit &unit) { return true; }
    ),
    game_model::spell(
        3,
        game_model::spell::type::UNIT,
        "heal\n unit",
        "restore unit's\n health \nto maximum",
        5,
        [](game_model::cell &cell) { return; },
        [](game_model::unit &unit) { unit.restore_health(); },
        [](const game_model::cell &cell, int player_id) {
            return cell.get_player_index() == player_id;
        },
        [](const game_model::unit &unit) {
            return unit.get_health() < unit.get_max_health();
        }
    )};
}

#endif  // SPELL_LIST_HPP
