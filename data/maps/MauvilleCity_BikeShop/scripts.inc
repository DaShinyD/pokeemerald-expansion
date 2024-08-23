MauvilleCity_BikeShop_MapScripts::
	.byte 0

@beginning of my new code

MauvilleCity_BikeShop_EventScript_Rydel::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_BIKE, MauvilleCity_BikeShop_EventScript_HowsTheBike
	msgbox MauvilleCity_BikeShop_Text_Greeting, MSGBOX_DEFAULT
	giveitem ITEM_ACRO_BIKE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	msgbox MauvilleCity_BikeShop_Text_EnjoyBike, MSGBOX_DEFAULT
	setflag FLAG_RECEIVED_BIKE
	release
	end

MauvilleCity_BikeShop_EventScript_HowsTheBike::
	msgbox MauvilleCity_BikeShop_Text_HowsTheBike, MSGBOX_DEFAULT
	release
	end

MauvilleCity_BikeShop_Text_Greeting:
	.string "Hi there trainer.\n"
	.string "Wow, your shoes are dirty, you\p"
	.string "must have come from far away.\n"
	.string "I should give you a bike.\p"
	.string "In the past, we had two\n"
	.string "different kind of bikes, but\p"
	.string "now, we developed a new bike\n"
	.string "that functions as both.\p"
	.string "Simply press the R button\n"
	.string "while moving to switch.\p"
	.string "Pretty simple, huh?\n"
	.string "Anyway, here you go.$"


MauvilleCity_BikeShop_Text_EnjoyBike:
	.string "Enjoy the bike!$"

MauvilleCity_BikeShop_Text_HowsTheBike:
	.string "How's the bike?$"

