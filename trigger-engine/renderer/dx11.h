#ifdef _WIN64
#ifndef DX11_HEADER
#define DX11_HEADER

#include "renderer.h"
#include "../../lib/dx11/D3DX11Effect.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui.h>
#include "../core/imgui/imgui_impl_dx11.h"
#include "../core/imgui/imgui_impl_win32.h"
#include <d3d11.h>
#include <dxgi.h>
#include <cassert>
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include "../core/game/object_renderer.h"


namespace trigger
{
    namespace rend
    {
        class dx11 : public trigger::rend::renderer 
        {
        protected:
            INSTANCE    app_instance;
            HWND        app_hwnd;
			UINT		app_Msaa_quality;
			
			ID3D11Device*			device;
			ID3D11DeviceContext* 	immediate_context;
			IDXGISwapChain*			swap_chain;
			ID3D11Texture2D* 		depth_stencil_buffer;
			ID3D11RenderTargetView* render_target_view = nullptr;
			ID3D11DepthStencilView* depth_stencil_view = nullptr;
			D3D11_VIEWPORT			screen_viewport;

			EFFECT					fxs;
			ID3DX11EffectTechnique*	tech;
			ID3D11InputLayout*		input_layout;
			ID3D11Buffer*			vb_box;
			ID3D11Buffer*			ib_box;
			ID3DX11EffectMatrixVariable* mfxWorldViewProj;
			glm::mat4				proj;
			glm::mat4				view;
			glm::mat4				world;

			std::string title;
			D3D_DRIVER_TYPE driver_type;
			bool enable_msaa;
			
        public:
            virtual int init()      override;
            virtual void set_up()   override;
            virtual int rendering() override;
            virtual void resize()   override;
            virtual void draw()     override;

            void build_vertex_layout();
            void build_effects();
            void build_draw_object();
            
            virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			virtual ~dx11();

            dx11(INSTANCE inst, int width, int height, bool edit_mode, trigger::core::engine* engine) 
            : renderer(width, height, edit_mode, engine)
            {
                this->app_instance = inst;
				this->title = "Trigger Engine Dx11";
				this->driver_type = D3D_DRIVER_TYPE_HARDWARE;
				ZeroMemory(&this->screen_viewport , sizeof(D3D11_VIEWPORT));
                this->init();
                this->set_up();
            }

			INSTANCE 	get_instance()const;
			HWND		get_HWND()const;
			float		get_aspect_ratio() const;
        };
    }
}

#endif
#endif