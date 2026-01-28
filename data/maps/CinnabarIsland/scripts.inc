CinnabarIsland_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CinnabarIsland_OnTransition
	.byte 0

CinnabarIsland_OnTransition:
	setflag FLAG_VISITED_CINNABAR
	end

@TP Man Stuff

CinnabarIsland_EventScript_TPMAN::
	lock
	faceplayer
	msgbox CinnabarIsland_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_CINNABAR,  CinnabarIsland_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  CinnabarIsland_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    CinnabarIsland_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  CinnabarIsland_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    CinnabarIsland_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  CinnabarIsland_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   CinnabarIsland_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   CinnabarIsland_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   CinnabarIsland_EventScript_WarpFuchsia
	case TPMAN_SELECTION_VERMILION, CinnabarIsland_EventScript_WarpVermilion
	case TPMAN_SELECTION_EXIT,      CinnabarIsland_EventScript_Exit
	case MULTI_B_PRESSED,           CinnabarIsland_EventScript_Rude
	release
	end

CinnabarIsland_EventScript_YoureHere::
	msgbox CinnabarIsland_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

CinnabarIsland_EventScript_WarpCerulean::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

CinnabarIsland_EventScript_WarpPewter::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

CinnabarIsland_EventScript_WarpViridian::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

CinnabarIsland_EventScript_WarpPallet::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

CinnabarIsland_EventScript_WarpLavender::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

CinnabarIsland_EventScript_WarpSaffron::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

CinnabarIsland_EventScript_WarpCeladon::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

CinnabarIsland_EventScript_WarpFuchsia::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

CinnabarIsland_EventScript_WarpVermilion::
	msgbox CinnabarIsland_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

CinnabarIsland_EventScript_Exit::
	msgbox CinnabarIsland_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

CinnabarIsland_EventScript_Rude::
	msgbox CinnabarIsland_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

CinnabarIsland_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

CinnabarIsland_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

CinnabarIsland_Text_GuessNot:
	.string "Guess not.$"

CinnabarIsland_Text_Rude:
	.string "Rude, there's an exit button.$"

CinnabarIsland_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

