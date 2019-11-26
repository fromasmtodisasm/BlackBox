#pragma once
#include <BlackBox/IConsole.hpp>
#include <BlackBox/IInput.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/Texture.hpp>

#include <map>
#include <set>
#include <list>
#include <queue>
#include <vector>
#include <string>

#ifndef stricmp
#define stricmp _stricmp
#endif // !stricmp

struct IFont;

struct cmpKeys {
  bool operator()(const std::string& a, const std::string& b) const {
    return stricmp(a.c_str(), b.c_str()) < 0;
  }
};

class CCVar : public ICVar
{
public:
  CCVar(const char* name, int value, char* help) : name(name), value(value), type(CVAR_INT), help(help) {}
  CCVar(const char* name, const char* value, char* help) : name(name), value(value), type(CVAR_STRING), help(help) {}
  CCVar(const char* name, float value, char* help) : name(name), value(value), type(CVAR_FLOAT), help(help) {}
  CCVar() : name(""), value(0.0f), type(CVAR_STRING), help(nullptr) {}

  ~CCVar()
  {
  }

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

private:
  const char* name;
  union Value {
    int i;
    char* s;
    float f;
    Value(int i) : i(i) {}
    Value(const char* s) : s(const_cast<char*>(s)) {}
    Value(float f) : f(f) {}
  }value;
  int type;
  char* help;
};

class CCVarRef : public ICVar
{
public:
  CCVarRef(const char* name, int* value, const char* help) : name(name), value(value), type(CVAR_INT), help(help) {}
  CCVarRef(const char* name, const char** value, const char* help) : name(name), value(value), type(CVAR_STRING), help(help) {}
  CCVarRef(const char* name, float* value, const char* help) : name(name), value(value), type(CVAR_FLOAT), help(help) {}
  //CCVarRef() : name(""), value(nullptr), type(CVAR_STRING), help(nullptr) {}

  ~CCVarRef()
  {
  }

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

private:
  const char* name;
  union Value {
    int* i;
    char** s;
    float* f;
    Value(int* i) : i(i) {}
    Value(const char** s) : s(const_cast<char**>(s)) {}
    Value(float* f) : f(f) {}
  }value;
  int type;
  const char* help;
};

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
    :
    data(data), color(color), scale(scale), x(0), y(0)
  {
  }
};

struct Cursor : Text
{
  Cursor() : Text(std::string("_"), glm::vec3(1, 0, 0), 1.f) {}
};

typedef std::vector<Text> CommandLine;

class CConsole : public IConsole, public IInputEventListener, public ICVarDumpSink
{
  friend class SetCommand;
  friend class GetCommand;
  friend class DumpCommand;
  const int MESSAGE_BUFFER_SIZE = 1024 * 16;
public:
  CConsole();
  ~CConsole();
  // Inherited via IConsole
  bool Init(ISystem* pSystem);
  virtual void ShowConsole(bool show) override;
  virtual void SetImage(ITexture* pTexture) override;
  virtual void Update() override;
  virtual void Draw() override;
  void Animate(float deltatime, IRenderer* render);
  void CalcMetrics(size_t& end);
  virtual void AddCommand(const char* sName, IConsoleCommand* command, const char* help = "") override;
  virtual void AddCommand(const char* sName, const char* sScriptFunc, const uint32_t indwFlags = 0, const char* help = "") override;
  virtual void ExecuteString(const char* command) override;
  virtual void ExecuteFile(const char* file) override;
  virtual bool OnInputEvent(const SInputEvent& event) override;
  virtual bool OnInputEventUI(const SUnicodeEvent& event);
  void getHistoryElement();
  void completeCommand(std::vector<std::wstring>& completion);
  void setBuffer();
  bool handleEnterText();
  virtual void AddArgumentCompletion(const char* cmd, const char* arg, int n) override;
  virtual void Clear() override;
  virtual void Help(const char* cmd) override;
  virtual void PrintLine(const char* format, ...) override;
  virtual bool IsOpened() override;

  virtual ICVar* CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help = "") override;

  virtual ICVar* CreateVariable(const char* sName, int iValue, int nFlags, const char* help = "") override;

  virtual ICVar* CreateVariable(const char* sName, float fValue, int nFlags, const char* help = "") override;

  virtual ICVar* GetCVar(const char* name, const bool bCaseSensitive = true) override;

  virtual void DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter = 0) override;
  virtual void OnElementFound(ICVar* pCVar) override;
  virtual void AddConsoleVarSink(IConsoleVarSink* pSink) override;
  virtual void RemoveConsoleVarSink(IConsoleVarSink* pSink) override;

  void CreateKeyBind(const char* key, const char* cmd);
  virtual void SetInputLine(const char* szLine) override;
  virtual void AddCommand(const char* sCommand, ConsoleCommandFunc func, int nFlags = 0, const char* help = NULL) override;
  virtual void AddWorkerCommand(IWorkerCommand* cmd) override;
  virtual void RemoveWorkerCommand(IWorkerCommand* cmd) override;
  virtual void UnregisterVariable(const char* sVarName, bool bDelete = false) override;
  virtual char* Register(const char* name, const char** src, const char* defaultvalue, int flags = 0, const char* help = "")  override;
  virtual float Register(const char* name, float* src, float defaultvalue, int flags = 0, const char* help = "") override;
  virtual int Register(const char* name, int* src, int defaultvalue, int flags = 0, const char* help = "") override;

  virtual void Exit(const char* command, ...) override;
  virtual char* GetVariable(const char* szVarName, const char* szFileName, const char* def_val) override;
  virtual float GetVariable(const char* szVarName, const char* szFileName, float def_val) override;
  virtual void PrintLinePlus(const char* s) override;

  virtual void SetScrollMax(int value) override;
  virtual void SetLoadingImage(const char* szFilename) override;
  virtual void ResetProgressBar(int nProgressRange) override;
  virtual void TickProgressBar() override;

