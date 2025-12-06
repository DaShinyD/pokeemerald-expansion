#include "constants/abilities.h"
#include "species_info/shared_dex_text.h"
#include "species_info/shared_front_pic_anims.h"

// Macros for ease of use.

#define EVOLUTION(...) (const struct Evolution[]) { __VA_ARGS__, { EVOLUTIONS_END }, }

#define ANIM_FRAMES(...) (const union AnimCmd *const[]) { sAnim_GeneralFrame0, (const union AnimCmd[]) { __VA_ARGS__ ANIMCMD_END, }, }

#if P_FOOTPRINTS
#define FOOTPRINT(sprite) .footprint = gMonFootprint_## sprite,
#else
#define FOOTPRINT(sprite)
#endif

#if B_ENEMY_MON_SHADOW_STYLE >= GEN_4 && P_GBA_STYLE_SPECIES_GFX == FALSE
#define SHADOW(x, y, size)  .enemyShadowXOffset = x, .enemyShadowYOffset = y, .enemyShadowSize = size,
#define NO_SHADOW           .suppressEnemyShadow = TRUE,
#else
#define SHADOW(x, y, size)  .enemyShadowXOffset = 0, .enemyShadowYOffset = 0, .enemyShadowSize = 0,
#define NO_SHADOW           .suppressEnemyShadow = FALSE,
#endif

#define SIZE_32x32 1
#define SIZE_64x64 0

// Set .compressed = OW_GFX_COMPRESS
#define COMP OW_GFX_COMPRESS

#if OW_POKEMON_OBJECT_EVENTS
#if OW_PKMN_OBJECTS_SHARE_PALETTES == FALSE
#define OVERWORLD_PAL(...)                                  \
    .overworldPalette = DEFAULT(NULL, __VA_ARGS__),         \
    .overworldShinyPalette = DEFAULT_2(NULL, __VA_ARGS__),
#if P_GENDER_DIFFERENCES
#define OVERWORLD_PAL_FEMALE(...)                                 \
    .overworldPaletteFemale = DEFAULT(NULL, __VA_ARGS__),         \
    .overworldShinyPaletteFemale = DEFAULT_2(NULL, __VA_ARGS__),
#else
#define OVERWORLD_PAL_FEMALE(...)
#endif //P_GENDER_DIFFERENCES
#else
#define OVERWORLD_PAL(...)
#define OVERWORLD_PAL_FEMALE(...)
#endif //OW_PKMN_OBJECTS_SHARE_PALETTES == FALSE

#define OVERWORLD_DATA(picTable, _size, shadow, _tracks, _anims)                                                                     \
{                                                                                                                                       \
    .tileTag = TAG_NONE,                                                                                                                \
    .paletteTag = OBJ_EVENT_PAL_TAG_DYNAMIC,                                                                                            \
    .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,                                                                                     \
    .size = (_size == SIZE_32x32 ? 512 : 2048),                                                                                         \
    .width = (_size == SIZE_32x32 ? 32 : 64),                                                                                           \
    .height = (_size == SIZE_32x32 ? 32 : 64),                                                                                          \
    .paletteSlot = PALSLOT_NPC_1,                                                                                                       \
    .shadowSize = shadow,                                                                                                               \
    .inanimate = FALSE,                                                                                                                 \
    .compressed = COMP,                                                                                                                 \
    .tracks = _tracks,                                                                                                                  \
    .oam = (_size == SIZE_32x32 ? &gObjectEventBaseOam_32x32 : &gObjectEventBaseOam_64x64),                                             \
    .subspriteTables = (_size == SIZE_32x32 ? sOamTables_32x32 : sOamTables_64x64),                                                     \
    .anims = _anims,                                                                                                                    \
    .images = picTable,                                                                                                                 \
    .affineAnims = gDummySpriteAffineAnimTable,                                                                                         \
}

#define OVERWORLD(objEventPic, _size, shadow, _tracks, _anims, ...)                                 \
    .overworldData = OVERWORLD_DATA(objEventPic, _size, shadow, _tracks, _anims),                   \
    OVERWORLD_PAL(__VA_ARGS__)

#if P_GENDER_DIFFERENCES
#define OVERWORLD_FEMALE(objEventPic, _size, shadow, _tracks, _anims, ...)                          \
    .overworldDataFemale = OVERWORLD_DATA(objEventPic, _size, shadow, _tracks, _anims),             \
    OVERWORLD_PAL_FEMALE(__VA_ARGS__)
#else
#define OVERWORLD_FEMALE(...)
#endif //P_GENDER_DIFFERENCES

#else
#define OVERWORLD(...)
#define OVERWORLD_FEMALE(...)
#define OVERWORLD_PAL(...)
#define OVERWORLD_PAL_FEMALE(...)
#endif //OW_POKEMON_OBJECT_EVENTS

// Maximum value for a female Pokémon is 254 (MON_FEMALE) which is 100% female.
// 255 (MON_GENDERLESS) is reserved for genderless Pokémon.
#define PERCENT_FEMALE(percent) min(254, ((percent * 255) / 100))

#define MON_TYPES(type1, ...) { type1, DEFAULT(type1, __VA_ARGS__) }
#define MON_EGG_GROUPS(group1, ...) { group1, DEFAULT(group1, __VA_ARGS__) }

#define FLIP    0
#define NO_FLIP 1

const struct SpeciesInfo gSpeciesInfo[] =
{
    [SPECIES_NONE] =
    {
        .speciesName = _("??????????"),
        .cryId = CRY_NONE,
        .natDexNum = NATIONAL_DEX_NONE,
        .categoryName = _("Unknown"),
        .height = 0,
        .weight = 0,
        .description = gFallbackPokedexText,
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CircledQuestionMark,
        .frontPicSize = MON_COORDS_SIZE(40, 40),
        .frontPicYOffset = 12,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CircledQuestionMark,
        .backPicSize = MON_COORDS_SIZE(40, 40),
        .backPicYOffset = 12,
        .backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CircledQuestionMark,
        .shinyPalette = gMonShinyPalette_CircledQuestionMark,
        .iconSprite = gMonIcon_QuestionMark,
        .iconPalIndex = 0,
        FOOTPRINT(QuestionMark)
        SHADOW(1, 4, SHADOW_SIZE_S)
    #if OW_POKEMON_OBJECT_EVENTS
        .overworldData = {
            .tileTag = TAG_NONE,
            .paletteTag = OBJ_EVENT_PAL_TAG_SUBSTITUTE,
            .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,
            .size = 512,
            .width = 32,
            .height = 32,
            .paletteSlot = PALSLOT_NPC_1,
            .shadowSize = SHADOW_SIZE_M,
            .inanimate = FALSE,
            .compressed = COMP,
            .tracks = TRACKS_FOOT,
            .oam = &gObjectEventBaseOam_32x32,
            .subspriteTables = sOamTables_32x32,
            .anims = sAnimTable_Following,
            .images = sPicTable_Substitute,
            .affineAnims = gDummySpriteAffineAnimTable,
        },
    #endif
        .levelUpLearnset = sNoneLevelUpLearnset,
        .teachableLearnset = sNoneTeachableLearnset,
        .eggMoveLearnset = sNoneEggMoveLearnset,
    },

    #include "species_info/gen_1_families.h"
    #include "species_info/gen_2_families.h"
    #include "species_info/gen_3_families.h"
    #include "species_info/gen_4_families.h"
    #include "species_info/gen_5_families.h"
    #include "species_info/gen_6_families.h"
    #include "species_info/gen_7_families.h"
    #include "species_info/gen_8_families.h"
    #include "species_info/gen_9_families.h"

    [SPECIES_EGG] =
    {
        .frontPic = gMonFrontPic_Egg,
        .frontPicSize = MON_COORDS_SIZE(24, 24),
        .frontPicYOffset = 20,
        .backPic = gMonFrontPic_Egg,
        .backPicSize = MON_COORDS_SIZE(24, 24),
        .backPicYOffset = 20,
        .palette = gMonPalette_Egg,
        .shinyPalette = gMonPalette_Egg,
        .iconSprite = gMonIcon_Egg,
        .iconPalIndex = 1,
    },

[SPECIES_LEDIASTRA] =
    {
        .baseHP        = 75,
        .baseAttack    = 130,
        .baseDefense   = 70,
        .baseSpeed     = 130,
        .baseSpAttack  = 45,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_BUG, TYPE_FIGHTING),
        .catchRate = 90,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 137 : 134,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_SWARM, ABILITY_IRON_FIST, ABILITY_IRON_FIST },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Lediastra"),
        .cryId = CRY_LEDIAN,
        .natDexNum = NATIONAL_DEX_LEDIASTRA,
        .categoryName = _("Pummel Bug"),
        .height = 18,
        .weight = 396,
        .description = COMPOUND_STRING(
            "This Pokemon loves to beat on weaker\n"
            "bugs. It can often be found using\n"
            "it's many arms to bully."),
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Lediastra,
        .frontPicFemale = gMonFrontPic_Lediastra,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicSizeFemale = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = 6,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SLIDE_WOBBLE,
        .enemyMonElevation = 10,
        .backPic = gMonBackPic_Lediastra,
        .backPicFemale = gMonBackPic_Lediastra,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 3,
        .backAnimId = BACK_ANIM_CONVEX_DOUBLE_ARC,
        .palette = gMonPalette_Lediastra,
        .shinyPalette = gMonShinyPalette_Lediastra,
        .iconSprite = gMonIcon_Lediastra,
        .iconPalIndex = 0,
        FOOTPRINT(Ledian)
        SHADOW(2, 11, SHADOW_SIZE_L)
        OVERWORLD(
            sPicTable_Ledian,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Ledian,
            gShinyOverworldPalette_Ledian
        )
        .levelUpLearnset = sLedianLevelUpLearnset,
        .teachableLearnset = sLedianTeachableLearnset,
    },
#if P_FAMILY_LAPRAS
#if P_MEGA_EVOLUTIONS
    [SPECIES_LAPRAS_MEGA] =
    {
        .baseHP        = 150,
        .baseAttack    = 85,
        .baseDefense   = 90,
        .baseSpeed     = 60,
        .baseSpAttack  = 125,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_WATER, TYPE_ICE),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 187 : 219,
        .evYield_HP = 2,
        .itemCommon = ITEM_MYSTIC_WATER,
        .itemRare = ITEM_MYSTIC_WATER,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_WATER_1),
        .abilities = { ABILITY_WATER_ABSORB, ABILITY_SHELL_ARMOR, ABILITY_HYDRATION },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Lapras"),
        .cryId = CRY_LAPRAS,
        .natDexNum = NATIONAL_DEX_LAPRAS,
        .categoryName = _("Transport"),
        .height = 25,
        .weight = 2200,
        .description = COMPOUND_STRING(
            "People have driven Lapras almost to the\n"
            "point of extinction. In the evenings,\n"
            "it is said to sing plaintively as it seeks\n"
            "what few others of its kind still remain."),
        .pokemonScale = 257,
        .pokemonOffset = 10,
        .trainerScale = 423,
        .trainerOffset = 8,
        .frontPic = gMonFrontPic_LaprasMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_LaprasMega,
        .backPicSize = MON_COORDS_SIZE(56, 64),
        .backPicYOffset = 3,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_LaprasMega,
        .shinyPalette = gMonShinyPalette_LaprasMega,
        .iconSprite = gMonIcon_LaprasGmax,
        .iconPalIndex = 2,
        FOOTPRINT(Lapras)
        OVERWORLD(
            sPicTable_Lapras,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Lapras,
            gShinyOverworldPalette_Lapras
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sLaprasLevelUpLearnset,
        .teachableLearnset = sLaprasTeachableLearnset,
        .eggMoveLearnset = sLaprasEggMoveLearnset,
        .formSpeciesIdTable = sLaprasFormSpeciesIdTable,
        .formChangeTable = sLaprasFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_KRABBY
#if P_MEGA_EVOLUTIONS
    [SPECIES_KINGLER_MEGA] =
    {
        .baseHP        = 55,
        .baseAttack    = 150,
        .baseDefense   = 115,
        .baseSpeed     = 95,
        .baseSpAttack  = 50,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 60,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 166 : 206,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_3),
        .abilities = { ABILITY_HYPER_CUTTER, ABILITY_SHELL_ARMOR, ABILITY_SHEER_FORCE },
        .bodyColor = BODY_COLOR_RED,
        .noFlip = TRUE,
        .speciesName = _("Kingler"),
        .cryId = CRY_KINGLER,
        .natDexNum = NATIONAL_DEX_KINGLER,
        .categoryName = _("Pincer"),
        .height = 190,
        .weight = 0,
        .description = COMPOUND_STRING(
            "The flow of Gmax energy has\n"
            "spurred this Pokémon's left pincer\n"
            "to grow to an enormous size. That\n"
            "claw can pulverize anything."),
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_KinglerGmax,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_KinglerGmax,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 5,
        //.backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_KinglerGmax,
        .shinyPalette = gMonShinyPalette_KinglerGmax,
        .iconSprite = gMonIcon_KinglerGmax,
        .iconPalIndex = 0,
        FOOTPRINT(Kingler)
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sKinglerLevelUpLearnset,
        .teachableLearnset = sKinglerTeachableLearnset,
        .formSpeciesIdTable = sKinglerFormSpeciesIdTable,
        .formChangeTable = sKinglerFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_SNORLAX
#if P_MEGA_EVOLUTIONS
    [SPECIES_SNORLAX_MEGA] =
    {
        .baseHP        = 180,
        .baseAttack    = 110,
        .baseDefense   = 85,
        .baseSpeed     = 30,
        .baseSpAttack  = 65,
        .baseSpDefense = 120,
        .types = MON_TYPES(TYPE_NORMAL),
        .catchRate = 25,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 189 : 154,
        .evYield_HP = 2,
        .itemCommon = ITEM_LEFTOVERS,
        .itemRare = ITEM_LEFTOVERS,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 40,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER),
        .abilities = { ABILITY_IMMUNITY, ABILITY_THICK_FAT, ABILITY_GLUTTONY },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Snorlax"),
        .cryId = CRY_SNORLAX,
        .natDexNum = NATIONAL_DEX_SNORLAX,
        .categoryName = _("Sleeping"),
        .height = 350,
        .weight = 0,
        .description = COMPOUND_STRING(
            "Gmax energy has affected\n"
            "stray seeds and even pebbles that\n"
            "got stuck to Snorlax, making them grow\n"
            "to a huge size."),
        .pokemonScale = 256,
        .pokemonOffset = 4,
        .trainerScale = 423,
        .trainerOffset = 11,
        .frontPic = gMonFrontPic_SnorlaxGmax,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_SnorlaxGmax,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 5,
        //.backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_SnorlaxGmax,
        .shinyPalette = gMonShinyPalette_SnorlaxGmax,
        .iconSprite = gMonIcon_SnorlaxGmax,
        .iconPalIndex = 3,
        FOOTPRINT(Snorlax)
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sSnorlaxLevelUpLearnset,
        .teachableLearnset = sSnorlaxTeachableLearnset,
        .eggMoveLearnset = sSnorlaxEggMoveLearnset,
        .formSpeciesIdTable = sSnorlaxFormSpeciesIdTable,
        .formChangeTable = sSnorlaxFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_MACHOP
#if P_MEGA_EVOLUTIONS
    [SPECIES_MACHAMP_MEGA] =
    {
        .baseHP        = 120,
        .baseAttack    = 150,
        .baseDefense   = 80,
        .baseSpeed     = 55,
        .baseSpAttack  = 65,
        .baseSpDefense = 85,
        .types = MON_TYPES(TYPE_FIGHTING),
        .catchRate = 45,
        .expYield = MACHAMP_EXP_YIELD,
        .evYield_Attack = 3,
        .itemRare = ITEM_FOCUS_BAND,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_GUTS, ABILITY_NO_GUARD, ABILITY_STEADFAST },
    #else
        .abilities = { ABILITY_GUTS, ABILITY_NONE, ABILITY_STEADFAST },
    #endif
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Machamp"),
        .cryId = CRY_MACHAMP,
        .natDexNum = NATIONAL_DEX_MACHAMP,
        .categoryName = _("Superpower"),
        .height = 250,
        .weight = 0,
        .description = COMPOUND_STRING(
            "One of these Pokémon once used\n"
            "its immeasurable strength to lift a\n"
            "large ship that was in trouble. It\n"
            "then carried the ship to port."),
        .pokemonScale = 280,
        .pokemonOffset = 1,
        .trainerScale = 269,
        .trainerOffset = -1,
        .frontPic = gMonFrontPic_MachampGmax,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_MachampGmax,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 3,
        //.backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_MachampGmax,
        .shinyPalette = gMonShinyPalette_MachampGmax,
        .iconSprite = gMonIcon_MachampGmax,
        .iconPalIndex = 0,
        FOOTPRINT(Machamp)
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sMachampLevelUpLearnset,
        .teachableLearnset = sMachampTeachableLearnset,
        .formSpeciesIdTable = sMachampFormSpeciesIdTable,
        .formChangeTable = sMachampFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_CATERPIE
#if P_MEGA_EVOLUTIONS
    [SPECIES_BUTTERFREE_MEGA] =
    {
        .baseHP        = 70,
        .baseAttack    = 45,
        .baseDefense   = 70,
        .baseSpeed     = 110,
        .baseSpAttack  = P_UPDATED_STATS >= GEN_6 ? 130 : 130,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_BUG, TYPE_PSYCHIC),
        .catchRate = 45,
        .expYield = BUTTERFREE_EXP_YIELD,
        .evYield_SpAttack = 2,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_SILVER_POWDER,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_COMPOUND_EYES, ABILITY_NONE, ABILITY_TINTED_LENS },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Butterfree"),
        .cryId = CRY_BUTTERFREE,
        .natDexNum = NATIONAL_DEX_BUTTERFREE,
        .categoryName = _("Butterfly"),
        .height = 170,
        .weight = 0,
        .description = COMPOUND_STRING(
            "Once it has opponents trapped in a\n"
            "tornado that could blow away a 10-\n"
            "ton truck, it finishes them off with\n"
            "its poisonous scales."),
        .pokemonScale = 312,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_ButterfreeGmax,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 3,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .enemyMonElevation = 7,
        .backPic = gMonBackPic_ButterfreeGmax,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 3,
        //.backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_ButterfreeGmax,
        .shinyPalette = gMonShinyPalette_ButterfreeGmax,
        .iconSprite = gMonIcon_ButterfreeGmax,
        .iconPalIndex = 0,
        FOOTPRINT(Butterfree)
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sButterfreeLevelUpLearnset,
        .teachableLearnset = sButterfreeTeachableLearnset,
        .formSpeciesIdTable = sButterfreeFormSpeciesIdTable,
        .formChangeTable = sButterfreeFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_SIZZLIPEDE
#if P_MEGA_EVOLUTIONS
    [SPECIES_CENTISKORCH_MEGA] =
    {
        .baseHP        = 130,
        .baseAttack    = 135,
        .baseDefense   = 65,
        .baseSpeed     = 65,
        .baseSpAttack  = 90,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_FIRE, TYPE_BUG),
        .catchRate = 75,
        .expYield = 184,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_FLASH_FIRE, ABILITY_WHITE_SMOKE, ABILITY_FLAME_BODY },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Centiskorch"),
        .cryId = CRY_CENTISKORCH,
        .natDexNum = NATIONAL_DEX_CENTISKORCH,
        .categoryName = _("Radiator"),
        .height = 750,
        .weight = 0,
        .description = COMPOUND_STRING(
            "The heat that comes off a\n"
            "Gmax Centiskorch may destabilize air\n"
            "currents. Sometimes it can even\n"
            "cause storms."),
        .pokemonScale = 275,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_CentiskorchGmax,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 5,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_CentiskorchGmax,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 1,
        //.backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_CentiskorchGmax,
        .shinyPalette = gMonShinyPalette_CentiskorchGmax,
        .iconSprite = gMonIcon_CentiskorchGmax,
        .iconPalIndex = 0,
        FOOTPRINT(Centiskorch)
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sCentiskorchLevelUpLearnset,
        .teachableLearnset = sCentiskorchTeachableLearnset,
        .formSpeciesIdTable = sCentiskorchFormSpeciesIdTable,
        .formChangeTable = sCentiskorchFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_TOTODILE
