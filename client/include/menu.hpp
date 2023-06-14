#ifndef BATTLE_OF_HEROES_HEROES_MENU_HPP
#define BATTLE_OF_HEROES_HEROES_MENU_HPP

#include <vector>
#include "caption.hpp"
#include "menu_button.hpp"
#include "textbox.hpp"

namespace menu_interface {

class MenuButton {
public:
    MenuButton() = default;
    ~MenuButton() = default;

    MenuButton(
        sf::Vector2f position,
        sf::Vector2f size,
        sf::Color color,
        interface::Fonts font,
        unsigned character_size,
        const std::string &tittle,
        PageType current_page,
        PageType next_page,
        sf::Color font_color
    );

    [[nodiscard]] PageType get_current_page() const;
    [[nodiscard]] PageType get_next_page() const;

    void update_tittle(const std::string &new_tittle);
    bool update(sf::Event event, game_interface::Window *window);
    void render(sf::RenderWindow *window);

private:
    void set_label_size(unsigned int character_size);

    unsigned int m_character_size;
    sf::RectangleShape m_table;
    sf::Color m_button_color;
    sf::Text m_data;
    interface::Button m_button;
    PageType m_current_page;
    PageType m_next_page;
};

class Menu {
public:
    Menu();

    void music_stop();
    void music_play();

    game_interface::Window *get_window();
    void change_page(PageType new_page);
    void print_error();
    void render();
    void update();

    [[nodiscard]] bool is_exit() const;
    void reopen();

private:
    game_interface::Window m_window;
    std::vector<MenuButton> m_buttons;
    std::vector<Caption> m_captions;
    sf::Sprite m_background;
    PageType m_current_page;
    TextBox m_signup_login;
    TextBox m_signup_password;
    MenuButton m_show_signup_password;
    Caption m_signup_error;
    TextBox m_registration_login;
    TextBox m_registration_password;
    TextBox m_registration_password_checker;
    MenuButton m_show_registration_password;
    Caption m_registration_error;
    sf::Sound m_soundtrack;
    MenuButton m_sound_button;
    sf::RectangleShape m_sound_icon;
};

[[nodiscard]] Menu *get_menu_state();

}  // namespace menu_interface

#endif  // BATTLE_OF_HEROES_HEROES_MENU_HPP
