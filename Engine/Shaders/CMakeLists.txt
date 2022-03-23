
start_sources()
# -- hwscripts.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "FX"
		"fx/PostProcess.fx"
		"fx/auxgeom.fx"
		"fx/bb.fx"
		"fx/common.fx"
		"fx/cube.fx"
		"fx/framebuffer.fx"
		"fx/gui.fx"
		"fx/hlsl_common.fx"
		"fx/pbr.fx"
		"fx/illum.fx"
		"fx/primitive.fx"
		"fx/screen.fx"
		"fx/screen_glsl.fx"
		"fx/shadow.fx"
		"fx/sprite.fx"
		"fx/z.fx"
)

end_sources()

CryfileContainer( Shaders SOLUTION_FOLDER "Shaders")