#if P_MEGA_EVOLUTIONS
    [SPECIES_FERALIGATR_MEGA] =
    {
        .baseHP        = 85,
        .baseAttack    = 135,
        .baseDefense   = 120,
        .baseSpeed     = 78,
        .baseSpAttack  = 79,
        .baseSpDefense = 93,
        .types = MON_TYPES(TYPE_WATER, TYPE_DARK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 265,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 239,
    #else
        .expYield = 210,
    #endif
        .evYield_Attack = 2,
        .evYield_Defense = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_WATER_1),
        .abilities = { ABILITY_STRONG_JAW, ABILITY_NONE, ABILITY_SHEER_FORCE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Feraligatr"),
        .cryId = CRY_FERALIGATR,
        .natDexNum = NATIONAL_DEX_FERALIGATR,
        .categoryName = _("Big Jaw"),
        .height = 23,
        .weight = 888,
        .description = COMPOUND_STRING(
            "It opens its huge mouth to intimidate\n"
            "enemies. In battle, it runs using its thick\n"
            "and powerful hind legs to charge the\n"
            "foe with incredible speed."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 342,
        .trainerOffset = 7,
        .frontPic = gMonFrontPic_FeraligatrMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .frontAnimDelay = 5,
        .backPic = gMonBackPic_FeraligatrMega,
        .backPicSize = MON_COORDS_SIZE(56, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_FeraligatrMega,
        .shinyPalette = gMonShinyPalette_FeraligatrMega,
        .iconSprite = gMonIcon_FeraligatrMega,
        .iconPalIndex = 0,
        FOOTPRINT(Feraligatr)
        OVERWORLD(
            sPicTable_Feraligatr,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Feraligatr,
            gShinyOverworldPalette_Feraligatr
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sFeraligatrLevelUpLearnset,
        .teachableLearnset = sFeraligatrTeachableLearnset,
        .formSpeciesIdTable = sFeraligatrFormSpeciesIdTable,
        .formChangeTable = sFeraligatrFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_CYNDAQUIL
#if P_MEGA_EVOLUTIONS
    [SPECIES_TYPHLOSION_MEGA] =
    {
        .baseHP        = 90,
        .baseAttack    = 84,
        .baseDefense   = 78,
        .baseSpeed     = 120,
        .baseSpAttack  = 130,
        .baseSpDefense = 85,
        .types = MON_TYPES(TYPE_FIRE, TYPE_GROUND),
        .catchRate = 45,
        .expYield = TYPHLOSION_EXP_YIELD,
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_BLAZE, ABILITY_NONE, ABILITY_FLASH_FIRE },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Typhlosion"),
        .cryId = CRY_TYPHLOSION,
        .natDexNum = NATIONAL_DEX_TYPHLOSION,
        .categoryName = _("Volcano"),
        .height = 17,
        .weight = 795,
        .description = COMPOUND_STRING(
            "It can hide behind a shimmering heat haze\n"
            "that it creates using its intense flames.\n"
            "Typhlosion create blazing explosive\n"
            "blasts that burn everything to cinders."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 268,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_TyphlosionMega,
        .frontPicSize = MON_COORDS_SIZE(56, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .frontAnimDelay = 20,
        .backPic = gMonBackPic_TyphlosionMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 3,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_TyphlosionMega,
        .shinyPalette = gMonShinyPalette_TyphlosionMega,
        .iconSprite = gMonIcon_TyphlosionMega,
        .iconPalIndex = 3,
        FOOTPRINT(Typhlosion)
        OVERWORLD(
            sPicTable_Typhlosion,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Typhlosion,
            gShinyOverworldPalette_Typhlosion
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sTyphlosionLevelUpLearnset,
        .teachableLearnset = sTyphlosionTeachableLearnset,
        .formSpeciesIdTable = sTyphlosionFormSpeciesIdTable,
        .formChangeTable = sTyphlosionFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_CHIKORITA
#if P_MEGA_EVOLUTIONS
    [SPECIES_MEGANIUM_MEGA] =
    {
        .baseHP        = 100,
        .baseAttack    = 82,
        .baseDefense   = 130,
        .baseSpeed     = 80,
        .baseSpAttack  = 83,
        .baseSpDefense = 130,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FAIRY),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 263,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 236,
    #else
        .expYield = 208,
    #endif
        .evYield_Defense = 1,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_OVERGROW, ABILITY_NONE, ABILITY_LEAF_GUARD },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Meganium"),
        .cryId = CRY_MEGANIUM,
        .natDexNum = NATIONAL_DEX_MEGANIUM,
        .categoryName = _("Herb"),
        .height = 18,
        .weight = 1005,
        .description = COMPOUND_STRING(
            "The fragrance of a Meganium's flower\n"
            "soothes and calms emotions. In battle,\n"
            "it gives off more of its becalming scent\n"
            "to blunt the foe's fighting spirit."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 277,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_MeganiumMega,
        .frontPicFemale = gMonFrontPic_MeganiumMega,
        .frontPicSize = MON_COORDS_SIZE(48, 64),
        .frontPicSizeFemale = MON_COORDS_SIZE(48, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_MeganiumMega,
        .backPicFemale = gMonBackPic_MeganiumMega,
        .backPicSize = MON_COORDS_SIZE(56, 64),
        .backPicSizeFemale = MON_COORDS_SIZE(56, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_MeganiumMega,
        .shinyPalette = gMonShinyPalette_MeganiumMega,
        .iconSprite = gMonIcon_MeganiumMega,
        .iconPalIndex = 1,
        FOOTPRINT(Meganium)
        OVERWORLD(
            sPicTable_Meganium,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Meganium,
            gShinyOverworldPalette_Meganium
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sMeganiumLevelUpLearnset,
        .teachableLearnset = sMeganiumTeachableLearnset,
        .formSpeciesIdTable = sMeganiumFormSpeciesIdTable,
        .formChangeTable = sMeganiumFormChangeTable,
    },
#endif
#endif

    [SPECIES_AQUAR] =
    {
        .baseHP        = 41,
        .baseAttack    = 64,
        .baseDefense   = 45,
        .baseSpeed     = 50,
        .baseSpAttack  = 50,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_WATER),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 60 : 67,
        .evYield_Attack = 1,
        .itemRare = ITEM_DRAGON_SCALE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
        .abilities = { ABILITY_SHED_SKIN, ABILITY_NONE, ABILITY_MARVEL_SCALE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Aquar"),
        .cryId = CRY_DRATINI,
        .natDexNum = NATIONAL_DEX_AQUAR,
        .categoryName = _("Dragon"),
        .height = 18,
        .weight = 33,
        .description = COMPOUND_STRING(
            "Thought to be a descendent of Dratini,\n"
            "it shares many characteristics. It can\n"
            "often be found hanging around them."),
        .pokemonScale = 256,
        .pokemonOffset = 8,
        .trainerScale = 386,
        .trainerOffset = 6,
        .frontPic = gMonFrontPic_Aquar,
        .frontPicSize = MON_COORDS_SIZE(56, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_Aquar,
        .backPicSize = MON_COORDS_SIZE(56, 48),
        .backPicYOffset = 22,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_Aquar,
        .shinyPalette = gMonShinyPalette_Aquar,
        .iconSprite = gMonIcon_Aquar,
        .iconPalIndex = 4,
        FOOTPRINT(Popplio)
        OVERWORLD(
            sPicTable_Dratini,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dratini,
            gShinyOverworldPalette_Dratini
        )
        .levelUpLearnset = sAquarLevelUpLearnset,
        .teachableLearnset = sDratiniTeachableLearnset,
        .eggMoveLearnset = sDratiniEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 30, SPECIES_AQUARIA}),
    },

    [SPECIES_AQUARIA] =
    {
        .baseHP        = 61,
        .baseAttack    = 84,
        .baseDefense   = 65,
        .baseSpeed     = 70,
        .baseSpAttack  = 70,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_WATER),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 147 : 144,
        .evYield_Attack = 2,
        .itemRare = ITEM_DRAGON_SCALE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
        .abilities = { ABILITY_SHED_SKIN, ABILITY_NONE, ABILITY_MARVEL_SCALE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Aquaria"),
        .cryId = CRY_DRAGONAIR,
        .natDexNum = NATIONAL_DEX_AQUARIA,
        .categoryName = _("Dragon"),
        .height = 40,
        .weight = 165,
        .description = COMPOUND_STRING(
            "Aquaria harnesses the power of water\n"
            "to protect its herd. It can unleash\n"
            "500 gallons of water."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 411,
        .trainerOffset = 5,
        .frontPic = gMonFrontPic_Aquaria,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_Aquaria,
        .backPicSize = MON_COORDS_SIZE(40, 56),
        .backPicYOffset = 5,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_Aquaria,
        .shinyPalette = gMonShinyPalette_Aquaria,
        .iconSprite = gMonIcon_Aquaria,
        .iconPalIndex = 4,
        FOOTPRINT(Brionne)
        OVERWORLD(
            sPicTable_Dragonair,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dragonair,
            gShinyOverworldPalette_Dragonair
        )
        .levelUpLearnset = sAquariaLevelUpLearnset,
        .teachableLearnset = sDragonairTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 55, SPECIES_AQUARIUS}),
    },

    [SPECIES_AQUARIUS] =
    {
        .baseHP        = 120,
        .baseAttack    = 145,
        .baseDefense   = 95,
        .baseSpeed     = 120,
        .baseSpAttack  = 80,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_WATER),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 300,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 270,
    #else
        .expYield = 218,
    #endif
        .evYield_Attack = 3,
        .itemRare = ITEM_DRAGON_SCALE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
        .abilities = { ABILITY_INNER_FOCUS, ABILITY_NONE, ABILITY_MULTISCALE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Aquarius"),
        .cryId = CRY_DRAGONITE,
        .natDexNum = NATIONAL_DEX_AQUARIUS,
        .categoryName = _("Dragon"),
        .height = 22,
        .weight = 2100,
        .description = COMPOUND_STRING(
            "It can cross an ocean in under 3 hours.\n"
            "This incredible speed makes it one of\n"
            "the hardest hitters in water."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 309,
        .trainerOffset = 4,
        .frontPic = gMonFrontPic_Aquarius,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 8,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Aquarius,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_Aquarius,
        .shinyPalette = gMonShinyPalette_Aquarius,
        .iconSprite = gMonIcon_Aquarius,
        .iconPalIndex = 4,
        FOOTPRINT(Primarina)
        OVERWORLD(
            sPicTable_Primarina,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Primarina,
            gShinyOverworldPalette_Primarina
        )
        .levelUpLearnset = sAquariusLevelUpLearnset,
        .teachableLearnset = sDragoniteTeachableLearnset,
    },

    [SPECIES_CRYSTAL_ONIX] =
    {
        .baseHP        = 65,
        .baseAttack    = 45,
        .baseDefense   = 160,
        .baseSpeed     = 40,
        .baseSpAttack  = 30,
        .baseSpDefense = 145,
        .types = MON_TYPES(TYPE_ROCK, TYPE_ICE),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 77 : 108,
        .evYield_Defense = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MINERAL),
        .abilities = { ABILITY_ROCK_HEAD, ABILITY_STURDY, ABILITY_WEAK_ARMOR },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Onix"),
        .cryId = CRY_ONIX,
        .natDexNum = NATIONAL_DEX_CRYSTAL_ONIX,
        .categoryName = _("Ice Snake"),
        .height = 88,
        .weight = 2100,
        .description = COMPOUND_STRING(
            "After eating stones in cold mountains,\n"
            "its body began to change. A layer\n"
            "of ice formed, altering it."),
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 515,
        .trainerOffset = 14,
        .frontPic = gMonFrontPic_CrystalOnix,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_CrystalOnix,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 1,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_CrystalOnix,
        .shinyPalette = gMonShinyPalette_CrystalOnix,
        .iconSprite = gMonIcon_CrystalOnix,
        .iconPalIndex = 0,
        FOOTPRINT(Onix)
        OVERWORLD(
            sPicTable_Onix,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Onix,
            gShinyOverworldPalette_Onix
        )
        .levelUpLearnset = sCrystalOnixLevelUpLearnset,
        .teachableLearnset = sOnixTeachableLearnset,
        .eggMoveLearnset = sOnixEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_ITEM, ITEM_ICE_STONE, SPECIES_CRYSTAL_STEELIX}),
    },

    [SPECIES_CRYSTAL_STEELIX] =
    {
        .baseHP        = 85,
        .baseAttack    = 125,
        .baseDefense   = 200,
        .baseSpeed     = 20,
        .baseSpAttack  = 35,
        .baseSpDefense = 105,
        .types = MON_TYPES(TYPE_STEEL, TYPE_ICE),
        .catchRate = 25,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 179 : 196,
        .evYield_Defense = 2,
        .itemRare = ITEM_METAL_COAT,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MINERAL),
        .abilities = { ABILITY_ROCK_HEAD, ABILITY_STURDY, ABILITY_SHEER_FORCE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Steelix"),
        .cryId = CRY_STEELIX,
        .natDexNum = NATIONAL_DEX_CRYSTAL_STEELIX,
        .categoryName = _("Ice Snake"),
        .height = 92,
        .weight = 4000,
        .description = COMPOUND_STRING(
            "Steelix live even further underground\n"
            "than Onix. This Pokémon is known to dig\n"
            "toward the earth's core, reaching a depth\n"
            "of over six-tenths of a mile underground."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 516,
        .trainerOffset = 13,
        .frontPic = gMonFrontPic_CrystalSteelix,
        .frontPicFemale = gMonFrontPic_CrystalSteelix,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .frontAnimDelay = 45,
        .backPic = gMonBackPic_CrystalSteelix,
        .backPicFemale = gMonBackPic_CrystalSteelix,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_CrystalSteelix,
        .shinyPalette = gMonShinyPalette_CrystalSteelix,
        .iconSprite = gMonIcon_CrystalSteelix,
        .iconPalIndex = 0,
        FOOTPRINT(Steelix)
        OVERWORLD(
            sPicTable_Steelix,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Steelix,
            gShinyOverworldPalette_Steelix
        )
        .levelUpLearnset = sCrystalSteelixLevelUpLearnset,
        .teachableLearnset = sSteelixTeachableLearnset,
    },

    [SPECIES_DELICLAUS] =
    {
        .baseHP        = 95,
        .baseAttack    = 45,
        .baseDefense   = 65,
        .baseSpeed     = 130,
        .baseSpAttack  = 130,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_ICE, TYPE_FLYING),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 116 : 183,
        .evYield_Speed = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
        .abilities = { ABILITY_VITAL_SPIRIT, ABILITY_HUSTLE, ABILITY_INSOMNIA },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Deliclaus"),
        .cryId = CRY_DELIBIRD,
        .natDexNum = NATIONAL_DEX_DELICLAUS,
        .categoryName = _("Jolly Giver"),
        .height = 9,
        .weight = 160,
        .description = COMPOUND_STRING(
            "It has an incredibly good nature.\n"
            "It is often seen giving gifts to\n"
            "orphaned children."),
        .pokemonScale = 293,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Deliclaus,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_Deliclaus,
        .backPicSize = MON_COORDS_SIZE(56, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_Deliclaus,
        .shinyPalette = gMonShinyPalette_Deliclaus,
        .iconSprite = gMonIcon_Deliclaus,
        .iconPalIndex = 1,
        FOOTPRINT(Delibird)
        OVERWORLD(
            sPicTable_Delibird,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Delibird,
            gShinyOverworldPalette_Delibird
        )
        .levelUpLearnset = sDelibirdLevelUpLearnset,
        .teachableLearnset = sDelibirdTeachableLearnset,
        .eggMoveLearnset = sDelibirdEggMoveLearnset,
    },

    [SPECIES_DIALKIA] =
    {
        .baseHP        = 70,
        .baseAttack    = 160,
        .baseDefense   = 160,
        .baseSpeed     = 80,
        .baseSpAttack  = 160,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_STEEL, TYPE_WATER),
        .catchRate = 3,
        .expYield = DIALGA_EXP_YIELD,
        .evYield_SpAttack = 3,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_TELEPATHY },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Dialkia"),
        .cryId = CRY_DIALGA,
        .natDexNum = NATIONAL_DEX_DIALGA,
        .categoryName = _("Temporal"),
        .height = 54,
        .weight = 6830,
        .description = COMPOUND_STRING(
            "A Pokémon spoken of in legend.\n"
            "It completely controls the flow of time.\n"
            "It uses its power to travel at will\n"
            "through the past and future."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 721,
        .trainerOffset = 19,
        .frontPic = gMonFrontPic_Dialkia,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_Dialkia,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_Dialkia,
        .shinyPalette = gMonShinyPalette_Dialkia,
        .iconSprite = gMonIcon_Dialga,
        .iconPalIndex = 2,
        FOOTPRINT(Dialga)
        OVERWORLD(
            sPicTable_Dialga,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dialga,
            gShinyOverworldPalette_Dialga
        )
        .isLegendary = TRUE,
        .isFrontierBanned = TRUE,
        .levelUpLearnset = sDialgaLevelUpLearnset,
        .teachableLearnset = sDialgaTeachableLearnset,
    },

    [SPECIES_INSECTEON] =
    {
        .baseHP        = 55,
        .baseAttack    = 110,
        .baseDefense   = 65,
        .baseSpeed     = 130,
        .baseSpAttack  = 100,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_BUG),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SWARM, ABILITY_SWARM, ABILITY_HYPER_CUTTER },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Isecteon"),
        .cryId = CRY_EEVEE,
        .natDexNum = NATIONAL_DEX_INSECTEON,
        .categoryName = _("Harvester"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "Insecteon evolved after consuming a\n"
            "special honey found in rare hives.\n"),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Insecteon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Insecteon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 10,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Insecteon,
        .shinyPalette = gMonShinyPalette_Insecteon,
        .iconSprite = gMonIcon_Insecteon,
        .iconPalIndex = 0,
        FOOTPRINT(Eevee)
        OVERWORLD(
            sPicTable_Eevee,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Eevee,
            gShinyOverworldPalette_Eevee
        )
        .levelUpLearnset = sButterfreeLevelUpLearnset,
        .teachableLearnset = sButterfreeTeachableLearnset,
    },

    [SPECIES_MYSTEON] =
    {
        .baseHP        = 105,
        .baseAttack    = 140,
        .baseDefense   = 70,
        .baseSpeed     = 95,
        .baseSpAttack  = 50,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_DRAGON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_INTIMIDATE, ABILITY_INTIMIDATE, ABILITY_MOLD_BREAKER },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Mysteon"),
        .cryId = CRY_EEVEE,
        .natDexNum = NATIONAL_DEX_MYSTEON,
        .categoryName = _("Mystic"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "Its presence is often seen as a mirage.\n"
            "Not many have been spotted in the wild."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Mysteon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Mysteon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 10,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Mysteon,
        .shinyPalette = gMonShinyPalette_Mysteon,
        .iconSprite = gMonIcon_Mysteon,
        .iconPalIndex = 1,
        FOOTPRINT(Eevee)
        OVERWORLD(
            sPicTable_Eevee,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Eevee,
            gShinyOverworldPalette_Eevee
        )
        .levelUpLearnset = sHaxorusLevelUpLearnset,
        .teachableLearnset = sHaxorusTeachableLearnset,
    },

    [SPECIES_BRAWLEON] =
    {
        .baseHP        = 70,
        .baseAttack    = 150,
        .baseDefense   = 70,
        .baseSpeed     = 150,
        .baseSpAttack  = 30,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_FIGHTING),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_SYNCHRONIZE, ABILITY_INNER_FOCUS },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Brawleon"),
        .cryId = CRY_UMBREON,
        .natDexNum = NATIONAL_DEX_BRAWLEON,
        .categoryName = _("Scrapper"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "It lives to fight. Strikes are hard,\n"
            "but it takes as much back."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Brawleon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Brawleon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Brawleon,
        .shinyPalette = gMonShinyPalette_Brawleon,
        .iconSprite = gMonIcon_Brawleon,
        .iconPalIndex = 0,
        FOOTPRINT(Umbreon)
        OVERWORLD(
            sPicTable_Umbreon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Umbreon,
            gShinyOverworldPalette_Umbreon
        )
        .levelUpLearnset = sHitmonleeLevelUpLearnset,
        .teachableLearnset = sHitmonleeTeachableLearnset,
    },

    [SPECIES_AEREON] =
    {
        .baseHP        = 100,
        .baseAttack    = 20,
        .baseDefense   = 70,
        .baseSpeed     = 150,
        .baseSpAttack  = 110,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_FLYING),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_SYNCHRONIZE, ABILITY_AERILATE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Aereon"),
        .cryId = CRY_GLACEON,
        .natDexNum = NATIONAL_DEX_AEREON,
        .categoryName = _("High Flyer"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "An Eevee dreamt of flying, and with\n"
            "its evolution abilities, it did."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Aereon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Aereon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Aereon,
        .shinyPalette = gMonShinyPalette_Aereon,
        .iconSprite = gMonIcon_Aereon,
        .iconPalIndex = 0,
        FOOTPRINT(Glaceon)
        OVERWORLD(
            sPicTable_Glaceon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Glaceon,
            gShinyOverworldPalette_Glaceon
        )
        .levelUpLearnset = sAltariaLevelUpLearnset,
        .teachableLearnset = sAltariaTeachableLearnset,
    },

    [SPECIES_WISPEON] =
    {
        .baseHP        = 70,
        .baseAttack    = 60,
        .baseDefense   = 70,
        .baseSpeed     = 100,
        .baseSpAttack  = 130,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_GHOST),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_SYNCHRONIZE, ABILITY_LEVITATE },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Wispeon"),
        .cryId = CRY_JOLTEON,
        .natDexNum = NATIONAL_DEX_WISPEON,
        .categoryName = _("Afterlife"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "Death didn't stop evolution. Eevee\n"
            "gained powers of the spirits upon\n"
            "passing."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Wispeon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .enemyMonElevation = 7,
        .backPic = gMonBackPic_Wispeon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 7,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Wispeon,
        .shinyPalette = gMonShinyPalette_Wispeon,
        .iconSprite = gMonIcon_Wispeon,
        .iconPalIndex = 0,
        FOOTPRINT(Haunter)
        OVERWORLD(
            sPicTable_Jolteon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Jolteon,
            gShinyOverworldPalette_Jolteon
        )
        .levelUpLearnset = sGengarLevelUpLearnset,
        .teachableLearnset = sGengarTeachableLearnset,
    },

    [SPECIES_BOULDEON] =
    {
        .baseHP        = 120,
        .baseAttack    = 70,
        .baseDefense   = 130,
        .baseSpeed     = 50,
        .baseSpAttack  = 30,
        .baseSpDefense = 130,
        .types = MON_TYPES(TYPE_GROUND),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_STURDY, ABILITY_STURDY, ABILITY_MOLD_BREAKER },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Bouldeon"),
        .cryId = CRY_FLAREON,
        .natDexNum = NATIONAL_DEX_BOULDEON,
        .categoryName = _("Hardhead"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "Capable of taking any strike, it has,\n"
            "incredible defensive capabilities."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Bouldeon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Bouldeon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Bouldeon,
        .shinyPalette = gMonShinyPalette_Bouldeon,
        .iconSprite = gMonIcon_Bouldeon,
        .iconPalIndex = 1,
        FOOTPRINT(Espeon)
        OVERWORLD(
            sPicTable_Espeon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Espeon,
            gShinyOverworldPalette_Espeon
        )
        .levelUpLearnset = sGolemLevelUpLearnset,
        .teachableLearnset = sGolemTeachableLearnset,
    },

    [SPECIES_VEXEON] =
    {
        .baseHP        = 80,
        .baseAttack    = 80,
        .baseDefense   = 80,
        .baseSpeed     = 80,
        .baseSpAttack  = 80,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_NORMAL),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_SYNCHRONIZE, ABILITY_INNER_FOCUS },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Vexeon"),
        .cryId = CRY_EEVEE,
        .natDexNum = NATIONAL_DEX_VEXEON,
        .categoryName = _("Evolution"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "On a path without the elements, it\n"
            "stays true to itself."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Vexeon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Vexeon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 7,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Vexeon,
        .shinyPalette = gMonShinyPalette_Vexeon,
        .iconSprite = gMonIcon_Vexeon,
        .iconPalIndex = 2,
        FOOTPRINT(Eevee)
        OVERWORLD(
            sPicTable_Eevee,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Eevee,
            gShinyOverworldPalette_Eevee
        )
        .levelUpLearnset = sEeveeLevelUpLearnset,
        .teachableLearnset = sEeveeTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 45, SPECIES_OMNEON}),
    },

    [SPECIES_OMNEON] =
    {
        .baseHP        = 110,
        .baseAttack    = 110,
        .baseDefense   = 110,
        .baseSpeed     = 110,
        .baseSpAttack  = 110,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_NORMAL),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_WONDER_GUARD, ABILITY_WONDER_GUARD, ABILITY_WONDER_GUARD },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Omneon"),
        .cryId = CRY_EEVEE,
        .natDexNum = NATIONAL_DEX_OMNEON,
        .categoryName = _("Omni"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "The final form of an Eevee. It uses the\n"
            "power of all elements to veil itself."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Omneon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Omneon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Omneon,
        .shinyPalette = gMonShinyPalette_Omneon,
        .iconSprite = gMonIcon_Omneon,
        .iconPalIndex = 0,
        FOOTPRINT(Eevee)
        OVERWORLD(
            sPicTable_Eevee,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Eevee,
            gShinyOverworldPalette_Eevee
        )
        .levelUpLearnset = sExploudLevelUpLearnset,
        .teachableLearnset = sExploudTeachableLearnset,
    },

    [SPECIES_TOXEON] =
    {
        .baseHP        = 70,
        .baseAttack    = 50,
        .baseDefense   = 120,
        .baseSpeed     = 50,
        .baseSpAttack  = 120,
        .baseSpDefense = 120,
        .types = MON_TYPES(TYPE_POISON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_POISON_POINT, ABILITY_ARENA_TRAP, ABILITY_INNER_FOCUS },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Toxeon"),
        .cryId = CRY_VAPOREON,
        .natDexNum = NATIONAL_DEX_TOXEON,
        .categoryName = _("Poison Hold"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "The sludge from its body traps and\n"
            "poisons its prey."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Toxeon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Toxeon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Toxeon,
        .shinyPalette = gMonShinyPalette_Toxeon,
        .iconSprite = gMonIcon_Toxeon,
        .iconPalIndex = 2,
        FOOTPRINT(Glaceon)
        OVERWORLD(
            sPicTable_Glaceon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Glaceon,
            gShinyOverworldPalette_Glaceon
        )
        .levelUpLearnset = sSwalotLevelUpLearnset,
        .teachableLearnset = sSwalotTeachableLearnset,
    },

    [SPECIES_ANCEON] =
    {
        .baseHP        = 100,
        .baseAttack    = 120,
        .baseDefense   = 120,
        .baseSpeed     = 50,
        .baseSpAttack  = 60,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_ROCK),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_ROCK_HEAD, ABILITY_ROCK_HEAD, ABILITY_STURDY },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Ancieon"),
        .cryId = CRY_VAPOREON,
        .natDexNum = NATIONAL_DEX_ANCEON,
        .categoryName = _("Old Rock"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "An old evolution of Eevee. It was\n"
            "around during the times of Pokemon\n"
            "like Aerodactyl."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Anceon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Anceon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Anceon,
        .shinyPalette = gMonShinyPalette_Anceon,
        .iconSprite = gMonIcon_Anceon,
        .iconPalIndex = 5,
        FOOTPRINT(Espeon)
        OVERWORLD(
            sPicTable_Eevee,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Eevee,
            gShinyOverworldPalette_Eevee
        )
        .levelUpLearnset = sOnixLevelUpLearnset,
        .teachableLearnset = sOnixTeachableLearnset,
    },

    [SPECIES_METALLEON] =
    {
        .baseHP        = 130,
        .baseAttack    = 40,
        .baseDefense   = 150,
        .baseSpeed     = 30,
        .baseSpAttack  = 30,
        .baseSpDefense = 150,
        .types = MON_TYPES(TYPE_STEEL),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 184 : 197,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 35,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_CLEAR_BODY, ABILITY_CLEAR_BODY, ABILITY_INNER_FOCUS },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Metalleon"),
        .cryId = CRY_UMBREON,
        .natDexNum = NATIONAL_DEX_METALLEON,
        .categoryName = _("Sturdy Body"),
        .height = 10,
        .weight = 270,
        .description = COMPOUND_STRING(
            "A body harder than steel. It has nearly\n"
            "no equal in defense."),
        .pokemonScale = 317,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Metalleon,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Metalleon,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_Metalleon,
        .shinyPalette = gMonShinyPalette_Metalleon,
        .iconSprite = gMonIcon_Metalleon,
        .iconPalIndex = 0,
        FOOTPRINT(Umbreon)
        OVERWORLD(
            sPicTable_Umbreon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Umbreon,
            gShinyOverworldPalette_Umbreon
        )
        .levelUpLearnset = sSteelixLevelUpLearnset,
        .teachableLearnset = sSteelixTeachableLearnset,
    },

    [SPECIES_DARG] =
    {
        .baseHP        = 45,
        .baseAttack    = 75,
        .baseDefense   = 60,
        .baseSpeed     = 50,
        .baseSpAttack  = 40,
        .baseSpDefense = 30,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_POISON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 60 : 89,
        .evYield_Attack = 1,
        .itemRare = ITEM_DRAGON_FANG,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_DRAGON),
        .abilities = { ABILITY_ROCK_HEAD, ABILITY_NONE, ABILITY_SHEER_FORCE },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Darg"),
        .cryId = CRY_BAGON,
        .natDexNum = NATIONAL_DEX_DARG,
        .categoryName = _("Poison"),
        .height = 6,
        .weight = 421,
        .description = COMPOUND_STRING(
            "Its large head often tricks predators\n"
            "into thinking it has physical prowess.\n"
            "It actually launches toxic attacks from\n"
            "a distance."),
        .pokemonScale = 448,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Darg,
        .frontPicSize = MON_COORDS_SIZE(32, 48),
        .frontPicYOffset = 10,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_Darg,
        .backPicSize = MON_COORDS_SIZE(48, 56),
        .backPicYOffset = 9,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_Darg,
        .shinyPalette = gMonShinyPalette_Darg,
        .iconSprite = gMonIcon_Darg,
        .iconPalIndex = 2,
        FOOTPRINT(Deino)
        OVERWORLD(
            sPicTable_Deino,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Deino,
            gShinyOverworldPalette_Deino
        )
        .levelUpLearnset = sBagonLevelUpLearnset,
        .teachableLearnset = sBagonTeachableLearnset,
        .eggMoveLearnset = sBagonEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 30, SPECIES_DARGO}),
    },

    [SPECIES_DARGO] =
    {
        .baseHP        = 65,
        .baseAttack    = 75,
        .baseDefense   = 80,
        .baseSpeed     = 50,
        .baseSpAttack  = 90,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_POISON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 147 : 144,
        .evYield_Defense = 2,
        .itemRare = ITEM_DRAGON_FANG,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_DRAGON),
        .abilities = { ABILITY_ROCK_HEAD, ABILITY_NONE, ABILITY_OVERCOAT },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Dargo"),
        .cryId = CRY_SHELGON,
        .natDexNum = NATIONAL_DEX_DARGO,
        .categoryName = _("Toxin"),
        .height = 11,
        .weight = 1105,
        .description = COMPOUND_STRING(
            "The poison it secretes is said to be\n"
            "able to take down a Dragonite."),
        .pokemonScale = 311,
        .pokemonOffset = 12,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Dargo,
        .frontPicSize = MON_COORDS_SIZE(48, 48),
        .frontPicYOffset = 5,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SLIDE,
        .backPic = gMonBackPic_Dargo,
        .backPicSize = MON_COORDS_SIZE(64, 40),
        .backPicYOffset = 8,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_Dargo,
        .shinyPalette = gMonShinyPalette_Dargo,
        .iconSprite = gMonIcon_Dargo,
        .iconPalIndex = 2,
        FOOTPRINT(Shelgon)
        OVERWORLD(
            sPicTable_Shelgon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Shelgon,
            gShinyOverworldPalette_Shelgon
        )
        .levelUpLearnset = sNaganadelLevelUpLearnset,
        .teachableLearnset = sNaganadelTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 50, SPECIES_DARGON}),
    },

    [SPECIES_DARGON] =
    {
        .baseHP        = 95,
        .baseAttack    = 90,
        .baseDefense   = 80,
        .baseSpeed     = 120,
        .baseSpAttack  = 135,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_POISON),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 300,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 270,
    #else
        .expYield = 218,
    #endif
        .evYield_Attack = 3,
        .itemRare = ITEM_DRAGON_FANG,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_DRAGON),
        .abilities = { ABILITY_INTIMIDATE, ABILITY_NONE, ABILITY_GRIM_NEIGH },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Dargon"),
        .cryId = CRY_SALAMENCE,
        .natDexNum = NATIONAL_DEX_DARGON,
        .categoryName = _("Venom Dragon"),
        .height = 15,
        .weight = 1026,
        .description = COMPOUND_STRING(
            "From a distant past, this beast boasts\n"
            "venom and poison. It uses these toxins\n"
            "for ultimate offense and defense."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Dargon,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 3,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .frontAnimDelay = 70,
        .backPic = gMonBackPic_Dargon,
        .backPicSize = MON_COORDS_SIZE(56, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SHAKE,
        .palette = gMonPalette_Dargon,
        .shinyPalette = gMonShinyPalette_Dargon,
        .iconSprite = gMonIcon_Dargon,
        .iconPalIndex = 2,
        FOOTPRINT(Salamence)
        OVERWORLD(
            sPicTable_Salamence,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Salamence,
            gShinyOverworldPalette_Salamence
        )
        .levelUpLearnset = sNaganadelLevelUpLearnset,
        .teachableLearnset = sNaganadelTeachableLearnset,
    },

    [SPECIES_FAKE_GROUDON] =
    {
        .baseHP        = 100,
        .baseAttack    = 150,
        .baseDefense   = 140,
        .baseSpeed     = 90,
        .baseSpAttack  = 100,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_GROUND, TYPE_FIRE),
        .catchRate = 3,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 335,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 302,
    #else
        .expYield = 218,
    #endif
        .evYield_Attack = 3,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_DROUGHT, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Groudon?"),
        .cryId = CRY_GROUDON,
        .natDexNum = NATIONAL_DEX_GROUDON,
        .categoryName = _("Continent"),
        .height = 35,
        .weight = 9500,
        .description = COMPOUND_STRING(
            "Groudon has appeared in mythology as the\n"
            "creator of the land. It sleeps in magma\n"
            "underground and is said to make volcanoes\n"
            "erupt on awakening."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 515,
        .trainerOffset = 14,
        .frontPic = gMonFrontPic_FakeGroudon,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_FakeGroudon,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 8,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_FakeGroudon,
        .shinyPalette = gMonShinyPalette_FakeGroudon,
        .iconSprite = gMonIcon_FakeGroudon,
        .iconPalIndex = 0,
        FOOTPRINT(Groudon)
        OVERWORLD(
            sPicTable_Groudon,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Groudon,
            gShinyOverworldPalette_Groudon
        )
        .isLegendary = TRUE,
        .isFrontierBanned = TRUE,
        .levelUpLearnset = sGroudonLevelUpLearnset,
        .teachableLearnset = sGroudonTeachableLearnset,
    },

    [SPECIES_ABRA_HOENNIAN] =
    {
        .baseHP        = 25,
        .baseAttack    = 105,
        .baseDefense   = 15,
        .baseSpeed     = 90,
        .baseSpAttack  = 25,
        .baseSpDefense = 55,
        .types = MON_TYPES(TYPE_FIGHTING),
        .catchRate = 200,
    #if P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 62,
    #elif P_UPDATED_EXP_YIELDS >= GEN_4
        .expYield = 75,
    #else
        .expYield = 73,
    #endif
        .evYield_SpAttack = 1,
        .itemRare = ITEM_BLACK_BELT,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_INNER_FOCUS, ABILITY_MAGIC_GUARD },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Abra"),
        .cryId = CRY_ABRA,
        .natDexNum = NATIONAL_DEX_ABRA_HOENNIAN,
        .categoryName = _("Punch"),
        .height = 9,
        .weight = 195,
        .description = COMPOUND_STRING(
            "Tired of dealing with matters of\n"
            "the mind, it instead took up\n"
            "martial arts."),
        .pokemonScale = 363,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_AbraH,
        .frontPicSize = MON_COORDS_SIZE(56, 48),
        .frontPicYOffset = 10,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_VIBRATE,
        .backPic = gMonBackPic_AbraH,
        .backPicSize = MON_COORDS_SIZE(56, 48),
        .backPicYOffset = 12,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_AbraH,
        .shinyPalette = gMonShinyPalette_AbraH,
        .iconSprite = gMonIcon_AbraH,
        .iconPalIndex = 2,
        FOOTPRINT(Abra)
        OVERWORLD(
            sPicTable_Abra,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Abra,
            gShinyOverworldPalette_Abra
        )
        .levelUpLearnset = sMankeyLevelUpLearnset,
        .teachableLearnset = sMankeyTeachableLearnset,
        .eggMoveLearnset = sMankeyEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 16, SPECIES_KADABRA_HOENNIAN}),
    },

    [SPECIES_KADABRA_HOENNIAN] =
    {
        .baseHP        = 40,
        .baseAttack    = 120,
        .baseDefense   = 30,
        .baseSpeed     = 105,
        .baseSpAttack  = 35,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_FIGHTING),
        .catchRate = 100,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 140 : 145,
        .evYield_SpAttack = 2,
        .itemRare = ITEM_BLACK_BELT,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_INNER_FOCUS, ABILITY_MAGIC_GUARD },
        .bodyColor = BODY_COLOR_GRAY,
        .noFlip = TRUE,
        .speciesName = _("Kadabra"),
        .cryId = CRY_KADABRA,
        .natDexNum = NATIONAL_DEX_KADABRA_HOENNIAN,
        .categoryName = _("Punch"),
        .height = 13,
        .weight = 565,
        .description = COMPOUND_STRING(
            "A martial artist learned every kind\n"
            "of stance he could. One day, he\n"
            "turned into a Kadabra."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_KadabraH,
        .frontPicFemale = gMonFrontPic_KadabraH,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 5,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_KadabraH,
        .backPicFemale = gMonBackPic_KadabraH,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 8,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_KadabraH,
        .shinyPalette = gMonShinyPalette_KadabraH,
        .iconSprite = gMonIcon_KadabraH,
        .iconPalIndex = 2,
        FOOTPRINT(Kadabra)
        OVERWORLD(
            sPicTable_Kadabra,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Kadabra,
            gShinyOverworldPalette_Kadabra
        )
        .levelUpLearnset = sPrimeapeLevelUpLearnset,
        .teachableLearnset = sPrimeapeTeachableLearnset,
        .evolutions = EVOLUTION({EVO_ITEM, ITEM_LINKING_CORD, SPECIES_ALAKAZAM_HOENNIAN}),
    },

    [SPECIES_ALAKAZAM_HOENNIAN] =
    {
        .baseHP        = 55,
        .baseAttack    = 135,
        .baseDefense   = 45,
        .baseSpeed     = 120,
        .baseSpAttack  = 50,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_FIGHTING),
        .catchRate = 50,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 250,
    #elif P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 225,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 221,
    #else
        .expYield = 186,
    #endif
        .evYield_SpAttack = 3,
        .itemRare = ITEM_EXPERT_BELT,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_SYNCHRONIZE, ABILITY_INNER_FOCUS, ABILITY_MAGIC_GUARD },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Alakazam"),
        .cryId = CRY_ALAKAZAM,
        .natDexNum = NATIONAL_DEX_ALAKAZAM_HOENNIAN,
        .categoryName = _("Punch"),
        .height = 15,
        .weight = 480,
        .description = COMPOUND_STRING(
            "Unlike its Kantonian counterpart, its\n"
            "psychic powers are weak. It is more\n"
            "effective in hand to hand."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_AlakazamH,
        .frontPicFemale = gMonFrontPic_AlakazamH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_AlakazamH,
        .backPicFemale = gMonBackPic_AlakazamH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_GROW_STUTTER,
        .palette = gMonPalette_AlakazamH,
        .shinyPalette = gMonShinyPalette_AlakazamH,
        .iconSprite = gMonIcon_AlakazamH,
        .iconPalIndex = 2,
        FOOTPRINT(Alakazam)
        OVERWORLD(
            sPicTable_Alakazam,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Alakazam,
            gShinyOverworldPalette_Alakazam
        )
        .levelUpLearnset = sMachampLevelUpLearnset,
        .teachableLearnset = sMachampTeachableLearnset,
    },

    [SPECIES_BULBASAUR_HOENNIAN] =
    {
        .baseHP        = 45,
        .baseAttack    = 49,
        .baseDefense   = 49,
        .baseSpeed     = 45,
        .baseSpAttack  = 65,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_ROCK, TYPE_FIRE),
        .catchRate = 45,
        .expYield = 64,
        .evYield_SpAttack = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_FLAME_BODY, ABILITY_NONE, ABILITY_MAGMA_ARMOR },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Bulbasaur"),
        .cryId = CRY_BULBASAUR,
        .natDexNum = NATIONAL_DEX_BULBASAUR_HOENNIAN,
        .categoryName = _("Blaze"),
        .height = 7,
        .weight = 69,
        .description = COMPOUND_STRING(
            "Bulbasaur subjected to incredibly harsh\n"
            "sunlight evolved to use fire energy."),
        .pokemonScale = 356,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_BulbasaurH,
        .frontPicSize = MON_COORDS_SIZE(40, 40),
        .frontPicYOffset = 10,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_JUMPS_H_JUMPS,
        .backPic = gMonBackPic_BulbasaurH,
        .backPicSize = MON_COORDS_SIZE(56, 40),
        .backPicYOffset = 8,
        .backAnimId = BACK_ANIM_DIP_RIGHT_SIDE,
        .palette = gMonPalette_BulbasaurH,
        .shinyPalette = gMonShinyPalette_BulbasaurH,
        .iconSprite = gMonIcon_BulbasaurH,
        .iconPalIndex = 0,
        FOOTPRINT(Bulbasaur)
        OVERWORLD(
            sPicTable_Bulbasaur,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Bulbasaur,
            gShinyOverworldPalette_Bulbasaur
        )
        .levelUpLearnset = sSlugmaLevelUpLearnset,
        .teachableLearnset = sSlugmaTeachableLearnset,
        .eggMoveLearnset = sSlugmaEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 16, SPECIES_IVYSAUR_HOENNIAN}),
    },

    [SPECIES_IVYSAUR_HOENNIAN] =
    {
        .baseHP        = 60,
        .baseAttack    = 62,
        .baseDefense   = 63,
        .baseSpeed     = 60,
        .baseSpAttack  = 80,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_FIRE, TYPE_ROCK),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 142 : 141,
        .evYield_SpAttack = 1,
        .evYield_SpDefense = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_FLAME_BODY, ABILITY_NONE, ABILITY_MAGMA_ARMOR },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Ivysaur"),
        .cryId = CRY_IVYSAUR,
        .natDexNum = NATIONAL_DEX_IVYSAUR_HOENNIAN,
        .categoryName = _("Blaze"),
        .height = 10,
        .weight = 130,
        .description = COMPOUND_STRING(
            "To support its bulb, Ivysaur's legs\n"
            "grow sturdy. If it spends more time lying in\n"
            "the sunlight, the bud will soon bloom into\n"
            "a mini volcano."),
        .pokemonScale = 335,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_IvysaurH,
        .frontPicSize = MON_COORDS_SIZE(56, 48),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_IvysaurH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 8,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_IvysaurH,
        .shinyPalette = gMonShinyPalette_IvysaurH,
        .iconSprite = gMonIcon_IvysaurH,
        .iconPalIndex = 0,
        FOOTPRINT(Ivysaur)
        OVERWORLD(
            sPicTable_Ivysaur,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Ivysaur,
            gShinyOverworldPalette_Ivysaur
        )
        .levelUpLearnset = sMagcargoLevelUpLearnset,
        .teachableLearnset = sMagcargoTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 32, SPECIES_VENUSAUR_HOENNIAN}),
    },

    [SPECIES_VENUSAUR_HOENNIAN] =
    {
        .baseHP        = 100,
        .baseAttack    = 120,
        .baseDefense   = 100,
        .baseSpeed     = 80,
        .baseSpAttack  = 90,
        .baseSpDefense = 130,
        .types = MON_TYPES(TYPE_FIRE, TYPE_ROCK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 263,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 236,
    #else
        .expYield = 208,
    #endif
        .evYield_SpAttack = 2,
        .evYield_SpDefense = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_FLAME_BODY, ABILITY_NONE, ABILITY_MAGMA_ARMOR },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Venusaur"),
        .cryId = CRY_VENUSAUR,
        .natDexNum = NATIONAL_DEX_VENUSAUR_HOENNIAN,
        .categoryName = _("Blaze"),
        .height = 20,
        .weight = 1000,
        .description = COMPOUND_STRING(
            "Venusaur's volcano is said to leak\n"
            "magma that can scorch even the\n"
            "sturdiest metal."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 388,
        .trainerOffset = 6,
        .frontPic = gMonFrontPic_VenusaurH,
        .frontPicFemale = gMonFrontPic_VenusaurH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 3,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_ROTATE_UP_SLAM_DOWN,
        .backPic = gMonBackPic_VenusaurH,
        .backPicFemale = gMonBackPic_VenusaurH,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 10,
        .backAnimId = BACK_ANIM_H_SHAKE,
        .palette = gMonPalette_VenusaurH,
        .shinyPalette = gMonShinyPalette_VenusaurH,
        .iconSprite = gMonIcon_VenusaurH,
        .iconPalIndex = 0,
        FOOTPRINT(Venusaur)
        OVERWORLD(
            sPicTable_Venusaur,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Venusaur,
            gShinyOverworldPalette_Venusaur
        )
        .levelUpLearnset = sCameruptLevelUpLearnset,
        .teachableLearnset = sCameruptTeachableLearnset,
    },

    [SPECIES_MUDKIP_HOENNIAN] =
    {
        .baseHP        = 50,
        .baseAttack    = 70,
        .baseDefense   = 50,
        .baseSpeed     = 40,
        .baseSpAttack  = 50,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_WATER, TYPE_GHOST),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 62 : 65,
        .evYield_Attack = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_WATER_1),
        .abilities = { ABILITY_TORRENT, ABILITY_NONE, ABILITY_DAMP },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Mudkip"),
        .cryId = CRY_MUDKIP,
        .natDexNum = NATIONAL_DEX_MUDKIP_HOENNIAN,
        .categoryName = _("Ghost Fish"),
        .height = 4,
        .weight = 76,
        .description = COMPOUND_STRING(
            "On land, it can powerfully lift large\n"
            "boulders by planting its four feet and\n"
            "heaving. It sleeps by burying itself in soil\n"
            "at the water's edge."),
        .pokemonScale = 535,
        .pokemonOffset = 20,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_MudkipH,
        .frontPicSize = MON_COORDS_SIZE(40, 40),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_CIRCULAR_STRETCH_TWICE,
        .backPic = gMonBackPic_MudkipH,
        .backPicSize = MON_COORDS_SIZE(48, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_MudkipH,
        .shinyPalette = gMonShinyPalette_MudkipH,
        .iconSprite = gMonIcon_MudkipH,
        .iconPalIndex = 0,
        FOOTPRINT(Mudkip)
        OVERWORLD(
            sPicTable_Mudkip,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Mudkip,
            gShinyOverworldPalette_Mudkip
        )
        .levelUpLearnset = sFrillishLevelUpLearnset,
        .teachableLearnset = sFrillishTeachableLearnset,
        .eggMoveLearnset = sMudkipEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 16, SPECIES_MARSHTOMP_HOENNIAN}),
    },

    [SPECIES_MARSHTOMP_HOENNIAN] =
    {
        .baseHP        = 70,
        .baseAttack    = 85,
        .baseDefense   = 70,
        .baseSpeed     = 50,
        .baseSpAttack  = 60,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_WATER, TYPE_GHOST),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 142 : 143,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_WATER_1),
        .abilities = { ABILITY_TORRENT, ABILITY_NONE, ABILITY_DAMP },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Marshtomp"),
        .cryId = CRY_MARSHTOMP,
        .natDexNum = NATIONAL_DEX_MARSHTOMP_HOENNIAN,
        .categoryName = _("Ghost Fish"),
        .height = 7,
        .weight = 280,
        .description = COMPOUND_STRING(
            "Its toughened hind legs enable it to stand\n"
            "upright. Because it weakens if its skin\n"
            "dries out, it replenishes fluids by playing\n"
            "in mud."),
        .pokemonScale = 340,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_MarshtompH,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_MarshtompH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_CONCAVE_ARC_SMALL,
        .palette = gMonPalette_MarshtompH,
        .shinyPalette = gMonShinyPalette_MarshtompH,
        .iconSprite = gMonIcon_MarshtompH,
        .iconPalIndex = 0,
        FOOTPRINT(Marshtomp)
        OVERWORLD(
            sPicTable_Marshtomp,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Marshtomp,
            gShinyOverworldPalette_Marshtomp
        )
        .levelUpLearnset = sJellicentLevelUpLearnset,
        .teachableLearnset = sJellicentTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 36, SPECIES_SWAMPERT_HOENNIAN}),
    },

    [SPECIES_SWAMPERT_HOENNIAN] =
    {
        .baseHP        = 100,
        .baseAttack    = 110,
        .baseDefense   = 95,
        .baseSpeed     = 60,
        .baseSpAttack  = 125,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_WATER, TYPE_GHOST),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 268,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 241,
    #else
        .expYield = 210,
    #endif
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_WATER_1),
        .abilities = { ABILITY_TORRENT, ABILITY_NONE, ABILITY_DAMP },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Swampert"),
        .cryId = CRY_SWAMPERT,
        .natDexNum = NATIONAL_DEX_SWAMPERT_HOENNIAN,
        .categoryName = _("Mud Fish"),
        .height = 15,
        .weight = 819,
        .description = COMPOUND_STRING(
            "If it senses the approach of a storm and\n"
            "a tidal wave, it protects its seaside nest\n"
            "by piling up boulders. It swims as fast as\n"
            "a jet ski."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_SwampertH,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_SwampertH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_SwampertH,
        .shinyPalette = gMonShinyPalette_SwampertH,
        .iconSprite = gMonIcon_SwampertH,
        .iconPalIndex = 0,
        FOOTPRINT(Swampert)
        OVERWORLD(
            sPicTable_Swampert,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Swampert,
            gShinyOverworldPalette_Swampert
        )
        .levelUpLearnset = sJellicentLevelUpLearnset,
        .teachableLearnset = sJellicentTeachableLearnset,
    },

    [SPECIES_PIDGEY_HOENNIAN] =
    {
        .baseHP        = 40,
        .baseAttack    = 45,
        .baseDefense   = 40,
        .baseSpeed     = 56,
        .baseSpAttack  = 35,
        .baseSpDefense = 35,
        .types = MON_TYPES(TYPE_GHOST, TYPE_FLYING),
        .catchRate = 255,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 50 : 55,
        .evYield_Speed = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FLYING),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_KEEN_EYE, ABILITY_TANGLED_FEET, ABILITY_BIG_PECKS },
    #else
        .abilities = { ABILITY_KEEN_EYE, ABILITY_NONE, ABILITY_BIG_PECKS },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Pidgey"),
        .cryId = CRY_PIDGEY,
        .natDexNum = NATIONAL_DEX_PIDGEY_HOENNIAN,
        .categoryName = _("Wraith Bird"),
        .height = 3,
        .weight = 18,
        .description = COMPOUND_STRING(
            "It has an extremely sharp sense of\n"
            "direction. It can unerringly return home to\n"
            "its nest, however far it may be removed\n"
            "from its familiar surroundings."),
        .pokemonScale = 508,
        .pokemonOffset = -3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_PidgeyH,
        .frontPicSize = MON_COORDS_SIZE(40, 40),
        .frontPicYOffset = 12,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_PidgeyH,
        .backPicSize = MON_COORDS_SIZE(56, 48),
        .backPicYOffset = 11,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_PidgeyH,
        .shinyPalette = gMonShinyPalette_PidgeyH,
        .iconSprite = gMonIcon_PidgeyH,
        .iconPalIndex = 0,
        FOOTPRINT(Pidgey)
        OVERWORLD(
            sPicTable_Pidgey,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Pidgey,
            gShinyOverworldPalette_Pidgey
        )
        .levelUpLearnset = sDrifloonLevelUpLearnset,
        .teachableLearnset = sDrifloonTeachableLearnset,
        .eggMoveLearnset = sPidgeyEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 18, SPECIES_PIDGEOTTO_HOENNIAN}),
    },

    [SPECIES_PIDGEOTTO_HOENNIAN] =
    {
        .baseHP        = 63,
        .baseAttack    = 60,
        .baseDefense   = 55,
        .baseSpeed     = 71,
        .baseSpAttack  = 50,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_GHOST, TYPE_FLYING),
        .catchRate = 120,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 122 : 113,
        .evYield_Speed = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FLYING),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_KEEN_EYE, ABILITY_TANGLED_FEET, ABILITY_BIG_PECKS },
    #else
        .abilities = { ABILITY_KEEN_EYE, ABILITY_NONE, ABILITY_BIG_PECKS },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Pidgeotto"),
        .cryId = CRY_PIDGEOTTO,
        .natDexNum = NATIONAL_DEX_PIDGEOTTO_HOENNIAN,
        .categoryName = _("Wraith Bird"),
        .height = 11,
        .weight = 300,
        .description = COMPOUND_STRING(
            "This Pokémon flies around, patrolling its\n"
            "large territory. If its living space is\n"
            "violated, it shows no mercy in thoroughly\n"
            "punishing the foe with its sharp claws."),
        .pokemonScale = 331,
        .pokemonOffset = 10,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_PidgeottoH,
        .frontPicSize = MON_COORDS_SIZE(56, 56),
        .frontPicYOffset = 6,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .frontAnimDelay = 25,
        .backPic = gMonBackPic_PidgeottoH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_PidgeottoH,
        .shinyPalette = gMonShinyPalette_PidgeottoH,
        .iconSprite = gMonIcon_PidgeottoH,
        .iconPalIndex = 0,
        FOOTPRINT(Pidgeotto)
        OVERWORLD(
            sPicTable_Pidgeotto,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Pidgeotto,
            gShinyOverworldPalette_Pidgeotto
        )
        .levelUpLearnset = sDrifblimLevelUpLearnset,
        .teachableLearnset = sDrifblimTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 36, SPECIES_PIDGEOT_HOENNIAN}),
    },

    [SPECIES_PIDGEOT_HOENNIAN] =
    {
        .baseHP        = 83,
        .baseAttack    = 70,
        .baseDefense   = 75,
        .baseSpeed     = 120,
        .baseSpAttack  = 130,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_GHOST, TYPE_FLYING),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 240,
    #elif P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 216,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 211,
    #else
        .expYield = 172,
    #endif
        .evYield_Speed = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FLYING),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_KEEN_EYE, ABILITY_TANGLED_FEET, ABILITY_BIG_PECKS },
    #else
        .abilities = { ABILITY_KEEN_EYE, ABILITY_NONE, ABILITY_BIG_PECKS },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Pidgeot"),
        .cryId = CRY_PIDGEOT,
        .natDexNum = NATIONAL_DEX_PIDGEOT,
        .categoryName = _("Bird"),
        .height = 15,
        .weight = 395,
        .description = COMPOUND_STRING(
            "This Pokémon has gorgeous, glossy\n"
            "feathers. Many Trainers are so captivated\n"
            "by the beautiful feathers on its head that\n"
            "they choose Pidgeot as their Pokémon."),
        .pokemonScale = 269,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_PidgeotH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .enemyMonElevation = 5,
        .backPic = gMonBackPic_PidgeotH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_PidgeotH,
        .shinyPalette = gMonShinyPalette_PidgeotH,
        .iconSprite = gMonIcon_PidgeotH,
        .iconPalIndex = 0,
        FOOTPRINT(Pidgeot)
        OVERWORLD(
            sPicTable_Pidgeot,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Pidgeot,
            gShinyOverworldPalette_Pidgeot
        )
        .levelUpLearnset = sDrifblimLevelUpLearnset,
        .teachableLearnset = sDrifblimTeachableLearnset,
    },

    [SPECIES_GOROCHU] =
    {
        .baseHP        = 60,
        .baseAttack    = 120,
        .baseDefense   = 55,
        .baseSpeed     = 140,
        .baseSpAttack  = 120,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_ELECTRIC),
        .catchRate = 75,
        .expYield = RAICHU_EXP_YIELD,
        .evYield_Speed = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 10,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD, EGG_GROUP_FAIRY),
        .abilities = { ABILITY_STATIC, ABILITY_NONE, ABILITY_LIGHTNING_ROD },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Gorochu"),
        .cryId = CRY_RAICHU,
        .natDexNum = NATIONAL_DEX_GOROCHU,
        .categoryName = _("Mouse"),
        .height = 8,
        .weight = 300,
        .description = COMPOUND_STRING(
            "With too much electricity stored,\n"
            "it has reached a breaking point."),
        .pokemonScale = 426,
        .pokemonOffset = 13,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Gorochu,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 3,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_Gorochu,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_SHAKE_FLASH_YELLOW,
        .palette = gMonPalette_Gorochu,
        .shinyPalette = gMonShinyPalette_Gorochu,
        .iconSprite = gMonIcon_Gorochu,
        .iconPalIndex = 0,
        FOOTPRINT(Raichu)
        OVERWORLD(
            sPicTable_Raichu,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Raichu,
            gShinyOverworldPalette_Raichu
        )
        .levelUpLearnset = sRaichuLevelUpLearnset,
        .teachableLearnset = sRaichuTeachableLearnset,
    },

    [SPECIES_HELIX] =
    {
        .baseHP        = 70,
        .baseAttack    = 60,
        .baseDefense   = 125,
        .baseSpeed     = 55,
        .baseSpAttack  = 115,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_ROCK, TYPE_WATER),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 173 : 199,
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 30,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_WATER_3),
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_SHELL_ARMOR, ABILITY_WEAK_ARMOR },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Helix"),
        .cryId = CRY_OMASTAR,
        .natDexNum = NATIONAL_DEX_HELIX,
        .categoryName = _("Spiral"),
        .height = 10,
        .weight = 350,
        .description = COMPOUND_STRING(
            "Fuck you."),
        .pokemonScale = 307,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Helix,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_Helix,
        .backPicSize = MON_COORDS_SIZE(56, 56),
        .backPicYOffset = 7,
        .backAnimId = BACK_ANIM_DIP_RIGHT_SIDE,
        .palette = gMonPalette_Helix,
        .shinyPalette = gMonShinyPalette_Helix,
        .iconSprite = gMonIcon_Helix,
        .iconPalIndex = 0,
        FOOTPRINT(Omastar)
        OVERWORLD(
            sPicTable_Omastar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Omastar,
            gShinyOverworldPalette_Omastar
        )
        .levelUpLearnset = sOmastarLevelUpLearnset,
        .teachableLearnset = sOmastarTeachableLearnset,
    },

    [SPECIES_NUMEL_HOENNIAN] =
    {
        .baseHP        = 60,
        .baseAttack    = 60,
        .baseDefense   = 40,
        .baseSpeed     = 35,
        .baseSpAttack  = 65,
        .baseSpDefense = 45,
        .types = MON_TYPES(TYPE_ICE, TYPE_GROUND),
        .catchRate = 255,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 61 : 88,
        .evYield_SpAttack = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_OBLIVIOUS, ABILITY_SIMPLE, ABILITY_OWN_TEMPO },
    #else
        .abilities = { ABILITY_OBLIVIOUS, ABILITY_NONE, ABILITY_OWN_TEMPO },
    #endif
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Numel"),
        .cryId = CRY_NUMEL,
        .natDexNum = NATIONAL_DEX_NUMEL_HOENNIAN,
        .categoryName = _("Numb"),
        .height = 7,
        .weight = 240,
        .description = COMPOUND_STRING(
            "A colder variant of the normally\n"
            "boiling numel. The ice on its back\n"
            "was often used for food storage."),
        .pokemonScale = 342,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_NumelH,
        .frontPicFemale = gMonFrontPic_NumelH,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicSizeFemale = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 3,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SLIDE,
        .backPic = gMonBackPic_NumelH,
        .backPicFemale = gMonBackPic_NumelH,
        .backPicSize = MON_COORDS_SIZE(56, 56),
        .backPicSizeFemale = MON_COORDS_SIZE(56, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE_LOW,
        .palette = gMonPalette_NumelH,
        .shinyPalette = gMonShinyPalette_NumelH,
        .iconSprite = gMonIcon_NumelH,
        .iconPalIndex = 4,
        FOOTPRINT(Numel)
        OVERWORLD(
            sPicTable_Numel,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Numel,
            gShinyOverworldPalette_Numel
        )
        .levelUpLearnset = sPiloswineLevelUpLearnset,
        .teachableLearnset = sPiloswineTeachableLearnset,
        .eggMoveLearnset = sNumelEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 33, SPECIES_CAMERUPT_HOENNIAN}),
    },

    [SPECIES_CAMERUPT_HOENNIAN] =
    {
        .baseHP        = 80,
        .baseAttack    = 140,
        .baseDefense   = 90,
        .baseSpeed     = 40,
        .baseSpAttack  = 105,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_ICE, TYPE_GROUND),
        .catchRate = 150,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 161 : 175,
        .evYield_Attack = 1,
        .evYield_SpAttack = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_MAGMA_ARMOR, ABILITY_SOLID_ROCK, ABILITY_ANGER_POINT },
    #else
        .abilities = { ABILITY_MAGMA_ARMOR, ABILITY_NONE, ABILITY_ANGER_POINT },
    #endif
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Camerupt"),
        .cryId = CRY_CAMERUPT,
        .natDexNum = NATIONAL_DEX_CAMERUPT_HOENNIAN,
        .categoryName = _("Avalanche"),
        .height = 19,
        .weight = 2200,
        .description = COMPOUND_STRING(
            "A Pokémon that lives at the peak of\n"
            "a mountain. Every 10 years, the mountain\n"
            "on its back blows out snow. Research is\n"
            "under way on the cause of this."),
        .pokemonScale = 256,
        .pokemonOffset = 7,
        .trainerScale = 345,
        .trainerOffset = 6,
        .frontPic = gMonFrontPic_CameruptH,
        .frontPicFemale = gMonFrontPic_CameruptH,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 6,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_CameruptH,
        .backPicFemale = gMonBackPic_CameruptH,
        .backPicSize = MON_COORDS_SIZE(64, 40),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 40),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_CameruptH,
        .shinyPalette = gMonShinyPalette_CameruptH,
        .iconSprite = gMonIcon_CameruptH,
        .iconPalIndex = 2,
        FOOTPRINT(Camerupt)
        OVERWORLD(
            sPicTable_Camerupt,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Camerupt,
            gShinyOverworldPalette_Camerupt
        )
        .levelUpLearnset = sMamoswineLevelUpLearnset,
        .teachableLearnset = sMamoswineTeachableLearnset,
    },

    [SPECIES_PONYTA_HOENNIAN] =
    {
        .baseHP        = 50,
        .baseAttack    = 85,
        .baseDefense   = 55,
        .baseSpeed     = 90,
        .baseSpAttack  = 65,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_ICE),
        .catchRate = 190,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 82 : 152,
        .evYield_Speed = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_RUN_AWAY, ABILITY_FLASH_FIRE, ABILITY_SNOW_CLOAK },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Ponyta"),
        .cryId = CRY_PONYTA,
        .natDexNum = NATIONAL_DEX_PONYTA_HOENNIAN,
        .categoryName = _("Ice Horse"),
        .height = 10,
        .weight = 300,
        .description = COMPOUND_STRING(
            "A Ponyta is very weak at birth. It can\n"
            "barely stand up. Its legs become stronger\n"
            "as it stumbles and falls while trying to\n"
            "keep up with its parent."),
        .pokemonScale = 283,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_PonytaH,
        .frontPicSize = MON_COORDS_SIZE(56, 56),
        .frontPicYOffset = 6,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .frontAnimDelay = 10,
        .backPic = gMonBackPic_PonytaH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 4,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_PonytaH,
        .shinyPalette = gMonShinyPalette_PonytaH,
        .iconSprite = gMonIcon_PonytaH,
        .iconPalIndex = 2,
        FOOTPRINT(Ponyta)
        OVERWORLD(
            sPicTable_Ponyta,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Ponyta,
            gShinyOverworldPalette_Ponyta
        )
        .levelUpLearnset = sCubchooLevelUpLearnset,
        .teachableLearnset = sCubchooTeachableLearnset,
        .eggMoveLearnset = sCubchooEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 40, SPECIES_RAPIDASH_HOENNIAN}),
    },

    [SPECIES_RAPIDASH_HOENNIAN] =
    {
        .baseHP        = 65,
        .baseAttack    = 125,
        .baseDefense   = 70,
        .baseSpeed     = 125,
        .baseSpAttack  = 80,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_ICE),
        .catchRate = 60,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 175 : 192,
        .evYield_Speed = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_RUN_AWAY, ABILITY_FLASH_FIRE, ABILITY_SNOW_CLOAK },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Rapidash"),
        .cryId = CRY_RAPIDASH,
        .natDexNum = NATIONAL_DEX_RAPIDASH_HOENNIAN,
        .categoryName = _("Ice Horse"),
        .height = 17,
        .weight = 950,
        .description = COMPOUND_STRING(
            "It usually canters casually in the fields\n"
            "and plains. But once a Rapidash turns\n"
            "serious, its icy manes freeze\n"
            "as it gallops its way up to 150 mph."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 289,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_RapidashH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_RapidashH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_RapidashH,
        .shinyPalette = gMonShinyPalette_RapidashH,
        .iconSprite = gMonIcon_RapidashH,
        .iconPalIndex = 2,
        FOOTPRINT(Rapidash)
        OVERWORLD(
            sPicTable_Rapidash,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Rapidash,
            gShinyOverworldPalette_Rapidash
        )
        .levelUpLearnset = sBearticLevelUpLearnset,
        .teachableLearnset = sBearticTeachableLearnset,
    },

    [SPECIES_KINGKARP] =
    {
        .baseHP        = 20,
        .baseAttack    = 10,
        .baseDefense   = 55,
        .baseSpeed     = 80,
        .baseSpAttack  = 15,
        .baseSpDefense = 20,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 255,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 40 : 20,
        .evYield_Speed = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 5,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_2, EGG_GROUP_DRAGON),
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_NONE, ABILITY_RATTLED },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Kingkarp"),
        .cryId = CRY_MAGIKARP,
        .natDexNum = NATIONAL_DEX_KINGKARP,
        .categoryName = _("King Fish"),
        .height = 9,
        .weight = 100,
        .description = COMPOUND_STRING(
            "A Magikarp that failed to climb the\n"
            "waterfall. Perhaps its time will come."),
        .pokemonScale = 310,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Kingkarp,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_BOUNCE_ROTATE_TO_SIDES,
        .backPic = gMonBackPic_Kingkarp,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_CONCAVE_ARC_LARGE,
        .palette = gMonPalette_Kingkarp,
        .shinyPalette = gMonShinyPalette_Kingkarp,
        .iconSprite = gMonIcon_Kingkarp,
        .iconPalIndex = 0,
        FOOTPRINT(Magikarp)
        OVERWORLD(
            sPicTable_Magikarp,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Magikarp,
            gShinyOverworldPalette_Magikarp
        )
        .tmIlliterate = TRUE,
        .levelUpLearnset = sMagikarpLevelUpLearnset,
        .teachableLearnset = sMagikarpTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 30, SPECIES_GYARADOS_HOENNIAN}),
    },

    [SPECIES_GYARADOS_HOENNIAN] =
    {
        .baseHP        = 105,
        .baseAttack    = 145,
        .baseDefense   = 85,
        .baseSpeed     = 100,
        .baseSpAttack  = 60,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_WATER, TYPE_DRAGON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 189 : 214,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 5,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_2, EGG_GROUP_DRAGON),
        .abilities = { ABILITY_INTIMIDATE, ABILITY_NONE, ABILITY_MOXIE },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Gyarados"),
        .cryId = CRY_GYARADOS,
        .natDexNum = NATIONAL_DEX_GYARADOS_HOENNIAN,
        .categoryName = _("Atrocious"),
        .height = 65,
        .weight = 2350,
        .description = COMPOUND_STRING(
            "It is an extremely vicious and violent\n"
            "Pokémon. When humans begin to fight,\n"
            "it will appear and burn everything to the\n"
            "ground with intensely hot flames."),
        .pokemonScale = 256,
        .pokemonOffset = 6,
        .trainerScale = 481,
        .trainerOffset = 13,
        .frontPic = gMonFrontPic_GyaradosH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_BOUNCE_ROTATE_TO_SIDES_SMALL,
        .backPic = gMonBackPic_GyaradosH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_GyaradosH,
        .shinyPalette = gMonShinyPalette_GyaradosH,
        .iconSprite = gMonIcon_GyaradosH,
        .iconPalIndex = 0,
        FOOTPRINT(Gyarados)
        OVERWORLD(
            sPicTable_Gyarados,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Gyarados,
            gShinyOverworldPalette_Gyarados
        )
        .levelUpLearnset = sGyaradosLevelUpLearnset,
        .teachableLearnset = sGyaradosTeachableLearnset,
    },

    [SPECIES_MILOTIC_HOENNIAN] =
    {
        .baseHP        = 95,
        .baseAttack    = 60,
        .baseDefense   = 79,
        .baseSpeed     = 81,
        .baseSpAttack  = 100,
        .baseSpDefense = 125,
        .types = MON_TYPES(TYPE_GROUND, TYPE_FAIRY),
        .catchRate = 60,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 189 : 213,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_ERRATIC,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_MARVEL_SCALE, ABILITY_COMPETITIVE, ABILITY_CUTE_CHARM },
    #else
        .abilities = { ABILITY_MARVEL_SCALE, ABILITY_NONE, ABILITY_CUTE_CHARM },
    #endif
        .bodyColor = BODY_COLOR_PINK,
        .speciesName = _("Milotic"),
        .cryId = CRY_MILOTIC,
        .natDexNum = NATIONAL_DEX_MILOTIC_HOENNIAN,
        .categoryName = _("Tender"),
        .height = 62,
        .weight = 1620,
        .description = COMPOUND_STRING(
            "Considered ugly compared to its\n"
            "beautiful cousin, this Pokemon lives\n"
            "in the mud."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 360,
        .trainerOffset = 7,
        .frontPic = gMonFrontPic_MiloticH,
        .frontPicFemale = gMonFrontPic_MiloticH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_CIRCULAR_STRETCH_TWICE,
        .frontAnimDelay = 45,
        .backPic = gMonBackPic_MiloticH,
        .backPicFemale = gMonBackPic_MiloticH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicSizeFemale = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_MiloticH,
        .shinyPalette = gMonShinyPalette_MiloticH,
        .iconSprite = gMonIcon_MiloticH,
        .iconPalIndex = 2,
        FOOTPRINT(Milotic)
        OVERWORLD(
            sPicTable_Milotic,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Milotic,
            gShinyOverworldPalette_Milotic
        )
        .levelUpLearnset = sMiloticHLevelUpLearnset,
        .teachableLearnset = sMiloticTeachableLearnset,
    },

    [SPECIES_POLICROAK] =
    {
        .baseHP        = 100,
        .baseAttack    = 75,
        .baseDefense   = 75,
        .baseSpeed     = 70,
        .baseSpAttack  = 120,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_WATER, TYPE_POISON),
        .catchRate = 45,
        .expYield = 250,
        .evYield_SpDefense = 3,
        .itemRare = ITEM_KINGS_ROCK,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1),
        .abilities = { ABILITY_WATER_ABSORB, ABILITY_DAMP, ABILITY_DRIZZLE },
        .bodyColor = BODY_COLOR_BLUE,
        .noFlip = TRUE,
        .speciesName = _("Policroak"),
        .cryId = CRY_POLITOED,
        .natDexNum = NATIONAL_DEX_POLICROAK,
        .categoryName = _("Frog-Toad"),
        .height = 11,
        .weight = 339,
        .description = COMPOUND_STRING(
            "The poisonous glands on its face\n"
            "are used to defend against enemies."
            "It can often be found lounging in\n"
            "swamps."),
        .pokemonScale = 289,
        .pokemonOffset = 6,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Policroak,
        .frontPicFemale = gMonFrontPic_Policroak,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicSizeFemale = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_JUMPS_V_STRETCH,
        .frontAnimDelay = 40,
        .backPic = gMonBackPic_Policroak,
        .backPicFemale = gMonBackPic_Policroak,
        .backPicSize = MON_COORDS_SIZE(56, 56),
        .backPicSizeFemale = MON_COORDS_SIZE(56, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_CONCAVE_ARC_LARGE,
        .palette = gMonPalette_Policroak,
        .shinyPalette = gMonShinyPalette_Policroak,
        .iconSprite = gMonIcon_Policroak,
        .iconPalIndex = 4,
        FOOTPRINT(Politoed)
        OVERWORLD(
            sPicTable_Politoed,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Politoed,
            gShinyOverworldPalette_Politoed
        )
        .levelUpLearnset = sTentacruelLevelUpLearnset,
        .teachableLearnset = sTentacruelTeachableLearnset,
    },

    [SPECIES_REGIALPHA] =
    {
        .baseHP        = 110,
        .baseAttack    = 160,
        .baseDefense   = 110,
        .baseSpeed     = 100,
        .baseSpAttack  = 120,
        .baseSpDefense = 110,
        .types = MON_TYPES(TYPE_DARK),
        .catchRate = 3,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 335,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 302,
    #else
        .expYield = 220,
    #endif
        .evYield_Attack = 3,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Regialpha"),
        .cryId = CRY_REGIGIGAS,
        .natDexNum = NATIONAL_DEX_REGIALPHA,
        .categoryName = _("Old Shadow"),
        .height = 37,
        .weight = 4200,
        .description = COMPOUND_STRING(
            "The shadow of the titan. This\n"
            "Pokemon was formed alongside\n"
            "Regigigas, always hidden."),
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 610,
        .trainerOffset = 17,
        .frontPic = gMonFrontPic_Regialpha,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Regialpha,
        .backPicSize = MON_COORDS_SIZE(64, 40),
        .backPicYOffset = 13,
        .backAnimId = BACK_ANIM_V_SHAKE_LOW,
        .palette = gMonPalette_Regialpha,
        .shinyPalette = gMonShinyPalette_Regialpha,
        .iconSprite = gMonIcon_Regialpha,
        .iconPalIndex = 4,
        FOOTPRINT(Regigigas)
        OVERWORLD(
            sPicTable_Regigigas,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Regigigas,
            gShinyOverworldPalette_Regigigas
        )
        .isLegendary = TRUE,
        .levelUpLearnset = sDarkraiLevelUpLearnset,
        .teachableLearnset = sRegigigasTeachableLearnset,
    },

    [SPECIES_SHADOW_LUGIA] =
    {
        .baseHP        = 100,
        .baseAttack    = 130,
        .baseDefense   = 130,
        .baseSpeed     = 100,
        .baseSpAttack  = 110,
        .baseSpDefense = 140,
        .types = MON_TYPES(TYPE_DARK, TYPE_PSYCHIC),
        .catchRate = 3,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 340,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 306,
    #else
        .expYield = 220,
    #endif
        .evYield_SpDefense = 3,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_MULTISCALE },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("SHDW Lugia"),
        .cryId = CRY_LUGIA,
        .natDexNum = NATIONAL_DEX_SHADOW_LUGIA,
        .categoryName = _("Corrupted"),
        .height = 52,
        .weight = 2160,
        .description = COMPOUND_STRING(
            "Created by a dangerous scientist,\n"
            "Shadow Lugia is forever tainted\n"
            "by human evil."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 721,
        .trainerOffset = 19,
        .frontPic = gMonFrontPic_ShadowLugia,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_IN_STAGES,
        .frontAnimDelay = 20,
        .enemyMonElevation = 6,
        .backPic = gMonBackPic_ShadowLugia,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 1,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_ShadowLugia,
        .shinyPalette = gMonShinyPalette_ShadowLugia,
        .iconSprite = gMonIcon_ShadowLugia,
        .iconPalIndex = 0,
        FOOTPRINT(Lugia)
        OVERWORLD(
            sPicTable_Lugia,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Lugia,
            gShinyOverworldPalette_Lugia
        )
        .isLegendary = TRUE,
        .isFrontierBanned = TRUE,
        .levelUpLearnset = sLugiaLevelUpLearnset,
        .teachableLearnset = sLugiaTeachableLearnset,
    },

    [SPECIES_SHUCKLE_HOENNIAN] =
    {
        .baseHP        = 50,
        .baseAttack    = 160,
        .baseDefense   = 40,
        .baseSpeed     = 120,
        .baseSpAttack  = 160,
        .baseSpDefense = 40,
        .types = MON_TYPES(TYPE_GRASS, TYPE_ROCK),
        .catchRate = 190,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 177 : 80,
        .evYield_Defense = 1,
        .evYield_SpDefense = 1,
        .itemCommon = ITEM_BERRY_JUICE,
        .itemRare = ITEM_BERRY_JUICE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_STURDY, ABILITY_GLUTTONY, ABILITY_CONTRARY },
    #else
        .abilities = { ABILITY_STURDY, ABILITY_NONE, ABILITY_CONTRARY },
    #endif
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Shuckle"),
        .cryId = CRY_SHUCKLE,
        .natDexNum = NATIONAL_DEX_SHUCKLE_HOENNIAN,
        .categoryName = _("Mold"),
        .height = 6,
        .weight = 205,
        .description = COMPOUND_STRING(
            "A Shuckle hides under rocks, keeping its\n"
            "body concealed inside its shell while\n"
            "eating stored berries. The berries mix with\n"
            "its body fluids to become a juice."),
        .pokemonScale = 485,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_ShuckleH,
        .frontPicSize = MON_COORDS_SIZE(56, 48),
        .frontPicYOffset = 8,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_SWING_CONCAVE,
        .backPic = gMonBackPic_ShuckleH,
        .backPicSize = MON_COORDS_SIZE(48, 48),
        .backPicYOffset = 11,
        .backAnimId = BACK_ANIM_DIP_RIGHT_SIDE,
        .palette = gMonPalette_ShuckleH,
        .shinyPalette = gMonShinyPalette_ShuckleH,
        .iconSprite = gMonIcon_ShuckleH,
        .iconPalIndex = 1,
        FOOTPRINT(Shuckle)
        OVERWORLD(
            sPicTable_Shuckle,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Shuckle,
            gShinyOverworldPalette_Shuckle
        )
        .levelUpLearnset = sShuckleLevelUpLearnset,
        .teachableLearnset = sShuckleTeachableLearnset,
        .eggMoveLearnset = sShuckleEggMoveLearnset,
    },

    [SPECIES_UNOWN_GOD] =
    {
        .baseHP        = 80,
        .baseAttack    = 80,
        .baseDefense   = 80,
        .baseSpeed     = 160,
        .baseSpAttack  = 160,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_PSYCHIC),
        .catchRate = 125,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 52 : 44,
        .evYield_HP = 1,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_TELEPATHY },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Unone"),
        .cryId = CRY_UNOWN,
        .natDexNum = NATIONAL_DEX_UNOWN_GOD,
        .categoryName = _("Bright"),
        .height = 6,
        .weight = 140,
        .description = COMPOUND_STRING(
            "T.e cr..t.r n..ds me."),
        .pokemonScale = 484,
        .pokemonOffset = 19,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_UnownGod,
        .frontPicSize = MON_COORDS_SIZE(48, 40),
        .frontPicYOffset = 11,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_JUMPS_V_STRETCH,
        .enemyMonElevation = 20,
        .frontAnimDelay = 15,
        .backPic = gMonBackPic_UnownGod,
        .backPicSize = MON_COORDS_SIZE(48, 48),
        .backPicYOffset = 11,
        .backAnimId = BACK_ANIM_CONCAVE_ARC_SMALL,
        .palette = gMonPalette_UnownGod,
        .shinyPalette = gMonShinyPalette_UnownGod,
        .iconSprite = gMonIcon_UnownGod,
        .iconPalIndex = 0,
        FOOTPRINT(Unown)
        OVERWORLD(
            sPicTable_Substitute,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Wynaut,
            gShinyOverworldPalette_Wynaut
        )
        .tmIlliterate = TRUE,
        .levelUpLearnset = sUnownLevelUpLearnset,
        .teachableLearnset = sUnownTeachableLearnset,
    },

    [SPECIES_GROWLITHE_HOENNIAN] =
    {
        .baseHP        = 55,
        .baseAttack    = 70,
        .baseDefense   = 45,
        .baseSpeed     = 60,
        .baseSpAttack  = 70,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 190,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 70 : 91,
        .evYield_Attack = 1,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_INTIMIDATE, ABILITY_WATER_ABSORB, ABILITY_JUSTIFIED },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Growlithe"),
        .cryId = CRY_GROWLITHE,
        .natDexNum = NATIONAL_DEX_GROWLITHE_HOENNIAN,
        .categoryName = _("Water Dog"),
        .height = 7,
        .weight = 190,
        .description = COMPOUND_STRING(
            "Dog wit the wata, wata dog,\n"
            "dog wit the wata on it."),
        .pokemonScale = 346,
        .pokemonOffset = 14,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_GrowlitheH,
        .frontPicSize = MON_COORDS_SIZE(48, 48),
        .frontPicYOffset = 9,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .frontAnimDelay = 30,
        .backPic = gMonBackPic_GrowlitheH,
        .backPicSize = MON_COORDS_SIZE(48, 56),
        .backPicYOffset = 8,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_GrowlitheH,
        .shinyPalette = gMonShinyPalette_GrowlitheH,
        .iconSprite = gMonIcon_GrowlitheH,
        .iconPalIndex = 0,
        FOOTPRINT(Growlithe)
        OVERWORLD(
            sPicTable_Growlithe,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Growlithe,
            gShinyOverworldPalette_Growlithe
        )
        .levelUpLearnset = sCarvanhaLevelUpLearnset,
        .teachableLearnset = sCarvanhaTeachableLearnset,
        .eggMoveLearnset = sCarvanhaEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_ITEM, ITEM_WATER_STONE, SPECIES_ARCANINE_HOENNIAN}),
    },

    [SPECIES_ARCANINE_HOENNIAN] =
    {
        .baseHP        = 90,
        .baseAttack    = 120,
        .baseDefense   = 80,
        .baseSpeed     = 120,
        .baseSpAttack  = 120,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 75,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 194 : 213,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(25),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_INTIMIDATE, ABILITY_FLASH_FIRE, ABILITY_JUSTIFIED },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Arcanine"),
        .cryId = CRY_ARCANINE,
        .natDexNum = NATIONAL_DEX_ARCANINE_HOENNIAN,
        .categoryName = _("Legendary"),
        .height = 19,
        .weight = 1550,
        .description = COMPOUND_STRING(
            "This fleet-footed Pokémon is said to swim\n"
            "over 6,200 miles in a single day and night.\n"
            "The fins that adorn its body\n"
            "is its source of speed."),
        .pokemonScale = 256,
        .pokemonOffset = 1,
        .trainerScale = 312,
        .trainerOffset = 4,
        .frontPic = gMonFrontPic_ArcanineH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .frontAnimDelay = 8,
        .backPic = gMonBackPic_ArcanineH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_ArcanineH,
        .shinyPalette = gMonShinyPalette_ArcanineH,
        .iconSprite = gMonIcon_ArcanineH,
        .iconPalIndex = 2,
        FOOTPRINT(Arcanine)
        OVERWORLD(
            sPicTable_Arcanine,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Arcanine,
            gShinyOverworldPalette_Arcanine
        )
        .levelUpLearnset = sSharpedoLevelUpLearnset,
        .teachableLearnset = sSharpedoTeachableLearnset,
    },

    [SPECIES_SLIFER] =
    {
        .baseHP        = 150,
        .baseAttack    = 150,
        .baseDefense   = 150,
        .baseSpeed     = 150,
        .baseSpAttack  = 150,
        .baseSpDefense = 150,
        .types = MON_TYPES(TYPE_DRAGON, TYPE_ELECTRIC),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 340,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 306,
    #else
        .expYield = 220,
    #endif
        .evYield_Attack = 2,
        .evYield_SpAttack = 1,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_AIR_LOCK, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Slifer TSD"),
        .cryId = CRY_RAYQUAZA,
        .natDexNum = NATIONAL_DEX_RAYQUAZA,
        .categoryName = _("Sky High"),
        .height = 70,
        .weight = 2065,
        .description = COMPOUND_STRING(
            "A Pokémon that flies endlessly in the\n"
            "ozone layer. It is said it would descend\n"
            "to the ground if Kyogre and Groudon\n"
            "were to fight."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 448,
        .trainerOffset = 12,
        .frontPic = gMonFrontPic_Slifer,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .frontAnimDelay = 60,
        .enemyMonElevation = 6,
        .backPic = gMonBackPic_Rayquaza,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_GROW_STUTTER,
        .palette = gMonPalette_Slifer,
        .shinyPalette = gMonShinyPalette_Rayquaza,
        .iconSprite = gMonIcon_Rayquaza,
        .iconPalIndex = 1,
        FOOTPRINT(Rayquaza)
        OVERWORLD(
            sPicTable_Rayquaza,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Rayquaza,
            gShinyOverworldPalette_Rayquaza
        )
        .isLegendary = TRUE,
        .isFrontierBanned = TRUE,
        .levelUpLearnset = sRayquazaLevelUpLearnset,
        .teachableLearnset = sRayquazaTeachableLearnset,
    },
    [SPECIES_KACHOW] =
    {
        .baseHP        = 70,
        .baseAttack    = 160,
        .baseDefense   = 160,
        .baseSpeed     = 80,
        .baseSpAttack  = 160,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_STEEL, TYPE_WATER),
        .catchRate = 3,
        .expYield = DIALGA_EXP_YIELD,
        .evYield_SpAttack = 3,
        .genderRatio = MON_GENDERLESS,
        .eggCycles = 120,
        .friendship = 0,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_NO_EGGS_DISCOVERED),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_TELEPATHY },
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Dialkia"),
        .cryId = CRY_KACHOW,
        .natDexNum = NATIONAL_DEX_DIALGA,
        .categoryName = _("Temporal"),
        .height = 54,
        .weight = 6830,
        .description = COMPOUND_STRING(
            "A Pokémon spoken of in legend.\n"
            "It completely controls the flow of time.\n"
            "It uses its power to travel at will\n"
            "through the past and future."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 721,
        .trainerOffset = 19,
        .frontPic = gMonFrontPic_Dialkia,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_Dialkia,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_Dialkia,
        .shinyPalette = gMonShinyPalette_Dialkia,
        .iconSprite = gMonIcon_Dialga,
        .iconPalIndex = 2,
        FOOTPRINT(Dialga)
        OVERWORLD(
            sPicTable_Dialga,
            SIZE_64x64,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dialga,
            gShinyOverworldPalette_Dialga
        )
        .isLegendary = TRUE,
        .isFrontierBanned = TRUE,
        .levelUpLearnset = sDialgaLevelUpLearnset,
        .teachableLearnset = sDialgaTeachableLearnset,
    },

