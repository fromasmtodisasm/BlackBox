-- defines entity classes of players in this game
--
-- _CLASS_ID is used for C++ (don't change those)
-- defines entity classes of players in this game
--
-- _CLASS_ID is used for C++ (don't change those)

EntityClassRegistry = 
{
-- entity_type,name,ID,script_file
--
	{"Player",		"Player",PLAYER_CLASS_ID,"Player/player.lua"},							--  1
	
	{"",					"Health",23,"Pickups/health.lua"},													-- 23
	{"",					"Armor",24,"Pickups/armor.lua"},														-- 24
	
	-----------------------------------------------------------------------------
	-- All Other classes -----------------------------------------------------------

	{ "",	"Checkpoint",	171,	"Pickups/Checkpoint.lua" },
	{ "",	"ProximityTrigger",	188,	"Triggers/ProximityTrigger.lua" },
	{ "",	"VisibilityTrigger",	189,	"Triggers/VisibilityTrigger.lua" },
	{ "",	"AreaTrigger",	190,	"Triggers/AreaTrigger.lua" },
	{ "",	"DelayTrigger",	193,	"Triggers/DelayTrigger.lua" },
	{ "",	"BoatTrampolineTrigger",	194,	"Triggers/BoatTrampolineTrigger.lua" },
	{ "",	"ImpulseTrigger",	195,	"Triggers/ImpulseTrigger.lua" },
	{ "",	"AITrigger",	196,	"Triggers/AITrigger.lua" },
	{ "",	"MultipleTrigger",	197,	"Triggers/MultipleTrigger.lua" },
	{ "",	"AISphere",	198,	"AI/AISphere.lua" },
	{ "",	"Watch",	201,	"Others/Watch.lua" },
	{ "",	"SaveableBasicEntity",	202,	"Others/SaveableBasicEntity.lua" },
	{ "",	"ProximityKeyTrigger",	203,	"Triggers/ProximityKeyTrigger.lua" },
};