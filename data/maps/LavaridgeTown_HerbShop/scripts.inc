LavaridgeTown_HerbShop_MapScripts::
	.byte 0

LavaridgeTown_HerbShop_EventScript_Clerk::
	lock
	faceplayer
	goto_if_set FLAG_BADGE04_GET, LavaridgeTown_HerbShop_EventScript_SellItems
	msgbox LavaridgeTown_HerbShop_Text_AintGoodEnough, MSGBOX_DEFAULT
	closemessage
	release
	end

LavaridgeTown_HerbShop_Text_AintGoodEnough:
	.string "Sorry, we can't sell to you\n"
	.string "unless you have at least four\l"
	.string "gym badges.\p"
	.string "Come back with four and you can\n"
	.string "buy great competitive items.$"

LavaridgeTown_HerbShop_EventScript_SellItems::
	message LavaridgeTown_HerbShop_Text_HowCanIHelp
	waitmessage
	pokemart LavaridgeTown_CompItems_Mart_Pokemart
	msgbox LavaridgeTown_HerbShop_Text_ComeAgain, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_HerbShop_Text_HowCanIHelp:
	.string "How can I help you?$"

	.align 2
LavaridgeTown_CompItems_Mart_Pokemart:
	.2byte ITEM_ABILITY_CAPSULE
	.2byte ITEM_ABILITY_PATCH
	.2byte ITEM_ABSORB_BULB
	.2byte ITEM_AIR_BALLOON
	.2byte ITEM_ASSAULT_VEST
	.2byte ITEM_BIG_ROOT
	.2byte ITEM_BLACK_BELT
	.2byte ITEM_BLACK_GLASSES
	.2byte ITEM_BLACK_SLUDGE
	.2byte ITEM_BRIGHT_POWDER
	.2byte ITEM_CELL_BATTERY
	.2byte ITEM_CHARCOAL
	.2byte ITEM_CHOICE_BAND
	.2byte ITEM_CHOICE_SCARF
	.2byte ITEM_CHOICE_SPECS
	.2byte ITEM_DAMP_ROCK
	.2byte ITEM_DRAGON_FANG
	.2byte ITEM_EJECT_BUTTON
	.2byte ITEM_ELECTRIC_SEED
	.2byte ITEM_EVIOLITE
	.2byte ITEM_EXPERT_BELT
	.2byte ITEM_FLAME_ORB
	.2byte ITEM_FLOAT_STONE
	.2byte ITEM_FOCUS_BAND
	.2byte ITEM_FOCUS_SASH
	.2byte ITEM_GRASSY_SEED
	.2byte ITEM_GRIP_CLAW
	.2byte ITEM_HARD_STONE
	.2byte ITEM_HEAT_ROCK
	.2byte ITEM_HEAVY_DUTY_BOOTS
	.2byte ITEM_ICY_ROCK
	.2byte ITEM_IRON_BALL
	.2byte ITEM_KINGS_ROCK
	.2byte ITEM_LAGGING_TAIL
	.2byte ITEM_LEEK
	.2byte ITEM_LEFTOVERS
	.2byte ITEM_LIFE_ORB
	.2byte ITEM_LIGHT_BALL
	.2byte ITEM_LIGHT_CLAY
	.2byte ITEM_LUMINOUS_MOSS
	.2byte ITEM_MAGNET
	.2byte ITEM_MENTAL_HERB
	.2byte ITEM_METAL_COAT
	.2byte ITEM_METAL_POWDER
	.2byte ITEM_METRONOME
	.2byte ITEM_MIRACLE_SEED
	.2byte ITEM_MISTY_SEED
	.2byte ITEM_MUSCLE_BAND
	.2byte ITEM_MYSTIC_WATER
	.2byte ITEM_NEVER_MELT_ICE
	.2byte ITEM_NORMAL_GEM
	.2byte ITEM_POISON_BARB
	.2byte ITEM_POWER_HERB
	.2byte ITEM_PSYCHIC_SEED
	.2byte ITEM_QUICK_CLAW
	.2byte ITEM_QUICK_POWDER
	.2byte ITEM_RAZOR_CLAW
	.2byte ITEM_RED_CARD
	.2byte ITEM_ROCKY_HELMET
	.2byte ITEM_ROOM_SERVICE
	.2byte ITEM_RUSTED_SHIELD
	.2byte ITEM_RUSTED_SWORD
	.2byte ITEM_SAFETY_GOGGLES
	.2byte ITEM_SCOPE_LENS
	.2byte ITEM_SHARP_BEAK
	.2byte ITEM_SHELL_BELL
	.2byte ITEM_SILK_SCARF
	.2byte ITEM_SILVER_POWDER
	.2byte ITEM_SMOOTH_ROCK
	.2byte ITEM_SOFT_SAND
	.2byte ITEM_SPELL_TAG
	.2byte ITEM_STICKY_BARB
	.2byte ITEM_THROAT_SPRAY
	.2byte ITEM_TOXIC_ORB
	.2byte ITEM_TWISTED_SPOON
	.2byte ITEM_WEAKNESS_POLICY
	.2byte ITEM_WHITE_HERB
	.2byte ITEM_WIDE_LENS
	.2byte ITEM_WISE_GLASSES
	.2byte ITEM_ZOOM_LENS
	.2byte ITEM_CALCIUM
	.2byte ITEM_ZINC
	.2byte ITEM_CARBOS
	.2byte ITEM_PROTEIN
	.2byte ITEM_IRON
	.2byte ITEM_HP_UP
	.2byte ITEM_PP_UP
	.2byte ITEM_PP_MAX
	.2byte ITEM_POMEG_BERRY
	.2byte ITEM_KELPSY_BERRY
	.2byte ITEM_QUALOT_BERRY
	.2byte ITEM_HONDEW_BERRY
	.2byte ITEM_GREPA_BERRY
	.2byte ITEM_TAMATO_BERRY
	.2byte ITEM_LONELY_MINT
	.2byte ITEM_ADAMANT_MINT
	.2byte ITEM_NAUGHTY_MINT
	.2byte ITEM_BRAVE_MINT
	.2byte ITEM_BOLD_MINT
	.2byte ITEM_IMPISH_MINT
	.2byte ITEM_LAX_MINT
	.2byte ITEM_RELAXED_MINT
	.2byte ITEM_MODEST_MINT
	.2byte ITEM_MILD_MINT
	.2byte ITEM_RASH_MINT
	.2byte ITEM_QUIET_MINT
	.2byte ITEM_CALM_MINT
	.2byte ITEM_GENTLE_MINT
	.2byte ITEM_CAREFUL_MINT
	.2byte ITEM_SASSY_MINT
	.2byte ITEM_TIMID_MINT
	.2byte ITEM_HASTY_MINT
	.2byte ITEM_JOLLY_MINT
	.2byte ITEM_NAIVE_MINT
	.2byte ITEM_SERIOUS_MINT
	.2byte ITEM_NONE
	release
	end

