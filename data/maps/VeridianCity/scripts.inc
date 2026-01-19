VeridianCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VeridianCity_OnTransition
	.byte 0

VeridianCity_OnTransition:
	setflag FLAG_VISITED_VIRIDIAN
	end

VeridianCity_EventScript_Locked::
	msgbox VeridianCity_Text_Locked, MSGBOX_SIGN
	end

VeridianCity_Text_Locked:
	.string "It's locked.$"

VeridianCity_EventScript_SchoolGuards::
	msgbox VeridianCity_Text_SchoolSession, MSGBOX_NPC
	end

VeridianCity_Text_SchoolSession:
	.string "School is currently in session.$"

VeridianCity_EventScript_Route1Guards::
	msgbox VeridianCity_Text_Route1Guards, MSGBOX_NPC
	end

VeridianCity_Text_Route1Guards:
	.string "Sorry this way is blocked.\p"
	.string "We ran out of excuses so leave.$"

VeridianCity_EventScript_Route24Guards::
	msgbox VeridianCity_Text_Route24Guards, MSGBOX_NPC
	end

VeridianCity_Text_Route24Guards:
	.string "Construction is going on this way.\p"
	.string "The Pokemon League is now in\n"
	.string "Viridian.$"

VeridianCity_EventScript_OldGardener::
	msgbox VeridianCity_Text_OldGardener, MSGBOX_NPC
	end

VeridianCity_Text_OldGardener:
	.string "Gardening is so peaceful.$"

VeridianCity_EventScript_Fisher::
	msgbox VeridianCity_Text_Fisher, MSGBOX_NPC
	end

VeridianCity_Text_Fisher:
	.string "There aren't any Pokemon here!$"

VeridianCity_EventScript_FatMan::
	msgbox VeridianCity_Text_FatMan, MSGBOX_NPC
	end

VeridianCity_Text_FatMan:
	.string "Mmm mmph mm.\p"
	.string "I love burgers.$"

VeridianCity_EventScript_TownSign::
	msgbox VeridianCity_Text_TownSign, MSGBOX_SIGN
	end

VeridianCity_Text_TownSign:
	.string "VIRIDIAN CITY:\n"
	.string "SPELT WITH 3 Is??$"

VeridianCity_EventScript_BugCatcher::
	msgbox VeridianCity_Text_BugCatcher, MSGBOX_NPC
	end

VeridianCity_Text_BugCatcher:
	.string "I can't find any bugs!$"

VeridianCity_EventScript_OldWoman::
	msgbox VeridianCity_Text_OldWoman, MSGBOX_NPC
	end

VeridianCity_Text_OldWoman:
	.string "This city has changed so much.\p"
	.string "It used to be half the size.\p"
	.string "Now, we even have the Pokemon\n"
	.string "League here.$"

VeridianCity_EventScript_Youngster::
	msgbox VeridianCity_Text_Youngster, MSGBOX_NPC
	end

VeridianCity_Text_Youngster:
	.string "One day, I'm gonna challenge the\n"
	.string "Pokemon League.$"

VeridianCity_EventScript_GymSign::
	msgbox VeridianCity_Text_GymSign, MSGBOX_SIGN
	end

VeridianCity_Text_GymSign:
	.string "VIRIDIAN GYM:\n"
	.string "BLUE THE PALLET TOWN POWER$"

