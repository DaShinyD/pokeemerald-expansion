.set LOCALID_MEGAMAN, 28

Route114_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route114_OnTransition
	map_script MAP_SCRIPT_ON_LOAD, Route114_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Route114_OnFrame
	.byte 0

Route114_OnTransition:
	call_if_eq VAR_SHOULD_END_ABNORMAL_WEATHER, 1, AbnormalWeather_EventScript_HideMapNamePopup
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_114_NORTH, AbnormalWeather_StartGroudonWeather
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_114_SOUTH, AbnormalWeather_StartGroudonWeather
	end

Route114_OnLoad:
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_114_NORTH, AbnormalWeather_EventScript_PlaceTilesRoute114North
	call_if_eq VAR_ABNORMAL_WEATHER_LOCATION, ABNORMAL_WEATHER_ROUTE_114_SOUTH, AbnormalWeather_EventScript_PlaceTilesRoute114South
	end

Route114_OnFrame:
	map_script_2 VAR_SHOULD_END_ABNORMAL_WEATHER, 1, AbnormalWeather_EventScript_EndEventAndCleanup_1
	.2byte 0

Route114_EventScript_Man::
	lock
	faceplayer
	dotimebasedevents
	goto_if_set FLAG_DAILY_ROUTE_114_RECEIVED_BERRY, Route114_EventScript_ReceivedBerry
	msgbox Route114_Text_LoveUsingBerryCrushShareBerry, MSGBOX_DEFAULT
	random NUM_ROUTE_114_MAN_BERRIES
	addvar VAR_RESULT, NUM_ROUTE_114_MAN_BERRIES_SKIPPED
	addvar VAR_RESULT, FIRST_BERRY_INDEX
	giveitem VAR_RESULT
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_DAILY_ROUTE_114_RECEIVED_BERRY
	msgbox Route114_Text_TryBerryCrushWithFriends, MSGBOX_DEFAULT
	release
	end

Route114_EventScript_ReceivedBerry::
	msgbox Route114_Text_FunToThinkAboutBerries, MSGBOX_DEFAULT
	release
	end

Route114_EventScript_RoarGentleman::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_TM_ROAR, Route114_EventScript_ReceivedRoar
	msgbox Route114_Text_AllMyMonDoesIsRoarTakeThis, MSGBOX_DEFAULT
	giveitem ITEM_TM_ROAR
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_TM_ROAR
	msgbox Route114_Text_ExplainRoar, MSGBOX_DEFAULT
	release
	end

Route114_EventScript_ReceivedRoar::
	msgbox Route114_Text_ExplainRoar, MSGBOX_DEFAULT
	release
	end

Route114_EventScript_Poochyena::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_POOCHYENA, CRY_MODE_ENCOUNTER
	msgbox Route114_Text_Poochyena, MSGBOX_DEFAULT
	waitmoncry
	release
	end

Route114_EventScript_MeteorFallsSign::
	msgbox Route114_Text_MeteorFallsSign, MSGBOX_SIGN
	end

Route114_EventScript_FossilManiacsHouseSign::
	msgbox Route114_Text_FossilManiacsHouseSign, MSGBOX_SIGN
	end

Route114_EventScript_LanettesHouseSign::
	msgbox Route114_Text_LanettesHouse, MSGBOX_SIGN
	end

Route114_EventScript_Lenny::
	trainerbattle_single TRAINER_LENNY, Route114_Text_LennyIntro, Route114_Text_LennyDefeat
	msgbox Route114_Text_LennyPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Lucas::
	trainerbattle_single TRAINER_LUCAS_1, Route114_Text_LucasIntro, Route114_Text_LucasDefeat
	msgbox Route114_Text_LucasPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Shane::
	trainerbattle_single TRAINER_SHANE, Route114_Text_ShaneIntro, Route114_Text_ShaneDefeat
	msgbox Route114_Text_ShanePostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Nancy::
	trainerbattle_single TRAINER_NANCY, Route114_Text_NancyIntro, Route114_Text_NancyDefeat
	msgbox Route114_Text_NancyPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Steve::
	trainerbattle_single TRAINER_STEVE_1, Route114_Text_SteveIntro, Route114_Text_SteveDefeat, Route114_EventScript_RegisterSteve
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route114_EventScript_RematchSteve
	msgbox Route114_Text_StevePostBattle, MSGBOX_DEFAULT
	release
	end

