#include "ISystem.h"
#include <ILog.h>


#define LOG_FUNCTION() CryLog("called: %s", __FUNCTION__)

namespace Legacy
{
class CSystemLegacy : public legacy::ISystem
{
	// Inherited via ISystem
	virtual bool CreateGame(const legacy::SGameInitParams& params) override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual void Release() override
	{
		LOG_FUNCTION();
	}
	virtual bool Update(int updateFlags = 0, int nPauseMode = 0) override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual void UpdateScriptSink() override
	{
		LOG_FUNCTION();
	}
	virtual void RenderBegin() override
	{
		LOG_FUNCTION();
	}
	virtual void Render() override
	{
		LOG_FUNCTION();
	}
	virtual void RenderEnd() override
	{
		LOG_FUNCTION();
	}
	virtual void RenderStatistics() override
	{
		LOG_FUNCTION();
	}
	virtual int GetCPUFlags() override
	{
		LOG_FUNCTION();
		return 0;
	}
	virtual double GetSecondsPerCycle() override
	{
		LOG_FUNCTION();
		return 0.0;
	}
	virtual void DumpMemoryUsageStatistics() override
	{
		LOG_FUNCTION();
	}
	virtual void Quit() override
	{
		LOG_FUNCTION();
	}
	virtual void Relaunch(bool bRelaunch) override
	{
		LOG_FUNCTION();
	}
	virtual bool IsQuitting() override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual void Error(const char* sFormat, ...) override
	{
		LOG_FUNCTION();
	}
	virtual void Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...) override
	{
		LOG_FUNCTION();
	}
	virtual bool CheckLogVerbosity(int verbosity) override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual IGame* GetIGame() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual INetwork* GetINetwork() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IRenderer* GetIRenderer() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IInput* GetIInput() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual ITimer* GetITimer() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IConsole* GetIConsole() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IScriptSystem* GetIScriptSystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual I3DEngine* GetI3DEngine() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual ISoundSystem* GetISoundSystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IMusicSystem* GetIMusicSystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IPhysicalWorld* GetIPhysicalWorld() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IMovieSystem* GetIMovieSystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IAISystem* GetAISystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IMemoryManager* GetIMemoryManager() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IEntitySystem* GetIEntitySystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual ICryFont* GetICryFont() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual ICryPak* GetIPak() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual ILog* GetILog() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IStreamEngine* GetStreamEngine() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual ICryCharManager* GetIAnimationSystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IValidator* GetIValidator() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IFrameProfileSystem* GetIProfileSystem() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual void DebugStats(bool checkpoint, bool leaks) override
	{
		LOG_FUNCTION();
	}
	virtual void DumpWinHeaps() override
	{
		LOG_FUNCTION();
	}
	virtual int DumpMMStats(bool log) override
	{
		LOG_FUNCTION();
		return 0;
	}
	virtual void SetForceNonDevMode(const bool bValue) override
	{
		LOG_FUNCTION();
	}
	virtual bool GetForceNonDevMode() const override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual bool WasInDevMode() const override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual bool IsDevMode() const override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual XDOM::IXMLDOMDocument* CreateXMLDocument() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual XmlNodeRef CreateXmlNode(const char* sNodeName = "") override
	{
		LOG_FUNCTION();
		return XmlNodeRef();
	}
	virtual XmlNodeRef LoadXmlFile(const char* sFilename) override
	{
		LOG_FUNCTION();
		return XmlNodeRef();
	}
	virtual XmlNodeRef LoadXmlFromString(const char* sXmlString) override
	{
		LOG_FUNCTION();
		return XmlNodeRef();
	}
	virtual void SetViewCamera(CCamera& Camera) override
	{
		LOG_FUNCTION();
	}
	virtual CCamera& GetViewCamera() override
	{
		LOG_FUNCTION();
		// TODO: insert return statement here
	}
	virtual void CreateEntityScriptBinding(IEntity* pEntity) override
	{
		LOG_FUNCTION();
	}
	virtual void IgnoreUpdates(bool bIgnore) override
	{
		LOG_FUNCTION();
	}
	virtual void SetGCFrequency(const float fRate) override
	{
		LOG_FUNCTION();
	}
	virtual void SetIProcess(IProcess* process) override
	{
		LOG_FUNCTION();
	}
	virtual IProcess* GetIProcess() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual IRenderer* CreateRenderer(bool fullscreen, void* hinst, void* hWndAttach = 0) override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual bool IsTestMode() const override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual void ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason) override
	{
		LOG_FUNCTION();
	}
	virtual void SetFrameProfiler(bool on, bool display, char* prefix) override
	{
		LOG_FUNCTION();
	}
	virtual void StartProfilerSection(CFrameProfilerSection* pProfileSection) override
	{
		LOG_FUNCTION();
	}
	virtual void EndProfilerSection(CFrameProfilerSection* pProfileSection) override
	{
		LOG_FUNCTION();
	}
	virtual void VTuneResume() override
	{
		LOG_FUNCTION();
	}
	virtual void VTunePause() override
	{
		LOG_FUNCTION();
	}
	virtual void Deltree(const char* szFolder, bool bRecurse) override
	{
		LOG_FUNCTION();
	}
	virtual const SFileVersion& GetFileVersion() override
	{
		LOG_FUNCTION();
		// TODO: insert return statement here
	}
	virtual const SFileVersion& GetProductVersion() override
	{
		LOG_FUNCTION();
		// TODO: insert return statement here
	}
	virtual bool WriteCompressedFile(char* filename, void* data, unsigned int bitlen) override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual unsigned int ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen) override
	{
		LOG_FUNCTION();
		return 0;
	}
	virtual bool GetSSFileInfo(const char* inszFileName, char* outszInfo, const DWORD indwBufferSize) override
	{
		LOG_FUNCTION();
		return false;
	}
	virtual IDataProbe* GetIDataProbe() override
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual void SaveConfiguration() override
	{
		LOG_FUNCTION();
	}
	virtual void LoadConfiguration(const string& sFilename) override
	{
		LOG_FUNCTION();
	}
	virtual legacy::ESystemConfigSpec GetConfigSpec() override
	{
		LOG_FUNCTION();
		return legacy::ESystemConfigSpec();
	}
	virtual const char* GetUserNameA()
	{
		LOG_FUNCTION();
		return nullptr;
	}
	virtual unsigned int GetCompressedFileSizeA(char* filename)
	{
		LOG_FUNCTION();
		return 0;
	}
};
}
