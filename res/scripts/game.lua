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

function player.TestChanges(string)
	Console:PrintLine("TestChanges"):PrintLine("call chain"..string)
	Console:PrintLine("Username: " .. player.name)

	return Console;
end


function test(  )
	player.OnInit(123)
end

test()


