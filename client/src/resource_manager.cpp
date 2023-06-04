#include "resource_manager.hpp"
#include <SFML/Graphics.hpp>

namespace game_interface {

std::unordered_map<TextureType, sf::Texture> ResourceManager::m_textures{};
std::unordered_map<interface::Fonts, sf::Font> ResourceManager::m_fonts{};
std::unordered_map<CursorType, sf::Image> ResourceManager::m_cursors{};
std::unordered_map<interface::Sounds, sf::SoundBuffer>
    ResourceManager::m_sounds{};

std::unordered_map<Characters, sf::Texture> ResourceManager::m_icons{};
std::unordered_map<int, sf::Texture> ResourceManager::m_sound_icons{};

std::unordered_map<CellType, sf::Texture> ResourceManager::m_cell_textures{};
std::unordered_map<CellPropertyType, sf::Texture>
    ResourceManager::m_cell_property_textures{};
std::unordered_map<CracksStage, sf::Texture>
    ResourceManager::m_cracks_textures{};
std::unordered_map<UnitType, sf::Texture> ResourceManager::m_unit_textures{};
std::unordered_map<UnitType, sf::Texture>
    ResourceManager::m_selected_unit_textures{};

std::unordered_map<UnitType, sf::Texture> ResourceManager::m_attack_animation{};
std::unordered_map<UnitType, sf::Texture> ResourceManager::m_hurt_animation{};
std::unordered_map<UnitType, sf::Texture> ResourceManager::m_move_animation{};
std::unordered_map<UnitType, sf::Texture> ResourceManager::m_dead_animation{};

std::unordered_map<UnitType, int>
    ResourceManager::m_amount_of_sprites_in_attack_animation{};
std::unordered_map<UnitType, int>
    ResourceManager::m_amount_of_sprites_in_hurt_animation{};
std::unordered_map<UnitType, int>
    ResourceManager::m_amount_of_sprites_in_move_animation{};
std::unordered_map<UnitType, int>
    ResourceManager::m_amount_of_sprites_in_dead_animation{};

void ResourceManager::load_resources() {
    static bool is_initialised = false;
    if (is_initialised) {
        return;
    }

    ResourceManager::m_icons[Characters::Bear].loadFromFile(
        interface::source_dir + "icons/bear.png"
    );
    ResourceManager::m_icons[Characters::Bee].loadFromFile(
        interface::source_dir + "icons/bee.png"
    );
    ResourceManager::m_icons[Characters::Boar].loadFromFile(
        interface::source_dir + "icons/boar.png"
    );
    ResourceManager::m_icons[Characters::Cat].loadFromFile(
        interface::source_dir + "icons/cat.png"
    );
    ResourceManager::m_icons[Characters::Raven].loadFromFile(
        interface::source_dir + "icons/raven.png"
    );
    ResourceManager::m_icons[Characters::Skull].loadFromFile(
        interface::source_dir + "icons/skull.png"
    );
    ResourceManager::m_icons[Characters::Spider].loadFromFile(
        interface::source_dir + "icons/spider.png"
    );
    ResourceManager::m_icons[Characters::Stone].loadFromFile(
        interface::source_dir + "icons/stone.png"
    );
    ResourceManager::m_icons[Characters::Turtle].loadFromFile(
        interface::source_dir + "icons/turtle.png"
    );
    ResourceManager::m_icons[Characters::Wolf].loadFromFile(
        interface::source_dir + "icons/wolf.png"
    );

    ResourceManager::m_sound_icons[0].loadFromFile(
        interface::source_dir + "menu_icons/sound_off.png"
    );
    ResourceManager::m_sound_icons[1].loadFromFile(
        interface::source_dir + "menu_icons/sound_on.png"
    );

    ResourceManager::m_cell_textures[CellType::Type1].loadFromFile(
        interface::source_dir + "cells/backgrounds/cell_type1.png"
    );
    ResourceManager::m_cell_textures[CellType::Type2].loadFromFile(
        interface::source_dir + "cells/backgrounds/cell_type2.png"
    );
    ResourceManager::m_cell_textures[CellType::Type3].loadFromFile(
        interface::source_dir + "cells/backgrounds/cell_type3.png"
    );
    ResourceManager::m_cell_textures[CellType::Type4].loadFromFile(
        interface::source_dir + "cells/backgrounds/cell_type4.png"
    );
    ResourceManager::m_cell_textures[CellType::Type5].loadFromFile(
        interface::source_dir + "cells/backgrounds/cell_type5.png"
    );

    ResourceManager::m_cell_property_textures[CellPropertyType::Empty]
        .loadFromFile(
            interface::source_dir + "cells/properties/empty_property.png"
        );
    ResourceManager::m_cell_property_textures[CellPropertyType::Move]
        .loadFromFile(
            interface::source_dir + "cells/properties/move_property.png"
        );
    ResourceManager::m_cell_property_textures[CellPropertyType::Enemy]
        .loadFromFile(
            interface::source_dir + "cells/properties/enemy_property.png"
        );
    ResourceManager::m_cell_property_textures[CellPropertyType::Attack]
        .loadFromFile(
            interface::source_dir + "cells/properties/attack_property.png"
        );
    ResourceManager::m_cell_property_textures[CellPropertyType::Spell]
        .loadFromFile(
            interface::source_dir + "cells/properties/spell_property.png"
        );
    ResourceManager::m_cell_property_textures[CellPropertyType::AttackSpell]
        .loadFromFile(
            interface::source_dir + "cells/properties/attack_property.png"
        );

    ResourceManager::m_cracks_textures[CracksStage::Stage0].loadFromFile(
        interface::source_dir + "cells/cracks/cracks_stage0.png"
    );
    ResourceManager::m_cracks_textures[CracksStage::Stage1].loadFromFile(
        interface::source_dir + "cells/cracks/cracks_stage1.png"
    );
    ResourceManager::m_cracks_textures[CracksStage::Stage2].loadFromFile(
        interface::source_dir + "cells/cracks/cracks_stage2.png"
    );
    ResourceManager::m_cracks_textures[CracksStage::Stage3].loadFromFile(
        interface::source_dir + "cells/cracks/cracks_stage3.png"
    );
    ResourceManager::m_cracks_textures[CracksStage::Stage4].loadFromFile(
        interface::source_dir + "cells/cracks/cracks_stage4.png"
    );
    ResourceManager::m_cracks_textures[CracksStage::Stage5].loadFromFile(
        interface::source_dir + "cells/cracks/cracks_stage5.png"
    );

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
        interface::source_dir + "units_selected/unit_selected_type1.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type2].loadFromFile(
        interface::source_dir + "units_selected/unit_selected_type2.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type3].loadFromFile(
        interface::source_dir + "units_selected/unit_selected_type3.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type4].loadFromFile(
        interface::source_dir + "units_selected/unit_selected_type4.png"
    );
    ResourceManager::m_selected_unit_textures[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_selected/unit_selected_type5.png"
    );

