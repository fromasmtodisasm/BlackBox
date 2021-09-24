#pragma once

#include <cstdint>
#include <string>
#include <vector>

#ifdef IMGUICONSOLE
struct ImGuiInputTextCallbackData;

struct IConsole
{
	virtual void ClearLog()							   = 0;
	virtual void Log(const char* fmt, ...)			   = 0;
	virtual void Draw(const char* title, bool* p_open) = 0;
	virtual void ExecCommand(const char* command_line) = 0;
	//virtual static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) = 0;
	virtual int TextEditCallback(ImGuiInputTextCallbackData* data) = 0;
};
#endif // IMGUICONSOLE

struct IConsoleCommand;
struct CommandDesc;
struct ICVar;
class CXFont;
struct ITexture;

//! Flags used by ICVar.
enum EVarFlags : uint64
{
	VF_NULL					   = 0x00000000, //!< Just to have one recognizable spot where the flags are located in the Register call.
	VF_CHEAT				   = 0x00000002, //!< Stays in the default state when cheats are disabled.
	VF_DEV_ONLY				   = 0x00000004, //!< Cvar is only registered with the console in non release builds.
	VF_DEDI_ONLY			   = 0x00000008, //!< Cvar is only registered with the console in non release or dedicated server builds.
	VF_SAVEGAME				   = 0x00000010,
	VF_NET_SYNCED			   = 0x00000080, //!< Synchronised between server and client(s); server is authoritative.
	VF_DUMPTODISK			   = 0x00000100,
	VF_READONLY				   = 0x00000800, //!< Can not be changed by the user.
	VF_REQUIRE_LEVEL_RELOAD	   = 0x00001000,
	VF_REQUIRE_APP_RESTART	   = 0x00002000,
	VF_WARNING_NOTUSED		   = 0x00004000, //!< Shows warning that this var was not used in config file.
	VF_COPYNAME				   = 0x00008000, //!< Otherwise the const char * to the name will be stored without copying the memory.
	VF_MODIFIED				   = 0x00010000, //!< Set when variable value modified.
	VF_WASINCONFIG			   = 0x00020000, //!< Set when variable was present in config file.
	VF_BITFIELD				   = 0x00040000, //!< Allow bitfield setting syntax.
	VF_RESTRICTEDMODE		   = 0x00080000, //!< Is visible and usable in restricted (normal user) console mode.
	VF_INVISIBLE			   = 0x00100000, //!< Invisible to the user in console.
	VF_ALWAYSONCHANGE		   = 0x00200000, //!< Always accept variable value and call on change callback even if variable value didnt change.
	VF_BLOCKFRAME			   = 0x00400000, //!< Blocks the execution of console commands for one frame.
	VF_CONST_CVAR			   = 0x00800000, //!< Set if it is a const cvar not to be set inside cfg-files.
	VF_CHEAT_ALWAYS_CHECK	   = 0x01000000, //!< This variable is critical to check in every hash, since it's extremely vulnerable.
	VF_CHEAT_NOCHECK		   = 0x02000000, //!< This variable is set as VF_CHEAT but doesn't have to be checked/hashed since it's harmless to hack.
	VF_SYSSPEC_OVERWRITE	   = 0x04000000, //!< This variable is specified by system.cfg with the intention to overwrite all subsequent settings.
	VF_CVARGRP_IGNOREINREALVAL = 0x08000000, //!< This variable will be ignored when cvar group's real val is checked (Needed for cvars which are in a group but change in various situations).
	VF_LIVE_CREATE_SYNCED	   = 0x10000000, //!< This variable will be synced with LiveCreate clients.
	VF_RENDERER_CVAR		   = 0x20000000, //!< The update of this variable will be done in render thread.
	VF_DEPRECATED			   = 0x40000000, //!< Deprecated cvars use default values which cannot be modified outside the code.
	VF_EXPERIMENTAL			   = 0x80000000, //!< This variable is used by WIP or experimental feature.

	VF_BOOL			   = 0x0000000200000000, //!< This variable is used by WIP or experimental feature.
};

#define CVAR_INT 1
#define CVAR_FLOAT 2
#define CVAR_STRING 3
#define CVAR_OBJECT 4

enum class ECVarType
{
	Invalid = 0,
	Int     = 1,
	Float   = 2,
	String  = 3,
	Int64   = 4
};

