
start_sources()
# -- hwscripts.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "FX"
		"fx/auxgeom.fx"
		"fx/bb.fx"
		"fx/common.fx"
		"fx/cube.fx"
		"fx/framebuffer.fx"
		"fx/gui.fx"
		"fx/hlsl_common.fx"
		"fx/primitive.fx"
		"fx/screen.fx"
		"fx/screen_glsl.fx"
		"fx/sprite.fx"
		"fx/test.fx"
)

end_sources()

CryfileContainer( Shaders SOLUTION_FOLDER "BlackBox/Shaders")