#if P_FAMILY_NIDORAN
#if P_MEGA_EVOLUTIONS
    [SPECIES_NIDOKING_MEGA] =
    {
        .baseHP        = 95,
        .baseAttack    = 135,
        .baseDefense   = 90,
        .baseSpeed     = 125,
        .baseSpAttack  = 85,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_POISON, TYPE_GROUND),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 253,
    #elif P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 227,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 223,
    #else
        .expYield = 195,
    #endif
        .evYield_Attack = 3,
        .genderRatio = MON_MALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_POISON_POINT, ABILITY_RIVALRY, ABILITY_SHEER_FORCE },
    #else
        .abilities = { ABILITY_POISON_POINT, ABILITY_NONE, ABILITY_SHEER_FORCE },
    #endif
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Nidoking"),
        .cryId = CRY_NIDOKING,
        .natDexNum = NATIONAL_DEX_NIDOKING,
        .categoryName = _("Drill"),
        .height = 14,
        .weight = 620,
        .description = COMPOUND_STRING(
            "A Nidoking's thick tail packs enormously\n"
            "destructive power capable of toppling\n"
            "a metal transmission tower. Once it goes\n"
            "on a rampage, there is no stopping it."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_NidokingMega,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 64) : MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .frontAnimDelay = 25,
        .backPic = gMonBackPic_NidokingMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_NidokingMega,
        .shinyPalette = gMonShinyPalette_NidokingMega,
        .iconSprite = gMonIcon_Nidoking,
        .iconPalIndex = 2,
        SHADOW(0, 8, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Nidoking)
        OVERWORLD(
            sPicTable_Nidoking,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Nidoking,
            gShinyOverworldPalette_Nidoking
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sNidokingLevelUpLearnset,
        .teachableLearnset = sNidokingTeachableLearnset,
        .formSpeciesIdTable = sNidokingFormSpeciesIdTable,
        .formChangeTable = sNidokingFormChangeTable,
    },
