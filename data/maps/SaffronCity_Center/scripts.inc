.set LOCALID_NURSE, 1

SaffronCity_Center_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SaffronCity_Center_OnTransition
	.byte 0

SaffronCity_Center_OnTransition:
	setrespawn HEAL_LOCATION_SAFFRON_CITY
	end

SaffronCity_Center_EventScript_Nurse::
	lock
	msgbox SaffronCity_Center_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox SaffronCity_Center_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, SaffronCity_Center_Movement_Turn
	release
	end

SaffronCity_Center_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

SaffronCity_Center_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

SaffronCity_Center_Movement_Turn:
	face_down
	step_end

