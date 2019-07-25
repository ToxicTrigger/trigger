# Trigger Engine 
![Logo](https://github.com/ToxicTrigger/trigger/blob/master/Assets/Resource/Image/trigger.png)

## Version
>>[0.1.89](Unfortunate Orge) Lua Editor
>>![](https://github.com/ToxicTrigger/end-trigger/blob/Toxic/image/0.1.89.gif)

Trigger Engine 은 imgui 와 DX12 & Vulkan 을 결합한 게임 엔진 입니다.


아래의 작업을 진행 중 입니다. 

## Work process?

| Branch  |                mean                |
| :-----: | :--------------------------------: |
| master  |            Stable Build            |
| Nightly |           Nightly Build            |
|   test  |      Nightly Build unit test       |
|  Toxic  | un-stable & nightly & current work |

|    gitmoji     |          mean           |
| :------------: | :---------------------: |
|   :sparkles:   |       New stuff!        |
|   :arrow_up:   |      Updated code!      |
|     :bug:      |       Found Bug..       |
|    :wrench:    |      Resolved Bug!      |
|     :fire:      | Removing code or Files! |
| :construction: |    Work in progress.    |
|       🚿          |    code cleaning        |

# Todo list

- [ ] Attach Thread for All Engines Job ex) World Exporter & Editor & CPU DesHeap 
- [x] make Template Macro ex) TCLASS like UCLASS-unreal
- [x] make Save Variable Data Like .. SAVE_VAR(var_name) :: -> component.h 
- [ ] Add Mesh Renderer 
- [ ] Add Model importer
- [ ] Add Path Finding 
- [ ] Add Simple Collision Modeling ex) Box, Sphere, Mesh, Capsule ... 
- [ ] Add Simple Physics System
- [ ] Add Simple Shader & Material editor
- [ ] Add Simple Texture Loading 
- [ ] Add Simple Lighting 
- [ ] Add MSAA
- [ ] Add Simple Post-process effect ex) DoF, Blur , Bloom , Motion Blur .. 
- [ ] Add More Effective InputManager ex) use Observer Pattern
- [ ] Add Simple Terrain Engine 
- [ ] Add Tessellation Based LOD
- [ ] Add View debugger.
- [ ] Make it Buildable
- [ ] FSM , Animation  node Editor -> like BluePrint
- [ ] Use Imgui Dock
- [x] Implement Vulkan API 
- [x] Implement DirectX 12 API
- [ ] Shader Converter (hlsl <-> glsl <-> spri-v)
- [ ] Enable Mode
- [ ] Import Zinny & wget

# Build?
- Windows 10 + VS2017 + CMake + 64bit
  1. `Clone Project`
  2. `cd trigger`
  3. `mkdir build`
  4. `cd build`
  5. `cmake -G "Visual Studio 15 2017 Win64" ..`
  6. `cd .. `
  7. `cmake --build build --config Release`
  8. Check `trigger/build/Release/trigger.exe`
  ---

- Linux + c17 + CMake + VulkanSDK + 64bit
  1. `Clone Project`
  2. `cd trigger`
  3. `mkdir build`
  4. `cd build`
  5. `cmake ..`
  6. `cd .. `
  7. `cmake --build build --config Release`
  8. Check `trigger/build/Release/trigger`

  ---

- MacOS

  1. Install Vulkan + GLFW3 + glm
  2. Follow [Command Line](https://vulkan.lunarg.com/doc/sdk/1.1.92.1/mac/getting_started.html) setup!
  3. `clone project `
  4. `cd trigger`
  5. `cmake .`
  6. `cmake —build build —config Release`
  7. Check `build/trigger.app `

## Version Name

|      |      0      |      0       |      0       |
| :--: | :---------: | :----------: | :----------: |
| mean |   Stable    |   Nightly    | commit-count |
|  0   | Unfortunate |     Zin      |              |
|  1   |   Furious   |     ogre     |              |
|  2   |   Greedy    |    Troll     |              |
|  3   |    Heavy    |    Tiamat    |              |
|  4   |   Fearful   | Frankenstein |              |
|  5   |   Raging    |    Fenrir    |              |
|  6   |   Hallway   |    Siren     |              |
|  7   |   Awesome   |   Skeleton   |              |
|  8   |  Nonsense   |    Eredin    |              |
|  9   |  Immortal   |    Naiads    |              |



Thanks to
- ImGuiColorTextEdit https://github.com/BalazsJako/ImGuiColorTextEdit
- ImGuiFileDialog  https://github.com/aiekick/ImGuiFileDialog
- ImGui https://github.com/ocornut/imgui
- cpptoml https://github.com/skystrife/cpptoml
