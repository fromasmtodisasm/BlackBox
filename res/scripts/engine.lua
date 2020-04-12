config = {
	r_window_width = 1366,
	r_window_height = 768,
	r_bpp = 32,
	r_zbpp = 24,
	r_sbpp = 8,
	r_fullscreen = 0, 
	r_debug = 1,
	r_backbuffer_w = 2560,
	r_backbuffer_h = 1600, 
	r_cam_w = 0,
	r_cam_h = 0,

	fogR = 0.0,
	fogG = 0.01,
	fogB = 0.01,

	shader_path = "res/shaders/",
	image_path = "res/images/",
	geometry_path = "res/geom/",
	materials_path = "res/materials/",
	scenes_path = "res/scenes/",

	console_background = "console/bb.jpg",

	game_config = "shadow.cfg",

	testid = 1, 
	shader_print = 1,
	sh_use_cache = 0,
}

function print_config()
	for k,v in pairs(config) do
		Console:PrintLine(k .. "\t=\t" .. v)
	end
end
