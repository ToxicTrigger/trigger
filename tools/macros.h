#ifndef MACROS_H
#define MACROS_H

#ifdef _WIN64
#define slash "\\"
#else
#define slash "/"
#endif

#ifdef _WIN64
    #define TRIGGER_API __declspec(dllexport)
    #define API_IMPORT __declspec(dllimport)
#else
    #define TRIGGER_API __attribute__((visibility("default")))
    #define API_IMPORT
#endif

#ifdef _WIN64
#define REND dx11
#define INSTANCE HINSTANCE
#define WH HWND
#else
#define REND vk
#define INSTANCE VkInstance
#define WH GLFWwindow
#endif

#endif