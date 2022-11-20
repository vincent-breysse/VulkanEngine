
#include <pch.hpp>
#include <string.h>
#include <Vex/Loader.hpp>

#ifdef _WIN32
typedef const char* LPCSTR;
typedef struct HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
#ifdef _WIN64
typedef __int64(__stdcall* FARPROC)(void);
#else
typedef int(__stdcall* FARPROC)(void);
#endif
#else
#    include <dlfcn.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    __declspec(dllimport) HMODULE __stdcall LoadLibraryA(LPCSTR);
    __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
#endif

#ifdef __cplusplus
}
#endif

PFN_vkGetInstanceProcAddr VexFindInstanceProcAddr()
{
    PFN_vkGetInstanceProcAddr fnGetProcAddress = nullptr;

#if defined(_WIN32)
    HMODULE module = LoadLibraryA("vulkan-1.dll");
    if (!module)
        return nullptr;

    // note: function pointer is cast through void function pointer to silence cast-function-type warning on gcc8
    fnGetProcAddress = (PFN_vkGetInstanceProcAddr)(void(*)(void))GetProcAddress(module, "vkGetInstanceProcAddr");
#elif defined(__APPLE__)
    void* module = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!module)
        module = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!module)
        module = dlopen("libMoltenVK.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!module)
        return nullptr;

    fnGetProcAddress = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr");
#else
    void* module = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!module)
        module = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    if (!module)
        return nullptr;

    fnGetProcAddress = (PFN_vkGetInstanceProcAddr)dlsym(module, "vkGetInstanceProcAddr");
#endif

    return fnGetProcAddress;
}

void VexInitProcTable(PFN_vkGetInstanceProcAddr fnGetProcAddr, VexProcTable* outTable)
{
    memset(outTable, 0, sizeof(VexProcTable));

#if defined(VK_VERSION_1_0)
    outTable->GetInstanceProcAddr = fnGetProcAddr;
#endif

    /* VEX_GENERATE_LOAD_GLOBAL */
#if defined(VK_VERSION_1_0)
    outTable->CreateInstance = (PFN_vkCreateInstance)fnGetProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
    outTable->EnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)fnGetProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceExtensionProperties");
    outTable->EnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)fnGetProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceLayerProperties");
#endif
#if defined(VK_VERSION_1_1)
    outTable->EnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)fnGetProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion");
#endif
    /* VEX_GENERATE_LOAD_GLOBAL */
}

void VexLoadInstanceProcTable(PFN_vkGetInstanceProcAddr fnGetProcAddr, VkInstance instance, VexProcTable* table)
{
    /* VEX_GENERATE_LOAD_INSTANCE */
#if defined(VK_VERSION_1_0)
    table->DestroyInstance = (PFN_vkDestroyInstance)fnGetProcAddr(instance, "vkDestroyInstance");
    table->EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)fnGetProcAddr(instance, "vkEnumeratePhysicalDevices");
    table->GetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)fnGetProcAddr(instance, "vkGetPhysicalDeviceFeatures");
    table->GetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceFormatProperties");
    table->GetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties");
    table->GetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceProperties");
    table->GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    table->GetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
    table->GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)fnGetProcAddr(instance, "vkGetDeviceProcAddr");
    table->CreateDevice = (PFN_vkCreateDevice)fnGetProcAddr(instance, "vkCreateDevice");
    table->EnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)fnGetProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
    table->EnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)fnGetProcAddr(instance, "vkEnumerateDeviceLayerProperties");
    table->GetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif
#if defined(VK_VERSION_1_1)
    table->EnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups)fnGetProcAddr(instance, "vkEnumeratePhysicalDeviceGroups");
    table->GetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)fnGetProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
    table->GetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)fnGetProcAddr(instance, "vkGetPhysicalDeviceProperties2");
    table->GetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2)fnGetProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2");
    table->GetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2)fnGetProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2");
    table->GetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)fnGetProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2");
    table->GetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)fnGetProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2");
    table->GetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)fnGetProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
    table->GetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalBufferProperties");
    table->GetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalFenceProperties");
    table->GetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
#endif
#if defined(VK_KHR_surface)
    table->DestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)fnGetProcAddr(instance, "vkDestroySurfaceKHR");
    table->GetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    table->GetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    table->GetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    table->GetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#endif
#if defined(VK_KHR_display)
    table->GetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceDisplayPropertiesKHR");
    table->GetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
    table->GetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)fnGetProcAddr(instance, "vkGetDisplayPlaneSupportedDisplaysKHR");
    table->GetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR)fnGetProcAddr(instance, "vkGetDisplayModePropertiesKHR");
    table->CreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR)fnGetProcAddr(instance, "vkCreateDisplayModeKHR");
    table->GetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR)fnGetProcAddr(instance, "vkGetDisplayPlaneCapabilitiesKHR");
    table->CreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR)fnGetProcAddr(instance, "vkCreateDisplayPlaneSurfaceKHR");
#endif
#if defined(VK_KHR_xlib_surface)
    table->CreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)fnGetProcAddr(instance, "vkCreateXlibSurfaceKHR");
    table->GetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif
#if defined(VK_KHR_xcb_surface)
    table->CreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)fnGetProcAddr(instance, "vkCreateXcbSurfaceKHR");
    table->GetPhysicalDeviceXcbPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif
#if defined(VK_KHR_wayland_surface)
    table->CreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR)fnGetProcAddr(instance, "vkCreateWaylandSurfaceKHR");
    table->GetPhysicalDeviceWaylandPresentationSupportKHR = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif
#if defined(VK_KHR_android_surface)
    table->CreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)fnGetProcAddr(instance, "vkCreateAndroidSurfaceKHR");
#endif
#if defined(VK_KHR_win32_surface)
    table->CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)fnGetProcAddr(instance, "vkCreateWin32SurfaceKHR");
    table->GetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif
#if defined(VK_EXT_debug_report)
    table->CreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)fnGetProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    table->DestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)fnGetProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    table->DebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)fnGetProcAddr(instance, "vkDebugReportMessageEXT");
#endif
#if defined(VK_GGP_stream_descriptor_surface)
    table->CreateStreamDescriptorSurfaceGGP = (PFN_vkCreateStreamDescriptorSurfaceGGP)fnGetProcAddr(instance, "vkCreateStreamDescriptorSurfaceGGP");
#endif
#if defined(VK_NV_external_memory_capabilities)
    table->GetPhysicalDeviceExternalImageFormatPropertiesNV = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
