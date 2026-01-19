CeruleanCity_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_OnTransition
	.byte 0

CeruleanCity_OnTransition:
	setflag FLAG_VISITED_CERULEAN
	end

CeruleanCity_EventScript_Locked::
	msgbox CeruleanCity_Text_Locked, MSGBOX_SIGN
	end

CeruleanCity_Text_Locked:
	.string "It's locked.$"

CeruleanCity_EventScript_NoPass1::
	msgbox CeruleanCity_Text_NoPass1, MSGBOX_NPC
	end

CeruleanCity_Text_NoPass1:
	.string "Sorry, Route 9 is blocked off due\n"
	.string "to a Power Plant issue.$"

CeruleanCity_EventScript_NoPass2::
	msgbox CeruleanCity_Text_NoPass2, MSGBOX_NPC
	end

CeruleanCity_Text_NoPass2:
	.string "Unfortunately, the bridge has\n"
	.string "flooded.\p"
	.string "Everyone has been evacuated.\p"
	.string "You can still reach the cave by\n"
	.string "going West.$"

CeruleanCity_EventScript_CitySign::
	msgbox CeruleanCity_Text_CitySign, MSGBOX_SIGN
	end

CeruleanCity_Text_CitySign:
	.string "CERULEAN CITY:\n"
	.string "THE CITY OF WATER$"

CeruleanCity_EventScript_GymSign::
	msgbox CeruleanCity_Text_GymSign, MSGBOX_SIGN
	end

CeruleanCity_Text_GymSign:
	.string "CERULEAN GYM:\n"
	.string "HOME OF THE MERMAID$"

CeruleanCity_EventScript_SlowbroGirl::
	msgbox CeruleanCity_Text_SlowbroGirl, MSGBOX_NPC
	end

CeruleanCity_Text_SlowbroGirl:
	.string "My Slowbro never listens to me.$"

CeruleanCity_EventScript_OldMan::
	msgbox CeruleanCity_Text_OldMan, MSGBOX_NPC
	end

CeruleanCity_Text_OldMan:
	.string "I sure hope no one was trapped\n"
	.string "on the other side of the bridge.$"

CeruleanCity_EventScript_Youngster::
	msgbox CeruleanCity_Text_Youngster, MSGBOX_NPC
	end

CeruleanCity_Text_Youngster:
	.string "Misty is so...pretty.$"

