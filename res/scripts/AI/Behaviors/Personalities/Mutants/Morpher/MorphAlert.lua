-- : ./AI/Behaviors/Personalities/Mutants/Morpher/MorphAlert.lua
--------------------------------------------------
--    Created By: Petar


AIBehaviour.MorphAlert = {
	Name = "MorphAlert",


	---------------------------------------------
	OnPlayerSeen = function( self, entity, fDistance )

		entity:SelectPipe(0,"morpher_attack_wrapper");

		if (entity:NotifyGroup()==nil) then
			AI:Signal(SIGNALFILTER_SUPERGROUP, 1, "HEADS_UP_GUYS",entity.id);
			AI:Signal(SIGNALFILTER_SUPERGROUP, 1, "wakeup",entity.id);
		end

		if (entity.AI_GunOut == nil) then
			entity:InsertSubpipe(0,"DRAW_GUN");
		end
		entity:InsertSubpipe(0,"DropBeaconAt");
	end,
	---------------------------------------------
	OnEnemyMemory = function( self, entity )
		-- called when the enemy can no longer see its foe, but remembers where it saw it last
	end,
	---------------------------------------------
	OnInterestingSoundHeard = function( self, entity )
		-- called when the enemy hears an interesting sound
	end,
	---------------------------------------------
	OnThreateningSoundHeard = function( self, entity )
		-- called when the enemy hears a scary sound
	end,
	---------------------------------------------
	OnReload = function( self, entity )
		-- called when the enemy goes into automatic reload after its clip is empty
	end,
	---------------------------------------------
	OnGroupMemberDied = function( self, entity )
		-- called when a member of the group dies
	end,
	---------------------------------------------
	OnNoHidingPlace = function( self, entity, sender )
		-- called when no hiding place can be found with the specified parameters
	end,	
	---------------------------------------------
	OnReceivingDamage = function ( self, entity, sender)
	end,
	--------------------------------------------------
	OnBulletRain = function ( self, entity, sender)
		-- called when the enemy detects bullet trails around him
	end,
	--------------------------------------------------
	OnCloseContact = function ( self, entity, sender)
		entity:GoVisible();
		entity:SelectPipe(0,"morpher_attack_wrapper");
		if (entity.MELEE_ANIM_COUNT) then
			local rnd = random(1,entity.MELEE_ANIM_COUNT);
			local melee_anim_name = format("attack_melee%01d",rnd);
			entity:InsertAnimationPipe(melee_anim_name,3);
		else
			Hud:AddMessage("==================UNACCEPTABLE ERROR====================");
			Hud:AddMessage("Entity "..entity:GetName().." made melee attack but has no melee animations.");
			Hud:AddMessage("==================UNACCEPTABLE ERROR====================");
		end
	end,
	--------------------------------------------------
	DECIDE_IF_INVESTIGATING_MORE = function ( self, entity, sender)
		entity:SelectPipe(0,"stealth_hunt");
	end,


}