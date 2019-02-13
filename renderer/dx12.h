#include "renderer.h"

//DX12 can only be built on Windows64
#ifdef _WIN64
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

namespace trigger 
{
    namespace renderer
    {
        class dx12 : public trigger::abst::renderer
        {
        private:
            virtual void init()
            {
                std::cout << "init dx12  ... " << std::endl;
            } 

            virtual void set_up()
            {

            }

        public:
            dx12()
            {
                this->init();
            }
        };
    }
}


#endif