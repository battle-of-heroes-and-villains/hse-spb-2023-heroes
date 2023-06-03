#ifndef BATTLE_OF_HEROES_UNIT_HPP
#define BATTLE_OF_HEROES_UNIT_HPP

#include <SFML/Graphics.hpp>
#include "animation.hpp"
#include "client.hpp"
#include "coordinates.hpp"
#include "popup_window.hpp"

namespace game_interface {
class Unit;

class UnitInfo {
public:
    UnitInfo() = default;
    ~UnitInfo() = default;

    void update_characteristics(const namespace_proto::Unit &unit);
    [[nodiscard]] std::string get_unit_info() const;

    friend Unit;

private:
    int m_unit_id;
    int m_hero_id;
    int m_amount_of_units;
    int m_max_health;
    int m_weight;
    int m_damage;
    int m_attack_range;
    int m_movement_range;
};

class Unit {
public:
    Unit() : m_type(UnitType::Empty), m_coords({-1, -1}) {
    }

    ~Unit() = default;

    [[nodiscard]] Coords get_coords() const;
    [[nodiscard]] int get_hero_id() const;
    [[nodiscard]] int get_health() const;
    [[nodiscard]] int get_damage() const;
    bool is_moving() const;
    void set_selection();
    void disable_selection();
    void update_unit(
        namespace_proto::Cell cell,
        const namespace_proto::Unit &unit,
        sf::Vector2f new_position,
        sf::Vector2f size
    );
    void update_statistic(EventType event_type, const sf::Window *window);
    void play_animation(AnimationType type, Coords destination_cell = {-1, -1});
    void render(sf::RenderWindow *window);
    void render_animation(sf::RenderWindow *window);
    void render_statistic(sf::RenderWindow *window);

private:
    void update_table_position(sf::Vector2f new_position);

    UnitType m_type;
    Coords m_coords;
    UnitInfo m_unit_info;

    sf::Sprite m_unit;
    sf::RectangleShape m_table;
    sf::Text m_label;
    interface::PopUpWindow m_statistic;
    Animation m_animation{};
};
}  // namespace game_interface

#endif  // BATTLE_OF_HEROES_UNIT_HPP
