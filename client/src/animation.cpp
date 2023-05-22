#include "animation.hpp"

namespace game_interface {
void Animation::update_animation(
    sf::Vector2f unit_size,
    sf::Vector2f cell_size,
    sf::Vector2f position,
    UnitType type
) {
    m_frame_width = m_animation_sheet.getSize().x / m_amount_of_frames;
    m_current_frame =
        sf::IntRect(0, 0, m_frame_width, m_animation_sheet.getSize().y);

    m_animation.setTexture(m_animation_sheet);
    m_animation.setTextureRect(m_current_frame);
    m_animation.setScale(
        (m_is_reversed ? -1 : 1) * unit_size.y / m_animation_sheet.getSize().y,
        unit_size.y / m_animation_sheet.getSize().y
    );

    if (!m_is_playing) {
        m_animation.setPosition(position);
    }
    m_animation.setOrigin(
        m_animation.getTextureRect().width / 2,
        m_animation.getTextureRect().height / 2
    );

    m_unit_type = type;
    m_unit_size = unit_size;
    m_cell_size = cell_size;
}

void Animation::update() {
    if (!m_is_playing) {
        return;
    }
    if (m_clock.getElapsedTime().asSeconds() > m_delta_time) {
        if (m_current_frame.left == m_frame_width * (m_amount_of_frames - 1)) {
            m_current_frame.left = 0;
        } else {
            m_current_frame.left += m_frame_width;
        }
        m_animation.setPosition(
            m_animation.getPosition().x + m_delta.x,
            m_animation.getPosition().y + m_delta.y
        );
        m_repeats--;
        if (!m_repeats) {
            m_is_playing = false;
        }
        m_animation.setTextureRect(m_current_frame);
        m_clock.restart();
    }
}

void Animation::render(sf::RenderWindow *window) {
    if (m_is_playing) {
        window->draw(m_animation);
    }
}

void Animation::update_texture() {
    m_frame_width = m_animation_sheet.getSize().x / m_amount_of_frames;
    m_current_frame =
        sf::IntRect(0, 0, m_frame_width, m_animation_sheet.getSize().y);

    m_animation.setTexture(m_animation_sheet);
    m_animation.setTextureRect(m_current_frame);
    m_animation.setScale(
        (m_is_reversed ? -1 : 1) * m_unit_size.y /
            m_animation_sheet.getSize().y,
        m_unit_size.y / m_animation_sheet.getSize().y
    );
    m_animation.setOrigin(
        m_animation.getTextureRect().width / 2,
        m_animation.getTextureRect().height / 2
    );
}

void Animation::reverse() {
    // m_animation.scale(-1, 1);
}

void Animation::play_animation(
    AnimationType type,
    Coords source_cell,
    Coords destination_cell
) {
    m_is_playing = true;
    if (type == AnimationType::Attack) {
        m_animation_sheet =
            ResourceManager::load_attack_animation_sprite_sheet(m_unit_type);
        m_amount_of_frames =
            ResourceManager::amount_of_frames_in_attack_animation(m_unit_type);
        m_repeats = 6;
        m_delta = {0.0f, 0.0f};
    } else if (type == AnimationType::GetAttacked) {
        m_animation_sheet =
            ResourceManager::load_hurt_animation_sprite_sheet(m_unit_type);
        m_amount_of_frames =
            ResourceManager::amount_of_frames_in_hurt_animation(m_unit_type);
        m_repeats = 6;
        m_delta = {0.0f, 0.0f};
    } else if (type == AnimationType::Move) {
        m_animation_sheet =
            ResourceManager::load_move_animation_sprite_sheet(m_unit_type);
        m_amount_of_frames =
            ResourceManager::amount_of_frames_in_move_animation(m_unit_type);
        m_repeats = 12;
        sf::Vector2f shift = {
            m_cell_size.x *
                (destination_cell.get_column() - source_cell.get_column()),
            m_cell_size.y *
                (destination_cell.get_row() - source_cell.get_row())};
        m_delta = {shift.x / m_repeats, shift.y / m_repeats};
    } else if (type == AnimationType::Dead) {
        m_animation_sheet =
            ResourceManager::load_dead_animation_sprite_sheet(m_unit_type);
        m_amount_of_frames =
            ResourceManager::amount_of_frames_in_dead_animation(m_unit_type);
        m_repeats = m_amount_of_frames;
        m_delta = {0.0f, 0.0f};
    }
    m_delta_time = 1.5f / m_repeats;
    m_is_reversed = destination_cell.get_column() < source_cell.get_column() ||
                    destination_cell.get_column() == source_cell.get_column() &&
                        destination_cell.get_row() > source_cell.get_row();
    if (m_is_reversed) {
        reverse();
    }
    update_texture();
}

bool Animation::is_playing() const {
    return m_is_playing;
}
}  // namespace game_interface
