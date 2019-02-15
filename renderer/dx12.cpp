#ifdef _WIN64
#include "dx12.h"

using namespace trigger::renderer;

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    return trigger::renderer::dx12::get_renderer()->MsgProc(hwnd, msg, wp, lp);
};

trigger::renderer::dx12* trigger::renderer::dx12::app = nullptr;
trigger::renderer::dx12* trigger::renderer::dx12::get_renderer()
{
    return app;
}

LRESULT trigger::renderer::dx12::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
	{
    case WM_ACTIVATE:
        if(LOWORD(wParam) == WA_INACTIVE)
        {

        }
        return 0;

    case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


void dx12::init()
{
    std::cout << "init dx12  ... " << std::endl;

    if(this->app == nullptr) this->app = this;

    WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->mhAppInst;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBoxW(0, L"RegisterClass Failed.", 0, 0);
		return ;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mClientWidth, mClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	this->mhMainWnd = CreateWindow("MainWnd", this->title.c_str() , WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, this->mhAppInst, 0);

	if (!mhMainWnd)
	{
		MessageBoxW(0, L"CreateWindow Failed.", 0, 0);
		return ;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);

	return;
}

void dx12::set_up()
{
	CreateDXGIFactory1(IID_PPV_ARGS(&this->mdxgiFactory));
	HRESULT hardwareResult = D3D12CreateDevice( nullptr , D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&this->md3dDevice));
	this->md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->mFence));

	this->mRtvDescriptorSize = this->md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	this->mDsvDescriptorSize = this->md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	this->mCbvSrvUavDescriptorSize = this->md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = mBackBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 4;
	md3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels));

	m4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");

	CreateCommandObjects();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();
}

int dx12::rendering()
{
    MSG msg = {0};
    while(msg.message != WM_QUIT)
    {
        // If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
        else
        {
            //Game Time
        }
    }
    return (int)msg.wParam;
}
#endif