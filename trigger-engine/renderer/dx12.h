//DX12 can only be built on Windows64
#ifdef _WIN64

#ifndef DX12_H
#define DX12_H

#include "renderer.h"
#include "../lib/dx12/d3dx12.h"

#include <windows.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include <tchar.h>
#include <limits>
#include <iostream>
#include <WindowsX.h>
#include "../imgui/imgui.h"
#include "../imgui/examples/imgui_impl_dx12.h"
#include "../imgui/examples/imgui_impl_win32.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                  FenceValue;
};

// Data
static int const                    NUM_FRAMES_IN_FLIGHT = 3;
static FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
static UINT                         g_frameIndex = 0;

static int const                    NUM_BACK_BUFFERS = 3;
static ID3D12Device*                g_pd3dDevice = NULL;
static ID3D12DescriptorHeap*        g_pd3dRtvDescHeap = NULL;
static ID3D12DescriptorHeap*        g_pd3dSrvDescHeap = NULL;
static ID3D12CommandQueue*          g_pd3dCommandQueue = NULL;
static ID3D12GraphicsCommandList*   g_pd3dCommandList = NULL;
static ID3D12Fence*                 g_fence = NULL;
static HANDLE                       g_fenceEvent = NULL;
static UINT64                       g_fenceLastSignaledValue = 0;
static IDXGISwapChain3*             g_pSwapChain = NULL;
static HANDLE                       g_hSwapChainWaitableObject = NULL;
static ID3D12Resource*              g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

namespace trigger 
{
    namespace core
    {
        class engine;
    }
    namespace rend
    {
        class dx12 : public trigger::rend::renderer
        {
        private:
            static dx12 *app;
            HINSTANCE mhAppInst = nullptr;
            HWND      mhMainWnd = nullptr;
            std::string title;
	        bool      mAppPaused = false;  // is the application paused?
	        bool      mMinimized = false;  // is the application minimized?
	        bool      mMaximized = false;  // is the application maximized?
	        bool      mResizing = false;   // are the resize bars being dragged?
            bool      mFullscreenState = false;// fullscreen enabled

	        // Set true to use 4X MSAA (?.1.8).  The default is false.
	        bool      m4xMsaaState = false;    // 4X MSAA enabled
            UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

            Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
            
            Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
            Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;
            Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
            UINT64 mCurrentFence = 0;

            Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
            Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
            Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

            static const int SwapChainBufferCount = 2;
            int mCurrBackBuffer = 0;
            Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
            Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

            D3D12_VIEWPORT mScreenViewport;
            D3D12_RECT mScissorRect;

            UINT mRtvDescriptorSize = 0;
            UINT mDsvDescriptorSize = 0;
            UINT mCbvSrvUavDescriptorSize = 0;

            // Derived class should set these in derived constructor to customize starting values.
            std::wstring mMainWndCaption = L"Trigger Engine";
            D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
            DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
            DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
            
            const int gNumFrameResources = 3;

          private:
            virtual int init() override;
            virtual void set_up() override;
            virtual int rendering() override;
            virtual void resize() override;
            void CreateCommandObjects();
            void CreateSwapChain();
            void CreateRtvAndDsvDescriptorHeaps();
            D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
            void FlushCommandQueue();
            ID3D12Resource* CurrentBackBuffer() const;
            D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
            void draw_editors();
            void draw_dx12();

          public:
            dx12(INSTANCE inst, bool mode, std::string project_name, trigger::core::engine* engine) : renderer(1280, 680, mode, engine)
            {
                if(mode)
                {
                    this->mClientHeight = 900;
                    this->mClientWidth = 980;
                    this->title = "trigger-engine : dx12";
                }
                else
                {
                    this->title = project_name;
                }
                this->mhAppInst = inst;

                this->init();
                this->set_up();
                //this->resize();
                this->rendering();
            }

            static dx12* get_renderer();

            virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        };
    }
}

#endif
#endif