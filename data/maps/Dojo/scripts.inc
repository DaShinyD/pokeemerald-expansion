Dojo_MapScripts::
	.byte 0

Dojo_EventScript_Master1::
	trainerbattle_single TRAINER_KARATE_1, Dojo_Text_Master1Intro, Dojo_Text_Master1Defeat
	msgbox Dojo_Text_Master1After, MSGBOX_AUTOCLOSE
	end

Dojo_EventScript_Master2::
	trainerbattle_single TRAINER_KARATE_2, Dojo_Text_Master2Intro, Dojo_Text_Master2Defeat
	msgbox Dojo_Text_Master2After, MSGBOX_AUTOCLOSE
	end

Dojo_Text_Master1Intro:
	.string "HAH HOO HAH.\p"
	.string "Come, fight me.$"

Dojo_Text_Master1Defeat:
	.string "HAH HOO...HUH?$"

Dojo_Text_Master1After:
	.string "I'm weak.$"

Dojo_Text_Master2Intro:
	.string "HAH HOO HAH.\p"
	.string "Taste defeat.$"

Dojo_Text_Master2Defeat:
	.string "HAH HOO...HRNGH?$"

Dojo_Text_Master2After:
	.string "Get out of my face.$"

