### Overview 
My hobby rendering engine. Mostly worked on it in mid-2021 and stopped since then.  
The code is a bit messy but the output is kind of pretty :3 Also decently fast for a CPU driven renderer.  
Note that all the 65 light sources in the demo below are casting **realtime shadows**. Nothing is baked.  

Demo running in 1080p with an i9-10980XE CPU + RX 6900 XT GPU.

https://user-images.githubusercontent.com/45799308/202927609-f58dbcd5-c1ef-42e8-a690-84de3320f810.mp4

### Can handle the scene instantiated 100 times at 120 FPS (1M shadow casters to cull)
![Scene x100](https://user-images.githubusercontent.com/45799308/202936621-72dbb1a5-f3ab-44ea-9805-cdc1b68f16cc.png)

### Can also handle the scene instantiated 1000 times, but only at 30 FPS (14M shadow casters to cull)
![Scene x1000](https://user-images.githubusercontent.com/45799308/202936624-8b73f305-6f37-4006-994d-1f90da263e67.png)

### Technical key points
- Vulkan backend
- PBR. Cook-Torrance BRDF
- Tiled deferred rendering. Per-tile local light GPU culling
- Directional light soft shadows. Single cascade, 3x3x3 PCF
- Directional light volumetric rendering. Raymarching, Henyey-Greenstein phase function, Beer's law
- Point light soft shadows. Omnidirectional shadow mapping, Multiview rendering, 3x3x3 PCF
- Simple post processing. SSAO, Bloom, Color Grading, Contrast, Saturation, Tone mapping
- Visibility culling (threaded and vectorized)
  - Point light culling. Frustum/Sphere test 
  - Point light shadow casters culling. Sphere/AABB test 
  - Directional light shadow casters culling. Orthographic Frustum/AABB test
  - Camera frustum culling. Frustum/AABB test
- PVS for manual occlusion culling
- GPU frame buffering. Triple buffering by default
- Batched rendering. One instanced draw call per material/mesh combo
- ECS module. Similar to the one in Unity DOTS
- Threaded native command buffer recording
- Lock-free job scheduler with support for transient allocations
- VMath. My dependency free vector-math library. Includes notably:
  - A bare bones SIMD math layer using __m128 and friends directly. Based on DirectXMath
  - An easier to use but less efficient layer based on the SIMD one. Includes notably:
    - Vector2/3/4 (float/int/uint/bool), Matrix4x4, Quaternion, Plane
    - Vector Swizzling
    - Most of the utilities you typically need for 3D maths
- Vex (Vulkan Extended). My Vulkan centered library. Includes notably:
  - Device creation. Based on vk-bootstrap
  - Entrypoints loading directly from the drivers. Based on volk
  - Shader runtime compilation. Based on shaderc
  - Shader reflection. Based on SPIRV-Reflect
  - Various facilities
- Unity scene to json conversion

### Dependencies
- Vulkan 1.1
- [Sponza HDRP](https://github.com/Unity-Technologies/Classic-Sponza)
- [DirectXMath](https://github.com/microsoft/DirectXMath)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [EASTL](https://github.com/electronicarts/EASTL) (Basic utilities like containers and so on)
- [Vk-bootstrap](https://github.com/charles-lunarg/vk-bootstrap) (Vulkan device creation)
- [Volk](https://github.com/zeux/volk) (Vulkan entrypoints loading)
- [Shaderc](https://github.com/google/shaderc) (GLSL -> SPIR-V runtime shader compilation)
- [SPIRV-Reflect](https://github.com/KhronosGroup/SPIRV-Reflect) (Shader reflection)
- [GLFW](https://github.com/glfw/glfw) (Window and inputs handling)
- [SFML](https://github.com/SFML/SFML) It was used for window and input handling at some point. Now mostly just used for image loading. Should be replaced by [stb_image](https://github.com/nothings/stb)
- [Assimp](https://github.com/assimp/assimp) (Model loading)
- [Fmt](https://github.com/fmtlib/fmt) (String formatting)
- [JsonCpp](https://github.com/open-source-parsers/jsoncpp) (Json parsing)
