#include <BlackBox/System/Console.hpp>
#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Core/Utils.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/Renderer/TextureManager.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/System/IClipBoard.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <string>
#include <fstream>
#include <ctime>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <locale>
#include <functional>
#include "NullImplementation/NullFont.hpp"

bool isnumber(const char* s)
{
  auto p = s;
  while (*p)
  {
    if (!isdigit(*p++))
      return false;
  }
  return true;
}

class HelpCommand : public IConsoleCommand
{
public:
  HelpCommand()
  {
  }
  // Inherited via IEditCommand
  virtual bool execute(CommandDesc& cd) override
  {
    for (auto& cmd : cd.args)
      GetISystem()->GetIConsole()->Help(wstr_to_str(cmd).c_str());
    if (cd.args.size() == 0)
      GetISystem()->GetIConsole()->Help(nullptr);
    return true;
  }
};
class SetCommand : public IConsoleCommand
{
  CConsole* m_Console;
public:
  SetCommand(CConsole* console) : m_Console(console)
  {
  }
  // Inherited via IEditCommand
  virtual bool execute(CommandDesc& cd) override
  {
    m_Console->Set(cd);
    return true;
  }
};

class GetCommand : public IConsoleCommand
{
  CConsole* m_Console;
public:
  GetCommand(CConsole* console) : m_Console(console)
  {
  }
  // Inherited via IEditCommand
  virtual bool execute(CommandDesc& cd) override
  {
    m_Console->Get(cd);
    return true;
  }
};

class DumpCommand : public IConsoleCommand
{
  CConsole* m_Console;
public:
  DumpCommand(CConsole* console) : m_Console(console)
  {
  }
  // Inherited via IEditCommand
  virtual bool execute(CommandDesc& cd) override
  {
    m_Console->Dump();
    return true;
  }
};

void CConsole::SetImage(ITexture* pTexture)
{
  if (m_pBackGround != nullptr)
    delete m_pBackGround;
  m_pBackGround = pTexture;
}

void CConsole::Update()
{
	m_ScrollHeight = m_pRenderer->GetHeight() / 2.0f;
  for (const auto& worker : m_workers)
  {
    worker->OnUpdate();
  }

  //=====================
  for (auto& worker : m_worker_to_delete)
  {
    m_workers.erase(worker);
  }
  m_worker_to_delete.clear();
}

void CConsole::Draw()
{
  if (!isOpened) return;
  auto deltatime = GetISystem()->GetDeltaTime();
  if (!m_pRenderer)
  {
	  return;
  }
  auto render = m_pRenderer;
  //m_ScrollHeight = (float)(render->GetHeight()) / 2;
  Animate(deltatime, render);
  size_t end;
  auto prompt = getPrompt();
  time += GetISystem()->GetDeltaTime();
  if (!m_nProgressRange)
  {
    render->SetRenderTarget(0);
    render->DrawImage(0, 0, (float)render->GetWidth(), m_ScrollHeight, m_pBackGround ? m_pBackGround->getId() : 0, time * r_anim_speed->GetFVal(), 0, 0, 0, 1, 1, 1, transparency);
    CalcMetrics(end);
    m_Font->SetXPos(0);
    m_Font->SetYPos(16);
    for (on_line = current_line; on_line < end; on_line++)
    {
      printLine(on_line);
    }
    for (auto& element : prompt)
    {
      printText(element, line_count - 1);
    }
    //auto cursor = needShowCursor() ? "*" : " ";
    //command_text.replace(command_text.size() - 1, 1, 1, cursor);
    //command_text[command.length()] = cursor;

    //printText(Text(std::string("cursor:<" + std::string(cursor) + ">\n"), textColor, 1.0f), 0);
    printText(Text(std::string("\n#"), glm::vec3(1.0, 0.3, 0.5), 1.0), 0);
    printText(Text(std::string(m_CommandA), textColor, 1.0f), 0);
    drawCursor();
  }
  else
  {
    // draw progress bar
    //render->SetState(GS_BLSRC_SRCALPHA | GS_BLDST_ONEMINUSSRCALPHA | GS_NODEPTHTEST);
    render->DrawImage(0.0, 0.0, 800.0f, 600.0f, m_nLoadingBackTexID, 0.0f, 0.0f, 0.0f, 0, 0, 0, 0, 1.0);
  }
  /*m_Font->RenderText(
    command_text + "\n",
    m_Font->GetXPos(), height / 2 - line_count * line_height - line_height, 1.0f, textColor);
  */
}

void CConsole::Animate(float deltatime, IRenderer* render)
{
  m_AnimationParams.animate = false;
  if (m_AnimationParams.animate)
  {
    m_AnimationParams.curr_height += m_AnimationParams.curr_speed * deltatime;
    if (m_AnimationParams.curr_height >= m_ScrollHeight)
    {
      m_ScrollHeight = static_cast<float>(render->GetHeight());
      m_AnimationParams.animate = false;
      m_AnimationParams.curr_speed = m_AnimationParams.speed;
      m_AnimationParams.curr_height = 0.0f;
    }
    else
    {
      m_ScrollHeight = m_AnimationParams.curr_height;
      m_AnimationParams.curr_speed -= m_AnimationParams.gravity * deltatime;
    }
  }
}

void CConsole::CalcMetrics(size_t& end)
{
  constexpr int MAGIC = 1;
  line_in_console = (int)((m_ScrollHeight)) / (int)line_height;
  auto num_all_lines = m_CmdBuffer.size();
  if (line_in_console > num_all_lines)
  {
    current_line = 0;
    line_count = num_all_lines;
    end = num_all_lines;
  }
  else
  {
    current_line = num_all_lines - line_in_console + MAGIC;
    if (/*page_up && */current_line > 0)
      current_line++;
    else if (/*page_dn && */current_line < m_CmdBuffer.size() - line_in_console)
    {
      current_line--;
    }
    line_count = line_in_console - MAGIC;
    end = num_all_lines;
  }
  //page_up = false;
  //page_dn = false;
}

void CConsole::AddCommand(const char* sName, IConsoleCommand* command, const char* help)
{
  CommandInfo cmdInfo;
  cmdInfo.Command = command;
  if (help) cmdInfo.help = help;
  cmdInfo.type = CommandInfo::Type::INTERFACE;
  m_Commands[str_to_wstr(std::string(sName))] = cmdInfo;
}

void CConsole::ExecuteString(const char* command)
{
  handleCommand(str_to_wstr(std::string(command)));
}

