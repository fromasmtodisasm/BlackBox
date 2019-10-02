player = {
	["name"] = "user",
	["age"] = 47,
}


Console:PrintLine("Begin game update");
Console:Show(true)
Game:gotoMenu(true)
Console:PrintLine("Fps count = " .. Game:getFPS())

