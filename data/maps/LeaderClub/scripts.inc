.set LOCALID_GREETER, 17

LeaderClub_MapScripts::
	.byte 0

LeaderClub_EventScript_GreetAndExplain::
	lock
	applymovement LOCALID_GREETER, LeaderClub_Movement_ComeGreet
	waitmovement 0
	msgbox LeaderClub_Text_Hi, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_GREETER, LeaderClub_Movement_GoBack
	waitmovement 0
	setvar VAR_LEADERS_CLUB, 1
	release
	end

LeaderClub_Movement_ComeGreet:
	walk_left
	walk_left
	face_down
	step_end

LeaderClub_Text_Hi:
	.string "Hey there!\p"
	.string "Welcome to the Gym Leader Club.\p"
	.string "The gym leaders from our sister\n"
	.string "regions come here to challenge\l"
	.string "strong trainers.\p"
	.string "Unfortunately, the Johto leaders\n"
	.string "cannot provide a badge as their\l"
	.string "league is under construction.\p"
	.string "However, if you beat a Kanto\n"
	.string "leader, they will give you a\l"
	.string "colorless badge.\p"
	.string "Getting all 8 allows you to\n"
	.string "challenge the Indigo League.\p"
	.string "However, you cannot earn the\n"
	.string "title as you need color badges\l"
	.string "to do so.\p"
	.string "You'll have to challenge them\n"
	.string "in order.\p"
	.string "Once you beat them all, you'll\n"
	.string "get permission to take the\l"
	.string "boat to Vermilion.\p"
	.string "That's all!\p"
	.string "Best of luck.$"

LeaderClub_Movement_GoBack:
	walk_right
	walk_right
	face_down
	step_end

