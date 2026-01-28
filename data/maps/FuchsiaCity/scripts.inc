FuchsiaCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, FuchsiaCity_OnTransition
	.byte 0

FuchsiaCity_OnTransition:
	setflag FLAG_VISITED_FUCHSIA
	end

@ TP Man Stuff

FuchsiaCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox FuchsiaCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_FUCHSIA,   FuchsiaCity_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  FuchsiaCity_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    FuchsiaCity_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  FuchsiaCity_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    FuchsiaCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  FuchsiaCity_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   FuchsiaCity_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   FuchsiaCity_EventScript_WarpCeladon
	case TPMAN_SELECTION_VERMILION, FuchsiaCity_EventScript_WarpVermilion
	case TPMAN_SELECTION_CINNABAR,  FuchsiaCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      FuchsiaCity_EventScript_Exit
	case MULTI_B_PRESSED,           FuchsiaCity_EventScript_Rude
	release
	end

FuchsiaCity_EventScript_YoureHere::
	msgbox FuchsiaCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

FuchsiaCity_EventScript_WarpCerulean::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

FuchsiaCity_EventScript_WarpPewter::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

FuchsiaCity_EventScript_WarpViridian::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

FuchsiaCity_EventScript_WarpPallet::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

FuchsiaCity_EventScript_WarpLavender::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

FuchsiaCity_EventScript_WarpSaffron::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

FuchsiaCity_EventScript_WarpCeladon::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

FuchsiaCity_EventScript_WarpVermilion::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

FuchsiaCity_EventScript_WarpCinnabar::
	msgbox FuchsiaCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

FuchsiaCity_EventScript_Exit::
	msgbox FuchsiaCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

FuchsiaCity_EventScript_Rude::
	msgbox FuchsiaCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

FuchsiaCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

FuchsiaCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

FuchsiaCity_Text_GuessNot:
	.string "Guess not.$"

FuchsiaCity_Text_Rude:
	.string "Rude, there's an exit button.$"

FuchsiaCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