#endif
#endif

#if P_FAMILY_SPINARAK
    [SPECIES_ARACHTRES] =
    {
        .baseHP        = 100,
        .baseAttack    = 130,
        .baseDefense   = 80,
        .baseSpeed     = 100,
        .baseSpAttack  = 60,
        .baseSpDefense = P_UPDATED_STATS >= GEN_7 ? 70 : 60,
        .types = MON_TYPES(TYPE_BUG, TYPE_POISON),
        .catchRate = 90,
    #if P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 140,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 137,
    #else
        .expYield = 134,
    #endif
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_SWARM, ABILITY_INSOMNIA, ABILITY_SNIPER },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Arachtres"),
        .cryId = CRY_ARIADOS,
        .natDexNum = NATIONAL_DEX_ARACHTRES,
        .categoryName = _("Long Leg"),
        .height = 13,
        .weight = 375,
        .description = COMPOUND_STRING(
            "Its feet are tipped with tiny hooked claws\n"
            "that enable it to scuttle on ceilings and\n"
            "vertical walls. It constricts its foe with\n"
            "thin and strong silk webbing."),
        .pokemonScale = 316,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Arachtres,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 5 : 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_Arachtres,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 10,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_Arachtres,
        .shinyPalette = gMonShinyPalette_Arachtres,
        .iconSprite = gMonIcon_Arachtres,
        .iconPalIndex = 0,
        SHADOW(1, 3, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Ariados)
        OVERWORLD(
            sPicTable_Ariados,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_BUG,
            sAnimTable_Following,
            gOverworldPalette_Ariados,
            gShinyOverworldPalette_Ariados
        )
        .levelUpLearnset = sAriadosLevelUpLearnset,
        .teachableLearnset = sAriadosTeachableLearnset,
    },
