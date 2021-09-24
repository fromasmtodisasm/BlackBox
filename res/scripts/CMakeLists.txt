start_sources()
# -- hwscripts.waf_files --
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Scripts"
)

end_sources()

CryfileContainer( Scripts SOLUTION_FOLDER "BlackBox/Scripts")

