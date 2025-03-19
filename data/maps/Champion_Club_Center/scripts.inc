.set LOCALID_NURSE, 1

Champion_Club_Center_MapScripts::
	.byte 0

Champion_Club_Center_EventScript_Nurse::
	setvar VAR_0x800B, LOCALID_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end