Route114_EventScript_RegisterSteve::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route114_Text_SteveRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_STEVE_1
	release
	end

Route114_EventScript_RematchSteve::
	trainerbattle_rematch TRAINER_STEVE_1, Route114_Text_SteveRematchIntro, Route114_Text_SteveRematchDefeat
	msgbox Route114_Text_StevePostRematch, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Bernie::
	trainerbattle_single TRAINER_BERNIE_1, Route114_Text_BernieIntro, Route114_Text_BernieDefeat, Route114_EventScript_RegisterBernie
	specialvar VAR_RESULT, ShouldTryRematchBattle
	goto_if_eq VAR_RESULT, TRUE, Route114_EventScript_RematchBernie
	msgbox Route114_Text_BerniePostBattle, MSGBOX_DEFAULT
	release
	end

Route114_EventScript_RegisterBernie::
	special PlayerFaceTrainerAfterBattle
	waitmovement 0
	msgbox Route114_Text_BernieRegister, MSGBOX_DEFAULT
	register_matchcall TRAINER_BERNIE_1
	release
	end

Route114_EventScript_RematchBernie::
	trainerbattle_rematch TRAINER_BERNIE_1, Route114_Text_BernieRematchIntro, Route114_Text_BernieRematchDefeat
	msgbox Route114_Text_BerniePostRematch, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Claude::
	trainerbattle_single TRAINER_CLAUDE, Route114_Text_ClaudeIntro, Route114_Text_ClaudeDefeat
	msgbox Route114_Text_ClaudePostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Nolan::
	trainerbattle_single TRAINER_NOLAN, Route114_Text_NolanIntro, Route114_Text_NolanDefeat
	msgbox Route114_Text_NolanPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Tyra::
	trainerbattle_double TRAINER_TYRA_AND_IVY, Route114_Text_TyraIntro, Route114_Text_TyraDefeat, Route114_Text_TyraNotEnoughMons
	msgbox Route114_Text_TyraPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Ivy::
	trainerbattle_double TRAINER_TYRA_AND_IVY, Route114_Text_IvyIntro, Route114_Text_IvyDefeat, Route114_Text_IvyNotEnoughMons
	msgbox Route114_Text_IvyPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Angelina::
	trainerbattle_single TRAINER_ANGELINA, Route114_Text_AngelinaIntro, Route114_Text_AngelinaDefeat
	msgbox Route114_Text_AngelinaPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Charlotte::
	trainerbattle_single TRAINER_CHARLOTTE, Route114_Text_CharlotteIntro, Route114_Text_CharlotteDefeat
	msgbox Route114_Text_CharlottePostBattle, MSGBOX_AUTOCLOSE
	end

Route114_EventScript_Kai::
	trainerbattle_single TRAINER_KAI, Route114_Text_KaiIntro, Route114_Text_KaiDefeat
	msgbox Route114_Text_KaiPostBattle, MSGBOX_AUTOCLOSE
	end

Route114_Text_AllMyMonDoesIsRoarTakeThis:
	.string "All my POKéMON does is ROAR…\n"
	.string "No one dares to come near me…\p"
	.string "Sigh… If you would, please take\n"
	.string "this TM away…$"

Route114_Text_ExplainRoar:
	.string "TM05 contains ROAR.\n"
	.string "A ROAR sends POKéMON scurrying.$"

Route114_Text_Poochyena:
	.string "Bow! Bowwow!$"

Route114_Text_MeteorFallsSign:
	.string "METEOR FALLS\n"
	.string "RUSTBORO CITY THROUGH HERE$"

Route114_Text_FossilManiacsHouseSign:
	.string "FOSSIL MANIAC'S HOUSE\n"
	.string "“Fossils gratefully accepted!”$"

Route114_Text_LanettesHouse:
	.string "LANETTE'S HOUSE$"




