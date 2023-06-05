#include "spell.hpp"
#include "game.hpp"

namespace game_interface {
Spell::Spell(
    sf::Vector2f position,
    sf::Vector2f size,
    sf::Color color,
    sf::Color pressed_color,
    const namespace_proto::Spell &spell
) {
    m_id = spell.id();
    m_name = spell.name();
    m_description = spell.description();

    m_button = interface::Button(position, size);

    m_button_color = color;

    m_table.setSize(size);
    m_table.setFillColor(color);
    m_table.setOutlineColor(pressed_color);
    m_table.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_table.setPosition(position);

    m_data.setFont(ResourceManager::load_font(interface::Fonts::CaptionFont));
    m_is_name_showed = false;
    update_data();
    m_data.setPosition(position);
}

void Spell::update(sf::Event event, Window *window) {
    EventType event_type =
        m_button.handling_event(event, window->get_render_window());
    if (m_is_name_showed) {
        set_label_size(22);
    } else {
        set_label_size(18);
    }
    m_table.setOutlineThickness(0);
    m_table.setFillColor(m_button_color);
    if (event_type == EventType::FirstPress ||
        event_type == EventType::SecondPress) {
        update_data();
        if (m_is_name_showed) {
            get_game_state()->get_board()->remove_enable_for_spelling_cells();
        } else {
            std::vector<std::pair<int, int>> enable_cells =
                Client::select_spell(m_id);
            get_game_state()->get_board()->add_enable_for_spelling_cells(
                enable_cells, m_id
            );
        }
    } else if (event_type == EventType::Targeting && m_is_name_showed) {
        m_table.setFillColor(m_table.getOutlineColor());
        m_table.setOutlineThickness(5);
        set_label_size(24);
    }
}

void Spell::remove() {
    update_data();
}

void Spell::render(sf::RenderWindow *window) {
    window->draw(m_table);
    window->draw(m_data);
}

void Spell::update_data() {
    if (m_is_name_showed) {
        m_data.setString(m_description);
        m_data.setCharacterSize(18);
    } else {
        m_data.setString(m_name);
        set_label_size(22);
    }
    m_is_name_showed = !m_is_name_showed;
}

void Spell::set_name() {
    if (!m_is_name_showed) {
        update_data();
    }
}

void Spell::set_label_size(unsigned int character_size) {
    m_data.setCharacterSize(character_size);
    m_data.setOrigin(
        m_data.getLocalBounds().left + m_data.getLocalBounds().width / 2.0f,
        m_data.getLocalBounds().top + m_data.getLocalBounds().height / 2.0f
    );
}
}  // namespace game_interface