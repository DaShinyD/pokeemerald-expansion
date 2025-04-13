Route2_MapScripts::
	.byte 0

Route2_EventScript_Youngster::
	msgbox Route2_Text_Youngster, MSGBOX_NPC
	end

Route2_Text_Youngster:
	.string "I need more Rattatas!$"

Route2_EventScript_OldMan::
	msgbox Route2_Text_OldMan, MSGBOX_NPC
	end

Route2_Text_OldMan:
	.string "I'm GRUMPY!$"

Route2_EventScript_OldMan2::
	msgbox Route2_Text_OldMan2, MSGBOX_NPC
	end

Route2_Text_OldMan2:
	.string "I'm GRUMPIER!$"

Route2_EventScript_Locked::
	msgbox Route2_Text_Locked, MSGBOX_SIGN
	end

Route2_Text_Locked:
	.string "It's locked.$"

Route2_EventScript_Sign::
	msgbox Route2_Text_Sign, MSGBOX_SIGN
	end

Route2_Text_Sign:
	.string "I HATE my neighbor.$"

