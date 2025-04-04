.set LOCALID_NURSE, 1

Champion_Club_Center_MapScripts::
	.byte 0

Champion_Club_Center_EventScript_Nurse::
	lock
	msgbox Champion_Club_Center_Text_IHeal, MSGBOX_AUTOCLOSE
	call Common_EventScript_OutOfCenterPartyHeal
	msgbox Champion_Club_Center_Text_AllDone, MSGBOX_AUTOCLOSE
	applymovement OBJ_EVENT_ID_PLAYER, Champion_Club_Center_Movement_Turn
	release
	end

Champion_Club_Center_Text_IHeal:
	.string "Hello, I'll heal your Pokemon.$"

Champion_Club_Center_Text_AllDone:
	.string "There you go.\p"
	.string "Have a good day!$"

Champion_Club_Center_Movement_Turn:
	face_down
	step_end

