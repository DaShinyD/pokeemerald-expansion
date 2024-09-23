Route117_PokemonDayCare_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route117_PokemonDayCare_OnTransition
	.byte 0

Route117_PokemonDayCare_OnTransition:
	setflag FLAG_LANDMARK_POKEMON_DAYCARE
	end

Route_117_PokemonDayCare_EventScript_Daycare_Guy1::
	lock
	faceplayer
	msgbox Route117_Text_Guy1Daycare, MSGBOX_DEFAULT
	release
	end

Route_117_PokemonDayCare_EventScript_Daycare_Guy2::
	lock
	faceplayer
	msgbox Route117_Text_Guy2Daycare, MSGBOX_DEFAULT
	release
	end

Route_117_PokemonDayCare_EventScript_Daycare_Guy3::
	lock
	faceplayer
	msgbox Route117_Text_Guy3Daycare, MSGBOX_DEFAULT
	release
	end

Route_117_PokemonDayCare_EventScript_Daycare_Guy4::
	lock
	faceplayer
	msgbox Route117_Text_Guy4Daycare, MSGBOX_DEFAULT
	release
	end

Route117_Text_Guy1Daycare:
	.string "I'm waiting for an egg.$"

Route117_Text_Guy2Daycare:
	.string "How do I get egg moves?$"

Route117_Text_Guy3Daycare:
	.string "Breeding is complicated.$"

Route117_Text_Guy4Daycare:
	.string "I really need false swipe\n"
	.string "on my Breloom.$"

