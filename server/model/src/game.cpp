#include "game.hpp"
#include <vector>
#include "cell.hpp"
#include "spell_list.hpp"

namespace game_model {

std::vector<std::vector<int>> game::troops = {
    {1, 2, 3, 4, 5, 1, 2, 3, 4, 5}
};

cell &game::get_cell(const coordinates &cell_coordinates) {
    return m_board.get_cell(cell_coordinates);
}

int game::get_troop(){
    int items = static_cast<int>(troops.size());
    return 0;
}

std::vector<std::reference_wrapper<cell>>
game::get_reachable_cells(const coordinates &cell_coordinates, int user_id) {
    int player_id = (m_players_list[0]->get_id() == user_id ? 0 : 1);
    if (!get_cell(cell_coordinates).is_unit_movable(player_id))
        return {};
    int max_distance =
        m_players_list[player_id]
            ->get_unit(m_board.get_cell(cell_coordinates).get_unit_index())
            .get_movement_range();
    return m_board.get_reachable_cells(cell_coordinates, max_distance);
}

std::vector<std::reference_wrapper<cell>>
game::get_attackable_cells(const coordinates &cell_coordinates, int user_id) {
    int player_id = (m_players_list[0]->get_id() == user_id ? 0 : 1);
    if (!get_cell(cell_coordinates).is_unit_movable(player_id))
        return {};
    int max_distance =
        m_players_list[player_id]
            ->get_unit(m_board.get_cell(cell_coordinates).get_unit_index())
            .get_attack_range();
    return m_board.get_attackable_cells(cell_coordinates, max_distance);
}

std::vector<std::reference_wrapper<cell>>
game::get_spellable_cells(int user_id, int spell_id) {
    int player_id = (m_players_list[0]->get_id() == user_id ? 0 : 1);
    const class spell &current_spell = const_game_info::SPELL_LIST[spell_id];
    if (get_player(player_id)->get_mana() < current_spell.get_mana_cost())
        return {};
    std::vector<std::reference_wrapper<cell>> board_result =
        m_board.get_spellable_cells(player_id, spell_id);
    if (current_spell.get_type() == spell::type::CELL)
        return board_result;
    std::vector<std::reference_wrapper<cell>> result;
    for (auto &element : board_result)
        if (current_spell.is_selectable(
                get_player(element.get().get_player_index())
                    ->get_unit(element.get().get_unit_index())
            ))
            result.emplace_back(element);
    return result;
}

[[nodiscard]] player *game::get_player(int index) {
    return m_players_list[index].get();
}

void game::move(
    const coordinates &current_cell_coordinates,
    const coordinates &new_cell_coordinates
) {
    cell &current_cell = get_cell(current_cell_coordinates);
    cell &new_cell = get_cell(new_cell_coordinates);
    int player_index = current_cell.get_player_index();
    int unit_index = current_cell.get_unit_index();
    clear_cell(current_cell);
    new_cell.set_player_index(player_index);
    new_cell.set_unit_index(unit_index);
    new_cell.decrease_cell_durability(
        get_player(player_index)->get_unit(unit_index).get_weight()
    );
}

void game::attack(const coordinates &attacking, const coordinates &attacked) {
    cell &attacking_cell = get_cell(attacking);
    cell &attacked_cell = get_cell(attacked);
    attack_cell(attacking_cell, attacked_cell);
}

void game::attack_cell(const cell &attacking, cell &attacked) {
    unit &attacking_unit =
        m_players_list[attacking.get_player_index()]->get_unit(
            attacking.get_unit_index()
        );
    unit &attacked_unit = m_players_list[attacked.get_player_index()]->get_unit(
        attacked.get_unit_index()
    );
    attacked_unit.decrease_health(attacking_unit.get_damage());
    if (attacked_unit.is_dead())
        clear_cell(attacked);
}

void game::clear_cell(cell &current_cell) {
    current_cell.set_player_index(-1);
    current_cell.set_unit_index(-1);
}

void game::spell(
    const coordinates &cell_coordinates,
    int user_id,
    int spell_id
) {
    int player_id = (m_players_list[0]->get_id() == user_id ? 0 : 1);
    const class spell &current_spell = const_game_info::SPELL_LIST[spell_id];
    cell &current_cell = get_cell(cell_coordinates);
    current_spell(current_cell);
    if (current_spell.get_type() == spell::type::UNIT)
        current_spell(get_player(current_cell.get_player_index())
                          ->get_unit(current_cell.get_unit_index()));
    get_player(player_id)->decrease_mana(current_spell.get_mana_cost());
}
}  // namespace game_model
