PalleteTown_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, PalleteTown_OnTransition
	.byte 0

PalleteTown_OnTransition:
	setflag FLAG_VISITED_PALLET
	end
