# ##############################################################################
sources_platform(ALL)
start_sources()
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Root"
		"Main.cpp"
)
end_sources()

ConsoleApplication(PrependText)