#endif //P_FAMILY_SPINARAK

    [SPECIES_WEEDLE_HOENNIAN] =
    {
        .baseHP        = 40,
        .baseAttack    = 35,
        .baseDefense   = 30,
        .baseSpeed     = 50,
        .baseSpAttack  = 50,
        .baseSpDefense = 30,
        .types = MON_TYPES(TYPE_ICE, TYPE_POISON),
        .catchRate = 255,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 39 : 52,
        .evYield_Speed = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_SHIELD_DUST, ABILITY_NONE, ABILITY_RUN_AWAY },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Weedle"),
        .cryId = CRY_WEEDLE,
        .natDexNum = NATIONAL_DEX_WEEDLE_HOENNIAN,
        .categoryName = _("Icy Bug"),
        .height = 3,
        .weight = 32,
        .description = COMPOUND_STRING(
            "A Weedle has an extremely acute sense\n"
            "of smell. It distinguishes its favorite\n"
            "kinds of leaves from those it dislikes by\n"
            "sniffing with its big red nose."),
        .pokemonScale = 455,
        .pokemonOffset = 22,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_WeedleH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(40, 32) : MON_COORDS_SIZE(40, 40),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 18 : 13,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SLIDE_SLOW,
        .frontAnimDelay = 10,
        .backPic = gMonBackPic_WeedleH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(40, 48) : MON_COORDS_SIZE(56, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_WeedleH,
        .shinyPalette = gMonShinyPalette_WeedleH,
        .iconSprite = gMonIcon_WeedleH,
        .iconPalIndex = 0,
        SHADOW(0, 0, SHADOW_SIZE_S)
        FOOTPRINT(Weedle)
        OVERWORLD(
            sPicTable_Weedle,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Weedle,
            gShinyOverworldPalette_Weedle
        )
        .tmIlliterate = TRUE,
        .levelUpLearnset = sWeedleHLevelUpLearnset,
        .teachableLearnset = sWeedleTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 14, SPECIES_KAKUNA_HOENNIAN}),
    },

    [SPECIES_KAKUNA_HOENNIAN] =
    {
        .baseHP        = 55,
        .baseAttack    = 25,
        .baseDefense   = 70,
        .baseSpeed     = 35,
        .baseSpAttack  = 25,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_ICE, TYPE_POISON),
        .catchRate = 120,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 72 : 71,
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_SHED_SKIN, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Kakuna"),
        .cryId = CRY_KAKUNA,
        .natDexNum = NATIONAL_DEX_KAKUNA_HOENNIAN,
        .categoryName = _("Cocoon"),
        .height = 6,
        .weight = 100,
        .description = COMPOUND_STRING(
            "It remains virtually immobile while it\n"
            "clings to a tree. However, on the inside,\n"
            "it busily prepares for evolution. This is\n"
            "evident from how hot its shell becomes."),
        .pokemonScale = 424,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_KakunaH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(32, 40) : MON_COORDS_SIZE(24, 48),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 14 : 11,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GLOW_ORANGE,
        .frontAnimDelay = 20,
        .backPic = gMonBackPic_KakunaH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(32, 48) : MON_COORDS_SIZE(32, 40),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_DIP_RIGHT_SIDE,
        .palette = gMonPalette_KakunaH,
        .shinyPalette = gMonShinyPalette_KakunaH,
        .iconSprite = gMonIcon_KakunaH,
        .iconPalIndex = 0,
        SHADOW(0, 2, SHADOW_SIZE_S)
        FOOTPRINT(Kakuna)
        OVERWORLD(
            sPicTable_Kakuna,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Kakuna,
            gShinyOverworldPalette_Kakuna
        )
        .tmIlliterate = TRUE,
        .levelUpLearnset = sKakunaHLevelUpLearnset,
        .teachableLearnset = sKakunaTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 18, SPECIES_BEEDRILL_HOENNIAN}),
    },

#define BEEDRILL_ATTACK (P_UPDATED_STATS >= GEN_6 ? 110 : 110)

    [SPECIES_BEEDRILL_HOENNIAN] =
    {
        .baseHP        = 85,
        .baseAttack    = BEEDRILL_ATTACK,
        .baseDefense   = 60,
        .baseSpeed     = 105,
        .baseSpAttack  = 45,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_ICE, TYPE_POISON),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 198,
    #elif P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 178,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 173,
    #else
        .expYield = 159,
    #endif
        .evYield_Attack = 2,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_POISON_BARB,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_SWARM, ABILITY_NONE, ABILITY_SNIPER },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Beedrill"),
        .cryId = CRY_BEEDRILL,
        .natDexNum = NATIONAL_DEX_BEEDRILL_HOENNIAN,
        .categoryName = _("Frozen Bee"),
        .height = 10,
        .weight = 295,
        .description = COMPOUND_STRING(
            "A Beedrill is extremely territorial.\n"
            "For safety reasons, no one should ever\n"
            "approach its nest. If angered, they will\n"
            "attack in a swarm."),
        .pokemonScale = 366,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_BeedrillH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 48) : MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 9 : 5,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_H_VIBRATE : ANIM_ZIGZAG_SLOW,
        .enemyMonElevation = P_GBA_STYLE_SPECIES_GFX ? 8 : 9,
        .backPic = gMonBackPic_BeedrillH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 48) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_VIBRATE,
        .palette = gMonPalette_BeedrillH,
        .shinyPalette = gMonShinyPalette_BeedrillH,
        .iconSprite = gMonIcon_BeedrillH,
        .iconPalIndex = 0,
        SHADOW(4, 15, SHADOW_SIZE_M)
        FOOTPRINT(Beedrill)
        OVERWORLD(
            sPicTable_Beedrill,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Beedrill,
            gShinyOverworldPalette_Beedrill
        )
        .levelUpLearnset = sBeedrillHLevelUpLearnset,
        .teachableLearnset = sBeedrillTeachableLearnset,
        .formSpeciesIdTable = sBeedrillHFormSpeciesIdTable,
        .formChangeTable = sBeedrillHFormChangeTable,
    },

#if P_MEGA_EVOLUTIONS
    [SPECIES_BEEDRILL_HOENNIAN_MEGA] =
    {
        .baseHP        = 85,
        .baseAttack    = BEEDRILL_ATTACK + 70,
        .baseDefense   = 100,
        .baseSpeed     = 155,
        .baseSpAttack  = 15,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_ICE, TYPE_POISON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 248 : 223,
        .evYield_Attack = 2,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_POISON_BARB,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_ADAPTABILITY, ABILITY_ADAPTABILITY, ABILITY_ADAPTABILITY },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Beedrill"),
        .cryId = CRY_BEEDRILL_MEGA,
        .natDexNum = NATIONAL_DEX_BEEDRILL_HOENNIAN,
        .categoryName = _("Frozen Bee"),
        .height = 14,
        .weight = 405,
        .description = COMPOUND_STRING(
            "A Beedrill is extremely territorial.\n"
            "For safety reasons, no one should ever\n"
            "approach its nest. If angered, they will\n"
            "attack in a swarm."),
        .pokemonScale = 366,
        .pokemonOffset = 2,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_BeedrillHMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .enemyMonElevation = 5,
        .backPic = gMonBackPic_BeedrillHMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 1,
        .backAnimId = BACK_ANIM_H_VIBRATE,
        .palette = gMonPalette_BeedrillHMega,
        .shinyPalette = gMonShinyPalette_BeedrillHMega,
        .iconSprite = gMonIcon_BeedrillHMega,
        .iconPalIndex = 0,
        SHADOW(1, 14, SHADOW_SIZE_S)
        FOOTPRINT(Beedrill)
    #if OW_BATTLE_ONLY_FORMS
        OVERWORLD(
            sPicTable_BeedrillMega,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_BeedrillMega,
            gShinyOverworldPalette_BeedrillMega
        )
    #endif //OW_BATTLE_ONLY_FORMS
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sBeedrillHLevelUpLearnset,
        .teachableLearnset = sBeedrillTeachableLearnset,
        .formSpeciesIdTable = sBeedrillHFormSpeciesIdTable,
        .formChangeTable = sBeedrillHFormChangeTable,
    },
#endif //P_MEGA_EVOLUTIONS

    [SPECIES_PAWNIARD_HOENNIAN] =
    {
        .baseHP        = 45,
        .baseAttack    = 85,
        .baseDefense   = 70,
        .baseSpeed     = 60,
        .baseSpAttack  = 40,
        .baseSpDefense = 40,
        .types = MON_TYPES(TYPE_FIGHTING, TYPE_STEEL),
        .catchRate = 120,
        .expYield = 68,
        .evYield_Attack = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_DEFIANT, ABILITY_INNER_FOCUS, ABILITY_PRESSURE },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Pawniard"),
        .cryId = CRY_PAWNIARD,
        .natDexNum = NATIONAL_DEX_PAWNIARD_HOENNIAN,
        .categoryName = _("Sharp Blade"),
        .height = 5,
        .weight = 102,
        .description = COMPOUND_STRING(
            "Ignoring their injuries, groups attack by\n"
            "sinking the blades that cover their bodies\n"
            "into their prey. If battling dulls them, it\n"
            "sharpens them on stones by the river."),
        .pokemonScale = 432,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_PawniardH,
        .frontPicSize = MON_COORDS_SIZE(56, 48),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_VIBRATE,
        .backPic = gMonBackPic_PawniardH,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_PawniardH,
        .shinyPalette = gMonShinyPalette_PawniardH,
        .iconSprite = gMonIcon_PawniardH,
        .iconPalIndex = 0,
        SHADOW(4, 4, SHADOW_SIZE_S)
        FOOTPRINT(Pawniard)
        OVERWORLD(
            sPicTable_Pawniard,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Pawniard,
            gShinyOverworldPalette_Pawniard
        )
        .levelUpLearnset = sPawniardHLevelUpLearnset,
        .teachableLearnset = sPawniardTeachableLearnset,
        .eggMoveLearnset = sPawniardEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 36, SPECIES_BISHARP_HOENNIAN}),
    },

    [SPECIES_BISHARP_HOENNIAN] =
    {
        .baseHP        = 65,
        .baseAttack    = 125,
        .baseDefense   = 100,
        .baseSpeed     = 70,
        .baseSpAttack  = 60,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_FIGHTING, TYPE_STEEL),
        .catchRate = 45,
        .expYield = 172,
        .evYield_Attack = 2,
        .itemRare = ITEM_LEADERS_CREST,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_DEFIANT, ABILITY_INNER_FOCUS, ABILITY_PRESSURE },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Bisharp"),
        .cryId = CRY_BISHARP,
        .natDexNum = NATIONAL_DEX_BISHARP_HOENNIAN,
        .categoryName = _("Sword Blade"),
        .height = 16,
        .weight = 700,
        .description = COMPOUND_STRING(
            "This pitiless Pokémon commands a group\n"
            "of Pawniard to hound prey into immobility.\n"
            "It battles to become the boss, but will be\n"
            "driven from the group if it loses."),
        .pokemonScale = 259,
        .pokemonOffset = 1,
        .trainerScale = 296,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_BisharpH,
        .frontPicSize = MON_COORDS_SIZE(48, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_STRETCH,
        .backPic = gMonBackPic_BisharpH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_GROW_STUTTER,
        .palette = gMonPalette_BisharpH,
        .shinyPalette = gMonShinyPalette_BisharpH,
        .iconSprite = gMonIcon_BisharpH,
        .iconPalIndex = 0,
        SHADOW(-2, 14, SHADOW_SIZE_M)
        FOOTPRINT(Bisharp)
        OVERWORLD(
            sPicTable_Bisharp,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Bisharp,
            gShinyOverworldPalette_Bisharp
        )
        .levelUpLearnset = sBisharpHLevelUpLearnset,
        .teachableLearnset = sBisharpTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 52, SPECIES_KINGAMBIT_HOENNIAN}),
    },

    [SPECIES_KINGAMBIT_HOENNIAN] =
    {
        .baseHP        = 100,
        .baseAttack    = 135,
        .baseDefense   = 120,
        .baseSpeed     = 50,
        .baseSpAttack  = 60,
        .baseSpDefense = 85,
        .types = MON_TYPES(TYPE_FIGHTING, TYPE_STEEL),
        .catchRate = 25,
        .expYield = 275,
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_DEFIANT, ABILITY_SUPREME_OVERLORD, ABILITY_PRESSURE },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Kingambit"),
        .cryId = CRY_KINGAMBIT,
        .natDexNum = NATIONAL_DEX_KINGAMBIT_HOENNIAN,
        .categoryName = _("Big Blade"),
        .height = 20,
        .weight = 1200,
        .description = COMPOUND_STRING(
            "Though it commands a massive army\n"
            "in battle, it's not skilled at\n"
            "devising complex strategies. It just uses\n"
            "brute strength to keep pushing."),
        .pokemonScale = 356,
        .pokemonOffset = 17,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_KingambitH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        //.frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_KingambitH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        //.backAnimId = BACK_ANIM_NONE,
        .palette = gMonPalette_KingambitH,
        .shinyPalette = gMonShinyPalette_KingambitH,
        .iconSprite = gMonIcon_KingambitH,
        .iconPalIndex = 0,
        SHADOW(-2, 14, SHADOW_SIZE_M)
        FOOTPRINT(Kingambit)
        OVERWORLD(
            sPicTable_Kingambit,
            SIZE_64x64,
            SHADOW_SIZE_L,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Kingambit,
            gShinyOverworldPalette_Kingambit
        )
        .levelUpLearnset = sKingambitHLevelUpLearnset,
        .teachableLearnset = sKingambitTeachableLearnset,
    },
#if P_FAMILY_CORPHISH
    [SPECIES_CRUSTULES] =
    {
        .baseHP        = 86,
        .baseAttack    = 160,
        .baseDefense   = 105,
        .baseSpeed     = 65,
        .baseSpAttack  = 75,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_WATER, TYPE_STEEL),
        .catchRate = 155,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 164 : 161,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_FLUCTUATING,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_WATER_3),
        .abilities = { ABILITY_HYPER_CUTTER, ABILITY_SHELL_ARMOR, ABILITY_ADAPTABILITY },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Crustules"),
        .cryId = CRY_CRAWDAUNT,
        .natDexNum = NATIONAL_DEX_CRUSTULES,
        .categoryName = _("Rogue"),
        .height = 11,
        .weight = 328,
        .description = COMPOUND_STRING(
            "A brutish Pokémon that loves to battle.\n"
            "A veteran Crawdaunt that has prevailed in\n"
            "hundreds of battles has giant pincers\n"
            "marked with countless scars."),
        .pokemonScale = 365,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Crustules,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GROW_VIBRATE,
        .backPic = gMonBackPic_Crustules,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_Crustules,
        .shinyPalette = gMonShinyPalette_Crustules,
        .iconSprite = gMonIcon_Crustules,
        .iconPalIndex = 0,
        SHADOW(6, 9, SHADOW_SIZE_M)
        FOOTPRINT(Crawdaunt)
        OVERWORLD(
            sPicTable_Crawdaunt,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Crawdaunt,
            gShinyOverworldPalette_Crawdaunt
        )
        .levelUpLearnset = sCrawdauntLevelUpLearnset,
        .teachableLearnset = sCrawdauntTeachableLearnset,
    },
