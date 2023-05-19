#ifndef STATE_MACHINE_HPP_
#define STATE_MACHINE_HPP_

class State {
public:
    virtual void display_state() = 0;
    virtual State *switcher() = 0;
};

class GameState;

class MenuState : public State {
    GameState *game;

public:

    void display_state() override {
    }

    State *switcher() override;
};

class GameState : public State {
    MenuState *menu;

public:
    void display_state() override {
    }

    State *switcher() override {
        return menu;
    }
};

State *MenuState::switcher() {
    return game;
}

class Context {
    State *window_state;

public:
    void switch_state() {
        window_state = window_state->switcher();
    }

    void display() {
        window_state->display_state();
    }
};


/*
while (true){
    context::display();
    context::switch();
}
 */

#endif  // STATE_MACHINE_HPP_