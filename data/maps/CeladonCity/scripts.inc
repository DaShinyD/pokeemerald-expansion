CeladonCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_OnTransition
	.byte 0

CeladonCity_OnTransition:
	setflag FLAG_VISITED_CELADON
	end

@TP Man Stuff

CeladonCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox CeladonCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_CELADON,   CeladonCity_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  CeladonCity_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    CeladonCity_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  CeladonCity_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    CeladonCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  CeladonCity_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   CeladonCity_EventScript_WarpSaffron
	case TPMAN_SELECTION_VERMILION, CeladonCity_EventScript_WarpVermilion
	case TPMAN_SELECTION_FUCHSIA,   CeladonCity_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  CeladonCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      CeladonCity_EventScript_Exit
	case MULTI_B_PRESSED,           CeladonCity_EventScript_Rude
	release
	end

CeladonCity_EventScript_YoureHere::
	msgbox CeladonCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

CeladonCity_EventScript_WarpCerulean::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

CeladonCity_EventScript_WarpPewter::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

CeladonCity_EventScript_WarpViridian::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

CeladonCity_EventScript_WarpPallet::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

CeladonCity_EventScript_WarpLavender::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

CeladonCity_EventScript_WarpSaffron::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

CeladonCity_EventScript_WarpVermilion::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

CeladonCity_EventScript_WarpFuchsia::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

CeladonCity_EventScript_WarpCinnabar::
	msgbox CeladonCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

CeladonCity_EventScript_Exit::
	msgbox CeladonCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

CeladonCity_EventScript_Rude::
	msgbox CeladonCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

CeladonCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

CeladonCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

CeladonCity_Text_GuessNot:
	.string "Guess not.$"

CeladonCity_Text_Rude:
	.string "Rude, there's an exit button.$"

CeladonCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

