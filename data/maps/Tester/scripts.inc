.set LOCALID_MD_START, 1

Tester_MapScripts::
	.byte 0

Tester_EventScript_MysteryDungeonGuide::
	lock
	faceplayer
	msgbox Tester_Text_MdAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Tester_EventScript_MdCancel
	message Tester_Text_MdPickStarter
	waitmessage
	multichoice 18, 0, MULTI_MYSTERY_DUNGEON_STARTER, FALSE
	@ Anything past the three starters is the EXIT row or a B press.
	goto_if_lt VAR_RESULT, 3, Tester_EventScript_MdStart
	goto Tester_EventScript_MdCancel

Tester_EventScript_MdStart::
	copyvar VAR_0x8004, VAR_RESULT
	closemessage
	hidefollower
	helpwindow HELP_MD_FIND_FLOOR
	special StartMysteryDungeon
	waitstate
	goto_if_eq VAR_RESULT, TRUE, Tester_EventScript_MdCleared
	msgbox Tester_Text_MdFailed, MSGBOX_DEFAULT
	release
	end

Tester_EventScript_MdCleared::
	msgbox Tester_Text_MdCleared, MSGBOX_DEFAULT
	release
	end

Tester_EventScript_MdCancel::
	closemessage
	msgbox Tester_Text_MdCancel, MSGBOX_DEFAULT
	release
	end

Tester_Text_MdAsk:
	.string "Want to try a Mystery Dungeon?$"

Tester_Text_MdPickStarter:
	.string "Who would you like to become?$"

Tester_Text_MdCancel:
	.string "Come back if you change your mind.$"

Tester_Text_MdCleared:
	.string "You reached the next floor!\n"
	.string "That's the end of this trial.$"

Tester_Text_MdFailed:
	.string "The expedition failed…\n"
	.string "Talk to me when you want to try again.$"
