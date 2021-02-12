#pragma once
#include "../BaseRenderer.hpp"
#include <BlackBox/Renderer/Camera.hpp>
#ifdef BB_PLATFORM_WINDOWS
#	include <vulkan/vulkan_win32.h>
#endif
#include <vulkan/vulkan.hpp>
class CVKRenderer;
extern CVKRenderer* gD3DRender;

inline bool SUCCESS_VK(VkResult result)
{
	return result == VK_SUCCESS;
}

inline bool FAILED_VK(VkResult result)
{
	return result != VK_SUCCESS;
}

#define VK_LOG(fmt, ...) CryLog("[VK] " fmt, __VA_ARGS__)
#define VK_ERROR(fmt, ...) CryError("[VK] " fmt, __VA_ARGS__)

#define RETURN_B_IF_FAILED(result, msg, ...) \
	if (FAILED_VK((result)))                 \
	{                                        \
		VK_ERROR(msg, __VA_ARGS__);          \
		return {};                           \
	}

#define RETURN_IF_FALSE(result, msg, ...) \
	if (!(result))                        \
	{                                     \
		VK_ERROR(msg, __VA_ARGS__);       \
		return {};                        \
	}

#define VK_VERIFY(result, msg, ...) \
	if (!(result))                  \
	{                               \
		VK_ERROR(msg, __VA_ARGS__); \
	}

#define RETURN_IF_FAILED(result, msg, ...) \
	if (FAILED_VK((result)))               \
	{                                      \
		VK_ERROR(msg, __VA_ARGS__);        \
		return;                            \
	}

class CVKRenderer : public CRenderer
{
  public:
	//HRESULT InitCube();
	CVKRenderer(ISystem* pSystem);
	~CVKRenderer();
	// Inherited via CRenderer
	virtual void ShareResources(IRenderer* renderer) override;
	virtual void SetRenderCallback(IRenderCallback* pCallback) override;
	virtual void PushProfileMarker(char* label) override;
	virtual void PopProfileMarker(char* label) override;
	virtual int CreateRenderTarget() override;
	virtual void DrawFullscreenQuad() override;
	virtual float GetDepthValue(int x, int y) override;
	virtual void Sh_Reload() override;
	virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;
	virtual void BeginFrame(void) override;
	virtual void Update(void) override;
	virtual void GetViewport(int* x, int* y, int* width, int* height) override;
	virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) override;
	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;
	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
	virtual void DrawFullScreenImage(int texture_id) override;
	virtual int SetPolygonMode(int mode) override;
	virtual void ScreenShot(const char* filename = nullptr) override;
	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;
	virtual void SetState(State state, bool enable) override;
	virtual void SetCullMode(CullMode mode = CullMode::BACK) override;
	virtual bool DeleteContext(WIN_HWND hWnd) override;
	virtual bool CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) override;
	virtual bool SetCurrentContext(WIN_HWND hWnd) override;
	virtual void MakeMainContextActive() override;
	virtual WIN_HWND GetCurrentContextHWND() override;
	virtual bool IsCurrentContextMainVP() override;
	virtual int GetCurrentContextViewportHeight() const override;
	virtual int GetCurrentContextViewportWidth() const override;
	virtual void SetClearColor(const Vec3& vColor) override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearColorBuffer(const Vec3 vColor) override;
	virtual void SetRenderTarget(int nHandle) override;
	// Inherited via CRenderer
	virtual bool InitOverride() override;
	bool CreateInstance(bool& retflag);
	static auto GetDevice(IRenderer* pThis) { return static_cast<CVKRenderer*>(pThis)->m_Device; }

	bool OnResizeSwapchain(int newWidth, int newHeight);

	void PrintProperties(const VkPhysicalDeviceProperties& properties);
	void PrintLimits(const VkPhysicalDeviceLimits& limits);
	void PrintQueueFamilyProperties(const VkQueueFamilyProperties& properties);

	bool CreateDevice();

	void setupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	struct QueueInfo
	{
		uint32_t FamilyIndex;
		std::vector<float> Priorities;
	};

	//=====================================
	std::vector<VkPhysicalDevice> EnumeratePhysicalDevices();

  private:
	VkInstance m_Instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkDevice m_Device;
	VkQueue m_GraphicsQueue{VK_NULL_HANDLE};
	VkQueue m_ComputeQueue{VK_NULL_HANDLE};
};

VkDevice GetDevice();
