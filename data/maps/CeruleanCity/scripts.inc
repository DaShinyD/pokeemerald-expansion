CeruleanCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_OnTransition
	.byte 0

CeruleanCity_OnTransition:
	setflag FLAG_VISITED_CERULEAN
	end

CeruleanCity_EventScript_Locked::
	msgbox CeruleanCity_Text_Locked, MSGBOX_SIGN
	end

CeruleanCity_Text_Locked:
	.string "It's locked.$"

CeruleanCity_EventScript_NoPass1::
	msgbox CeruleanCity_Text_NoPass1, MSGBOX_NPC
	end

CeruleanCity_Text_NoPass1:
	.string "Sorry, Route 9 is blocked off due\n"
	.string "to a Power Plant issue.$"

CeruleanCity_EventScript_NoPass2::
	msgbox CeruleanCity_Text_NoPass2, MSGBOX_NPC
	end

CeruleanCity_Text_NoPass2:
	.string "Unfortunately, the bridge has\n"
	.string "flooded.\p"
	.string "Everyone has been evacuated.\p"
	.string "You can still reach the cave by\n"
	.string "going West.$"

CeruleanCity_EventScript_CitySign::
	msgbox CeruleanCity_Text_CitySign, MSGBOX_SIGN
	end

CeruleanCity_Text_CitySign:
	.string "CERULEAN CITY:\n"
	.string "THE CITY OF WATER$"

CeruleanCity_EventScript_GymSign::
	msgbox CeruleanCity_Text_GymSign, MSGBOX_SIGN
	end

CeruleanCity_Text_GymSign:
	.string "CERULEAN GYM:\n"
	.string "HOME OF THE MERMAID$"

CeruleanCity_EventScript_SlowbroGirl::
	msgbox CeruleanCity_Text_SlowbroGirl, MSGBOX_NPC
	end

CeruleanCity_Text_SlowbroGirl:
	.string "My Slowbro never listens to me.$"

CeruleanCity_EventScript_OldMan::
	msgbox CeruleanCity_Text_OldMan, MSGBOX_NPC
	end

CeruleanCity_Text_OldMan:
	.string "I sure hope no one was trapped\n"
	.string "on the other side of the bridge.$"

CeruleanCity_EventScript_Youngster::
	msgbox CeruleanCity_Text_Youngster, MSGBOX_NPC
	end

CeruleanCity_Text_Youngster:
	.string "Misty is so...pretty.$"

@ TP Man Stuff

CeruleanCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox CeruleanCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_VERMILION, CeruleanCity_EventScript_WarpVermilion
	case TPMAN_SELECTION_CERULEAN,  CeruleanCity_EventScript_YoureHere
	case TPMAN_SELECTION_PEWTER,    CeruleanCity_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  CeruleanCity_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    CeruleanCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  CeruleanCity_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   CeruleanCity_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   CeruleanCity_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   CeruleanCity_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  CeruleanCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      CeruleanCity_EventScript_Exit
	case MULTI_B_PRESSED,           CeruleanCity_EventScript_Rude
	release
	end

CeruleanCity_EventScript_YoureHere::
	msgbox CeruleanCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

CeruleanCity_EventScript_WarpVermilion::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

CeruleanCity_EventScript_WarpPewter::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

CeruleanCity_EventScript_WarpViridian::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

CeruleanCity_EventScript_WarpPallet::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

CeruleanCity_EventScript_WarpLavender::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

CeruleanCity_EventScript_WarpSaffron::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

CeruleanCity_EventScript_WarpCeladon::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

CeruleanCity_EventScript_WarpFuchsia::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

CeruleanCity_EventScript_WarpCinnabar::
	msgbox CeruleanCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

CeruleanCity_EventScript_Exit::
	msgbox CeruleanCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

CeruleanCity_EventScript_Rude::
	msgbox CeruleanCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

CeruleanCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

CeruleanCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

CeruleanCity_Text_GuessNot:
	.string "Guess not.$"

CeruleanCity_Text_Rude:
	.string "Rude, there's an exit button.$"

CeruleanCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

