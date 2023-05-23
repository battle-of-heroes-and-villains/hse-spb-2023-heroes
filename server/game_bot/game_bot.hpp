#ifndef GAME_BOT_HPP
#define GAME_BOT_HPP

#include <memory>
#include "game.hpp"

namespace bot {
enum class bot_response_type { SINGLE_CELL, PAIR_OF_CELLS, SPELL, NONE };

class bot_response {
private:
    bot_response_type type{bot_response_type::NONE};
    game_model::cell from{};
    game_model::cell to{};
    int spell_id{-1};

public:
    bot_response() = default;

    bot_response(
        bot_response_type type_,
        game_model::cell from_,
        game_model::cell to_
    )
        : type(type_), from(from_), to(to_) {
    }

    bot_response(bot_response_type type_, game_model::cell from_)
        : type(type_), from(from_) {
    }

    bot_response(bot_response_type type_, game_model::cell from_, int spell_id_)
        : type(type_), from(from_), spell_id(spell_id_) {
    }

    [[nodiscard]] const bot_response_type &get_type() const {
        return type;
    }

    [[nodiscard]] const game_model::cell &get_from_cell() const {
        return from;
    }

    [[nodiscard]] const game_model::cell &get_to_cell() const {
        return to;
    }

    [[nodiscard]] const int get_spell_id() const {
        return spell_id;
    }

    void set_type(bot_response_type type_){
        type = type_;
    }

    void set_from_cell(game_model::cell cell){
        from = cell;
    }

    void set_to_cell(game_model::cell cell){
        to = cell;
    }
};

class game_bot {
private:
    game_model::game &m_game;

public:
    explicit game_bot(game_model::game &current_game_state)
        : m_game(current_game_state) {
    }

    bot_response position_evaluation() const;
    std::vector<std::reference_wrapper<bot_response>> operator()();
};
}  // namespace bot

#endif  // GAME_BOT_HPP