LavaridgeTown_HerbShop_Text_ComeAgain:
	.string "Come again!$"

LavaridgeTown_HerbShop_EventScript_ExpertM::
	msgbox LavaridgeTown_HerbShop_Text_HerbalMedicineWorksButMonWillDislike, MSGBOX_NPC
	end

LavaridgeTown_HerbShop_EventScript_OldMan::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_CHARCOAL, LavaridgeTown_HerbShop_EventScript_ExplainCharcoal
	msgbox LavaridgeTown_HerbShop_Text_YouveComeToLookAtHerbalMedicine, MSGBOX_DEFAULT
	giveitem ITEM_CHARCOAL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_CHARCOAL
	release
	end

LavaridgeTown_HerbShop_EventScript_ExplainCharcoal::
	msgbox LavaridgeTown_HerbShop_Text_ExplainCharcoal, MSGBOX_DEFAULT
	release
	end

LavaridgeTown_HerbShop_Text_YouveComeToLookAtHerbalMedicine:
	.string "You've come to look at the items\n"
	.string "in LAVARIDGE?\p"
	.string "That's rather commendable.\p"
	.string "I like you! Take this!$"

LavaridgeTown_HerbShop_Text_ExplainCharcoal:
	.string "That CHARCOAL I gave you, it's used\n"
	.string "for making herbal medicine.\p"
	.string "It also does wonders when held by\n"
	.string "a POKéMON.\p"
	.string "It intensifies the power of FIRE-type\n"
	.string "moves.$"

LavaridgeTown_HerbShop_Text_HerbalMedicineWorksButMonWillDislike:
	.string "The items here can make Pokemon\n"
	.string "really strong!$"