#endif
#if defined(VK_KHR_get_physical_device_properties2)
    table->GetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceFeatures2KHR");
    table->GetPhysicalDeviceProperties2KHR = (PFN_vkGetPhysicalDeviceProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceProperties2KHR");
    table->GetPhysicalDeviceFormatProperties2KHR = (PFN_vkGetPhysicalDeviceFormatProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2KHR");
    table->GetPhysicalDeviceImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2KHR");
    table->GetPhysicalDeviceQueueFamilyProperties2KHR = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
    table->GetPhysicalDeviceMemoryProperties2KHR = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2KHR");
    table->GetPhysicalDeviceSparseImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
#endif
#if defined(VK_NN_vi_surface)
    table->CreateViSurfaceNN = (PFN_vkCreateViSurfaceNN)fnGetProcAddr(instance, "vkCreateViSurfaceNN");
#endif
#if defined(VK_KHR_device_group_creation)
    table->EnumeratePhysicalDeviceGroupsKHR = (PFN_vkEnumeratePhysicalDeviceGroupsKHR)fnGetProcAddr(instance, "vkEnumeratePhysicalDeviceGroupsKHR");
#endif
#if defined(VK_KHR_external_memory_capabilities)
    table->GetPhysicalDeviceExternalBufferPropertiesKHR = (PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
#endif
#if defined(VK_KHR_external_semaphore_capabilities)
    table->GetPhysicalDeviceExternalSemaphorePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
#endif
#if defined(VK_NVX_device_generated_commands)
    table->GetPhysicalDeviceGeneratedCommandsPropertiesNVX = (PFN_vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX)fnGetProcAddr(instance, "vkGetPhysicalDeviceGeneratedCommandsPropertiesNVX");
#endif
#if defined(VK_EXT_direct_mode_display)
    table->ReleaseDisplayEXT = (PFN_vkReleaseDisplayEXT)fnGetProcAddr(instance, "vkReleaseDisplayEXT");
#endif
#if defined(VK_EXT_acquire_xlib_display)
    table->AcquireXlibDisplayEXT = (PFN_vkAcquireXlibDisplayEXT)fnGetProcAddr(instance, "vkAcquireXlibDisplayEXT");
    table->GetRandROutputDisplayEXT = (PFN_vkGetRandROutputDisplayEXT)fnGetProcAddr(instance, "vkGetRandROutputDisplayEXT");
#endif
#if defined(VK_EXT_display_surface_counter)
    table->GetPhysicalDeviceSurfaceCapabilities2EXT = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
#endif
#if defined(VK_KHR_external_fence_capabilities)
    table->GetPhysicalDeviceExternalFencePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
#endif
#if defined(VK_KHR_get_surface_capabilities2)
    table->GetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
    table->GetPhysicalDeviceSurfaceFormats2KHR = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormats2KHR");
#endif
#if defined(VK_KHR_get_display_properties2)
    table->GetPhysicalDeviceDisplayProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceDisplayProperties2KHR");
    table->GetPhysicalDeviceDisplayPlaneProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)fnGetProcAddr(instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
    table->GetDisplayModeProperties2KHR = (PFN_vkGetDisplayModeProperties2KHR)fnGetProcAddr(instance, "vkGetDisplayModeProperties2KHR");
    table->GetDisplayPlaneCapabilities2KHR = (PFN_vkGetDisplayPlaneCapabilities2KHR)fnGetProcAddr(instance, "vkGetDisplayPlaneCapabilities2KHR");
#endif
#if defined(VK_MVK_ios_surface)
    table->CreateIOSSurfaceMVK = (PFN_vkCreateIOSSurfaceMVK)fnGetProcAddr(instance, "vkCreateIOSSurfaceMVK");
#endif
#if defined(VK_MVK_macos_surface)
    table->CreateMacOSSurfaceMVK = (PFN_vkCreateMacOSSurfaceMVK)fnGetProcAddr(instance, "vkCreateMacOSSurfaceMVK");
#endif
#if defined(VK_EXT_debug_utils)
    table->SetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)fnGetProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");
    table->SetDebugUtilsObjectTagEXT = (PFN_vkSetDebugUtilsObjectTagEXT)fnGetProcAddr(instance, "vkSetDebugUtilsObjectTagEXT");
    table->QueueBeginDebugUtilsLabelEXT = (PFN_vkQueueBeginDebugUtilsLabelEXT)fnGetProcAddr(instance, "vkQueueBeginDebugUtilsLabelEXT");
    table->QueueEndDebugUtilsLabelEXT = (PFN_vkQueueEndDebugUtilsLabelEXT)fnGetProcAddr(instance, "vkQueueEndDebugUtilsLabelEXT");
    table->QueueInsertDebugUtilsLabelEXT = (PFN_vkQueueInsertDebugUtilsLabelEXT)fnGetProcAddr(instance, "vkQueueInsertDebugUtilsLabelEXT");
    table->CmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)fnGetProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT");
    table->CmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)fnGetProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT");
    table->CmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)fnGetProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT");
    table->CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)fnGetProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    table->DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)fnGetProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    table->SubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT)fnGetProcAddr(instance, "vkSubmitDebugUtilsMessageEXT");
#endif
#if defined(VK_EXT_sample_locations)
    table->GetPhysicalDeviceMultisamplePropertiesEXT = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)fnGetProcAddr(instance, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
#endif
#if defined(VK_EXT_calibrated_timestamps)
    table->GetPhysicalDeviceCalibrateableTimeDomainsEXT = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT)fnGetProcAddr(instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
#endif
#if defined(VK_FUCHSIA_imagepipe_surface)
    table->CreateImagePipeSurfaceFUCHSIA = (PFN_vkCreateImagePipeSurfaceFUCHSIA)fnGetProcAddr(instance, "vkCreateImagePipeSurfaceFUCHSIA");
#endif
#if defined(VK_EXT_metal_surface)
    table->CreateMetalSurfaceEXT = (PFN_vkCreateMetalSurfaceEXT)fnGetProcAddr(instance, "vkCreateMetalSurfaceEXT");
#endif
#if defined(VK_NV_cooperative_matrix)
    table->GetPhysicalDeviceCooperativeMatrixPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)fnGetProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#endif
#if defined(VK_NV_coverage_reduction_mode)
    table->GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)fnGetProcAddr(instance, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
#endif
#if defined(VK_EXT_full_screen_exclusive)
    table->GetPhysicalDeviceSurfacePresentModes2EXT = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)fnGetProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif
#if defined(VK_EXT_headless_surface)
    table->CreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT)fnGetProcAddr(instance, "vkCreateHeadlessSurfaceEXT");
#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
    table->GetPhysicalDevicePresentRectanglesKHR = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)fnGetProcAddr(instance, "vkGetPhysicalDevicePresentRectanglesKHR");
#endif
    /* VEX_GENERATE_LOAD_INSTANCE */
}

