.set LOCALID_NURSE, 1

CeladonCity_Center_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CeladonCity_Center_OnTransition
	.byte 0

CeladonCity_Center_OnTransition:
	setrespawn HEAL_LOCATION_CELADON_CITY
	end

CeladonCity_Center_EventScript_Nurse::
	lock
	msgbox CeladonCity_Center_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox CeladonCity_Center_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, CeladonCity_Center_Movement_Turn
	release
	end

CeladonCity_Center_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

CeladonCity_Center_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

CeladonCity_Center_Movement_Turn:
	face_down
	step_end

