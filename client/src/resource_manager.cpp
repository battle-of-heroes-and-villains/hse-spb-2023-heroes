#include "resource_manager.hpp"
#include <SFML/Graphics.hpp>

namespace game_interface {

std::unordered_map<TextureType, sf::Texture> ResourceManager::m_textures{};
std::unordered_map<interface::Fonts, sf::Font> ResourceManager::m_fonts{};
std::unordered_map<CursorType, sf::Image> ResourceManager::m_cursors{};

std::unordered_map<CellType, sf::Texture> ResourceManager::m_cell_textures{};
std::unordered_map<CellType, sf::Texture>
    ResourceManager::m_cell_property_textures{};
std::unordered_map<UnitType, sf::Texture> ResourceManager::m_unit_textures{};
std::unordered_map<UnitType, sf::Texture>
    ResourceManager::m_selected_unit_textures{};

std::unordered_map<UnitType, sf::Texture>
    ResourceManager::m_attack_animation{};
std::unordered_map<UnitType, sf::Texture>
    ResourceManager::m_hurt_animation{};
std::unordered_map<UnitType, int>
    ResourceManager::m_amount_of_sprites_in_attack_animation{};
std::unordered_map<UnitType, int>
    ResourceManager::m_amount_of_sprites_in_hurt_animation{};

void ResourceManager::load_resources() {
    static bool is_initialised = false;
    if (is_initialised) {
        return;
    }

    ResourceManager::m_cell_textures[CellType::Type1].loadFromFile(
        interface::source_dir + "cells/grass.jpg"
    );
    ResourceManager::m_cell_textures[CellType::Broken].loadFromFile(
        interface::source_dir + "cells/grass_broken.jpg"
    );

    ResourceManager::m_cell_property_textures[CellType::Type1].loadFromFile(
        interface::source_dir + "cells/default.png"
    );
    ResourceManager::m_cell_property_textures[CellType::Move].loadFromFile(
        interface::source_dir + "cells/move.png"
    );
    ResourceManager::m_cell_property_textures[CellType::Enemy].loadFromFile(
        interface::source_dir + "cells/enemy.png"
    );
    ResourceManager::m_cell_property_textures[CellType::Attack].loadFromFile(
        interface::source_dir + "cells/attack.png"
    );
    ResourceManager::m_cell_property_textures[CellType::Spell].loadFromFile(
        interface::source_dir + "cells/spell.png"
    );
    ResourceManager::m_cell_property_textures[CellType::AttackSpell]
        .loadFromFile(interface::source_dir + "attack.png");

    ResourceManager::m_unit_textures[UnitType::Type1].loadFromFile(
        interface::source_dir + "units/unit_type1.png"
    );
    ResourceManager::m_unit_textures[UnitType::Type2].loadFromFile(
        interface::source_dir + "units/unit_type2.png"
    );
    ResourceManager::m_unit_textures[UnitType::Type3].loadFromFile(
        interface::source_dir + "units/unit_type3.png"
    );
    ResourceManager::m_unit_textures[UnitType::Type4].loadFromFile(
        interface::source_dir + "units/unit_type4.png"
    );
    ResourceManager::m_unit_textures[UnitType::Type5].loadFromFile(
        interface::source_dir + "units/unit_type5.png"
    );

    ResourceManager::m_selected_unit_textures[UnitType::Type1].loadFromFile(
        interface::source_dir + "units_move/unit_type1_move.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type2].loadFromFile(
        interface::source_dir + "units_move/unit_type2_move.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type3].loadFromFile(
        interface::source_dir + "units_move/unit_type3_move.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type4].loadFromFile(
        interface::source_dir + "units_move/unit_type4_move.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_move/unit_type5_move.png"
    );

    ResourceManager::m_attack_animation[UnitType::Type1].loadFromFile(
        interface::source_dir + "units_attack/unit_attack_type1.png"
    );
    ResourceManager::m_attack_animation[UnitType::Type2].loadFromFile(
        interface::source_dir + "units_attack/unit_attack_type2.png"
    );
    ResourceManager::m_attack_animation[UnitType::Type3].loadFromFile(
        interface::source_dir + "units_attack/unit_attack_type3.png"
    );
    ResourceManager::m_attack_animation[UnitType::Type4].loadFromFile(
        interface::source_dir + "units_attack/unit_attack_type2.png"
    );
    ResourceManager::m_attack_animation[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_attack/unit_attack_type3.png"
    );

    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type1] = 4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type2] = 4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type3] = 4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type4] = 4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type5] = 4;
    
    ResourceManager::m_hurt_animation[UnitType::Type1].loadFromFile(
        interface::source_dir + "units_hurt/unit_hurt_type1.png"
    );
    ResourceManager::m_hurt_animation[UnitType::Type2].loadFromFile(
        interface::source_dir + "units_hurt/unit_hurt_type2.png"
    );
    ResourceManager::m_hurt_animation[UnitType::Type3].loadFromFile(
        interface::source_dir + "units_hurt/unit_hurt_type3.png"
    );
    ResourceManager::m_hurt_animation[UnitType::Type4].loadFromFile(
        interface::source_dir + "units_hurt/unit_hurt_type4.png"
    );
    ResourceManager::m_hurt_animation[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_hurt/unit_hurt_type5.png"
    );
    
    ResourceManager::m_amount_of_sprites_in_hurt_animation[UnitType::Type1] = 2;
    ResourceManager::m_amount_of_sprites_in_hurt_animation[UnitType::Type2] = 3;
    ResourceManager::m_amount_of_sprites_in_hurt_animation[UnitType::Type3] = 2;
    ResourceManager::m_amount_of_sprites_in_hurt_animation[UnitType::Type4] = 2;
    ResourceManager::m_amount_of_sprites_in_hurt_animation[UnitType::Type5];

    ResourceManager::m_textures[TextureType::MenuBackground].loadFromFile(
        interface::source_dir + "backgrounds/menu_background.jpg"
    );

    ResourceManager::m_fonts[interface::Fonts::CaptionFont].loadFromFile(
        interface::source_dir + "fonts/caption_font.otf"
    );
    ResourceManager::m_fonts[interface::Fonts::TittleFont].loadFromFile(
        interface::source_dir + "fonts/tittle_font.otf"
    );

    ResourceManager::m_cursors[CursorType::Attack].loadFromFile(
        interface::source_dir + "cursors/sword.png"
    );

    is_initialised = true;
}