Route114_EventScript_SellStones::
	lock
	faceplayer
	goto_if_set FLAG_BATTLE_ITEMS, Route114_EventScript_SellStones1
	msgbox Route114_Text_GiveBattleItems, MSGBOX_DEFAULT
	giveitem ITEM_MEGA_RING
	msgbox Route114_Text_ThereYaGo, MSGBOX_DEFAULT
	setflag FLAG_BATTLE_ITEMS
	pokemart MegaStone_Mart_Pokemart
	release
	end
	
Route114_Text_GiveBattleItems:
	.string "Hello the-oh!\p"
	.string "It seems you're an upcoming\n"
	.string "young trainer, but you don't\l"
	.string "have any battle power items!\p"
	.string "I'm talking about the mega\n"
	.string "ring.\p"
	.string "Here, let me give you one.$"

Route114_Text_ThereYaGo:
	.string "These items and mega stones\p"
	.string "are abundant now. So take any\n"
	.string "free of charge.$"

Route114_EventScript_SellStones1::
	msgbox Route114_Text_Hello, MSGBOX_DEFAULT
	waitmessage
	pokemart MegaStone_Mart_Pokemart
	msgbox Route114_Text_SeeYa, MSGBOX_DEFAULT
	release
	end

	.align 2
MegaStone_Mart_Pokemart:
	.2byte ITEM_ABOMASITE
	.2byte ITEM_ABSOLITE
	.2byte ITEM_AGGRONITE
	.2byte ITEM_AERODACTYLITE
	.2byte ITEM_ALAKAZITE
	.2byte ITEM_ALTARIANITE
	.2byte ITEM_AMPHAROSITE
	.2byte ITEM_ARCEUSITE
	.2byte ITEM_AUDINITE
	.2byte ITEM_BANETTITE
	.2byte ITEM_BEEDRILLITE
	.2byte ITEM_BEEDRILLITE_H
	.2byte ITEM_BLASTOISINITE
	.2byte ITEM_BLASTOISINITE_Y
	.2byte ITEM_BLAZIKENITE
	.2byte ITEM_BUTTERFRENITE
	.2byte ITEM_CAMERUPTITE
	.2byte ITEM_CENTISKORCHITE
	.2byte ITEM_CHARIZARDITE_X
	.2byte ITEM_CHARIZARDITE_Y
	.2byte ITEM_CHARIZARDITE_Z
	.2byte ITEM_CHESNITE
	.2byte ITEM_CINDERACITE
	.2byte ITEM_DECIDUITE
	.2byte ITEM_DELPHOXITE
	.2byte ITEM_DIANCITE
	.2byte ITEM_DARKRITE
	.2byte ITEM_DRAGONTITE
	.2byte ITEM_EMBOARITE
	.2byte ITEM_EMPOLEONITE
	.2byte ITEM_FERALIGATRITE
	.2byte ITEM_FLYGONITE
	.2byte ITEM_FROSLASSITE
	.2byte ITEM_GALLADITE
	.2byte ITEM_GARCHOMPITE
	.2byte ITEM_GARDEVOIRITE
	.2byte ITEM_GENGARITE
	.2byte ITEM_GLALITITE
	.2byte ITEM_GRENINJITE
	.2byte ITEM_GYARADOSITE
	.2byte ITEM_HAWLUCHITE
	.2byte ITEM_HERACRONITE
	.2byte ITEM_HOOHITE
	.2byte ITEM_HOUNDOOMINITE
	.2byte ITEM_INCINERITE
	.2byte ITEM_INFERNITE
	.2byte ITEM_INTELEONITE
	.2byte ITEM_KANGASKHANITE
	.2byte ITEM_KINGDRITE_X
	.2byte ITEM_KINGDRITE_Y
	.2byte ITEM_KINGLERITE
	.2byte ITEM_LAPRASITE
	.2byte ITEM_LATIASITE
	.2byte ITEM_LATIOSITE
	.2byte ITEM_LOPUNNITE
	.2byte ITEM_LUCARIONITE
	.2byte ITEM_LUGITE
	.2byte ITEM_LUXRITE
	.2byte ITEM_MACHAMPITE
	.2byte ITEM_MANECTITE
	.2byte ITEM_MAWILITE
	.2byte ITEM_MEDICHAMITE
	.2byte ITEM_MEGANIUMITE
	.2byte ITEM_METAGROSSITE
	.2byte ITEM_MEOWSCARITE
	.2byte ITEM_MEWTWONITE_X
	.2byte ITEM_MEWTWONITE_Y
	.2byte ITEM_MIMIKITE
	.2byte ITEM_NIDOKINGITE
	.2byte ITEM_PIDGEOTITE
	.2byte ITEM_PINSIRITE
	.2byte ITEM_PRIMARINITE
	.2byte ITEM_QUAQUAVITE
	.2byte ITEM_RILLABOOMITE
	.2byte ITEM_SABLENITE
	.2byte ITEM_SALAMENCITE
	.2byte ITEM_SAMUROTTITE
	.2byte ITEM_SANDSLASHITE
	.2byte ITEM_SCEPTILITE
	.2byte ITEM_SCIZORITE
	.2byte ITEM_SERPERIORITE
	.2byte ITEM_SHARPEDONITE
	.2byte ITEM_SHUCKLITE
	.2byte ITEM_SKARMITE
	.2byte ITEM_SKELEDIRGITE
	.2byte ITEM_SLAKINGITE
	.2byte ITEM_SLOWBRONITE
	.2byte ITEM_SNORLAXITE
	.2byte ITEM_STEELIXITE
	.2byte ITEM_SWAMPERTITE
	.2byte ITEM_TORTERRITE
	.2byte ITEM_TYRANITARITE
	.2byte ITEM_TYPHLOSIONITE
	.2byte ITEM_VENUSAURITE
	.2byte ITEM_VENUSAURITE_Y
	.2byte ITEM_WEAVILITE
	.2byte ITEM_NONE
	release
	end

