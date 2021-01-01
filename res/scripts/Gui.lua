Gui ={
	texture = 0
}

function Gui:Init()
	--self.texture = System:LoadTexture("fcsplash.bmp")
end


function Gui:OnDraw()
	--System:DrawImage(self.texture, 100, 100, 300, 300, 4)
	--int CScriptObjectSystem::Print(std::string text, float x, float y, float scale, float r, float g, float b, float a)
	System:Print("Press Ctrl+Alt+Shift+G than Ctrl+Alt+G", 0, 14, 1, 1, 1, 0, 1);
end