    ResourceManager::m_move_animation[UnitType::Type1].loadFromFile(
        interface::source_dir + "units_move/unit_move_type1.png"
    );
    ResourceManager::m_move_animation[UnitType::Type2].loadFromFile(
        interface::source_dir + "units_move/unit_move_type2.png"
    );
    ResourceManager::m_move_animation[UnitType::Type3].loadFromFile(
        interface::source_dir + "units_move/unit_move_type3.png"
    );
    ResourceManager::m_move_animation[UnitType::Type4].loadFromFile(
        interface::source_dir + "units_move/unit_move_type4.png"
    );
    ResourceManager::m_move_animation[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_move/unit_move_type5.png"
    );

    m_amount_of_sprites_in_move_animation[UnitType::Type1] = 6;
    m_amount_of_sprites_in_move_animation[UnitType::Type2] = 6;
    m_amount_of_sprites_in_move_animation[UnitType::Type3] = 6;
    m_amount_of_sprites_in_move_animation[UnitType::Type4] = 6;
    m_amount_of_sprites_in_move_animation[UnitType::Type5] = 6;

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
        interface::source_dir + "units_attack/unit_attack_type4.png"
    );
    ResourceManager::m_attack_animation[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_attack/unit_attack_type5.png"
    );

    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type1] =
        4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type2] =
        4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type3] =
        4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type4] =
        4;
    ResourceManager::m_amount_of_sprites_in_attack_animation[UnitType::Type5] =
        4;

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
    ResourceManager::m_amount_of_sprites_in_hurt_animation[UnitType::Type5] = 2;

    ResourceManager::m_dead_animation[UnitType::Type1].loadFromFile(
        interface::source_dir + "units_dead/unit_dead_type1.png"
    );
    ResourceManager::m_dead_animation[UnitType::Type2].loadFromFile(
        interface::source_dir + "units_dead/unit_dead_type2.png"
    );
    ResourceManager::m_dead_animation[UnitType::Type3].loadFromFile(
        interface::source_dir + "units_dead/unit_dead_type3.png"
    );
    ResourceManager::m_dead_animation[UnitType::Type4].loadFromFile(
        interface::source_dir + "units_dead/unit_dead_type4.png"
    );
    ResourceManager::m_dead_animation[UnitType::Type5].loadFromFile(
        interface::source_dir + "units_dead/unit_dead_type5.png"
    );

    ResourceManager::m_amount_of_sprites_in_dead_animation[UnitType::Type1] = 4;
    ResourceManager::m_amount_of_sprites_in_dead_animation[UnitType::Type2] = 4;
    ResourceManager::m_amount_of_sprites_in_dead_animation[UnitType::Type3] = 4;
    ResourceManager::m_amount_of_sprites_in_dead_animation[UnitType::Type4] = 4;
    ResourceManager::m_amount_of_sprites_in_dead_animation[UnitType::Type5] = 4;

    ResourceManager::m_textures[TextureType::MenuBackground].loadFromFile(
        interface::source_dir + "backgrounds/menu_background.jpg"
    );

    ResourceManager::m_fonts[interface::Fonts::CaptionFont].loadFromFile(
        interface::source_dir + "fonts/caption_font.otf"
    );
    ResourceManager::m_fonts[interface::Fonts::TittleFont].loadFromFile(
        interface::source_dir + "fonts/tittle_font.ttf"
    );

    ResourceManager::m_sounds[interface::Sounds::MenuBackgroundSound]
        .loadFromFile(interface::source_dir + "sounds/menu_background.wav");
    ResourceManager::m_sounds[interface::Sounds::GameBackgroundSound]
        .loadFromFile(interface::source_dir + "sounds/game_background.wav");

    ResourceManager::m_cursors[CursorType::Attack].loadFromFile(
        interface::source_dir + "cursors/sword.png"
    );

    is_initialised = true;
}