bool CConsole::OnInputEvent(const SInputEvent& event)
{
  bool keyPressed = event.state == eIS_Pressed;
  bool control = event.modifiers & eMM_Ctrl;
  if (!isOpened)
  {
		const char* cmd = 0;

		if (event.modifiers == 0)
		{
			// fast
			cmd = FindKeyBind(event.keyName.c_str());
		}
		else
		{
			// slower
			char szCombinedName[40];
			int iLen = 0;

			if (event.modifiers & eMM_Ctrl)
			{ strcpy(szCombinedName, "ctrl_"); iLen += 5; }
			if (event.modifiers & eMM_Shift)
			{ strcpy(&szCombinedName[iLen], "shift_"); iLen += 6; }
			if (event.modifiers & eMM_Alt)
			{ strcpy(&szCombinedName[iLen], "alt_");  iLen += 4; }
			if (event.modifiers & eMM_Win)
			{ strcpy(&szCombinedName[iLen], "win_");  iLen += 4; }

			strcpy(&szCombinedName[iLen], event.keyName.c_str());

			cmd = FindKeyBind(szCombinedName);
		}

		if (cmd)
		{
			SetInputLine("");
			//ExecuteStringInternal(cmd, true);    // keybinds are treated as they would come from console
      return handleCommand(str_to_wstr(cmd).data());
		}
    if (keyPressed)
    {
      if (control || event.deviceType == eIDT_Gamepad)
      {
        auto it = m_keyBind.find(event.keyId);
        if (it != m_keyBind.end())
        {
          return handleCommand(it->second);
        }
      }
    }
    return false;
  }
  std::vector<std::wstring> completion;
  //m_World->getActiveScene()->setPostProcessor(postProcessors[4]);

  if (cmd_is_compete)
  {
    //SetInputLine("");
    ClearInputLine();
  }
  cmd_is_compete = false;

	if (event.keyName != "commit")
  {
    if (auto result = m_InputBindings.find(event); result != m_InputBindings.end())
    {
      switch (result->second)
      {
      case EAutoComplete:
			{
        completion = autocomplete(m_CommandW);
        if (completion.size() > 0)
        {
          completeCommand(completion);
        }
        return true;
			}
      case EGotoBeginLine:
      {
				m_Cursor.x = 0;
        return true;
      }
      case EGotoEndLine:
      {
				m_Cursor.x = (int)m_CommandW.size();
        return true;
      }
      case ESubmit:
      {
        handleEnterText();
        m_Cursor.x = 0;
        return true;
      }
      case EPaste:
			{
				setBuffer();
				return true;
			}
      case ECopy:
			{
				getBuffer();
        return true;
      }
      case EClearInputLine:
      {
        ClearInputLine();
        return true;
      }
      case EPrevHistoryElement:
      {
				if (history_line > 0)
					--history_line;
				getHistoryElement();
				return true;
      }
      case ENextHistoryElement:
      {
				if (history_line < (m_CmdBuffer.size() - 1))
					++history_line;
				getHistoryElement();
				return true;
      }
      case EMoveCursorToPrevChar:
      {
        moveCursor(true);
        return true;
      }
      case EMoveCursorToNextChar:
      {
        moveCursor(false);
        return true;
      }
      case EMoveCursorToPrevWord:
      {
				moveCursor(true, true);
				return true;
      }
      case EMoveCursorToNextWord:
      {
				moveCursor(false, true);
				return true;
      }
      case EDeleteRightChar:
      {
        //TODO: rewrite erasing
        m_CommandW.erase((int)m_Cursor.x, 1);
        fillCommandText();
        return true;
      }
      case EDeleteLeftChar:
      {
        if (m_CommandW.size() > 0 && (int)m_Cursor.x > 0)
        {
          //command.pop_back();
          m_CommandW.erase(std::max(0, (int)m_Cursor.x - 1), 1);
          moveCursor(true);
        }
        fillCommandText();
        return true;
      }
			case EClear:
			{
				Clear();
				return true;
			}
			case EScrolUp:
			{
				Clear();
				return true;
			}
			case EScrolDown:
			{
				Clear();
				return true;
			}
      default:
        return false;
      }
    }
    return false;
  }
  return false;
}

bool CConsole::OnInputEventUI(const SUnicodeEvent& event)
{
  if (isOpened && event.inputChar >= 32 && event.inputChar != 96)
    AddInputChar(event.inputChar);
  return false;
}

void CConsole::getHistoryElement()
{
  SetInputLine("");
  if (m_CmdBuffer.size() < 1)
    return;
  auto line_history = m_CmdBuffer[history_line];
  for (auto& element : line_history)
  {
    for (auto& ch : element.data)
    {
      AddInputChar(ch);
    }
  }
}

void CConsole::completeCommand(std::vector<std::wstring>& completion)
{
  SetInputLine("");
  std::wstring result;

  if (completion.size() == 1)
    result = completion[0];

  // Sort the given array 
  std::sort(completion.begin(), completion.end());

  // Find the minimum length from  
  // first and last string 
  auto en = std::min(completion[0].size(),
    completion[completion.size() - 1].size());

  // Now the common prefix in first and  
  // last string is the longest common prefix 
  std::wstring first = completion[0], last = completion[completion.size() - 1];
  decltype(en) i = 0;
  while (i < en && first[i] == last[i])
    i++;

  result = first.substr(0, i);

  for (auto& ch : result)
  {
    AddInputChar(ch);
  }

  if (completion.size() > 1)
  {
    addToCommandBuffer(completion);
  }
  else
  {
    AddInputChar(L' ');
  }
}

void CConsole::setBuffer()
{
  std::wstring clipboard = bb::ClipBoard::GetString();
  if (clipboard.size() != 0)
  {
    for (auto& ch : clipboard)
    {
      AddInputChar(ch);
    }
  }
}

bool CConsole::handleEnterText()
{
  cmd_is_compete = true;
  CommandLine cmd;
  /*for (auto& element : getPrompt())
  {
    cmd.push_back(element);
  }*/
  cmd.push_back(Text(wstr_to_str(m_CommandW) + "\n", textColor, 1.0));
  m_CmdBuffer.push_back(cmd);
  history_line = m_CmdBuffer.size();
  return handleCommand(m_CommandW);
}

void CConsole::addToCommandBuffer(std::vector<std::wstring>& completion)
{
  m_CmdBuffer.push_back(getPrompt());
  m_CmdBuffer.push_back({ Text(std::string("\n"), textColor, 1.0f) });
  for (auto& cmd : completion)
  {
    addText(cmd);
  }
}

void CConsole::addText(std::wstring const& cmd)
{
  m_CmdBuffer.push_back({ Text(wstr_to_str(cmd) + "\n", textColor, 1.0f) });
}

void CConsole::Set(CommandDesc& cd)
{
  if (cd.args.size() == 2)
  {
    auto name = wstr_to_str(cd.args[0]);
    auto value = wstr_to_str(cd.args[1]);
    auto var = m_mapVariables.find(name);
    if (var != m_mapVariables.end())
    {
      for (auto onChanger : varSinks)
      {
        if (onChanger->OnBeforeVarChange(var->second, value.c_str()))
        {
          return;
        }
      }
      SetInternal(var->second, value, name);
    }
    else
    {
      //PrintLine("Variable [%s] not found. Creating", name.c_str());
      CreateVariable(name.c_str(), value.c_str(), 0);
    }
  }
}

void CConsole::SetInternal(ICVar* pVar, std::string& value, std::string& name)
{
  switch (pVar->GetType())
  {
  case CVAR_INT:
    pVar->Set(static_cast<int>(std::atoi(value.c_str())));
    break;
  case CVAR_FLOAT:
    pVar->Set(static_cast<float>(std::atof(value.c_str())));
    break;
  case CVAR_STRING:
    pVar->Set(static_cast<const char*>(value.c_str()));
    break;
  default:
    PrintLine("Unknown type for [%s] variable", name.c_str());
  }
}

void CConsole::Get(CommandDesc& cd)
{
  if (cd.args.size() == 1)
  {
    auto name = wstr_to_str(cd.args[0]);
    auto var = m_mapVariables.find(name);
    if (var != m_mapVariables.end())
    {
      GetInternal(var->second, name);
    }
    else
    {
      PrintLine("Variable [%s] not found", name.c_str());
    }
  }
}

