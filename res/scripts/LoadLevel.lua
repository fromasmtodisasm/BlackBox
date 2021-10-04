objects = {
	bmw = {
		mesh =  "res/geom/bmw.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "dx_box",
		visible =  "1",
		transparent =  "0"
	},
	--[[
	MyPlayer = {
		mesh =  "res/geom/pengium.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "player",
		material =  "player",
		visible =  "1",
		transparent =  "0"
	},
	terrain = {
		mesh =  "res/geom/terrain.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "terrain",
		visible =  "1",
		transparent =  "1"
	},
	dx_box = {
		mesh =  "res/geom/box.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "dx_box",
		visible =  "1",
		transparent =  "0"
	},
	brick_normal_box = {
		mesh =  "res/geom/box.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "specular_box",
		visible =  "1",
		transparent =  "0"
	},
	light = {
		mesh =  "res/geom/cube.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "light",
		visible =  "0",
		transparent =  "0"
	},
	water = {
		mesh =  "res/geom/plane.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "water",
		visible =  "1",
		transparent =  "1"
	},
	quad = {
		mesh =  "res/geom/plane.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "water",
		visible =  "1",
		transparent =  "0"
	},
	sphere = {
		mesh =  "res/geom/sphere.obj",
		transform =  {position = { x = 0, y = 0, z = 0}, rotation = { x = 0, y = 0, z = 0}, scale = { x = 0, y = 0, z = 0}},
		type =  "object",
		material =  "red",
		visible =  "0",
		transparent =  "0"
	}
	]]
}


function LoadLevel(level)
	System:Log("Loading Level: "..level)
	for k, v in pairs(objects) do
		System:Log(v.mesh)
		Game:LoadObject(v.mesh)
	end
end