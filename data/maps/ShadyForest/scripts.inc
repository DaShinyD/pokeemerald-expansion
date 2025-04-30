.set LOCALID_CELEBI, 2
.set LOCALID_GHOST_1, 3
.set LOCALID_GHOST_2, 4
.set LOCALID_GHOST_3, 5

ShadyForest_MapScripts::
	.byte 0

ShadyForest_EventScript_Celebi::
	lock
	msgbox ShadyForest_Text_Pray, MSGBOX_DEFAULT
	playmoncry SPECIES_CELEBI, CRY_MODE_ENCOUNTER
	waitmoncry
	applymovement LOCALID_CELEBI, ShadyForest_Movement_Approach
	waitmovement 0
	playmoncry SPECIES_CELEBI, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_CELEBI, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	removeobject LOCALID_CELEBI
	setobjectxy LOCALID_CELEBI, 68, 0
	addobject LOCALID_CELEBI
	release
	end

ShadyForest_Text_Pray:
	.string "You pray to the Time Shrine.$"

ShadyForest_Movement_Approach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

ShadyForest_EventScript_GoAway::
	msgbox ShadyForest_Text_GoAway, MSGBOX_SIGN
	end

ShadyForest_Text_GoAway:
	.string "Go...awaaay.$"

ShadyForest_EventScript_Ghost1::
	lock
	applymovement LOCALID_GHOST_1, ShadyForest_Movement_Disappear
	waitmovement 0
	release
	end

ShadyForest_EventScript_Ghost2::
	lock
	applymovement LOCALID_GHOST_2, ShadyForest_Movement_Disappear
	waitmovement 0
	release
	end

ShadyForest_EventScript_Ghost3::
	lock
	applymovement LOCALID_GHOST_3, ShadyForest_Movement_Disappear
	waitmovement 0
	release
	end

ShadyForest_Movement_Disappear:
	set_visible
	delay_4
	set_invisible
	delay_4
	set_visible
	delay_4
	set_invisible
	delay_4
	set_visible
	delay_8
	set_invisible
	delay_8
	set_visible
	delay_8
	set_invisible
	delay_8
	set_visible
	delay_16
	set_invisible
	step_end