#ifdef EXCLUDE_CVARHELP
#define CVARHELP(_comment) 0
#else
#define CVARHELP(_comment) _comment
#endif

struct IWorkerCommand
{
	virtual bool OnUpdate() = 0;
};

struct IConsoleCommand
{
	virtual bool execute(CommandDesc& cd) = 0;
};

//////////////////////////////////////////////////////////////////////
struct ICVarDumpSink
{
	virtual void OnElementFound(ICVar* pCVar) = 0;
};

//////////////////////////////////////////////////////////////////////
struct IKeyBindDumpSink
{
	virtual void OnKeyBindFound(const char* sBind, const char* sCommand) = 0;
};

//////////////////////////////////////////////////////////////////////
struct IOutputPrintSink
{
	virtual void Print(const char* inszText) = 0;
};

//////////////////////////////////////////////////////////////////////
//! Callback class to derive from when you want to receive callbacks when console var changes.
struct IConsoleVarSink
{
	// <interfuscator:shuffle>
	virtual ~IConsoleVarSink()
	{
	}

	//! Called by Console before changing console var value, to validate if var can be changed.
	//! \return true if ok to change value, false if should not change value.
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) = 0;

	//! Called by Console after variable has changed value.
	virtual void OnAfterVarChange(ICVar* pVar) = 0;

	//! Called by Console after variable has been unregistered.
	virtual void OnVarUnregister(ICVar* pVar) = 0;
	// </interfuscator:shuffle>
};

//! Interface to the arguments of the console command.
struct IConsoleCmdArgs
{
	// <interfuscator:shuffle>
	virtual ~IConsoleCmdArgs()
	{
	}

	//! Gets number of arguments supplied to the command (including the command itself).
	virtual int GetArgCount() const = 0;

	//! Gets argument by index, nIndex must be in 0 <= nIndex < GetArgCount().
	virtual const char* GetArg(int nIndex) const = 0;

	//! Gets complete command line.
	virtual const char* GetCommandLine() const = 0;
	// </interfuscator:shuffle>
};

//! Interface to the arguments of the console command.
struct IConsoleArgumentAutoComplete
{
	// <interfuscator:shuffle>
	virtual ~IConsoleArgumentAutoComplete(){}

	//! Gets number of matches for the argument to auto complete.
	virtual int GetCount() const = 0;

	//! Gets argument value by index, nIndex must be in 0 <= nIndex < GetCount().
	virtual const char* GetValue(int nIndex) const = 0;
	// </interfuscator:shuffle>
};


//! This a definition of the console command function that can be added to console with AddCommand.
typedef void (* ConsoleCommandFunc)(IConsoleCmdArgs*);

//! This a definition of the callback function that is called when variable change.
typedef void (* ConsoleVarFunc)(ICVar*);

//! This a definition of the console command function that can be added to console with AddCommand.
//typedef void (* ConsoleCommandFunc)(IConsoleCmdArgs*);

