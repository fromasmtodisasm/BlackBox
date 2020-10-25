#pragma once
#include "NullImplementation/NullFont.hpp"
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Core/StlUtils.hpp>

#include <list>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#ifndef stricmp
#	define stricmp _stricmp
#endif // !stricmp

inline string& TrimLeft(string& str)
{
	for (size_t i = 0; i < str.length() && isspace(str[i]); i++)
	{
		str.erase(i);
	}
	return str;
}

inline string& TrimRight(string& str)
{
	for (size_t i = str.length(); i > 0 && isspace(str[i]); i++)
	{
		str.erase(i);
	}
	return str;
}

inline string& TrimLeft(string& str, const string& pattern)
{
	for (size_t i = 0; i < str.length() && strchr(pattern.data(), str[i]); i++)
	{
		str.erase(i);
	}
	return str;
}

inline string& TrimRight(string& str, const string& pattern)
{
	for (size_t i = str.length() - 1; i >= 0 && strchr(pattern.data(), str[i]); i++)
	{
		str.erase(i);
	}
	return str;
}

inline string& Trim(string& str)
{
	return TrimRight(TrimLeft(str));
}

inline string& Trim(string& str, const string& pattern)
{
	if (str.length() > 0)
		return TrimRight(TrimLeft(str, pattern), pattern);
	return str;
}


struct IFont;
struct IIpnut;
struct INetwork;
struct IRenderer;
struct ITexture;

class CSystem;
#define MAX_HISTORY_ENTRIES 50
#define LINE_BORDER 10
enum ScrollDir
{
	sdDOWN,
	sdUP,
	sdNONE
};

struct CConsoleCommand
{
	string             m_sName;    // Console command name
	string             m_sCommand; // lua code that is executed when this command is invoked
	string             m_sHelp;    // optional help string - can be shown in the console with "<commandname> ?"
	int                m_nFlags;   // bitmask consist of flag starting with VF_ e.g. VF_CHEAT
	ConsoleCommandFunc m_func;     // Pointer to console command.
	bool               m_isManagedExternally; // true if console command is added from C# and the notification of console commands will be through C# class method invocation via mono

	//////////////////////////////////////////////////////////////////////////
	CConsoleCommand()
		: m_nFlags(0)
		, m_func(0)
		, m_isManagedExternally(false)
	{}
	size_t sizeofThis() const { return sizeof(*this) + m_sName.capacity() + 1 + m_sCommand.capacity() + 1; }
	void   GetMemoryUsage(class ICrySizer* pSizer) const
	{
		#if 0
		pSizer->AddObject(m_sName);
		pSizer->AddObject(m_sCommand);
		pSizer->AddObject(m_sHelp);
		#endif
	}
};

struct CConsoleCommandArgs : public IConsoleCmdArgs
{
	CConsoleCommandArgs(string& line, std::vector<string>& args) : m_line(line), m_args(args) {}
	virtual int         GetArgCount() const { return m_args.size(); }
	// Get argument by index, nIndex must be in 0 <= nIndex < GetArgCount()
	virtual const char* GetArg(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < GetArgCount());
		if (!(nIndex >= 0 && nIndex < GetArgCount()))
			return NULL;
		return m_args[nIndex].c_str();
	}
	virtual const char* GetCommandLine() const
	{
		return m_line.c_str();
	}

private:
	string&              m_line;
	std::vector<string>& m_args;
};

struct string_nocase_lt
{
	bool operator()(const char* s1, const char* s2) const
	{
		return stricmp(s1, s2) < 0;
	}
};


#if 0
class CBaseVariable : public ICVar
{
  public:
	CBaseVariable(IConsole *pConsole, const char* name, int flags)
		: m_pConsole(pConsole), m_Name(name), m_flags(flags)
	{
		CBaseVariable::SetFlags(VF_NULL);
	}
	~CBaseVariable()
	{
		//gEnv->pLog->Log("Unregistering %s variable", m_Name.c_str());
	}
	virtual void Release() override
	{
		gEnv->pConsole->UnregisterVariable(m_Name.c_str());
	}

	virtual const char* GetName() override
	{
		return m_Name.c_str();
	}

	virtual void ClearFlags(int flags) override
	{
		m_flags &= ~flags;
	}

	virtual int GetFlags() override
	{
		return m_flags;
	}

	virtual int SetFlags(int flags) override
	{
		m_flags |= flags;
		return m_flags;
	}

