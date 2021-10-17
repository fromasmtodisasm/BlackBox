Script:LoadScript("scripts/materials/commoneffects.lua");
Materials["mat_concrete"] = {
	type="concrete",
-------------------------------------
	PhysicsSounds=PhysicsSoundsTable.Hard,
-------------------------------------	
	bullet_drop_single = CommonEffects.common_bullet_drop_single_ashphalt,
	bullet_drop_rapid = CommonEffects.common_bullet_drop_rapid_ashphalt,
-------------------------------------	
	bullet_hit = {
		sounds = {
			{"Sounds/bullethits/brock1.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/brock2.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/brock3.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/brock4.wav",SOUND_UNSCALABLE,200,5,60},
		},
		
		decal = { 
			texture = System:LoadTexture("Textures/Decal/concrete.dds"),
			scale = 0.045,
			random_scale = 44
		},
		
		particleEffects = {
			name = "bullet.hit_concrete.a",
		},
	},

	pancor_bullet_hit = {
		sounds = {
			{"Sounds/bullethits/brock1.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/brock2.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/brock3.wav",SOUND_UNSCALABLE,200,5,60},
			{"Sounds/bullethits/brock4.wav",SOUND_UNSCALABLE,200,5,60},
		},
		
		decal = { 
			texture = System:LoadTexture("Textures/Decal/concrete.dds"),
			scale = 0.045,
			random_scale = 44
		},
		
		particleEffects = {
			name = "bullet.hit_concrete_pancor.a",
		},
	},


-- primary explosion from rocket

	projectile_hit = 
		{
		
				particleEffects = {
				name = "explosions.rocket_nofire.a",
				},
	
		},

	mortar_hit = CommonEffects.common_mortar_hit,
	smokegrenade_hit = CommonEffects.common_smokegrenade_hit,
	flashgrenade_hit = CommonEffects.common_flashgrenade_hit,
	melee_slash = {
		sounds = {
			{"sounds/weapons/machete/macheteconc1.wav",SOUND_UNSCALABLE,185,5,30,
			  {fRadius=10,fInterest=1,fThreat=0,},
			},
			{"sounds/weapons/machete/macheteconc2.wav",SOUND_UNSCALABLE,185,5,30,
			  {fRadius=10,fInterest=1,fThreat=0,},
			},
			{"sounds/weapons/machete/macheteconc3.wav",SOUND_UNSCALABLE,185,5,30,
			  {fRadius=10,fInterest=1,fThreat=0,},
			},
		},
		particleEffects = {
			name = "bullet.hit_rock.a",
				},

	},
-------------------------------------
	player_walk = CommonEffects.player_conc_walk,
	player_run = CommonEffects.player_conc_run,
	player_crouch = CommonEffects.player_conc_crouch,
	player_prone = CommonEffects.player_conc_prone,
	player_walk_inwater = CommonEffects.player_walk_inwater,
	
	player_drop = {
		sounds = {
			{"sounds/player/bodyfalls/bodyfallrock1.wav",SOUND_UNSCALABLE,210,10,150},
			{"sounds/player/bodyfalls/bodyfallrock2.wav",SOUND_UNSCALABLE,210,10,150},
		},

	},
	gameplay_physic = {
		piercing_resistence = 15,
		friction = 1.0,
		bouncyness= 0.2, --default 0
	},

	AI = {
		fImpactRadius = 5,
	},
			
	--vehicle effects: particle is called when wheels are slipping, smoke in any case if the vehicle is moving.
	VehicleParticleEffect = CommonEffects.common_vehicle_smoke_concrete,
	--VehicleSmokeEffect = CommonEffects.common_vehicle_smoke_concrete,
}