const sf::Texture &ResourceManager::load_icon(Characters texture) {
    load_resources();
    return m_icons[texture];
}

const sf::Texture &ResourceManager::load_cell_texture(CellType texture) {
    load_resources();
    return m_cell_textures[texture];
}

const sf::Texture &ResourceManager::load_cell_property_texture(
    CellPropertyType texture
) {
    load_resources();
    return m_cell_property_textures[texture];
}

[[nodiscard]] const sf::Texture &ResourceManager::load_cracks_texture(
    CracksStage texture
) {
    load_resources();
    return m_cracks_textures[texture];
}

const sf::Texture &ResourceManager::load_unit_texture(UnitType texture) {
    load_resources();
    return m_unit_textures[texture];
}

const sf::Texture &ResourceManager::load_selected_unit_texture(UnitType texture
) {
    load_resources();
    return m_selected_unit_textures[texture];
}

const sf::Font &ResourceManager::load_font(interface::Fonts font) {
    load_resources();
    return m_fonts[font];
}

const sf::Texture &ResourceManager::load_texture(TextureType texture) {
    load_resources();
    return m_textures[texture];
}

const sf::Image &ResourceManager::load_cursor(CursorType cursor) {
    load_resources();
    return m_cursors[cursor];
}

const sf::Texture &ResourceManager::load_attack_animation_sprite_sheet(
    UnitType texture
) {
    load_resources();
    return m_attack_animation[texture];
}

const sf::Texture &ResourceManager::load_hurt_animation_sprite_sheet(
    UnitType texture
) {
    load_resources();
    return m_hurt_animation[texture];
}

const sf::Texture &ResourceManager::load_move_animation_sprite_sheet(
    UnitType texture
) {
    load_resources();
    return m_move_animation[texture];
}

const sf::Texture &ResourceManager::load_dead_animation_sprite_sheet(
    UnitType texture
) {
    load_resources();
    return m_dead_animation[texture];
}

int ResourceManager::amount_of_frames_in_attack_animation(UnitType type) {
    load_resources();
    return m_amount_of_sprites_in_attack_animation[type];
}

int ResourceManager::amount_of_frames_in_hurt_animation(UnitType type) {
    load_resources();
    return m_amount_of_sprites_in_hurt_animation[type];
}

int ResourceManager::amount_of_frames_in_move_animation(UnitType type) {
    load_resources();
    return m_amount_of_sprites_in_move_animation[type];
}

int ResourceManager::amount_of_frames_in_dead_animation(UnitType type) {
    load_resources();
    return m_amount_of_sprites_in_dead_animation[type];
}

const sf::SoundBuffer &ResourceManager::load_sound(interface::Sounds sound) {
    load_resources();
    return m_sounds[sound];
}

const sf::Texture &ResourceManager::load_sound_icon(int texture) {
    load_resources();
    return m_sound_icons[texture];
}
}  // namespace game_interface