struct CommandDesc
{
	std::wstring command;
	std::vector<std::wstring> args;
	std::vector<std::wstring>* history;
	CommandDesc()
		: history(nullptr)
	{
	}
	CommandDesc(decltype(history)* history)
		: history(nullptr)
	{
	}
	auto argsCount()
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
//////////////////////////////////////////////////////////////////////
/*! Interface to the engine console.

	The engine console allow to manipulate the internal engine parameters
	and to invoke commands.
	This interface allow external modules to integrate their functionalities
	into the console as commands or variables.

	IMPLEMENTATIONS NOTES:
	The console takes advantage of the script engine to store the console variables,
	this mean that all variables visible through script and console.

*/ 
struct IBaseConsole
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! delete the variable
		NOTE: the variable will automatically unregister itself from the console
	*/
	virtual void Release() = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Crate a new console variable
		@param sName console variable name
		@param sValue default value
		@param nFlag user definded flag, this parameter is used by other subsystems 
			and doesn't affect the console varible (basically of user data)
		@return a pointer to the interface ICVar
		@see ICVar
	*/
	virtual ICVar *CreateVariable(const char *sName,const char *sValue,int nFlags, const char *help = "")=0;
	virtual ICVar *CreateVariable(const char *sName,int iValue,int nFlags, const char *help = "")=0;
	virtual ICVar *CreateVariable(const char *sName,float fValue,int nFlags, const char *help = "")=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Remove a variable from the console
		@param sVarName console variable name
		@param bDelete if true the variable is deleted
		@see ICVar
	*/
	virtual void UnregisterVariable(const char *sVarName,bool bDelete=false) = 0 ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Set the y coordinate where the console will stop to scroll when is dropped
		@param value y in screen coordinates
	*/
	virtual void SetScrollMax(int value)=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! add output sink (clases which are interested in the output) - order is not guaranteed
		@param inpSink must not be 0 and is not allowed to be added twice
	*/
	virtual void AddOutputPrintSink( IOutputPrintSink *inpSink )=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! remove output sink (clases which are interested in the output) - order is not guaranteed
		@param inpSink must not be 0 and has to be added before
	*/
	virtual void RemoveOutputPrintSink( IOutputPrintSink *inpSink )=0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! show/hide the console
		@param specifies if the window must be (true=show,false=hide)
	*/
	virtual void	ShowConsole(bool show)=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Crate a new console variable that store the value in a user defined memory block
		@param sName console variable name
		@param src pointer to the memory that will store the value 
		@param value default value
		@param type type of the value (can be CVAR_INT|CVAR_FLOAT)
		@return the value
		@see ICVar
	*/
	virtual int Register(const char *name, void  *src, float defaultvalue, int flags, int type, const char *help = "")=0;    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Crate a new console variable that store the value in a user defined floating point
		@param sName console variable name
		@param src pointer to the memory that will store the value 
		@param value default value
		@return the value
		@see ICVar
	*/
	virtual float Register(const char *name, float *src, float defaultvalue, int flags=0, const char *help = "")=0;    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Crate a new console variable that store the value in a user defined integer
		@param sName console variable name
		@param src pointer to the memory that will store the value 
		@param value default value
		@return the value
		@see ICVar
	*/
	virtual int Register(const char *name, int   *src, float defaultvalue, int flags=0, const char *help = "")=0;    
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Dump all console-variables to a callback-interface
		@param Callback callback-interface which needs to be called for each element
	*/
	virtual void DumpCVars(ICVarDumpSink *pCallback,unsigned int nFlagsFilter=0 )=0;
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Bind a console command to a key
		@param sCmd console command that must be executed
		@param sRes name of the key to invoke the command
		@param bExecute legacy parameter(will be removed soon)
	*/
	virtual void CreateKeyBind(const char *sCmd,const char *sRes,bool bExecute)=0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Sets the background-image
		@param pImage background-image
	*/
	virtual void	SetImage(struct ITexPic *pImage,bool bDeleteCurrent)=0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Retrieves the background-image
		@return background-image
	*/
	virtual struct ITexPic *GetImage()=0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Sets static/scroll background-mode
		@param bStatic true if static
	*/
	virtual void StaticBackground(bool bStatic)=0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Sets the loading-background-image
	@param pImage background-image
	*/
	virtual void	SetLoadingImage( const char *szFilename )=0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Iterate through the lines - used for dedicated server (truncated if needed)
		@param indwLineNo 0.. counted from the last printed line on
		@param outszBuffer pointer to the destination string buffer (zero terminted afterwards), must not be 0
		@param indwBufferSize 1.. size of the buffer
		@return true=line was returned, false=there are no more lines
	*/

	virtual bool GetLineNo( const DWORD indwLineNo, char *outszBuffer, const DWORD indwBufferSize ) const=0;

