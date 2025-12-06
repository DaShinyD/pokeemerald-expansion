.set LOCALID_STRATA_GRUNT, 1
.set LOCALID_STRATA_GRUNT_2, 2
.set LOCALID_STRATA_GRUNT_3, 3

BirchGarden_MapScripts::
	.byte 0

BirchGarden_EventScript_ExplainSign::
	msgbox BirchGarden_Text_Sign, MSGBOX_SIGN
	end

BirchGarden_Text_Sign:
	.string "Birch's Garden\p"
	.string "Each grass patch has unique Pokemon.$"

BirchGarden_EventScript_StrataGrunt::
	lock
	faceplayer
	msgbox BirchGarden_Text_StrataGrunt, MSGBOX_DEFAULT
	closemessage
	goto BirchGarden_EventScript_StrataGruntFight
	end

BirchGarden_Text_StrataGrunt:
	.string "How did you even know to come\n"
	.string "here?$"

BirchGarden_EventScript_StrataGruntFight::
	trainerbattle_no_intro TRAINER_STRATA_GRUNT_4, BirchGarden_Text_StrataGruntDefeat
	goto BirchGarden_EventScript_StrataGruntLeave
	end

BirchGarden_Text_StrataGruntDefeat:
	.string "Tssk.$"

BirchGarden_EventScript_StrataGruntLeave::
	msgbox BirchGarden_Text_StrataGruntBye, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STRATA_GRUNT
	setvar VAR_BIRCH_GARDEN_STATE, 1
	fadescreen FADE_FROM_BLACK
	release
	end

BirchGarden_Text_StrataGruntBye:
	.string "You won't beat us.$"

BirchGarden_EventScript_StrataGrunt2::
	lock
	faceplayer
	goto_if_eq VAR_BIRCH_GARDEN_STATE, 0, BirchGarden_EventScript_No
	goto_if_eq VAR_BIRCH_GARDEN_STATE, 2, BirchGarden_EventScript_No
	msgbox BirchGarden_Text_StrataGruntTwo, MSGBOX_DEFAULT
	closemessage
	goto BirchGarden_EventScript_StrataGrunt2Fight
	end

BirchGarden_Text_StrataGruntTwo:
	.string "What fool gave us away?$"

BirchGarden_EventScript_StrataGrunt2Fight::
	trainerbattle_no_intro TRAINER_STRATA_GRUNT_5, BirchGarden_Text_StrataGrunt2Defeat
	goto BirchGarden_EventScript_StrataGrunt2Leave
	end

BirchGarden_Text_StrataGrunt2Defeat:
	.string "Tssk.$"

BirchGarden_EventScript_StrataGrunt2Leave::
	msgbox BirchGarden_Text_StrataGrunt2Bye, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STRATA_GRUNT_2
	setvar VAR_BIRCH_GARDEN_STATE, 2
	fadescreen FADE_FROM_BLACK
	release
	end

BirchGarden_Text_StrataGrunt2Bye:
	.string "Nothing can stop us.$"
	
BirchGarden_EventScript_StrataGrunt3::
	lock
	faceplayer
	goto_if_eq VAR_BIRCH_GARDEN_STATE, 0, BirchGarden_EventScript_No
	goto_if_eq VAR_BIRCH_GARDEN_STATE, 1, BirchGarden_EventScript_No
	msgbox BirchGarden_Text_StrataGruntThree, MSGBOX_DEFAULT
	closemessage
	goto BirchGarden_EventScript_StrataGrunt3Fight
	end

BirchGarden_Text_StrataGruntThree:
	.string "I won't be the last.$"

BirchGarden_EventScript_StrataGrunt3Fight::
	trainerbattle_no_intro TRAINER_STRATA_GRUNT_6, BirchGarden_Text_StrataGrunt3Defeat
	goto BirchGarden_EventScript_StrataGrunt3Leave
	end

BirchGarden_Text_StrataGrunt3Defeat:
	.string "Tssk.$"

BirchGarden_EventScript_StrataGrunt3Leave::
	msgbox BirchGarden_Text_StrataGrunt3Bye, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_STRATA_GRUNT_3
	setflag FLAG_STRATA_PRE
	setflag FLAG_STRATA_FINALE
	fadescreen FADE_FROM_BLACK
	release
	end

BirchGarden_Text_StrataGrunt3Bye:
	.string "You idiot.\p"
	.string "It's too late now, we have all we\n"
	.string "need to finish.\p"
	.string "Did you really think you were\n"
	.string "doing anything?\p"
	.string "You've been stalled.\p"
	.string "You were completely convinced that\n"
	.string "hiker was just a civilian.\p"
	.string "Keh heh heh.$"

BirchGarden_EventScript_No::
	msgbox BirchGarden_Text_GruntUnaware, MSGBOX_DEFAULT
	closemessage
	release
	end

BirchGarden_Text_GruntUnaware:
	.string "The Pokemon here...$"

