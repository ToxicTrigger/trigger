#ifndef RENDERER_H
#define RENDERER_H

#ifdef _WIN64
#define REND dx12
#define INSTANCE HINSTANCE
#define WH HWND
#else
#define REND vk
#define INSTANCE VkInstance
#define WH GLFWwindow
#endif

namespace trigger
{
    namespace abst
    {
        class renderer
        {
        private:
            virtual void init() = 0;
            virtual void set_up() = 0;
            virtual int rendering() = 0;

        public:
            renderer(int width, int height) : mClientWidth(width), mClientHeight(height)
            {};

            virtual ~renderer() {}
            int mClientWidth, mClientHeight;
        };
    } ;// namespace abst
} ;// namespace trigger

#endif