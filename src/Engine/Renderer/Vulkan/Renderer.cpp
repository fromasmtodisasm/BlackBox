#include "Renderer.h"
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

#include <BlackBox/System/IProjectManager.hpp>
#include <BlackBox/System/IWindow.hpp>

CVKRenderer* gD3DRender;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

bool checkValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}
	return true;
}

VkDevice GetDevice()
{
	return CVKRenderer::GetDevice(gD3DRender);
}

CVKRenderer::CVKRenderer(ISystem* pSystem)
	: CRenderer(pSystem)
{
	gD3DRender = this;
}

CVKRenderer::~CVKRenderer()
{
	if (m_Device)
	{
		vkDestroyDevice(m_Device, nullptr);
		m_Device = VK_NULL_HANDLE;
	}

	if (enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(m_Instance, debugMessenger, nullptr);
	}
	vkDestroyInstance(m_Instance, nullptr);
}

void CVKRenderer::ShareResources(IRenderer* renderer)
{
}

void CVKRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
}

void CVKRenderer::PushProfileMarker(char* label)
{
}

void CVKRenderer::PopProfileMarker(char* label)
{
}

int CVKRenderer::CreateRenderTarget()
{
	return 0;
}

void CVKRenderer::DrawFullscreenQuad()
{
}

float CVKRenderer::GetDepthValue(int x, int y)
{
	return 0.0f;
}

void CVKRenderer::Sh_Reload()
{
}

bool CVKRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CVKRenderer::BeginFrame(void)
{
}

void CVKRenderer::Update(void)
{
}

void CVKRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
}

void CVKRenderer::SetViewport(int x, int y, int width, int height)
{
}

void CVKRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CVKRenderer::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

bool CVKRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CVKRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void CVKRenderer::DrawFullScreenImage(int texture_id)
{
}

int CVKRenderer::SetPolygonMode(int mode)
{
	return 0;
}

void CVKRenderer::ScreenShot(const char* filename)
{
}

void CVKRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void CVKRenderer::SetState(State state, bool enable)
{
}

void CVKRenderer::SetCullMode(CullMode mode)
{
}

bool CVKRenderer::DeleteContext(WIN_HWND hWnd)
{
	return false;
}

bool CVKRenderer::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX, int SSY)
{
	return false;
}

bool CVKRenderer::SetCurrentContext(WIN_HWND hWnd)
{
	return false;
}

void CVKRenderer::MakeMainContextActive()
{
}

WIN_HWND CVKRenderer::GetCurrentContextHWND()
{
	return m_hWnd;
}

bool CVKRenderer::IsCurrentContextMainVP()
{
	return false;
}

int CVKRenderer::GetCurrentContextViewportHeight() const
{
	return 0;
}

int CVKRenderer::GetCurrentContextViewportWidth() const
{
	return 0;
}

void CVKRenderer::SetClearColor(const Vec3& vColor)
{
}

void CVKRenderer::ClearDepthBuffer()
{
}

void CVKRenderer::ClearColorBuffer(const Vec3 vColor)
{
}

void CVKRenderer::SetRenderTarget(int nHandle)
{
}

bool CVKRenderer::InitOverride()
{
	VK_LOG("Initialize");

	bool retflag;
	bool retval = CreateInstance(retflag);
	if (retflag)
		return retval;
	setupDebugMessenger();

#ifdef VK_USE_PLATFORM_WIN32_KHR

	VkWin32SurfaceCreateInfoKHR surface_create_info = {
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		nullptr,
		0,
		GetModuleHandle(0),
		(HWND)m_hWnd};

	RETURN_B_IF_FAILED(vkCreateWin32SurfaceKHR(m_Instance, &surface_create_info, nullptr, &m_PresentationSurface), "Could not create surface");
#endif

	return CreateDevice();
}

bool CVKRenderer::CreateInstance(bool& retflag)
{
	retflag = true;
	uint32 extensions_count;
	RETURN_B_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr), "Could not get the number of Instance extensions.");

	std::vector<VkExtensionProperties> available_extensions(extensions_count);
	RETURN_B_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]), "Could not enumerate %d instance extensions", extensions_count);

	VK_LOG("Dump %d extensions", extensions_count);
	for (const auto& ext : available_extensions)
	{
		VK_LOG("%s", ext.extensionName);
	}

	std::vector<char*> desired_extensions{
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#ifndef NDEBUG
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif // !NDEBUG
	};

