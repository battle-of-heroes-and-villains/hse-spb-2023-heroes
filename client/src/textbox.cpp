#include "textbox.hpp"
#include "resource_manager.hpp"

namespace menu_interface {
TextBox::TextBox(
    sf::Vector2f position,
    sf::Vector2f size,
    interface::Fonts font,
    unsigned int character_size,
    bool is_active
)
    : m_type_sound(
          game_interface::ResourceManager::load_sound(interface::Sounds::Type)
      ) {
    m_table.setSize(size);
    m_table.setFillColor(sf::Color::White);
    m_table.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_table.setPosition(position);

    m_button = interface::Button(position, size);

    m_label.setFont(game_interface::ResourceManager::load_font(font));
    m_label.setFillColor(sf::Color::Black);
    m_label.setCharacterSize(character_size);

    sf::FloatRect data_bounds = m_label.getLocalBounds();
    m_label.setOrigin(
        data_bounds.left + data_bounds.width / 2.0f,
        data_bounds.top + data_bounds.height / 2.0f
    );

    m_label.setPosition(
        position.x - size.x / 2 + character_size / 2,
        position.y - size.y / 2 + character_size / 2
    );
    m_is_active = is_active;

    m_type_sound.setPitch(3);
    m_type_sound.setVolume(40);

    m_cursor.setSize({3.0f, character_size * 1.2f});
    m_cursor.setFillColor(sf::Color::Black);
    m_cursor.setOrigin(
        m_cursor.getSize().x / 2.0f, m_cursor.getSize().y / 2.0f
    );
    m_cursor.setPosition(
        data_bounds.left + data_bounds.width + 7.0f,
        m_label.getGlobalBounds().top + 1.2f * character_size / 2.0f
    );
    m_is_cursor_showed = true;
}

bool TextBox::is_active() const {
    return m_is_active;
}

bool TextBox::is_showed() const {
    return m_is_showed;
}

std::string TextBox::get_data() const {
    return m_data;
}

void TextBox::activate() {
    m_is_active = false;
}

void TextBox::hide_data() {
    m_is_showed = false;
    m_label.setString(m_hidden_data);
}

void TextBox::show_data() {
    m_is_showed = true;
    m_label.setString(m_data);
}

void TextBox::clear() {
    m_data = "";
    m_hidden_data = "";
    m_label.setString(m_data);
}

bool TextBox::update(sf::Event event, game_interface::Window *window) {
    bool result = false;
    if (m_button.handling_event(event, window->get_render_window()) ==
        game_interface::EventType::FirstPress) {
        m_is_active = true;
        result = true;
    } else if (m_is_active && event.type == sf::Event::TextEntered && event.text.unicode != 10 &&  event.text.unicode != 8) {
        m_data += event.text.unicode;
        m_hidden_data += '*';
    } else if (m_is_active && event.type == sf::Event::TextEntered && event.text.unicode == 8) {
        if (!m_data.isEmpty()) {
            m_data.erase(m_data.getSize() - 1, 1);
            m_hidden_data.erase(m_hidden_data.getSize() - 1, 1);
        }
    }
    if (m_is_showed) {
        m_label.setString(m_data);
    } else {
        m_label.setString(m_hidden_data);
    }
    m_cursor.setPosition(
        m_label.getGlobalBounds().left + m_label.getGlobalBounds().width + 3.0f,
        m_cursor.getPosition().y
    );
    if (event.type == sf::Event::TextEntered) {
        m_type_sound.play();
    }
    return result;
}

void TextBox::render(sf::RenderWindow *window) {
    window->draw(m_table);
    window->draw(m_label);
    if (m_timer.getElapsedTime().asSeconds() > 0.5f) {
        m_is_cursor_showed = !m_is_cursor_showed;
        m_timer.restart();
    }
    if (m_is_active && m_is_cursor_showed) {
        window->draw(m_cursor);
    }
}
}  // namespace menu_interface