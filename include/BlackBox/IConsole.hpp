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
class ITexture;
struct CommandDesc;


struct IEditCommand
{
	virtual bool execute(CommandDesc& cd) = 0;
};

struct IConsole
{
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
	//! Add a argument completion for command
	virtual void AddArgumentCompletion(const char* cmd, const char* arg, int n) = 0;

	virtual void Clear() = 0;

};
