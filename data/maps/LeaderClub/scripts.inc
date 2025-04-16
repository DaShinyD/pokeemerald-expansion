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
	.string "boat to Vermilion from Lilycove.\p"
	.string "That's all!\p"
	.string "Best of luck.$"

LeaderClub_Movement_GoBack:
	walk_right
	walk_right
	face_down
	step_end



LeaderClub_EventScript_NotQuite::
	msgbox LeaderClub_Text_NotYet, MSGBOX_DEFAULT
	closemessage
	release
	end

LeaderClub_Text_NotYet:
	.string "Sorry, you can't challenge me yet.$"



@ Brock

LeaderClub_EventScript_Brock::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_BROCK, LeaderClub_EventScript_BrockAlreadyBeat
	msgbox LeaderClub_Text_BrockIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_BrockBattle
	end

LeaderClub_Text_BrockIntro:
	.string "Yo, I'm Brock.\p"
	.string "I'm rock hard.\p"
	.string "...\p"
	.string "I mean my Pokemon are rock hard!\p"
	.string "Let's see what the Hoenn Champion\n"
	.string "is capable of.$"

LeaderClub_EventScript_BrockBattle::
	trainerbattle_no_intro TRAINER_BROCK, LeaderClub_Text_BrockDefeat
	goto LeaderClub_EventScript_BrockBeaten
	end

LeaderClub_Text_BrockDefeat:
	.string "Gah!$"

LeaderClub_EventScript_BrockBeaten::
	msgbox LeaderClub_Text_BrockBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_BOULDER_BADGE
	setflag FLAG_BEAT_BROCK
	release
	end

LeaderClub_Text_BrockBeaten:
	.string "Man, you got me!\p"
	.string "Take this colorless Boulder Badge.\p"
	.string "Maybe you can beat that guy...$"

LeaderClub_EventScript_BrockAlreadyBeat::
	msgbox LeaderClub_Text_BrockAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_BrockAlreadyBeat:
	.string "Maybe we'll battle again.$"



@ Misty

LeaderClub_EventScript_Misty::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_BROCK, LeaderClub_EventScript_NotQuite
	goto_if_set FLAG_BEAT_MISTY, LeaderClub_EventScript_MistyAlreadyBeat
	msgbox LeaderClub_Text_MistyIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_MistyBattle
	end

LeaderClub_Text_MistyIntro:
	.string "Hi, I'm Misty, the sexiest woman\n"
	.string "in the whole world.\p"
	.string "My water types will drown you.\p"
	.string "Come on!$"

LeaderClub_EventScript_MistyBattle::
	trainerbattle_no_intro TRAINER_MISTY, LeaderClub_Text_MistyDefeat
	goto LeaderClub_EventScript_MistyBeaten
	end

LeaderClub_Text_MistyDefeat:
	.string "What?!$"

LeaderClub_EventScript_MistyBeaten::
	msgbox LeaderClub_Text_MistyBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_CASCADE_BADGE
	setflag FLAG_BEAT_MISTY
	release
	end

LeaderClub_Text_MistyBeaten:
	.string "How could my beautiful water types\n"
	.string "lose?\p"
	.string "Ugh, take this Cascade Badge.$"

LeaderClub_EventScript_MistyAlreadyBeat::
	msgbox LeaderClub_Text_MistyAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_MistyAlreadyBeat:
	.string "Don't get any ideas, I have a\n"
	.string "boyfriend.$"
	


@ Surge

LeaderClub_EventScript_Surge::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_MISTY, LeaderClub_EventScript_NotQuite
	goto_if_set FLAG_BEAT_SURGE, LeaderClub_EventScript_SurgeAlreadyBeat
	msgbox LeaderClub_Text_SurgeIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_SurgeBattle
	end

LeaderClub_Text_SurgeIntro:
	.string "POWER!\p"
	.string "That's what this world takes.\p"
	.string "I learned that in the Army.\p"
	.string "Show me yours.$"

LeaderClub_EventScript_SurgeBattle::
	trainerbattle_no_intro TRAINER_SURGE, LeaderClub_Text_SurgeDefeat
	goto LeaderClub_EventScript_SurgeBeaten
	end

LeaderClub_Text_SurgeDefeat:
	.string "Now that's power!$"

