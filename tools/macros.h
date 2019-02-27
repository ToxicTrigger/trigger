#ifndef MACROS_H
#define MACROS_H

#ifdef _WIN64
#define slash "\\"

#else
#define slash "/"

#endif

#ifdef _WIN64
#define REND dx12
#define INSTANCE HINSTANCE
#define WH HWND
#else
#define REND vk
#define INSTANCE VkInstance
#define WH GLFWwindow
#endif

#endif