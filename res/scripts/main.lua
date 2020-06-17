function Init()
	--System:LoadFont("radiosta");
	--System:LoadFont("hud");
	Script:ReloadScript("scripts/common.lua");
	Script:ReloadScript("scripts/game.lua");
	Script:ReloadScript("scripts/Gui.lua");
	--Script:ReloadScript("scripts/methoddispatcher.lua");
	--Script:ReloadScript("scripts/saveutils.lua" );

	-----------------------------------------------------------------
	-- loading of the game mission list
	------------------------------------------------------------
	--Script:ReloadScript("scripts/defiant.lua");
	--Script:ReloadScript("scripts/DebugTagPointsMgr.lua");
	
end

function Shutdown()
	Game:SaveConfiguration();
end

