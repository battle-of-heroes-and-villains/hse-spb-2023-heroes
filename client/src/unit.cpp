#include "unit.hpp"

namespace game_interface {
void UnitInfo::update_characteristics(const namespace_proto::Unit &unit) {
    m_amount_of_units = unit.amount_unit();
    m_max_health = unit.sum_of_health();
    m_damage = unit.damage();
    m_attack_range = unit.attack_range();
    m_movement_range = unit.movement_range();
    m_weight = unit.weight();
}

Coords Unit::get_coords() const {
    return m_coords;
}

int Unit::get_hero_id() const {
    return m_unit_info.m_hero_id;
}

int Unit::get_health() const {
    return m_unit_info.m_max_health * m_unit_info.m_amount_of_units;
}

int Unit::get_damage() const {
    return m_unit_info.m_damage;
}

bool Unit::is_moving() const {
    return m_animation.is_moving();
}

void Unit::render(sf::RenderWindow *window) {
    if (!m_animation.is_playing()) {
        window->draw(m_unit);
    }
    render_animation(window);
    if (m_animation.is_playing()) {
        update_table_position(m_animation.get_position());
    } else {
        update_table_position(sf::Vector2f(
            m_unit.getGlobalBounds().left + m_unit.getGlobalBounds().width / 2,
            m_unit.getGlobalBounds().top + m_unit.getGlobalBounds().height
        ));
    }
    window->draw(m_table);
    window->draw(m_label);
}

void Unit::render_animation(sf::RenderWindow *window) {
    m_animation.update();
    m_animation.render(window);
}

void Unit::render_statistic(sf::RenderWindow *window) {
    m_statistic.render(window);
}

void Unit::set_selection() {
    auto old_scale = m_unit.getScale();
    m_unit.setTexture(ResourceManager::load_selected_unit_texture(m_type));
    m_unit.setScale(old_scale);
}

void Unit::disable_selection() {
    auto old_scale = m_unit.getScale();
    m_unit.setTexture(ResourceManager::load_unit_texture(m_type));
    m_unit.setScale(old_scale);
}

void Unit::update_unit(
    namespace_proto::Cell cell,
    const namespace_proto::Unit &unit,
    sf::Vector2f new_position,
    sf::Vector2f size
) {
    new_position.y -= 10;
    if (unit.id_hero() != get_client_state()->m_user.user().id()) {
        new_position.x += size.x / 5;
    } else {
        new_position.x -= size.x / 5;
    }
    if (m_type != static_cast<UnitType>(unit.type_unit())) {
        m_type = static_cast<UnitType>(unit.type_unit());
        m_unit.setTexture(ResourceManager::load_unit_texture(m_type));
        m_unit.setScale(
            0.9f * size.y / m_unit.getGlobalBounds().height,
            0.9f * size.y / m_unit.getGlobalBounds().height
        );
        if (unit.id_hero() != get_client_state()->m_user.user().id()) {
            m_unit.setOrigin(m_unit.getTexture()->getSize().x, 0);
            m_unit.scale(-1, 1);
        }
    }
    m_coords = {cell.row(), cell.column()};
    m_unit_info.m_amount_of_units = unit.amount_unit();
    m_unit_info.m_unit_id = unit.id_unit();
    m_unit_info.m_hero_id = unit.id_hero();
    m_unit_info.update_characteristics(unit);

    m_unit.setPosition(new_position);
    m_unit.move(
        -m_unit.getGlobalBounds().width / 2,
        -m_unit.getGlobalBounds().height / 2
    );

    m_animation.update_animation(
        {m_unit.getGlobalBounds().width, m_unit.getGlobalBounds().height}, size,
        new_position, m_type
    );

    m_table.setSize(sf::Vector2f(size.x / 4, size.y / 4));
    m_table.setFillColor(sf::Color(71, 78, 50));
    m_table.setOrigin(m_table.getSize().x / 2.0f, m_table.getSize().y / 2.0f);

    m_label.setFont(ResourceManager::load_font(interface::Fonts::CaptionFont));
    m_label.setString(sf::String(std::to_string(m_unit_info.m_amount_of_units))
    );
    m_label.setCharacterSize(24);

    sf::FloatRect rect = m_label.getLocalBounds();
    m_label.setOrigin(
        rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f
    );

    update_table_position(sf::Vector2f(
        m_unit.getGlobalBounds().left + m_unit.getGlobalBounds().width / 2.0f,
        m_unit.getGlobalBounds().top + m_unit.getGlobalBounds().height
    ));

    m_statistic = interface::PopUpWindow(
        new_position, {230, 150}, interface::Fonts::CaptionFont, 20,
        m_unit_info.get_unit_info()
    );
}

std::string UnitInfo::get_unit_info() const {
    return "number of units: " + std::to_string(m_amount_of_units) +
           "\n"
           "max health: " +
           std::to_string(m_max_health) +
           "\n"
           "damage: " +
           std::to_string(m_damage) +
           "\n"
           "attack range: " +
           std::to_string(m_attack_range) +
           "\n"
           "movement range: " +
           std::to_string(m_movement_range) +
           "\n"
           "weight: " +
           std::to_string(m_weight);
}

void Unit::update_statistic(EventType event_type, const sf::Window *window) {
    m_statistic.update(m_unit_info.get_unit_info(), event_type, window);
}

void Unit::play_animation(AnimationType type, Coords destination_cell) {
    m_animation.play_animation(type, m_coords, destination_cell);
}

void Unit::update_table_position(sf::Vector2f new_position) {
    m_table.setPosition(sf::Vector2f(
        new_position.x, new_position.y - m_table.getGlobalBounds().height
    ));
    if (m_unit_info.m_hero_id != get_client_state()->m_user.user().id()) {
        m_table.move(
            -m_table.getSize().x / 2.0f - m_unit.getGlobalBounds().width / 2.0f,
            0
        );
    } else {
        m_table.move(
            m_unit.getGlobalBounds().width / 2.0f + m_table.getSize().x / 2.0f,
            0
        );
    }

    m_label.setPosition(sf::Vector2f(m_table.getPosition()));
}
}  // namespace game_interface