LeaderClub_EventScript_SurgeBeaten::
	msgbox LeaderClub_Text_SurgeBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_THUNDER_BADGE
	setflag FLAG_BEAT_SURGE
	release
	end

LeaderClub_Text_SurgeBeaten:
	.string "You've earned this.$"

LeaderClub_EventScript_SurgeAlreadyBeat::
	msgbox LeaderClub_Text_SurgeAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_SurgeAlreadyBeat:
	.string "Go on, show them your power.$"
	


@ Erika

LeaderClub_EventScript_Erika::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_SURGE, LeaderClub_EventScript_NotQuite
	goto_if_set FLAG_BEAT_ERIKA, LeaderClub_EventScript_ErikaAlreadyBeat
	msgbox LeaderClub_Text_ErikaIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_ErikaBattle
	end

LeaderClub_Text_ErikaIntro:
	.string "Oh hello.\p"
	.string "I'm Erika.\p"
	.string "I apologize for my slow speaking.\p"
	.string "I'm just...so tired.\p"
	.string "A battle? I see.\p"
	.string "Okay.$"

LeaderClub_EventScript_ErikaBattle::
	trainerbattle_no_intro TRAINER_ERIKA, LeaderClub_Text_ErikaDefeat
	goto LeaderClub_EventScript_ErikaBeaten
	end

LeaderClub_Text_ErikaDefeat:
	.string "Oh my.$"

LeaderClub_EventScript_ErikaBeaten::
	msgbox LeaderClub_Text_ErikaBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_RAINBOW_BADGE
	setflag FLAG_BEAT_ERIKA
	release
	end

LeaderClub_Text_ErikaBeaten:
	.string "Please accept this.$"

LeaderClub_EventScript_ErikaAlreadyBeat::
	msgbox LeaderClub_Text_ErikaAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_ErikaAlreadyBeat:
	.string "I'm so tired.$"
	


@ Koga

LeaderClub_EventScript_Koga::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_ERIKA, LeaderClub_EventScript_NotQuite
	goto_if_set FLAG_BEAT_KOGA, LeaderClub_EventScript_KogaAlreadyBeat
	msgbox LeaderClub_Text_KogaIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_KogaBattle
	end

LeaderClub_Text_KogaIntro:
	.string "A ninja operates in the shadows.\p"
	.string "However, if he is discovered, he\n"
	.string "must be able to fight.$"

LeaderClub_EventScript_KogaBattle::
	trainerbattle_no_intro TRAINER_KOGA, LeaderClub_Text_KogaDefeat
	goto LeaderClub_EventScript_KogaBeaten
	end

LeaderClub_Text_KogaDefeat:
	.string "Hm.$"

LeaderClub_EventScript_KogaBeaten::
	msgbox LeaderClub_Text_KogaBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_SOUL_BADGE
	setflag FLAG_BEAT_KOGA
	release
	end

LeaderClub_Text_KogaBeaten:
	.string "It seems I must train more.\p"
	.string "Take it, you've bested me.$"

LeaderClub_EventScript_KogaAlreadyBeat::
	msgbox LeaderClub_Text_KogaAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_KogaAlreadyBeat:
	.string "I will return stronger.$"
		


@ Sabrina

LeaderClub_EventScript_Sabrina::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_KOGA, LeaderClub_EventScript_NotQuite
	goto_if_set FLAG_BEAT_SABRINA, LeaderClub_EventScript_SabrinaAlreadyBeat
	msgbox LeaderClub_Text_SabrinaIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_SabrinaBattle
	end

LeaderClub_Text_SabrinaIntro:
	.string "I can read your mind.\p"
	.string "...\p"
	.string "I wish I hadn't.$"

LeaderClub_EventScript_SabrinaBattle::
	trainerbattle_no_intro TRAINER_SABRINA, LeaderClub_Text_SabrinaDefeat
	goto LeaderClub_EventScript_SabrinaBeaten
	end

LeaderClub_Text_SabrinaDefeat:
	.string "I couldn't foretell this.$"

LeaderClub_EventScript_SabrinaBeaten::
	msgbox LeaderClub_Text_SabrinaBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_MARSH_BADGE
	setflag FLAG_BEAT_SABRINA
	release
	end

