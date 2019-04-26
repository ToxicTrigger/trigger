#ifdef _WIN64
#ifndef DX11_HEADER
#define DX11_HEADER

#include "renderer.h"
#include <d3d11.h>
#include <dxgi.h>
#include <cassert>
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { delete x; x = 0; }

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
			ID3D11RenderTargetView* render_target_view;
			ID3D11DepthStencilView* depth_stencil_view;
			D3D11_VIEWPORT			screen_viewport;

			std::string title;
			D3D_DRIVER_TYPE driver_type;
			bool enable_msaa;
			
        public:
            virtual int init()      override;
            virtual void set_up()   override;
            virtual int rendering() override;
            virtual void resize()   override;
            virtual void draw()     override;
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
                this->rendering();
            }

			INSTANCE 	get_instance()const;
			HWND		get_HWND()const;
			float		get_aspect_ratio() const;
        };
    }
}

#endif
#endif