void VexLoadDeviceProcTable(PFN_vkGetDeviceProcAddr fnGetProcAddr, VkDevice device, VexProcTable* table)
{
    /* VEX_GENERATE_LOAD_DEVICE */
#if defined(VK_VERSION_1_0)
    table->DestroyDevice = (PFN_vkDestroyDevice)fnGetProcAddr(device, "vkDestroyDevice");
    table->GetDeviceQueue = (PFN_vkGetDeviceQueue)fnGetProcAddr(device, "vkGetDeviceQueue");
    table->QueueSubmit = (PFN_vkQueueSubmit)fnGetProcAddr(device, "vkQueueSubmit");
    table->QueueWaitIdle = (PFN_vkQueueWaitIdle)fnGetProcAddr(device, "vkQueueWaitIdle");
    table->DeviceWaitIdle = (PFN_vkDeviceWaitIdle)fnGetProcAddr(device, "vkDeviceWaitIdle");
    table->AllocateMemory = (PFN_vkAllocateMemory)fnGetProcAddr(device, "vkAllocateMemory");
    table->FreeMemory = (PFN_vkFreeMemory)fnGetProcAddr(device, "vkFreeMemory");
    table->MapMemory = (PFN_vkMapMemory)fnGetProcAddr(device, "vkMapMemory");
    table->UnmapMemory = (PFN_vkUnmapMemory)fnGetProcAddr(device, "vkUnmapMemory");
    table->FlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)fnGetProcAddr(device, "vkFlushMappedMemoryRanges");
    table->InvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)fnGetProcAddr(device, "vkInvalidateMappedMemoryRanges");
    table->GetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)fnGetProcAddr(device, "vkGetDeviceMemoryCommitment");
    table->BindBufferMemory = (PFN_vkBindBufferMemory)fnGetProcAddr(device, "vkBindBufferMemory");
    table->BindImageMemory = (PFN_vkBindImageMemory)fnGetProcAddr(device, "vkBindImageMemory");
    table->GetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)fnGetProcAddr(device, "vkGetBufferMemoryRequirements");
    table->GetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)fnGetProcAddr(device, "vkGetImageMemoryRequirements");
    table->GetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)fnGetProcAddr(device, "vkGetImageSparseMemoryRequirements");
    table->QueueBindSparse = (PFN_vkQueueBindSparse)fnGetProcAddr(device, "vkQueueBindSparse");
    table->CreateFence = (PFN_vkCreateFence)fnGetProcAddr(device, "vkCreateFence");
    table->DestroyFence = (PFN_vkDestroyFence)fnGetProcAddr(device, "vkDestroyFence");
    table->ResetFences = (PFN_vkResetFences)fnGetProcAddr(device, "vkResetFences");
    table->GetFenceStatus = (PFN_vkGetFenceStatus)fnGetProcAddr(device, "vkGetFenceStatus");
    table->WaitForFences = (PFN_vkWaitForFences)fnGetProcAddr(device, "vkWaitForFences");
    table->CreateSemaphore = (PFN_vkCreateSemaphore)fnGetProcAddr(device, "vkCreateSemaphore");
    table->DestroySemaphore = (PFN_vkDestroySemaphore)fnGetProcAddr(device, "vkDestroySemaphore");
    table->CreateEvent = (PFN_vkCreateEvent)fnGetProcAddr(device, "vkCreateEvent");
    table->DestroyEvent = (PFN_vkDestroyEvent)fnGetProcAddr(device, "vkDestroyEvent");
    table->GetEventStatus = (PFN_vkGetEventStatus)fnGetProcAddr(device, "vkGetEventStatus");
    table->SetEvent = (PFN_vkSetEvent)fnGetProcAddr(device, "vkSetEvent");
    table->ResetEvent = (PFN_vkResetEvent)fnGetProcAddr(device, "vkResetEvent");
    table->CreateQueryPool = (PFN_vkCreateQueryPool)fnGetProcAddr(device, "vkCreateQueryPool");
    table->DestroyQueryPool = (PFN_vkDestroyQueryPool)fnGetProcAddr(device, "vkDestroyQueryPool");
    table->GetQueryPoolResults = (PFN_vkGetQueryPoolResults)fnGetProcAddr(device, "vkGetQueryPoolResults");
    table->CreateBuffer = (PFN_vkCreateBuffer)fnGetProcAddr(device, "vkCreateBuffer");
    table->DestroyBuffer = (PFN_vkDestroyBuffer)fnGetProcAddr(device, "vkDestroyBuffer");
    table->CreateBufferView = (PFN_vkCreateBufferView)fnGetProcAddr(device, "vkCreateBufferView");
    table->DestroyBufferView = (PFN_vkDestroyBufferView)fnGetProcAddr(device, "vkDestroyBufferView");
    table->CreateImage = (PFN_vkCreateImage)fnGetProcAddr(device, "vkCreateImage");
    table->DestroyImage = (PFN_vkDestroyImage)fnGetProcAddr(device, "vkDestroyImage");
    table->GetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)fnGetProcAddr(device, "vkGetImageSubresourceLayout");
    table->CreateImageView = (PFN_vkCreateImageView)fnGetProcAddr(device, "vkCreateImageView");
    table->DestroyImageView = (PFN_vkDestroyImageView)fnGetProcAddr(device, "vkDestroyImageView");
    table->CreateShaderModule = (PFN_vkCreateShaderModule)fnGetProcAddr(device, "vkCreateShaderModule");
    table->DestroyShaderModule = (PFN_vkDestroyShaderModule)fnGetProcAddr(device, "vkDestroyShaderModule");
    table->CreatePipelineCache = (PFN_vkCreatePipelineCache)fnGetProcAddr(device, "vkCreatePipelineCache");
    table->DestroyPipelineCache = (PFN_vkDestroyPipelineCache)fnGetProcAddr(device, "vkDestroyPipelineCache");
    table->GetPipelineCacheData = (PFN_vkGetPipelineCacheData)fnGetProcAddr(device, "vkGetPipelineCacheData");
    table->MergePipelineCaches = (PFN_vkMergePipelineCaches)fnGetProcAddr(device, "vkMergePipelineCaches");
    table->CreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)fnGetProcAddr(device, "vkCreateGraphicsPipelines");
    table->CreateComputePipelines = (PFN_vkCreateComputePipelines)fnGetProcAddr(device, "vkCreateComputePipelines");
    table->DestroyPipeline = (PFN_vkDestroyPipeline)fnGetProcAddr(device, "vkDestroyPipeline");
    table->CreatePipelineLayout = (PFN_vkCreatePipelineLayout)fnGetProcAddr(device, "vkCreatePipelineLayout");
    table->DestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)fnGetProcAddr(device, "vkDestroyPipelineLayout");
    table->CreateSampler = (PFN_vkCreateSampler)fnGetProcAddr(device, "vkCreateSampler");
    table->DestroySampler = (PFN_vkDestroySampler)fnGetProcAddr(device, "vkDestroySampler");
    table->CreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)fnGetProcAddr(device, "vkCreateDescriptorSetLayout");
    table->DestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)fnGetProcAddr(device, "vkDestroyDescriptorSetLayout");
    table->CreateDescriptorPool = (PFN_vkCreateDescriptorPool)fnGetProcAddr(device, "vkCreateDescriptorPool");
    table->DestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)fnGetProcAddr(device, "vkDestroyDescriptorPool");
    table->ResetDescriptorPool = (PFN_vkResetDescriptorPool)fnGetProcAddr(device, "vkResetDescriptorPool");
    table->AllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)fnGetProcAddr(device, "vkAllocateDescriptorSets");
    table->FreeDescriptorSets = (PFN_vkFreeDescriptorSets)fnGetProcAddr(device, "vkFreeDescriptorSets");
    table->UpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)fnGetProcAddr(device, "vkUpdateDescriptorSets");
    table->CreateFramebuffer = (PFN_vkCreateFramebuffer)fnGetProcAddr(device, "vkCreateFramebuffer");
    table->DestroyFramebuffer = (PFN_vkDestroyFramebuffer)fnGetProcAddr(device, "vkDestroyFramebuffer");
    table->CreateRenderPass = (PFN_vkCreateRenderPass)fnGetProcAddr(device, "vkCreateRenderPass");
    table->DestroyRenderPass = (PFN_vkDestroyRenderPass)fnGetProcAddr(device, "vkDestroyRenderPass");
    table->GetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)fnGetProcAddr(device, "vkGetRenderAreaGranularity");
    table->CreateCommandPool = (PFN_vkCreateCommandPool)fnGetProcAddr(device, "vkCreateCommandPool");
    table->DestroyCommandPool = (PFN_vkDestroyCommandPool)fnGetProcAddr(device, "vkDestroyCommandPool");
    table->ResetCommandPool = (PFN_vkResetCommandPool)fnGetProcAddr(device, "vkResetCommandPool");
    table->AllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)fnGetProcAddr(device, "vkAllocateCommandBuffers");
    table->FreeCommandBuffers = (PFN_vkFreeCommandBuffers)fnGetProcAddr(device, "vkFreeCommandBuffers");
    table->BeginCommandBuffer = (PFN_vkBeginCommandBuffer)fnGetProcAddr(device, "vkBeginCommandBuffer");
    table->EndCommandBuffer = (PFN_vkEndCommandBuffer)fnGetProcAddr(device, "vkEndCommandBuffer");
    table->ResetCommandBuffer = (PFN_vkResetCommandBuffer)fnGetProcAddr(device, "vkResetCommandBuffer");
    table->CmdBindPipeline = (PFN_vkCmdBindPipeline)fnGetProcAddr(device, "vkCmdBindPipeline");
    table->CmdSetViewport = (PFN_vkCmdSetViewport)fnGetProcAddr(device, "vkCmdSetViewport");
    table->CmdSetScissor = (PFN_vkCmdSetScissor)fnGetProcAddr(device, "vkCmdSetScissor");
    table->CmdSetLineWidth = (PFN_vkCmdSetLineWidth)fnGetProcAddr(device, "vkCmdSetLineWidth");
    table->CmdSetDepthBias = (PFN_vkCmdSetDepthBias)fnGetProcAddr(device, "vkCmdSetDepthBias");
    table->CmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)fnGetProcAddr(device, "vkCmdSetBlendConstants");
    table->CmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)fnGetProcAddr(device, "vkCmdSetDepthBounds");
    table->CmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)fnGetProcAddr(device, "vkCmdSetStencilCompareMask");
    table->CmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)fnGetProcAddr(device, "vkCmdSetStencilWriteMask");
    table->CmdSetStencilReference = (PFN_vkCmdSetStencilReference)fnGetProcAddr(device, "vkCmdSetStencilReference");
    table->CmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)fnGetProcAddr(device, "vkCmdBindDescriptorSets");
    table->CmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)fnGetProcAddr(device, "vkCmdBindIndexBuffer");
    table->CmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)fnGetProcAddr(device, "vkCmdBindVertexBuffers");
    table->CmdDraw = (PFN_vkCmdDraw)fnGetProcAddr(device, "vkCmdDraw");
    table->CmdDrawIndexed = (PFN_vkCmdDrawIndexed)fnGetProcAddr(device, "vkCmdDrawIndexed");
    table->CmdDrawIndirect = (PFN_vkCmdDrawIndirect)fnGetProcAddr(device, "vkCmdDrawIndirect");
    table->CmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)fnGetProcAddr(device, "vkCmdDrawIndexedIndirect");
    table->CmdDispatch = (PFN_vkCmdDispatch)fnGetProcAddr(device, "vkCmdDispatch");
    table->CmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)fnGetProcAddr(device, "vkCmdDispatchIndirect");
    table->CmdCopyBuffer = (PFN_vkCmdCopyBuffer)fnGetProcAddr(device, "vkCmdCopyBuffer");
    table->CmdCopyImage = (PFN_vkCmdCopyImage)fnGetProcAddr(device, "vkCmdCopyImage");
    table->CmdBlitImage = (PFN_vkCmdBlitImage)fnGetProcAddr(device, "vkCmdBlitImage");
    table->CmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)fnGetProcAddr(device, "vkCmdCopyBufferToImage");
    table->CmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)fnGetProcAddr(device, "vkCmdCopyImageToBuffer");
    table->CmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)fnGetProcAddr(device, "vkCmdUpdateBuffer");
    table->CmdFillBuffer = (PFN_vkCmdFillBuffer)fnGetProcAddr(device, "vkCmdFillBuffer");
    table->CmdClearColorImage = (PFN_vkCmdClearColorImage)fnGetProcAddr(device, "vkCmdClearColorImage");
    table->CmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)fnGetProcAddr(device, "vkCmdClearDepthStencilImage");
    table->CmdClearAttachments = (PFN_vkCmdClearAttachments)fnGetProcAddr(device, "vkCmdClearAttachments");
    table->CmdResolveImage = (PFN_vkCmdResolveImage)fnGetProcAddr(device, "vkCmdResolveImage");
    table->CmdSetEvent = (PFN_vkCmdSetEvent)fnGetProcAddr(device, "vkCmdSetEvent");
    table->CmdResetEvent = (PFN_vkCmdResetEvent)fnGetProcAddr(device, "vkCmdResetEvent");
    table->CmdWaitEvents = (PFN_vkCmdWaitEvents)fnGetProcAddr(device, "vkCmdWaitEvents");
    table->CmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)fnGetProcAddr(device, "vkCmdPipelineBarrier");
    table->CmdBeginQuery = (PFN_vkCmdBeginQuery)fnGetProcAddr(device, "vkCmdBeginQuery");
    table->CmdEndQuery = (PFN_vkCmdEndQuery)fnGetProcAddr(device, "vkCmdEndQuery");
    table->CmdResetQueryPool = (PFN_vkCmdResetQueryPool)fnGetProcAddr(device, "vkCmdResetQueryPool");
    table->CmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)fnGetProcAddr(device, "vkCmdWriteTimestamp");
    table->CmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)fnGetProcAddr(device, "vkCmdCopyQueryPoolResults");
    table->CmdPushConstants = (PFN_vkCmdPushConstants)fnGetProcAddr(device, "vkCmdPushConstants");
    table->CmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)fnGetProcAddr(device, "vkCmdBeginRenderPass");
    table->CmdNextSubpass = (PFN_vkCmdNextSubpass)fnGetProcAddr(device, "vkCmdNextSubpass");
    table->CmdEndRenderPass = (PFN_vkCmdEndRenderPass)fnGetProcAddr(device, "vkCmdEndRenderPass");
    table->CmdExecuteCommands = (PFN_vkCmdExecuteCommands)fnGetProcAddr(device, "vkCmdExecuteCommands");
