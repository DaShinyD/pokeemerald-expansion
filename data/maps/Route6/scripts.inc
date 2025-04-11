Route6_MapScripts::
	.byte 0

Route6_EventScript_Guards::
	msgbox Route6_Text_NoPass, MSGBOX_NPC
	end

Route6_Text_NoPass:
	.string "Saffron City is absolutely off\n"
	.string "limits.\p"
	.string "No way the Emerald tiles can make\n"
	.string "it even passable looking.$"

Route6_EventScript_Swimmer::
	msgbox Route6_Text_Swimmer, MSGBOX_NPC
	end

Route6_Text_Swimmer:
	.string "It's either this or the ocean.\p"
	.string "This doesn't have Tentacruel.$"

Route6_EventScript_BugCatcher::
	msgbox Route6_Text_BugCatcher, MSGBOX_NPC
	end

Route6_Text_BugCatcher:
	.string "Kanto hasn't imported new bug\n"
	.string "Pokemon yet.\p"
	.string "Maybe I should move to Hoenn.$"

Route6_EventScript_CoupleMan::
	msgbox Route6_Text_CoupleMan, MSGBOX_NPC
	end

Route6_Text_CoupleMan:
	.string "I brought Ribs, mmm.$"

Route6_EventScript_CoupleWoman::
	msgbox Route6_Text_CoupleWoman, MSGBOX_NPC
	end

Route6_Text_CoupleWoman:
	.string "The man brought Ribs to a picnic\n"
	.string "spot with no grill.$"

Route6_EventScript_RouteSign::
	msgbox Route6_Text_RouteSign, MSGBOX_SIGN
	end

Route6_Text_RouteSign:
	.string "UNDERGROUND PATH$"

Route6_EventScript_SaffronSign::
	msgbox Route6_Text_SaffronSign, MSGBOX_SIGN
	end

Route6_Text_SaffronSign:
	.string "TO SAFFRON CITY$"

