.set LOCALID_RYE, 3

GraniteCave_1F_MapScripts::
	.byte 0

GraniteCave_1F_EventScript_Hiker::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_HM_FLASH, GraniteCave_1F_EventScript_ReceivedFlash
	msgbox GraniteCave_1F_Text_GetsDarkAheadHereYouGo, MSGBOX_DEFAULT
	giveitem ITEM_HM_FLASH
	setflag FLAG_RECEIVED_HM_FLASH
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_EventScript_ReceivedFlash::
	msgbox GraniteCave_1F_Text_ExplainFlash, MSGBOX_DEFAULT
	release
	end

GraniteCave_1F_Text_GetsDarkAheadHereYouGo:
	.string "Hey, you.\n"
	.string "It gets awfully dark ahead.\l"
	.string "It'll be tough trying to explore.\p"
	.string "That guy who came by earlier…\n"
	.string "STEVEN, I think it was.\p"
	.string "He knew how to use FLASH, so he ought\n"
	.string "to be all right, but…\p"
	.string "Well, for us HIKERS, helping out those\n"
	.string "that we meet is our motto.\p"
	.string "Here you go, I'll pass this on to you.$"

GraniteCave_1F_Text_ExplainFlash:
	.string "Teach that hidden move FLASH to\n"
	.string "a POKéMON and use it.\p"
	.string "It lights up even the inky darkness\n"
	.string "of caves.\p"
	.string "But, to use it, you need the GYM BADGE\n"
	.string "from DEWFORD's POKéMON GYM.$"

GraniteCave_1F_EventScript_Rye::
	lock
	playse SE_PIN
	applymovement LOCALID_RYE, Common_Movement_ExclamationMark
	playbgm MUS_ENCOUNTER_BRENDAN, TRUE
	waitmovement 0
	applymovement LOCALID_RYE, GraniteCave_1F_Movement_RyeNotices
	waitmovement 0
	applymovement LOCALID_RYE, GraniteCave_1F_Movement_RyeApproaches
	waitmovement 0
	msgbox GraniteCave_1F_Text_RyeGreet, MSGBOX_DEFAULT
	waitmessage
	goto GraniteCave_1F_EventScript_RyeBattle
	end

GraniteCave_1F_EventScript_RyeBattle::
	trainerbattle_no_intro TRAINER_RYE_GRANITE_CAVE, GraniteCave_1F_Text_RyeDefeat
	goto GraniteCave_1F_EventScript_RyeBye
	end

GraniteCave_1F_EventScript_RyeBye::
	msgbox GraniteCave_1F_Text_RyeSeeYa, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RYE, GraniteCave_1F_Movement_RyeLeaves
	waitmovement 0
	removeobject LOCALID_RYE
	setflag FLAG_RYE_GRANITE
	setvar VAR_RYE_GRANITE, 1
	fadedefaultbgm
	release
	end

GraniteCave_1F_Movement_RyeNotices:
	face_up
	step_end

GraniteCave_1F_Movement_RyeApproaches:
	walk_up
	walk_up
	walk_right
	step_end

GraniteCave_1F_Movement_RyeLeaves:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

GraniteCave_1F_Text_RyeGreet:
	.string "What's going on. The name is\n"
	.string "Rye. I'm a trainer doing the\p"
	.string "Pokemon League challenge.\p"
	.string "Who are you?\p"
	.string "{PLAYER}? Cool name.\p"
	.string "You're looking for a guy named\n"
	.string "Steven? He's just ahead.\p"
	.string "But first, you're gonna have\n"
	.string "a battle with me!$"

GraniteCave_1F_Text_RyeDefeat:
	.string "Unreal.$"

GraniteCave_1F_Text_RyeSeeYa:
	.string "Awesome battle.\p"
	.string "I'm sure we will see more of\n"
	.string "each other.\p"
	.string "For now, I've gotta go.\p"
	.string "See you later, {PLAYER}.$"


