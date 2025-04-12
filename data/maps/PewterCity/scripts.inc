PewterCity_MapScripts::
	.byte 0

PewterCity_EventScript_Locked::
	msgbox PewterCity_Text_Locked, MSGBOX_SIGN
	end

PewterCity_Text_Locked:
	.string "It's locked.$"

PewterCity_EventScript_MuseumClosed::
	msgbox PewterCity_Text_MuseumClosed, MSGBOX_NPC
	end

PewterCity_Text_MuseumClosed:
	.string "The museum is currently closed due\n"
	.string "to a robbery investigation.$"

PewterCity_EventScript_GymSign::
	msgbox PewterCity_Text_GymSign, MSGBOX_SIGN
	end

PewterCity_Text_GymSign:
	.string "PEWTER GYM:\n"
	.string "WE ARE ROCK HARD$"

PewterCity_EventScript_TownSign::
	msgbox PewterCity_Text_TownSign, MSGBOX_SIGN
	end

PewterCity_Text_TownSign:
	.string "PEWTER CITY:\n"
	.string "UH, ROCKS$"

PewterCity_EventScript_ShopBlock::
	msgbox PewterCity_Text_ShopBlock, MSGBOX_NPC
	end

PewterCity_Text_ShopBlock:
	.string "We're still setting up shop.\p"
	.string "Come back later.$"