	bool IsOwnedByConsole() const  { return true; }
	virtual bool IsConstCVar() const override { return (m_flags & VF_CONST_CVAR) != 0; }

	#if 0
	virtual uint64 AddOnChange(std::function<void()> changeFunctor) override
	{
		uint64 functorId = m_changeFunctorIdTotal;
		m_onChangeCallbacks.emplace_back(SCallback{ std::move(changeFunctor), functorId });
		++m_changeFunctorIdTotal;
		return functorId;
	}
	#endif
	string m_Name;
	int m_flags;
	struct SCallback
	{
		std::function<void()> function;
		uint64                id;
	};

	std::vector<std::function<void()>> m_onChangeCallbacks;
	uint64                 m_changeFunctorIdTotal = 0;
	IConsole*              m_pConsole;              // used for the callback OnBeforeVarChange()
};

class CNullCVar : public ICVar
{
  public:
	// Inherited via ICVar
	virtual void Release() override;
	virtual int GetIVal() override;
	virtual float GetFVal() override;
	virtual char* GetString() override;
	virtual void Set(const char* s) override;
	virtual void ForceSet(const char* s) override;
	virtual void Set(float f) override;
	virtual void Set(int i) override;
	virtual void Refresh() override;
	virtual void ClearFlags(int flags) override;
	virtual int GetFlags() override;
	virtual int SetFlags(int flags) override;
	virtual int GetType() override;
	virtual const char* GetName() override;
	virtual const char* GetHelp() override;

	operator bool() const
	{
		return false;
	}
};

class CCVar : public CBaseVariable
{
  public:
	CCVar(IConsole *pConsole, const char* name, int value, int flags, const char* help)
		: CBaseVariable(pConsole, name, flags), value(value), type(CVAR_INT), help(help)
	{
	}
	CCVar(IConsole *pConsole, const char* name, const char* value, int flags, const char* help)
		: CBaseVariable(pConsole, name, flags), value(value), type(CVAR_STRING), help(help)
	{
	}
	CCVar(IConsole *pConsole, const char* name, float value, int flags, const char* help)
		: CBaseVariable(pConsole, name, flags), value(value), type(CVAR_FLOAT), help(help)
	{
	}
	CCVar()
		: CBaseVariable(nullptr, "", 0), value(0.0f), type(CVAR_STRING), help(nullptr)
	{
	}

	~CCVar()
	{
	}

	// Inherited via ICVar
	virtual int GetIVal() override;
	virtual float GetFVal() override;
	virtual char* GetString() override;
	virtual void Set(const char* s) override;
	virtual void ForceSet(const char* s) override;
	virtual void Set(float f) override;
	virtual void Set(int i) override;
	virtual void Refresh() override;
	virtual int GetType() override;
	virtual const char* GetHelp() override;

  private:
	union Value
	{
		int i;
		char* s;
		float f;
		Value(int i)
			: i(i)
		{
		}
		Value(const char* s)
			: s(const_cast<char*>(s))
		{
		}
		Value(float f)
			: f(f)
		{
		}
	} value;
	int type;
	const char* help;
};

class CCVarRef : public CBaseVariable
{
  public:
	CCVarRef(IConsole *pConsole, const char* name, int* value, int flags, const char* help)
		: CBaseVariable(pConsole, name, flags), value(value), type(CVAR_INT), help(help)
	{

	}
	CCVarRef(IConsole *pConsole, const char* name, const char** value, int flags, const char* help)
		: CBaseVariable(pConsole, name, flags), value(value), type(CVAR_STRING), help(help)
	{
		//*value = strdup(defaultValue);
	}
	CCVarRef(IConsole *pConsole, const char* name, float* value, int flags, const char* help)
		: CBaseVariable(pConsole, name, flags), value(value), type(CVAR_FLOAT), help(help)
	{
		//*value = defaultValue;
	}
	//CCVarRef() : name(""), value(nullptr), type(CVAR_STRING), help(nullptr) {}

	~CCVarRef()
	{
	}

	// Inherited via ICVar
	virtual int GetIVal() override;
	virtual float GetFVal() override;
	virtual char* GetString() override;
	virtual void Set(const char* s) override;
	virtual void ForceSet(const char* s) override;
	virtual void Set(float f) override;
	virtual void Set(int i) override;
	virtual void Refresh() override;
	virtual int GetType() override;
	virtual const char* GetHelp() override;