#endif
#if defined(VK_VERSION_1_1)
    table->BindBufferMemory2 = (PFN_vkBindBufferMemory2)fnGetProcAddr(device, "vkBindBufferMemory2");
    table->BindImageMemory2 = (PFN_vkBindImageMemory2)fnGetProcAddr(device, "vkBindImageMemory2");
    table->GetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures)fnGetProcAddr(device, "vkGetDeviceGroupPeerMemoryFeatures");
    table->CmdSetDeviceMask = (PFN_vkCmdSetDeviceMask)fnGetProcAddr(device, "vkCmdSetDeviceMask");
    table->CmdDispatchBase = (PFN_vkCmdDispatchBase)fnGetProcAddr(device, "vkCmdDispatchBase");
    table->GetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2)fnGetProcAddr(device, "vkGetImageMemoryRequirements2");
    table->GetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2)fnGetProcAddr(device, "vkGetBufferMemoryRequirements2");
    table->GetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2)fnGetProcAddr(device, "vkGetImageSparseMemoryRequirements2");
    table->TrimCommandPool = (PFN_vkTrimCommandPool)fnGetProcAddr(device, "vkTrimCommandPool");
    table->GetDeviceQueue2 = (PFN_vkGetDeviceQueue2)fnGetProcAddr(device, "vkGetDeviceQueue2");
    table->CreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion)fnGetProcAddr(device, "vkCreateSamplerYcbcrConversion");
    table->DestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion)fnGetProcAddr(device, "vkDestroySamplerYcbcrConversion");
    table->CreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate)fnGetProcAddr(device, "vkCreateDescriptorUpdateTemplate");
    table->DestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate)fnGetProcAddr(device, "vkDestroyDescriptorUpdateTemplate");
    table->UpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate)fnGetProcAddr(device, "vkUpdateDescriptorSetWithTemplate");
    table->GetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport)fnGetProcAddr(device, "vkGetDescriptorSetLayoutSupport");
