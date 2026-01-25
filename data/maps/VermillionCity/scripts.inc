.set LOCALID_SNORLAX, 1
.set LOCALID_LINK, 11
.set LOCALID_SNORLAX_STAND, 12

VermillionCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VermillionCity_OnTransition
	.byte 0

VermillionCity_OnTransition:
	setflag FLAG_SNORLAX
	setflag FLAG_POLICE
	setflag FLAG_VISITED_VERMILION
	end

VermillionCity_EventScript_Locked::
	msgbox VermillionCity_Text_Locked, MSGBOX_SIGN
	end

VermillionCity_Text_Locked:
	.string "It's locked.$"

VermillionCity_EventScript_Snorlax::
	msgbox VermillionCity_Text_SleepingSnorlax, MSGBOX_NPC
	end

VermillionCity_Text_SleepingSnorlax:
	.string "It's a sleeping Snorlax!\p"
	.string "How original.\p"
	.string "Too bad you don't have a flute.$"

VermillionCity_EventScript_Hotel::
	msgbox VermillionCity_Text_Hotel, MSGBOX_SIGN
	end

VermillionCity_Text_Hotel:
	.string "VERMILION HOTEL:\n"
	.string "OPEN SOON$"

VermillionCity_EventScript_MachampMan::
	msgbox VermillionCity_Text_MachampMan, MSGBOX_NPC
	end

VermillionCity_Text_MachampMan:
	.string "I started building this with my\n"
	.string "trusty Machop.\p"
	.string "Now, he's become a big Machamp!$"

VermillionCity_EventScript_HotelWoman::
	msgbox VermillionCity_Text_HotelWoman, MSGBOX_NPC
	end

VermillionCity_Text_HotelWoman:
	.string "Hello, we aren't quite open yet.\p"
	.string "Please come back when we are.$"

VermillionCity_EventScript_CitySign::
	msgbox VermillionCity_Text_CitySign, MSGBOX_SIGN
	end

VermillionCity_Text_CitySign:
	.string "VERMILION CITY:\n"
	.string "NOT MUCH GOING ON$"

VermillionCity_EventScript_FanClubSign::
	msgbox VermillionCity_Text_FanClubSign, MSGBOX_SIGN
	end

VermillionCity_Text_FanClubSign:
	.string "POKEMON FAN CLUB:\n"
	.string "WE <3 RAPIDASH$"

VermillionCity_EventScript_GymSign::
	msgbox VermillionCity_Text_GymSign, MSGBOX_SIGN
	end

VermillionCity_Text_GymSign:
	.string "VERMILION CITY GYM:\n"
	.string "HOME OF THE WAR HERO$"

VermillionCity_EventScript_Fisher1::
	msgbox VermillionCity_Text_Fisher1, MSGBOX_NPC
	end

VermillionCity_Text_Fisher1:
	.string "Only Magikarp today.\p"
	.string "Only Magikarp always...$"

VermillionCity_EventScript_Fisher2::
	msgbox VermillionCity_Text_Fisher2, MSGBOX_NPC
	end

VermillionCity_Text_Fisher2:
	.string "You came from Hoenn?\p"
	.string "That's pretty far!$"

VermillionCity_EventScript_Boy::
	msgbox VermillionCity_Text_Boy, MSGBOX_NPC
	end

VermillionCity_Text_Boy:
	.string "Why is my house backwards?$"

VermillionCity_EventScript_Lass::
	msgbox VermillionCity_Text_Lass, MSGBOX_NPC
	end

VermillionCity_Text_Lass:
	.string "I'm exercising!$"

VermillionCity_EventScript_FanClubPresident::
	msgbox VermillionCity_Text_FanClubPresident, MSGBOX_NPC
	end

VermillionCity_Text_FanClubPresident:
	.string "Our club is ruined!\p"
	.string "The police came and arrested over\n"
	.string "half our members!\p"
	.string "Those poor Lopunny and Gardevoir\n"
	.string "have no trainers now...$"

VermillionCity_EventScript_GuitarGuy::
	msgbox VermillionCity_Text_GuitarGuy, MSGBOX_NPC
	end

VermillionCity_Text_GuitarGuy:
	.string "Surge is in Hoenn right now at\n"
	.string "the Champion club.\p"
	.string "It's so boring when gym season\n"
	.string "isn't happening.$"

