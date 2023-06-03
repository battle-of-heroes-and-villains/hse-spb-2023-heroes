#include "menu.hpp"
#include "caption.hpp"
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

    m_current_page = current_page;
    m_next_page = next_page;

    m_data.setFont(game_interface::ResourceManager::load_font(font));
    m_data.setFillColor(font_color);
    m_table.setOutlineColor(sf::Color(164, 177, 123));
    m_data.setString(sf::String(tittle));
    m_data.setCharacterSize(character_size);

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
    m_table.setFillColor(sf::Color(71, 78, 50));
    if (result == game_interface::EventType::FirstPress) {
        return true;
    } else if (result == game_interface::EventType::Targeting) {
        m_table.setOutlineThickness(5);
        m_table.setFillColor(sf::Color(78, 101, 61));
    }
    return false;
}

void MenuButton::render(sf::RenderWindow *window) {
    window->draw(m_table);
    window->draw(m_data);
}

Menu::Menu()
    : m_window("Menu: Battle of Heroes and Villains", sf::Vector2u(1920, 1080)),
      m_soundtrack(game_interface::ResourceManager::load_sound(
          interface::Sounds::MenuBackgroundSound
      )) {
    m_current_page = PageType::Entry;

    m_buttons.resize(11);
    m_captions.resize(7);

    sf::Vector2f window_size =
        static_cast<sf::Vector2f>(m_window.get_render_window()->getSize());
    sf::Vector2f button_size = sf::Vector2f(200.0f, 60.0f);
    sf::Color button_color = sf::Color(53, 76, 43);

    m_background.setTexture(game_interface::ResourceManager::load_texture(
        game_interface::TextureType::MenuBackground
    ));
    m_background.setPosition(0, 0);

    // entry page
    m_captions[0] = Caption(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 - 5 * button_size.y),
        {0, 0}, interface::Fonts::TittleFont, 70,
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
        PageType::Entry, PageType::GameOver
    );

    // sign up page
    m_captions[1] = Caption(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 - 3.25 * button_size.y
        ),
        {0, 0}, interface::Fonts::TittleFont, 48, "Sign up", PageType::SignUp
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
        {0, 0}, interface::Fonts::TittleFont, 48, "Registration",
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
        {0, 0}, interface::Fonts::TittleFont, 48, "GAME OVER",
        PageType::GameOver
    );
    m_captions[6] = Caption(
        sf::Vector2f(window_size.x / 2, window_size.y / 2 - 2 * button_size.y),
        {0, 0}, interface::Fonts::CaptionFont, 24, "you win\nyour enemy lose",
        PageType::GameOver
    );
    m_buttons[9] = MenuButton(
        sf::Vector2f(
            window_size.x / 2, window_size.y / 2 + 1.5 * button_size.y
        ),
        button_size, button_color, interface::Fonts::CaptionFont, 24, "menu",
        PageType::GameOver, PageType::GameChoose
    );
}

game_interface::Window *Menu::get_window() {
    return &m_window;
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

    if (m_soundtrack.getStatus() == sf::SoundSource::Paused ||
        m_soundtrack.getStatus() == sf::SoundSource::Stopped) {
        m_soundtrack.play();
    }

    m_window.get_render_window()->draw(m_background);

    print_error();
    for (const auto &m_caption : m_captions) {
        if (m_current_page == m_caption.get_current_page()) {
            m_caption.render(m_window.get_render_window());
        }
    }
    for (auto &m_button : m_buttons) {
        if (m_current_page == m_button.get_current_page()) {
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
        for (auto &m_button : m_buttons) {
            if (m_current_page == m_button.get_current_page()) {
                if (m_button.update(event, &m_window)) {
                    if (m_button.get_next_page() == PageType::Exit) {
                        m_window.set_is_done();
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
    }
}

void Menu::music_stop() {
    m_soundtrack.pause();
}

void Menu::music_play() {
    m_soundtrack.pause();
}

bool Menu::is_exit() const {
    return m_current_page == PageType::Exit;
}

[[nodiscard]] Menu *get_menu_state() {
    static Menu menu{};
    return &menu;
}
}  // namespace menu_interface