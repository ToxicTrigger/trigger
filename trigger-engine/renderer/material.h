#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/game/file.h"
#include "../core/game/mini_core.h"
#include <vector>
#include <tuple>
#include <memory>
#include <string>
#include <stdlib.h>

#include <vulkan/vulkan.h>

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
        trigger::core::file path;
        shader_type type;


        shader(std::string shader_path, shader_type Type)
        {

        };

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

		bool add_shader(std::string path, shader_type type)
		{
			shader* s = new shader(trigger::core::get_real_path(path), type);
			if (s == nullptr) return false;
			this->shaders.push_back(s);
			return true;
		}
    };
}

#endif