#if 0
	auto ext_supported = [&]() -> auto
	{
		VkResult res = VK_SUCCESS;
		for (const auto ext : desired_extensions)
		{
			if (auto it = std::find(available_extensions.begin(), available_extensions.end(), [=](const char* str) {return strcmp(ext, str);}); 
				it == available_extensions.end())
			{
				VK_ERROR("Extension %s not supported", ext);
				res = VK_ERROR_UNKNOWN;
			}
		}
		return res;
	};
	RETURN_B_IF_FAILED(ext_supported(), "Extensions not supported");
#endif

	if (enableValidationLayers && !checkValidationLayerSupport())
	{
		VK_ERROR("Validation layers requested, but not available!");
		return false;
	}

	// Create instance
	{
		VkApplicationInfo application_info{};
		application_info.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		application_info.pNext				= nullptr;
		application_info.pApplicationName	= gEnv->pProjectManager->GetCurrentProjectName();
		application_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		application_info.pEngineName		= "BlackBox";
		application_info.engineVersion		= VK_MAKE_VERSION(0, 0, 1);

		VkInstanceCreateInfo instance_create_info{};
		instance_create_info.sType			  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pNext			  = nullptr;
		instance_create_info.flags			  = 0;
		instance_create_info.pApplicationInfo = &application_info;
		if (enableValidationLayers)
		{
			instance_create_info.enabledLayerCount	 = static_cast<uint32_t>(validationLayers.size());
			instance_create_info.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			instance_create_info.enabledLayerCount	 = 0;
			instance_create_info.ppEnabledLayerNames = nullptr;
		}
		instance_create_info.enabledExtensionCount	 = desired_extensions.size();
		instance_create_info.ppEnabledExtensionNames = desired_extensions.data();

		RETURN_B_IF_FAILED(vkCreateInstance(&instance_create_info, nullptr, &m_Instance), "Could not create VkInstance");
		VK_LOG("Instance Created");
	}
	retflag = false;
	return {};
}

bool CVKRenderer::OnResizeSwapchain(int newWidth, int newHeight)
{
	return false;
}

void CVKRenderer::PrintProperties(const VkPhysicalDeviceProperties& properties)
{
	//#define PRINT(msg, ...) VK_LOG("\t", msg, __VA_ARGS__)
	auto& p = properties;
	VK_LOG("Print properties for %s device", p.deviceName);
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		VK_LOG("This is discrete GPU");
	}
	VK_LOG("\tVendorID %d", p.vendorID);
	VK_LOG("\tDeviceID %d", p.deviceID);
	VK_LOG("Limits");
	PrintLimits(p.limits);
}

