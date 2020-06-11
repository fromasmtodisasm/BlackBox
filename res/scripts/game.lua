Player = {
	name = "PesBarbos",
	age = 47,
	alltime = 0.0
}

function Player:OnInit( data )
	Console:PrintLine("Player OnInit");
	Console:PrintLine("Username: " .. self.name)

	--Console:Show(true)
	--Game:gotoMenu(true)

	local formats = System:EnumDisplayFormats()
	--for i=1, #formats do
	--	Console:PrintLine("["..i.."]".. formats[i].width .. " x " .. formats[i].height .. " x " .. formats[i].bpp)
	--end

end

function Player:PostInit( )
	Game:LoadLevel("test")
	setvar("gravity", 200)
	setvar("floor", 20)
	System:ShowConsole(false)
	--Game:gotoMenu()
end


function Player:TestChanges(string)
	Console:PrintLine("TestChanges"):PrintLine("call chain"..string)
	Console:PrintLine("Username: " .. self.name)
	

	return Console;
end

function Player:Update( dt )
	self.alltime = self.alltime + dt;
end



function test(  )
	Player:OnInit(123)
end


Mission = {}

function Mission:OnCheckpointLoaded()
	Console:PrintLine("Checkpoint Loaded")
end

