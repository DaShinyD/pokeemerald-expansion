.set LOCALID_PROF_OAK, 1
.set LOCALID_CINE_BALL, 8
.set LOCALID_REGIROCK, 9
.set LOCALID_REGICE, 10
.set LOCALID_REGISTEEL, 11
.set LOCALID_REGIELEKI, 12
.set LOCALID_REGIDRAGO, 13
.set LOCALID_REGIGIGAS, 14

OakLab_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, OakLab_OnTransition
	.byte 0

OakLab_OnTransition:
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	setflag FLAG_TEMP_16
	setflag FLAG_TEMP_17
	goto_if_eq VAR_OAK_QUEST_STATE, OAK_QUEST_STATE_STUDYING, OakLab_EventScript_FinishQuest1OnReturn
	end

OakLab_EventScript_FinishQuest1OnReturn:
	setvar VAR_OAK_QUEST_STATE, OAK_QUEST_STATE_Q1_DONE
	setvar VAR_0x8004, SIDE_QUEST_1
	setvar VAR_0x8005, QUEST_FLAG_SET_COMPLETED
	special GetSetQuestFlagSpecial
	end

OakLab_EventScript_ProfOak::
	lock
	faceplayer
	switch VAR_OAK_QUEST_STATE
	case OAK_QUEST_STATE_CATCH_REGIS, OakLab_EventScript_CheckRegis
	case OAK_QUEST_STATE_STUDYING, OakLab_EventScript_NeedSomeTime
	case OAK_QUEST_STATE_Q1_DONE, OakLab_EventScript_DummyAfterQuest1
	goto OakLab_EventScript_Intro
	end

OakLab_EventScript_Intro::
	msgbox OakLab_Text_Intro, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, OakLab_EventScript_AcceptQuest1
	msgbox OakLab_Text_WhatAShame, MSGBOX_DEFAULT
	release
	end

OakLab_EventScript_AcceptQuest1::
	setflag FLAG_QUESTS_ACTIVATE
	setvar VAR_0x8004, SIDE_QUEST_1
	setvar VAR_0x8005, QUEST_FLAG_SET_UNLOCKED
	special GetSetQuestFlagSpecial
	setvar VAR_0x8004, SIDE_QUEST_1
	special SetActiveQuestSpecial
	setvar VAR_OAK_QUEST_STATE, OAK_QUEST_STATE_CATCH_REGIS
	playfanfare MUS_OBTAIN_ITEM
	message OakLab_Text_QuestAccepted
	waitmessage
	waitfanfare
	msgbox OakLab_Text_CatchEveryRegi, MSGBOX_DEFAULT
	closemessage
	helpwindow HELP_QUEST_1_CATCH_REGIS
	release
	end

OakLab_EventScript_CheckRegis::
	msgbox OakLab_Text_DidYouCatchThem, MSGBOX_DEFAULT
	closemessage
	specialvar VAR_RESULT, HasAllRegisInParty
	goto_if_eq VAR_RESULT, TRUE, OakLab_EventScript_BeginRegiRitual
	msgbox OakLab_Text_StillMissingRegis, MSGBOX_DEFAULT
	release
	end

OakLab_EventScript_BeginRegiRitual::
	msgbox OakLab_Text_AmazingWork, MSGBOX_DEFAULT
	closemessage
	delay 40
	msgbox OakLab_Text_ScienceCantWait, MSGBOX_DEFAULT
	closemessage
	delay 40
	msgbox OakLab_Text_PrepareForBattle, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_ALPHA_BALL, 10
	closemessage
	call Common_EventScript_SaveGame
	special SetPartyRegisToLevel100
	hidefollower
	call OakLab_EventScript_PlayerStepBack
	call OakLab_EventScript_SummonRegirock
	call OakLab_EventScript_SummonRegice
	call OakLab_EventScript_SummonRegisteel
	call OakLab_EventScript_SummonRegieleki
	call OakLab_EventScript_SummonRegidrago
	call OakLab_EventScript_SummonRegigigas
	delay 30
	setwildbattle SPECIES_REGIALPHA, 100
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, OakLab_EventScript_CaughtRegialpha
	goto OakLab_EventScript_FailedRegialpha
	end

OakLab_EventScript_CaughtRegialpha::
	msgbox OakLab_Text_IncredibleWork, MSGBOX_DEFAULT
	setvar VAR_OAK_QUEST_STATE, OAK_QUEST_STATE_STUDYING
	release
	end

OakLab_EventScript_FailedRegialpha::
	fadescreen FADE_TO_BLACK
	warpsilent MAP_PALLETE_TOWN, 27, 20
	waitstate
	end

OakLab_EventScript_NeedSomeTime::
	msgbox OakLab_Text_GiveMeSomeTime, MSGBOX_DEFAULT
	release
	end

OakLab_EventScript_DummyAfterQuest1::
	msgbox OakLab_Text_StillStudying, MSGBOX_DEFAULT
	release
	end

OakLab_EventScript_PlayerStepBack::
	applymovement OBJ_EVENT_ID_PLAYER, OakLab_Movement_PlayerStepBack
	waitmovement 0
	turnobject LOCALID_PROF_OAK, DIR_SOUTH
	return

OakLab_EventScript_SummonRegirock::
	setvar VAR_0x8008, 11
	setvar VAR_0x8009, 8
	call OakLab_EventScript_OpenCineBallAt
	clearflag FLAG_TEMP_11
	addobject LOCALID_REGIROCK
	call OakLab_EventScript_RegiEnergyWave
	playmoncry SPECIES_REGIROCK, CRY_MODE_ENCOUNTER
	waitmoncry
	return

