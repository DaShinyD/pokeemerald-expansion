#line 1 "src/data/battle_partners.party"

#line 1
    [PARTNER_NONE] =
    {
#line 3
        .trainerClass = TRAINER_CLASS_PKMN_TRAINER_1,
#line 4
        .trainerPic = TRAINER_BACK_PIC_BRENDAN,
        .encounterMusic_gender = 
#line 6
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 0,
        .party = (const struct TrainerMon[])
        {
        },
    },
#line 8
    [PARTNER_STEVEN] =
    {
#line 9
        .trainerName = _("STEVEN"),
#line 10
        .trainerClass = TRAINER_CLASS_RIVAL,
#line 11
        .trainerPic = TRAINER_BACK_PIC_STEVEN,
        .encounterMusic_gender = 
#line 13
            TRAINER_ENCOUNTER_MUSIC_MALE,
        .partySize = 3,
        .party = (const struct TrainerMon[])
        {
            {
#line 15
            .species = SPECIES_SKARMORY,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 19
            .ev = TRAINER_PARTY_EVS(252, 0, 252, 0, 0, 0),
#line 18
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 17
            .lvl = 67,
#line 16
            .nature = NATURE_IMPISH,
            .heldItem = ITEM_ROCKY_HELMET,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 20
                MOVE_BODY_PRESS,
                MOVE_IRON_DEFENSE,
                MOVE_SPIKES,
                MOVE_ROOST,
            },
            },
            {
#line 25
            .species = SPECIES_AGGRON,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 29
            .ev = TRAINER_PARTY_EVS(252, 0, 0, 0, 0, 252),
#line 28
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 27
            .lvl = 67,
#line 26
            .nature = NATURE_CAREFUL,
            .heldItem = ITEM_WEAKNESS_POLICY,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 30
                MOVE_CURSE,
                MOVE_HEAVY_SLAM,
                MOVE_BODY_PRESS,
                MOVE_EARTHQUAKE,
            },
            },
            {
#line 35
            .species = SPECIES_METAGROSS,
            .gender = TRAINER_MON_RANDOM_GENDER,
#line 39
            .ev = TRAINER_PARTY_EVS(252, 252, 0, 0, 0, 0),
#line 38
            .iv = TRAINER_PARTY_IVS(31, 31, 31, 31, 31, 31),
#line 37
            .lvl = 67,
#line 36
            .nature = NATURE_ADAMANT,
            .heldItem = ITEM_METAGROSSITE,
            .dynamaxLevel = MAX_DYNAMAX_LEVEL,
            .moves = {
#line 40
                MOVE_PSYCHIC_FANGS,
                MOVE_EARTHQUAKE,
                MOVE_HEAVY_SLAM,
                MOVE_BULLET_PUNCH,
            },
            },
        },
    },
