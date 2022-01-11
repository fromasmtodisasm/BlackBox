start_sources()
# -- launcher.waf_files --
sources_platform(WINDOWS)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Root"
		"App.config"
	SOURCE_GROUP "FormsProperties"
		"Form1.cs"
		"Form1.Designer.cs"
		"Program.cs"
		"Properties/AssemblyInfo.cs"
		"Properties/Resources.Designer.cs"
		"Properties/Resources.resx"
		"Properties/Settings.Designer.cs"
		"Properties/Settings.settings"
)

end_sources()

CSharpApplication(TestCSharp CSHARP SOLUTION_FOLDER "Sandbox" FRAMEWORK_VERSION "v4.6.1" )