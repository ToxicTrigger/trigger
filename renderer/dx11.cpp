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

	//ImGui INiT
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(this->app_hwnd);
		ImGui_ImplDX11_Init(this->device, this->immediate_context);
	}
	
	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
	
	this->build_draw_object();

    this->resize();
}

#include "../core/editor/impl_editor.h"
static ID3D11ShaderResourceView* texture_view = nullptr;
float tick = 0;
void dx11::draw()
{
	{
		vec3 pos(0.0f,3.0f, -10.0f);
		vec3 target(0.0f, 0.0f, 0.0f);
		vec3 up(0.0f, 1.0f, 0.0f);
		this->view = glm::lookAtLH(pos, target, up);
		this->world = glm::rotate(world, this->engine->editors->get_delta_time(), glm::vec3(0,0,1));
	}

	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	auto editors = this->engine->editors->get_objects<trigger::edit::impl_editor>();
	for (auto e : editors)
	{
		e->draw();
	}
	
	auto objs = this->engine->object->get_all();
	for (auto i : objs)
	{
		ImGui::Begin(i.second->name.c_str());
		ImGui::Text("%f", i.second->get_component<trigger::comp::object_renderer>()->time);
		ImGui::End();
	}

	this->immediate_context->ClearRenderTargetView(this->render_target_view, reinterpret_cast<const float*>(&trigger::colors::LightSteelBlue));
	this->immediate_context->ClearDepthStencilView(this->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	this->immediate_context->IASetInputLayout(this->input_layout);
	// TODO :: modify
	this->immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(trigger::math::vertex);
	UINT offset = 0;
	this->immediate_context->IASetVertexBuffers(0, 1, &this->vb_box, &stride, &offset);
	this->immediate_context->IASetIndexBuffer(this->ib_box, DXGI_FORMAT_R32_UINT, 0);
	
	glm::mat4 wvp =  proj *view * world;
	this->mfxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&wvp));

	D3DX11_TECHNIQUE_DESC techDesc;
	this->tech->GetDesc(&techDesc);
	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		this->tech->GetPassByIndex(i)->Apply(0, this->immediate_context);
		//box indices
		//TODO :: how i got my indices..
		D3D11_BUFFER_DESC indices;
		this->ib_box->GetDesc(&indices);
		this->immediate_context->DrawIndexed(indices.ByteWidth, 0, 0);
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    this->swap_chain->Present(1,0);
}

void trigger::rend::dx11::build_vertex_layout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	D3DX11_PASS_DESC passDesc;
	this->tech->GetPassByIndex(0)->GetDesc(&passDesc);
	this->device->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &this->input_layout);
}

void trigger::rend::dx11::build_effects()
{
	trigger::rend::material* mat = new trigger::rend::material();
	mat->add_shader("Assets/Shaders/color.hlsl", shader_type::frag, this->device, this->fxs);
	this->tech = mat->shaders[0]->fx->GetTechniqueByName("ColorTech");
	this->mfxWorldViewProj = mat->shaders[0]->fx->GetVariableByName("gWorldViewProj")->AsMatrix();
	this->build_vertex_layout();
}

void trigger::rend::dx11::build_draw_object()
{
	this->proj = glm::mat4(1.0f);
	this->world = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);

	auto tmp = new transform();
	auto rend = new trigger::comp::object_renderer();
	tmp->add_component(rend);	
	this->engine->object->add(tmp);

	// Create vertex buffer
	trigger::math::vertex vertices[] =
	{
		{vec3(-1.0f, -1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)},
		{vec3(0.0f, 1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)},
		{vec3(1.0f, -1.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)},
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(trigger::math::vertex) * ARRAYSIZE(vertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	this->device->CreateBuffer(&vbd, &vinitData, &this->vb_box);

	UINT indices[] = {
		// front face
		0, 1, 2,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	(this->device->CreateBuffer(&ibd, &iinitData, &this->ib_box));

	this->build_effects();
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

	ReleaseCOM(this->render_target_view);
	ReleaseCOM(this->depth_stencil_view);

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

	//TEST
	this->proj = glm::perspectiveFovLH(20.0f, static_cast<float>(mClientWidth), static_cast<float>(mClientHeight), 0.0f, 1000.0f);
	//this->proj = glm::perspective(glm::radians(60.0f), this->get_aspect_ratio(), 0.1f, 100.0f);
	//this->proj = glm::perspectiveFov(60.0f, (float)mClientWidth, (float)mClientHeight, 0.1f, 100.0f);
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
    return static_cast<float>(this->mClientWidth) / static_cast<float>(this->mClientHeight);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT dx11::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;

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
                this->mClientHeight = HIWORD(lParam);
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
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
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