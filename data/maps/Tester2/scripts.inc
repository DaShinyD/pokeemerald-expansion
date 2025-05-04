Tester2_MapScripts::
	.byte 0

Tester2_EventScript_Test::
	createfieldmugshot MUGSHOT_GOKU
	playse SE_DRIPKU
	msgbox Tester2_Text_AttentionDuelist, MSGBOX_NPC
	removefieldmugshot
	waitse
	end

Tester2_Text_AttentionDuelist:
	.string "BOO!$"

