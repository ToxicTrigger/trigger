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
#define DEVICE ID3D11Device*
#define EFFECT ID3DX11Effect*
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = 0; }
#else
#define REND vk
#define INSTANCE VkInstance
#define WH GLFWwindow
#define DEVICE 
#define EFFECT 
#endif

#endif