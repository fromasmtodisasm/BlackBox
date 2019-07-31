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


struct IEditCommand
{
	virtual bool execute(CommandDesc& cd) = 0;
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
	/*! Print a string in the console and go to the new line
		@param s the string to print
	*/
	virtual void PrintLine(const char* s, ...) = 0;

};

struct ICVar
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! Return the float value of the variable
		@return the value
	*/
	virtual float GetFVal() = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! set the float value of the variable
		@param s float representation the value
	*/
	virtual void Set(float f) = 0;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*! return the variable's name
		@return the variable's name
	*/
	virtual const char* GetName() = 0;
};
