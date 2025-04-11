.set LOCALID_SAILOR, 1
.set LOCALID_SS_TIDAL, 2

VermillionHarber_MapScripts::
	.byte 0

VermillionHarber_EventScript_Sailor::
	lock
	faceplayer
	msgbox VermillionHarber_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, VermillionHarber_EventScript_AsYouLike
	msgbox EventTicket_Text_SailHome, MSGBOX_DEFAULT
	closemessage
	applymovement VAR_LAST_TALKED, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	delay 30
	hideobjectat LOCALID_SAILOR, MAP_VERMILLION_HARBER
	setvar VAR_0x8004, LOCALID_SS_TIDAL
	call Common_EventScript_FerryDepartIsland
	warp MAP_LILYCOVE_CITY_HARBOR, 8, 11
	waitstate
	release
	end

VermillionHarber_EventScript_AsYouLike::
	msgbox EventTicket_Text_AsYouLike, MSGBOX_DEFAULT
	release
	end

VermillionHarber_Text_SailorReturn:
	.string "Do you want to return to Lilycove?$"

