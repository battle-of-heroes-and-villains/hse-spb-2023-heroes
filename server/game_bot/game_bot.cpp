#include "game_bot.hpp"
#include <chrono>
#include <random>
#include <thread>

namespace bot {

int game_bot::get_score(const bot_response &move) const {
    if (move.get_type() == bot_response_type::PAIR_OF_CELLS) {
        game_model::cell cell_from =
            m_game.get_cell(move.get_from_cell().get_coordinates());
        game_model::cell cell_to =
            m_game.get_cell(move.get_to_cell().get_coordinates());
        if (cell_to.get_unit_index() != -1) {
            return m_game.score_attack(
                cell_from.get_coordinates(), cell_to.get_coordinates()
            );
        }
        return m_game.score_move(
            cell_from.get_coordinates(), cell_to.get_coordinates()
        );
    }
    return 0;
}

bot_response game_bot::position_evaluation() const {
    int optimal_unit_id = -1;
    int score = 0;
    bool is_score_exist = false;
    bot_response response;
    response.set_type(bot_response_type::PAIR_OF_CELLS);
    bool can_attack = false;
    bot_response attack;
    bot_response move;
    std::vector<std::reference_wrapper<game_model::cell>> attack_response;
    std::vector<std::reference_wrapper<game_model::cell>> move_response;
    std::vector<std::pair<game_model::coordinates, game_model::cell>>
        random_move;
    std::vector<std::pair<game_model::coordinates, game_model::cell>>
        random_attack;
    for (int row = 0; row < 10; row++) {
        for (int column = 0; column < 10; column++) {
            game_model::coordinates coord{row, column};
            game_model::cell current_cell = m_game.get_cell(coord);
            if (m_game.get_cell(coord).get_unit_index() != -1 &&
                m_game.get_cell(coord).is_unit_movable(1)) {
                attack_response = m_game.get_attackable_cells(coord, -2);
                move_response = m_game.get_reachable_cells(coord, -2);
            }

            /*if (move_response.size() > 0) {
                int sz = move_response.size();
                if (sz == 1) {
                    random_move.emplace_back(coord, move_response[0]);
                } else {
                    random_move.emplace_back(coord, move_response[rand() % sz]);
                }
            }*/
            for (auto cell : move_response) {
                bot_response current_move{
                    bot_response_type::PAIR_OF_CELLS, current_cell, cell};
                int current_score = get_score(current_move);
                if (!is_score_exist || score < current_score) {
                    is_score_exist = true;
                    score = current_score;
                    random_move.clear();
                    random_attack.clear();
                    random_move.emplace_back(coord, cell);
                } else if (score == current_score) {
                    random_move.emplace_back(coord, cell);
                }
            }

            /*if (attack_response.size() > 0) {
                int sz = attack_response.size();
                if (sz == 1) {
                    random_attack.emplace_back(coord, attack_response[0]);
                } else {
                    random_attack.emplace_back(
                        coord, attack_response[rand() % sz]
                    );
                }
                can_attack = true;
            }*/
            for (auto cell : attack_response) {
                can_attack = true;
                bot_response current_move{
                    bot_response_type::PAIR_OF_CELLS, current_cell, cell};
                int current_score = get_score(current_move);
                if (!is_score_exist || score < current_score) {
                    is_score_exist = true;
                    score = current_score;
                    random_move.clear();
                    random_attack.clear();
                    random_attack.emplace_back(coord, cell);
                } else if (score == current_score) {
                    random_attack.emplace_back(coord, cell);
                }
            }
            attack_response.clear();
            move_response.clear();
        }
    } /*
     if (!can_attack) {
         int random_move_size = random_move.size();
         if (random_move_size == 0) {
             response.set_type(bot_response_type::GIVE_UP);
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
     } else {
         attack.set_type(bot_response_type::PAIR_OF_CELLS);
         game_model::cell cell_from;
         int sz = random_attack.size();
         int id = rand() % sz;
         cell_from.set_coordinates(random_attack[id].first);
         attack.set_from_cell(cell_from);
         attack.set_to_cell(random_attack[id].second);
         response = attack;
     }*/
    int random_size = random_move.size() + random_attack.size();
    if (random_size == 0) {
        response.set_type(bot_response_type::GIVE_UP);
    } else {
        int pos = rand() % random_size;
        if (pos < random_move.size()) {
            response.set_to_cell(random_move[pos].second);
            game_model::cell cell_from;
            cell_from.set_coordinates(random_move[pos].first);
            response.set_from_cell(cell_from);
        } else {
            response.set_to_cell(random_attack[pos].second);
            game_model::cell cell_from;
            cell_from.set_coordinates(random_attack[pos].first);
            response.set_from_cell(cell_from);
        }
    }
    return response;
}

bot_response game_bot::operator()() {
    bot_response result;

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
            bot_response response{
                bot_response_type::SINGLE_CELL, cell_to, cell_from};
            result = response;
        } else {
            m_game.move(cell_from.get_coordinates(), cell_to.get_coordinates());
            bot_response response{
                bot_response_type::PAIR_OF_CELLS, cell_from, cell_to};
            result = response;
        }
    } else if (move.get_type() == bot_response_type::SINGLE_CELL) {
        game_model::cell cell =
            m_game.get_cell(move.get_from_cell().get_coordinates());
        m_game.spell(cell.get_coordinates(), -2, move.get_spell_id());
        bot_response response{
            bot_response_type::SINGLE_CELL, cell, move.get_spell_id()};
        result = response;
    } else {
        result = move;
    }
    return result;
}

}  // namespace bot