#endif
#if defined(VK_KHR_swapchain)
    table->CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)fnGetProcAddr(device, "vkCreateSwapchainKHR");
    table->DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)fnGetProcAddr(device, "vkDestroySwapchainKHR");
    table->GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)fnGetProcAddr(device, "vkGetSwapchainImagesKHR");
    table->AcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)fnGetProcAddr(device, "vkAcquireNextImageKHR");
    table->QueuePresentKHR = (PFN_vkQueuePresentKHR)fnGetProcAddr(device, "vkQueuePresentKHR");
#endif
#if defined(VK_KHR_display_swapchain)
    table->CreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR)fnGetProcAddr(device, "vkCreateSharedSwapchainsKHR");
#endif
#if defined(VK_EXT_debug_marker)
    table->DebugMarkerSetObjectTagEXT = (PFN_vkDebugMarkerSetObjectTagEXT)fnGetProcAddr(device, "vkDebugMarkerSetObjectTagEXT");
    table->DebugMarkerSetObjectNameEXT = (PFN_vkDebugMarkerSetObjectNameEXT)fnGetProcAddr(device, "vkDebugMarkerSetObjectNameEXT");
    table->CmdDebugMarkerBeginEXT = (PFN_vkCmdDebugMarkerBeginEXT)fnGetProcAddr(device, "vkCmdDebugMarkerBeginEXT");
    table->CmdDebugMarkerEndEXT = (PFN_vkCmdDebugMarkerEndEXT)fnGetProcAddr(device, "vkCmdDebugMarkerEndEXT");
    table->CmdDebugMarkerInsertEXT = (PFN_vkCmdDebugMarkerInsertEXT)fnGetProcAddr(device, "vkCmdDebugMarkerInsertEXT");
#endif
#if defined(VK_EXT_transform_feedback)
    table->CmdBindTransformFeedbackBuffersEXT = (PFN_vkCmdBindTransformFeedbackBuffersEXT)fnGetProcAddr(device, "vkCmdBindTransformFeedbackBuffersEXT");
    table->CmdBeginTransformFeedbackEXT = (PFN_vkCmdBeginTransformFeedbackEXT)fnGetProcAddr(device, "vkCmdBeginTransformFeedbackEXT");
    table->CmdEndTransformFeedbackEXT = (PFN_vkCmdEndTransformFeedbackEXT)fnGetProcAddr(device, "vkCmdEndTransformFeedbackEXT");
    table->CmdBeginQueryIndexedEXT = (PFN_vkCmdBeginQueryIndexedEXT)fnGetProcAddr(device, "vkCmdBeginQueryIndexedEXT");
    table->CmdEndQueryIndexedEXT = (PFN_vkCmdEndQueryIndexedEXT)fnGetProcAddr(device, "vkCmdEndQueryIndexedEXT");
    table->CmdDrawIndirectByteCountEXT = (PFN_vkCmdDrawIndirectByteCountEXT)fnGetProcAddr(device, "vkCmdDrawIndirectByteCountEXT");
#endif
#if defined(VK_NVX_image_view_handle)
    table->GetImageViewHandleNVX = (PFN_vkGetImageViewHandleNVX)fnGetProcAddr(device, "vkGetImageViewHandleNVX");
#endif
#if defined(VK_AMD_draw_indirect_count)
    table->CmdDrawIndirectCountAMD = (PFN_vkCmdDrawIndirectCountAMD)fnGetProcAddr(device, "vkCmdDrawIndirectCountAMD");
    table->CmdDrawIndexedIndirectCountAMD = (PFN_vkCmdDrawIndexedIndirectCountAMD)fnGetProcAddr(device, "vkCmdDrawIndexedIndirectCountAMD");
#endif
#if defined(VK_AMD_shader_info)
    table->GetShaderInfoAMD = (PFN_vkGetShaderInfoAMD)fnGetProcAddr(device, "vkGetShaderInfoAMD");
#endif
#if defined(VK_NV_external_memory_win32)
    table->GetMemoryWin32HandleNV = (PFN_vkGetMemoryWin32HandleNV)fnGetProcAddr(device, "vkGetMemoryWin32HandleNV");
#endif
#if defined(VK_KHR_device_group)
    table->GetDeviceGroupPeerMemoryFeaturesKHR = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)fnGetProcAddr(device, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
    table->CmdSetDeviceMaskKHR = (PFN_vkCmdSetDeviceMaskKHR)fnGetProcAddr(device, "vkCmdSetDeviceMaskKHR");
    table->CmdDispatchBaseKHR = (PFN_vkCmdDispatchBaseKHR)fnGetProcAddr(device, "vkCmdDispatchBaseKHR");
#endif
#if defined(VK_KHR_maintenance1)
    table->TrimCommandPoolKHR = (PFN_vkTrimCommandPoolKHR)fnGetProcAddr(device, "vkTrimCommandPoolKHR");
#endif
#if defined(VK_KHR_external_memory_win32)
    table->GetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR)fnGetProcAddr(device, "vkGetMemoryWin32HandleKHR");
    table->GetMemoryWin32HandlePropertiesKHR = (PFN_vkGetMemoryWin32HandlePropertiesKHR)fnGetProcAddr(device, "vkGetMemoryWin32HandlePropertiesKHR");
