#include "cursor.hpp"

namespace interface {
sf::Cursor &get_cursor() {
    static sf::Cursor cursor;
    return cursor;
}

bool &get_cursor_state() {
    static bool cursor_state_is_change;
    return cursor_state_is_change;
}
}  // namespace interface