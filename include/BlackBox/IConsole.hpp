#pragma once

#include <cstdint>
#include <string>
#include <vector>

#ifdef IMGUICONSOLE
struct ImGuiInputTextCallbackData;

struct IConsole
{
  virtual void    ClearLog() = 0;
  virtual void    AddLog(const char* fmt, ...) = 0;
  virtual void    Draw(const char* title, bool* p_open) = 0;
  virtual void    ExecCommand(const char* command_line) = 0;
  //virtual static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) = 0;
  virtual int     TextEditCallback(ImGuiInputTextCallbackData* data) = 0;
};
#endif // IMGUICONSOLE

struct IConsoleCommand;
struct CommandDesc;
struct ICVar;
struct ITexture;

//! Flags used by ICVar.
enum EVarFlags : unsigned int 
{
	VF_NULL = 0x00000000,      //!< Just to have one recognizable spot where the flags are located in the Register call.
	VF_CHEAT = 0x00000002,      //!< Stays in the default state when cheats are disabled.
	VF_DEV_ONLY = 0x00000004,      //!< Cvar is only registered with the console in non release builds.
	VF_DEDI_ONLY = 0x00000008,      //!< Cvar is only registered with the console in non release or dedicated server builds.
	VF_NET_SYNCED = 0x00000080,      //!< Synchronised between server and client(s); server is authoritative.
	VF_DUMPTODISK = 0x00000100,
	VF_READONLY = 0x00000800,      //!< Can not be changed by the user.
	VF_REQUIRE_LEVEL_RELOAD = 0x00001000,
	VF_REQUIRE_APP_RESTART = 0x00002000,
	VF_WARNING_NOTUSED = 0x00004000,      //!< Shows warning that this var was not used in config file.
	VF_COPYNAME = 0x00008000,      //!< Otherwise the const char * to the name will be stored without copying the memory.
	VF_MODIFIED = 0x00010000,      //!< Set when variable value modified.
	VF_WASINCONFIG = 0x00020000,      //!< Set when variable was present in config file.
	VF_BITFIELD = 0x00040000,      //!< Allow bitfield setting syntax.
	VF_RESTRICTEDMODE = 0x00080000,      //!< Is visible and usable in restricted (normal user) console mode.
	VF_INVISIBLE = 0x00100000,      //!< Invisible to the user in console.
	VF_ALWAYSONCHANGE = 0x00200000,      //!< Always accept variable value and call on change callback even if variable value didnt change.
	VF_BLOCKFRAME = 0x00400000,      //!< Blocks the execution of console commands for one frame.
	VF_CONST_CVAR = 0x00800000,      //!< Set if it is a const cvar not to be set inside cfg-files.
	VF_CHEAT_ALWAYS_CHECK = 0x01000000,      //!< This variable is critical to check in every hash, since it's extremely vulnerable.
	VF_CHEAT_NOCHECK = 0x02000000,      //!< This variable is set as VF_CHEAT but doesn't have to be checked/hashed since it's harmless to hack.
	VF_SYSSPEC_OVERWRITE = 0x04000000,      //!< This variable is specified by system.cfg with the intention to overwrite all subsequent settings.
	VF_CVARGRP_IGNOREINREALVAL = 0x08000000,      //!< This variable will be ignored when cvar group's real val is checked (Needed for cvars which are in a group but change in various situations).
	VF_LIVE_CREATE_SYNCED = 0x10000000,      //!< This variable will be synced with LiveCreate clients.
	VF_RENDERER_CVAR = 0x20000000,      //!< The update of this variable will be done in render thread.
	VF_DEPRECATED = 0x40000000,      //!< Deprecated cvars use default values which cannot be modified outside the code.
	VF_EXPERIMENTAL = 0x80000000,      //!< This variable is used by WIP or experimental feature.
};

#define     CVAR_INT    1
#define     CVAR_FLOAT  2
#define     CVAR_STRING 3
#define     CVAR_OBJECT 4

struct IWorkerCommand
{
	virtual bool OnUpdate() = 0;
};

struct IConsoleCommand
{
	virtual bool execute(CommandDesc& cd) = 0;
};

//! This a definition of the console command function that can be added to console with AddCommand.
typedef bool (*ConsoleCommandFunc)(CommandDesc&);

//////////////////////////////////////////////////////////////////////
struct ICVarDumpSink
{
	virtual void OnElementFound(ICVar* pCVar) = 0;
};

//////////////////////////////////////////////////////////////////////
//! Callback class to derive from when you want to recieve callbacks when console var changes.
struct IConsoleVarSink
{
	//! Called by Console before changing console var value, to validate if var can be changed.
	//! @return true if ok to change value, false if should not change value.
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) = 0;
};


struct CommandDesc
{
	std::wstring command;
	std::vector<std::wstring> args;
	std::vector<std::wstring> *history;
	CommandDesc() : history(nullptr) {}
	CommandDesc(decltype(history) *history) : history(nullptr)
	{

	}
	int argsCount()
	{
		return args.size();
	}
  std::wstring get(int i)
  {
    if (i < args.size())
      return args[i];
    return std::wstring();
  }
};