#endif
#if defined(VK_KHR_external_memory_fd)
    table->GetMemoryFdKHR = (PFN_vkGetMemoryFdKHR)fnGetProcAddr(device, "vkGetMemoryFdKHR");
    table->GetMemoryFdPropertiesKHR = (PFN_vkGetMemoryFdPropertiesKHR)fnGetProcAddr(device, "vkGetMemoryFdPropertiesKHR");
#endif
#if defined(VK_KHR_external_semaphore_win32)
    table->ImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR)fnGetProcAddr(device, "vkImportSemaphoreWin32HandleKHR");
    table->GetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR)fnGetProcAddr(device, "vkGetSemaphoreWin32HandleKHR");
#endif
#if defined(VK_KHR_external_semaphore_fd)
    table->ImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR)fnGetProcAddr(device, "vkImportSemaphoreFdKHR");
    table->GetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR)fnGetProcAddr(device, "vkGetSemaphoreFdKHR");
#endif
#if defined(VK_KHR_push_descriptor)
    table->CmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)fnGetProcAddr(device, "vkCmdPushDescriptorSetKHR");
#endif
#if defined(VK_EXT_conditional_rendering)
    table->CmdBeginConditionalRenderingEXT = (PFN_vkCmdBeginConditionalRenderingEXT)fnGetProcAddr(device, "vkCmdBeginConditionalRenderingEXT");
    table->CmdEndConditionalRenderingEXT = (PFN_vkCmdEndConditionalRenderingEXT)fnGetProcAddr(device, "vkCmdEndConditionalRenderingEXT");
#endif
#if defined(VK_KHR_descriptor_update_template)
    table->CreateDescriptorUpdateTemplateKHR = (PFN_vkCreateDescriptorUpdateTemplateKHR)fnGetProcAddr(device, "vkCreateDescriptorUpdateTemplateKHR");
    table->DestroyDescriptorUpdateTemplateKHR = (PFN_vkDestroyDescriptorUpdateTemplateKHR)fnGetProcAddr(device, "vkDestroyDescriptorUpdateTemplateKHR");
    table->UpdateDescriptorSetWithTemplateKHR = (PFN_vkUpdateDescriptorSetWithTemplateKHR)fnGetProcAddr(device, "vkUpdateDescriptorSetWithTemplateKHR");
#endif
#if defined(VK_NVX_device_generated_commands)
    table->CmdProcessCommandsNVX = (PFN_vkCmdProcessCommandsNVX)fnGetProcAddr(device, "vkCmdProcessCommandsNVX");
    table->CmdReserveSpaceForCommandsNVX = (PFN_vkCmdReserveSpaceForCommandsNVX)fnGetProcAddr(device, "vkCmdReserveSpaceForCommandsNVX");
    table->CreateIndirectCommandsLayoutNVX = (PFN_vkCreateIndirectCommandsLayoutNVX)fnGetProcAddr(device, "vkCreateIndirectCommandsLayoutNVX");
    table->DestroyIndirectCommandsLayoutNVX = (PFN_vkDestroyIndirectCommandsLayoutNVX)fnGetProcAddr(device, "vkDestroyIndirectCommandsLayoutNVX");
    table->CreateObjectTableNVX = (PFN_vkCreateObjectTableNVX)fnGetProcAddr(device, "vkCreateObjectTableNVX");
    table->DestroyObjectTableNVX = (PFN_vkDestroyObjectTableNVX)fnGetProcAddr(device, "vkDestroyObjectTableNVX");
    table->RegisterObjectsNVX = (PFN_vkRegisterObjectsNVX)fnGetProcAddr(device, "vkRegisterObjectsNVX");
    table->UnregisterObjectsNVX = (PFN_vkUnregisterObjectsNVX)fnGetProcAddr(device, "vkUnregisterObjectsNVX");
#endif
#if defined(VK_NV_clip_space_w_scaling)
    table->CmdSetViewportWScalingNV = (PFN_vkCmdSetViewportWScalingNV)fnGetProcAddr(device, "vkCmdSetViewportWScalingNV");
#endif
#if defined(VK_EXT_display_control)
    table->DisplayPowerControlEXT = (PFN_vkDisplayPowerControlEXT)fnGetProcAddr(device, "vkDisplayPowerControlEXT");
    table->RegisterDeviceEventEXT = (PFN_vkRegisterDeviceEventEXT)fnGetProcAddr(device, "vkRegisterDeviceEventEXT");
    table->RegisterDisplayEventEXT = (PFN_vkRegisterDisplayEventEXT)fnGetProcAddr(device, "vkRegisterDisplayEventEXT");
    table->GetSwapchainCounterEXT = (PFN_vkGetSwapchainCounterEXT)fnGetProcAddr(device, "vkGetSwapchainCounterEXT");
#endif
#if defined(VK_GOOGLE_display_timing)
    table->GetRefreshCycleDurationGOOGLE = (PFN_vkGetRefreshCycleDurationGOOGLE)fnGetProcAddr(device, "vkGetRefreshCycleDurationGOOGLE");
    table->GetPastPresentationTimingGOOGLE = (PFN_vkGetPastPresentationTimingGOOGLE)fnGetProcAddr(device, "vkGetPastPresentationTimingGOOGLE");
#endif
#if defined(VK_EXT_discard_rectangles)
    table->CmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)fnGetProcAddr(device, "vkCmdSetDiscardRectangleEXT");
#endif
#if defined(VK_EXT_hdr_metadata)
    table->SetHdrMetadataEXT = (PFN_vkSetHdrMetadataEXT)fnGetProcAddr(device, "vkSetHdrMetadataEXT");
#endif
#if defined(VK_KHR_create_renderpass2)
    table->CreateRenderPass2KHR = (PFN_vkCreateRenderPass2KHR)fnGetProcAddr(device, "vkCreateRenderPass2KHR");
    table->CmdBeginRenderPass2KHR = (PFN_vkCmdBeginRenderPass2KHR)fnGetProcAddr(device, "vkCmdBeginRenderPass2KHR");
    table->CmdNextSubpass2KHR = (PFN_vkCmdNextSubpass2KHR)fnGetProcAddr(device, "vkCmdNextSubpass2KHR");
    table->CmdEndRenderPass2KHR = (PFN_vkCmdEndRenderPass2KHR)fnGetProcAddr(device, "vkCmdEndRenderPass2KHR");
#endif
#if defined(VK_KHR_shared_presentable_image)
    table->GetSwapchainStatusKHR = (PFN_vkGetSwapchainStatusKHR)fnGetProcAddr(device, "vkGetSwapchainStatusKHR");
#endif
#if defined(VK_KHR_external_fence_win32)
    table->ImportFenceWin32HandleKHR = (PFN_vkImportFenceWin32HandleKHR)fnGetProcAddr(device, "vkImportFenceWin32HandleKHR");
    table->GetFenceWin32HandleKHR = (PFN_vkGetFenceWin32HandleKHR)fnGetProcAddr(device, "vkGetFenceWin32HandleKHR");
