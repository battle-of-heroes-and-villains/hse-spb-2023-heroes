#ifndef BATTLE_OF_HEROES_CURSOR_HPP
#define BATTLE_OF_HEROES_CURSOR_HPP

#include <SFML/Graphics.hpp>

namespace interface {
[[nodiscard]] sf::Cursor &get_cursor();

[[nodiscard]] bool &get_cursor_state();
}  // namespace interface

#endif  // BATTLE_OF_HEROES_CURSOR_HPP
