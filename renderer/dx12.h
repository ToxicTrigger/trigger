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

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace trigger 
{
    namespace renderer
    {
        class dx12 : public trigger::abst::renderer
        {
        private:
            static dx12 *app;
            HINSTANCE mhAppInst = nullptr;
            HWND      mhMainWnd = nullptr;
        private:
            virtual void init() override;
            virtual void set_up() override;
            virtual int rendering() override;

        public:
            dx12(INSTANCE inst) : renderer(1280, 680)
            {
                this->init();
                this->mhAppInst = inst;
                this->rendering();
            }

            static dx12* get_renderer();

            virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        };
    }
}

#endif
#endif