struct IConsole
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Crate a new console variable
		@param sName console variable name
		@param sValue default value
		@param nFlag user definded flag, this parameter is used by other subsystems
			and doesn't affect the console varible (basically of user data)
		@return a pointer to the interface ICVar
		@see ICVar
	*/
	virtual ICVar* CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help = "") = 0;
	virtual ICVar* CreateVariable(const char* sName, int iValue, int nFlags, const char* help = "") = 0;
	virtual ICVar* CreateVariable(const char* sName, float fValue, int nFlags, const char* help = "") = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Dump all console-variables to a callback-interface
		@param Callback callback-interface which needs to be called for each element
	*/
	virtual void DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter = 0) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! show/hide the console
		@param specifies if the window must be (true=show,false=hide)
	*/
	virtual void	ShowConsole(bool show) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Sets the background-image
		@param pImage background-image
	*/
	virtual void	SetImage(struct ITexture* pTexture) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Update the console
	*/
	virtual void	Update() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! Draw the console
		*/
	virtual void	Draw() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Register a new console command.
	//! \param sCommand Command name.
	//! \param func     Pointer to the console command function to be called when command is invoked.
	//! \param nFlags   Bitfield consisting of VF_ flags (e.g. VF_CHEAT).
	//! \param sHelp    Help string, will be displayed when typing in console "command ?".
	virtual void AddCommand(const char* sCommand, ConsoleCommandFunc func, int nFlags = 0, const char* help = NULL) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void AddCommand(const char* sName, IConsoleCommand *command, const char* help = "") = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Add a Console command
		@param sName name of the command (ex "connect")
		@param sScriptFunc script buffer the contain the command implementation
			EG "Game.Connect(%1)" the symbol "%1" will be replaced with the command parameter 1
			writing in the console "connect 127.0.0.1" will invoke Game.Connect("127.0.0.1")
		@param indwFlags bitfield consist of VF_ flags (e.g. VF_CHEAT)
	*/
	virtual void AddCommand(const char* sName, const char* sScriptFunc, const uint32_t indwFlags = 0, const char* help = "") = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Execute a string in the console
		@param command console command
	*/
	virtual void ExecuteString(const char* command) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Execute a file in the console
	virtual void ExecuteFile(const char* file) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Return true if the console is opened
		@return the variable value(true = opened/false = closed)
	*/
	virtual bool IsOpened() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! Add a argument completion for command
	virtual void AddArgumentCompletion(const char* cmd, const char* arg, int n) = 0;

	virtual void Clear() = 0;

	virtual void Help(const char *cmd) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Retrieve a console variable by name
		@param sName variable name
		@param bCaseSensitive true=faster, false=much slower but allowes names with wrong case (use only where performce doesn't matter)
		@return a pointer to the ICVar interface, NULL if is not found
		@see ICVar
	*/
	virtual ICVar* GetCVar(const char* name, const bool bCaseSensitive = true) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Print a string in the console and go to the new line
		@param s the string to print
	*/
	virtual void PrintLine(const char* s, ...) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Console variable sink.
	//////////////////////////////////////////////////////////////////////////
	//! Adds a new console variables sink callback.
	virtual void AddConsoleVarSink(IConsoleVarSink* pSink) = 0;
	//! Removes a console variables sink callback.
	virtual void RemoveConsoleVarSink(IConsoleVarSink* pSink) = 0;
	//! \param szLine Must not be 0.
	virtual void SetInputLine(const char* szLine) = 0;


	virtual void AddWorkerCommand(IWorkerCommand* cmd) = 0;
	virtual void RemoveWorkerCommand(IWorkerCommand* cmd) = 0;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! this interface is the 1:1 "C++ representation"
//! of a console variable.
//! NOTE: a console variable is accessible in C++ trough
//! this interface and in all scripts as global variable
//! (with the same name of the variable in the console)
struct ICVar
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! delete the variable
		NOTE: the variable will automatically unregister itself from the console
	*/
	virtual void Release() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! Return the integer value of the variable
			@return the value
		*/
	virtual int GetIVal() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! Return the float value of the variable
			@return the value
		*/
	virtual float GetFVal() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! Return the string value of the variable
			@return the value
		*/
	virtual char* GetString() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! set the string value of the variable
			@param s string representation the value
		*/
	virtual void Set(const char* s) = 0;

	/*! Force to set the string value of the variable - can be called
			from inside code only
		@param s string representation the value
	*/
	virtual void ForceSet(const char* s) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! set the float value of the variable
			@param s float representation the value
		*/
	virtual void Set(float f) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! set the float value of the variable
			@param s integer representation the value
		*/
	virtual void Set(int i) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! refresh the values of the variable
		*/
	virtual void Refresh() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! clear the specified bits in the flag field
		*/
	virtual void ClearFlags(int flags) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! return the variable's flags
			@return the variable's flags
		*/
	virtual int GetFlags() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! Set the variable's flags
		*/
	virtual int SetFlags(int flags) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! return the primary variable's type
			@return the primary variable's type
		*/
	virtual int GetType() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! return the variable's name
			@return the variable's name
		*/
	virtual const char* GetName() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*! return the variable's name
			@return the variable's name
		*/
	virtual const char* GetHelp() = 0;

};

#define CREATE_CONSOLE_VAR(name, value, flags, ...) GetISystem()->GetIConsole()->CreateVariable(name, value, flags, __VA_ARGS__)
