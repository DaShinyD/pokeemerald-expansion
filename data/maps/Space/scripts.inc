.set LOCALID_LUNALA, 1
.set LOCALID_SOLGALEO, 2
.set LOCALID_GRUNT_3, 3
.set LOCALID_JIRACHI, 4
.set LOCALID_VICTINI, 5
.set LOCALID_GRUNT_4, 6
.set LOCALID_GRUNT_5, 7
.set LOCALID_GRUNT_6, 8
.set LOCALID_GRUNT_7, 9
.set LOCALID_GIOVANNI, 10
.set LOCALID_GRUNT_1, 11
.set LOCALID_GRUNT_2, 12

Space_MapScripts::
	.byte 0


@ Legendaries

Space_EventScript_Solgaleo::
	lock
	faceplayer
	playmoncry SPECIES_SOLGALEO, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_SOLGALEO, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Space_EventScript_SolgaleoGone
	release
	end

Space_EventScript_SolgaleoGone:
	setflag FLAG_SOLGALEO
	removeobject LOCALID_SOLGALEO
	release
	end

Space_EventScript_Lunala::
	lock
	faceplayer
	playmoncry SPECIES_LUNALA, CRY_MODE_ENCOUNTER
	waitmoncry
	setwildbattle SPECIES_LUNALA, 40
	special BattleSetup_StartLegendaryBattle
	waitstate
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, Space_EventScript_LunalaGone
	release
	end

Space_EventScript_LunalaGone:
	setflag FLAG_LUNALA
	removeobject LOCALID_LUNALA
	release
	end


@ Team Rocket

Space_EventScript_Rocket1::
	trainerbattle_single TRAINER_ROCKET_1, Space_Text_GruntIntro1, Space_Text_GruntDefeat1
	msgbox Space_Text_GruntAfter1, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro1:
	.string "Team Rocket lives on!$"

Space_Text_GruntDefeat1:
	.string "No way.$"

Space_Text_GruntAfter1:
	.string "Giovanni will succeed, always.$"

Space_EventScript_Rocket2::
	trainerbattle_single TRAINER_ROCKET_2, Space_Text_GruntIntro2, Space_Text_GruntDefeat2
	msgbox Space_Text_GruntAfter2, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro2:
	.string "Get some.$"

Space_Text_GruntDefeat2:
	.string "I got some...$"

Space_Text_GruntAfter2:
	.string "Team Rocket never dies.$"

Space_EventScript_Rocket3::
	trainerbattle_single TRAINER_ROCKET_3, Space_Text_GruntIntro3, Space_Text_GruntDefeat3
	msgbox Space_Text_GruntAfter3, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro3:
	.string "Even on an asteroid we have kids\n"
	.string "harassing us.$"

Space_Text_GruntDefeat3:
	.string "Damn.$"

Space_Text_GruntAfter3:
	.string "Go on, lose ahead of me.$"

Space_EventScript_Rocket4::
	trainerbattle_single TRAINER_ROCKET_4, Space_Text_GruntIntro4, Space_Text_GruntDefeat4
	msgbox Space_Text_GruntAfter4, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro4:
	.string "Stay away from Giovanni.$"

Space_Text_GruntDefeat4:
	.string "Gah.$"

Space_Text_GruntAfter4:
	.string "Why are we always stopped.$"

Space_EventScript_Rocket5::
	trainerbattle_single TRAINER_ROCKET_5, Space_Text_GruntIntro5, Space_Text_GruntDefeat5
	msgbox Space_Text_GruntAfter5, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro5:
	.string "How did you get here?$"

Space_Text_GruntDefeat5:
	.string "Nooooo.$"

Space_Text_GruntAfter5:
	.string "Those legendaries will restore our\n"
	.string "former glory.$"

Space_EventScript_Rocket6::
	trainerbattle_single TRAINER_ROCKET_6, Space_Text_GruntIntro6, Space_Text_GruntDefeat6
	msgbox Space_Text_GruntAfter6, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro6:
	.string "And make it double.$"

Space_Text_GruntDefeat6:
	.string "Okay, we stole those lines.$"

Space_Text_GruntAfter6:
	.string "You won't beat him.$"

Space_EventScript_Rocket7::
	trainerbattle_single TRAINER_ROCKET_7, Space_Text_GruntIntro7, Space_Text_GruntDefeat7
	msgbox Space_Text_GruntAfter7, MSGBOX_AUTOCLOSE
	end

Space_Text_GruntIntro7:
	.string "Prepare for trouble.$"

Space_Text_GruntDefeat7:
	.string "Now we're in trouble.$"

Space_Text_GruntAfter7:
	.string "Giovanni doesn't lose.$"

Space_EventScript_Giovanni::
	lock
	applymovement OBJ_EVENT_ID_PLAYER, Space_Movement_WalkToGio
	waitmovement 0
	applymovement LOCALID_GIOVANNI, Common_Movement_FacePlayer
	waitmovement 0
	msgbox Space_Text_GioSpeech, MSGBOX_DEFAULT
	goto Space_EventScript_GiovanniBattle
	end

Space_Movement_WalkToGio:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_up
	face_left
	step_end

Space_Text_GioSpeech:
	.string "Giovanni: ...\p"
	.string "Ha. Haha. HAHAHA.\p"
	.string "This...this is...absolutely\n"
	.string "INCREDIBLE.\p"
	.string "To think, here I am, on a literal\n"
	.string "asteroid, and still, a child is\l"
	.string "here to stop me.\p"
	.string "Is this a sign?\p"
	.string "Is this my final test?\p"
	.string "No longer have I limited myself to\n"
	.string "low level crime.\p"
	.string "Here, Team Rocket takes the next\n"
	.string "and final step.\p"
	.string "Become rulers, or fall forever.\p"
	.string "I am no longer the man I used\n"
	.string "to be.\p"
	.string "I will bring my full strength to\n"
	.string "ensure my plans come through.\p"
	.string "Today, Giovanni will become who he\n"
	.string "is meant to be.\p"
	.string "Come, show me my future.$"

Space_EventScript_GiovanniBattle::
	trainerbattle_no_intro TRAINER_GIOVANNI, Space_Text_GioDefeat
	goto Space_EventScript_GioAfter
	end

Space_Text_GioDefeat:
	.string "..............$"

Space_EventScript_GioAfter::
	msgbox Space_Text_GioSpeech2, MSGBOX_DEFAULT
	fadescreen FADE_TO_BLACK
	setflag FLAG_TEAM_ROCKET
	removeobject LOCALID_GIOVANNI
	removeobject LOCALID_GRUNT_1
	removeobject LOCALID_GRUNT_2
	removeobject LOCALID_GRUNT_3
	removeobject LOCALID_GRUNT_4
	removeobject LOCALID_GRUNT_5
	removeobject LOCALID_GRUNT_6
	removeobject LOCALID_GRUNT_7
	fadescreen FADE_FROM_BLACK
	setvar VAR_MOSSDEEP_ROCKET, 3
	release
	end

Space_Text_GioSpeech2:
	.string "It was never meant to be.\p"
	.string "My defeat is apparently written in\n"
	.string "the stars even.\p"
	.string "Team Rocket is done.$"

