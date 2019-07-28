#pragma once
#include <BlackBox/IConsole.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>

#include <map>
#include <vector>
#include <string>

struct IFont;

struct CommandDesc
{
	std::wstring command;
	std::vector<std::wstring> args;
	std::vector<std::wstring> *history;
	CommandDesc(){}
	CommandDesc(decltype(history) *history) : history(nullptr)
	{

	}
};

class CConsole : public IConsole, public IInputEventListener
{
public:
	CConsole();
	// Унаследовано через IConsole
	virtual void ShowConsole(bool show) override;
	virtual void SetImage(ITexture* pTexture) override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void AddCommand(const char* sName, IEditCommand* command, const char* help = "") override;
	virtual void ExecuteString(const char* command) override;
	virtual void ExecuteFile(const char* file) override;

	// Унаследовано через IInputEventListener
	virtual bool OnInputEvent(sf::Event& event) override;

private:
private:
	CommandDesc parseCommand(std::wstring& command);
	bool handleCommand(std::wstring command);
	void handleCommandTextEnter(uint32_t ch);
	std::vector<std::wstring> autocomplete(std::wstring cmd);
	void doFile(std::ifstream& cfg);
	void fillCommandText();
	void setFont(IFont* font);
private:
	std::map<std::wstring, IEditCommand*> m_Commands;
	std::map<std::string, std::ifstream> scripts;
	bool is_input = false;
	bool input_trigered = false;
	std::wstring command;
	std::string command_text;
	IFont* m_Font;
	bool isShow = false;
};