  private:
	union Value
	{
		int* i;
		char** s;
		float* f;
		Value(int* i)
			: i(i)
		{
		}
		Value(const char** s)
			: s(const_cast<char**>(s))
		{
		}
		Value(float* f)
			: f(f)
		{
		}
	} value;
	int type;
	const char* help;
};
#endif

struct CommandInfo
{
	enum class Type
	{
		FUNC,
		INTERFACE,
		SCRIPT,
		UNKNOWN
	};
	struct SScript
	{
		const char* code;
		int arg_cnt;
		int* args_pos;
	};
	union
	{
		ConsoleCommandFunc Func = nullptr;
		IConsoleCommand* Command;
		SScript Script;
	};
	std::multimap<int, std::string> argsCompletion;
	std::string help;
	Type type = Type::UNKNOWN;
};

struct Text
{
	std::string data;
	glm::vec3 color;
	int x, y;
	float scale;
	Text(std::string data, glm::vec3 color, float scale)
		: data(data), color(color), scale(scale), x(0), y(0)
	{
	}
};

struct Cursor : Text
{
	Cursor()
		: Text(std::string("_"), glm::vec3(1, 0, 0), 1.f)
	{
	}

	bool needDraw	 = true;
	float blinking	 = 0.0f;
	ICVar* blinkTime = nullptr;
};

typedef std::vector<Text> CommandLine;

struct ConsolePrompt
{
	std::string user = gEnv->pSystem->GetUserName();
	std::string pc	 = "HackMan";
	std::string env	 = "BlackBox";
	std::string cd	 = "~"; //current directory
	Vec3 color		 = glm::vec3(0.0, 1.0, 0.0);

	ConsolePrompt(
		std::string user = gEnv->pSystem->GetUserName(),
		std::string pc	 = "HackMan",
		std::string env	 = "BlackBox",
		std::string cd	 = "~", //current directory
		Vec3 color		 = Vec3(0.0, 1.0, 0.0))
	{
		this->user	= user;
		this->pc	= pc;
		this->env	= env;
		this->cd	= cd;
		this->color = color;
	}
	CommandLine get();
};

enum EInputFunctions
{
	EAutoComplete,
	EGotoBeginLine,
	EGotoEndLine,
	ESubmit,
	ECopy,
	EPaste,
	EClearInputLine,
	ENextHistoryElement,
	EPrevHistoryElement,
	EMoveCursorToPrevChar,
	EMoveCursorToNextChar,
	EMoveCursorToPrevWord,
	EMoveCursorToNextWord,
	EDeleteRightChar,
	EDeleteLeftChar,
	EDeleteAllAfterCursor,
	EDeleteAllBeforeCursor,
	EDeleteWordAfterCursor,
	EClear,
	EScrolUp,
	EScrolDown,
};
struct cmpInputEvents
	{
		bool operator()(const SInputEvent& a, const SInputEvent& b) const
		{
			auto am = a.modifiers & (~eMM_NumLock);
			auto bm = b.modifiers & (~eMM_NumLock);
			if (a.keyId != b.keyId)
				return (a.keyId < b.keyId);
			if (am != bm)
				return (am < bm);
			return (a.state < b.state);
		}
	};

