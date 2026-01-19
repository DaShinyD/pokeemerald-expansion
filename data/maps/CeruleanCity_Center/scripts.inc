.set LOCALID_NURSE, 1

CeruleanCity_Center_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeruleanCity_Center_OnTransition
	.byte 0

CeruleanCity_Center_OnTransition:
	setrespawn HEAL_LOCATION_CERULEAN_CITY
	end

CeruleanCity_Center_EventScript_Nurse::
	lock
	msgbox CeruleanCity_Center_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox CeruleanCity_Center_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, CeruleanCity_Center_Movement_Turn
	release
	end

CeruleanCity_Center_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

CeruleanCity_Center_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

CeruleanCity_Center_Movement_Turn:
	face_down
	step_end
