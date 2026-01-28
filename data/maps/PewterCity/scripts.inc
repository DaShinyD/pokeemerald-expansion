.set LOCALID_OLD_WOMAN, 7
.set LOCALID_LIGHTNING, 8
.set LOCALID_INVISIBLE_1, 9
.set LOCALID_INVISIBLE_2, 10

PewterCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PewterCity_OnTransition
	.byte 0

PewterCity_OnTransition:
	setflag FLAG_VISITED_PEWTER
	end

PewterCity_EventScript_Locked::
	msgbox PewterCity_Text_Locked, MSGBOX_SIGN
	end

PewterCity_Text_Locked:
	.string "It's locked.$"

PewterCity_EventScript_MuseumClosed::
	msgbox PewterCity_Text_MuseumClosed, MSGBOX_NPC
	end

PewterCity_Text_MuseumClosed:
	.string "The museum is currently closed due\n"
	.string "to a robbery investigation.$"

PewterCity_EventScript_GymSign::
	msgbox PewterCity_Text_GymSign, MSGBOX_SIGN
	end

PewterCity_Text_GymSign:
	.string "PEWTER GYM:\n"
	.string "WE ARE ROCK HARD$"

PewterCity_EventScript_TownSign::
	msgbox PewterCity_Text_TownSign, MSGBOX_SIGN
	end

PewterCity_Text_TownSign:
	.string "PEWTER CITY:\n"
	.string "UH, ROCKS$"

PewterCity_EventScript_ShopBlock::
	msgbox PewterCity_Text_ShopBlock, MSGBOX_NPC
	end

PewterCity_Text_ShopBlock:
	.string "We're still setting up shop.\p"
	.string "Come back later.$"

PewterCity_EventScript_LightningJoke::
	lock
	playbgm MUS_NONE, FALSE
	playse SE_PIN
	applymovement LOCALID_OLD_WOMAN, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_OLD_WOMAN, Common_Movement_FaceRight
	waitmovement 0
	msgbox PewterCity_Text_BeCareful, MSGBOX_DEFAULT
	closemessage
	addobject LOCALID_LIGHTNING
	addobject LOCALID_INVISIBLE_1
	addobject LOCALID_INVISIBLE_2
	createfieldmugshot MUGSHOT_MCQUEEN, EMOTE_NORMAL
	playmoncry SPECIES_KACHOW, CRY_MODE_ENCOUNTER
	msgbox PewterCity_Text_Kachow, MSGBOX_AUTOCLOSE
	removefieldmugshot
	waitmoncry
	applymovement LOCALID_LIGHTNING, PewterCity_Movement_Crash
	applymovement LOCALID_INVISIBLE_1, PewterCity_Movement_Crash
	applymovement LOCALID_INVISIBLE_2, PewterCity_Movement_Crash
	waitmovement 0
	setvar VAR_0x8004, 1  @ vertical pan
	setvar VAR_0x8005, 1  @ horizontal pan
	setvar VAR_0x8006, 8  @ num shakes
	setvar VAR_0x8007, 3  @ shake delay
	special ShakeCamera
	msgbox PewterCity_Text_Crash, MSGBOX_DEFAULT
	createfieldmugshot MUGSHOT_MCQUEEN, EMOTE_NORMAL
	msgbox PewterCity_Text_Kach_Ow, MSGBOX_DEFAULT
	removefieldmugshot
	closemessage
	setvar VAR_LIGHTNING_JOKE, 1
	setflag FLAG_LIGHTNING_M
	fadedefaultbgm
	release
	end

PewterCity_Text_BeCareful:
	.string "Old Woman: Be careful there!\p"
	.string "These lamps tend to get struck by\n"
	.string "lightning in this weather.\p"
	.string "{COLOR RED}Low rumbling noise\p"
	.string "{COLOR DARK_GRAY}Oh no, I hear it coming!$"