private:
  void RegisterVar(ICVar* pCVar/*, ConsoleVarFunc pChangeFunc = 0*/);
private:
  CommandDesc parseCommand(std::wstring& command);
  bool handleCommand(std::wstring command);
  void AddInputChar(uint32_t ch);
  std::vector<std::wstring> autocomplete(std::wstring cmd);
  void doFile(std::ifstream& cfg);
  void fillCommandText();
  void setFont(IFont* font);
  CommandLine getPrompt();
  void printLine(size_t line);
  void printText(Text const& element, size_t line);
  void addToCommandBuffer(std::vector<std::wstring>& completion);
  void addText(std::wstring const& cmd);
  ;
  void Set(CommandDesc& cd);
  void SetInternal(ICVar* pVar, std::string& value, std::string& name);
  void Get(CommandDesc& cd);
  void GetInternal(ICVar* pVar, std::string& name);
  void Dump();
  void getBuffer();
  bool needShowCursor();
  void pageUp(bool isPgUp);
  void drawCursor();
  void moveCursor(bool left, bool wholeWord = false);

  void initBind();
  void ClearInputLine();
private:
  std::vector<IConsoleVarSink*> varSinks;
  std::map<std::wstring, CommandInfo> m_Commands;
  std::map<std::string, std::ifstream> scripts;
  bool is_input = false;
  bool input_trigered = false;
  std::wstring command;
  std::string command_text;
  IFont* m_Font = nullptr;
  bool isOpened = false;
  bool cmd_is_compete = false;
  ISystem* m_pSystem = nullptr;
  IRenderer* m_pRenderer = nullptr;
  IScriptSystem* m_pScriptSystem = nullptr;
  IInput* m_pInput = nullptr;
  ITexture* m_pBackGround;
  //ITexture* m_pBackGround = nullptr;

  float m_ScrollHeight = 0.0;

  // for animate console
  bool animate = false;
  float gravity = 1;
  float speed = 400;
  float curr_speed = 1300;
  float curr_height = 0;
  //
  size_t line_count = 0;
  size_t line_in_console = 0;
  size_t line_height = 18;
  size_t current_line = 0;
  size_t on_line = 0;
  bool page_up = false;
  bool page_dn = false;
  std::vector<CommandLine> cmd_buffer;
  std::vector<std::wstring> history;
  std::wstring last_command;
  std::string m_prompt;
  std::string message_buffer;

  std::string user = "root";
  std::string pc = "HackMan";
  std::string env = "BlackBox";
  std::string cd = "~"; //current directory
  //
  glm::vec3 promptColor = glm::vec3(0.0, 1.0, 0.0);
  glm::vec3 textColor = glm::vec3(1.0, 1.0, 0.0);

  std::map<std::string, ICVar*> m_mapVariables;

  ICVarDumpSink* m_pCVarDumpCallback = nullptr;
  ICVar* r_anim_speed;
  float time = 0.0f;
  float cursor_tick = 0.0, cursor_tack = 0.0;
  bool cursor_tick_tack = true;
  size_t history_line = 0;
  //
  float transparency = 0.5f;
  Cursor cursor;
  bool needDrawCursor = true;
  float blinking = 0.0f;
  //float blinkTime = 1.0;
  ICVar* blinkTime;
  std::map<EKeyId, std::wstring> m_keyBind;
  std::map<std::string, EKeyId, cmpKeys> m_str2key;

  std::set<IWorkerCommand*> m_workers;
  std::list<IWorkerCommand*> m_worker_to_delete;

  int lines = 0;

  // --------------------------------------------------------------------------------

  bool                           m_bStaticBackground = 1;
  int                            m_nLoadingBackTexID = -1;
  int                            m_nWhiteTexID = 0;
  int                            m_nProgress = 0;
  int                            m_nProgressRange = 0;

  int                            m_nScrollPos;
  int                            m_nTempScrollMax;          // for currently opened console, reset to m_nScrollMax
  int                            m_nScrollMax;              //
  int                            m_nScrollLine;
  int                            m_nHistoryPos;
  size_t                         m_nCursorPos;                // x position in characters

  // Inherited via IConsole
};