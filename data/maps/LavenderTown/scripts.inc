LavenderTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavenderTown_OnTransition
	.byte 0

LavenderTown_OnTransition:
	setflag FLAG_VISITED_LAVENDER
	end

LavenderTown_EventScript_OldWoman::
	msgbox LavenderTown_Text_OldWoman, MSGBOX_NPC
	end

LavenderTown_EventScript_CamperGirl::
	msgbox LavenderTown_Text_CamperGirl, MSGBOX_NPC
	end

LavenderTown_EventScript_LittleGirl::
	msgbox LavenderTown_Text_LittleGirl, MSGBOX_NPC
	end

LavenderTown_EventScript_Locked::
	msgbox LavenderTown_Text_Locked, MSGBOX_SIGN
	end

LavenderTown_EventScript_TownSign::
	msgbox LavenderTown_Text_TownSign, MSGBOX_SIGN
	end

LavenderTown_EventScript_RouteSign::
	msgbox LavenderTown_Text_RouteSign, MSGBOX_SIGN
	end

LavenderTown_Text_OldWoman:
	.string "I hear that they might be replacing the\n"
	.string "Lavender Tower with a radio tower.\p"
	.string "How awful would that be?$"

LavenderTown_Text_CamperGirl:
	.string "I came here from Celadon.\p"
	.string "There isn't much to do...$"

LavenderTown_Text_LittleGirl:
	.string "Do you believe in ghosts?\p"
	.string "Oh, then don't go in the tower.$"

LavenderTown_Text_Locked:
	.string "It's locked.$"

LavenderTown_Text_TownSign:
.string "LAVENDER TOWN:\n"
.string "IT'S SPOOKY!$"

LavenderTown_Text_RouteSign:
	.string "To Route 8.$"

@ TP Man Stuff

LavenderTown_EventScript_TPMAN::
	lock
	faceplayer
	msgbox LavenderTown_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_LAVENDER,  LavenderTown_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  LavenderTown_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    LavenderTown_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  LavenderTown_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    LavenderTown_EventScript_WarpPallet
	case TPMAN_SELECTION_VERMILION, LavenderTown_EventScript_WarpVermilion
	case TPMAN_SELECTION_SAFFRON,   LavenderTown_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   LavenderTown_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   LavenderTown_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  LavenderTown_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      LavenderTown_EventScript_Exit
	case MULTI_B_PRESSED,           LavenderTown_EventScript_Rude
	release
	end

LavenderTown_EventScript_YoureHere::
	msgbox LavenderTown_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

LavenderTown_EventScript_WarpCerulean::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

LavenderTown_EventScript_WarpPewter::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

LavenderTown_EventScript_WarpViridian::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

LavenderTown_EventScript_WarpPallet::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

LavenderTown_EventScript_WarpVermilion::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

LavenderTown_EventScript_WarpSaffron::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

LavenderTown_EventScript_WarpCeladon::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

LavenderTown_EventScript_WarpFuchsia::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

LavenderTown_EventScript_WarpCinnabar::
	msgbox LavenderTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

LavenderTown_EventScript_Exit::
	msgbox LavenderTown_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

LavenderTown_EventScript_Rude::
	msgbox LavenderTown_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

LavenderTown_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

LavenderTown_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

LavenderTown_Text_GuessNot:
	.string "Guess not.$"

LavenderTown_Text_Rude:
	.string "Rude, there's an exit button.$"

LavenderTown_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