#endif //P_FAMILY_CORPHISH

    [SPECIES_DEINO_HOENNIAN] =
    {
        .baseHP        = 52,
        .baseAttack    = 65,
        .baseDefense   = 50,
        .baseSpeed     = 38,
        .baseSpAttack  = 45,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_FAIRY, TYPE_DRAGON),
        .catchRate = 45,
        .expYield = 60,
        .evYield_Attack = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_DRAGON),
        .abilities = { ABILITY_HUSTLE, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Deino"),
        .cryId = CRY_DEINO,
        .natDexNum = NATIONAL_DEX_DEINO_HOENNIAN,
        .categoryName = _("Irate"),
        .height = 8,
        .weight = 173,
        .description = COMPOUND_STRING(
            "Lacking sight, it's unaware of its\n"
            "surroundings, so it bumps into things and\n"
            "eats anything that moves. Approaching\n"
            "it carelessly is dangerous."),
        .pokemonScale = 366,
        .pokemonOffset = 12,
        .trainerScale = 257,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_DeinoH,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_DeinoH,
        .backPicSize = MON_COORDS_SIZE(48, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_DeinoH,
        .shinyPalette = gMonShinyPalette_DeinoH,
        .iconSprite = gMonIcon_DeinoH,
        .iconPalIndex = 0,
        SHADOW(1, 3, SHADOW_SIZE_S)
        FOOTPRINT(Deino)
        OVERWORLD(
            sPicTable_Deino,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Deino,
            gShinyOverworldPalette_Deino
        )
        .levelUpLearnset = sDeinoHLevelUpLearnset,
        .teachableLearnset = sDeinoTeachableLearnset,
        .eggMoveLearnset = sDeinoEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 30, SPECIES_ZWEILOUS_HOENNIAN}),
    },

    [SPECIES_ZWEILOUS_HOENNIAN] =
    {
        .baseHP        = 72,
        .baseAttack    = 85,
        .baseDefense   = 70,
        .baseSpeed     = 58,
        .baseSpAttack  = 65,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_FAIRY, TYPE_DRAGON),
        .catchRate = 45,
        .expYield = 147,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_DRAGON),
        .abilities = { ABILITY_HUSTLE, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Zweilous"),
        .cryId = CRY_ZWEILOUS,
        .natDexNum = NATIONAL_DEX_ZWEILOUS_HOENNIAN,
        .categoryName = _("Hostile"),
        .height = 14,
        .weight = 500,
        .description = COMPOUND_STRING(
            "After it has eaten up all the food in its\n"
            "territory, it moves on to another area.\n"
            "Whichever head eats more than the other\n"
            "gets to be the leader."),
        .pokemonScale = 265,
        .pokemonOffset = 3,
        .trainerScale = 262,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_ZweilousH,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_STRETCH,
        .backPic = gMonBackPic_ZweilousH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SHAKE,
        .palette = gMonPalette_ZweilousH,
        .shinyPalette = gMonShinyPalette_ZweilousH,
        .iconSprite = gMonIcon_ZweilousH,
        .iconPalIndex = 0,
        SHADOW(1, 6, SHADOW_SIZE_L)
        FOOTPRINT(Zweilous)
        OVERWORLD(
            sPicTable_Zweilous,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Zweilous,
            gShinyOverworldPalette_Zweilous
        )
        .levelUpLearnset = sZweilousHLevelUpLearnset,
        .teachableLearnset = sZweilousTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 50, SPECIES_HYDREIGON_HOENNIAN}),
    },

    [SPECIES_HYDREIGON_HOENNIAN] =
    {
        .baseHP        = 92,
        .baseAttack    = 105,
        .baseDefense   = 90,
        .baseSpeed     = 98,
        .baseSpAttack  = 125,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_FAIRY, TYPE_DRAGON),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 300 : 270,
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_DRAGON),
        .abilities = { ABILITY_LEVITATE, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Hydreigon"),
        .cryId = CRY_HYDREIGON,
        .natDexNum = NATIONAL_DEX_HYDREIGON_HOENNIAN,
        .categoryName = _("Brutal"),
        .height = 18,
        .weight = 1600,
        .description = COMPOUND_STRING(
            "This brutal Pokémon travels the skies on\n"
            "its six wings. The heads on their arms do\n"
            "not have brains. They use all three heads\n"
            "to consume and destroy everything."),
        .pokemonScale = 267,
        .pokemonOffset = 2,
        .trainerScale = 286,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_HydreigonH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_RISING_WOBBLE,
        .enemyMonElevation = 3,
        .backPic = gMonBackPic_HydreigonH,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_GROW_STUTTER,
        .palette = gMonPalette_HydreigonH,
        .shinyPalette = gMonShinyPalette_HydreigonH,
        .iconSprite = gMonIcon_HydreigonH,
        .iconPalIndex = 0,
        SHADOW(1, 16, SHADOW_SIZE_M)
        FOOTPRINT(Hydreigon)
        OVERWORLD(
            sPicTable_Hydreigon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Hydreigon,
            gShinyOverworldPalette_Hydreigon
        )
        .levelUpLearnset = sHydreigonHLevelUpLearnset,
        .teachableLearnset = sHydreigonTeachableLearnset,
    },

    [SPECIES_SEEL_HOENNIAN] =
    {
        .baseHP        = 65,
        .baseAttack    = 45,
        .baseDefense   = 55,
        .baseSpeed     = 45,
        .baseSpAttack  = 45,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_ICE, TYPE_DRAGON),
        .catchRate = 190,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 65 : 100,
        .evYield_SpDefense = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_THICK_FAT, ABILITY_HYDRATION, ABILITY_ICE_BODY },
    #else
        .abilities = { ABILITY_THICK_FAT, ABILITY_NONE, ABILITY_ICE_BODY },
    #endif
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Seel"),
        .cryId = CRY_SEEL,
        .natDexNum = NATIONAL_DEX_SEEL_HOENNIAN,
        .categoryName = _("Sea Lion"),
        .height = 11,
        .weight = 900,
        .description = COMPOUND_STRING(
            "Seel hunt for prey in frigid, ice-covered\n"
            "seas. When it needs to breathe, it punches\n"
            "a hole through the ice with the sharply\n"
            "protruding section of its head."),
        .pokemonScale = 297,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_SeelH,
        .frontPicSize = MON_COORDS_SIZE(56, 48),
        .frontPicYOffset = 8,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_SWING_CONCAVE : ANIM_H_SLIDE_SLOW,
        .backPic = gMonBackPic_SeelH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(48, 48) : MON_COORDS_SIZE(64, 40),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_DIP_RIGHT_SIDE,
        .palette = gMonPalette_SeelH,
        .shinyPalette = gMonShinyPalette_SeelH,
        .iconSprite = gMonIcon_SeelH,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 0 : 0,
        SHADOW(0, 3, SHADOW_SIZE_L)
        FOOTPRINT(Seel)
        OVERWORLD(
            sPicTable_Seel,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Seel,
            gShinyOverworldPalette_Seel
        )
        .levelUpLearnset = sSeelHLevelUpLearnset,
        .teachableLearnset = sSeelTeachableLearnset,
        .eggMoveLearnset = sSeelEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 34, SPECIES_DEWGONG_HOENNIAN}),
    },

    [SPECIES_DEWGONG_HOENNIAN] =
    {
        .baseHP        = 100,
        .baseAttack    = 70,
        .baseDefense   = 100,
        .baseSpeed     = 70,
        .baseSpAttack  = 70,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_ICE, TYPE_DRAGON),
        .catchRate = 75,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 166 : 176,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_THICK_FAT, ABILITY_HYDRATION, ABILITY_ICE_BODY },
    #else
        .abilities = { ABILITY_THICK_FAT, ABILITY_NONE, ABILITY_ICE_BODY },
    #endif
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Dewgong"),
        .cryId = CRY_DEWGONG,
        .natDexNum = NATIONAL_DEX_DEWGONG_HOENNIAN,
        .categoryName = _("Sea Lion"),
        .height = 17,
        .weight = 1200,
        .description = COMPOUND_STRING(
            "It loves to snooze on bitterly cold ice.\n"
            "The sight of this Pokémon sleeping on\n"
            "a glacier was mistakenly thought to be\n"
            "a mermaid by a mariner long ago."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 275,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_DewgongH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 56) : MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 7 : 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE_SLOW,
        .backPic = gMonBackPic_DewgongH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 56) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_DewgongH,
        .shinyPalette = gMonShinyPalette_DewgongH,
        .iconSprite = gMonIcon_DewgongH,
        .iconPalIndex = 0,
        SHADOW(2, 9, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Dewgong)
        OVERWORLD(
            sPicTable_Dewgong,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dewgong,
            gShinyOverworldPalette_Dewgong
        )
        .levelUpLearnset = sDewgongHLevelUpLearnset,
        .teachableLearnset = sDewgongTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 50, SPECIES_BARIONG}),
    },
    [SPECIES_BARIONG] =
    {
        .baseHP        = 100,
        .baseAttack    = 100,
        .baseDefense   = 100,
        .baseSpeed     = 60,
        .baseSpAttack  = 140,
        .baseSpDefense = 115,
        .types = MON_TYPES(TYPE_ICE, TYPE_DRAGON),
        .catchRate = 75,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 166 : 176,
        .evYield_SpDefense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_THICK_FAT, ABILITY_HYDRATION, ABILITY_ICE_BODY },
    #else
        .abilities = { ABILITY_THICK_FAT, ABILITY_NONE, ABILITY_ICE_BODY },
    #endif
        .bodyColor = BODY_COLOR_WHITE,
        .speciesName = _("Bariong"),
        .cryId = CRY_DEWGONG,
        .natDexNum = NATIONAL_DEX_BARIONG,
        .categoryName = _("Sea Lion"),
        .height = 17,
        .weight = 1200,
        .description = COMPOUND_STRING(
            "It loves to snooze on bitterly cold ice.\n"
            "The sight of this Pokémon sleeping on\n"
            "a glacier was mistakenly thought to be\n"
            "a mermaid by a mariner long ago."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 275,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Bariong,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 56) : MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 7 : 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE_SLOW,
        .backPic = gMonBackPic_Bariong,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 56) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_Bariong,
        .shinyPalette = gMonShinyPalette_Bariong,
        .iconSprite = gMonIcon_Bariong,
        .iconPalIndex = 0,
        SHADOW(2, 9, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Dewgong)
        OVERWORLD(
            sPicTable_Dewgong,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dewgong,
            gShinyOverworldPalette_Dewgong
        )
        .levelUpLearnset = sDewgongHLevelUpLearnset,
        .teachableLearnset = sDewgongTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 50, SPECIES_BARIONG}),
    },
    [SPECIES_DODUO_HOENNIAN] =
    {
        .baseHP        = 35,
        .baseAttack    = 85,
        .baseDefense   = 45,
        .baseSpeed     = 75,
        .baseSpAttack  = 35,
        .baseSpDefense = 35,
        .types = MON_TYPES(TYPE_POISON, TYPE_DARK),
        .catchRate = 190,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 62 : 96,
        .evYield_Attack = 1,
        .itemRare = ITEM_SHARP_BEAK,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FLYING),
        .abilities = { ABILITY_RUN_AWAY, ABILITY_EARLY_BIRD, ABILITY_TANGLED_FEET },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Doduo"),
        .cryId = CRY_DODUO,
        .natDexNum = NATIONAL_DEX_DODUO_HOENNIAN,
        .categoryName = _("Twin Bird"),
        .height = 14,
        .weight = 392,
        .description = COMPOUND_STRING(
            "Even while eating or sleeping, one of the\n"
            "heads remains always vigilant for any sign\n"
            "of danger. When threatened, it flees at\n"
            "over 60 miles per hour."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 257,
        .trainerOffset = -1,
        .frontPic = gMonFrontPic_DoduoH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(40, 56) : MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 5 : 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE_SLOW,
        .backPic = gMonBackPic_DoduoH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(48, 48) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_DoduoH,
        .shinyPalette = gMonShinyPalette_DoduoH,
        .iconSprite = gMonIcon_DoduoH,
        .iconPalIndex = 2,
        SHADOW(6, 5, SHADOW_SIZE_M)
        FOOTPRINT(Doduo)
        OVERWORLD(
            sPicTable_Doduo,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Doduo,
            gShinyOverworldPalette_Doduo
        )
        .levelUpLearnset = sDoduoHLevelUpLearnset,
        .teachableLearnset = sDoduoTeachableLearnset,
        .eggMoveLearnset = sDoduoEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 31, SPECIES_DODRIO_HOENNIAN}),
    },

    [SPECIES_DODRIO_HOENNIAN] =
    {
        .baseHP        = 60,
        .baseAttack    = 110,
        .baseDefense   = 70,
        .baseSpeed     = P_UPDATED_STATS >= GEN_7 ? 110 : 100,
        .baseSpAttack  = 60,
        .baseSpDefense = 60,
        .types = MON_TYPES(TYPE_POISON, TYPE_DARK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 165,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 161,
    #else
        .expYield = 158,
    #endif
        .evYield_Attack = 2,
        .itemRare = ITEM_SHARP_BEAK,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FLYING),
        .abilities = { ABILITY_RUN_AWAY, ABILITY_EARLY_BIRD, ABILITY_TANGLED_FEET },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Dodrio"),
        .cryId = CRY_DODRIO,
        .natDexNum = NATIONAL_DEX_DODRIO_HOENNIAN,
        .categoryName = _("Triple Bird"),
        .height = 18,
        .weight = 852,
        .description = COMPOUND_STRING(
            "A peculiar Pokémon species with three\n"
            "heads. It vigorously races across grassy\n"
            "plains even in arid seasons with little\n"
            "rainfall."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 268,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_DodrioH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_LUNGE_GROW : ANIM_V_STRETCH,
        .backPic = gMonBackPic_DodrioH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_DodrioH,
        .shinyPalette = gMonShinyPalette_DodrioH,
        .iconSprite = gMonIcon_DodrioH,
        .iconPalIndex = 2,
        SHADOW(3, 12, SHADOW_SIZE_L)
        FOOTPRINT(Dodrio)
        OVERWORLD(
            sPicTable_Dodrio,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Dodrio,
            gShinyOverworldPalette_Dodrio
        )
        .levelUpLearnset = sDoduoHLevelUpLearnset,
        .teachableLearnset = sDodrioTeachableLearnset,
    },
#if P_FAMILY_TURTWIG
#if P_MEGA_EVOLUTIONS
    [SPECIES_TORTERRA_MEGA] =
    {
        .baseHP        = 105,
        .baseAttack    = 129,
        .baseDefense   = 115,
        .baseSpeed     = 56,
        .baseSpAttack  = 75,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_GRASS, TYPE_GROUND),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 263,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 236,
    #else
        .expYield = 208,
    #endif
        .evYield_Attack = 2,
        .evYield_Defense = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
        .abilities = { ABILITY_OVERGROW, ABILITY_NONE, ABILITY_SHELL_ARMOR },
        .bodyColor = BODY_COLOR_GREEN,
        .noFlip = TRUE,
        .speciesName = _("Torterra"),
        .cryId = CRY_TORTERRA,
        .natDexNum = NATIONAL_DEX_TORTERRA,
        .categoryName = _("Continent"),
        .height = 22,
        .weight = 3100,
        .description = COMPOUND_STRING(
            "Some Pokémon are born on a Torterra's\n"
            "back and spend their entire life there.\n"
            "Ancient people imagined that beneath \n"
            "the ground, a gigantic Torterra dwelled."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 348,
        .trainerOffset = 6,
        .frontPic = gMonFrontPic_TorterraMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SHAKE,
        .backPic = gMonBackPic_TorterraMega,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 4,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_GREEN,
        .palette = gMonPalette_TorterraMega,
        .shinyPalette = gMonShinyPalette_TorterraMega,
        .iconSprite = gMonIcon_TorterraMega,
        .iconPalIndex = 1,
        SHADOW(1, 10, SHADOW_SIZE_L)
        FOOTPRINT(Torterra)
        OVERWORLD(
            sPicTable_Torterra,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Torterra,
            gShinyOverworldPalette_Torterra
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sTorterraLevelUpLearnset,
        .teachableLearnset = sTorterraTeachableLearnset,
        .formSpeciesIdTable = sTorterraFormSpeciesIdTable,
        .formChangeTable = sTorterraFormChangeTable,
    },
#endif //P_FAMILY_TURTWIG
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_CHIMCHAR
#if P_MEGA_EVOLUTIONS
    [SPECIES_INFERNAPE_MEGA] =
    {
        .baseHP        = 76,
        .baseAttack    = 145,
        .baseDefense   = 71,
        .baseSpeed     = 130,
        .baseSpAttack  = 145,
        .baseSpDefense = 71,
        .types = MON_TYPES(TYPE_FIRE, TYPE_FIGHTING),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 267,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 240,
    #else
        .expYield = 209,
    #endif
        .evYield_Attack = 1,
        .evYield_Speed = 1,
        .evYield_SpAttack = 1,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD, EGG_GROUP_HUMAN_LIKE),
        .abilities = { ABILITY_BLAZE, ABILITY_NONE, ABILITY_IRON_FIST },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Infernape"),
        .cryId = CRY_INFERNAPE,
        .natDexNum = NATIONAL_DEX_INFERNAPE,
        .categoryName = _("Flame"),
        .height = 12,
        .weight = 550,
        .description = COMPOUND_STRING(
            "Its crown of fire is indicative of its\n"
            "fiery nature. It uses a special kind of\n"
            "martial arts involving all of its limbs to\n"
            "take on any opponent."),
        .pokemonScale = 282,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_InfernapeMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_InfernapeMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 1,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_InfernapeMega,
        .shinyPalette = gMonShinyPalette_InfernapeMega,
        .iconSprite = gMonIcon_InfernapeMega,
        .iconPalIndex = 0,
        SHADOW(0, 9, SHADOW_SIZE_L)
        FOOTPRINT(Infernape)
        OVERWORLD(
            sPicTable_Infernape,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Infernape,
            gShinyOverworldPalette_Infernape
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sInfernapeLevelUpLearnset,
        .teachableLearnset = sInfernapeTeachableLearnset,
        .formSpeciesIdTable = sInfernapeFormSpeciesIdTable,
        .formChangeTable = sInfernapeFormChangeTable,
    },
#endif //P_FAMILY_CHIMCHAR
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_PIPLUP
#if P_MEGA_EVOLUTIONS
    [SPECIES_EMPOLEON_MEGA] =
    {
        .baseHP        = 84,
        .baseAttack    = 86,
        .baseDefense   = 118,
        .baseSpeed     = 80,
        .baseSpAttack  = 136,
        .baseSpDefense = 121,
        .types = MON_TYPES(TYPE_WATER, TYPE_STEEL),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 265,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 239,
    #else
        .expYield = 210,
    #endif
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_9
        .abilities = { ABILITY_TORRENT, ABILITY_NONE, ABILITY_COMPETITIVE },
    #else
        .abilities = { ABILITY_TORRENT, ABILITY_NONE, ABILITY_DEFIANT },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Empoleon"),
        .cryId = CRY_EMPOLEON,
        .natDexNum = NATIONAL_DEX_EMPOLEON,
        .categoryName = _("Emperor"),
        .height = 17,
        .weight = 845,
        .description = COMPOUND_STRING(
            "The three horns that extend from its\n"
            "beak attest to its power. It avoids\n"
            "unnecessary disputes, but it will decimate\n"
            "anything that threatens its pride."),
        .pokemonScale = 259,
        .pokemonOffset = 0,
        .trainerScale = 290,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_EmpoleonMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_EmpoleonMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_EmpoleonMega,
        .shinyPalette = gMonShinyPalette_EmpoleonMega,
        .iconSprite = gMonIcon_EmpoleonMega,
        .iconPalIndex = 0,
        SHADOW(2, 12, SHADOW_SIZE_M)
        FOOTPRINT(Empoleon)
        OVERWORLD(
            sPicTable_Empoleon,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Empoleon,
            gShinyOverworldPalette_Empoleon
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sEmpoleonLevelUpLearnset,
        .teachableLearnset = sEmpoleonTeachableLearnset,
        .formSpeciesIdTable = sEmpoleonFormSpeciesIdTable,
        .formChangeTable = sEmpoleonFormChangeTable,
    },
#endif //P_FAMILY_PIPLUP
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_SNIVY
#if P_MEGA_EVOLUTIONS
    [SPECIES_SERPERIOR_MEGA] =
    {
        .baseHP        = 85,
        .baseAttack    = 105,
        .baseDefense   = 95,
        .baseSpeed     = 133,
        .baseSpAttack  = 105,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_GRASS),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 264 : 238,
        .evYield_Speed = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD, EGG_GROUP_GRASS),
        .abilities = { ABILITY_OVERGROW, ABILITY_NONE, ABILITY_CONTRARY },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Serperior"),
        .cryId = CRY_SERPERIOR,
        .natDexNum = NATIONAL_DEX_SERPERIOR,
        .categoryName = _("Regal"),
        .height = 33,
        .weight = 630,
        .description = COMPOUND_STRING(
            "They raise their heads to intimidate foes,\n"
            "but only give it their all when fighting\n"
            "powerful opponents who are not fazed\n"
            "by the glare from its noble eyes."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 405,
        .trainerOffset = 8,
        .frontPic = gMonFrontPic_SerperiorMega,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_CIRCULAR_STRETCH_TWICE,
        .backPic = gMonBackPic_SerperiorMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_STRETCH,
        .palette = gMonPalette_SerperiorMega,
        .shinyPalette = gMonShinyPalette_SerperiorMega,
        .iconSprite = gMonIcon_SerperiorMega,
        .iconPalIndex = 1,
        SHADOW(2, 8, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Serperior)
        OVERWORLD(
            sPicTable_Serperior,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Serperior,
            gShinyOverworldPalette_Serperior
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sSerperiorLevelUpLearnset,
        .teachableLearnset = sSerperiorTeachableLearnset,
        .formSpeciesIdTable = sSerperiorFormSpeciesIdTable,
        .formChangeTable = sSerperiorFormChangeTable,
    },
#endif //P_FAMILY_SNIVY
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_TEPIG
#if P_MEGA_EVOLUTIONS
    [SPECIES_EMBOAR_MEGA] =
    {
        .baseHP        = 130,
        .baseAttack    = 143,
        .baseDefense   = 95,
        .baseSpeed     = 65,
        .baseSpAttack  = 100,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_FIRE, TYPE_FIGHTING),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 264 : 238,
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_BLAZE, ABILITY_NONE, ABILITY_RECKLESS },
        .bodyColor = BODY_COLOR_RED,
        .noFlip = TRUE,
        .speciesName = _("Emboar"),
        .cryId = CRY_EMBOAR,
        .natDexNum = NATIONAL_DEX_EMBOAR,
        .categoryName = _("Fire Pig"),
        .height = 16,
        .weight = 1500,
        .description = COMPOUND_STRING(
            "It is adept at using many different moves.\n"
            "It can throw a fire punch by setting its\n"
            "fists on fire with its fiery chin.\n"
            "It cares deeply about its friends."),
        .pokemonScale = 259,
        .pokemonOffset = 1,
        .trainerScale = 296,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_EmboarMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_DEEP_V_SQUISH_AND_BOUNCE,
        .backPic = gMonBackPic_EmboarMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_EmboarMega,
        .shinyPalette = gMonShinyPalette_EmboarMega,
        .iconSprite = gMonIcon_EmboarMega,
        .iconPalIndex = 0,
        SHADOW(-1, 12, SHADOW_SIZE_L)
        FOOTPRINT(Emboar)
        OVERWORLD(
            sPicTable_Emboar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Emboar,
            gShinyOverworldPalette_Emboar
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sEmboarLevelUpLearnset,
        .teachableLearnset = sEmboarTeachableLearnset,
        .formSpeciesIdTable = sEmboarFormSpeciesIdTable,
        .formChangeTable = sEmboarFormChangeTable,
    },
#endif //P_FAMILY_TEPIG
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_OSHAWOTT
#if P_MEGA_EVOLUTIONS
    [SPECIES_SAMUROTT_MEGA] =
    {
        .baseHP        = 125,
        .baseAttack    = 130,
        .baseDefense   = 85,
        .baseSpeed     = 70,
        .baseSpAttack  = 138,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_8) ? 264 : 238,
        .evYield_SpAttack = 3,
        .genderRatio = PERCENT_FEMALE(12.5),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_TORRENT, ABILITY_NONE, ABILITY_SHELL_ARMOR },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Samurott"),
        .cryId = CRY_SAMUROTT,
        .natDexNum = NATIONAL_DEX_SAMUROTT,
        .categoryName = _("Formidable"),
        .height = 15,
        .weight = 946,
        .description = COMPOUND_STRING(
            "One swing of the seamitars attached\n"
            "to its armor can fell an opponent.\n"
            "A simple glare from one of them is\n"
            "enough to intimidate most enemies."),
        .pokemonScale = 268,
        .pokemonOffset = 2,
        .trainerScale = 271,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_SamurottMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_SamurottMega,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 6,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_SamurottMega,
        .shinyPalette = gMonShinyPalette_SamurottMega,
        .iconSprite = gMonIcon_SamurottMega,
        .iconPalIndex = 3,
        SHADOW(-2, 13, SHADOW_SIZE_L)
        FOOTPRINT(Samurott)
        OVERWORLD(
            sPicTable_Samurott,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Samurott,
            gShinyOverworldPalette_Samurott
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sSamurottLevelUpLearnset,
        .teachableLearnset = sSamurottTeachableLearnset,
        .formSpeciesIdTable = sSamurottFormSpeciesIdTable,
        .formChangeTable = sSamurottFormChangeTable,
    },
#endif //P_FAMILY_OSHAWOTT
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_SLUGMA
    [SPECIES_ESCARGINITE] =
    {
        .baseHP        = 100,
        .baseAttack    = 50,
        .baseDefense   = 140,
        .baseSpeed     = 35,
        .baseSpAttack  = 100,
        .baseSpDefense = 120,
        .types = MON_TYPES(TYPE_FIRE, TYPE_ROCK),
        .catchRate = 75,
    #if P_UPDATED_EXP_YIELDS >= GEN_7
        .expYield = 151,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 144,
    #else
        .expYield = 154,
    #endif
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_AMORPHOUS),
        .abilities = { ABILITY_MAGMA_ARMOR, ABILITY_WATER_ABSORB, ABILITY_WEAK_ARMOR },
        .bodyColor = BODY_COLOR_RED,
        .noFlip = TRUE,
        .speciesName = _("Escarginite"),
        .cryId = CRY_MAGCARGO,
        .natDexNum = NATIONAL_DEX_ESCARGINITE,
        .categoryName = _("Lava"),
        .height = 8,
        .weight = 550,
        .description = COMPOUND_STRING(
            "The shell on its back is made of hardened\n"
            "magma. Tens of thousands of years spent\n"
            "living in volcanic craters have turned\n"
            "Magcargo's bodies into magma."),
        .pokemonScale = 332,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Escarginite,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(40, 56) : MON_COORDS_SIZE(56, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 13 : 6,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_Escarginite,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 48) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_Escarginite,
        .shinyPalette = gMonShinyPalette_Escarginite,
        .iconSprite = gMonIcon_Escarginite,
        .iconPalIndex = 0,
        SHADOW(0, 5, SHADOW_SIZE_M)
        FOOTPRINT(Magcargo)
        OVERWORLD(
            sPicTable_Magcargo,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SLITHER,
            sAnimTable_Following,
            gOverworldPalette_Magcargo,
            gShinyOverworldPalette_Magcargo
        )
        .levelUpLearnset = sMagcargoLevelUpLearnset,
        .teachableLearnset = sMagcargoTeachableLearnset,
    },
