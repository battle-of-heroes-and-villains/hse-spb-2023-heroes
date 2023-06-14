#include "menu.hpp"
#include "caption.hpp"
#include "cursor.hpp"
#include "resource_manager.hpp"

namespace menu_interface {
MenuButton::MenuButton(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color color,
    interface::Fonts font,
    unsigned int character_size,
    const std::string &tittle,
    PageType current_page,
    PageType next_page,
    sf::Color font_color = sf::Color::White
) {
    m_table.setSize(size);
    m_table.setFillColor(color);
    m_table.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_table.setPosition(position);

    m_button = interface::Button(position, size);

    m_button_color = color;

    m_current_page = current_page;
    m_next_page = next_page;

    m_data.setFont(game_interface::ResourceManager::load_font(font));
    m_data.setFillColor(font_color);
    m_table.setOutlineColor(sf::Color(78, 87, 56));
    m_data.setString(sf::String(tittle));

    m_character_size = character_size;
    m_data.setCharacterSize(m_character_size);

    sf::FloatRect rect = m_data.getLocalBounds();
    m_data.setOrigin(
        rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f
    );

    m_data.setPosition(position);
}

[[nodiscard]] PageType MenuButton::get_current_page() const {
    return m_current_page;
}

[[nodiscard]] PageType MenuButton::get_next_page() const {
    return m_next_page;
}

void MenuButton::update_tittle(const std::string &new_tittle) {
    m_data.setString(sf::String(new_tittle));
    sf::FloatRect rect = m_data.getLocalBounds();
    m_data.setOrigin(
        rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f
    );
}

bool MenuButton::update(sf::Event event, game_interface::Window *window) {
    auto result = m_button.handling_event(event, window->get_render_window());
    m_table.setOutlineThickness(0);
    m_table.setFillColor(m_button_color);
    set_label_size(m_character_size);
    if (result == game_interface::EventType::FirstPress) {
        return true;
    } else if (result == game_interface::EventType::Targeting) {
        interface::get_cursor().loadFromSystem(sf::Cursor::Hand);
        window->get_render_window()->setMouseCursor(interface::get_cursor());
        interface::get_cursor_state() = true;
        set_label_size(m_character_size + 4);
        m_table.setOutlineThickness(5);
        m_table.setFillColor(m_table.getOutlineColor());
    }
    return false;
}

void MenuButton::render(sf::RenderWindow *window) {
    window->draw(m_table);
    window->draw(m_data);
}

void MenuButton::set_label_size(unsigned int character_size) {
    m_data.setCharacterSize(character_size);
    m_data.setOrigin(
        m_data.getLocalBounds().left + m_data.getLocalBounds().width / 2.0f,
        m_data.getLocalBounds().top + m_data.getLocalBounds().height / 2.0f
    );
}

void Menu::reopen() {
    m_window.reopen(
        "Menu: Battle of Heroes and Villains", sf::Vector2u(1920, 1080)
    );
}

Menu::Menu()
    : m_window("Menu: Battle of Heroes and Villains", sf::Vector2u(1920, 1080)),
      m_soundtrack(game_interface::ResourceManager::load_sound(
          interface::Sounds::MenuBackgroundSound
      )) {
    m_current_page = PageType::Entry;

    m_buttons.resize(12);
    m_captions.resize(7);

    sf::Vector2f window_size =
        static_cast<sf::Vector2f>(m_window.get_render_window()->getSize());
    sf::Vector2f button_size = sf::Vector2f(210.0f, 60.0f);
    sf::Color button_color = sf::Color(89, 100, 64);

    m_background.setTexture(game_interface::ResourceManager::load_texture(
        game_interface::TextureType::MenuBackground
    ));
    m_background.setPosition(0, 0);

    // entry page
    m_captions[0] = Caption(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 - 5 * button_size.y),
        {0, 0}, interface::Fonts::TittleFont, 90,
        "Battle of Heroes and Villains", PageType::Entry
    );

