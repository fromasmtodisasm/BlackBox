-- : ./FFA/Hud/scoreboard.lua
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
Script:LoadScript("SCRIPTS/GUI/ScoreBoardLib.lua");
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
function ScoreBoardManager:Render()
	self:RenderDMGame();
end