#endif //P_FAMILY_SLUGMA

    [SPECIES_SNORUNT_HOENNIAN] =
    {
        .baseHP        = 50,
        .baseAttack    = 50,
        .baseDefense   = 50,
        .baseSpeed     = 50,
        .baseSpAttack  = 50,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_DARK),
        .catchRate = 190,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 60 : 74,
        .evYield_HP = 1,
        .itemRare = ITEM_SNOWBALL,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FAIRY, EGG_GROUP_MINERAL),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_INNER_FOCUS, ABILITY_ICE_BODY, ABILITY_MOODY },
    #else
        .abilities = { ABILITY_INNER_FOCUS, ABILITY_NONE, ABILITY_MOODY },
    #endif
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Snorunt"),
        .cryId = CRY_SNORUNT,
        .natDexNum = NATIONAL_DEX_SNORUNT_HOENNIAN,
        .categoryName = _("Horn Hat"),
        .height = 7,
        .weight = 168,
        .description = COMPOUND_STRING(
            "They tend to move about in groups of\n"
            "around five Snorunt. In snowy regions,\n"
            "it is said that when they are seen late at\n"
            "night, snowfall will arrive by morning."),
        .pokemonScale = 380,
        .pokemonOffset = 15,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_SnoruntH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(40, 48) : MON_COORDS_SIZE(32, 40),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 11 : 12,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE_SLOW,
        .frontAnimDelay = 20,
        .backPic = gMonBackPic_SnoruntH,
        .backPicSize = MON_COORDS_SIZE(56, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_SnoruntH,
        .shinyPalette = gMonShinyPalette_SnoruntH,
        .iconSprite = gMonIcon_SnoruntH,
        .iconPalIndex = 2,
        SHADOW(1, 2, SHADOW_SIZE_S)
        FOOTPRINT(Snorunt)
        OVERWORLD(
            sPicTable_Snorunt,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Snorunt,
            gShinyOverworldPalette_Snorunt
        )
        .levelUpLearnset = sImpidimpLevelUpLearnset,
        .teachableLearnset = sImpidimpTeachableLearnset,
        .eggMoveLearnset = sSnoruntEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 42, SPECIES_GLALIE_HOENNIAN},
                                {EVO_ITEM, ITEM_DAWN_STONE, SPECIES_FROSLASS_HOENNIAN}),
    },

    [SPECIES_GLALIE_HOENNIAN] =
    {
        .baseHP        = 90,
        .baseAttack    = 90,
        .baseDefense   = 90,
        .baseSpeed     = 90,
        .baseSpAttack  = 90,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_DARK, TYPE_FIGHTING),
        .catchRate = 75,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 168 : 187,
        .evYield_HP = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FAIRY, EGG_GROUP_MINERAL),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_INNER_FOCUS, ABILITY_ICE_BODY, ABILITY_MOODY },
    #else
        .abilities = { ABILITY_INNER_FOCUS, ABILITY_NONE, ABILITY_MOODY },
    #endif
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Glalie"),
        .cryId = CRY_GLALIE,
        .natDexNum = NATIONAL_DEX_GLALIE_HOENNIAN,
        .categoryName = _("Face"),
        .height = 15,
        .weight = 2565,
        .description = COMPOUND_STRING(
            "A Glalie has the power to instantaneously\n"
            "freeze moisture in the atmosphere.\n"
            "A dazzling cloud of diamondlike ice\n"
            "crystals forms around its body."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 344,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_GlalieH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 48) : MON_COORDS_SIZE(56, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 10 : 8,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_ZIGZAG_FAST,
        .enemyMonElevation = 12,
        .backPic = gMonBackPic_GlalieH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 40) : MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_TRIANGLE_DOWN,
        .palette = gMonPalette_GlalieH,
        .shinyPalette = gMonShinyPalette_GlalieH,
        .iconSprite = gMonIcon_GlalieH,
        .iconPalIndex = 0,
        SHADOW(1, 15, SHADOW_SIZE_M)
        FOOTPRINT(Glalie)
        OVERWORLD(
            sPicTable_Glalie,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Glalie,
            gShinyOverworldPalette_Glalie
        )
        .levelUpLearnset = sPangoroLevelUpLearnset,
        .teachableLearnset = sPangoroTeachableLearnset,
    },

    [SPECIES_FROSLASS_HOENNIAN] =
    {
        .baseHP        = 70,
        .baseAttack    = 80,
        .baseDefense   = 70,
        .baseSpeed     = 110,
        .baseSpAttack  = 80,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_DARK, TYPE_GHOST),
        .catchRate = 75,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 168 : 187,
        .evYield_Speed = 2,
        .genderRatio = MON_FEMALE,
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FAIRY, EGG_GROUP_MINERAL),
        .abilities = { ABILITY_SNOW_CLOAK, ABILITY_NONE, ABILITY_CURSED_BODY },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Froslass"),
        .cryId = CRY_FROSLASS,
        .natDexNum = NATIONAL_DEX_FROSLASS_HOENNIAN,
        .categoryName = _("Snow Oni"),
        .height = 13,
        .weight = 266,
        .description = COMPOUND_STRING(
            "When it finds people or Pokémon it likes,\n"
            "it freezes them and takes them to its\n"
            "chilly den, where they become its\n"
            "decorations."),
        .pokemonScale = 272,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_FroslassH,
        .frontPicSize = MON_COORDS_SIZE(48, 48),
        .frontPicYOffset = 8,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SLIDE_WOBBLE,
        .enemyMonElevation = 7,
        .backPic = gMonBackPic_FroslassH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_CONVEX_DOUBLE_ARC,
        .palette = gMonPalette_FroslassH,
        .shinyPalette = gMonShinyPalette_FroslassH,
        .iconSprite = gMonIcon_FroslassH,
        .iconPalIndex = 0,
        SHADOW(-1, 10, SHADOW_SIZE_S)
        FOOTPRINT(Froslass)
        OVERWORLD(
            sPicTable_Froslass,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Froslass,
            gShinyOverworldPalette_Froslass
        )
        .levelUpLearnset = sSpiritombLevelUpLearnset,
        .teachableLearnset = sSpiritombTeachableLearnset,
    },

    [SPECIES_GLIGAR_HOENNIAN] =
    {
        .baseHP        = 65,
        .baseAttack    = 75,
        .baseDefense   = 105,
        .baseSpeed     = 85,
        .baseSpAttack  = 35,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_POISON, TYPE_FIRE),
        .catchRate = 60,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 86 : 108,
        .evYield_Defense = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_HYPER_CUTTER, ABILITY_SAND_VEIL, ABILITY_IMMUNITY },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Gligar"),
        .cryId = CRY_GLIGAR,
        .natDexNum = NATIONAL_DEX_GLIGAR_HOENNIAN,
        .categoryName = _("Fly Scorpion"),
        .height = 11,
        .weight = 648,
        .description = COMPOUND_STRING(
            "It glides without making a single sound.\n"
            "It grasps the face of its foe using its\n"
            "hind and large front claws, then stabs\n"
            "with its poison barb."),
        .pokemonScale = 350,
        .pokemonOffset = -1,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_GligarH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 64) : MON_COORDS_SIZE(56, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 3 : 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_SHRINK_GROW,
        .enemyMonElevation = P_GBA_STYLE_SPECIES_GFX ? 6 : 8,
        .backPic = gMonBackPic_GligarH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 56) : MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHRINK_GROW,
        .palette = gMonPalette_GligarH,
        .shinyPalette = gMonShinyPalette_GligarH,
        .iconSprite = gMonIcon_GligarH,
        .iconPalIndex = 0,
        SHADOW(0, 15, SHADOW_SIZE_S)
        FOOTPRINT(Gligar)
        OVERWORLD(
            sPicTable_Gligar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_NONE,
            sAnimTable_Following,
            gOverworldPalette_Gligar,
            gShinyOverworldPalette_Gligar
        )
        .levelUpLearnset = sSalanditLevelUpLearnset,
        .teachableLearnset = sSalanditTeachableLearnset,
        .eggMoveLearnset = sSalanditEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_ITEM_HOLD_NIGHT, ITEM_RAZOR_FANG, SPECIES_GLISCOR_HOENNIAN},
                                {EVO_ITEM, ITEM_RAZOR_FANG, SPECIES_GLISCOR_HOENNIAN}),
    },

    [SPECIES_GLISCOR_HOENNIAN] =
    {
        .baseHP        = 85,
        .baseAttack    = 65,
        .baseDefense   = 125,
        .baseSpeed     = 105,
        .baseSpAttack  = 105,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_POISON, TYPE_FIRE),
        .catchRate = 30,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 179 : 192,
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_HYPER_CUTTER, ABILITY_SAND_VEIL, ABILITY_POISON_HEAL },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Gliscor"),
        .cryId = CRY_GLISCOR,
        .natDexNum = NATIONAL_DEX_GLISCOR_HOENNIAN,
        .categoryName = _("Poison Scorp"),
        .height = 20,
        .weight = 425,
        .description = COMPOUND_STRING(
            "Gliscor observes prey while hanging\n"
            "upside down from branches. Its flight is\n"
            "soundless. When the chance presents\n"
            "itself, it swoops!"),
        .pokemonScale = 261,
        .pokemonOffset = 1,
        .trainerScale = 334,
        .trainerOffset = 4,
        .frontPic = gMonFrontPic_GliscorH,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_SWING_CONVEX,
        .enemyMonElevation = 9,
        .backPic = gMonBackPic_GliscorH,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_STRETCH,
        .palette = gMonPalette_GliscorH,
        .shinyPalette = gMonShinyPalette_GliscorH,
        .iconSprite = gMonIcon_GliscorH,
        .iconPalIndex = 0,
        SHADOW(-1, 13, SHADOW_SIZE_M)
        FOOTPRINT(Gliscor)
        OVERWORLD(
            sPicTable_Gliscor,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Gliscor,
            gShinyOverworldPalette_Gliscor
        )
        .levelUpLearnset = sSalazzleLevelUpLearnset,
        .teachableLearnset = sSalazzleTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 48, SPECIES_MAGNESCOR}),
    },

    [SPECIES_MAGNESCOR] =
    {
        .baseHP        = 95,
        .baseAttack    = 65,
        .baseDefense   = 145,
        .baseSpeed     = 125,
        .baseSpAttack  = 125,
        .baseSpDefense = 75,
        .types = MON_TYPES(TYPE_POISON, TYPE_FIRE),
        .catchRate = 30,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 179 : 192,
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_HYPER_CUTTER, ABILITY_SAND_VEIL, ABILITY_POISON_HEAL },
        .bodyColor = BODY_COLOR_PURPLE,
        .speciesName = _("Magnescor"),
        .cryId = CRY_GLISCOR,
        .natDexNum = NATIONAL_DEX_MAGNESCOR,
        .categoryName = _("Poison Scorp"),
        .height = 20,
        .weight = 425,
        .description = COMPOUND_STRING(
            "Magnescor observes prey while hanging\n"
            "upside down from branches. Its flight is\n"
            "soundless. When the chance presents\n"
            "itself, it swoops!"),
        .pokemonScale = 261,
        .pokemonOffset = 1,
        .trainerScale = 334,
        .trainerOffset = 4,
        .frontPic = gMonFrontPic_Magnescor,
        .frontPicSize = MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_SWING_CONVEX,
        .enemyMonElevation = 9,
        .backPic = gMonBackPic_Magnescor,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_STRETCH,
        .palette = gMonPalette_Magnescor,
        .shinyPalette = gMonShinyPalette_Magnescor,
        .iconSprite = gMonIcon_Magnescor,
        .iconPalIndex = 0,
        SHADOW(-1, 13, SHADOW_SIZE_M)
        FOOTPRINT(Gliscor)
        OVERWORLD(
            sPicTable_Gliscor,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Gliscor,
            gShinyOverworldPalette_Gliscor
        )
        .levelUpLearnset = sSalazzleLevelUpLearnset,
        .teachableLearnset = sSalazzleTeachableLearnset,
    },

#if P_FAMILY_HORSEA
#if P_MEGA_EVOLUTIONS
    [SPECIES_KINGDRA_MEGA_X] =
    {
        .baseHP        = 75,
        .baseAttack    = 105,
        .baseDefense   = 100,
        .baseSpeed     = 115,
        .baseSpAttack  = 145,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_WATER, TYPE_DRAGON),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 270,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 243,
    #else
        .expYield = 207,
    #endif
        .evYield_Attack = 1,
        .evYield_SpAttack = 1,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_DRAGON_SCALE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_SNIPER, ABILITY_DAMP },
    #else
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_NONE, ABILITY_DAMP },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Kingdra"),
        .cryId = CRY_KINGDRA,
        .natDexNum = NATIONAL_DEX_KINGDRA,
        .categoryName = _("Dragon"),
        .height = 18,
        .weight = 1520,
        .description = COMPOUND_STRING(
            "It sleeps quietly, deep on the seafloor.\n"
            "When it comes up to the surface, it\n"
            "creates a huge whirlpool that can swallow\n"
            "even ships."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 287,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_KingdraMegaX,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 64) : MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 4 : 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_CIRCLE_INTO_BG,
        .backPic = gMonBackPic_KingdraMegaX,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_KingdraMegaX,
        .shinyPalette = gMonShinyPalette_KingdraMegaX,
        .iconSprite = gMonIcon_KingdraMegaX,
        .iconPalIndex = 0,
        SHADOW(3, 12, SHADOW_SIZE_M)
        FOOTPRINT(Kingdra)
        OVERWORLD(
            sPicTable_Kingdra,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Kingdra,
            gShinyOverworldPalette_Kingdra
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sKingdraLevelUpLearnset,
        .teachableLearnset = sKingdraTeachableLearnset,
        .formSpeciesIdTable = sKingdraFormSpeciesIdTable,
        .formChangeTable = sKingdraFormChangeTable,
    },
#endif //P_FAMILY_HORSEA
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_HORSEA
#if P_MEGA_EVOLUTIONS
    [SPECIES_KINGDRA_MEGA_Y] =
    {
        .baseHP        = 75,
        .baseAttack    = 105,
        .baseDefense   = 95,
        .baseSpeed     = 145,
        .baseSpAttack  = 135,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_WATER, TYPE_DRAGON),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 270,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 243,
    #else
        .expYield = 207,
    #endif
        .evYield_Attack = 1,
        .evYield_SpAttack = 1,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_DRAGON_SCALE,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_1, EGG_GROUP_DRAGON),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_SNIPER, ABILITY_DAMP },
    #else
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_NONE, ABILITY_DAMP },
    #endif
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Kingdra"),
        .cryId = CRY_KINGDRA,
        .natDexNum = NATIONAL_DEX_KINGDRA,
        .categoryName = _("Dragon"),
        .height = 18,
        .weight = 1520,
        .description = COMPOUND_STRING(
            "It sleeps quietly, deep on the seafloor.\n"
            "When it comes up to the surface, it\n"
            "creates a huge whirlpool that can swallow\n"
            "even ships."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 287,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_KingdraMegaY,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 64) : MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 4 : 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_CIRCLE_INTO_BG,
        .backPic = gMonBackPic_KingdraMegaY,
        .backPicSize = MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_BLUE,
        .palette = gMonPalette_KingdraMegaY,
        .shinyPalette = gMonShinyPalette_KingdraMegaY,
        .iconSprite = gMonIcon_KingdraMegaY,
        .iconPalIndex = 0,
        SHADOW(3, 12, SHADOW_SIZE_M)
        FOOTPRINT(Kingdra)
        OVERWORLD(
            sPicTable_Kingdra,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Kingdra,
            gShinyOverworldPalette_Kingdra
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sKingdraLevelUpLearnset,
        .teachableLearnset = sKingdraTeachableLearnset,
        .formSpeciesIdTable = sKingdraFormSpeciesIdTable,
        .formChangeTable = sKingdraFormChangeTable,
    },
#endif //P_FAMILY_HORSEA
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_SHINX
#if P_MEGA_EVOLUTIONS
    [SPECIES_LUXRAY_MEGA] =
    {
        .baseHP        = 90,
        .baseAttack    = 140,
        .baseDefense   = 89,
        .baseSpeed     = 80,
        .baseSpAttack  = 105,
        .baseSpDefense = 89,
        .types = MON_TYPES(TYPE_ELECTRIC, TYPE_DARK),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 262,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 235,
    #else
        .expYield = 194,
    #endif
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_RIVALRY, ABILITY_INTIMIDATE, ABILITY_GUTS },
        .bodyColor = BODY_COLOR_BLUE,
        .speciesName = _("Luxray"),
        .cryId = CRY_LUXRAY,
        .natDexNum = NATIONAL_DEX_LUXRAY,
        .categoryName = _("Gleam Eyes"),
        .height = 14,
        .weight = 420,
        .description = COMPOUND_STRING(
            "It has eyes which can see through\n"
            "anything. Luxray's ability to see\n"
            "through objects comes in handy when\n"
            "it's scouting for danger."),
        .pokemonScale = 265,
        .pokemonOffset = 2,
        .trainerScale = 262,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_LuxrayMega,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_GLOW_YELLOW,
        .backPic = gMonBackPic_LuxrayMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_SHRINK_GROW_VIBRATE,
        .palette = gMonPalette_LuxrayMega,
        .shinyPalette = gMonShinyPalette_LuxrayMega,
        .iconSprite = gMonIcon_LuxrayMega,
        .iconPalIndex = 0,
        SHADOW(-1, 10, SHADOW_SIZE_L)
        FOOTPRINT(Luxray)
        OVERWORLD(
            sPicTable_Luxray,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Luxray,
            gShinyOverworldPalette_Luxray
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sLuxrayLevelUpLearnset,
        .teachableLearnset = sLuxrayTeachableLearnset,
        .formSpeciesIdTable = sLuxrayFormSpeciesIdTable,
        .formChangeTable = sLuxrayFormChangeTable,
    },
#endif //P_FAMILY_SHINX
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_SANDSHREW
#if P_MEGA_EVOLUTIONS
    [SPECIES_SANDSLASH_MEGA] =
    {
        .baseHP        = 95,
        .baseAttack    = 150,
        .baseDefense   = 150,
        .baseSpeed     = 65,
        .baseSpAttack  = 45,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_GROUND),
        .catchRate = 90,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 158 : 163,
        .evYield_Defense = 2,
        .itemRare = ITEM_GRIP_CLAW,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_SAND_VEIL, ABILITY_NONE, ABILITY_SAND_RUSH },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Sandslash"),
        .cryId = CRY_SANDSLASH,
        .natDexNum = NATIONAL_DEX_SANDSLASH,
        .categoryName = _("Mouse"),
        .height = 10,
        .weight = 295,
        .description = COMPOUND_STRING(
            "It curls up in a ball to protect itself from\n"
            "enemy attacks. It also curls up to prevent\n"
            "heatstroke during the daytime when\n"
            "temperatures rise sharply."),
        .pokemonScale = 341,
        .pokemonOffset = 11,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_SandslashMega,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 48) : MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 9 : 6,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_V_STRETCH : ANIM_H_SHAKE,
        .backPic = gMonBackPic_SandslashMega,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 9 : 11,
        .backAnimId = BACK_ANIM_CONCAVE_ARC_LARGE,
        .palette = gMonPalette_SandslashMega,
        .shinyPalette = gMonShinyPalette_SandslashMega,
        .iconSprite = gMonIcon_SandslashMega,
        .iconPalIndex = 2,
        SHADOW(4, 4, SHADOW_SIZE_L)
        FOOTPRINT(Sandslash)
        OVERWORLD(
            sPicTable_Sandslash,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Sandslash,
            gShinyOverworldPalette_Sandslash
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sSandslashLevelUpLearnset,
        .teachableLearnset = sSandslashTeachableLearnset,
        .formSpeciesIdTable = sSandslashFormSpeciesIdTable,
        .formChangeTable = sSandslashFormChangeTable,
    },
#endif //P_FAMILY_SANDSHREW
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_SLAKOTH
#if P_MEGA_EVOLUTIONS
    [SPECIES_SLAKING_MEGA] =
    {
        .baseHP        = 170,
        .baseAttack    = 180,
        .baseDefense   = 120,
        .baseSpeed     = 120,
        .baseSpAttack  = 95,
        .baseSpDefense = 65,
        .types = MON_TYPES(TYPE_NORMAL),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 285,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 252,
    #else
        .expYield = 210,
    #endif
        .evYield_HP = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Slaking"),
        .cryId = CRY_SLAKING,
        .natDexNum = NATIONAL_DEX_SLAKING,
        .categoryName = _("Lazy"),
        .height = 20,
        .weight = 1305,
        .description = COMPOUND_STRING(
            "Hordes of Slaking gather around trees\n"
            "when fruits come into season. They wait\n"
            "around patiently for ripened fruits to fall\n"
            "out of the trees."),
        .pokemonScale = 256,
        .pokemonOffset = 2,
        .trainerScale = 300,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_SlakingMega,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 48) : MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SQUISH_AND_BOUNCE_SLOW,
        .backPic = gMonBackPic_SlakingMega,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 48) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SHAKE,
        .palette = gMonPalette_SlakingMega,
        .shinyPalette = gMonShinyPalette_SlakingMega,
        .iconSprite = gMonIcon_SlakingMega,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 1 : 2,
        SHADOW(0, 6, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Slaking)
        OVERWORLD(
            sPicTable_Slaking,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Slaking,
            gShinyOverworldPalette_Slaking
        )
        .isMegaEvolution = TRUE,
        .levelUpLearnset = sSlakingLevelUpLearnset,
        .teachableLearnset = sSlakingTeachableLearnset,
        .formSpeciesIdTable = sSlakingFormSpeciesIdTable,
        .formChangeTable = sSlakingFormChangeTable,
    },
#endif //P_FAMILY_SLAKOTH
#endif //P_MEGA_EVOLUTIONS

#if P_FAMILY_SNEASEL
#if P_MEGA_EVOLUTIONS
    [SPECIES_WEAVILE_MEGA] =
    {
        .baseHP        = 70,
        .baseAttack    = 150,
        .baseDefense   = 65,
        .baseSpeed     = 145,
        .baseSpAttack  = 45,
        .baseSpDefense = 85,
        .types = MON_TYPES(TYPE_DARK, TYPE_ICE),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 179 : 199,
        .evYield_Attack = 1,
        .evYield_Speed = 1,
        .itemRare = ITEM_QUICK_CLAW,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = 35,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_PICKPOCKET },
        .bodyColor = BODY_COLOR_BLACK,
        .speciesName = _("Weavile"),
        .cryId = CRY_WEAVILE,
        .natDexNum = NATIONAL_DEX_WEAVILE,
        .categoryName = _("Sharp Claw"),
        .height = 11,
        .weight = 340,
        .description = COMPOUND_STRING(
            "They travel in groups of four or five,\n"
            "leaving signs for one another on trees\n"
            "and rocks. They bring down their prey\n"
            "with coordinated attacks."),
        .pokemonScale = 320,
        .pokemonOffset = 7,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_WeavileMega,
        .frontPicSize = MON_COORDS_SIZE(56, 56),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_VIBRATE,
        .backPic = gMonBackPic_WeavileMega,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 2,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_WeavileMega,
        .shinyPalette = gMonShinyPalette_WeavileMega,
        .iconSprite = gMonIcon_WeavileMega,
        .iconPalIndex = 0,
        SHADOW(-4, 10, SHADOW_SIZE_M)
        FOOTPRINT(Weavile)
        OVERWORLD(
            sPicTable_Weavile,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Weavile,
            gShinyOverworldPalette_Weavile
        )
        .levelUpLearnset = sWeavileLevelUpLearnset,
        .teachableLearnset = sWeavileTeachableLearnset,
        .formSpeciesIdTable = sWeavileFormSpeciesIdTable,
        .formChangeTable = sWeavileFormChangeTable,
    },
