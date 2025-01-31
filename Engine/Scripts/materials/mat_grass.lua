-- : ./materials/mat_grass.lua
--#Script:ReloadScript("scripts/materials/mat_grass.lua");
Script:LoadScript("scripts/materials/commoneffects.lua");
Materials["mat_grass"] = {
	type="grass",
-------------------------------------
	PhysicsSounds=PhysicsSoundsTable.Soft,
-------------------------------------
	bullet_hit = {
		sounds = {
			{"Sounds/bullethits/bgrass1.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/bgrass2.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/bgrass3.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/bgrass4.wav",SOUND_UNSCALABLE,200,5,60},
			
			
		},
		
		decal = { 
			texture = System:LoadTexture("Textures/Decal/ground.dds"),
			scale = 0.03,
		},

		particleEffects = {
			name = "bullet.hit_leaf.a",
		},
	},

	pancor_bullet_hit = {
		sounds = {
			{"Sounds/bullethits/bgrass1.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/bgrass2.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/bgrass3.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/bgrass4.wav",SOUND_UNSCALABLE,200,5,60},
			
			
		},
		
		decal = { 
			texture = System:LoadTexture("Textures/Decal/ground.dds"),
			scale = 0.03,
		},

		particleEffects = {
			name = "bullet.hit_leaf.a",
		},
	},

	projectile_hit = CommonEffects.common_projectile_hit,
	mortar_hit = CommonEffects.common_mortar_hit,
	smokegrenade_hit = CommonEffects.common_smokegrenade_hit,
	flashgrenade_hit = CommonEffects.common_flashgrenade_hit,
	grenade_hit = CommonEffects.common_grenade_hit,
	melee_slash = {
		sounds = {
			{"sounds/weapons/machete/machetesand4.wav",SOUND_UNSCALABLE,255,5,30,{fRadius=10,fInterest=1,fThreat=0,},},
		},
		particleEffects = {
			name = "bullet.hit_leaf.a",
		},

	},
-------------------------------------
	player_walk = CommonEffects.player_grass_walk,
	player_run = CommonEffects.player_grass_run,
	player_crouch = CommonEffects.player_grass_crouch,
	player_prone = CommonEffects.player_grass_prone,
	player_walk_inwater = CommonEffects.player_walk_inwater,
	
-------------------------------------
	player_drop = {
		sounds = {
			{"sounds/player/bodyfalls/bodyfallgrass1.wav",SOUND_UNSCALABLE,210,10,150},
			{"sounds/player/bodyfalls/bodyfallgrass2.wav",SOUND_UNSCALABLE,210,10,150},
		},
	},
-------------------------------------
	
	gameplay_physic = {
		piercing_resistence = 2,
		friction = 0.6,
		bouncyness= -2, -- default 0
		no_collide=1,
	},

---------------------------------------------
	AI = {
		fImpactRadius = 5,
	},	
	
	--vehicle effects: particle is called when wheels are slipping, smoke in any case if the vehicle is moving.
	VehicleParticleEffect = CommonEffects.common_vehicle_particles_grass,
	--VehicleSmokeEffect = CommonEffects.common_vehicle_smoke_grass,
}