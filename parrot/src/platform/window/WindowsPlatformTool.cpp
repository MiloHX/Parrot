#include "prpch.h"
#include "parrot/tool/PlatformTool.h"
#include "parrot/core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace parrot {

    std::string FileDialogs::openFile(const char* filter) {
        OPENFILENAMEA ofn;
        CHAR sz_file[260] = { 0 };

        // initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize  = sizeof(OPENFILENAME);
        ofn.hwndOwner    = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
        ofn.lpstrFile    = sz_file;
        ofn.nMaxFile     = sizeof(sz_file);
        ofn.lpstrFilter  = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }
        return std::string();
    }
    std::string FileDialogs::saveFile(const char* filter) {
        OPENFILENAMEA ofn;
        CHAR sz_file[260] = { 0 };

        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize  = sizeof(OPENFILENAME);
        ofn.hwndOwner    = glfwGetWin32Window((GLFWwindow*)Application::get().getWindow().getNativeWindow());
        ofn.lpstrFile    = sz_file;
        ofn.nMaxFile     = sizeof(sz_file);
        ofn.lpstrFilter  = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }
        return std::string();
    }
    
}