#endif //P_FAMILY_SNEASEL
#endif //P_MEGA_EVOLUTIONS

    [SPECIES_LARVITAR_HOENNIAN] =
    {
        .baseHP        = 50,
        .baseAttack    = 64,
        .baseDefense   = 50,
        .baseSpeed     = 41,
        .baseSpAttack  = 45,
        .baseSpDefense = 50,
        .types = MON_TYPES(TYPE_ROCK, TYPE_FIRE),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 60 : 67,
        .evYield_Attack = 1,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER),
        .abilities = { ABILITY_GUTS, ABILITY_NONE, ABILITY_SAND_VEIL },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Larvitar"),
        .cryId = CRY_LARVITAR,
        .natDexNum = NATIONAL_DEX_LARVITAR_HOENNIAN,
        .categoryName = _("Rock Skin"),
        .height = 6,
        .weight = 720,
        .description = COMPOUND_STRING(
            "A Larvitar is born deep under the ground.\n"
            "It must eat its way through the soil above\n"
            "and reach the surface for it to see its\n"
            "parents' faces."),
        .pokemonScale = 472,
        .pokemonOffset = 18,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_LarvitarH,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(32, 48) : MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 9 : 10,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_JUMPS_SMALL,
        .backPic = gMonBackPic_LarvitarH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(48, 48) : MON_COORDS_SIZE(56, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE_LOW,
        .palette = gMonPalette_LarvitarH,
        .shinyPalette = gMonShinyPalette_LarvitarH,
        .iconSprite = gMonIcon_LarvitarH,
        .iconPalIndex = 0,
        SHADOW(0, 3, SHADOW_SIZE_S)
        FOOTPRINT(Larvitar)
        OVERWORLD(
            sPicTable_Larvitar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Larvitar,
            gShinyOverworldPalette_Larvitar
        )
        .levelUpLearnset = sMagcargoLevelUpLearnset,
        .teachableLearnset = sMagcargoTeachableLearnset,
        .eggMoveLearnset = sSlugmaEggMoveLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 30, SPECIES_PUPITAR_HOENNIAN}),
    },

    [SPECIES_PUPITAR_HOENNIAN] =
    {
        .baseHP        = 70,
        .baseAttack    = 84,
        .baseDefense   = 70,
        .baseSpeed     = 51,
        .baseSpAttack  = 65,
        .baseSpDefense = 70,
        .types = MON_TYPES(TYPE_ROCK, TYPE_FIRE),
        .catchRate = 45,
        .expYield = 144,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER),
        .abilities = { ABILITY_SHED_SKIN, ABILITY_NONE, ABILITY_NONE },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Pupitar"),
        .cryId = CRY_PUPITAR,
        .natDexNum = NATIONAL_DEX_PUPITAR_HOENNIAN,
        .categoryName = _("Hard Shell"),
        .height = 12,
        .weight = 1520,
        .description = COMPOUND_STRING(
            "A Pupitar creates a gas inside its body\n"
            "that it ejects under compression to propel\n"
            "itself like a jet. Its body can withstand\n"
            "a collision with solid steel."),
        .pokemonScale = 292,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_PupitarH,
        .frontPicSize = MON_COORDS_SIZE(40, 48),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 9 : 11,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_V_SHAKE : ANIM_H_SHAKE,
        .backPic = gMonBackPic_PupitarH,
        .backPicSize = MON_COORDS_SIZE(48, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE,
        .palette = gMonPalette_PupitarH,
        .shinyPalette = gMonShinyPalette_PupitarH,
        .iconSprite = gMonIcon_PupitarH,
        .iconPalIndex = P_GBA_STYLE_SPECIES_ICONS ? 0 : 2,
        SHADOW(3, 3, SHADOW_SIZE_S)
        FOOTPRINT(Pupitar)
        OVERWORLD(
            sPicTable_Pupitar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Pupitar,
            gShinyOverworldPalette_Pupitar
        )
        .levelUpLearnset = sMagcargoLevelUpLearnset,
        .teachableLearnset = sMagcargoTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 55, SPECIES_TYRANITAR_HOENNIAN}),
    },

    [SPECIES_TYRANITAR_HOENNIAN] =
    {
        .baseHP        = 100,
        .baseAttack    = 144,
        .baseDefense   = 110,
        .baseSpeed     = 61,
        .baseSpAttack  = 115,
        .baseSpDefense = 100,
        .types = MON_TYPES(TYPE_ROCK, TYPE_FIRE),
        .catchRate = 45,
    #if P_UPDATED_EXP_YIELDS >= GEN_8
        .expYield = 300,
    #elif P_UPDATED_EXP_YIELDS >= GEN_5
        .expYield = 270,
    #else
        .expYield = 218,
    #endif
        .evYield_Attack = 3,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 40,
        .friendship = 35,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER),
        .abilities = { ABILITY_SAND_STREAM, ABILITY_NONE, ABILITY_UNNERVE },
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Tyranitar"),
        .cryId = CRY_TYRANITAR,
        .natDexNum = NATIONAL_DEX_TYRANITAR_HOENNIAN,
        .categoryName = _("Armor"),
        .height = 20,
        .weight = 2020,
        .description = COMPOUND_STRING(
            "A Tyranitar is so overwhelmingly powerful,\n"
            "it can bring down a whole mountain to make\n"
            "its nest. It roams in mountains seeking\n"
            "new opponents to fight."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 345,
        .trainerOffset = 7,
        .frontPic = gMonFrontPic_TyranitarH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_H_SHAKE : ANIM_V_SHAKE,
        .frontAnimDelay = 10,
        .backPic = gMonBackPic_TyranitarH,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_V_SHAKE_LOW,
        .palette = gMonPalette_TyranitarH,
        .shinyPalette = gMonShinyPalette_TyranitarH,
        .iconSprite = gMonIcon_TyranitarH,
        .iconPalIndex = 1,
        SHADOW(-1, 11, SHADOW_SIZE_L)
        FOOTPRINT(Tyranitar)
        OVERWORLD(
            sPicTable_Tyranitar,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Tyranitar,
            gShinyOverworldPalette_Tyranitar
        )
        .levelUpLearnset = sCoalossalLevelUpLearnset,
        .teachableLearnset = sCoalossalTeachableLearnset,
    },

#if P_FAMILY_GOLDEEN
    [SPECIES_SEALORD] =
    {
        .baseHP        = 100,
        .baseAttack    = 122,
        .baseDefense   = 75,
        .baseSpeed     = 68,
        .baseSpAttack  = 65,
        .baseSpDefense = 10,
        .types = MON_TYPES(TYPE_WATER),
        .catchRate = 60,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 158 : 170,
        .evYield_Attack = 2,
        .itemRare = ITEM_MYSTIC_WATER,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_WATER_2),
        .abilities = { ABILITY_SWIFT_SWIM, ABILITY_WATER_VEIL, ABILITY_LIGHTNING_ROD },
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Sealord"),
        .cryId = CRY_SEAKING,
        .natDexNum = NATIONAL_DEX_SEALORD,
        .categoryName = _("Goldfish"),
        .height = 13,
        .weight = 390,
        .description = COMPOUND_STRING(
            "It punches holes in boulders on stream-\n"
            "beds. This is a clever innovation that\n"
            "prevents its eggs from being attacked or\n"
            "washed away by the current."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Sealord,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 56) : MON_COORDS_SIZE(64, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 4 : 5,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SLIDE_WOBBLE,
        .backPic = gMonBackPic_Sealord,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 48) : MON_COORDS_SIZE(64, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_CONVEX_DOUBLE_ARC,
        .palette = gMonPalette_Sealord,
        .shinyPalette = gMonShinyPalette_Sealord,
        .iconSprite = gMonIcon_Sealord,
        .iconPalIndex = 0,
        SHADOW(0, 8, SHADOW_SIZE_XL_BATTLE_ONLY)
        FOOTPRINT(Seaking)
        OVERWORLD(
            sPicTable_Seaking,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_SPOT,
            sAnimTable_Following,
            gOverworldPalette_Seaking,
            gShinyOverworldPalette_Seaking
        )
        .levelUpLearnset = sSeakingLevelUpLearnset,
        .teachableLearnset = sSeakingTeachableLearnset,
        .evolutions = EVOLUTION({EVO_LEVEL, 48, SPECIES_SEALORD}),
    },
#endif //P_FAMILY_GOLDEEN

#if P_FAMILY_SUDOWOODO
    [SPECIES_BOGUSPRUCE] =
    {
        .baseHP        = 110,
        .baseAttack    = 140,
        .baseDefense   = 130,
        .baseSpeed     = 30,
        .baseSpAttack  = 30,
        .baseSpDefense = 95,
        .types = MON_TYPES(TYPE_ROCK, TYPE_GRASS),
        .catchRate = 65,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 144 : 135,
        .evYield_Defense = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MINERAL),
        .abilities = { ABILITY_STURDY, ABILITY_ROCK_HEAD, ABILITY_RATTLED },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Boguspruce"),
        .cryId = CRY_SUDOWOODO,
        .natDexNum = NATIONAL_DEX_BOGUSPRUCE,
        .categoryName = _("Imitation"),
        .height = 12,
        .weight = 380,
        .description = COMPOUND_STRING(
            "It mimics a tree to avoid being attacked\n"
            "by enemies. But since its forelegs\n"
            "remain green throughout the year, it is\n"
            "easily identified as a fake in the winter."),
        .pokemonScale = 305,
        .pokemonOffset = 8,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Boguspruce,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 6 : 7,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SLIDE_SLOW,
        .backPic = gMonBackPic_Boguspruce,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(48, 48) : MON_COORDS_SIZE(48, 56),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_H_SLIDE,
        .palette = gMonPalette_Boguspruce,
        .shinyPalette = gMonShinyPalette_Boguspruce,
        .iconSprite = gMonIcon_Boguspruce,
        .iconPalIndex = 1,
        SHADOW(-2, 7, SHADOW_SIZE_S)
        FOOTPRINT(Sudowoodo)
        OVERWORLD(
            sPicTable_Sudowoodo,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Sudowoodo,
            gShinyOverworldPalette_Sudowoodo
        )
        .levelUpLearnset = sSudowoodoLevelUpLearnset,
        .teachableLearnset = sSudowoodoTeachableLearnset,
        .eggMoveLearnset = sSudowoodoEggMoveLearnset,
    },
#endif //P_FAMILY_SUDOWOODO

    [SPECIES_SKARMORY_HOENNIAN] =
    {
        .baseHP        = 65,
        .baseAttack    = 100,
        .baseDefense   = 100,
        .baseSpeed     = 110,
        .baseSpAttack  = 100,
        .baseSpDefense = 60,
        .types = MON_TYPES(TYPE_STEEL, TYPE_FIRE),
        .catchRate = 25,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 163 : 168,
        .evYield_Defense = 2,
        .itemRare = ITEM_METAL_COAT,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FLYING),
        .abilities = { ABILITY_KEEN_EYE, ABILITY_STURDY, ABILITY_WEAK_ARMOR },
        .bodyColor = BODY_COLOR_GRAY,
        .speciesName = _("Skarmory"),
        .cryId = CRY_SKARMORY,
        .natDexNum = NATIONAL_DEX_SKARMORY_HOENNIAN,
        .categoryName = _("Armor Bird"),
        .height = 17,
        .weight = 505,
        .description = COMPOUND_STRING(
            "A Pokémon that has a body and wings of\n"
            "steel. People in the past used feathers\n"
            "fallen from Skarmory to make swords and\n"
            "knives."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 271,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_SkarmoryH,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 0 : 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = P_GBA_STYLE_SPECIES_GFX ? ANIM_V_STRETCH : ANIM_V_SHAKE,
        .backPic = gMonBackPic_SkarmoryH,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 56) : MON_COORDS_SIZE(48, 64),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_JOLT_RIGHT,
        .palette = gMonPalette_SkarmoryH,
        .shinyPalette = gMonShinyPalette_SkarmoryH,
        .iconSprite = gMonIcon_SkarmoryH,
        .iconPalIndex = 0,
        SHADOW(2, 9, SHADOW_SIZE_M)
        FOOTPRINT(Skarmory)
        OVERWORLD(
            sPicTable_Skarmory,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Skarmory,
            gShinyOverworldPalette_Skarmory
        )
        .levelUpLearnset = sHeatranLevelUpLearnset,
        .teachableLearnset = sHeatranTeachableLearnset,
        .eggMoveLearnset = sSkarmoryEggMoveLearnset,
    },

#if P_FAMILY_TORKOAL
    [SPECIES_TORTEMPLE] =
    {
        .baseHP        = 80,
        .baseAttack    = 55,
        .baseDefense   = 180,
        .baseSpeed     = 20,
        .baseSpAttack  = 105,
        .baseSpDefense = 90,
        .types = MON_TYPES(TYPE_FIRE),
        .catchRate = 90,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 165 : 161,
        .evYield_Defense = 2,
        .itemRare = ITEM_CHARCOAL,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_FIELD),
    #if P_UPDATED_ABILITIES >= GEN_7
        .abilities = { ABILITY_WHITE_SMOKE, ABILITY_DROUGHT, ABILITY_SHELL_ARMOR },
    #else
        .abilities = { ABILITY_WHITE_SMOKE, ABILITY_NONE, ABILITY_SHELL_ARMOR },
    #endif
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Tortemple"),
        .cryId = CRY_TORKOAL,
        .natDexNum = NATIONAL_DEX_TORTEMPLE,
        .categoryName = _("Coal"),
        .height = 5,
        .weight = 804,
        .description = COMPOUND_STRING(
            "It battles using energy it gets from\n"
            "burning coal. When loosing smoke from its\n"
            "nostrils, it lets off a sound that is\n"
            "similar to a locomotive's horn."),
        .pokemonScale = 390,
        .pokemonOffset = 9,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Tortemple,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 2 : 3,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_Tortemple,
        .backPicSize = MON_COORDS_SIZE(64, 48),
        .backPicYOffset = 0,
        .backAnimId = BACK_ANIM_SHAKE_GLOW_RED,
        .palette = gMonPalette_Tortemple,
        .shinyPalette = gMonShinyPalette_Tortemple,
        .iconSprite = gMonIcon_Tortemple,
        .iconPalIndex = 0,
        SHADOW(2, 7, SHADOW_SIZE_L)
        FOOTPRINT(Torkoal)
        OVERWORLD(
            sPicTable_Torkoal,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Torkoal,
            gShinyOverworldPalette_Torkoal
        )
        .levelUpLearnset = sTorkoalLevelUpLearnset,
        .teachableLearnset = sTorkoalTeachableLearnset,
        .eggMoveLearnset = sTorkoalEggMoveLearnset,
    },
#endif //P_FAMILY_TORKOAL

#if P_FAMILY_TROPIUS
    [SPECIES_TROPIARCH] =
    {
        .baseHP        = 130,
        .baseAttack    = 95,
        .baseDefense   = 95,
        .baseSpeed     = 50,
        .baseSpAttack  = 85,
        .baseSpDefense = 105,
        .types = MON_TYPES(TYPE_GRASS, TYPE_FLYING),
        .catchRate = 200,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 161 : 169,
        .evYield_HP = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_MONSTER, EGG_GROUP_GRASS),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_CHLOROPHYLL, ABILITY_SOLAR_POWER, ABILITY_HARVEST },
    #else
        .abilities = { ABILITY_CHLOROPHYLL, ABILITY_NONE, ABILITY_HARVEST },
    #endif
        .bodyColor = BODY_COLOR_GREEN,
        .speciesName = _("Tropiarch"),
        .cryId = CRY_TROPIUS,
        .natDexNum = NATIONAL_DEX_TROPIARCH,
        .categoryName = _("Fruit"),
        .height = 20,
        .weight = 1000,
        .description = COMPOUND_STRING(
            "It flies by flapping its broad leaves.\n"
            "The bunch of fruit that grows around its\n"
            "neck is deliciously sweet. In the spring,\n"
            "it scatters pollen from its neck."),
        .pokemonScale = 256,
        .pokemonOffset = 0,
        .trainerScale = 344,
        .trainerOffset = 7,
        .frontPic = gMonFrontPic_Tropiarch,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 0,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_SHAKE,
        .backPic = gMonBackPic_Tropiarch,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 56) : MON_COORDS_SIZE(56, 64),
        .backPicYOffset = P_GBA_STYLE_SPECIES_GFX ? 7 : 0,
        .backAnimId = BACK_ANIM_V_SHAKE_LOW,
        .palette = gMonPalette_Tropiarch,
        .shinyPalette = gMonShinyPalette_Tropiarch,
        .iconSprite = gMonIcon_Tropiarch,
        .iconPalIndex = 1,
        SHADOW(-6, 13, SHADOW_SIZE_L)
        FOOTPRINT(Tropius)
        OVERWORLD(
            sPicTable_Tropius,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Tropius,
            gShinyOverworldPalette_Tropius
        )
        .levelUpLearnset = sTropiusLevelUpLearnset,
        .teachableLearnset = sTropiusTeachableLearnset,
        .eggMoveLearnset = sTropiusEggMoveLearnset,
    },
#endif //P_FAMILY_TROPIUS

#if P_FAMILY_PHANTUMP
    [SPECIES_DREDWOOD] =
    {
        .baseHP        = 105,
        .baseAttack    = 130,
        .baseDefense   = 86,
        .baseSpeed     = 56,
        .baseSpAttack  = 65,
        .baseSpDefense = 102,
        .types = MON_TYPES(TYPE_GHOST, TYPE_GRASS),
        .catchRate = 60,
        .expYield = 166,
        .evYield_Attack = 2,
        .genderRatio = PERCENT_FEMALE(50),
        .eggCycles = 20,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_GRASS, EGG_GROUP_AMORPHOUS),
        .abilities = { ABILITY_NATURAL_CURE, ABILITY_FRISK, ABILITY_HARVEST },
        .bodyColor = BODY_COLOR_BROWN,
        .speciesName = _("Dredwood"),
        .cryId = CRY_TREVENANT,
        .natDexNum = NATIONAL_DEX_DREDWOOD,
        .categoryName = _("Elder Tree"),
        .height = 15,
        .weight = 710,
        .description = COMPOUND_STRING(
            "Through its roots, it exerts control over\n"
            "other trees. A deadly curse falls upon\n"
            "anyone cutting down trees in forests\n"
            "where Trevenant dwell."),
        .pokemonScale = 268,
        .pokemonOffset = 2,
        .trainerScale = 271,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Dredwood,
        .frontPicSize = MON_COORDS_SIZE(64, 64),
        .frontPicYOffset = 1,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_H_SLIDE_SLOW,
        .backPic = gMonBackPic_Dredwood,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 5,
        .backAnimId = BACK_ANIM_V_SHAKE_LOW,
        .palette = gMonPalette_Dredwood,
        .shinyPalette = gMonShinyPalette_Dredwood,
        .iconSprite = gMonIcon_Dredwood,
        .iconPalIndex = 1,
        SHADOW(-1, 13, SHADOW_SIZE_M)
        FOOTPRINT(Trevenant)
        OVERWORLD(
            sPicTable_Trevenant,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Trevenant,
            gShinyOverworldPalette_Trevenant
        )
        .levelUpLearnset = sTrevenantLevelUpLearnset,
        .teachableLearnset = sTrevenantTeachableLearnset,
    },
#endif //P_FAMILY_PHANTUMP

#if P_FAMILY_JYNX
    [SPECIES_SOPRANICE] =
    {
        .baseHP        = 75,
        .baseAttack    = 50,
        .baseDefense   = 35,
        .baseSpeed     = 105,
        .baseSpAttack  = 135,
        .baseSpDefense = 125,
        .types = MON_TYPES(TYPE_ICE, TYPE_PSYCHIC),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 159 : 137,
        .evYield_SpAttack = 2,
        .genderRatio = MON_FEMALE,
        .eggCycles = 25,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_FAST,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_HUMAN_LIKE),
    #if P_UPDATED_ABILITIES >= GEN_4
        .abilities = { ABILITY_OBLIVIOUS, ABILITY_FOREWARN, ABILITY_DRY_SKIN },
    #else
        .abilities = { ABILITY_OBLIVIOUS, ABILITY_NONE, ABILITY_DRY_SKIN },
    #endif
        .bodyColor = BODY_COLOR_RED,
        .speciesName = _("Sopranice"),
        .cryId = CRY_JYNX,
        .natDexNum = NATIONAL_DEX_SOPRANICE,
        .categoryName = _("Human Shape"),
        .height = 14,
        .weight = 406,
        .description = COMPOUND_STRING(
            "A Sopranice sashays rhythmically as if it were\n"
            "dancing. Its motions are so bouncingly\n"
            "alluring, people seeing it are compelled to\n"
            "shake their hips without noticing."),
        .pokemonScale = 256,
        .pokemonOffset = 3,
        .trainerScale = 300,
        .trainerOffset = 1,
        .frontPic = gMonFrontPic_Sopranice,
        .frontPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(56, 56) : MON_COORDS_SIZE(64, 48),
        .frontPicYOffset = 2,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_V_STRETCH,
        .backPic = gMonBackPic_Sopranice,
        .backPicSize = P_GBA_STYLE_SPECIES_GFX ? MON_COORDS_SIZE(64, 48) : MON_COORDS_SIZE(56, 64),
        .backPicYOffset = 4,
        .backAnimId = BACK_ANIM_DIP_RIGHT_SIDE,
        .palette = gMonPalette_Sopranice,
        .shinyPalette = gMonShinyPalette_Sopranice,
        .iconSprite = gMonIcon_Sopranice,
        .iconPalIndex = 3,
        SHADOW(0, 3, SHADOW_SIZE_L)
        FOOTPRINT(Jynx)
        OVERWORLD(
            sPicTable_Jynx,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Jynx,
            gShinyOverworldPalette_Jynx
        )
        .levelUpLearnset = sJynxLevelUpLearnset,
        .teachableLearnset = sJynxTeachableLearnset,
    },
#endif //P_FAMILY_JYNX

#if P_FAMILY_COMBEE
    [SPECIES_BEEFENDER] =
    {
        .baseHP        = 110,
        .baseAttack    = 110,
        .baseDefense   = 122,
        .baseSpeed     = 40,
        .baseSpAttack  = 80,
        .baseSpDefense = 80,
        .types = MON_TYPES(TYPE_BUG, TYPE_ROCK),
        .catchRate = 45,
        .expYield = (P_UPDATED_EXP_YIELDS >= GEN_5) ? 166 : 188,
        .evYield_Defense = 1,
        .evYield_SpDefense = 1,
        .itemRare = ITEM_POISON_BARB,
        .genderRatio = MON_MALE,
        .eggCycles = 15,
        .friendship = STANDARD_FRIENDSHIP,
        .growthRate = GROWTH_MEDIUM_SLOW,
        .eggGroups = MON_EGG_GROUPS(EGG_GROUP_BUG),
        .abilities = { ABILITY_PRESSURE, ABILITY_NONE, ABILITY_UNNERVE },
        .bodyColor = BODY_COLOR_YELLOW,
        .speciesName = _("Beefender"),
        .cryId = CRY_VESPIQUEN,
        .natDexNum = NATIONAL_DEX_BEEFENDER,
        .categoryName = _("Beehive"),
        .height = 12,
        .weight = 385,
        .description = COMPOUND_STRING(
            "It houses its colony in cells in its body\n"
            "and releases various pheromones to\n"
            "make those grubs do its bidding.\n"
            "There is only one in a colony."),
        .pokemonScale = 282,
        .pokemonOffset = 4,
        .trainerScale = 256,
        .trainerOffset = 0,
        .frontPic = gMonFrontPic_Beefender,
        .frontPicSize = MON_COORDS_SIZE(48, 56),
        .frontPicYOffset = 4,
        .frontAnimFrames = sAnims_SingleFramePlaceHolder,
        .frontAnimId = ANIM_LUNGE_GROW,
        .enemyMonElevation = 4,
        .backPic = gMonBackPic_Beefender,
        .backPicSize = MON_COORDS_SIZE(64, 64),
        .backPicYOffset = 4,
        .backAnimId = BACK_ANIM_CIRCLE_COUNTERCLOCKWISE,
        .palette = gMonPalette_Beefender,
        .shinyPalette = gMonShinyPalette_Beefender,
        .iconSprite = gMonIcon_Beefender,
        .iconPalIndex = 0,
        SHADOW(-1, 11, SHADOW_SIZE_L)
        FOOTPRINT(Vespiquen)
        OVERWORLD(
            sPicTable_Vespiquen,
            SIZE_32x32,
            SHADOW_SIZE_M,
            TRACKS_FOOT,
            sAnimTable_Following,
            gOverworldPalette_Vespiquen,
            gShinyOverworldPalette_Vespiquen
        )
        .levelUpLearnset = sCrustleLevelUpLearnset,
        .teachableLearnset = sCrustleTeachableLearnset,
    },
#endif //P_FAMILY_COMBEE
};

