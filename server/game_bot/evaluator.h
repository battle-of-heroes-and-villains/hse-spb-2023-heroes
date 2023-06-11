#ifndef HSE_SPB_2023_HEROES_EVALUATOR_H
#define HSE_SPB_2023_HEROES_EVALUATOR_H

#include "game_session.hpp"

namespace evaluator{
    class Evaluator{
        GameSession *game_session_ref;

    public:
        Evaluator(GameSession* game_session_ref_) : game_session_ref(game_session_ref_) {}


    };
}

#endif  // HSE_SPB_2023_HEROES_EVALUATOR_H
