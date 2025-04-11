Route5_MapScripts::
	.byte 0

Route5_EventScript_Guards::
	msgbox Route5_Text_NoPass, MSGBOX_NPC
	end

Route5_Text_NoPass:
	.string "Saffron City is absolutely off\n"
	.string "limits.\p"
	.string "No way the Emerald tiles can make\n"
	.string "it even passable looking.$"

Route5_EventScript_DaycareSign::
	msgbox Route5_Text_DaycareSign, MSGBOX_SIGN
	end

Route5_Text_DaycareSign:
	.string "DAYCARE MOVED TO\n"
	.string "FOUR ISLAND$"

Route5_EventScript_RouteSign::
	msgbox Route5_Text_RouteSign, MSGBOX_SIGN
	end

Route5_Text_RouteSign:
	.string "UNDERGROUND PATH$"

