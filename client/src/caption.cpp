#include "caption.hpp"

namespace menu_interface {
Caption::Caption(
    sf::Vector2f position,
    sf::Vector2f size,
    interface::Fonts font,
    unsigned int character_size,
    const std::string &text,
    PageType current_page
) {
    m_table.setSize(size);
    m_table.setFillColor(sf::Color::Black);
    m_table.setOrigin(size.x / 2.0f, size.y / 2.0f);
    m_table.setPosition(position);

    m_current_page = current_page;

    m_data.setFont(game_interface::ResourceManager::load_font(font));
    m_data.setString(sf::String(text));
    m_data.setFillColor(sf::Color::White);
    m_data.setCharacterSize(character_size);

    sf::FloatRect data_bounds = m_data.getLocalBounds();
    m_data.setOrigin(
        data_bounds.left + data_bounds.width / 2.0f,
        data_bounds.top + data_bounds.height / 2.0f
    );
    m_data.setPosition(position);
}

[[nodiscard]] PageType Caption::get_current_page() const {
    return m_current_page;
}

void Caption::update_text(const std::string &text) {
    m_data.setString(text);
    sf::FloatRect data_bounds = m_data.getLocalBounds();
    m_data.setOrigin(
        data_bounds.left + data_bounds.width / 2.0f,
        data_bounds.top + data_bounds.height / 2.0f
    );
    m_data.setPosition(m_table.getPosition());
}

void Caption::render(sf::RenderWindow *window) const {
    window->draw(m_table);
    window->draw(m_data);
}
}  // namespace menu_interface