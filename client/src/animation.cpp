#include "animation.hpp"

namespace game_interface {
void Animation::update_animation(
    sf::Vector2f size,
    sf::Vector2f position,
    UnitType type
) {
//    m_animation_sheet =
//        ResourceManager::load_attack_animation_sprite_sheet(type);
//    m_amount_of_frames =
//        ResourceManager::amount_of_frames_in_attack_animation(type);

    m_frame_width = m_animation_sheet.getSize().x / m_amount_of_frames;
    m_current_frame =
        sf::IntRect(0, 0, m_frame_width, m_animation_sheet.getSize().y);

    m_animation.setTexture(m_animation_sheet);
    m_animation.setTextureRect(m_current_frame);
    m_animation.setScale(
        0.9f * size.y / m_animation_sheet.getSize().y,
        0.9f * size.y / m_animation_sheet.getSize().y
    );

    m_animation.setPosition(position);
    m_animation.move(
        -m_animation.getGlobalBounds().width / 2,
        -3 * m_animation.getGlobalBounds().height / 4
    );

    m_unit_type = type;
    m_size = size;
}

void Animation::update_position(sf::Vector2f position) {
    m_animation.setPosition(position);
}

void Animation::update() {
    if (!m_is_playing) {
        return;
    }
    if (m_clock.getElapsedTime().asSeconds() > 0.5f) {
        if (m_current_frame.left == m_frame_width * (m_amount_of_frames - 1)) {
            m_current_frame.left = 0;
            m_is_playing = false;
        } else {
            m_current_frame.left += m_frame_width;
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
        0.9f * m_size.y / m_animation_sheet.getSize().y,
        0.9f * m_size.y / m_animation_sheet.getSize().y
    );
}

void Animation::play_animation(AnimationType type, Coords destination_cell) {
    if (type == AnimationType::Attack) {
        m_is_playing = true;
        m_animation_sheet =
            ResourceManager::load_attack_animation_sprite_sheet(m_unit_type);
        m_amount_of_frames =
            ResourceManager::amount_of_frames_in_attack_animation(m_unit_type);
    } else if (type == AnimationType::GetAttacked) {
        m_is_playing = true;
        m_animation_sheet =
            ResourceManager::load_hurt_animation_sprite_sheet(m_unit_type);
        m_amount_of_frames =
            ResourceManager::amount_of_frames_in_hurt_animation(m_unit_type);
        // TODO: if there is zero of health ?
    }
    update_texture();
}

bool Animation::is_playing() const {
    return m_is_playing;
}
}  // namespace game_interface
