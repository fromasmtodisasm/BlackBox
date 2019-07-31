#pragma once

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

struct IEditCommand;
struct CommandDesc;
struct ICVar;
class ITexture;

#define     CVAR_INT    1
#define     CVAR_FLOAT  2
#define     CVAR_STRING 3

struct IEditCommand
{
	virtual bool execute(CommandDesc& cd) = 0;
};

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

struct IConsole
{
	virtual bool Init() = 0;
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
	virtual void AddCommand(const char* sName, IEditCommand *command, const char* help = "") = 0;
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
