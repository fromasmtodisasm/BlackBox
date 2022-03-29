
start_sources()
# -- hwscripts.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "FX"
		"fx/PostProcess.fx"
		"fx/auxgeom.fx"
		"fx/bb.fx"
		"fx/cube.fx"
		"fx/framebuffer.fx"
		"fx/gui.fx"
		"fx/illum.fx"
		"fx/primitive.fx"
		"fx/screen.fx"
		"fx/shadow.fx"
		"fx/sprite.fx"
		"fx/z.fx"
	SOURCE_GROUP "FX\\\\Include"
		"fx/common.fx"
		"fx/shadeLib.fx"
)

end_sources()

CryfileContainer( Shaders SOLUTION_FOLDER "Shaders")