    m_buttons[0] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 1.5 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "sign up",
        PageType::Entry, PageType::SignUp
    );
    m_buttons[1] = MenuButton(
        sf::Vector2f(window_size.x / 2, window_size.y / 2), button_size,
        button_color, interface::Fonts::CaptionFont, 24, "registration",
        PageType::Entry, PageType::Registration
    );
    m_buttons[2] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 1.5 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "exit",
        PageType::Entry, PageType::Exit
    );

    // sign up page
    m_captions[1] = Caption(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 3.25 * button_size.y
        ),
        {0, 0}, interface::Fonts::TittleFont, 60, "Sign up", PageType::SignUp
    );

    m_signup_login = TextBox(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 1.75 * button_size.y
        ),
        sf::Vector2f(800, 60), interface::Fonts::CaptionFont, 24, true
    );
    m_signup_password = TextBox(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 0.25 * button_size.y
        ),
        sf::Vector2f(800, 60), interface::Fonts::CaptionFont, 24, false
    );
    m_captions[2] = Caption(
        sf::Vector2f(
            window_size.x / 2 - 25, window_size.y / 2 + 1.05 * button_size.y
        ),
        {0, 225}, interface::Fonts::CaptionFont, 24, "Show password",
        PageType::SignUp
    );
    m_show_signup_password = MenuButton(
        sf::Vector2f(
            window_size.x / 2 + 112.5,
            window_size.y / 2 + 1.05 * button_size.y - 2
        ),
        sf::Vector2f(25, 25), sf::Color::White, interface::Fonts::CaptionFont,
        30, "", PageType::SignUp, PageType::SignUp, sf::Color(129, 143, 92)
    );
    m_signup_password.hide_data();
    m_signup_error = Caption(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 2.05 * button_size.y
        ),
        {0, 0}, interface::Fonts::CaptionFont, 24, "", PageType::SignUp
    );

    m_buttons[3] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 3.25 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "submit",
        PageType::SignUp, PageType::GameChoose
    );
    m_buttons[4] = MenuButton(
        sf::Vector2f(50, 50), sf::Vector2f(60, 60), button_color,
        interface::Fonts::CaptionFont, 20, "back", PageType::SignUp,
        PageType::Entry
    );

    // registration page
    m_captions[3] = Caption(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 - 4 * button_size.y),
        {0, 0}, interface::Fonts::TittleFont, 60, "Registration",
        PageType::Registration
    );

    m_registration_login = TextBox(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 2.5 * button_size.y
        ),
        sf::Vector2f(800, 60), interface::Fonts::CaptionFont, 24, true
    );
    m_registration_password = TextBox(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 - button_size.y),
        sf::Vector2f(800, 60), interface::Fonts::CaptionFont, 24, false
    );
    m_registration_password.hide_data();
    m_registration_password_checker = TextBox(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 0.5 * button_size.y
        ),
        sf::Vector2f(800, 60), interface::Fonts::CaptionFont, 24, false
    );
    m_registration_password_checker.hide_data();
    m_registration_error = Caption(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 2.8 * button_size.y
        ),
        {0, 0}, interface::Fonts::CaptionFont, 24, "", PageType::Registration
    );
    m_show_registration_password = MenuButton(
        sf::Vector2f(
            window_size.x / 2 + 112.5,
            window_size.y / 2 + 1.8 * button_size.y - 2
        ),
        sf::Vector2f(25, 25), sf::Color::White, interface::Fonts::CaptionFont,
        30, "", PageType::Registration, PageType::Registration,
        sf::Color(129, 143, 92)
    );

    m_captions[4] = Caption(
        sf::Vector2f(
            window_size.x / 2 - 25, window_size.y / 2 + 1.8 * button_size.y
        ),
        {225, 0}, interface::Fonts::CaptionFont, 24, "Show password",
        PageType::Registration
    );
    m_buttons[5] = MenuButton(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 + 4 * button_size.y),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "submit",
        PageType::Registration, PageType::GameChoose
    );
    m_buttons[6] = MenuButton(
        sf::Vector2f(50, 50), sf::Vector2f(60, 60), button_color,
        interface::Fonts::CaptionFont, 20, "back", PageType::Registration,
        PageType::Entry
    );

    // game choose page
    m_buttons[7] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 1.5 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24,
        "single player", PageType::GameChoose, PageType::SinglePlayer
    );
    m_buttons[8] = MenuButton(
        sf::Vector2f(window_size.x / 2, window_size.y / 2), button_size,
        button_color, interface::Fonts::CaptionFont, 24, "multi player",
        PageType::GameChoose, PageType::MultiPlayer
    );
    m_buttons[9] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 1.5 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "exit",
        PageType::GameChoose, PageType::Exit
    );

    // game over page
    m_captions[5] = Caption(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 - 4 * button_size.y),
        {0, 0}, interface::Fonts::TittleFont, 60, "Game over",
        PageType::GameOver
    );
    if (Client::are_we_win()) {
        m_captions[6] = Caption(
            sf::Vector2f(
                window_size.x / 2, window_size.y / 2 - 2 * button_size.y
            ),
            {0, 0}, interface::Fonts::CaptionFont, 24,
            "you win\nyour enemy lose", PageType::GameOver
        );
    } else {
        m_captions[6] = Caption(
            sf::Vector2f(
                window_size.x / 2, window_size.y / 2 - 2 * button_size.y
            ),
            {0, 0}, interface::Fonts::CaptionFont, 24,
            "you lose\nyour enemy win", PageType::GameOver
        );
    }
    m_buttons[10] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "menu",
        PageType::GameOver, PageType::GameChoose
    );
    m_buttons[11] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 2 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "exit",
        PageType::GameOver, PageType::Exit
    );

    // set sound
    m_sound_button = MenuButton(
        sf::Vector2f(window_size.x - 50, window_size.y - 50),
        sf::Vector2f(60, 60), button_color, interface::Fonts::CaptionFont, 20,
        "", PageType::Any, PageType::Any
    );
    m_sound_icon.setSize({60.0f, 60.0f});
    m_sound_icon.setOrigin(30.0f, 30.0f);
    m_sound_icon.setPosition({window_size.x - 50, window_size.y - 50});
    m_sound_icon.setTexture(&game_interface::ResourceManager::load_sound_icon(0)
    );
}

