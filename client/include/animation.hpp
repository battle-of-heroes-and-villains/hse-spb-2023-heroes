#ifndef BATTLE_OF_HEROES_ANIMATION_HPP
#define BATTLE_OF_HEROES_ANIMATION_HPP

#include <SFML/Graphics.hpp>
#include "button.hpp"
#include "coordinates.hpp"
#include "window.hpp"

namespace game_interface {
class Animation {
public:
    Animation() = default;
    ~Animation() = default;

    sf::Vector2f get_position() const;

    void update_animation(
        sf::Vector2f unit_size,
        sf::Vector2f cell_size,
        sf::Vector2f position,
        UnitType type
    );
    void update_texture();

    void play_animation(
        AnimationType type,
        Coords source_cell,
        Coords destination_cell = {-1, -1}
    );
    bool is_playing() const;
    bool is_moving();
    void update();
    void render(sf::RenderWindow *window);

private:
    sf::IntRect m_current_frame;
    int m_frame_width;
    int m_amount_of_frames;
    bool m_is_reversed;
    AnimationType m_animation_type{AnimationType::Nothing};
    sf::Texture m_animation_sheet;
    sf::Sprite m_animation;
    sf::Clock m_clock;
    bool m_is_playing{false};
    UnitType m_unit_type;
    sf::Vector2f m_unit_size;
    sf::Vector2f m_cell_size;
    sf::Vector2f m_delta_distance;
    int m_repeats;
    float m_delta_time;
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_ANIMATION_HPP