void CConsole::GetInternal(ICVar* pVar, std::string& name)
{
  switch (pVar->GetType())
  {
  case CVAR_INT:
    PrintLine("Variable = [%d]", pVar->GetIVal());
    break;
  case CVAR_FLOAT:
    PrintLine("Variable = [%f]", pVar->GetFVal());
    break;
  case CVAR_STRING:
    PrintLine("Variable = [%s]", pVar->GetString());
    break;
  default:
    PrintLine("Unknown type for [%s] variable", name.c_str());
  }
}

void CConsole::Dump()
{
  DumpCVars(this, 0);
}

void CConsole::getBuffer()
{
  std::string toClipBoard = "";
  for (auto& cmd_line : m_CommandA)
  {
    //for (auto& cmd : cmd_line)
    //{
    toClipBoard += cmd_line;// .data;
  //}
  }
  bb::ClipBoard::SetString(str_to_wstr(toClipBoard));
}

bool CConsole::needShowCursor()
{
  float dt = GetISystem()->GetDeltaTime();
  /*
  if (cursor_tick_tack)
    cursor_tick += dt;
  else
    cursor_tack += dt;
  if (cursor_tick - cursor_tack >= 0.9f)
  {
    cursor_tack = cursor_tick;
    cursor_tick_tack = false;
  }
  else
  {
    cursor_tick = cursor_tack;
    cursor_tick_tack = true;
  }
  */

  m_Cursor.blinking += dt;

  if (m_Cursor.blinking >= m_Cursor.blinkTime->GetFVal())
  {
    m_Cursor.blinking = 0.0f;
    m_Cursor.needDraw = !m_Cursor.needDraw;
  }
  return m_Cursor.needDraw;
}

void CConsole::pageUp(bool isPgUp)
{
  if (isPgUp)
    ;// page_up = true;
  else
    ;// page_dn = true;
}

void CConsole::drawCursor()
{
  if (!needShowCursor())
    return;
  auto curr_y = m_Font->GetYPos();
  m_Font->RenderText(
    m_Cursor.data,
    m_Font->CharWidth('#') + m_Font->TextWidth(m_CommandA.substr(0, static_cast<int>(m_Cursor.x))), curr_y, 1.0f, &glm::vec4(m_Cursor.color, 1.0)[0]);
}

void CConsole::moveCursor(bool left, bool wholeWord)
{
  if (left)
  {
    if (wholeWord)
    {
      std::size_t found = m_CommandA.rfind(" ", m_Cursor.x - 1);
      if (found != std::string::npos)
        m_Cursor.x = static_cast<int>(std::min((size_t)m_Cursor.x, found - 1));
    }
    else
    {
      m_Cursor.x = std::max(0, (int)m_Cursor.x - 1);
    }
  }
  else
  {
    if (wholeWord)
    {
      std::size_t found = m_CommandA.find_first_of(" ", m_Cursor.x);
      if (found != std::string::npos)
        m_Cursor.x = (int)std::min(m_CommandA.size(), found + 1);
      else
        m_Cursor.x = (int)m_CommandA.size();
    }
    else
    {
      m_Cursor.x = std::min((int)m_CommandW.size(), (int)m_Cursor.x + 1);
    }
  }
}