game_interface::Window *Menu::get_window() {
    return &m_window;
}

menu_interface::PageType menu_interface::Menu::get_page() const {
    return this->m_current_page;
}

void Menu::change_page(PageType next_page) {
    m_current_page = next_page;
    if (m_current_page == PageType::SinglePlayer) {
        get_client_state()->m_user.set_is_single(true);
        m_window.set_is_done();
    } else if (m_current_page == PageType::MultiPlayer) {
        m_window.set_is_done();
    }
}

void Menu::print_error() {
    if (m_current_page == PageType::SignUp) {
        m_signup_error.render(m_window.get_render_window());
    } else if (m_current_page == PageType::Registration) {
        m_registration_error.render(m_window.get_render_window());
    } else {
        m_signup_error.update_text("");
        m_registration_error.update_text("");
        m_signup_login.clear();
        m_signup_password.clear();
        m_registration_login.clear();
        m_registration_password.clear();
        m_registration_password_checker.clear();
    }
}

void Menu::render() {
    m_window.begin_draw();

    m_window.get_render_window()->draw(m_background);

    m_sound_button.render(m_window.get_render_window());
    m_window.get_render_window()->draw(m_sound_icon);

    print_error();
    for (const auto &m_caption : m_captions) {
        if (m_current_page == m_caption.get_current_page()) {
            m_caption.render(m_window.get_render_window());
        }
    }
    for (auto &m_button : m_buttons) {
        if (m_button.get_current_page() == m_current_page ||
            m_button.get_current_page() == PageType::Any) {
            m_button.render(m_window.get_render_window());
        }
    }
    if (m_current_page == PageType::SignUp) {
        m_signup_login.render(m_window.get_render_window());
        m_signup_password.render(m_window.get_render_window());
        m_show_signup_password.render(m_window.get_render_window());
    }
    if (m_current_page == PageType::Registration) {
        m_registration_login.render(m_window.get_render_window());
        m_registration_password.render(m_window.get_render_window());
        m_registration_password_checker.render(m_window.get_render_window());
        m_show_registration_password.render(m_window.get_render_window());
    }
    m_window.end_draw();
}

