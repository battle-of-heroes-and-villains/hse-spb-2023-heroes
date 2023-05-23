#ifndef BATTLE_OF_HEROES_UNIT_HPP
#define BATTLE_OF_HEROES_UNIT_HPP

#include <SFML/Graphics.hpp>
#include "animation.hpp"
#include "client.hpp"
#include "coordinates.hpp"
#include "popup_window.hpp"

namespace game_interface {
class Unit {
public:
    Unit()
        : m_type(UnitType::Empty),
          m_coords({-1, -1}),
          m_amount_of_units(),
          m_unit_id(),
          m_hero_id(),
          is_selected() {
    }

    ~Unit() = default;

    [[nodiscard]] Coords get_coords() const;
    [[nodiscard]] int get_hero_id() const;
    [[nodiscard]] int get_health() const;
    [[nodiscard]] int get_damage() const;
    bool is_moving();
    void set_selection();
    void disable_selection();
    void update_characteristics(const namespace_proto::Unit &unit);
    void update_unit(
        namespace_proto::Cell cell,
        const namespace_proto::Unit &unit,
        sf::Vector2f new_position,
        sf::Vector2f size
    );
    [[nodiscard]] std::string get_unit_info() const;
    void update_statistic(EventType event_type, const sf::Window *window);
    void play_animation(AnimationType type, Coords destination_cell = {-1, -1});
    void render(sf::RenderWindow *window);
    void render_animation(sf::RenderWindow *window);
    void render_statistic(sf::RenderWindow *window);

private:
    void update_table_position(sf::Vector2f new_position);

    UnitType m_type;
    Coords m_coords;
    int m_amount_of_units;
    int m_unit_id;
    int m_hero_id;
    bool is_selected{false};
    int m_number_of_units;
    int m_max_health;
    int m_damage;
    int m_attack_range;
    int m_movement_range;
    int m_weight;

    sf::Sprite m_unit;
    sf::RectangleShape m_table;
    sf::Text m_label;
    interface::PopUpWindow m_statistic;
    Animation m_animation;
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_UNIT_HPP