void CConsole::initBind()
{
  {
    m_str2key[std::string("A")] = eKI_A;
    m_str2key[std::string("B")] = eKI_B;
    m_str2key[std::string("C")] = eKI_C;
    m_str2key[std::string("D")] = eKI_D;
    m_str2key[std::string("E")] = eKI_E;
    m_str2key[std::string("F")] = eKI_F;
    m_str2key[std::string("G")] = eKI_G;
    m_str2key[std::string("H")] = eKI_H;
    m_str2key[std::string("I")] = eKI_I;
    m_str2key[std::string("J")] = eKI_J;
    m_str2key[std::string("K")] = eKI_K;
    m_str2key[std::string("L")] = eKI_L;
    m_str2key[std::string("M")] = eKI_M;
    m_str2key[std::string("N")] = eKI_N;
    m_str2key[std::string("O")] = eKI_O;
    m_str2key[std::string("P")] = eKI_P;
    m_str2key[std::string("Q")] = eKI_Q;
    m_str2key[std::string("R")] = eKI_R;
    m_str2key[std::string("S")] = eKI_S;
    m_str2key[std::string("T")] = eKI_T;
    m_str2key[std::string("U")] = eKI_U;
    m_str2key[std::string("V")] = eKI_V;
    m_str2key[std::string("W")] = eKI_W;
    m_str2key[std::string("X")] = eKI_X;
    m_str2key[std::string("Y")] = eKI_Y;
    m_str2key[std::string("Z")] = eKI_Z;
    m_str2key[std::string("Num0")] = eKI_0;
    m_str2key[std::string("Num1")] = eKI_1;
    m_str2key[std::string("Num2")] = eKI_2;
    m_str2key[std::string("Num3")] = eKI_3;
    m_str2key[std::string("Num4")] = eKI_4;
    m_str2key[std::string("Num5")] = eKI_5;
    m_str2key[std::string("Num6")] = eKI_6;
    m_str2key[std::string("Num7")] = eKI_7;
    m_str2key[std::string("Num8")] = eKI_8;
    m_str2key[std::string("Num9")] = eKI_9;
    m_str2key[std::string("Escape")] = eKI_Escape;
    m_str2key[std::string("LControl")] = eKI_LCtrl;
    m_str2key[std::string("LShift")] = eKI_LShift;
    m_str2key[std::string("LAlt")] = eKI_LAlt;
    m_str2key[std::string("LSystem")] = eKI_LWin;
    m_str2key[std::string("RControl")] = eKI_RCtrl;
    m_str2key[std::string("RShift")] = eKI_RShift;
    m_str2key[std::string("RAlt")] = eKI_RAlt;
    m_str2key[std::string("RSystem")] = eKI_RWin;
    //m_str2key[std::string("Menu")] = eKI_Menu;
    m_str2key[std::string("LBracket")] = eKI_LBracket;
    m_str2key[std::string("RBracket")] = eKI_RBracket;
    m_str2key[std::string("Semicolon")] = eKI_Semicolon;
    m_str2key[std::string("Comma")] = eKI_Comma;
    m_str2key[std::string("Period")] = eKI_Period;
    //m_str2key[std::string("Quote")] = eKI_Quote;
    m_str2key[std::string("Slash")] = eKI_Slash;
    m_str2key[std::string("Backslash")] = eKI_Backslash;
    m_str2key[std::string("Tilde")] = eKI_Tilde;
    m_str2key[std::string("Equal")] = eKI_Equals;
    //m_str2key[std::string("Hyphen")] = eKI_Hyphen;
    m_str2key[std::string("Space")] = eKI_Space;
    m_str2key[std::string("Enter")] = eKI_Enter;
    m_str2key[std::string("Backspace")] = eKI_Backspace;
    m_str2key[std::string("Tab")] = eKI_Tab;
    m_str2key[std::string("PageUp")] = eKI_PgUp;
    m_str2key[std::string("PageDown")] = eKI_PgDn;
    m_str2key[std::string("End")] = eKI_End;
    m_str2key[std::string("Home")] = eKI_Home;
    m_str2key[std::string("Insert")] = eKI_Insert;
    m_str2key[std::string("Delete")] = eKI_Delete;
    m_str2key[std::string("Add")] = eKI_NP_Add;
    m_str2key[std::string("Subtract")] = eKI_NP_Substract;
    m_str2key[std::string("Multiply")] = eKI_NP_Multiply;
    m_str2key[std::string("Divide")] = eKI_NP_Divide;
    m_str2key[std::string("Left")] = eKI_Left;
    m_str2key[std::string("Right")] = eKI_Right;
    m_str2key[std::string("Up")] = eKI_Up;
    m_str2key[std::string("Down")] = eKI_Down;
    m_str2key[std::string("Numpad0")] = eKI_NP_0;
    m_str2key[std::string("Numpad1")] = eKI_NP_1;
    m_str2key[std::string("Numpad2")] = eKI_NP_2;
    m_str2key[std::string("Numpad3")] = eKI_NP_3;
    m_str2key[std::string("Numpad4")] = eKI_NP_4;
    m_str2key[std::string("Numpad5")] = eKI_NP_5;
    m_str2key[std::string("Numpad6")] = eKI_NP_6;
    m_str2key[std::string("Numpad7")] = eKI_NP_7;
    m_str2key[std::string("Numpad8")] = eKI_NP_8;
    m_str2key[std::string("Numpad9")] = eKI_NP_9;
    m_str2key[std::string("F1")] = eKI_F1;
    m_str2key[std::string("F2")] = eKI_F2;
    m_str2key[std::string("F3")] = eKI_F3;
    m_str2key[std::string("F4")] = eKI_F4;
    m_str2key[std::string("F5")] = eKI_F5;
    m_str2key[std::string("F6")] = eKI_F6;
    m_str2key[std::string("F7")] = eKI_F7;
    m_str2key[std::string("F8")] = eKI_F8;
    m_str2key[std::string("F9")] = eKI_F9;
    m_str2key[std::string("F10")] = eKI_F10;
    m_str2key[std::string("F11")] = eKI_F11;
    m_str2key[std::string("F12")] = eKI_F12;
    m_str2key[std::string("F13")] = eKI_F13;
    m_str2key[std::string("F14")] = eKI_F14;
    m_str2key[std::string("F15")] = eKI_F15;
    m_str2key[std::string("Pause")] = eKI_Pause;

    m_str2key[std::string("xi_dpad_up")] = eKI_XI_DPadUp;
    m_str2key[std::string("xi_dpad_down")] = eKI_XI_DPadDown;
    m_str2key[std::string("xi_dpad_left")] = eKI_XI_DPadLeft;
    m_str2key[std::string("xi_dpad_right")] = eKI_XI_DPadRight;
    m_str2key[std::string("xi_start")] = eKI_XI_Start;
    m_str2key[std::string("xi_back")] = eKI_XI_Back;
    m_str2key[std::string("xi_thumbl")] = eKI_XI_ThumbL;
    m_str2key[std::string("xi_thumbr")] = eKI_XI_ThumbR;
    m_str2key[std::string("xi_shoulderl")] = eKI_XI_ShoulderL;
    m_str2key[std::string("xi_shoulderr")] = eKI_XI_ShoulderR;
    m_str2key[std::string("xi_a")] = eKI_XI_A;
    m_str2key[std::string("xi_b")] = eKI_XI_B;
    m_str2key[std::string("xi_x")] = eKI_XI_X;
    m_str2key[std::string("xi_y")] = eKI_XI_Y;
    m_str2key[std::string("xi_triggerl")] = eKI_XI_TriggerL;
    m_str2key[std::string("xi_triggerr")] = eKI_XI_TriggerR;
    m_str2key[std::string("xi_thumblx")] = eKI_XI_ThumbLX;
    m_str2key[std::string("xi_thumbly")] = eKI_XI_ThumbLY;
    // Map left thumb dpad button events to corresponing dpad hat events
    /*m_str2key[std::string("xi_dpad_up")] = eKI_XI_DPadUp;
       m_str2key[std::string("xi_dpad_down")] = eKI_XI_DPadDown;
       m_str2key[std::string("xi_dpad_left")] = eKI_XI_DPadLeft;
       m_str2key[std::string("xi_dpad_right")] = eKI_XI_DPadRight;*/
    m_str2key[std::string("xi_thumbrx")] = eKI_XI_ThumbRX;
    m_str2key[std::string("xi_thumbry")] = eKI_XI_ThumbRY;
    /*m_str2key[std::string("xi_thumbr_up")] = eKI_XI_ThumbRUp;
       m_str2key[std::string("xi_thumbr_down")] = eKI_XI_ThumbRDown;
       m_str2key[std::string("xi_thumbr_left")] = eKI_XI_ThumbRLeft;
       m_str2key[std::string("xi_thumbr_right")] = eKI_XI_ThumbRRight;*/
    m_str2key[std::string("xi_triggerl_btn")] = eKI_XI_TriggerLBtn;
    m_str2key[std::string("xi_triggerr_btn")] = eKI_XI_TriggerRBtn;
  }
}

void CConsole::Exit(const char* command, ...)
{
}

char* CConsole::GetVariable(const char* szVarName, const char* szFileName, const char* def_val)
{
  return nullptr;
}

float CConsole::GetVariable(const char* szVarName, const char* szFileName, float def_val)
{
  return 0.0f;
}

void CConsole::PrintLinePlus(const char* s)
{
}

void CConsole::SetScrollMax(int value)
{
  m_nScrollMax = value;
  m_nTempScrollMax = m_nScrollMax;
}

void CConsole::SetLoadingImage(const char* szFilename)
{
  ITexture* pTex = 0;

  pTex = gEnv->pRenderer->LoadTexture(szFilename, 0, 0);
  //pTex = TextureManager::instance()->getTexture(szFilename, false);

  if (!pTex)
  {
    SAFE_DELETE(pTex);
  }

  if (pTex)
  {
    m_nLoadingBackTexID = pTex->getId();
  }
  else
  {
    m_nLoadingBackTexID = -1;
  }
}

void CConsole::ResetProgressBar(int nProgressRange)
{
}

void CConsole::TickProgressBar()
{
}

void CConsole::CreateKeyBind(const char* key, const char* cmd)
{
	m_mapBinds.insert(ConsoleBindsMap::value_type(key, cmd));
}

void CConsole::SetInputLine(const char* szLine)
{
  m_CommandW = str_to_wstr(szLine);
  m_Cursor.x = 0;
}

void CConsole::LoadConfigVar(const char* szVariable, const char* sValue)
{
	ICVar* pCVar = GetCVar(szVariable);
	if (pCVar)
	{
		const bool isCheat = ((pCVar->GetFlags() & (VF_CHEAT | VF_CHEAT_NOCHECK | VF_CHEAT_ALWAYS_CHECK)) != 0);
		const bool isReadOnly = ((pCVar->GetFlags() & VF_READONLY) != 0);
		const bool isDeprecated = ((pCVar->GetFlags() & VF_DEPRECATED) != 0);
		const bool wasInConfig = ((pCVar->GetFlags() & VF_WASINCONFIG) != 0);
		const bool fromSystemConfig = ((pCVar->GetFlags() & VF_SYSSPEC_OVERWRITE) != 0);
		bool allowChange = true;

		if (allowChange)
		{
			pCVar->Set(sValue);
		}
		return;
	}

  if (auto it = m_ConfigVars.find(szVariable); it == m_ConfigVars.end())
	{
	  m_ConfigVars[szVariable] = sValue;
	}
}

