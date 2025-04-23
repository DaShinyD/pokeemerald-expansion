Tester2_MapScripts::
	.byte 0

Tester2_EventScript_BattleTest::
	lock
	faceplayer
	trainerbattle_no_intro TRAINER_RED, Tester2_Text_Done
	release
	end

Tester2_Text_Done:
	.string "Battle test done.$"