VermillionCity_EventScript_LinkA::
	lock
	addobject LOCALID_LINK
	applymovement LOCALID_LINK, VermillionCity_Movement_WalkToSnorlax
	waitmovement 0
	playbgm MUS_RG_POKE_FLUTE, FALSE
	fadescreen FADE_TO_BLACK
	msgbox VermillionCity_Text_PlaysFlute, MSGBOX_AUTOCLOSE
	removeobject LOCALID_SNORLAX
	addobject LOCALID_SNORLAX_STAND
	delay 30
	fadedefaultbgm
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_LINK, VermillionCity_Movement_WalkPastSnorlax
	applymovement OBJ_EVENT_ID_PLAYER, VermillionCity_Movement_StepBack
	waitmovement 0
	fadescreen FADE_TO_BLACK
	addobject LOCALID_SNORLAX
	removeobject LOCALID_SNORLAX_STAND
	msgbox VermillionCity_Text_BackToSleep, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_PLAYER, VermillionCity_Movement_FaceLink
	waitmovement 0
	applymovement LOCALID_LINK, VermillionCity_Movement_FacePlayer
	waitmovement 0
	msgbox VermillionCity_Text_AskLink, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LINK, VermillionCity_Movement_Leave
	waitmovement 0
	removeobject LOCALID_LINK
	setvar VAR_LINK_THING, 1
	release
	end

VermillionCity_EventScript_LinkB::
	lock
	addobject LOCALID_LINK
	applymovement LOCALID_LINK, VermillionCity_Movement_WalkToSnorlax
	waitmovement 0
	playbgm MUS_RG_POKE_FLUTE, FALSE
	fadescreen FADE_TO_BLACK
	msgbox VermillionCity_Text_PlaysFlute, MSGBOX_AUTOCLOSE
	removeobject LOCALID_SNORLAX
	addobject LOCALID_SNORLAX_STAND
	delay 30
	fadedefaultbgm
	fadescreen FADE_FROM_BLACK
	applymovement LOCALID_LINK, VermillionCity_Movement_WalkPastSnorlax
	waitmovement 0
	fadescreen FADE_TO_BLACK
	addobject LOCALID_SNORLAX
	removeobject LOCALID_SNORLAX_STAND
	msgbox VermillionCity_Text_BackToSleep, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_FROM_BLACK
	applymovement OBJ_EVENT_ID_PLAYER, VermillionCity_Movement_FaceLink
	waitmovement 0
	applymovement LOCALID_LINK, VermillionCity_Movement_FacePlayer
	waitmovement 0
	msgbox VermillionCity_Text_AskLink, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LINK, VermillionCity_Movement_Leave
	waitmovement 0
	removeobject LOCALID_LINK
	setvar VAR_LINK_THING, 1
	release
	end

VermillionCity_Movement_WalkToSnorlax:
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

VermillionCity_Text_PlaysFlute:
	.string "The weirdly dressed boy plays an\n"
	.string "instrument, waking up the Snorlax!$"

VermillionCity_Movement_WalkPastSnorlax:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

VermillionCity_Text_BackToSleep:
	.string "The Snorlax quickly goes back to\n"
	.string "sleep.$"

VermillionCity_Movement_FaceLink:
	face_left
	step_end

VermillionCity_Movement_FacePlayer:
	face_right
	step_end

VermillionCity_Text_AskLink:
	.string "{PLAYER}: Hey do you think that\n"
	.string "I could use--\p"
	.string "Kid: HA HU HEH HA HYAA\p"
	.string "...\p"
	.string "{PLAYER}: Nevermind.$"

VermillionCity_Movement_Leave:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

VermillionCity_Movement_StepBack:
	walk_left
	walk_down
	face_up
	step_end

@ TP Man Stuff

VermillionCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox VermillionCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_VERMILION, VermillionCity_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  VermillionCity_EventScript_WarpCerulean
	case TPMAN_SELECTION_PEWTER,    VermillionCity_EventScript_WarpPewter
	case TPMAN_SELECTION_VIRIDIAN,  VermillionCity_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    VermillionCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  VermillionCity_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   VermillionCity_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   VermillionCity_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   VermillionCity_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  VermillionCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      VermillionCity_EventScript_Exit
	case MULTI_B_PRESSED,           VermillionCity_EventScript_Rude
	release
	end

VermillionCity_EventScript_YoureHere::
	msgbox VermillionCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

VermillionCity_EventScript_WarpCerulean::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

VermillionCity_EventScript_WarpPewter::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PEWTER_CITY, 15, 31
	release
	end

VermillionCity_EventScript_WarpViridian::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

VermillionCity_EventScript_WarpPallet::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

VermillionCity_EventScript_WarpLavender::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

VermillionCity_EventScript_WarpSaffron::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

VermillionCity_EventScript_WarpCeladon::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

VermillionCity_EventScript_WarpFuchsia::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

VermillionCity_EventScript_WarpCinnabar::
	msgbox VermillionCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

VermillionCity_EventScript_Exit::
	msgbox VermillionCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

VermillionCity_EventScript_Rude::
	msgbox VermillionCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

VermillionCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

VermillionCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

VermillionCity_Text_GuessNot:
	.string "Guess not.$"

VermillionCity_Text_Rude:
	.string "Rude, there's an exit button.$"

VermillionCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