void CVKRenderer::PrintLimits(const VkPhysicalDeviceLimits& limits)
{
#define PRINT(limit) VK_LOG("\t$3" #limit " = %d", limits.limit)
	PRINT(maxImageDimension1D);
	PRINT(maxImageDimension2D);
	PRINT(maxImageDimension3D);
	PRINT(maxImageDimensionCube);
	PRINT(maxImageArrayLayers);
	PRINT(maxTexelBufferElements);
	PRINT(maxUniformBufferRange);
	PRINT(maxStorageBufferRange);
	PRINT(maxPushConstantsSize);
	PRINT(maxMemoryAllocationCount);
	PRINT(maxSamplerAllocationCount);
	//VkDeviceSize          bufferImageGranularity;
	//VkDeviceSize          sparseAddressSpaceSize;
	PRINT(maxBoundDescriptorSets);
	PRINT(maxPerStageDescriptorSamplers);
	PRINT(maxPerStageDescriptorUniformBuffers);
	PRINT(maxPerStageDescriptorStorageBuffers);
	PRINT(maxPerStageDescriptorSampledImages);
	PRINT(maxPerStageDescriptorStorageImages);
	PRINT(maxPerStageDescriptorInputAttachments);
	PRINT(maxPerStageResources);
	PRINT(maxDescriptorSetSamplers);
	PRINT(maxDescriptorSetUniformBuffers);
	PRINT(maxDescriptorSetUniformBuffersDynamic);
	PRINT(maxDescriptorSetStorageBuffers);
	PRINT(maxDescriptorSetStorageBuffersDynamic);
	PRINT(maxDescriptorSetSampledImages);
	PRINT(maxDescriptorSetStorageImages);
	PRINT(maxDescriptorSetInputAttachments);
	PRINT(maxVertexInputAttributes);
	PRINT(maxVertexInputBindings);
	PRINT(maxVertexInputAttributeOffset);
	PRINT(maxVertexInputBindingStride);
	PRINT(maxVertexOutputComponents);
	PRINT(maxTessellationGenerationLevel);
	PRINT(maxTessellationPatchSize);
	PRINT(maxTessellationControlPerVertexInputComponents);
	PRINT(maxTessellationControlPerVertexOutputComponents);
	PRINT(maxTessellationControlPerPatchOutputComponents);
	PRINT(maxTessellationControlTotalOutputComponents);
	PRINT(maxTessellationEvaluationInputComponents);
	PRINT(maxTessellationEvaluationOutputComponents);
	PRINT(maxGeometryShaderInvocations);
	PRINT(maxGeometryInputComponents);
	PRINT(maxGeometryOutputComponents);
	PRINT(maxGeometryOutputVertices);
	PRINT(maxGeometryTotalOutputComponents);
	PRINT(maxFragmentInputComponents);
	PRINT(maxFragmentOutputAttachments);
	PRINT(maxFragmentDualSrcAttachments);
	PRINT(maxFragmentCombinedOutputResources);
	PRINT(maxComputeSharedMemorySize);
	//uint32_t              maxComputeWorkGroupCount[3];
	PRINT(maxComputeWorkGroupInvocations);
	//uint32_t              maxComputeWorkGroupSize[3];
	PRINT(subPixelPrecisionBits);
	PRINT(subTexelPrecisionBits);
	PRINT(mipmapPrecisionBits);
	PRINT(maxDrawIndexedIndexValue);
	PRINT(maxDrawIndirectCount);

#undef PRINT
}

void CVKRenderer::PrintQueueFamilyProperties(const VkQueueFamilyProperties& properties)
{
}