void CConsole::ClearInputLine()
{
  m_CommandW.clear();
  m_Cursor.x = 0;
  fillCommandText();
}

bool CConsole::MatchInput(const SInputEvent& event)
{
	return false;
}

IFont* CConsole::getFont(const char* name, float w, float h)
{
  if (gEnv->IsDedicated())
    m_Font = new CNullFont();
  else
  {
    m_Font = gEnv->pRenderer->GetIFont();
    auto font = name;
    auto var = GET_CVAR("s_font");
    if (var)
      font = var->GetString();
    m_Font->Init(font, static_cast<unsigned int>(w), static_cast<unsigned int>(h));
    //return m_Font;
  }
  return m_Font;
}

void CConsole::InitInputBindings()
{
	auto CreateInputEvent = [](EKeyId keyId, int modifiers, EInputState state)
	{
		SInputEvent event;
		event.keyId = keyId;
		event.modifiers = modifiers;
		event.state = state;
		return event;
	};
	auto CreateBinding = [this](SInputEvent event, EInputFunctions function)
	{
		m_InputBindings[event] = function;
	};

	CreateBinding(CreateInputEvent(eKI_Tab, eMM_None, EInputState::eIS_Pressed), EAutoComplete);
	CreateBinding(CreateInputEvent(eKI_A, eMM_LCtrl, EInputState::eIS_Pressed), EGotoBeginLine);
	CreateBinding(CreateInputEvent(eKI_E, eMM_LCtrl, EInputState::eIS_Pressed), EGotoEndLine);
	CreateBinding(CreateInputEvent(eKI_Enter, eMM_None, EInputState::eIS_Pressed), ESubmit);
	CreateBinding(CreateInputEvent(eKI_M, eMM_LCtrl, EInputState::eIS_Pressed), ESubmit);
	CreateBinding(CreateInputEvent(eKI_Insert, eMM_LCtrl, EInputState::eIS_Pressed), ECopy);
	CreateBinding(CreateInputEvent(eKI_Insert, eMM_LShift, EInputState::eIS_Pressed), EPaste);
	CreateBinding(CreateInputEvent(eKI_Escape, eMM_LCtrl, EInputState::eIS_Pressed), EClearInputLine);
	CreateBinding(CreateInputEvent(eKI_N, eMM_LCtrl, EInputState::eIS_Pressed), ENextHistoryElement);
	CreateBinding(CreateInputEvent(eKI_P, eMM_LCtrl, EInputState::eIS_Pressed), EPrevHistoryElement);
	CreateBinding(CreateInputEvent(eKI_Left, eMM_None, EInputState::eIS_Pressed), EMoveCursorToPrevChar);
	CreateBinding(CreateInputEvent(eKI_Right, eMM_None, EInputState::eIS_Pressed), EMoveCursorToNextChar);
	CreateBinding(CreateInputEvent(eKI_B, eMM_LAlt, EInputState::eIS_Pressed), EMoveCursorToPrevWord);
	CreateBinding(CreateInputEvent(eKI_F, eMM_LAlt, EInputState::eIS_Pressed), EMoveCursorToNextWord);
	CreateBinding(CreateInputEvent(eKI_Delete, eMM_None, EInputState::eIS_Pressed), EDeleteRightChar);
	CreateBinding(CreateInputEvent(eKI_Backspace, eMM_None, EInputState::eIS_Pressed), EDeleteLeftChar);
	CreateBinding(CreateInputEvent(eKI_L, eMM_LCtrl, EInputState::eIS_Pressed), EClear);

	CreateBinding(CreateInputEvent(eKI_1, eMM_LCtrl, EInputState::eIS_Pressed), EClear);
	CreateBinding(CreateInputEvent(eKI_2, eMM_LCtrl, EInputState::eIS_Pressed), EClear);
}

const char* CConsole::FindKeyBind(const char* sCmd)
{
	ConsoleBindsMap::const_iterator it = m_mapBinds.find(sCmd);

	if (it != m_mapBinds.end())
		return it->second.c_str();

	return 0;
}

void CConsole::Release()
{
	delete this;
}

void CConsole::AddCommand(const char* sCommand, ConsoleCommandFunc func, int nFlags/* = 0*/, const char* help/* = NULL*/)
{
  CommandInfo cmdInfo;
  cmdInfo.Func = func;
  if (help) cmdInfo.help = help;
  cmdInfo.type = CommandInfo::Type::FUNC;
  m_Commands[str_to_wstr(std::string(sCommand))] = cmdInfo;
}

void CConsole::AddWorkerCommand(IWorkerCommand* cmd)
{
  m_workers.insert(cmd);
}

void CConsole::RemoveWorkerCommand(IWorkerCommand* cmd)
{
  m_worker_to_delete.push_back(cmd);
}

void CConsole::UnregisterVariable(const char* sVarName, bool bDelete/* = false*/)
{
	if (auto it = m_mapVariables.find(sVarName); it != m_mapVariables.end())
	{
		ICVar* var = it->second;
		m_mapVariables.erase(it);
		delete var;
	}
}
void CConsole::AddCommand(const char* sName, const char* sScriptFunc, const uint32_t indwFlags/* = 0*/, const char* help/* = ""*/)
{
  CommandInfo cmdInfo;
  cmdInfo.Script.code = sScriptFunc;
  if (help) cmdInfo.help = help;
  cmdInfo.type = CommandInfo::Type::SCRIPT;
  m_Commands[str_to_wstr(std::string(sName))] = cmdInfo;
}

void CConsole::DumpCVars(ICVarDumpSink* pCallback, unsigned int nFlagsFilter)
{
	for (auto& pair : m_mapVariables)
	{
		if ((nFlagsFilter == 0) || ((nFlagsFilter != 0) && (pair.second->GetFlags() & nFlagsFilter)))
			pCallback->OnElementFound(pair.second);
	}
}

void CConsole::OnElementFound(ICVar* pCVar)
{
  auto name = pCVar->GetName();
  auto helpString = pCVar->GetHelp();
  auto help = helpString[0] != '\0' ? ". Help: " + std::string(helpString) : "";
  switch (pCVar->GetType())
  {
  case CVAR_INT:
    PrintLine("%s = %d%s", name, pCVar->GetIVal(), help.c_str());
    break;
  case CVAR_FLOAT:
    PrintLine("%s = %f%s", name, pCVar->GetFVal(), help.c_str());
    break;
  case CVAR_STRING:
    PrintLine("%s = %s%s", name, pCVar->GetString(), help.c_str());
    break;
  default:
    //PrintLine("Unknown type for [%s] variable", name.c_str());
    break;
  }
}

void CConsole::AddConsoleVarSink(IConsoleVarSink* pSink)
{
  varSinks.push_back(pSink);
}

void CConsole::RemoveConsoleVarSink(IConsoleVarSink* pSink)
{
  auto var = varSinks.begin();
  for (; var != varSinks.end(); var++)
  {
    if (*var == pSink)
    {
      break;
    }
  }
  if (var != varSinks.end())
  {
    varSinks.erase(var);
  }
}

ICVar* CConsole::GetCVar(const char* name, const bool bCaseSensitive)
{
  std::string data = name;
  //ICVar* pVar = &m_NullCVar;
  ICVar* pVar = nullptr;
  if (!bCaseSensitive)
  {
    std::transform(data.begin(), data.end(), data.begin(),
      [](unsigned char c) { return std::tolower(c); });
  }
  auto var = m_mapVariables.find(name);
  if (var != m_mapVariables.end())
  {
    pVar = var->second;
  }
  return pVar;
}

