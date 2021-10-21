function Texture(type, name)
	local result = {}
	result.type = type;
	result.name = name
	return result
end

materials = 
{
	sprite2 = 
	{
		textures = {
			Texture("diffuse", "DirectXLogo.png")
		},
		shader = "Phong"
	},
	dx_box = 
	{
		alpha = 0.2,
		textures = {
			Texture("diffuse", "opengl.png")
		},
		shader = "Phong"
	},
	specular_box = 
	{
		textures = {
			Texture("diffuse", "DirectXLogo.png")
		},
		shader = "Phong"
	},
	terrain = 
	{
		textures = {
			Texture("diffuse", "ground.jpg")
		},
		shader = "Phong"
	},
	terrain = 
	{
		textures = {
			Texture("diffuse", "ground.jpg")
		},
		shader = "Phong"
	},
	wood_box = 
	{
		textures = {
			Texture("diffuse", "container.jpg")
		},
		shader = "Phong"
	},
	wood_box = 
	{
		textures = {
			Texture("diffuse", "coral.jpg"),
			Texture("specular", "coral.jpg"),
		},
		shader = "Phong"
	},
	player = 
	{
		textures = {
			Texture("diffuse", "pengium.jpg"),
			Texture("specular", "gray.png"),
		},
		shader = "Phong"
	},
	bb = { shader = "bb" },
	player = 
	{
		textures = {
			Texture("diffuse", "check.png"),
		},
		shader = "Phong"
	},
	default = 
	{
		textures = {
			Texture("diffuse", "check.png"),
			Texture("normal", "normal_up.jpg")
		},
		shader = "Phong"
	},
}
