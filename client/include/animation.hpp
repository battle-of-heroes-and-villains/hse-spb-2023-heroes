#ifndef BATTLE_OF_HEROES_ANIMATION_HPP
#define BATTLE_OF_HEROES_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "window.hpp"

namespace game_interface {
class Animation {
public:
    Animation() = default;
    ~Animation() = default;

    void
    update_animation(sf::Vector2f size, sf::Vector2f position, UnitType type);
    void update_position(sf::Vector2f position);

    void play_animation();
    bool is_playing() const;
    void update();
    void render(sf::RenderWindow *window);

private:
    sf::IntRect m_current_frame;
    int m_frame_width;
    int m_amount_of_frames;
    sf::Texture m_animation_sheet;
    sf::Sprite m_animation;
    sf::Clock m_clock;
    bool m_is_playing{false};
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_ANIMATION_HPP
