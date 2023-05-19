#include "animation.hpp"

namespace game_interface {
void Animation::update_animation(
    sf::Vector2f size,
    sf::Vector2f position,
    AnimationType type
) {
    m_animation_sheet = resource_manager()->load_animation_sprite_sheet(type);
    m_amount_of_frames =
        resource_manager()->amount_of_frames_in_animation(type);

    m_frame_width = m_animation_sheet.getSize().x / m_amount_of_frames;
    m_current_frame =
        sf::IntRect(0, 0, m_frame_width, m_animation_sheet.getSize().y);

    m_animation.setTexture(m_animation_sheet);
    m_animation.setTextureRect(m_current_frame);
    m_animation.setScale(
        1.8f * size.y / m_animation_sheet.getSize().y,
        1.8f * size.y / m_animation_sheet.getSize().y
    );

    m_animation.setPosition(position);
    m_animation.move(-m_animation.getGlobalBounds().width / 2, -3 * m_animation.getGlobalBounds().height / 4);
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

void Animation::play_animation() {
    m_is_playing = true;
}

bool Animation::is_playing() const {
    return m_is_playing;
}
}  // namespace game_interface
