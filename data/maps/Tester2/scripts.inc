Tester2_MapScripts::
	.byte 0

@ Object 1: Test Dash vs Rye scripted battle
Tester2_EventScript_DashVsRyeBattle::
	lock
	faceplayer
	msgbox Tester2_Text_StartScriptedBattle, MSGBOX_YESNO
	closemessage
	goto_if_eq VAR_RESULT, NO, Tester2_EventScript_DashVsRyeCancel
	special StartDashVsRyeScriptedBattle
	waitstate
	goto_if_eq VAR_RESULT, 1, Tester2_EventScript_DashWon
	goto Tester2_EventScript_RyeWon
Tester2_EventScript_DashWon::
	msgbox Tester2_Text_DashWon, MSGBOX_DEFAULT
	goto Tester2_EventScript_DashVsRyeCancel
Tester2_EventScript_RyeWon::
	msgbox Tester2_Text_RyeWon, MSGBOX_DEFAULT
Tester2_EventScript_DashVsRyeCancel::
	release
	end

Tester2_Text_StartScriptedBattle:
	.string "Start the scripted battle\n"
	.string "Dash vs Rye?$"

Tester2_Text_DashWon:
	.string "Dash won!$"

Tester2_Text_RyeWon:
	.string "Rye won!$"

@ Object 2: Test ranger minigame (target = object 3)
Tester2_EventScript_RangerMinigame::
	lock
	faceplayer
	msgbox Tester2_Text_StartRangerMinigame, MSGBOX_YESNO
	closemessage
	goto_if_eq VAR_RESULT, NO, Tester2_EventScript_RangerCancel
	setvar VAR_0x8004, 3
	setvar VAR_0x8005, 0
	special StartRangerCapture
	waitstate
	goto_if_eq VAR_RESULT, TRUE, Tester2_EventScript_RangerSuccess
	msgbox Tester2_Text_RangerFailed, MSGBOX_DEFAULT
	goto Tester2_EventScript_RangerDone
Tester2_EventScript_RangerSuccess::
	msgbox Tester2_Text_RangerSuccess, MSGBOX_DEFAULT
Tester2_EventScript_RangerDone::
Tester2_EventScript_RangerCancel::
	release
	end

Tester2_Text_StartRangerMinigame:
	.string "Start the ranger minigame?\n"
	.string "Target: the third object.$"

Tester2_Text_RangerSuccess:
	.string "Capture successful!$"

Tester2_Text_RangerFailed:
	.string "Capture failed or cancelled.$"
