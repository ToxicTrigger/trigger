#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/game/file.h"
#include "../core/game/mini_core.h"
#include <vector>
#include <tuple>
#include <memory>
#include <string>

#ifdef _WIN64
#pragma comment(lib, "d3dx11.lib")
#include <windows.h>
#include <d3d11.h>
#include "../lib/dx11/D3DX11.h"
#include "../lib/dx11/D3DX11Effect.h"
#include <dxgi.h>
#include <d3dcompiler.h>
#else
#include <vulkan/vulkan.h>
#endif

namespace trigger::rend
{
    enum class shader_type
    {
        vert,
        frag
    };

    class shader
    {
    private:
        bool is_init = false; 

    public:
        std::unique_ptr<trigger::core::file> path;
        shader_type type;
		EFFECT fx;

        shader(std::string shader_path, shader_type Type, DEVICE d3d_device, EFFECT fxs)
        {
            this->path = std::make_unique<trigger::core::file>(shader_path);
            this->type = Type;
            if(this->path->get_type() == trigger::core::file::type::hlsl)
            {
                this->is_init = load_shader(d3d_device, fxs);
            }
        };

        bool load_shader(DEVICE d3d_device, EFFECT fxs)
        {
#ifdef _WIN64
			DWORD shader_flag = 0;
			ID3D10Blob* compiled_shader = 0;
			ID3D10Blob* compilation_msg = 0;
			HRESULT hr = D3DX11CompileFromFile((LPCSTR)this->path->get_path().c_str(), 0, 0, 0, "fx_5_0", shader_flag, 0, 0, &compiled_shader, &compilation_msg, 0);
			if (compilation_msg != 0)
			{
				MessageBoxA(0, (char*)compilation_msg->GetBufferPointer(), 0, 0);
				return false;
			}
			if (hr != S_OK)
			{
				MessageBoxA(0, "Can't Find File!", 0, 0);
				return false;
			}

			D3DX11CreateEffectFromMemory(compiled_shader->GetBufferPointer(), compiled_shader->GetBufferSize(), 0, d3d_device, &fxs);
			fx = fxs;
			ReleaseCOM(compiled_shader);
#else

#endif
            return true;
        }

        bool const is_usable()
        {
            return this->is_init;
        }
    };

    class material
    {
    public:
        std::vector<shader*> shaders;
        std::tuple<float, float, float, float> albedo;
        std::tuple<float, float, float> fresnel;
        float roughness;

    public:
        material()
        {
            this->shaders = std::vector<shader*>();
            this->albedo = std::make_tuple(1.0f, 1.0f, 1.0f, 1.0f);
            this->fresnel = std::make_tuple(0.1f, 0.1f, 0.1f);
            roughness = 1.0f;
        }

		bool add_shader(std::string path, shader_type type, DEVICE d3d_device, EFFECT fxs)
		{
			shader* s = new shader(trigger::core::get_real_path(path), type, d3d_device, fxs);
			if (s == nullptr) return false;
			this->shaders.push_back(s);
			return true;
		}
    };
}

#endif