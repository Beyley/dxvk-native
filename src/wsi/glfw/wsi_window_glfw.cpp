#include "../wsi_window.h"

#include "wsi_helpers_glfw.h"

//#include <GLFW/glfw3.h>

#include <windows.h>
#include <wsi/native_wsi.h>

#include "../../util/util_string.h"
#include "../../util/log/log.h"

namespace dxvk::wsi {

    void getWindowSize(
            HWND      hWindow,
            uint32_t* pWidth,
            uint32_t* pHeight) {
        GLFWwindow* window = fromHwnd(hWindow);

        int32_t w, h;
        glfwGetWindowSize(window, &w, &h);

        if (pWidth)
            *pWidth = uint32_t(w);

        if (pHeight)
            *pHeight = uint32_t(h);
    }


    void resizeWindow(
            HWND             hWindow,
            DxvkWindowState* pState,
            uint32_t         Width,
            uint32_t         Height) {
        GLFWwindow* window = fromHwnd(hWindow);

        glfwSetWindowSize(window, int32_t(Width), int32_t(Height));
    }


    bool setWindowMode(
            HMONITOR         hMonitor,
            HWND             hWindow,
            const WsiMode*   pMode,
            bool             EnteringFullscreen) {
        const int32_t displayId    = fromHmonitor(hMonitor);
        GLFWwindow* window         = fromHwnd(hWindow);
        
        if (!isDisplayValid(displayId))
            return false;

        int32_t displayCount = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&displayCount);
        GLFWmonitor* monitor = monitors[displayId];
        
        GLFWvidmode wantedMode = { };
        wantedMode.width = pMode->width;
        wantedMode.height = pMode->height;
        wantedMode.refreshRate = pMode->refreshRate.numerator != 0
                                 ? pMode->refreshRate.numerator / pMode->refreshRate.denominator
                                 : 0;
        // TODO: Implement lookup format for bitsPerPixel here.
        
        glfwSetWindowMonitor(window, monitor, 0, 0, wantedMode.width, wantedMode.width, wantedMode.refreshRate);

        return true;
    }
    
    bool enterFullscreenMode(
            HMONITOR         hMonitor,
            HWND             hWindow,
            DxvkWindowState* pState,
            bool             ModeSwitch) {
        const int32_t displayId    = fromHmonitor(hMonitor);
        GLFWwindow* window         = fromHwnd(hWindow);

        if (!isDisplayValid(displayId))
            return false;
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        auto videoMode = glfwGetVideoMode(monitor);
        
        // TODO: Set this on the correct monitor.
        // Docs aren't clear on this...
        glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);

        return true;
    }


    bool leaveFullscreenMode(
            HWND             hWindow,
            DxvkWindowState* pState) {
        GLFWwindow* window = fromHwnd(hWindow);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        auto videoMode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, nullptr, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);

        return true;
    }


    bool restoreDisplayMode(HMONITOR hMonitor) {
        const int32_t displayId = fromHmonitor(hMonitor);
        return isDisplayValid(displayId);
    }


    HMONITOR getWindowMonitor(HWND hWindow) {
        GLFWwindow* window      = fromHwnd(hWindow);
        const int32_t displayId = 0;

        return toHmonitor(displayId);
    }


    bool isWindow(HWND hWindow) {
        GLFWwindow* window = fromHwnd(hWindow);
        return window != nullptr;
    }

}