void Menu::update() {
    sf::Event event{};
    while (m_window.get_render_window()->pollEvent(event)) {
        interface::get_cursor_state() = false;
        if (m_current_page == PageType::SignUp) {
            bool is_login_chosen = m_signup_login.update(event, &m_window);
            bool is_password_chosen =
                m_signup_password.update(event, &m_window);
            if (m_signup_login.is_active() && m_signup_password.is_active()) {
                if (is_login_chosen) {
                    m_signup_password.activate();
                } else if (is_password_chosen) {
                    m_signup_login.activate();
                }
            }
        } else if (m_current_page == PageType::Registration) {
            bool is_login_chosen =
                m_registration_login.update(event, &m_window);
            bool is_password_chosen =
                m_registration_password.update(event, &m_window);
            bool is_password_checker_chosen =
                m_registration_password_checker.update(event, &m_window);
            if (is_login_chosen) {
                m_registration_password.activate();
                m_registration_password_checker.activate();
            } else if (is_password_chosen) {
                m_registration_login.activate();
                m_registration_password_checker.activate();
            } else if (is_password_checker_chosen) {
                m_registration_login.activate();
                m_registration_password.activate();
            }
        }
        if (m_show_signup_password.update(event, &m_window)) {
            if (m_signup_password.is_showed()) {
                m_signup_password.hide_data();
                m_show_signup_password.update_tittle("");
            } else {
                m_signup_password.show_data();
                m_show_signup_password.update_tittle("+");
            }
        }
        if (m_show_registration_password.update(event, &m_window)) {
            if (m_registration_password.is_showed()) {
                m_registration_password.hide_data();
                m_registration_password_checker.hide_data();
                m_show_registration_password.update_tittle("");
            } else {
                m_registration_password.show_data();
                m_registration_password_checker.show_data();
                m_show_registration_password.update_tittle("+");
            }
        }
        if (m_sound_button.update(event, &m_window)) {
            if (m_soundtrack.getStatus() == sf::SoundSource::Playing) {
                music_stop();
            } else {
                music_play();
            }
        }
        for (auto &m_button : m_buttons) {
            if (m_current_page == m_button.get_current_page() ||
                m_button.get_current_page() == PageType::Any) {
                if (m_button.update(event, &m_window)) {
                    if (m_button.get_next_page() == PageType::Exit) {
                        m_current_page = PageType::Exit;
                        m_window.set_is_done();
                        return;
                    }
                    if (m_current_page == PageType::SignUp &&
                        m_button.get_next_page() == PageType::GameChoose) {
                        Client::log_in(
                            m_signup_login.get_data(),
                            m_signup_password.get_data()
                        );
                        if (get_client_state()->m_user.user().id() == -1) {
                            m_signup_login.clear();
                            m_signup_password.clear();
                            m_signup_error.update_text("Wrong data from user");
                            break;
                        } else {
                            get_client_state()->m_hero.set_name(
                                m_signup_login.get_data()
                            );
                        }
                    } else if (m_current_page == PageType::Registration && m_button.get_next_page() == PageType::GameChoose) {
                        if (m_registration_password.get_data() !=
                            m_registration_password_checker.get_data()) {
                            m_registration_error.update_text(
                                "Passwords are different"
                            );
                            break;
                        }
                        Client::sign_up(
                            m_registration_login.get_data(),
                            m_registration_password.get_data()
                        );
                        if (get_client_state()->m_user.user().id() == -1) {
                            m_registration_login.clear();
                            m_registration_password.clear();
                            m_registration_password_checker.clear();
                            m_registration_error.update_text(
                                "Wrong data from user"
                            );
                            break;
                        } else {
                            get_client_state()->m_hero.set_name(
                                m_registration_login.get_data()
                            );
                        }
                    }
                    change_page(m_button.get_next_page());
                    break;
                }
            }
        }
        if (!interface::get_cursor_state()) {
            interface::get_cursor().loadFromSystem(sf::Cursor::Arrow);
            m_window.get_render_window()->setMouseCursor(interface::get_cursor()
            );
        }
    }
}

void Menu::music_stop() {
    m_sound_icon.setTexture(&game_interface::ResourceManager::load_sound_icon(0)
    );
    m_soundtrack.pause();
}

void Menu::music_play() {
    m_sound_icon.setTexture(&game_interface::ResourceManager::load_sound_icon(1)
    );
    m_soundtrack.play();
}

bool Menu::is_exit() const {
    return m_current_page == PageType::Exit;
}

[[nodiscard]] Menu *get_menu_state() {
    static Menu menu{};
    return &menu;
}
}  // namespace menu_interface