#endif
#if defined(VK_KHR_external_fence_fd)
    table->ImportFenceFdKHR = (PFN_vkImportFenceFdKHR)fnGetProcAddr(device, "vkImportFenceFdKHR");
    table->GetFenceFdKHR = (PFN_vkGetFenceFdKHR)fnGetProcAddr(device, "vkGetFenceFdKHR");
#endif
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
    table->GetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)fnGetProcAddr(device, "vkGetAndroidHardwareBufferPropertiesANDROID");
    table->GetMemoryAndroidHardwareBufferANDROID = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)fnGetProcAddr(device, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif
#if defined(VK_EXT_sample_locations)
    table->CmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)fnGetProcAddr(device, "vkCmdSetSampleLocationsEXT");
#endif
#if defined(VK_KHR_get_memory_requirements2)
    table->GetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR)fnGetProcAddr(device, "vkGetImageMemoryRequirements2KHR");
    table->GetBufferMemoryRequirements2KHR = (PFN_vkGetBufferMemoryRequirements2KHR)fnGetProcAddr(device, "vkGetBufferMemoryRequirements2KHR");
    table->GetImageSparseMemoryRequirements2KHR = (PFN_vkGetImageSparseMemoryRequirements2KHR)fnGetProcAddr(device, "vkGetImageSparseMemoryRequirements2KHR");
#endif
#if defined(VK_KHR_sampler_ycbcr_conversion)
    table->CreateSamplerYcbcrConversionKHR = (PFN_vkCreateSamplerYcbcrConversionKHR)fnGetProcAddr(device, "vkCreateSamplerYcbcrConversionKHR");
    table->DestroySamplerYcbcrConversionKHR = (PFN_vkDestroySamplerYcbcrConversionKHR)fnGetProcAddr(device, "vkDestroySamplerYcbcrConversionKHR");
#endif
#if defined(VK_KHR_bind_memory2)
    table->BindBufferMemory2KHR = (PFN_vkBindBufferMemory2KHR)fnGetProcAddr(device, "vkBindBufferMemory2KHR");
    table->BindImageMemory2KHR = (PFN_vkBindImageMemory2KHR)fnGetProcAddr(device, "vkBindImageMemory2KHR");
#endif
#if defined(VK_EXT_image_drm_format_modifier)
    table->GetImageDrmFormatModifierPropertiesEXT = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)fnGetProcAddr(device, "vkGetImageDrmFormatModifierPropertiesEXT");
#endif
#if defined(VK_EXT_validation_cache)
    table->CreateValidationCacheEXT = (PFN_vkCreateValidationCacheEXT)fnGetProcAddr(device, "vkCreateValidationCacheEXT");
    table->DestroyValidationCacheEXT = (PFN_vkDestroyValidationCacheEXT)fnGetProcAddr(device, "vkDestroyValidationCacheEXT");
    table->MergeValidationCachesEXT = (PFN_vkMergeValidationCachesEXT)fnGetProcAddr(device, "vkMergeValidationCachesEXT");
    table->GetValidationCacheDataEXT = (PFN_vkGetValidationCacheDataEXT)fnGetProcAddr(device, "vkGetValidationCacheDataEXT");
#endif
#if defined(VK_NV_shading_rate_image)
    table->CmdBindShadingRateImageNV = (PFN_vkCmdBindShadingRateImageNV)fnGetProcAddr(device, "vkCmdBindShadingRateImageNV");
    table->CmdSetViewportShadingRatePaletteNV = (PFN_vkCmdSetViewportShadingRatePaletteNV)fnGetProcAddr(device, "vkCmdSetViewportShadingRatePaletteNV");
    table->CmdSetCoarseSampleOrderNV = (PFN_vkCmdSetCoarseSampleOrderNV)fnGetProcAddr(device, "vkCmdSetCoarseSampleOrderNV");
#endif
#if defined(VK_NV_ray_tracing)
    table->CreateAccelerationStructureNV = (PFN_vkCreateAccelerationStructureNV)fnGetProcAddr(device, "vkCreateAccelerationStructureNV");
    table->DestroyAccelerationStructureNV = (PFN_vkDestroyAccelerationStructureNV)fnGetProcAddr(device, "vkDestroyAccelerationStructureNV");
    table->GetAccelerationStructureMemoryRequirementsNV = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)fnGetProcAddr(device, "vkGetAccelerationStructureMemoryRequirementsNV");
    table->BindAccelerationStructureMemoryNV = (PFN_vkBindAccelerationStructureMemoryNV)fnGetProcAddr(device, "vkBindAccelerationStructureMemoryNV");
    table->CmdBuildAccelerationStructureNV = (PFN_vkCmdBuildAccelerationStructureNV)fnGetProcAddr(device, "vkCmdBuildAccelerationStructureNV");
    table->CmdCopyAccelerationStructureNV = (PFN_vkCmdCopyAccelerationStructureNV)fnGetProcAddr(device, "vkCmdCopyAccelerationStructureNV");
    table->CmdTraceRaysNV = (PFN_vkCmdTraceRaysNV)fnGetProcAddr(device, "vkCmdTraceRaysNV");
    table->CreateRayTracingPipelinesNV = (PFN_vkCreateRayTracingPipelinesNV)fnGetProcAddr(device, "vkCreateRayTracingPipelinesNV");
    table->GetRayTracingShaderGroupHandlesNV = (PFN_vkGetRayTracingShaderGroupHandlesNV)fnGetProcAddr(device, "vkGetRayTracingShaderGroupHandlesNV");
    table->GetAccelerationStructureHandleNV = (PFN_vkGetAccelerationStructureHandleNV)fnGetProcAddr(device, "vkGetAccelerationStructureHandleNV");
    table->CmdWriteAccelerationStructuresPropertiesNV = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)fnGetProcAddr(device, "vkCmdWriteAccelerationStructuresPropertiesNV");
    table->CompileDeferredNV = (PFN_vkCompileDeferredNV)fnGetProcAddr(device, "vkCompileDeferredNV");
#endif
#if defined(VK_KHR_maintenance3)
    table->GetDescriptorSetLayoutSupportKHR = (PFN_vkGetDescriptorSetLayoutSupportKHR)fnGetProcAddr(device, "vkGetDescriptorSetLayoutSupportKHR");
#endif
#if defined(VK_KHR_draw_indirect_count)
    table->CmdDrawIndirectCountKHR = (PFN_vkCmdDrawIndirectCountKHR)fnGetProcAddr(device, "vkCmdDrawIndirectCountKHR");
    table->CmdDrawIndexedIndirectCountKHR = (PFN_vkCmdDrawIndexedIndirectCountKHR)fnGetProcAddr(device, "vkCmdDrawIndexedIndirectCountKHR");
#endif
#if defined(VK_EXT_external_memory_host)
    table->GetMemoryHostPointerPropertiesEXT = (PFN_vkGetMemoryHostPointerPropertiesEXT)fnGetProcAddr(device, "vkGetMemoryHostPointerPropertiesEXT");