class CXConsole : public IConsole
	, public IRemoteConsoleListener
	, public IInputEventListener
	, public IConsoleVarSink
{
	friend class SetCommand;
	friend class GetCommand;
	friend class DumpCommand;

  public:
	CXConsole(CSystem& system);
	~CXConsole();

	void PreProjectSystemInit();
	void PostRendererInit();

	void SetStatus(bool bActive) { m_bConsoleActive = bActive; }
	bool GetStatus() const       { return m_bConsoleActive; }
	
	void FreeRenderResources();
	void Copy();
	void Paste();

	bool Init(ISystem* pSystem);

	// Inherited via IConsole
	virtual void Release() override;
	virtual ICVar* CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help = "") override;
	virtual ICVar* CreateVariable(const char* sName, int iValue, int nFlags, const char* help = "") override;
	virtual ICVar* CreateVariable(const char* sName, float fValue, int nFlags, const char* help = "") override;
	virtual void UnregisterVariable(const char* sVarName, bool bDelete = false) override;
	virtual void SetScrollMax(int value) override;
	virtual void AddOutputPrintSink(IOutputPrintSink* inpSink) override;
	virtual void RemoveOutputPrintSink(IOutputPrintSink* inpSink) override;
	virtual void ShowConsole(bool show) override;
	virtual int Register(const char* name, void* src, float defaultvalue, int flags, int type, const char* help = "") override;
	virtual float Register(const char* name, float* src, float defaultvalue, int flags = 0, const char* help = "") override;
	virtual int Register(const char* name, int* src, float defaultvalue, int flags = 0, const char* help = "") override;
	virtual void DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter = 0) override;
	virtual void CreateKeyBind(const char* sCmd, const char* sRes, bool bExecute) override;
	virtual void SetImage(ITexPic* pImage, bool bDeleteCurrent) override;
	virtual ITexPic* GetImage() override;
	virtual void StaticBackground(bool bStatic) override;
	virtual void SetLoadingImage(const char* szFilename) override;
	virtual bool GetLineNo(const DWORD indwLineNo, char* outszBuffer, const DWORD indwBufferSize) const override;
	virtual int GetLineCount() const override;
	virtual ICVar* GetCVar(const char* name, const bool bCaseSensitive = true) override;
	virtual CXFont* GetFont() override;
	virtual void Help(const char* command = NULL) override;
	virtual char* GetVariable(const char* szVarName, const char* szFileName, const char* def_val) override;
	virtual float GetVariable(const char* szVarName, const char* szFileName, float def_val) override;
	virtual void PrintLine(const char* s) override;
	virtual void PrintLinePlus(const char* s) override;
	virtual bool GetStatus() override;
	virtual void Clear() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void AddCommand(const char* sName, const char* sScriptFunc, const DWORD indwFlags = 0, const char* help = "") override;
	virtual void ExecuteString(const char* command, bool bNeedSlash = false, bool bIgnoreDevMode = false) override;
	virtual void Exit(const char* command, ...) override;
	virtual bool IsOpened() override;
	virtual int GetNumVars() override;
	virtual void GetSortedVars(const char** pszArray, size_t numItems) override;
	virtual size_t                 GetSortedVars(const char** pszArray, size_t numItems, const char* szPrefix/* = 0*/, int nListTypes/* = 0*/) const;
	virtual const char* AutoComplete(const char* substr) override;
	virtual const char* AutoCompletePrev(const char* substr) override;
	virtual const char* ProcessCompletion(const char* szInputBuffer) override;
	virtual void ResetAutoCompletion() override;
	virtual void DumpCommandsVars(char* prefix) override;
	virtual void GetMemoryUsage(ICrySizer* pSizer) override;
	virtual void ResetProgressBar(int nProgressRange) override;
	virtual void TickProgressBar() override;
	virtual void DumpKeyBinds(IKeyBindDumpSink* pCallback) override;
	virtual const char* FindKeyBind(const char* sCmd) override;
	virtual void AddConsoleVarSink(IConsoleVarSink* pSink) override;
	virtual void RemoveConsoleVarSink(IConsoleVarSink* pSink) override;
	virtual const char* GetHistoryElement(const bool bUpOrDown) override;
	virtual void AddCommandToHistory(const char* szCommand) override;

	// Inherited via IConsole
	virtual void LoadConfigVar(const char* sVariable, const char* sValue) override;
	virtual void LoadConfigCommand(const char* szCommand, const char* szArguments = nullptr) override;

	// Inherited via IInputEventListener
	virtual bool OnInputEvent(const SInputEvent& event) override;
	virtual bool OnInputEventUI(const SUnicodeEvent& event) override;

	virtual void                   FindVar(const char* substr);
	// interface IRemoteConsoleListener ------------------------------------------------------------------

	virtual void OnConsoleCommand(const char* cmd) override;
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
	virtual void OnAfterVarChange(ICVar* pVar) override;

  protected:
	
	bool ProcessInput(const SInputEvent& event);
	void DrawBuffer(int nScrollPos, const char* szEffect);
	void AddLine(const char* inputStr);
	void AddLinePlus(const char* inputStr);
	void AddInputChar(const uint32 c);
	void RemoveInputChar(bool bBackSpace);
	void ExecuteInputBuffer();
	void ExecuteCommand(CConsoleCommand& cmd, string& params, bool bIgnoreDevMode = false);
	void ScrollConsole();
	void ConsoleLogInputResponse(const char* szFormat, ...) PRINTF_PARAMS(2, 3);
	void ConsoleLogInput(const char* szFormat, ...) ;PRINTF_PARAMS(2, 3);
	void ConsoleWarning(const char* szFormat, ...); //PRINTF_PARAMS(2, 3);

	void DisplayHelp(const char* help, const char* name);
	void DisplayVarValue(ICVar* pVar);

	void SplitCommands(const char* line, std::list<string>& split);
	// bFromConsole: true=from console, false=from outside
	void ExecuteStringInternal(const char* command, const bool bFromConsole, const bool bSilentMode = false);
	void ExecuteDeferredCommands();

	static const char* GetFlagsString(const uint32 dwFlags);
	static void        CmdDumpAllAnticheatVars(IConsoleCmdArgs* pArgs);
	static void        CmdDumpLastHashedAnticheatVars(IConsoleCmdArgs* pArgs);


	virtual ICVar* RegisterInternal(const char* name, float* src, float defaultvalue, int flags = 0, const char* help = "", ConsoleVarFunc pChangeFunc = 0, bool allowModify = true);
	virtual ICVar* RegisterInternal(const char* name, int* src, int defaultvalue, int flags = 0, const char* help = "", ConsoleVarFunc pChangeFunc = 0, bool allowModify = true);
	virtual ICVar* RegisterInternal(const char* name, const char** src, const char* defaultvalue, int flags = 0, const char* help = "", ConsoleVarFunc pChangeFunc = 0, bool allowModify = true);
	virtual ICVar* RegisterInternal(ICVar* pVar) { RegisterVar(pVar->GetName(), pVar); return pVar; }

	void          SetProcessingGroup(bool isGroup) { m_bIsProcessingGroup = isGroup; }
	bool          GetIsProcessingGroup(void) const { return m_bIsProcessingGroup; }

	IFont* CXConsole::GetFont(const char* name, float w, float h);;

