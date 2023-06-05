#ifndef STATE_MACHINE_HPP_
#define STATE_MACHINE_HPP_

#include "game.hpp"
#include "menu.hpp"

class State {
public:
    virtual void display_state() = 0;
    virtual State *switcher() = 0;
};

class GameState;

class MenuState : public State {
    GameState *game{};

public:
    void display_state() override {
        while (!menu_interface::get_menu_state()->get_window()->is_done()) {
            menu_interface::get_menu_state()->update();
            menu_interface::get_menu_state()->render();
        }
    }

    void set_game_state(GameState *game_);

    State *switcher() override;
};

class GameState : public State {
    MenuState *menu{};

public:
    void display_state() override {
        Client::get_hero();
        get_client_state()->m_opponent.set_type(-1);
        std::thread receiver(&Client::run_receiver);
        while (!game_interface::get_game_state()->get_window()->is_done()) {
            {
                std::unique_lock lock{get_client_state()->m_mutex};
                game_interface::get_game_state()->update();
            }
            game_interface::get_game_state()->render();
        }
        receiver.join();
    }

    void set_menu_state(MenuState *menu_) {
        menu = menu_;
    }

    State *switcher() override {
        return menu;
    }
};

State *MenuState::switcher() {
    return game;
}

void MenuState::set_game_state(GameState *game_) {
    game = game_;
}

class Context {
    State *window_state;

public:
    explicit Context(State *state) : window_state(state) {
    }

    void switch_state() {
        window_state = window_state->switcher();
    }

    void display() {
        window_state->display_state();
    }
};

#endif  // STATE_MACHINE_HPP_
