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

    m_button = interface::Button(position, size);

    m_button_color = color;

    m_table.setSize(size);
    m_table.setFillColor(color);
    m_table.setOutlineColor(pressed_color);
    m_table.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_table.setPosition(position);

    m_icon.setSize(size);
    m_icon.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_icon.setPosition(position);
    m_icon.setTexture(
        &ResourceManager::load_spell_icon(static_cast<SpellId>(m_id))
    );

    m_is_name_showed = false;

    m_name.setFont(ResourceManager::load_font(interface::Fonts::CaptionFont));
    m_name.setCharacterSize(20);
    m_name.setFillColor(pressed_color);
    m_name.setString(spell.name());
    m_name.setPosition(position.x, position.y - size.y / 2 - 20);
    m_name.setOrigin(
        m_name.getLocalBounds().left + m_name.getLocalBounds().width / 2.0f,
        m_name.getLocalBounds().top + m_name.getLocalBounds().height
    );

    m_description.setFont(
        ResourceManager::load_font(interface::Fonts::CaptionFont)
    );
    m_description.setCharacterSize(16);
    m_description.setString(spell.description());
    m_description.setPosition(position.x, position.y + size.y / 2 + 16);
    m_description.setOrigin(
        m_description.getLocalBounds().left +
            m_description.getLocalBounds().width / 2.0f,
        m_description.getLocalBounds().top
    );
}

void Spell::update(sf::Event event, Window *window) {
    EventType event_type =
        m_button.handling_event(event, window->get_render_window());
    m_table.setOutlineThickness(0);
    m_table.setFillColor(m_button_color);
    if (event_type == EventType::FirstPress ||
        event_type == EventType::SecondPress) {
        if (m_is_name_showed) {
            get_game_state()->get_game_menu_bar()->apply_spell();
            m_is_name_showed = false;
            get_game_state()->get_board()->remove_enable_for_spelling_cells();
        } else {
            get_game_state()->get_game_menu_bar()->apply_spell();
            m_is_name_showed = true;
            std::vector<std::pair<int, int>> enable_cells =
                Client::select_spell(m_id);
            get_game_state()->get_board()->add_enable_for_spelling_cells(
                enable_cells, m_id
            );
        }
    } else if (event_type == EventType::Targeting) {
        m_table.setFillColor(m_table.getOutlineColor());
        m_table.setOutlineThickness(5);
    }
}

void Spell::render(sf::RenderWindow *window) {
    window->draw(m_table);
    window->draw(m_icon);
    if (m_is_name_showed) {
        window->draw(m_name);
        window->draw(m_description);
    }
}

void Spell::set_default_state() {
    m_is_name_showed = false;
}
}  // namespace game_interface