PewterCity_Movement_Crash:
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	walk_faster_left
	step_end

PewterCity_Text_Kachow:
	.string "KACHOW$"

PewterCity_Text_Crash:
	.string "CRASH$"

PewterCity_Text_Kach_Ow:
	.string "Kach-OW!$"

@ TP Man Stuff

PewterCity_EventScript_TPMAN::
	lock
	faceplayer
	msgbox PewterCity_Text_ImTpGuy, MSGBOX_DEFAULT
	closemessage
	special ScriptMenu_CreateKantoTPManMultichoice
	waitstate
	special GetKantoTPManSelection
	switch VAR_RESULT
	case TPMAN_SELECTION_PEWTER, 	PewterCity_EventScript_YoureHere
	case TPMAN_SELECTION_CERULEAN,  PewterCity_EventScript_WarpCerulean
	case TPMAN_SELECTION_VERMILION, PewterCity_EventScript_WarpVermilion
	case TPMAN_SELECTION_VIRIDIAN,  PewterCity_EventScript_WarpViridian
	case TPMAN_SELECTION_PALLET,    PewterCity_EventScript_WarpPallet
	case TPMAN_SELECTION_LAVENDER,  PewterCity_EventScript_WarpLavender
	case TPMAN_SELECTION_SAFFRON,   PewterCity_EventScript_WarpSaffron
	case TPMAN_SELECTION_CELADON,   PewterCity_EventScript_WarpCeladon
	case TPMAN_SELECTION_FUCHSIA,   PewterCity_EventScript_WarpFuchsia
	case TPMAN_SELECTION_CINNABAR,  PewterCity_EventScript_WarpCinnabar
	case TPMAN_SELECTION_EXIT,      PewterCity_EventScript_Exit
	case MULTI_B_PRESSED,           PewterCity_EventScript_Rude
	release
	end

PewterCity_EventScript_YoureHere::
	msgbox PewterCity_Text_YoureHere, MSGBOX_DEFAULT
	closemessage
	release
	end

PewterCity_EventScript_WarpCerulean::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CERULEAN_CITY, 23, 22
	release
	end

PewterCity_EventScript_WarpVermilion::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERMILLION_CITY, 14, 7
	release
	end

PewterCity_EventScript_WarpViridian::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERIDIAN_CITY, 54, 7
	release
	end

PewterCity_EventScript_WarpPallet::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_PALLETE_TOWN, 18, 11
	release
	end

PewterCity_EventScript_WarpLavender::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_LAVENDER_TOWN, 20, 10
	release
	end

PewterCity_EventScript_WarpSaffron::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_SAFFRON_CITY, 28, 42
	release
	end

PewterCity_EventScript_WarpCeladon::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CELADON_CITY, 54, 10
	release
	end

PewterCity_EventScript_WarpFuchsia::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_FUCHSIA_CITY, 21, 32
	release
	end

PewterCity_EventScript_WarpCinnabar::
	msgbox PewterCity_Text_HereWeGo, MSGBOX_DEFAULT
	closemessage
	warp MAP_CINNABAR_ISLAND, 26, 13
	release
	end

PewterCity_EventScript_Exit::
	msgbox PewterCity_Text_GuessNot, MSGBOX_DEFAULT
	closemessage
	release
	end

PewterCity_EventScript_Rude::
	msgbox PewterCity_Text_Rude, MSGBOX_DEFAULT
	closemessage
	release
	end

PewterCity_Text_YoureHere:
	.string "...\p"
	.string "You're here.$"

PewterCity_Text_HereWeGo:
	.string "Here we go.\p"
	.string "Abra, teleport!$"

PewterCity_Text_GuessNot:
	.string "Guess not.$"

PewterCity_Text_Rude:
	.string "Rude, there's an exit button.$"

PewterCity_Text_ImTpGuy:
	.string "Hey, I'm the teleport man.\p"
	.string "Would you like to go somewhere?$"

