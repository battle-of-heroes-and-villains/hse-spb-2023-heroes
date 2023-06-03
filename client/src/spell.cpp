#include "spell.hpp"
#include "game.hpp"

namespace game_interface {
Spell::Spell(
    sf::Vector2f position,
    sf::Vector2f size,
    const namespace_proto::Spell &spell
) {
    m_id = spell.id();
    m_name = spell.name();
    m_description = spell.description();

    m_button = interface::Button(position, size);

    m_table.setSize(size);
    m_table.setFillColor(sf::Color(71, 78, 50));
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
        m_data.setCharacterSize(22);
    }
    m_is_name_showed = !m_is_name_showed;
    sf::FloatRect data_bounds = m_data.getLocalBounds();
    m_data.setOrigin(
        data_bounds.left + data_bounds.width / 2.0f,
        data_bounds.top + data_bounds.height / 2.0f
    );
}

void Spell::set_name() {
    if (!m_is_name_showed) {
        update_data();
    }
}
}  // namespace game_interface