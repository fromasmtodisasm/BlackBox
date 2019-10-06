player = {
	name = "PesBarbos",
	age = 47,
}

function player.OnInit( data )
	Console:PrintLine("Player OnInit");
	Console:PrintLine("Fps count = " .. Game:getFPS())
	Console:PrintLine("Username: " .. player.name)

	Console:Show(true)
	Game:gotoMenu(true)
end

function player.TestChanges()
	Console:PrintLine("TestChanges")
	Console:PrintLine("Username: " .. player.name)
end


function test(  )
	player.OnInit(123)
end

test()


