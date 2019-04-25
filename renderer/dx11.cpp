#ifdef _WIN64

#include "dx11.h"
using namespace trigger::rend;

namespace
{
    dx11* app = 0;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return app->MsgProc(hwnd, msg, wParam, lParam);
}

int dx11::init()
{
    
    return 0;
}

void dx11::set_up()
{
}

int dx11::rendering()
{
    MSG msg = {0};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (this->engine->get_state() != trigger::core::engine_state::paused)
            {
                //DRAWSOMTING
            }
            else
            {
                Sleep(100);
            }
        }
    }

    return (int)msg.wParam;
}

void dx11::resize()
{
    //Check Resources
    assert(this->immediate_context);
    assert(this->device);
    assert(this->swap_chain);

    // Release old stuff.
    ReleaseCOM(this->render_target_view);
    ReleaseCOM(this->depth_stencil_view);
    ReleaseCOM(this->depth_stencil_buffer);

    (this->swap_chain->ResizeBuffers(1, this->mClientWidth, this->mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0)); 
    //importent
    ID3D11Texture2D* back_buffer;
    (this->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)));
    (this->device->CreateRenderTargetView(back_buffer, 0, &this->render_target_view));
    ReleaseCOM(back_buffer);

    D3D11_TEXTURE2D_DESC depth_desc;
    depth_desc.Width = this->mClientWidth;
    depth_desc.Height = this->mClientHeight;
    depth_desc.MipLevels = 1;
    depth_desc.ArraySize = 1;
    depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    if(this->enable_msaa)
    {
        depth_desc.SampleDesc.Count = 4;
        depth_desc.SampleDesc.Quality = this->app_Msaa_quality - 1;
    }
    else
    {
        depth_desc.SampleDesc.Count = 0;
        depth_desc.SampleDesc.Quality = 0;
    }

    this->device->CreateTexture2D(&depth_desc, 0, &this->depth_stencil_buffer);
    this->device->CreateDepthStencilView(this->depth_stencil_buffer, 0, &this->depth_stencil_view);
    
    this->immediate_context->OMSetRenderTargets(1, &this->render_target_view, this->depth_stencil_view);

    this->screen_viewport.TopLeftX = 0;
    this->screen_viewport.TopLeftY = 0;
    this->screen_viewport.Width = static_cast<float>(this->mClientWidth);
    this->screen_viewport.Height = static_cast<float>(this->mClientHeight);
    this->screen_viewport.MinDepth = 0.0f;
    //May can changed..?
    this->screen_viewport.MaxDepth = 1.0f;
    
    this->immediate_context->RSSetViewports(1, &this->screen_viewport);
    //TODO..
}

INSTANCE dx11::get_instance() const
{
    return this->app_instance;
}

HWND dx11::get_HWND() const
{
    return this->app_hwnd;
}

float dx11::get_aspect_ratio() const
{
    return static_cast<float>(this->mClientWidth / this->mClientHeight);
}

LRESULT dx11::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

dx11::~dx11()
{
    ReleaseCOM(this->render_target_view);
    ReleaseCOM(this->depth_stencil_view);
    ReleaseCOM(this->swap_chain);
    ReleaseCOM(this->depth_stencil_buffer);
    if (this->immediate_context)
    {
        this->immediate_context->ClearState();
    }
    ReleaseCOM(this->immediate_context);
    ReleaseCOM(this->device);
}

#endif