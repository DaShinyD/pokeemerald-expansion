.set LOCALID_DASH, 3

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

@ Begin actual

CeladonCity_EventScript_DashT::
	lock
	applymovement LOCALID_DASH, CeladonCity_Movement_DashTurn
	waitmovement 0
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	playse SE_PIN
	waitmovement 0
	applymovement LOCALID_DASH, CeladonCity_Movement_DashApproachT
	waitmovement 0
	msgbox CeladonCity_Text_HeyPlayer, MSGBOX_DEFAULT
	closemessage
	goto CeladonCity_EventScript_DashBattle
	release
	end

CeladonCity_EventScript_DashM::
	lock
	applymovement LOCALID_DASH, CeladonCity_Movement_DashTurn
	waitmovement 0
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	playse SE_PIN
	waitmovement 0
	applymovement LOCALID_DASH, CeladonCity_Movement_DashApproachM
	waitmovement 0
	msgbox CeladonCity_Text_HeyPlayer, MSGBOX_DEFAULT
	closemessage
	goto CeladonCity_EventScript_DashBattle
	release
	end

CeladonCity_EventScript_DashB::
	lock
	applymovement LOCALID_DASH, CeladonCity_Movement_DashTurn
	waitmovement 0
	applymovement LOCALID_DASH, Common_Movement_ExclamationMark
	playse SE_PIN
	waitmovement 0
	applymovement LOCALID_DASH, CeladonCity_Movement_DashApproachB
	waitmovement 0
	msgbox CeladonCity_Text_HeyPlayer, MSGBOX_DEFAULT
	closemessage
	goto CeladonCity_EventScript_DashBattle
	release
	end

CeladonCity_Movement_DashTurn:
	face_right
	step_end

CeladonCity_Movement_DashApproachT:
	walk_right
	walk_right
	walk_up
	face_right
	step_end

CeladonCity_Movement_DashApproachM:
	walk_right
	walk_right
	step_end

CeladonCity_Movement_DashApproachB:
	walk_right
	walk_right
	walk_down
	face_right
	step_end

CeladonCity_Text_HeyPlayer:
	.string "{PLAYER}!\p"
	.string "Great to see you here.\p"
	.string "The Kanto region sure looks different\n"
	.string "from Hoenn, huh?\p"
	.string "I heard you also took on Red and Green.\p"
	.string "You know what that means?\p"
	.string "We have to battle!$"

CeladonCity_EventScript_DashBattle::
	trainerbattle_no_intro TRAINER_DASH_CELADON, CeladonCity_Text_Always
	goto CeladonCity_EventScript_DashAfter
	release
	end

CeladonCity_Text_Always:
	.string "Always the winner, aren't you?$"

CeladonCity_EventScript_DashAfter::
	msgbox CeladonCity_Text_Going, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DASH, CeladonCity_Movement_Leave
	waitmovement 0
	setvar VAR_DASH_CELADON, 1
	removeobject LOCALID_DASH
	setflag VAR_DASH_CELADON
	setflag FLAG_LEVEL_CAP_ELEVEN
	buffernumberstring STR_VAR_1, 150
	msgbox gText_NewLevelCapIs, MSGBOX_DEFAULT
	release
	end

CeladonCity_Text_Going:
	.string "One of these days, I'm the one who\n"
	.string "will win.\p"
	.string "I'm heading off to explore more.\p"
	.string "I heard Rye is hanging around in\n"
	.string "Fuchsia City.\p"
	.string "I'm sure he's gonna want to have\n"
	.string "a battle with you too!\p"
	.string "I'll see you around.$"

CeladonCity_Movement_Leave:
	walk_left
	walk_left
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

CeladonCity_EventScript_Locked::
	msgbox CeladonCity_Text_Locked, MSGBOX_SIGN
	end

CeladonCity_Text_Locked:
	.string "It's locked.$"

CeladonCity_EventScript_Locked2::
	msgbox CeladonCity_Text_Locked2, MSGBOX_SIGN
	end

CeladonCity_Text_Locked2:
	.string "Under renovations.$"

CeladonCity_EventScript_CitySign::
	msgbox CeladonCity_Text_CitySign, MSGBOX_SIGN
	end

CeladonCity_Text_CitySign:
	.string "CELADON CITY:\n"
	.string "VERDANT GREENS$"

CeladonCity_EventScript_LittleBoy::
	msgbox CeladonCity_Text_LittleBoy, MSGBOX_NPC
	end

CeladonCity_EventScript_LittleGirl::
	msgbox CeladonCity_Text_LittleGirl, MSGBOX_NPC
	end

CeladonCity_Text_LittleBoy:
	.string "My dream is to be a bug catcher!$"

CeladonCity_Text_LittleGirl:
	.string "My dream is to be a bug!$"

CeladonCity_EventScript_Cameraman::
	msgbox CeladonCity_Text_Cameraman, MSGBOX_NPC
	end

CeladonCity_Text_Cameraman:
	.string "I'm here recording footage for the\n"
	.string "people of Hoenn.$"