bool CConsole::Init(ISystem* pSystem)
{
  m_pSystem = pSystem;
  m_pRenderer = pSystem->GetIRenderer();
  m_pScriptSystem = pSystem->GetIScriptSystem();
  m_pInput = pSystem->GetIInput();
#pragma warning(push)
#pragma warning(disable: 4244)
  m_Font = getFont("arial.ttf", 16, line_height);
#pragma warning(pop)
  const char* texture_path = "console_background2.jpg";
  ICVar* background = GetCVar("console_background");
  r_anim_speed = CreateVariable("r_anim_speed", 0.1f, 0);
  m_Cursor.blinkTime = CreateVariable("btime", 1.0f, 0, "Time of cursor blinking");

  if (!gEnv->IsDedicated())
  {
    if (background != nullptr)
      texture_path = background->GetString();
    m_pBackGround = gEnv->pRenderer->LoadTexture(texture_path, 0, 0);
    initBind();

    m_ScrollHeight = m_pRenderer->GetHeight() / 2.0f;
    Register("scrol_height", &m_ScrollHeight, m_ScrollHeight, VF_DUMPTODISK, "Console scroll height");

		InitInputBindings();
  }

  return true;
}

bool CConsole::IsOpened()
{
  return isOpened;
}

ICVar* CConsole::CreateVariable(const char* sName, const char* sValue, int nFlags, const char* help)
{
  ICVar* pCVar(nullptr);
  auto it = m_mapVariables.find(sName);
  if (it != m_mapVariables.end())
  {
    pCVar = it->second;
    GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(string): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
    gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
    return pCVar;
  }
  /*
  if (!allowModify)
    nFlags |= VF_CONST_CVAR;
  */
  pCVar = new CCVar(sName, strdup(sValue), nFlags, const_cast<char*>(help));
  RegisterVar(pCVar/*, pChangeFunc*/);
  return pCVar;
}

ICVar* CConsole::CreateVariable(const char* sName, int iValue, int nFlags, const char* help)
{
  ICVar* pCVar(nullptr);
  auto it = m_mapVariables.find(sName);
  if (it != m_mapVariables.end())
  {
	  pCVar = it->second;
    GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(int): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
    gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
    return pCVar;
  }
  /*
  if (!allowModify)
    nFlags |= VF_CONST_CVAR;
  */
  pCVar = new CCVar(sName, iValue, nFlags, const_cast<char*>(help));
  RegisterVar(pCVar/*, pChangeFunc*/);
  return pCVar;
}

ICVar* CConsole::CreateVariable(const char* sName, float fValue, int nFlags, const char* help)
{
  ICVar* pCVar(nullptr);
  auto it = m_mapVariables.find(sName);
  if (it != m_mapVariables.end())
  {
    pCVar = it->second;
    GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(float): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
    gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
    return pCVar;
  }
  /*
  if (!allowModify)
    nFlags |= VF_CONST_CVAR;
  */
  
  pCVar = new CCVar(sName, fValue, nFlags, const_cast<char*>(help));
  RegisterVar(pCVar/*, pChangeFunc*/);
  return pCVar;
}

ICVar* CConsole::Register(const char* name, const char** src, const char* defaultvalue, int flags, const char* help/* = ""*/)
{
  ICVar* pCVar(nullptr);
  auto it = m_mapVariables.find(name);
  if (it != m_mapVariables.end())
  {
    pCVar = it->second;
    GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(string): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
    gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
	return pCVar;
  }
  /*
  if (!allowModify)
    nFlags |= VF_CONST_CVAR;
  */
  pCVar = new CCVarRef(name, src, defaultvalue, flags, help);
  RegisterVar(pCVar/*, pChangeFunc*/);
  return pCVar;
}

ICVar* CConsole::Register(const char* name, float* src, float defaultvalue, int flags/* = 0*/, const char* help/* = ""*/)
{
  ICVar* pCVar(nullptr);
  auto it = m_mapVariables.find(name);
  if (it != m_mapVariables.end())
  {
    pCVar = it->second;
    GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(float): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
    gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
    return pCVar;
  }
  /*
  if (!allowModify)
    nFlags |= VF_CONST_CVAR;
  */
  pCVar = new CCVarRef(name, src, defaultvalue, flags, help);
  RegisterVar(pCVar/*, pChangeFunc*/);
  return pCVar;
  
}

ICVar* CConsole::Register(const char* name, int* src, int defaultvalue, int flags/* = 0*/, const char* help/* = ""*/)
{
  ICVar* pCVar(nullptr);
  auto it = m_mapVariables.find(name);
  if (it != m_mapVariables.end())
  {
	  pCVar = it->second;
    GetISystem()->GetILog()->LogError("[CVARS]: [DUPLICATE] CXConsole::Register(int): variable [%s] is already registered", pCVar->GetName());
#if LOG_CVAR_INFRACTIONS_CALLSTACK
    gEnv->pSystem->debug_LogCallStack();
#endif // LOG_CVAR_INFRACTIONS_CALLSTACK
    return pCVar;
  }
  /*
  if (!allowModify)
    nFlags |= VF_CONST_CVAR;
  */
  pCVar = new CCVarRef(name, src, defaultvalue, flags, help);
  RegisterVar(pCVar/*, pChangeFunc*/);
  return pCVar;
}


void CConsole::AddInputChar(uint32_t ch)
{
  if (iswgraph(ch) || (iswblank(ch) && ch != '\t'))
  {
    m_CommandW.insert((int)m_Cursor.x, 1, ch);// += ch;
    moveCursor(false);
  }
  fillCommandText();
}

bool CConsole::handleCommand(std::wstring command)
{
#if !defined(RELEASE) || defined(ENABLE_DEVELOPER_CONSOLE_IN_RELEASE)
  ///////////////////////////
  //Execute as string
  if (command[0] == '#' || command[0] == '@')
  {
    if (/*!con_restricted || *//*isOpened*/true)      // in restricted mode we allow only VF_RESTRICTEDMODE CVars&CCmd
    {
      std::string str = wstr_to_str(command);
      //PrintLine(str.c_str());

      if (m_pSystem->IsDevMode())
      {
        if (m_pSystem->GetIScriptSystem())
          m_pSystem->GetIScriptSystem()->ExecuteBuffer(str.c_str() + 1, str.length() - 1);
        //m_bDrawCursor = 0;
      }
      else
      {
        // Warning.
        // No Cheat warnings. ConsoleWarning("Console execution is cheat protected");
      }
      return true;
    }
  }
#endif

  bool result = false;
  auto cd = parseCommand(command);
  //cd.history = &history;

  auto cmd_it = m_Commands.find(cd.command);

  if (cmd_it != m_Commands.end())
  {
    if (cmd_it->second.type == CommandInfo::Type::INTERFACE)
    {
      result = cmd_it->second.Command->execute(cd);
    }
    else if (cmd_it->second.type == CommandInfo::Type::SCRIPT)
    {
      std::string code(cmd_it->second.Script.code);
      findAndReplaceAll(code, "%", [&cd](int n) -> std::string {
        return "\"" + wstr_to_str(cd.get(n - 1)) + "\"";
      });
      result = m_pScriptSystem->ExecuteBuffer(code.c_str(), code.length());
    }
    else if (cmd_it->second.type == CommandInfo::Type::FUNC)
    {
      result = cmd_it->second.Func(cd);
    }
  }
  else
  {
    auto var_it = m_mapVariables.find(wstr_to_str(cd.command));
    if (var_it != m_mapVariables.end())
    {
      CommandDesc desc;
      desc.args.push_back(cd.command);
      if (cd.args.size() == 0)
      {
        Get(desc);
      }
      else if (cd.args.size() == 1)
      {
        desc.args.push_back(cd.args[0]);
        Set(desc);
      }
    }
    else if (cd.command == L"close")
      isOpened = false;
  }
  //history.push_back(str_to_wstr(getPrompt()) + command);
  return result;
}

