#ifndef ATTACK_SELECT_INTERACTOR_HPP
#define ATTACK_SELECT_INTERACTOR_HPP

#include <memory>
#include "game.hpp"

namespace interactors {
class attack_selecter {
private:
    game_model::game &m_game;

public:
    explicit attack_selecter(game_model::game &current_game_state)
        : m_game(current_game_state) {
    }

    std::vector<std::reference_wrapper<game_model::cell>> operator()(
        const game_model::coordinates &current_cell_coordinates,
        int user_id
    );
};
}  // namespace interactors

#endif  // ATTACK_SELECT_INTERACTOR_HPP