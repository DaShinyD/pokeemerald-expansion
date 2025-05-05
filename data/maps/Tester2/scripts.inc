.set LOCALID_FLOYD, 2
.set LOCALID_CHAUVIN, 1
.set LOCALID_COP_1, 4
.set LOCALID_COP_2, 5
.set LOCALID_ITEM, 6

Tester2_MapScripts::
	.byte 0

Tester2_EventScript_Test::
	lock
	removeobject LOCALID_FLOYD
	removeobject LOCALID_ITEM
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox Tester2_Text_Test, MSGBOX_AUTOCLOSE
	removefieldmugshot
	addobject LOCALID_FLOYD
	applymovement LOCALID_CHAUVIN, Tester2_Movement_PullOut
	applymovement LOCALID_FLOYD, Tester2_Movement_PullOut
	applymovement LOCALID_COP_1, Tester2_Movement_PullOut
	applymovement LOCALID_COP_2, Tester2_Movement_PullOut
	waitmovement 0
	createfieldmugshot MUGSHOT_CHAUVIN
	msgbox Tester2_Text_Test2, MSGBOX_AUTOCLOSE
	removefieldmugshot
	applymovement LOCALID_CHAUVIN, Tester2_Movement_JumpOnFloyd
	waitmovement 0
	createfieldmugshot MUGSHOT_FLOYD
	msgbox Tester2_Text_Test3, MSGBOX_AUTOCLOSE
	removefieldmugshot
	addobject LOCALID_ITEM
	applymovement LOCALID_ITEM, Tester2_Movement_BallThrow
	waitmovement 0
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_CHAUVIN
	removeobject LOCALID_COP_1
	removeobject LOCALID_COP_2
	removeobject LOCALID_FLOYD
	setflag FLAG_FLOYD
	setflag FLAG_FLOYD_COPS
	fadescreen FADE_FROM_BLACK
	release
	end

Tester2_Text_Test:
	.string "Get down criminal!\p"
	.string "Pull this thief out.$"

Tester2_Movement_PullOut:
	lock_facing_direction
	walk_slow_down
	unlock_facing_direction
	step_end

Tester2_Text_Test2:
	.string "Stop resisting!$"

Tester2_Movement_JumpOnFloyd:
	jump_up
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	jump_in_place_down
	step_end

Tester2_Text_Test3:
	.string "Stop stop!\p"
	.string "I dindu nuffin!\p"
	.string "I...can't...breeve...$"

Tester2_Movement_BallThrow:
	walk_faster_down
	walk_faster_down
	walk_faster_down
	step_end

Tester2_EventScript_Fent::
	finditem ITEM_FENT_BAG
	end