#endif
#if defined(VK_AMD_buffer_marker)
    table->CmdWriteBufferMarkerAMD = (PFN_vkCmdWriteBufferMarkerAMD)fnGetProcAddr(device, "vkCmdWriteBufferMarkerAMD");
#endif
#if defined(VK_EXT_calibrated_timestamps)
    table->GetCalibratedTimestampsEXT = (PFN_vkGetCalibratedTimestampsEXT)fnGetProcAddr(device, "vkGetCalibratedTimestampsEXT");
#endif
#if defined(VK_NV_mesh_shader)
    table->CmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV)fnGetProcAddr(device, "vkCmdDrawMeshTasksNV");
    table->CmdDrawMeshTasksIndirectNV = (PFN_vkCmdDrawMeshTasksIndirectNV)fnGetProcAddr(device, "vkCmdDrawMeshTasksIndirectNV");
    table->CmdDrawMeshTasksIndirectCountNV = (PFN_vkCmdDrawMeshTasksIndirectCountNV)fnGetProcAddr(device, "vkCmdDrawMeshTasksIndirectCountNV");
#endif
#if defined(VK_NV_scissor_exclusive)
    table->CmdSetExclusiveScissorNV = (PFN_vkCmdSetExclusiveScissorNV)fnGetProcAddr(device, "vkCmdSetExclusiveScissorNV");
#endif
#if defined(VK_NV_device_diagnostic_checkpoints)
    table->CmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)fnGetProcAddr(device, "vkCmdSetCheckpointNV");
    table->GetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)fnGetProcAddr(device, "vkGetQueueCheckpointDataNV");
#endif
#if defined(VK_KHR_timeline_semaphore)
    table->GetSemaphoreCounterValueKHR = (PFN_vkGetSemaphoreCounterValueKHR)fnGetProcAddr(device, "vkGetSemaphoreCounterValueKHR");
    table->WaitSemaphoresKHR = (PFN_vkWaitSemaphoresKHR)fnGetProcAddr(device, "vkWaitSemaphoresKHR");
    table->SignalSemaphoreKHR = (PFN_vkSignalSemaphoreKHR)fnGetProcAddr(device, "vkSignalSemaphoreKHR");
#endif
#if defined(VK_INTEL_performance_query)
    table->InitializePerformanceApiINTEL = (PFN_vkInitializePerformanceApiINTEL)fnGetProcAddr(device, "vkInitializePerformanceApiINTEL");
    table->UninitializePerformanceApiINTEL = (PFN_vkUninitializePerformanceApiINTEL)fnGetProcAddr(device, "vkUninitializePerformanceApiINTEL");
    table->CmdSetPerformanceMarkerINTEL = (PFN_vkCmdSetPerformanceMarkerINTEL)fnGetProcAddr(device, "vkCmdSetPerformanceMarkerINTEL");
    table->CmdSetPerformanceStreamMarkerINTEL = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)fnGetProcAddr(device, "vkCmdSetPerformanceStreamMarkerINTEL");
    table->CmdSetPerformanceOverrideINTEL = (PFN_vkCmdSetPerformanceOverrideINTEL)fnGetProcAddr(device, "vkCmdSetPerformanceOverrideINTEL");
    table->AcquirePerformanceConfigurationINTEL = (PFN_vkAcquirePerformanceConfigurationINTEL)fnGetProcAddr(device, "vkAcquirePerformanceConfigurationINTEL");
    table->ReleasePerformanceConfigurationINTEL = (PFN_vkReleasePerformanceConfigurationINTEL)fnGetProcAddr(device, "vkReleasePerformanceConfigurationINTEL");
    table->QueueSetPerformanceConfigurationINTEL = (PFN_vkQueueSetPerformanceConfigurationINTEL)fnGetProcAddr(device, "vkQueueSetPerformanceConfigurationINTEL");
    table->GetPerformanceParameterINTEL = (PFN_vkGetPerformanceParameterINTEL)fnGetProcAddr(device, "vkGetPerformanceParameterINTEL");
#endif
#if defined(VK_AMD_display_native_hdr)
    table->SetLocalDimmingAMD = (PFN_vkSetLocalDimmingAMD)fnGetProcAddr(device, "vkSetLocalDimmingAMD");
#endif
#if defined(VK_EXT_buffer_device_address)
    table->GetBufferDeviceAddressEXT = (PFN_vkGetBufferDeviceAddressEXT)fnGetProcAddr(device, "vkGetBufferDeviceAddressEXT");
#endif
#if defined(VK_EXT_full_screen_exclusive)
    table->AcquireFullScreenExclusiveModeEXT = (PFN_vkAcquireFullScreenExclusiveModeEXT)fnGetProcAddr(device, "vkAcquireFullScreenExclusiveModeEXT");
    table->ReleaseFullScreenExclusiveModeEXT = (PFN_vkReleaseFullScreenExclusiveModeEXT)fnGetProcAddr(device, "vkReleaseFullScreenExclusiveModeEXT");
#endif
#if defined(VK_EXT_line_rasterization)
    table->CmdSetLineStippleEXT = (PFN_vkCmdSetLineStippleEXT)fnGetProcAddr(device, "vkCmdSetLineStippleEXT");
#endif
#if defined(VK_EXT_host_query_reset)
    table->ResetQueryPoolEXT = (PFN_vkResetQueryPoolEXT)fnGetProcAddr(device, "vkResetQueryPoolEXT");
#endif
#if defined(VK_KHR_pipeline_executable_properties)
    table->GetPipelineExecutablePropertiesKHR = (PFN_vkGetPipelineExecutablePropertiesKHR)fnGetProcAddr(device, "vkGetPipelineExecutablePropertiesKHR");
    table->GetPipelineExecutableStatisticsKHR = (PFN_vkGetPipelineExecutableStatisticsKHR)fnGetProcAddr(device, "vkGetPipelineExecutableStatisticsKHR");
    table->GetPipelineExecutableInternalRepresentationsKHR = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)fnGetProcAddr(device, "vkGetPipelineExecutableInternalRepresentationsKHR");
#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
    table->GetDeviceGroupPresentCapabilitiesKHR = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)fnGetProcAddr(device, "vkGetDeviceGroupPresentCapabilitiesKHR");
    table->GetDeviceGroupSurfacePresentModesKHR = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)fnGetProcAddr(device, "vkGetDeviceGroupSurfacePresentModesKHR");
#endif
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain))
    table->AcquireNextImage2KHR = (PFN_vkAcquireNextImage2KHR)fnGetProcAddr(device, "vkAcquireNextImage2KHR");
#endif
#if (defined(VK_KHR_push_descriptor) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor))
    table->CmdPushDescriptorSetWithTemplateKHR = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)fnGetProcAddr(device, "vkCmdPushDescriptorSetWithTemplateKHR");
#endif
#if (defined(VK_EXT_full_screen_exclusive) && defined(VK_KHR_device_group)) || (defined(VK_EXT_full_screen_exclusive) && defined(VK_VERSION_1_1))
    table->GetDeviceGroupSurfacePresentModes2EXT = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)fnGetProcAddr(device, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif
    /* VEX_GENERATE_LOAD_DEVICE */
}