	/*! @return current number of lines in the console
	*/
	virtual int GetLineCount() const=0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Retrieve a console variable by name
		@param sName variable name
		@param bCaseSensitive true=faster, false=much slower but allowes names with wrong case (use only where performce doesn't matter)
		@return a pointer to the ICVar interface, NULL if is not found
		@see ICVar
	*/
	virtual ICVar* GetCVar( const char *name, const bool bCaseSensitive=true )=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! legacy function */
  virtual CXFont *GetFont()=0; 
	/*! legacy function */
  virtual void Help(const char *command = NULL)=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Read a value from a configuration file (.ini) and return the value
		@param szVarName variable name
		@param szFileName source configuration file
		@param def_val default value (if the variable is not found into the file)
		@return the variable value
	*/
  virtual char *GetVariable( const char * szVarName, const char * szFileName, const char * def_val )=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Read a value from a configuration file (.ini) and return the value
		@param szVarName variable name
		@param szFileName source configuration file
		@param def_val default value (if the variable is not found into the file)
		@return the variable value
	*/
  virtual float GetVariable( const char * szVarName, const char * szFileName, float def_val )=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Print a string in the console and go to the new line
		@param s the string to print
	*/
	virtual void PrintLine(const char *s)=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Append a string in the last console line
		@param s the string to print
	*/
	virtual void PrintLinePlus(const char *s)=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Retreive the status of the console (active/not active)
		@return the variable value(true = active/false = not active)
	*/
	virtual bool GetStatus()=0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Clear the console text
	*/
	virtual void	Clear()=0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Update the console
	*/
	virtual void	Update()=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Draw the console
	*/
	virtual void	Draw()=0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Add a Console command
		@param sName name of the command (ex "connect")
		@param sScriptFunc script buffer the contain the command implementation
			EG "Game.Connect(%1)" the symbol "%1" will be replaced with the command parameter 1
			writing in the console "connect 127.0.0.1" will invoke Game.Connect("127.0.0.1")
		@param indwFlags bitfield consist of VF_ flags (e.g. VF_CHEAT)
	*/
	virtual void AddCommand(const char *sName, const char *sScriptFunc, const DWORD indwFlags=0, const char *help = "") = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Execute a string in the console
		@param command console command
	*/
	virtual void ExecuteString(const char *command,bool bNeedSlash=false,bool bIgnoreDevMode=false) = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Print a message into the log and abort the execution of the application
		@param message error string to print in the log
	*/
	virtual void Exit(const char *command,...) = 0;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Return true if the console is opened
		@return the variable value(true = opened/false = closed)
	*/
	virtual bool IsOpened() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Auto completion.
	//////////////////////////////////////////////////////////////////////////
	virtual int		GetNumVars() = 0;
	virtual void	GetSortedVars( const char **pszArray,size_t numItems ) = 0;
	virtual const char*	AutoComplete( const char* substr ) = 0;
	virtual const char*	AutoCompletePrev( const char* substr ) = 0;
	virtual const char *ProcessCompletion( const char *szInputBuffer ) = 0;
	//! 
	virtual void ResetAutoCompletion()=0;
	
	virtual void DumpCommandsVars(char *prefix) = 0;

	//! Calculation of the memory used by the whole console system
	virtual void GetMemoryUsage (struct ICrySizer* pSizer) = 0;

	//! Function related to progress bar
	virtual void ResetProgressBar(int nProgressRange) = 0;
	//! Function related to progress bar
	virtual void TickProgressBar() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Dump all key bindings to a callback-interface
	@param Callback callback-interface which needs to be called for each element
	*/
	virtual void DumpKeyBinds(IKeyBindDumpSink *pCallback )=0;
	virtual const char* FindKeyBind( const char *sCmd ) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Console variable sink.
	//////////////////////////////////////////////////////////////////////////
	//! Adds a new console variables sink callback.
	virtual void AddConsoleVarSink( IConsoleVarSink *pSink )=0;
	//! Removes a console variables sink callback.
	virtual void RemoveConsoleVarSink( IConsoleVarSink *pSink )=0;

	//////////////////////////////////////////////////////////////////////////
	// History
	//////////////////////////////////////////////////////////////////////////

	//! \param bUpOrDown true=after pressed "up", false=after pressed "down"
	//! \return 0 if there is no history line or pointer to the null terminated history line
	virtual const char* GetHistoryElement( const bool bUpOrDown )=0;
	//! \param szCommand must not be 0
	virtual void AddCommandToHistory( const char *szCommand )=0;
};

struct IConsole : public IBaseConsole
{
	virtual void AddCommand(const char* sCommand, ConsoleCommandFunc func, int nFlags = 0, const char* sHelp = NULL, bool bIsManagedExternally = false) = 0;
	virtual void AddCommand(const char *sName, const char *sScriptFunc, const DWORD indwFlags=0, const char *help = "") = 0;
	//! Sets the value of a CVar as loaded from a config
	//! Will defer setting of the value until the CVar is registered if it hasn't been already
	virtual void LoadConfigVar(const char* sVariable, const char* sValue) = 0;
	//! Executes a command with optional arguments
	//! Will defer setting of the value until the command is registered if it hasn't been already
	virtual void LoadConfigCommand(const char* szCommand, const char* szArguments = nullptr) = 0;
	//! Removes a console command which was previously registered with AddCommand.
	//! \param sCommand Command name.
	//! \par Example
	//! \include CrySystem/Examples/ConsoleCommand.cpp
	virtual void RemoveCommand(const char* sName) = 0;
	//! \param szLine Must not be 0.
	virtual void SetInputLine(const char* szLine) = 0;

