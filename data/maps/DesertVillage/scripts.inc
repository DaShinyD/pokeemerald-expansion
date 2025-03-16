
.set LOCALID_EXPLORER, 9

DesertVillage_MapScripts::
	.byte 0

DesertVillage_EventScript_Leave::
	lock
	msgbox DesertVillage_Text_Leave, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DesertVillage_EventScript_WarpBack
	applymovement OBJ_EVENT_ID_PLAYER, DesertVillage_Movement_StepBack
	waitmovement 0
	release
	end

DesertVillage_Text_Leave:
	.string "Would you like to leave?$"

DesertVillage_Movement_StepBack:
	walk_up
	step_end

DesertVillage_EventScript_WarpBack::
	warp MAP_ROUTE111, 32, 52
	waitstate
	release
	end

DesertVillage_EventScript_Sandslash::
	msgbox DesertVillage_Text_Sandslash, MSGBOX_NPC
	end

DesertVillage_Text_Sandslash:
	.string "Slash, Slash, SLASH!$"

DesertVillage_EventScript_NPC_1::
	msgbox DesertVillage_Text_LoveTea, MSGBOX_NPC
	end

DesertVillage_Text_LoveTea:
	.string "I love tea, but it's so hard to\n"
	.string "drink during sandstorms.$"

DesertVillage_EventScript_NPC_2::
	msgbox DesertVillage_Text_WeSecluded, MSGBOX_NPC
	end

DesertVillage_Text_WeSecluded:
	.string "Even without that Pokemon, we are\n"
	.string "usually very secluded.\p"
	.string "I guess people don't like living\n"
	.string "in sand.$"

DesertVillage_EventScript_NPC_3::
	msgbox DesertVillage_Text_Watering, MSGBOX_NPC
	end

DesertVillage_Text_Watering:
	.string "I'm watering my plants.$"

DesertVillage_EventScript_NPC_4::
	msgbox DesertVillage_Text_Staring, MSGBOX_NPC
	end

DesertVillage_Text_Staring:
	.string "If I stare into the horizon long\n"
	.string "enough, I can see mirages.$"

DesertVillage_EventScript_NPC_5::
	msgbox DesertVillage_Text_SoupYum, MSGBOX_NPC
	end

DesertVillage_Text_SoupYum:
	.string "This soup is delicious.$"

DesertVillage_EventScript_NPC_6::
	msgbox DesertVillage_Text_NeedSomethingToDo, MSGBOX_NPC
	end

DesertVillage_Text_NeedSomethingToDo:
	.string "I'm so bored, there's nothing to\n"
	.string "do around here.$"

DesertVillage_EventScript_NPC_7::
	msgbox DesertVillage_Text_HateSand, MSGBOX_NPC
	end

DesertVillage_Text_HateSand:
	.string "I hate sand.\p"
	.string "It's rough, coarse, and it gets\n"
	.string "everywhere.\p"
	.string "Then why am I here?\p"
	.string "Don't get meta on me.$"

DesertVillage_EventScript_Explorer::
	lock
	faceplayer
	goto_if_set FLAG_MET_EXPLORER, DesertVillage_EventScript_ExplorerGoAlready
	msgbox DesertVillage_Text_Hello, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_EXPLORER, DesertVillage_Movement_Ponder
	waitmovement 0
	msgbox DesertVillage_Text_Okay, MSGBOX_DEFAULT
	setflag FLAG_MET_EXPLORER
	warp MAP_ROUTE135, 17, 26
	waitstate
	release
	end

DesertVillage_Text_Hello:
	.string "Hello there.\p"
	.string "You're new, who are you?\p"
	.string "...\p"
	.string "I see. You are here to help.$"

DesertVillage_Movement_Ponder:
	face_up
	delay_16
	face_down
	step_end

DesertVillage_Text_Okay:
	.string "Okay.\p"
	.string "I'll show you to the pyramid.\p"
	.string "I warn you, the Pokemon is tough.\p"
	.string "It seems to be a legendary from\n"
	.string "another region.\p"
	.string "Let's go.$"

DesertVillage_EventScript_ExplorerGoAlready:
	msgbox DesertVillage_Text_GoBack, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, DesertVillage_EventScript_BackToPyramid
	msgbox DesertVillage_Text_LetMeKnow, MSGBOX_DEFAULT
	closemessage
	release
	end

DesertVillage_Text_GoBack:
	.string "Would you like to go back to the\n"
	.string "pyramid?$"

DesertVillage_Text_LetMeKnow:
	.string "Alright, let me know if you do.$"

DesertVillage_EventScript_BackToPyramid:
	warp MAP_ROUTE135, 17, 26
	waitstate
	release
	end

