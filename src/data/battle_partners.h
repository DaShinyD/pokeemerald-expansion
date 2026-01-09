
    [DIFFICULTY_NORMAL][PARTNER_NONE] =
    {
        .trainerClass = TRAINER_CLASS_PKMN_TRAINER_1,
        .trainerPic = TRAINER_BACK_PIC_BRENDAN,
        .encounterMusic_gender = 
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 0,
        .party = (const struct TrainerMon[])
        {
        },
    },
    [DIFFICULTY_NORMAL][PARTNER_STEVEN] =
    {
        .trainerName = _("STEVEN"),
        .trainerClass = TRAINER_CLASS_RIVAL,
        .trainerPic = TRAINER_BACK_PIC_STEVEN,
        .encounterMusic_gender = 
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
            .species = SPECIES_SKARMORY,
            .gender = TRAINER_MON_RANDOM_GENDER,
            .ev = TRAINER_PARTY_EVS(252, 0, 252, 0, 0, 0),
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
            .lvl = 67,
            .nature = NATURE_IMPISH,
            .heldItem = ITEM_ROCKY_HELMET,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
                MOVE_BODY_PRESS,
                MOVE_IRON_DEFENSE,
                MOVE_SPIKES,
                MOVE_ROOST,
            },
            },
            {
            .species = SPECIES_AGGRON,
            .gender = TRAINER_MON_RANDOM_GENDER,
            .ev = TRAINER_PARTY_EVS(252, 0, 0, 0, 0, 252),
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
            .lvl = 67,
            .nature = NATURE_CAREFUL,
            .heldItem = ITEM_WEAKNESS_POLICY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
                MOVE_CURSE,
                MOVE_HEAVY_SLAM,
                MOVE_BODY_PRESS,
                MOVE_EARTHQUAKE,
            },
            },
            {
            .species = SPECIES_METAGROSS,
            .gender = TRAINER_MON_RANDOM_GENDER,
            .ev = TRAINER_PARTY_EVS(252, 252, 0, 0, 0, 0),
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
            .lvl = 67,
            .nature = NATURE_ADAMANT,
            .heldItem = ITEM_METAGROSSITE,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
                MOVE_PSYCHIC_FANGS,
                MOVE_EARTHQUAKE,
                MOVE_HEAVY_SLAM,
                MOVE_BULLET_PUNCH,
            },
            },
        },
    },
    [DIFFICULTY_NORMAL][PARTNER_WALLY] =
    {
        .trainerName = _("WALLY"),
        .trainerClass = TRAINER_CLASS_RIVAL,
        .trainerPic = TRAINER_BACK_PIC_WALLY,
        .encounterMusic_gender = 
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
            .species = SPECIES_KELDEO,
            .gender = TRAINER_MON_RANDOM_GENDER,
            .heldItem = ITEM_CHOICE_SPECS,
            .ability = ABILITY_JUSTIFIED,
            .iv = TRAINER_PARTY_IVS(26, 26, 26, 26, 26, 26),
            .ev = TRAINER_PARTY_EVS(0, 0, 0, 200, 200, 0),
            .lvl = 100,
            .nature = NATURE_TIMID,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
                MOVE_SURF,
                MOVE_SECRET_SWORD,
                MOVE_FLIP_TURN,
                MOVE_VACUUM_WAVE,
            },
            },
            {
            .species = SPECIES_CERULEDGE,
            .gender = TRAINER_MON_RANDOM_GENDER,
            .heldItem = ITEM_FOCUS_SASH,
            .ability = ABILITY_WEAK_ARMOR,
            .iv = TRAINER_PARTY_IVS(25, 25, 25, 25, 25, 25),
            .ev = TRAINER_PARTY_EVS(0, 200, 0, 200, 0, 0),
            .lvl = 100,
            .nature = NATURE_JOLLY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
                MOVE_SWORDS_DANCE,
                MOVE_BITTER_BLADE,
                MOVE_SHADOW_SNEAK,
                MOVE_CLOSE_COMBAT,
            },
            },
            {
            .species = SPECIES_GALLADE,
            .gender = TRAINER_MON_RANDOM_GENDER,
            .heldItem = ITEM_GALLADITE,
            .ability = ABILITY_SHARPNESS,
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
            .ev = TRAINER_PARTY_EVS(0, 200, 0, 200, 0, 0),
            .lvl = 100,
            .nature = NATURE_JOLLY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
                MOVE_SWORDS_DANCE,
                MOVE_SACRED_SWORD,
                MOVE_PSYCHO_CUT,
                MOVE_LEAF_BLADE,
            },
            },
        },
    },
    