LeaderClub_Text_SabrinaBeaten:
	.string "My mind powers weren't good enough\n"
	.string "to best you.\p"
	.string "Take it.$"

LeaderClub_EventScript_SabrinaAlreadyBeat::
	msgbox LeaderClub_Text_SabrinaAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_SabrinaAlreadyBeat:
	.string "I must meditate.$"
		


@ Blaine

LeaderClub_EventScript_Blaine::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_SABRINA, LeaderClub_EventScript_NotQuite
	goto_if_set FLAG_BEAT_BLAINE, LeaderClub_EventScript_BlaineAlreadyBeat
	msgbox LeaderClub_Text_BlaineIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_BlaineBattle
	end

LeaderClub_Text_BlaineIntro:
	.string "My fiery hot Pokemon can burn any\n"
	.string "challenger to a crisp.\p"
	.string "Show me your burning passion.$"

LeaderClub_EventScript_BlaineBattle::
	trainerbattle_no_intro TRAINER_BLAINE, LeaderClub_Text_BlaineDefeat
	goto LeaderClub_EventScript_BlaineBeaten
	end

LeaderClub_Text_BlaineDefeat:
	.string "Scalding!$"

LeaderClub_EventScript_BlaineBeaten::
	msgbox LeaderClub_Text_BlaineBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_VOLCANO_BADGE
	setflag FLAG_BEAT_BLAINE
	release
	end

LeaderClub_Text_BlaineBeaten:
	.string "A scorching battler like you has\n"
	.string "earned this.$"

LeaderClub_EventScript_BlaineAlreadyBeat::
	msgbox LeaderClub_Text_BlaineAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_BlaineAlreadyBeat:
	.string "Too hot to handle.$"
		


@ Blue

LeaderClub_EventScript_Blue::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_BLAINE, LeaderClub_EventScript_NotQuite
	goto_if_unset FLAG_MET_SCOTT_ON_SS_TIDAL, LeaderClub_EventScript_NoBlue
	goto_if_set FLAG_BEAT_BLUE, LeaderClub_EventScript_BlueAlreadyBeat
	msgbox LeaderClub_Text_BlueIntro, MSGBOX_DEFAULT
	closemessage
	goto LeaderClub_EventScript_BlueBattle
	end

LeaderClub_Text_BlueIntro:
	.string "For a brief period, I was the\n"
	.string "champion of the Indigo League.\p"
	.string "Then he came right behind me, as\n"
	.string "usual.\p"
	.string "Here and now, I'll show you the\n"
	.string "strength of a Kanto Leader.$"

LeaderClub_EventScript_BlueBattle::
	trainerbattle_no_intro TRAINER_BLUE, LeaderClub_Text_BlueDefeat
	goto LeaderClub_EventScript_BlueBeaten
	end

LeaderClub_Text_BlueDefeat:
	.string "Sigh.$"

LeaderClub_EventScript_BlueBeaten::
	msgbox LeaderClub_Text_BlueBeaten, MSGBOX_DEFAULT
	closemessage
	giveitem ITEM_EARTH_BADGE
	setflag FLAG_BEAT_BLUE
	setflag FLAG_GO_RED
	release
	end

LeaderClub_Text_BlueBeaten:
	.string "I won't allow myself to be doomed to\n"
	.string "second place always.\p"
	.string "We will battle again.\p"
	.string "By beating me, you've earned the right\n"
	.string "to challenge our League.\p"
	.string "Take my badge, and go to Lilycove.\p"
	.string "There, you can sail to Vermilion.$"

LeaderClub_EventScript_BlueAlreadyBeat::
	msgbox LeaderClub_Text_BlueAlreadyBeat, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_BlueAlreadyBeat:
	.string "Prepare for next time.$"

LeaderClub_EventScript_NoBlue::
	msgbox LeaderClub_Text_NoBlue, MSGBOX_DEFAULT
	release
	end

LeaderClub_Text_NoBlue:
	.string "You may have beaten all the previous\n"
	.string "leaders, but I won't accept your\l"
	.string "challenge yet.\p"
	.string "Only a trainer who's visited the\n"
	.string "Battle Frontier has the right to\l"
	.string "face me.$"

