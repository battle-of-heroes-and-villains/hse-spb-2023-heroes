#include "game_bot.hpp"
#include <random>

namespace bot {

bot_response game_bot::position_evaluation() const {
    int optimal_unit_id = -1;
    int score = -1;
    bot_response response;
    response.set_type(bot_response_type::PAIR_OF_CELLS);
    bool can_attack = false;
    bot_response attack;
    bot_response move;
    std::vector<std::reference_wrapper<game_model::cell>> attack_response;
    std::vector<std::reference_wrapper<game_model::cell>> move_response;
    std::vector<std::pair<game_model::coordinates, game_model::cell>> random_move;
    for(int row = 0; row < 10; row++){
        for (int column = 0; column < 10; column++){
            game_model::coordinates coord{row, column};
            if (column == 9){
                std::cout << "catch\n";
            }
            if (m_game.get_cell(coord).get_unit_index() != -1 && m_game.get_cell(coord).is_unit_movable(1)){
                attack_response = m_game.get_attackable_cells(coord, -2);
                move_response = m_game.get_reachable_cells(coord, -2);
            }

            if (move_response.size() > 0){
                int sz = move_response.size();
                if (sz == 1){
                    random_move.emplace_back(coord, move_response[0]);
                }
                else{
                    random_move.emplace_back(coord, move_response[rand() % sz]);
                }
            }

            if (attack_response.size() > 0){
                attack.set_type(bot_response_type::PAIR_OF_CELLS);
                game_model::cell cell_from;
                cell_from.set_coordinates(coord);
                attack.set_from_cell(cell_from);
                attack.set_to_cell(attack_response[0]);
                response = attack;
                can_attack = true;
                goto out;
            }
        }
    }
out:
    if (!can_attack) {
        int random_move_size = random_move.size();
        if (random_move_size == 0) {
            // todo give up
        } else if (random_move_size == 1) {
            response.set_to_cell(random_move[0].second);
            game_model::cell cell_from;
            cell_from.set_coordinates(random_move[0].first);
            response.set_from_cell(cell_from);
        } else {
            int pos = rand() % random_move_size;
            response.set_to_cell(random_move[pos].second);
            game_model::cell cell_from;
            cell_from.set_coordinates(random_move[pos].first);
            response.set_from_cell(cell_from);
        }
    }

    return response;
}

std::vector<std::reference_wrapper<bot_response>> game_bot::operator()() {
    std::vector<std::reference_wrapper<bot_response>> result;

    bot_response move = position_evaluation();
    if (move.get_type() == bot_response_type::PAIR_OF_CELLS) {
        game_model::cell cell_from =
            m_game.get_cell(move.get_from_cell().get_coordinates());
        game_model::cell cell_to =
            m_game.get_cell(move.get_to_cell().get_coordinates());
        if (cell_to.get_unit_index() != -1) {
            m_game.attack(
                cell_from.get_coordinates(), cell_to.get_coordinates()
            );
            bot_response response{bot_response_type::SINGLE_CELL, cell_to};
            result.emplace_back(response);
        } else {
            m_game.move(cell_from.get_coordinates(), cell_to.get_coordinates());
            bot_response response{
                bot_response_type::PAIR_OF_CELLS, cell_from, cell_to};
            result.emplace_back(response);
        }
    } else {
        game_model::cell cell =
            m_game.get_cell(move.get_from_cell().get_coordinates());
        m_game.spell(cell.get_coordinates(), -2, move.get_spell_id());
        bot_response response{
            bot_response_type::SINGLE_CELL, cell, move.get_spell_id()};
        result.emplace_back(response);
    }
    return result;
}

}  // namespace bot
