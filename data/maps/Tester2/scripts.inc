.set LOCALID_MON, 1
.set LOCALID_TEST, 2

Tester2_MapScripts::
	.byte 0

Tester2_EventScript_Test::
	removeobject LOCALID_TEST
	setflag FLAG_TEST_OBJECTS
	setvar VAR_0x8004, LOCALID_MON
    setvar VAR_0x8005, 6
    special StartRangerCapture
    waitstate
	compare VAR_RESULT, 1
	goto_if_eq Tester2_EventScript_Success
	goto Tester2_EventScript_Fail
	end

Tester2_EventScript_Success::
	lock
	msgbox Tester2_Text_Success, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_TEST_OBJECTS
	release
	end

Tester2_EventScript_Fail::
	lock
	msgbox Tester2_Text_Fail, MSGBOX_DEFAULT
	closemessage
	clearflag FLAG_TEST_OBJECTS
	release
	end

Tester2_Text_Success:
	.string "You got it!$"

Tester2_Text_Fail:
	.string "Fail!$"

