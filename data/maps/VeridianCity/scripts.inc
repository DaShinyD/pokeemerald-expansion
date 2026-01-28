VeridianCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VeridianCity_OnTransition
	.byte 0

VeridianCity_OnTransition:
	setflag FLAG_VISITED_VIRIDIAN
	end

VeridianCity_EventScript_Locked::
	msgbox VeridianCity_Text_Locked, MSGBOX_SIGN
	end

VeridianCity_Text_Locked:
	.string "It's locked.$"

VeridianCity_EventScript_SchoolGuards::
	msgbox VeridianCity_Text_SchoolSession, MSGBOX_NPC
	end

VeridianCity_Text_SchoolSession:
	.string "School is currently in session.$"

VeridianCity_EventScript_Route1Guards::
	msgbox VeridianCity_Text_Route1Guards, MSGBOX_NPC
	end

VeridianCity_Text_Route1Guards:
	.string "Sorry this way is blocked.\p"
	.string "We ran out of excuses so leave.$"

VeridianCity_EventScript_Route24Guards::
	msgbox VeridianCity_Text_Route24Guards, MSGBOX_NPC
	end

VeridianCity_Text_Route24Guards:
	.string "Construction is going on this way.\p"
	.string "The Pokemon League is now in\n"
	.string "Viridian.$"

VeridianCity_EventScript_OldGardener::
	msgbox VeridianCity_Text_OldGardener, MSGBOX_NPC
	end

VeridianCity_Text_OldGardener:
	.string "Gardening is so peaceful.$"

VeridianCity_EventScript_Fisher::
	msgbox VeridianCity_Text_Fisher, MSGBOX_NPC
	end

VeridianCity_Text_Fisher:
	.string "There aren't any Pokemon here!$"

VeridianCity_EventScript_FatMan::
	msgbox VeridianCity_Text_FatMan, MSGBOX_NPC
	end

VeridianCity_Text_FatMan:
	.string "Mmm mmph mm.\p"
	.string "I love burgers.$"

VeridianCity_EventScript_TownSign::
	msgbox VeridianCity_Text_TownSign, MSGBOX_SIGN
	end

VeridianCity_Text_TownSign:
	.string "VIRIDIAN CITY:\n"
	.string "SPELT WITH 3 Is??$"

VeridianCity_EventScript_BugCatcher::
	msgbox VeridianCity_Text_BugCatcher, MSGBOX_NPC
	end

VeridianCity_Text_BugCatcher:
	.string "I can't find any bugs!$"

VeridianCity_EventScript_OldWoman::
	msgbox VeridianCity_Text_OldWoman, MSGBOX_NPC
	end

VeridianCity_Text_OldWoman:
	.string "This city has changed so much.\p"
	.string "It used to be half the size.\p"
	.string "Now, we even have the Pokemon\n"
	.string "League here.$"

VeridianCity_EventScript_Youngster::
	msgbox VeridianCity_Text_Youngster, MSGBOX_NPC
	end

VeridianCity_Text_Youngster:
	.string "One day, I'm gonna challenge the\n"
	.string "Pokemon League.$"

VeridianCity_EventScript_GymSign::
	msgbox VeridianCity_Text_GymSign, MSGBOX_SIGN
	end

VeridianCity_Text_GymSign:
	.string "VIRIDIAN GYM:\n"
	.string "BLUE THE PALLET TOWN POWER$"

@ TP Man Stuff

VeridianCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox VeridianCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_VIRIDIAN,  VeridianCity_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  VeridianCity_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    VeridianCity_EventScript_WarpPewter
	case TPMAN_SELECTION_VERMILION, VeridianCity_EventScript_WarpVermilion
	case TPMAN_SELECTION_PALLET,    VeridianCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  VeridianCity_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   VeridianCity_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   VeridianCity_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   VeridianCity_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  VeridianCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      VeridianCity_EventScript_Exit
	case MULTI_B_PRESSED,           VeridianCity_EventScript_Rude
	release
	end

VeridianCity_EventScript_YoureHere::
	msgbox VeridianCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

VeridianCity_EventScript_WarpCerulean::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

VeridianCity_EventScript_WarpPewter::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

VeridianCity_EventScript_WarpVermilion::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

VeridianCity_EventScript_WarpPallet::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

VeridianCity_EventScript_WarpLavender::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

VeridianCity_EventScript_WarpSaffron::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

VeridianCity_EventScript_WarpCeladon::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

VeridianCity_EventScript_WarpFuchsia::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

VeridianCity_EventScript_WarpCinnabar::
	msgbox VeridianCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

VeridianCity_EventScript_Exit::
	msgbox VeridianCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

VeridianCity_EventScript_Rude::
	msgbox VeridianCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

VeridianCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

VeridianCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

VeridianCity_Text_GuessNot:
	.string "Guess not.$"

VeridianCity_Text_Rude:
	.string "Rude, there's an exit button.$"

VeridianCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

