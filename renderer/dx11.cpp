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
    app = this;
    WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = this->app_instance;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "TriggerEngine";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, "RegisterClass Failed.", 0, 0);
		return -1;
	}
    RECT R = { 0, 0, mClientWidth, mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

    this->app_hwnd = CreateWindow(
        "TriggerEngine", this->title.c_str(), 
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0,0, 
    this->app_instance, 0);

    if(!this->app_hwnd)
    {
        MessageBox(0, "RegisterClass Failed.", 0, 0);
		return -2;
    }

    ShowWindow(this->app_hwnd, SW_SHOW);
    UpdateWindow(this->app_hwnd);
    return 0;
}

void dx11::set_up()
{
    D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
			0,                 // default adapter
			this->driver_type,
			0,                 // no software device
			0, 
			0, 0,              // default feature level array
			D3D11_SDK_VERSION,
			&this->device,
			&featureLevel,
			&this->immediate_context);
    if(FAILED(hr))
    {
        MessageBox(0, "D3D11 Create Device Failed.", 0, 0);
		return;
    }

    if(featureLevel != D3D_FEATURE_LEVEL_11_0)
    {
        MessageBox(0, "Direct3D Feature Level 11 unsupported.", 0, 0);
		return;
    }

    this->device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &this->app_Msaa_quality);
    assert(this->app_Msaa_quality > 0);

    DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    if(this->enable_msaa)
    {
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = this->app_Msaa_quality - 1;
    }
    else
    {
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }

    sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = this->app_hwnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

    IDXGIDevice* dxgiDevice = 0;
    this->device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
    IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(this->device, &sd, &this->swap_chain);
	
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	//resource init


	trigger::rend::material* mat = new trigger::rend::material();
	mat->add_shader("Assets/Shaders/Default.hlsl", shader_type::frag, this->device, this->fxs);
    this->resize();
}


void dx11::draw()
{
    this->immediate_context->ClearRenderTargetView(this->render_target_view, reinterpret_cast<const float*>(&trigger::colors::LightSteelBlue));
    this->immediate_context->ClearDepthStencilView(this->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    this->immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    this->swap_chain->Present(0,0);
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
                this->draw();
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
    switch(msg)
    {
        case WM_ACTIVATE:
            if(LOWORD(wParam) == WA_INACTIVE)
            {
                
            }
            else
            {

            }
            return 0;
        case WM_SIZE:
                this->mClientWidth = LOWORD(lParam);
                this->mClientWidth = HIWORD(lParam);
                if(this->device && this->engine->get_state() != trigger::core::engine_state::not_inited)
                {
                    this->resize();
                }
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

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