void CConsole::RegisterVar(ICVar* pCVar)
{
	const bool isCheat = ((pCVar->GetFlags() & (VF_CHEAT | VF_CHEAT_NOCHECK | VF_CHEAT_ALWAYS_CHECK)) != 0);
	const bool isReadOnly = ((pCVar->GetFlags() & VF_READONLY) != 0);
	const bool isDeprecated = ((pCVar->GetFlags() & VF_DEPRECATED) != 0);

	auto it = m_ConfigVars.find(pCVar->GetName());
	if (it != m_ConfigVars.end())
	{
		bool allowChange			  = true;

		if (allowChange)
		{
			pCVar->Set(it->second.c_str());
		}

		m_ConfigVars.erase(it);
	}
	else
	{
		// Variable is not modified when just registered.
		pCVar->ClearFlags(VF_MODIFIED);
	}

  //gEnv->pLog->Log("Registered variable %s with value %s", pCVar->GetName(), pCVar->GetString());

  m_mapVariables[pCVar->GetName()] = pCVar;
}

CommandDesc CConsole::parseCommand(std::wstring& command)
{
  enum { COMMAND, ARGS, INCMD, INSPACE, INARGSPACE, AFTER_EQ, INARG, INSTRING } state1 = INSPACE;
  CommandDesc cd;
  int begin_cmd = 0;
  std::wstring current_arg;
  std::wstring value;
  bool get_value = false;

  auto getVal = [this](std::wstring name) -> std::wstring {
    auto str = wstr_to_str(name);

    if (auto var = GetCVar(str.c_str()))
    {
      return str_to_wstr(var->GetString());
    }
    return std::wstring();
  };

  for (int i = begin_cmd; i < command.size(); i++)
  {
    switch (state1)
    {
    case COMMAND:
      if (command[i] != L' ')
      {
        if (command[i] == L'"')
        {
          state1 = INSTRING;
          break;
        }
        cd.command += command[i];
      }
      else
      {
        state1 = INARGSPACE;
      }
      break;
    case INSTRING:
    {
      if (command[i] != L'"')
        current_arg += command[i];
      else
        state1 = ARGS;
      break;
    }
    case ARGS:
      if (command[i] != L' ')
      {
        if (command[i] == L'"')
        {
          state1 = INSTRING;
          break;
        }
        if (get_value)
          value += command[i];
        else
          current_arg += command[i];
      }
      else
      {
        state1 = INARGSPACE;
        if (get_value)
        {
          cd.args.push_back(getVal(value));
          value.clear();
          get_value = false;
        }
        else
        {
          cd.args.push_back(current_arg);
          current_arg.clear();
        }
      }
      break;
    case INCMD:
      break;
    case INSPACE:
      if (command[i] != L' ')
      {
        state1 = COMMAND;
        cd.command += command[i];
      }
      break;
    case AFTER_EQ:
		if (command[i] != L' ')
		{
			if (command[i] == '\"')
			{
				state1 = INSTRING;
			}
			else
			{
				state1 = ARGS;
				current_arg += command[i];
			}
		}
		break;
    case INARGSPACE:
      if (command[i] != L' ')
      {
        state1 = ARGS;
        if (command[i] == L'@')
        {
          get_value = true;
        }
        else if (command[i] == L'=')
        {
					auto arg   = cd.command;
					cd.args.insert(cd.args.begin(), arg);
					cd.command = L"set";
					state1	   = AFTER_EQ;
        }
        else
        {
          if (command[i] == L'"')
          {
            state1 = INSTRING;
            break;
          }
          else current_arg += command[i];
        }
      }
      break;
    case INARG:
      break;
    default:
      break;
    }
  }
  if (state1 == ARGS)
  {
    if (get_value)
      cd.args.push_back(getVal(value));
    else
      cd.args.push_back(current_arg);
  }

  return cd;
}

std::vector<std::wstring> CConsole::autocomplete(std::wstring cmd)
{
  std::vector<std::wstring> completion;
  for (auto& curr_cmd : m_Commands)
  {
    if (curr_cmd.first.substr(0, cmd.size()) == cmd)
    {
      completion.push_back(curr_cmd.first);
    }
  }
  for (auto& cur_var : m_mapVariables)
  {
    if (cur_var.first.substr(0, cmd.size()) == wstr_to_str(cmd))
    {
      completion.push_back(str_to_wstr(cur_var.first));
    }
  }
  return completion;
}

void CConsole::doFile(std::ifstream& cfg)
{
  std::string line;
  while (std::getline(cfg, line))
  {
    handleCommand(str_to_wstr(line));
  }
  cfg.clear();
  cfg.seekg(0, std::ios::beg);
}

void CConsole::ExecuteFile(const char* file)
{
  auto it = scripts.find(file);
  if (it == scripts.end())
  {
    std::ifstream f(file);
    if (!f.is_open())
      return;
    doFile(f);
    scripts[file] = std::move(f);
  }
}

CConsole::CConsole()
  :
  m_pBackGround(nullptr)
{
  //prompt = user + " #";
  AddCommand("help", new HelpCommand());
  AddCommand("set", new SetCommand(this));
  AddCommand("get", new GetCommand(this));
  AddCommand("dump", new DumpCommand(this));
  m_MessageBuffer.resize(MESSAGE_BUFFER_SIZE);
}
CConsole::~CConsole()
{
  if (m_Font) delete m_Font;
  if (m_pBackGround) delete m_pBackGround;

	if (!m_mapVariables.empty())
	{
		while (!m_mapVariables.empty())
			m_mapVariables.begin()->second->Release();

		m_mapVariables.clear();
	}
}

void CConsole::ShowConsole(bool show)
{
  if (isOpened != show)
  {
    isOpened = show;
    if (show)
    {
      m_AnimationParams.animate = true;
    }
  }
}

void CConsole::fillCommandText()
{
  m_CommandA.clear();
  for (auto ch : m_CommandW)
  {
    m_CommandA.push_back(static_cast<char>(ch));
  }
  //command_text.push_back(cursor);
}

void CConsole::setFont(IFont* font)
{
  m_Font = font;
}

CommandLine CConsole::getPrompt()
{
  return m_Prompt.get();
}

void CConsole::printLine(size_t line)
{
  //for (auto &element = cmd_buffer[line].begin(); element != cmd_buffer[line].end(); element++, i++)
  for (const auto& element : m_CmdBuffer[line])
  {
    printText(element, line);
  }
}

void CConsole::printText(Text const& element, size_t line)
{
  auto curr_y = m_Font->GetYPos();
  m_Font->RenderText(
    element.data,
    m_Font->GetXPos(), curr_y, 1.0f, &glm::vec4(element.color, 1.0)[0]);
  if (curr_y < m_Font->GetYPos())
  {
    // TODO: handle this
  }
}

