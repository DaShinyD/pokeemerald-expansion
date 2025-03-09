SeafloorCavern_Room1_MapScripts::
	.byte 0

SeafloorCavern_Room1_EventScript_Grunt1::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_1, SeafloorCavern_Room1_Text_Grunt1Intro, SeafloorCavern_Room1_Text_Grunt1Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt1PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_EventScript_Grunt2::
	trainerbattle_single TRAINER_GRUNT_SEAFLOOR_CAVERN_2, SeafloorCavern_Room1_Text_Grunt2Intro, SeafloorCavern_Room1_Text_Grunt2Defeat
	msgbox SeafloorCavern_Room1_Text_Grunt2PostBattle, MSGBOX_AUTOCLOSE
	end

SeafloorCavern_Room1_Text_Grunt1Intro:
	.string "We don't need a kid around!\n"
	.string "Go on home already!$"

SeafloorCavern_Room1_Text_Grunt1Defeat:
	.string "I want to go home…$"

SeafloorCavern_Room1_Text_Grunt1PostBattle:
	.string "I want to get a promotion so I can\n"
	.string "boss around the GRUNTS…$"

SeafloorCavern_Room1_Text_Grunt2Intro:
	.string "That submarine… It's tiny inside.\n"
	.string "I'm sore all over!$"

SeafloorCavern_Room1_Text_Grunt2Defeat:
	.string "Losing makes me sore!$"

SeafloorCavern_Room1_Text_Grunt2PostBattle:
	.string "That submarine we jacked, man,\n"
	.string "it's brutal as a ride.\l"
	.string "It's way too tight in there!$"


SeafloorCavern_Room1_EventScript_RivalsCall::
	lock
	msgbox SeafloorCavern_Room1_Text_RivalsCall, MSGBOX_DEFAULT
	waitmessage
	setvar VAR_SEAFLOOR_CALL, 1
	release
	end

SeafloorCavern_Room1_Text_RivalsCall:
	.string "...\n"
	.string "...\p"
	.string "A phone call!\p"
	.string "Rye: {PLAYER}!\p"
	.string "There's a whole Magma and Aqua war\n"
	.string "going on.\p"
	.string "Where are you?\n"
	.string "...\p"
	.string "A cave underwater?\p"
	.string "Magma went after Groudon, so now\n"
	.string "Aqua is going after Kyogre.\p"
	.string "{PLAYER}, whatever you do,\n"
	.string "you must stop them!\p"
	.string "Me, Dash, Wally, and {RIVAL}\n"
	.string "are taking care of things up here\l"
	.string "with the gym leaders.\p"
	.string "Stay safe and let's group up in\n"
	.string "Sootopolis after.\p"
	.string "Good luck.\p"
	.string "The call ended.$"

