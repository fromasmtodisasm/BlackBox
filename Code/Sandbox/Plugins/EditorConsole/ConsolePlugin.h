#include <IPlugin.h>
#define CONSOLE_MAX_HISTORY 512

class CEngineListener
{
public:
	CEngineListener(ISystem* pSystem) {}
	bool Init(int) { return true; }
};

//console plugin class, UI side
class CConsolePlugin : public IPlugin
	, public CEngineListener
	, public IEditorNotifyListener
{
	//called back by the EngineListener when we need to send messages
	void EmitCVar(ICVar* pVar) const {}
	void EmitLine(size_t index, const string& line) const {}
	void DestroyCVar(ICVar* pVar) const {}

  public:
	CConsolePlugin();
	~CConsolePlugin();

	//Messages::SAutoCompleteReply HandleAutoCompleteRequest(const Messages::SAutoCompleteRequest& req);

	//the singleton instance of the plugin
	static CConsolePlugin* GetInstance() { return s_pInstance; }

	//get a unique address for messages
	string GetUniqueAddress() const;

	//IPlugin implementation
	int32		GetPluginVersion() override { return 1; }
	const char* GetPluginName() override { return "Console"; }
	const char* GetPluginDescription() override { return "Adds the Console window"; }

  private:
	//singleton instance
	static CConsolePlugin* s_pInstance;

	// Inherited via IEditorNotifyListener
	virtual void OnEditorNotifyEvent(EEditorNotifyEvent event) override;
};
