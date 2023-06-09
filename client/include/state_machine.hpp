#ifndef STATE_MACHINE_HPP_
#define STATE_MACHINE_HPP_

#include "game.hpp"
#include "menu.hpp"

class State {
public:
    virtual void display_state() = 0;
    virtual State *switcher() = 0;
    virtual bool is_exit() = 0;
};

class GameState;

class MenuState : public State {
    GameState *game{};

public:
    void display_state() override {
        if (menu_interface::get_menu_state()->get_page() ==
            menu_interface::PageType::GameOver) {
            menu_interface::get_menu_state()->reopen();
        }
        while (!menu_interface::get_menu_state()->get_window()->is_done()) {
            menu_interface::get_menu_state()->update();
            menu_interface::get_menu_state()->render();
        }
        menu_interface::get_menu_state()->music_stop();
        menu_interface::get_menu_state()->change_page(
            menu_interface::PageType::GameOver
        );
    }

    void set_game_state(GameState *game_);

    bool is_exit() override {
        return menu_interface::get_menu_state()->is_exit();
    }

    State *switcher() override;
};

class GameState : public State {
    MenuState *menu{};

public:
    void display_state() override {
        Client::get_hero();
        if (game_interface::get_game_state()->get_window()->is_done()) {
            game_interface::get_game_state()->reopen();
        }
        get_client_state()->m_opponent.set_type(-1);
        get_client_state()->active_game = true;
        std::thread receiver(&Client::run_receiver);
        while (!game_interface::get_game_state()->get_window()->is_done()) {
            {
                std::unique_lock lock{get_client_state()->m_mutex};
                game_interface::get_game_state()->update();
            }
            game_interface::get_game_state()->render();
            if (!(get_client_state()->active_game)) {
                break;
            }
        }
        game_interface::get_game_state()->music_stop();
        receiver.join();
        menu_interface::get_menu_state()->set_up_winner();
        game_interface::get_game_state()->get_window()->set_is_done();
    }

    void set_menu_state(MenuState *menu_) {
        menu = menu_;
    }

    bool is_exit() override {
        return false;
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
    bool game_is_over{false};

public:
    explicit Context(State *state) : window_state(state) {
    }

    void switch_state() {
        window_state = window_state->switcher();
    }

    void display() {
        window_state->display_state();
    }

    [[nodiscard]] bool is_exit() const {
        return window_state->is_exit();
    }
};

#endif  // STATE_MACHINE_HPP_