void CConsole::AddArgumentCompletion(const char* cmd, const char* arg, int n)
{
}

void CConsole::Clear()
{
  m_CmdBuffer.clear();
  m_Cursor.x = 0;
  m_Cursor.y = 0;
  history_line = 0;
}

void CConsole::Help(const char* cmd)
{
  if (cmd != nullptr)
  {
    auto name = str_to_wstr(std::string(cmd));
    auto it = m_Commands.find(name);
    const char* help = nullptr;
    if (it != m_Commands.end())
    {
      help = it->second.help.c_str();
    }
    else
    {
      auto it = m_mapVariables.find(cmd);
      if (it != m_mapVariables.end())
        help = it->second->GetHelp();
      else
        return;
    }

    m_CmdBuffer.push_back({ Text(std::string(cmd) + ": " + help + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
  }
  else
  {
    for (auto& cmd : m_Commands)
    {
      if (cmd.second.help.size() > 0)
        m_CmdBuffer.push_back({ Text(std::string(wstr_to_str(cmd.first)) + ": " + cmd.second.help + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
    }
    for (auto& var : m_mapVariables)
    {
      m_CmdBuffer.push_back({ Text(var.first + ": " + var.second->GetHelp() + "\n", glm::vec3(1.f,1.f,1.f), 1.0) });
    }
  }
}

void CConsole::PrintLine(const char* format, ...)
{
  va_list ptr;
  va_start(ptr, format);
  vsnprintf(const_cast<char*>(m_MessageBuffer.data()), MESSAGE_BUFFER_SIZE, format, ptr);
  va_end(ptr);

  addText(str_to_wstr(m_MessageBuffer));
}

char* CCVar::GetString()
{
	static char sResult[256];
	char* result;
	if (type == CVAR_INT)
	{
		sprintf(sResult, "%d", value.i);
		result = sResult;
  }
	else if (type == CVAR_FLOAT)
	{
		sprintf(sResult, "%f", value.f);
		result = sResult;
	}
	else
	{
		result = value.s;
	}
  return result;
}

void CCVar::Set(const char* s)
{
  if (value.s != nullptr && type == CVAR_STRING)
    delete[] value.s;
  value.s = strdup(s);
  type = CVAR_STRING;
}

void CCVar::ForceSet(const char* s)
{
  if (value.s != nullptr)
    delete[] value.s;
  type = CVAR_STRING;
  value.s = const_cast<char*>(s);
}

void CCVar::Set(float f)
{
  if (type == CVAR_STRING)
  {
    delete[] value.s;
  }
  type = CVAR_FLOAT;
  value.f = f;
}

void CCVar::Set(int i)
{
  if (type == CVAR_STRING)
  {
    delete[] value.s;
  }
  type = CVAR_INT;
  value.i = i;
}

void CCVar::Refresh()
{
  value.i = 0;
}

int CCVar::GetType()
{
  return type;
}

const char* CCVar::GetHelp()
{
  return help;
}

int CCVar::GetIVal()
{
  if (type == CVAR_FLOAT)
    value.i = static_cast<int>(value.f);
  else if (type == CVAR_STRING)
    value.i = static_cast<int>(std::atoi(value.s));
  type = CVAR_INT;
  return value.i;
}

float CCVar::GetFVal()
{
  if (type == CVAR_INT)
    value.f = static_cast<float>(value.i);
  else if (type == CVAR_STRING)
    value.f = static_cast<float>(std::atof(value.s));
  type = CVAR_FLOAT;
  return value.f;
}

int CCVarRef::GetIVal()
{
  if (type == CVAR_FLOAT)
    *value.i = static_cast<int>(*value.f);
  else if (type == CVAR_STRING)
    *value.i = static_cast<int>(std::atoi(*value.s));
  type = CVAR_INT;
  return *value.i;
}

float CCVarRef::GetFVal()
{
  if (type == CVAR_INT)
    *value.f = static_cast<float>(*value.i);
  else if (type == CVAR_STRING)
    *value.f = static_cast<float>(std::atof(*value.s));
  type = CVAR_FLOAT;
  return *value.f;
}

char* CCVarRef::GetString()
{
	static char sResult[256];
	char* result;
	if (type == CVAR_INT)
	{
		sprintf(sResult, "%d", *value.i);
		result = sResult;
  }
	else if (type == CVAR_FLOAT)
	{
		sprintf(sResult, "%f", *value.f);
		result = sResult;
	}
	else
	{
		result = *value.s;
	}
  return result;
}

void CCVarRef::Set(const char* s)
{
  #if 0
  if (*value.s != nullptr && type == CVAR_STRING)
    delete[] *value.s;
  *value.s = strdup(s);
  type = CVAR_STRING;
  #endif
	if (type == CVAR_INT)
	{
		Set(atoi(s));	
  }
	else
	if (type == CVAR_FLOAT)
	{
		Set((float)atof(s));	
  }
	else
  {
		ForceSet(s); 
  }
}

void CCVarRef::ForceSet(const char* s)
{
  if (value.s != nullptr)
    delete[] * value.s;
  type = CVAR_STRING;
  *value.s = const_cast<char*>(s);
}

void CCVarRef::Set(float f)
{
  if (type == CVAR_STRING)
  {
    delete[] * value.s;
  }
  type = CVAR_FLOAT;
  *value.f = f;
}

void CCVarRef::Set(int i)
{
  if (type == CVAR_STRING)
  {
    delete[] * value.s;
  }
  type = CVAR_INT;
  *value.i = i;
}

void CCVarRef::Refresh()
{
  *value.i = 0;
}

int CCVarRef::GetType()
{
  return type;
}

const char* CCVarRef::GetHelp()
{
  return help;
}

CommandLine ConsolePrompt::get()
{
  auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string time_str = std::ctime(&time);
  time_str[time_str.size() - 1] = 0;
  return {
    Text(user + "@" + pc, glm::vec3(0.0, 1.0, 0.0), 1.0) ,
    Text(" " + env, glm::vec3(1.0, 0.0, 1.0), 1.0) ,
    Text(" " + cd, glm::vec3(1.0, 1.0, 0.0), 1.0) ,
    Text(std::string(" " + time_str), color, 1.0)//,
    //Text(" FPS: " + std::to_string(GetISystem()->GetIGame()->getFPS()) + "\n", glm::vec3(1.0, 0.3, 0.5), 1.0),
  };
}

void CNullCVar::Release()
{
}

int CNullCVar::GetIVal()
{
	return 0;
}

float CNullCVar::GetFVal()
{
	return 0.0f;
}

char* CNullCVar::GetString()
{
	return nullptr;
}

void CNullCVar::Set(const char* s)
{
}

void CNullCVar::ForceSet(const char* s)
{
}

void CNullCVar::Set(float f)
{
}

void CNullCVar::Set(int i)
{
}

void CNullCVar::Refresh()
{
}

void CNullCVar::ClearFlags(int flags)
{
}

int CNullCVar::GetFlags()
{
	return 0;
}

int CNullCVar::SetFlags(int flags)
{
	return 0;
}

int CNullCVar::GetType()
{
	return 0;
}

const char* CNullCVar::GetName()
{
	return nullptr;
}

const char* CNullCVar::GetHelp()
{
	return nullptr;
}
