LavenderTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LavenderTown_OnTransition
	.byte 0

LavenderTown_OnTransition:
	setflag FLAG_VISITED_LAVENDER
	end

LavenderTown_EventScript_OldWoman::
	msgbox LavenderTown_Text_OldWoman, MSGBOX_NPC
	end

LavenderTown_EventScript_CamperGirl::
	msgbox LavenderTown_Text_CamperGirl, MSGBOX_NPC
	end

LavenderTown_EventScript_LittleGirl::
	msgbox LavenderTown_Text_LittleGirl, MSGBOX_NPC
	end

LavenderTown_EventScript_Locked::
	msgbox LavenderTown_Text_Locked, MSGBOX_SIGN
	end

LavenderTown_EventScript_TownSign::
	msgbox LavenderTown_Text_TownSign, MSGBOX_SIGN
	end

LavenderTown_EventScript_RouteSign::
	msgbox LavenderTown_Text_RouteSign, MSGBOX_SIGN
	end

LavenderTown_Text_OldWoman:
	.string "I hear that they might be replacing the\n"
	.string "Lavender Tower with a radio tower.\p"
	.string "How awful would that be?$"

LavenderTown_Text_CamperGirl:
	.string "I came here from Celadon.\p"
	.string "There isn't much to do...$"

LavenderTown_Text_LittleGirl:
	.string "Do you believe in ghosts?\p"
	.string "Oh, then don't go in the tower.$"

LavenderTown_Text_Locked:
	.string "It's locked.$"

LavenderTown_Text_TownSign:
.string "LAVENDER TOWN:\n"
.string "IT'S SPOOKY!$"

LavenderTown_Text_RouteSign:
	.string "To Route 8.$"

