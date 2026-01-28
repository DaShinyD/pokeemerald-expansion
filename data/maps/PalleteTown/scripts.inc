PalleteTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalleteTown_OnTransition
	.byte 0

PalleteTown_OnTransition:
	setflag FLAG_VISITED_PALLET
	end

@TP Man Stuff

PalleteTown_EventScript_TPMAN::
	lock
	faceplayer
	msgbox PalleteTown_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_PALLET,    PalleteTown_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  PalleteTown_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    PalleteTown_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  PalleteTown_EventScript_WarpViridian
	case TPMAN_SELECTION_VERMILION, PalleteTown_EventScript_WarpVermilion
	case TPMAN_SELECTION_LAVENDER,  PalleteTown_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   PalleteTown_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   PalleteTown_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   PalleteTown_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  PalleteTown_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      PalleteTown_EventScript_Exit
	case MULTI_B_PRESSED,           PalleteTown_EventScript_Rude
	release
	end

PalleteTown_EventScript_YoureHere::
	msgbox PalleteTown_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

PalleteTown_EventScript_WarpCerulean::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

PalleteTown_EventScript_WarpPewter::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

PalleteTown_EventScript_WarpViridian::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

PalleteTown_EventScript_WarpVermilion::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

PalleteTown_EventScript_WarpLavender::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

PalleteTown_EventScript_WarpSaffron::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

PalleteTown_EventScript_WarpCeladon::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

PalleteTown_EventScript_WarpFuchsia::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

PalleteTown_EventScript_WarpCinnabar::
	msgbox PalleteTown_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

PalleteTown_EventScript_Exit::
	msgbox PalleteTown_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

PalleteTown_EventScript_Rude::
	msgbox PalleteTown_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

PalleteTown_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

PalleteTown_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

PalleteTown_Text_GuessNot:
	.string "Guess not.$"

PalleteTown_Text_Rude:
	.string "Rude, there's an exit button.$"

PalleteTown_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