const sf::Texture &ResourceManager::load_cell_texture(
    CellType texture
) {
    load_resources();
    return m_cell_textures[texture];
}

const sf::Texture &ResourceManager::load_cell_property_texture(
    CellType texture
) {
    load_resources();
    return m_cell_property_textures[texture];
}

const sf::Texture &ResourceManager::load_unit_texture(
    UnitType texture
) {
    load_resources();
    return m_unit_textures[texture];
}

const sf::Texture &ResourceManager::load_selected_unit_texture(
    UnitType texture
) {
    load_resources();
    return m_selected_unit_textures[texture];
}

const sf::Font &ResourceManager::load_font(interface::Fonts font
) {
    load_resources();
    return m_fonts[font];
}

const sf::Texture &ResourceManager::load_texture(
    TextureType texture
) {
    load_resources();
    return m_textures[texture];
}

const sf::Image &ResourceManager::load_cursor(CursorType cursor) {
    load_resources();
    return m_cursors[cursor];
}

const sf::Texture &ResourceManager::load_attack_animation_sprite_sheet(UnitType texture
) {
    load_resources();
    return m_attack_animation[texture];
}

const sf::Texture &ResourceManager::load_hurt_animation_sprite_sheet(UnitType texture
) {
    load_resources();
    return m_hurt_animation[texture];
}

int ResourceManager::amount_of_frames_in_attack_animation(UnitType type) {
    load_resources();
    return m_amount_of_sprites_in_attack_animation[type];
}

int ResourceManager::amount_of_frames_in_hurt_animation(UnitType type) {
    load_resources();
    return m_amount_of_sprites_in_hurt_animation[type];
}
}  // namespace game_interface