Route114_Text_Hello:
	.string "Hi there, what would you like?$"

Route114_Text_SeeYa:
	.string "Come again.$"

Route114_EventScript_MegaTriggerR::
	lock
	playse SE_PIN
	applymovement LOCALID_MEGAMAN, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Route114_Text_HeyTrainer, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Route114_Movement_ApproachMegaR
	waitmovement 0
	msgbox Route114_Text_GiveBattleItems, MSGBOX_DEFAULT
	giveitem ITEM_MEGA_RING
	msgbox Route114_Text_ThereYaGo, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_BATTLE_ITEMS
	setvar VAR_MEGA_MAN, 1
	pokemart MegaStone_Mart_Pokemart
	release
	end

Route114_EventScript_MegaTriggerM::
	lock
	playse SE_PIN
	applymovement LOCALID_MEGAMAN, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Route114_Text_HeyTrainer, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Route114_Movement_ApproachMegaM
	waitmovement 0
	msgbox Route114_Text_GiveBattleItems, MSGBOX_DEFAULT
	giveitem ITEM_MEGA_RING
	msgbox Route114_Text_ThereYaGo, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_BATTLE_ITEMS
	setvar VAR_MEGA_MAN, 1
	pokemart MegaStone_Mart_Pokemart
	release
	end

Route114_EventScript_MegaTriggerL::
	lock
	playse SE_PIN
	applymovement LOCALID_MEGAMAN, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox Route114_Text_HeyTrainer, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, Route114_Movement_ApproachMegaL
	waitmovement 0
	msgbox Route114_Text_GiveBattleItems, MSGBOX_DEFAULT
	giveitem ITEM_MEGA_RING
	msgbox Route114_Text_ThereYaGo, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_BATTLE_ITEMS
	setvar VAR_MEGA_MAN, 1
	pokemart MegaStone_Mart_Pokemart
	release
	end

Route114_Movement_ApproachMegaR:
	walk_left
	walk_left
	step_end

Route114_Movement_ApproachMegaM:
	walk_left
	step_end

Route114_Movement_ApproachMegaL:
	face_left
	step_end

Route114_Text_HeyTrainer:
	.string "Hey trainer, over here!$"

