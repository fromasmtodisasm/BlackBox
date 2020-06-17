Gui ={
	texture = 0
}

function Gui:Init()
	self.texture = System:LoadTexture("fcsplash.bmp")
end


function Gui:OnDraw()
	System:DrawImage(self.texture, 100, 100, 300, 300, 4)
end