private: // ----------------------------------------------------------

	struct SConfigVar
	{
		string m_value;
		bool   m_partOfGroup;
		uint32 nCVarOrFlags;
	};

	struct SDeferredCommand
	{
		string command;
		bool   silentMode;

		SDeferredCommand(const string& command, bool silentMode)
			: command(command), silentMode(silentMode)
		{}
	};

	typedef std::deque<string>                                                                                              ConsoleBuffer;
	typedef std::unordered_map<string, CConsoleCommand>/*, stl::hash_strcmp<string>, stl::hash_strcmp<string>>*/               ConsoleCommandsMap;
	typedef std::unordered_map<string, string>/*, stl::hash_strcmp<string>, stl::hash_strcmp<string>>*/                        ConsoleBindsMap;
	#if 0
	typedef std::unordered_map<string, IConsoleArgumentAutoComplete*, stl::hash_strcmp<string>, stl::hash_strcmp<string>> ArgumentAutoCompleteMap;
	#endif
	typedef std::unordered_map<string, SConfigVar>/*, stl::hash_strcmp<string>, stl::hash_strcmp<string>>*/                    ConfigVars;
	typedef std::list<SDeferredCommand>                                                                                     TDeferredCommandList;
	typedef std::list<IConsoleVarSink*>                                                                                     ConsoleVarSinks;
	#if 0
	typedef CListenerSet<IManagedConsoleCommandListener*>                                                                   TManagedConsoleCommandListener;
	#endif
	typedef std::unordered_map<string, ICVar*>/*, stl::hash_strcmp<string>, stl::hash_strcmp<string>>*/                        ConsoleVariablesMap;
	typedef std::vector<std::pair<const char*, ICVar*>>                                                                     ConsoleVariablesVector;

	
	void        AddCheckedCVar(ConsoleVariablesVector& vector, const ConsoleVariablesVector::value_type& value);
	void        RemoveCheckedCVar(ConsoleVariablesVector& vector, const ConsoleVariablesVector::value_type& value);
	//static void AddCVarsToHash(ConsoleVariablesVector::const_iterator begin, ConsoleVariablesVector::const_iterator end, CCrc32& runningNameCrc32, CCrc32& runningNameValueCrc32);
	static bool CVarNameLess(const std::pair<const char*, ICVar*>& lhs, const std::pair<const char*, ICVar*>& rhs);


protected:
	void UnregisterVariableImpl(const ConsoleVariablesMap::iterator& iter);
	void RegisterVar(const string& name, ICVar* pCVar, ConsoleVarFunc pChangeFunc = 0);

