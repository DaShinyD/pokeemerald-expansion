.set LOCALID_SNORLAX, 1
.set LOCALID_LINK, 11
.set LOCALID_SNORLAX_STAND, 12

VermillionCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Vermillion_City_OnTransition
	.byte 0

Vermillion_City_OnTransition:
	setflag FLAG_SNORLAX
	setflag FLAG_POLICE
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

