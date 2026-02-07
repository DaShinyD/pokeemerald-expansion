.set LOCALID_RANGER, 3
.set LOCALID_RYE, 4

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

FuchsiaCity_EventScript_Locked::
	msgbox FuchsiaCity_Text_Locked, MSGBOX_SIGN
	end

FuchsiaCity_EventScript_CitySign::
	msgbox FuchsiaCity_Text_CitySign, MSGBOX_SIGN
	end

FuchsiaCity_Text_Locked:
	.string "It's locked.$"

FuchsiaCity_Text_CitySign:
	.string "FUCHSIA CITY:\n"
	.string "HOME OF SAFARI ZONE$"

FuchsiaCity_EventScript_RyeRangerL::
	lock
	addobject LOCALID_RYE
	addobject LOCALID_RANGER
	special DrawWholeMapView
	applymovement LOCALID_RYE, FuchsiaCity_Movement_WalkToPlayerL
	waitmovement 0
	msgbox FuchsiaCity_Text_HeyPlayer, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, FuchsiaCity_Movement_GoRangerL
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaCity_Movement_GoRangerPL
	waitmovement 0
	applymovement LOCALID_RANGER, FuchsiaCity_Movement_TurnDown
	waitmovement 0
	msgbox FuchsiaCity_Text_Ranger, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RANGER, FuchsiaCity_Movement_GoUp
	waitmovement 0
	removeobject LOCALID_RANGER
	applymovement LOCALID_RYE, FuchsiaCity_Movement_LookRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaCity_Movement_LookLeft
	waitmovement 0
	msgbox FuchsiaCity_Text_GoOn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, FuchsiaCity_Movement_Leave
	waitmovement 0
	removeobject LOCALID_RYE
	setflag FLAG_RYE_SAFFRON
	setvar VAR_RYE_SAFFRON, 1
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaCity_Movement_PlayerEnter
	waitmovement 0
	warp MAP_FUCHSIA_SAFARI, 16, 21
	waitstate
	releaseall
	end

FuchsiaCity_EventScript_RyeRangerR::
	lock
	addobject LOCALID_RYE
	addobject LOCALID_RANGER
	special DrawWholeMapView
	applymovement LOCALID_RYE, FuchsiaCity_Movement_WalkToPlayerR
	waitmovement 0
	msgbox FuchsiaCity_Text_HeyPlayer, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, FuchsiaCity_Movement_GoRangerR
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaCity_Movement_GoRangerPR
	waitmovement 0
	applymovement LOCALID_RANGER, FuchsiaCity_Movement_TurnDown
	waitmovement 0
	msgbox FuchsiaCity_Text_Ranger, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RANGER, FuchsiaCity_Movement_GoUp
	waitmovement 0
	removeobject LOCALID_RANGER
	applymovement LOCALID_RYE, FuchsiaCity_Movement_LookRight
	waitmovement 0
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaCity_Movement_LookLeft
	waitmovement 0
	msgbox FuchsiaCity_Text_GoOn, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, FuchsiaCity_Movement_Leave
	waitmovement 0
	removeobject LOCALID_RYE
	setflag FLAG_RYE_SAFFRON
	setvar VAR_RYE_SAFFRON, 1
	applymovement OBJ_EVENT_ID_PLAYER, FuchsiaCity_Movement_PlayerEnter
	waitmovement 0
	warp MAP_FUCHSIA_SAFARI, 16, 21
	waitstate
	releaseall
	end

FuchsiaCity_Movement_WalkToPlayerL:
	walk_down
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

FuchsiaCity_Movement_WalkToPlayerR:
	walk_down
	walk_right
	walk_right
	walk_right
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

FuchsiaCity_Text_HeyPlayer:
	.string "{PLAYER}!\p"
	.string "Just the person I wanted to see.\p"
	.string "Listen, this ranger guy needs some help\n"
	.string "in the Safari Zone.\p"
	.string "Apparently, a pokemon is rampaging and\n"
	.string "he needs someone who really understands\l"
	.string "pokemon to help.\p"
	.string "I could think of no one better than\n"
	.string "you!\p"
	.string "The Safari Zone is under construction,\n"
	.string "so we gotta use a secret path.\p"
	.string "Come with me.$"

FuchsiaCity_Movement_GoRangerL:
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
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
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

FuchsiaCity_Movement_GoRangerPL:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
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
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	step_end

FuchsiaCity_Movement_GoRangerR:
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
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
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

FuchsiaCity_Movement_GoRangerPR:
	walk_up
	walk_up
	walk_up
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
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
	walk_up
	walk_up
	walk_up
	walk_up
	walk_right
	walk_up
	step_end

FuchsiaCity_Movement_TurnDown:
	face_down
	step_end

FuchsiaCity_Text_Ranger:
	.string "Ranger: Hello there.\p"
	.string "You must be {PLAYER}.\p"
	.string "Rye told me about you.\p"
	.string "Long story short, there's a rhyperior\n"
	.string "going wild in the Safari Zone.\p"
	.string "I'm not sure what caused it, but it\n"
	.string "simply won't listen to my heart.\p"
	.string "From what I've heard about you, if\n"
	.string "you give it a try, it may just work.\p"
	.string "Please, follow me so we can help this\n"
	.string "poor pokemon.$"

FuchsiaCity_Movement_GoUp:
	face_up
	walk_up
	step_end

FuchsiaCity_Movement_LookRight:
	face_right
	step_end

FuchsiaCity_Movement_LookLeft:
	face_left
	step_end

FuchsiaCity_Text_GoOn:
	.string "Go on, {PLAYER}.\p"
	.string "Usually I'd be hankering for a battle,\n"
	.string "but obviously this is more important.\p"
	.string "I'll be in Pallet visiting the famous\n"
	.string "Prof. Oak's lab when you're done.\p"
	.string "From the looks of things, there isn't\n"
	.string "much left to do this way.\p"
	.string "Good luck!$"

FuchsiaCity_Movement_Leave:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

FuchsiaCity_Movement_PlayerEnter:
	walk_up
	walk_up
	step_end

