Tester2_MapScripts::
	.byte 0

Tester2_EventScript_Test::
	lock
	faceplayer
	msgbox Tester2_Text_HaveYou, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8000, 0
	callnative OpenFullscreenPopupBgFromScript
	closemessage
	release
	end

Tester2_Text_HaveYou:
	.string "Have you seen this man?$"

