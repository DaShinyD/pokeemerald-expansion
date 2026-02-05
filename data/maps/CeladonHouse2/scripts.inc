.set LOCALID_CHARMANDER, 2

CeladonHouse2_MapScripts::
	.byte 0

CeladonHouse2_EventScript_OldWomanPre::
	lock
	faceplayer
	goto_if_set FLAG_CHARMANDER_CELADON, CeladonHouse2_EventScript_OldWomanPost
	msgbox CeladonHouse2_Text_TooOldHere, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_CHARMANDER
	fadescreen FADE_FROM_BLACK
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_CHARMANDER
	givemon SPECIES_CHARMANDER, 15, ITEM_LIFE_ORB, ITEM_FAST_BALL, NATURE_MODEST, 0, MON_GENDERLESS, 0, 0, 0, 0, 0, 0, 31, 31, 31, 31, 31, 31, MOVE_FLAMETHROWER, MOVE_SOLAR_BEAM, MOVE_OVERHEAT, MOVE_DRAGON_PULSE, TRUE, FALSE, TYPE_GRASS
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, CeladonHouse2_EventScript_ReceivedCharmander
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, CeladonHouse2_EventScript_ReceivedCharmander
	goto Common_EventScript_NoMoreRoomForPokemon
	release
	end

CeladonHouse2_Text_TooOldHere:
	.string "I don't have much, but it's mine.\p"
	.string "But I'm too old for this little guy\n"
	.string "to keep staying here.\p"
	.string "Please, take him.$"

CeladonHouse2_EventScript_OldWomanPost::
	msgbox CeladonHouse2_Text_HowIsCharm, MSGBOX_NPC
	release
	end

CeladonHouse2_Text_HowIsCharm:
	.string "How is Charmander doing?$"

CeladonHouse2_EventScript_ReceivedCharmander::
	msgbox CeladonHouse2_Text_ReceivedCharm, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_CHARMANDER_CELADON
	release
	end

CeladonHouse2_Text_ReceivedCharm:
	.string "Take good care of Charmander.$"

