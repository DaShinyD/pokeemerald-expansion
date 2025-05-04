.set LOCALID_OLD_WOMAN, 7
.set LOCALID_LIGHTNING, 8
.set LOCALID_INVISIBLE_1, 9
.set LOCALID_INVISIBLE_2, 10

PewterCity_MapScripts::
	.byte 0

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
	createfieldmugshot MUGSHOT_MCQUEEN
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
	createfieldmugshot MUGSHOT_MCQUEEN
	msgbox PewterCity_Text_Kach_Ow, MSGBOX_DEFAULT
	removefieldmugshot
	closemessage
	setvar VAR_LIGHTNING_JOKE, 1
	setflag FLAG_LIGHTNING_M
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

