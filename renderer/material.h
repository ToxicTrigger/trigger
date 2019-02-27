#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/game/file.h"
#include "../core/game/mini_core.h"
#include <vector>
#include <tuple>
#include <memory>

#ifdef _WIN64
#include <windows.h>
#include <d3d12.h>
#include <d3dcompiler.h>
using namespace Microsoft::WRL;
#else

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
        bool is_init; 
    
    #ifdef _WIN64
    #define DATA ComPtr<ID3DBlob>
    #else
    #define DATA    
    #endif

    public:
        std::unique_ptr<trigger::core::file> path;
        DATA shader_data;
        shader_type type;

        shader(std::string shader_path, shader_type Type)
        {
            this->path = std::make_unique<trigger::core::file>(shader_path);
            this->type = Type;
            if(this->path->get_type() == trigger::core::file::type::hlsl)
            {
                this->is_init = load_shader();
            }
        };

        bool load_shader()
        {
            bool done = true;
#ifdef _WIN64
            UINT flag = 0;
            HRESULT hr = S_OK;
            DATA byte = nullptr;
            DATA error;
            if(this->type == shader_type::vert)
            {
                WCHAR buf[512];
                MultiByteToWideChar(CP_ACP, 0, this->path->get_path().c_str(), -1, buf, 512);
                std::wstring w_path(buf);

                hr = D3DCompileFromFile(w_path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", flag, 0, &byte, &error);
                if(error != nullptr) done = false;
                if(byte == nullptr) done = false;
                this->shader_data = byte;
            }
            else
            {
                hr = D3DCompileFromFile((LPCWSTR)this->path->get_path().c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", flag, 0, &byte, &error);
                if(error != nullptr) return false;
                this->shader_data = byte;
            }
#else

#endif
            return done;
        }

        bool const is_usable()
        {
            return this->is_init;
        }
    };

    class material
    {
    public:
        std::vector<shader> shaders;

        std::tuple<float, float, float, float> albedo;
        std::tuple<float, float, float> fresnel;
        float roughness;
    public:
        material()
        {
            this->shaders = std::vector<shader>();
            this->albedo = std::make_tuple(1.0f, 1.0f, 1.0f, 1.0f);
            this->fresnel = std::make_tuple(0.1f, 0.1f, 0.1f);
            roughness = 1.0f;
            
            this->shaders.push_back
            (
                trigger::rend::shader
                (
                    trigger::core::get_path("Assets/Shaders/trigger/Default.hlsl"), 
                    shader_type::vert
                )
            );
        }
    };
}

#endif