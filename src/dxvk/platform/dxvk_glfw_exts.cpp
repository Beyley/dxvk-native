#include "../dxvk_platform_exts.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace dxvk {

    DxvkPlatformExts DxvkPlatformExts::s_instance;

    std::string_view DxvkPlatformExts::getName() {
        return "GLFW WSI";
    }

    DxvkNameSet DxvkPlatformExts::getInstanceExtensions() {
        if (!glfwVulkanSupported()) 
            throw DxvkError(str::format("GLFW WSI: Vulkan is not supported in any capacity!"));

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow* window = glfwCreateWindow(1, 1, "Dummy Window", nullptr, nullptr);
        
        if (window == nullptr)
            throw DxvkError(str::format("GLFW WSI: Unable to create dummy window"));
        
        uint32_t extensionCount = 0;
        const char** extensionArray = glfwGetRequiredInstanceExtensions(&extensionCount);

        if (extensionCount == 0)
            throw DxvkError(str::format("GLFW WSI: Failed to get required instance extensions"));
        
        DxvkNameSet names;
        for (int i = 0; i < extensionCount; ++i) { 
            names.add(extensionArray[i]);
        }

        glfwDestroyWindow(window);
        
        return names;
    }


    DxvkNameSet DxvkPlatformExts::getDeviceExtensions(
            uint32_t      adapterId) {
        return DxvkNameSet();
    }


    void DxvkPlatformExts::initInstanceExtensions() {

    }


    void DxvkPlatformExts::initDeviceExtensions(
            const DxvkInstance* instance) {

    }

}