	virtual ICVar* RegisterInternal(const char* name, float* src, float defaultvalue, int flags = 0, const char* help = "", ConsoleVarFunc pChangeFunc = 0, bool allowModify = true) = 0;
	virtual ICVar* RegisterInternal(const char* name, int* src, int defaultvalue, int flags = 0, const char* help = "", ConsoleVarFunc pChangeFunc = 0, bool allowModify = true) = 0;
	virtual ICVar* RegisterInternal(const char* name, const char** src, const char* defaultvalue, int flags = 0, const char* help = "", ConsoleVarFunc pChangeFunc = 0, bool allowModify = true) = 0;
	virtual ICVar* RegisterInternal(ICVar* pVar) = 0;

	virtual void        RegisterAutoComplete(const char* sVarOrCommand, IConsoleArgumentAutoComplete* pArgAutoComplete) = 0;
	virtual void        UnRegisterAutoComplete(const char* sVarOrCommand) = 0;
};

//! \cond INTERNAL
//! This interface for the remote console.
struct IRemoteConsoleListener
{
	virtual ~IRemoteConsoleListener()
	{
	}

	virtual void OnConsoleCommand(const char* cmd)
	{
	}
	virtual void OnGameplayCommand(const char* cmd)
	{
	}
};

struct IRemoteConsole
{
	virtual ~IRemoteConsole()
	{
	}

	virtual void RegisterConsoleVariables()	  = 0;
	virtual void UnregisterConsoleVariables() = 0;

	virtual void Start()		   = 0;
	virtual void Stop()			   = 0;
	virtual bool IsStarted() const = 0;

	virtual void AddLogMessage(const char* log) = 0;
	virtual void AddLogWarning(const char* log) = 0;
	virtual void AddLogError(const char* log)	= 0;

	virtual void Update() = 0;

	virtual void RegisterListener(IRemoteConsoleListener* pListener, const char* name) = 0;
	virtual void UnregisterListener(IRemoteConsoleListener* pListener)				   = 0;
};
//! \endcond

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! this interface is the 1:1 "C++ representation"
//! of a console variable.
//! NOTE: a console variable is accessible in C++ trough
//! this interface and in all scripts as global variable
//! (with the same name of the variable in the console)
struct ICVarBase
{
	virtual ~ICVarBase()
	{
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! delete the variable
    NOTE: the variable will automatically unregister itself from the console
  */
	virtual void Release() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Return the integer value of the variable
      @return the value
    */
	virtual int GetIVal() const = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Return the float value of the variable
      @return the value
    */
	virtual float GetFVal() const = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Return the string value of the variable
      @return the value
    */
	virtual const char* GetString() const = 0;
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
	virtual int GetFlags() const = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Set the variable's flags
    */
	virtual int SetFlags(int flags) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! return the primary variable's type
      @return the primary variable's type
    */
	virtual int GetType() const = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! return the variable's name
      @return the variable's name
    */
	virtual const char* GetName() const = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! return the variable's name
      @return the variable's name
    */
	virtual const char* GetHelp() const = 0;
};

struct ICVar : public ICVarBase
{
	//! Indicates whether the console owns the CVar and should delete it
	virtual bool IsOwnedByConsole() const = 0;
	//! \return true if the variable may be modified in config files.
	virtual bool  IsConstCVar() const = 0;
	//! Sets the value of the variable regardless of type
	//! \param s String representation of the value.
	virtual void SetFromString(const char* szValue) = 0;


	inline uint64 AddOnChange(ConsoleVarFunc pCallback)
	{
		return AddOnChange([this, pCallback](void) { pCallback(this); });
	}

	//! Adds a new on change callback function to the cvar.
	//! \return an ID associated with the function that is never invalidated
	virtual uint64 AddOnChange(std::function<void()> callback) = 0;
	//! Removes an on change functor.
	//! \return true if removal was successful.
	virtual bool RemoveOnChangeFunctor(const uint64 id) = 0;
};