bool CVKRenderer::CreateDevice()
{
	// Select device
	VkPhysicalDevice physical_device;
	std::vector<VkPhysicalDevice> available_devices;
	uint32 extensions_count{0};
	std::vector<VkExtensionProperties> available_extensions;

	VkPhysicalDeviceFeatures device_features;
	VkPhysicalDeviceProperties device_properties;

	std::vector<VkQueueFamilyProperties> queue_families;
	uint32 queue_familiy_index{0};

	{
		RETURN_IF_FALSE((available_devices = EnumeratePhysicalDevices()).size() != 0, "");
		physical_device = available_devices[0];
		RETURN_B_IF_FAILED(vkEnumerateDeviceExtensionProperties(available_devices[0], nullptr, &extensions_count, nullptr), "Could not get count of physical deivces Extension Properties");

		available_extensions.resize(extensions_count);
		RETURN_B_IF_FAILED(vkEnumerateDeviceExtensionProperties(available_devices[0], nullptr, &extensions_count, available_extensions.data()), "Could not enumerate physical deivces Extension Properties");

		VK_LOG("Dump %d device extensions", extensions_count);
		for (const auto& ext : available_extensions)
		{
			VK_LOG("$3%s", ext.extensionName);
		}

		{
			vkGetPhysicalDeviceFeatures(available_devices[0], &device_features);
			vkGetPhysicalDeviceProperties(available_devices[0], &device_properties);
			PrintProperties(device_properties);
		}

		uint32 queue_families_count{0};
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, nullptr);
		if (queue_families_count == 0)
		{
			VK_ERROR("Device has not queu");
			return false;
		}

		queue_families.resize(queue_families_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_families_count, &queue_families[0]);

		VkQueueFlags desired_capabilities{VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT};

		for (size_t i = 0; i < queue_families_count; i++)
		{
			auto family = queue_families[i];
			if ((family.queueCount > 0) && family.queueFlags & desired_capabilities)
			{
				queue_familiy_index = i;
				break;
			}
		}
		VkBool32 isSupported;
		if (vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, 0, m_PresentationSurface, &isSupported) != VkResult::VK_SUCCESS || isSupported == VK_FALSE)
		{
			VK_ERROR("Surface not supported");
			return false;
		}
	}

	std::vector<VkSurfaceFormatKHR> vk_surfaceFormats;
	{
		uint32_t vk_surfaceFormatsCount;
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_PresentationSurface, &vk_surfaceFormatsCount, nullptr) != VkResult::VK_SUCCESS)
		{
			VK_ERROR("failed to get surface formats count");
			return false;
		}

		vk_surfaceFormats.resize(vk_surfaceFormatsCount);
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, m_PresentationSurface, &vk_surfaceFormatsCount, vk_surfaceFormats.data()) != VkResult::VK_SUCCESS)
		{
			VK_ERROR("failed to get surface formats");
			return false;
		}
	}

	std::vector<const char*> extensions;
	for (const auto& ae : available_extensions)
	{
		extensions.push_back(ae.extensionName);
	}

	std::vector<QueueInfo> queue_infos;

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

	float priority = 1.f;
	queue_create_infos.push_back({VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
								  nullptr,
								  0,
								  queue_familiy_index,
								  1,
								  &priority});

	//queue_create_infos[0].

	VkDeviceCreateInfo device_create_info;
	device_create_info.sType				   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext				   = nullptr;
	device_create_info.flags				   = 0;
	device_create_info.queueCreateInfoCount	   = queue_create_infos.size();
	device_create_info.pQueueCreateInfos	   = &queue_create_infos[0];
	device_create_info.enabledLayerCount	   = 0;
	device_create_info.ppEnabledLayerNames	   = nullptr;
	device_create_info.enabledExtensionCount   = 0;		  //extensions.size();
	device_create_info.ppEnabledExtensionNames = nullptr; //&extensions[0];
	device_create_info.pEnabledFeatures		   = &device_features;

	RETURN_B_IF_FAILED(vkCreateDevice(physical_device, &device_create_info, nullptr, &m_Device), "Could not create device");

	vkGetDeviceQueue(m_Device, queue_familiy_index, 0, &m_GraphicsQueue);

	uint32 code[] = {
		1, 2, 3, 4, 5, 6};

#if 0
	VkShaderModuleCreateInfo shader_module_create_info;
	shader_module_create_info.sType	   = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_create_info.pNext	   = nullptr;
	shader_module_create_info.codeSize = 6 * sizeof(uint32);
	shader_module_create_info.pCode	   = code;
	shader_module_create_info.flags	   = 0;

	VkShaderModule shader;

	vkCreateShaderModule(m_Device, &shader_module_create_info, nullptr, &shader);
#endif

	return true;
}

void CVKRenderer::setupDebugMessenger()
{
	if (!enableValidationLayers)
		return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType		   = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType	   = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData	   = this; // Optional

	CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &debugMessenger);
}

VkResult CVKRenderer::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

inline VKAPI_ATTR VkBool32 VKAPI_CALL CVKRenderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	VK_ERROR("validation layer: %s", pCallbackData->pMessage);

	return VK_FALSE;
}

inline void CVKRenderer::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

std::vector<VkPhysicalDevice> CVKRenderer::EnumeratePhysicalDevices()
{
	uint32 devices_count{0};
	std::vector<VkPhysicalDevice> available_devices;
	RETURN_B_IF_FAILED(vkEnumeratePhysicalDevices(m_Instance, &devices_count, nullptr), "Could not get count of physical deivces");
	available_devices.resize(devices_count);
	RETURN_B_IF_FAILED(vkEnumeratePhysicalDevices(m_Instance, &devices_count, available_devices.data()), "Could not enumerate physical deivces");

	return available_devices;
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new CVKRenderer(pSystem);
}