private:
	// --------------------------------------------------------------------------------

	ConsoleBuffer                  m_dqConsoleBuffer;
	ConsoleBuffer                  m_dqHistory;

	bool                           m_bStaticBackground;
	int                            m_nLoadingBackTexID;
	int                            m_nWhiteTexID;
	int                            m_nProgress;
	int                            m_nProgressRange;

	string                         m_sInputBuffer;
	string                         m_sReturnString;

	string                         m_sPrevTab;
	int                            m_nTabCount;

	ConsoleCommandsMap             m_mapCommands;             //
	ConsoleBindsMap                m_mapBinds;                //
	ConsoleVariablesMap            m_mapVariables;            //
	ConsoleVariablesVector         m_randomCheckedVariables;
	ConsoleVariablesVector         m_alwaysCheckedVariables;
	std::vector<IOutputPrintSink*> m_OutputSinks;             // objects in this vector are not released
	#if 0
	TManagedConsoleCommandListener m_managedConsoleCommandListeners;
	#endif

	TDeferredCommandList           m_deferredCommands;        // A fifo of deferred commands
	bool                           m_deferredExecution;       // True when deferred commands are processed
	int                            m_waitFrames;              // A counter which is used by wait_frames command
	#if WAIT_SECONDS
	CTimeValue                     m_waitSeconds;             // An absolute timestamp which is used by wait_seconds command
	#endif
	int                            m_blockCounter;            // This counter is incremented whenever a blocker command (VF_BLOCKFRAME) is executed.

	#if 0
	ArgumentAutoCompleteMap        m_mapArgumentAutoComplete;
	#endif

	ConsoleVarSinks                m_consoleVarSinks;

	ConfigVars                     m_configVars;              // temporary data of cvars that haven't been created yet
	std::multimap<string, string>  m_configCommands;          // temporary data of commands that haven't been created yet

	int                            m_nScrollPos;
	int                            m_nTempScrollMax;          // for currently opened console, reset to m_nScrollMax
	int                            m_nScrollMax;              //
	int                            m_nScrollLine;
	int                            m_nHistoryPos;
	size_t                         m_nCursorPos;                // x position in characters
	ITexture*                      m_pImage;

	float                          m_fRepeatTimer;            // relative, next repeat even in .. decreses over time, repeats when 0, only valid if m_nRepeatEvent.keyId != eKI_Unknown
	SInputEvent                    m_nRepeatEvent;            // event that will be repeated

	float                          m_fCursorBlinkTimer;       // relative, increases over time,
	bool                           m_bDrawCursor;

	ScrollDir                      m_sdScrollDir;

	bool                           m_bConsoleActive;
	bool                           m_bActivationKeyEnable;
	bool                           m_bIsProcessingGroup;

	size_t                         m_nCheatHashRangeFirst;
	size_t                         m_nCheatHashRangeLast;
	bool                           m_bCheatHashDirty;
	uint64                         m_nCheatHash;

	CSystem&                       m_system;
	IFont*                        m_pFont;
	IRenderer*                     m_pRenderer;
	IInput*                        m_pInput;
	ITimer*                        m_pTimer;
	INetwork*                      m_pNetwork;                // EvenBalance - M. Quinn

	ICVar*                         m_pSysDeactivateConsole;

	ELoadConfigurationType         m_currentLoadConfigType;

	bool                           m_readOnly;

	static int                     con_display_last_messages;
	static int                     con_line_buffer_size;
	static float                   con_font_size;
	static int                     con_showonload;
	static int                     con_debug;
	static int                     con_restricted;

	friend void Command_SetWaitSeconds(IConsoleCmdArgs* Cmd);
	friend void Command_SetWaitFrames(IConsoleCmdArgs* Cmd);
#if ALLOW_AUDIT_CVARS
	friend void Command_AuditCVars(IConsoleCmdArgs* pArg);
#endif // ALLOW_AUDIT_CVARS
	friend void Command_DumpCommandsVars(IConsoleCmdArgs* Cmd);
	friend void Command_DumpVars(IConsoleCmdArgs* Cmd);

	// Inherited via IConsole
	virtual void RemoveCommand(const char* sName) override;

	// Inherited via IConsole
	virtual void SetInputLine(const char* szLine) override;

	// Inherited via IConsoleVarSink
	virtual void OnVarUnregister(ICVar* pVar) override;
};
