# Trigger Engine 

## Version
>>[0.1.89](Unfortunate Orge) Lua Editor
>>![](https://github.com/ToxicTrigger/end-trigger/blob/Toxic/image/0.1.89.gif)

Trigger Engine 은 imgui 와 DX12 를 결합한 게임 엔진 입니다.

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
|     :fire:     | Removing code or Files! |
| :construction: |    Work in progress.    |

# Todo list

- [ ] Attach Thread for All Engines Job ex) World Exporter & Editor & CPU DesHeap 
- [ ] make Template Macro ex) TCLASS like UCLASS-unreal
- [x] make Save Variable Data Like .. SAVE_VAR(var_name) :: -> component.h 
- [ ] parse Lua + engine-side editor ) build Scriptable API for access Core API
- [ ] make Default Lua Template :: -> trigger_lua.h & trigger/lua/lua_default
- [ ] Game Control Api for Lua Like .. move( actor , vec(0, 1, 0) * moveSpeed * deltaTime ) & rotation( actor , vec(0, 1, 0) * deltaTime )
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
- [ ] Dynamic Lighting like Day->Night 
- [ ] FSM , Animation  node Editor -> like BluePrint
- [ ] Use new Imgui Dock



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