OakLab_EventScript_SummonRegice::
	setvar VAR_0x8008, 12
	setvar VAR_0x8009, 8
	call OakLab_EventScript_OpenCineBallAt
	clearflag FLAG_TEMP_12
	addobject LOCALID_REGICE
	call OakLab_EventScript_RegiEnergyWave
	playmoncry SPECIES_REGICE, CRY_MODE_ENCOUNTER
	waitmoncry
	return

OakLab_EventScript_SummonRegisteel::
	setvar VAR_0x8008, 13
	setvar VAR_0x8009, 8
	call OakLab_EventScript_OpenCineBallAt
	clearflag FLAG_TEMP_13
	addobject LOCALID_REGISTEEL
	call OakLab_EventScript_RegiEnergyWave
	playmoncry SPECIES_REGISTEEL, CRY_MODE_ENCOUNTER
	waitmoncry
	return

OakLab_EventScript_SummonRegieleki::
	setvar VAR_0x8008, 14
	setvar VAR_0x8009, 8
	call OakLab_EventScript_OpenCineBallAt
	clearflag FLAG_TEMP_14
	addobject LOCALID_REGIELEKI
	call OakLab_EventScript_RegiEnergyWave
	playmoncry SPECIES_REGIELEKI, CRY_MODE_ENCOUNTER
	waitmoncry
	return

OakLab_EventScript_SummonRegidrago::
	setvar VAR_0x8008, 15
	setvar VAR_0x8009, 8
	call OakLab_EventScript_OpenCineBallAt
	clearflag FLAG_TEMP_15
	addobject LOCALID_REGIDRAGO
	call OakLab_EventScript_RegiEnergyWave
	playmoncry SPECIES_REGIDRAGO, CRY_MODE_ENCOUNTER
	waitmoncry
	return

OakLab_EventScript_SummonRegigigas::
	setvar VAR_0x8008, 13
	setvar VAR_0x8009, 7
	call OakLab_EventScript_OpenCineBallAt
	clearflag FLAG_TEMP_17
	addobject LOCALID_REGIGIGAS
	call OakLab_EventScript_RegiEnergyWave
	playmoncry SPECIES_REGIGIGAS, CRY_MODE_ENCOUNTER
	waitmoncry
	return

OakLab_EventScript_OpenCineBallAt::
	delay 10
	clearflag FLAG_TEMP_16
	addobject LOCALID_CINE_BALL
	setobjectxy LOCALID_CINE_BALL, VAR_0x8008, VAR_0x8009
	playse SE_BALL_OPEN
	delay 20
	removeobject LOCALID_CINE_BALL
	setflag FLAG_TEMP_16
	return

OakLab_EventScript_RegiEnergyWave::
	playse SE_M_SOLAR_BEAM
	setvar VAR_0x8004, 1
	setvar VAR_0x8005, 1
	setvar VAR_0x8006, 8
	setvar VAR_0x8007, 3
	special ShakeCamera
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_ExclamationMark
	waitmovement 0
	delay 16
	return

OakLab_Movement_PlayerStepBack:
	walk_down
	walk_down
	walk_down
	walk_down
	face_up
	step_end

OakLab_Text_Intro:
	.string "Hey {PLAYER}.\p"
	.string "Haha, wondering how I know you?\n"
	.string "Professor Birch told me about you.\p"
	.string "Not to mention, you've certainly\n"
	.string "made a name for yourself as a\l"
	.string "Trainer. You're nearing the top\l"
	.string "ranks these days.\p"
	.string "Listen, I have a special request\n"
	.string "for you. I need some help with my\l"
	.string "research. It may take a while, but\l"
	.string "I promise it'll be worth it.\p"
	.string "Will you do it?$"

OakLab_Text_WhatAShame:
	.string "What a shame…$"

OakLab_Text_QuestAccepted:
	.string "Quest accepted!$"

OakLab_Text_CatchEveryRegi:
	.string "I've gotten information on a\n"
	.string "secret new Regi that may\l"
	.string "completely change what we know\l"
	.string "about the Regis.\p"
	.string "I need you to catch every known\n"
	.string "Regi and come back to me with\l"
	.string "all of them in your party.$"

OakLab_Text_DidYouCatchThem:
	.string "Good to see you again, {PLAYER}.\n"
	.string "Did you end up catching those\l"
	.string "Regis? Let me check.$"

OakLab_Text_StillMissingRegis:
	.string "Hmm… you're still missing some.\n"
	.string "Bring me every known Regi!$"

OakLab_Text_AmazingWork:
	.string "Amazing work! Now, give me a\n"
	.string "second…$"

OakLab_Text_ScienceCantWait:
	.string "Okay, {PLAYER}, science can't\n"
	.string "wait! I'm going to do it, are you\l"
	.string "ready?$"

OakLab_Text_PrepareForBattle:
	.string "Oh yes, I didn't tell you.\n"
	.string "The short part is, prepare for\l"
	.string "battle! Take these.$"

OakLab_Text_IncredibleWork:
	.string "Incredible work, {PLAYER}!\n"
	.string "I must study this, immediately.\l"
	.string "Give me some time.$"

OakLab_Text_GiveMeSomeTime:
	.string "Give me some time.$"

OakLab_Text_StillStudying:
	.string "I'm still studying that specimen.\n"
	.string "Come back later, {PLAYER}.$"
