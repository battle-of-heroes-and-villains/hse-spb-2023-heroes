#include "game_bot.hpp"

namespace bot {

bot_response game_bot::position_evaluation() const {
    int optimal_unit_id = -1;
    int score = -1;
    bot_response response;
//    for (auto unit : m_game.get_player(-2)->get_units()) {
//        // todo - get optimal move (greedy)
//    }
    response.set_type(bot_response_type::PAIR_OF_CELLS);
    game_model::cell from;

    from.set_coordinates(game_model::coordinates{0, 9});

    response.set_from_cell(from);

    game_model::cell to;

    to.set_coordinates(game_model::coordinates{0, 8});

    response.set_to_cell(to);


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
