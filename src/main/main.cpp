
#include <pch.hpp>
#include <gyn/job_scheduler.hpp>
#include <gyn/entity_chunk.hpp>
#include <gyn/entity_manager.hpp>
#include <gyn/entity.hpp>
#include <gyn/transform.hpp>
#include <gyn/time_manager.hpp>
#include <gyn/world.hpp>
#include <gyn/window.hpp>
#include <gyn/engine.hpp>
#include <gyn/log_system.hpp>
#include <gyn/unity.hpp>
#include <gyn/gpu.hpp>
#include <gyn/vulkan/gpu.hpp>
#include <gyn/graphics/graphics_engine.hpp>
#include <gyn/graphics/render_component.hpp>
#include <gyn/graphics/render_asset_manager.hpp>
#include <gyn/graphics/image2d.hpp>

constexpr int32 WindowWidth = 1920;
constexpr int32 WindowHeight = 1080;
constexpr float WindowAspectRatio = WindowWidth / float(WindowHeight);
constexpr float3 ViewerPos = float3(0, 5, 0);
constexpr float MoveSpeed = 5.0f;

namespace
{
    struct Firefly
    {
        Entity entity;
        float3 originalPosition;
        float pathRadius;
        float moveSpeed;
        float cosOffset;
        float sinOffset;
    };

    struct MainWorld : public World
    {
        MainWorld() = default;

        bool isFirstFrame = false;
        bool isReplaying = false;
        float replayStartTime = 0;
        ea::vector<Firefly> fireflies;
    };
}

static void ProcessInputs(MainWorld& world)
{
    auto& engine = *world.engine;
    auto& entityManager = world.entityManager;
    auto& global = world.global;
    auto& window = *engine.window;
    auto& keyboard = *engine.keyboard;
    auto& timeManager = *engine.time;

    if (world.isFirstFrame)
    {
        window.SetCursorMode(CursorMode::Normal);
        return;
    }

    window.PollEvents();

    Entity cameraEntity = entityManager.GetTheEntity(GetCameraComponentTypes());
    Transform transform(cameraEntity);
    uint2 windowSize = window.GetSize();
    float dt = Clamp(timeManager.GetDeltaTime(), 0.0f, 0.1f);

    if (global.isGameViewFocused)
    {
        if (keyboard.IsDown(KeyCode::Escape))
        {
            if (window.GetCursorMode() == CursorMode::Lock)
                window.SetCursorMode(CursorMode::Normal);
        }

        if (window.IsMouseButtonPressed(MouseButton::Left))
        {
            window.SetCursorMode(CursorMode::Lock);
        }

        if (keyboard.IsDown(KeyCode::F3))
        {
            if (world.isReplaying)
            {
                world.isReplaying = false;
                world.replayStartTime = 0;
            }
            else
            {
                world.isReplaying = true;
                world.replayStartTime = timeManager.GetElapsedTime();
            }
        }

        if (!world.isReplaying && window.GetCursorMode() == CursorMode::Lock)
        {
            constexpr float Sensitivity = 0.5f;

            float2 mouseAccel = window.GetMouseDelta() * Sensitivity;
            float mouseAccelLength = Vec2Length(mouseAccel);

            float2 mouseAccelDirection = Vec2Zero;
            if (mouseAccelLength > 0)
                mouseAccelDirection = mouseAccel / mouseAccelLength;

            float mouseVelLength = mouseAccelLength * dt * Sensitivity;

            float2 mouseVel = mouseAccelDirection * mouseVelLength;

            transform.Rotate(QuatAxisAngle(transform.Up(), mouseVel.x));
            transform.Rotate(QuatAxisAngle(transform.Right(), mouseVel.y));

            float moveFactor = dt * MoveSpeed;
            if (keyboard.IsPressed(KeyCode::W))
            {
                transform.Translate(transform.Forward() * moveFactor);
            }
            else if (keyboard.IsPressed(KeyCode::S))
            {
                transform.Translate(transform.Backward() * moveFactor);
            }
            if (keyboard.IsPressed(KeyCode::D))
            {
                transform.Translate(transform.Right() * moveFactor);
            }
            else if (keyboard.IsPressed(KeyCode::A))
            {
                transform.Translate(transform.Left() * moveFactor);
            }
        }
    }
    else
    {
        window.SetCursorMode(CursorMode::Normal);
    }

    if (world.isReplaying)
    {
        const TransformRecord& record = global.transformRecord;

        const int sampleCount = ToI32(record.positions.size());

        float maxTime = sampleCount * record.sampleFrequency;
        float time = timeManager.GetElapsedTime() - world.replayStartTime;

        if (time < maxTime)
        {
            float fsample = time / record.sampleFrequency;
            int prevIndex = Clamp(int(std::floor(fsample)), 0, sampleCount - 1);
            int nextIndex = Clamp(int(std::ceil(fsample)), 0, sampleCount - 1);

            float lerpFactor = fsample - prevIndex;

            float3 prevPosition = record.positions[prevIndex];
            Quaternion prevRotation = record.rotations[prevIndex];

            float3 nextPosition = record.positions[nextIndex];
            Quaternion nextRotation = record.rotations[nextIndex];

            float3 position = Vec3Lerp(prevPosition, nextPosition, lerpFactor);
            Quaternion rotation = QuatSlerp(prevRotation, nextRotation, lerpFactor);

            transform.SetPosition(position);
            transform.SetRotation(rotation);
        }
        else
        {
            world.isReplaying = false;
            world.replayStartTime = 0;
        }
    }
}

static void InitWorld(Engine& engine, MainWorld& world)
{
    auto& renderAssets = *engine.renderAssets;
    auto& entityManager = world.entityManager;
    auto& global = world.global;

    world.isFirstFrame = true;
    world.engine = &engine;
    world.pointLightCommon = CreatePointLightCommonData(renderAssets);
    world.entityManager = EntityManager(world);

    Entity cameraEntity = entityManager.CreateEntity(GetCameraComponentTypes());
    cameraEntity.SetComponent(Position(ViewerPos));
    cameraEntity.SetComponent(Rotation(QuatIdentity));
    cameraEntity.SetComponent(Scale(Vec3One));
    cameraEntity.SetComponent(Resolution(WindowWidth, WindowHeight));
    cameraEntity.SetComponent(PerspectiveProjection(60.f, 0.1f, 1000.f));

    constexpr uint32 PointLightCount = 64;

    for (uint32 i = 0; i < PointLightCount; ++i)
    {
        float3 position = float3(0, 0.2f, 0) + float3(RandNeg1Pos1() * 5.0f, Rand01() * 3.0f, RandNeg1Pos1() * 10.0f);
        float3 scale = 0.05f * float3(1, 1, 1);

        float3 color = Vec3Lerp(Vec3One * 0.2f, Vec3One * 1.0f, RandV301());
        float range = 2.0f;

        SharedPtr<Material> material = renderAssets.CreateUnlitMaterial(color, 1.0f);
        SharedPtr<Model> model = renderAssets.CreateModel(material, renderAssets.GetCubeMesh());

        PointLight light = {};
        light.color = color;
        light.intensity = 7.0f;
        light.range = range;
        light.shadowNear = 0.1f;
        light.shadowFar = range;

        Entity entity = CreateLightRenderEntity(entityManager,
            *engine.matMeshRefCounts,
            model,
            false,
            position,
            QuatIdentity,
            scale,
            light);

        Firefly firefly = {};
        firefly.entity = entity;
        firefly.originalPosition = position;
        firefly.pathRadius = Lerp(0.5f, 5.0f, Rand01());
        firefly.moveSpeed = Lerp(0.1f, 0.3f, Rand01());
        firefly.cosOffset = Lerp(0, Pi, Rand01());
        firefly.sinOffset = Lerp(0, Pi, Rand01());

        world.fireflies.push_back(firefly);
    }

    global.transformRecord = LoadRecord("record.json");
    global.ambient = Vec3One;
    global.ambientIntensity = 0.05f;
    global.sunLightEulerAngles = float3(100, 90, 0);
    global.sunLightColor = float3(1.0f, 1.0f, 1.0f);
    global.sunLightIntensity = 7.0f;
    global.useAO = true;
    global.useEmissive = true;
    global.debugBuffer = 0;
    global.useBloom = true;
    global.usePVS = true;
    global.castShadows = true;
    global.threadedCulling = true;
    global.exposure = 0.5f;
    global.ssaoKernel = CreateSSAOKernel();
    global.ssaoRadius = 0.25f;
    global.ssaoStrength = 1.0f;
    global.bloomColor = Vec3One;
    global.bloomIntensity = 2.0f;
    global.bloomBlurPass = 4;
    global.debugFloat4 = float4(0, 0, 0, 0);
    global.colorFilter = Vec3One;
    global.colorFilterIntensity = 1.0f;
    global.saturation = 10.0f;

    global.stats = SetupWorldFromUnityScene(world, "Sponza");
    global.stats.pointLightCount += PointLightCount;
    global.stats.rendererCount += PointLightCount;

    engine.time->StartTiming();
    InitGraphicsEngine(world);
}

static void UpdateEditor(MainWorld& world, uint32 frameIndex)
{
    GPU* gpu = world.engine->gpu.get();
    GlobalData& global = world.global;

    gpu->ImGuiNewFrame();

    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if (ImGui::CollapsingHeader("Controls"))
    {
        static const char* s_AOVNameArray[] = 
        { 
            "Rendering", 
            "Albedo", 
            "Normal", 
            "Metallic", 
            "Roughness", 
            "AO (No SSAO)", 
            "AO + SSAO", 
            "Light count per tile" 
        };
        static const char* s_CurrentAOV = s_AOVNameArray[global.debugBuffer];

        if (ImGui::BeginCombo("AOV", s_CurrentAOV))
        {
            for (int i = 0; i < IM_ARRAYSIZE(s_AOVNameArray); i++)
            {
                bool isSelected = s_CurrentAOV == s_AOVNameArray[i];

                if (ImGui::Selectable(s_AOVNameArray[i], isSelected))
                {
                    s_CurrentAOV = s_AOVNameArray[i];
                    global.debugBuffer = i;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::NewLine();

        /*ImGui::Text("Debug float");
        ImGui::SliderFloat("##Z0", &global.debugFloat4.x, 0, 0);
        ImGui::SliderFloat("##Z1", &global.debugFloat4.y, 0, 0);
        ImGui::SliderFloat("##Z2", &global.debugFloat4.z, 0, 0);
        ImGui::SliderFloat("##Z3", &global.debugFloat4.w, 0, 0);*/

        ImGui::Checkbox("Enable Ambient Occlusion", &global.useAO);
        ImGui::Checkbox("Enable Emissive", &global.useEmissive);
        ImGui::Checkbox("Enable Bloom", &global.useBloom);
        ImGui::Checkbox("Enable PVS Occlusion culling", &global.usePVS);
        ImGui::Checkbox("Cast Shadows", &global.castShadows);
        ImGui::Checkbox("Threaded Culling", &global.threadedCulling);

        ImGui::Text("Sun light direction");
        ImGui::SliderFloat3("##A0", &global.sunLightEulerAngles.x, 60, 120);

        ImGui::Text("Sun light color");
        ImGui::ColorEdit3("##A1", &global.sunLightColor.x);

        ImGui::Text("Sun light intensity");
        ImGui::SliderFloat("##A2", &global.sunLightIntensity, 0.0f, 100.0f);

        ImGui::Text("Ambient light color");
        ImGui::ColorEdit3("##A3", &global.ambient.x);

        ImGui::Text("Ambient light intensity");
        ImGui::SliderFloat("##A4", &global.ambientIntensity, 0.0f, 1.0f);

        ImGui::Text("");

        ImGui::Text("Exposure");
        ImGui::SliderFloat("##B0", &global.exposure, -10.0f, 20.f);

        ImGui::Text("Contrast");
        ImGui::SliderFloat("##B1", &global.contrast, -100, 100);

        ImGui::Text("Saturation");
        ImGui::SliderFloat("##B2", &global.saturation, -100, 100);

        ImGui::Text("Color filter");
        ImGui::ColorEdit3("##B3", &global.colorFilter.x);

        ImGui::Text("Color filter intesity");
        ImGui::SliderFloat("##B4", &global.colorFilterIntensity, 0, 10.f);

        ImGui::Text("SSAO Radius");
        ImGui::SliderFloat("##B5", &global.ssaoRadius, 0, 5);

        ImGui::Text("SSAO Strength");
        ImGui::SliderFloat("##B6", &global.ssaoStrength, 0, 10);

        ImGui::Text("Bloom color");
        ImGui::ColorEdit3("##B7", &global.bloomColor.x);

        ImGui::Text("Bloom intensity");
        ImGui::SliderFloat("##B8", &global.bloomIntensity, 0, 10);

        ImGui::Text("Bloom range");
        ImGui::SliderInt("##B9", &global.bloomBlurPass, 1, 16);
    }

    if (ImGui::CollapsingHeader("Infos", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const ImVec4 fpsTextColor(1, 1, 0, 1);
        const ImVec4 statsColor(0.2f, 0.8f, 1, 1);
        const ImVec4 infosColor(1.0f, 0.5f, 0.2f, 1);

        const TimeManager& timeManager = world.timeManager;
        const ea::span<const float> fpsSamples = timeManager.GetFPSSamples();
        const uint32 fpsCount = timeManager.GetFPS();
        const uint32 minFPS = timeManager.GetMinFPS();
        const uint32 maxFPS = timeManager.GetMaxFPS();
        const uint32 averageFPS = timeManager.GetAverageFPS();

        Entity cameraEntity = world.entityManager.GetTheEntity(GetCameraComponentTypes());
        float3 position = cameraEntity.GetComponent<Position>().val;
        Quaternion rotation = cameraEntity.GetComponent<Rotation>().val;
        PerspectiveProjection projection = cameraEntity.GetComponent<PerspectiveProjection>();
        uint2 resolution = cameraEntity.GetComponent<Resolution>().val;
        EntityID viewPVSEntityID = cameraEntity.GetComponent<PVSEntityID>().val;

        ImGui::TextColored(fpsTextColor, "FPS [%d]", fpsCount);
        ImGui::TextColored(fpsTextColor, "Min/Average/Max [%d, %d, %d]", minFPS, averageFPS, maxFPS);
        ImGui::PlotHistogram("", fpsSamples.data(), int(fpsSamples.size()), 0, nullptr, 1.f, maxFPS * 1.1f, { 500, 200 });

        if (global.stats.rendererCount < 1000)
        {
            ImGui::TextColored(statsColor, "Nb Entity [%d / %d]", global.stats.visibleRendererCount, global.stats.rendererCount);
        }
        else if (global.stats.rendererCount < 1'000'000)
        {
            float visibleEntityK = global.stats.visibleRendererCount / 1'000.0f;
            float entityK = global.stats.rendererCount / 1'000.0f;
            ImGui::TextColored(statsColor, "Nb Entity [%.2fk / %.2fk]", visibleEntityK, entityK);
        }
        else
        {
            float visibleEntityM = global.stats.visibleRendererCount / 1'000'000.0f;
            float entityM = global.stats.rendererCount / 1'000'000.0f;
            ImGui::TextColored(statsColor, "Nb Entity [%.3fM / %.3fM]", visibleEntityM, entityM);
        }

        if (global.stats.shadowCasterCount < 1000)
        {
            ImGui::TextColored(statsColor, "Nb Shadow casters [%d / %d]", global.stats.visibleShadowCasterCount, global.stats.shadowCasterCount);
        }
        else if (global.stats.shadowCasterCount < 1'000'000)
        {
            float visibleShadowCastersK = global.stats.visibleShadowCasterCount / 1'000.0f;
            float shadowCastersK = global.stats.shadowCasterCount / 1'000.0f;
            ImGui::TextColored(statsColor, "Nb Shadow casters [%.2fk / %.2fk]", visibleShadowCastersK, shadowCastersK);
        }
        else
        {
            float visibleShadowCastersM = global.stats.visibleShadowCasterCount / 1'000'000.f;
            float shadowCastersM = global.stats.shadowCasterCount / 1'000'000.f;
            ImGui::TextColored(statsColor, "Nb Shadow casters [%.3fM / %.3fM]", visibleShadowCastersM, shadowCastersM);
        }

        ImGui::TextColored(statsColor, "Nb Texture2D [%d]", global.stats.texture2DCount);
        ImGui::TextColored(statsColor, "Nb Material [%d]", global.stats.materialCount);
        ImGui::TextColored(statsColor, "Nb Mesh [%d]", global.stats.meshCount);
        ImGui::TextColored(statsColor, "Nb Model [%d]", global.stats.modelCount);
        ImGui::TextColored(statsColor, "Nb Visible Point Light [%d / %d]", global.stats.visiblePointLightCount, global.stats.pointLightCount);
        ImGui::TextColored(statsColor, "Nb PVS [%d]", global.stats.pvsCount);
        ImGui::Text("");
        ImGui::TextColored(infosColor, "Position (%.1f, %.1f, %.1f)", position.x, position.y, position.z);
        ImGui::TextColored(infosColor, "Rotation (%.1f, %.1f, %.1f, %.1f)", rotation.x, rotation.y, rotation.z, rotation.w);
        ImGui::TextColored(infosColor, "FOV/Near/Far (%.1f, %.2f, %.1f)", projection.fov, projection.near, projection.far);
        ImGui::TextColored(infosColor, "Resolution (%dx%d)", resolution.x, resolution.y);
        ImGui::TextColored(infosColor, "Current PVS (0x%x)", viewPVSEntityID);
    }

    ImGui::End();

    ImGui::Begin("Game View", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    global.isGameViewFocused = ImGui::IsWindowFocused();
    ImVec2 gameViewWindowSize = ImGui::GetWindowSize();

    // Subttract 41 to the height otherwise ImGui starts to display a scroll bar
    gpu->ImGuiImage(world.frameData.frames[frameIndex].lightingOutput->GetView(GPUImageComponent::eColor), float2(gameViewWindowSize.x, gameViewWindowSize.y - 41));

    ImGui::End();

    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        ea::span<const Log> logs = LogSystem::GetLogs();
        for (const Log& log : logs)
        {
            ImVec4 color = {};
            if (log.type == LogType::Info)
            {
                color = ImVec4(1, 1, 1, 1);
            }
            else if (log.type == LogType::Error)
            {
                color = ImVec4(0.8f, 0.1f, 0.1f, 1);
            }

            ImGui::TextColored(color, "- %s", log.str.c_str());
        }
    }
    ImGui::End();
}

static void UpdateFireflies(MainWorld& world)
{
    float time = world.timeManager.GetElapsedTime();

    for (Firefly& firefly : world.fireflies)
    {
        float offsetX = cos(firefly.cosOffset + time * firefly.moveSpeed) * firefly.pathRadius;
        float offsetZ = sin(firefly.sinOffset + time * firefly.moveSpeed) * firefly.pathRadius;

        float3 newPosition = firefly.originalPosition + float3(offsetX, 0, offsetZ);

        firefly.entity.SetComponent(Position(newPosition));
    }
}

static void UpdateWorld(MainWorld& world)
{
    auto& engine = *world.engine;
    auto& window = *engine.window;
    auto& keyboard = *engine.keyboard;

    engine.time->Update();
    keyboard.UpdateKeyStates(window);
    ProcessInputs(world);
    UpdateFireflies(world);
    UpdateLocalToWorlds(world);
    UpdateWorldRenderBounds(world);
    UpdateEntityPVSIDs(world);
    UpdateWorldToView(world);
    UpdateViewToClip(world);
    UpdateWorldToClip(world);
}

static void RenderWorld(MainWorld& world, GPUSwapchain* swapchain, const ea::vector<SharedPtr<Material>>& presentProcesses, ea::vector<UniqueGPUFramebufferPtr>& framebuffers, uint32 frameIndex)
{
    if (world.isFirstFrame)
    {
        BeforeFirstUpdateGraphicsEngine(world);
    }

    GPU* gpu = world.engine->gpu.get();

    UniqueGPUFramebufferPtr framebuffer = gpu->CreateFramebuffer(&gpu->GetPresentRenderPass(), ea::vector<const GPUImageView*>
    {
        gpu->GetSwapchain()->GetView(frameIndex)
    });

    UpdateGraphicsEngine(world, framebuffer.get(), presentProcesses[frameIndex].get(), frameIndex);

    swapchain->Present(world.frameData.frames[frameIndex].fence.get());

    framebuffers[frameIndex] = std::move(framebuffer);

    if (world.isFirstFrame)
    {
        world.isFirstFrame = false;
    }
}

int main()
{
    RegisterAllComponents();

    srand(456);
    LogSystem::Init();
    Window::InitSharedContext();
    {
        auto window = NewUnique<Window>(uint2(WindowWidth, WindowHeight), "Vulkan Engine", false);
        LogSystem::LogInfo(fmt::format("Window initialized. Resolution {}x{}\n", WindowWidth, WindowHeight));

        auto jobScheduler = NewUnique<JobScheduler>();
        LogSystem::LogInfo(fmt::format("JobScheduler initialized. {} Worker threads\n", jobScheduler->GetWorkerCount()));

        auto gpu = NewUnique<GPU, GPUVulkan>(*window, *jobScheduler);
        LogSystem::LogInfo(fmt::format("GPU initialized. {}\n", gpu->GetProperties().deviceName.c_str()));

        auto keyboard = NewUnique<Keyboard>();
        auto time = NewUnique<TimeManager>();
        auto renderAssets = NewUnique<RenderAssetManager>(*gpu);
        auto matMeshRefCounts = NewUnique<MatMeshRefCountManager>(*renderAssets);

        gpu->ImGuiInit();

        auto engine = NewUnique<Engine>();
        engine->window = std::move(window);
        engine->jobScheduler = std::move(jobScheduler);
        engine->gpu = std::move(gpu);
        engine->keyboard = std::move(keyboard);
        engine->time = std::move(time);
        engine->renderAssets = std::move(renderAssets);
        engine->matMeshRefCounts = std::move(matMeshRefCounts);

        auto world = NewUnique<MainWorld>();
        InitWorld(*engine, *world);

        uint32 imageCount = engine->gpu->GetSwapchain()->GetImageCount();

        ea::vector<SharedPtr<Material>> presentProcesses;
        presentProcesses.resize(imageCount);

        for (uint32 i = 0; i < imageCount; ++i)
        {
            presentProcesses[i] = CreatePresentProcess(engine->renderAssets.get(), world->frameData.frames[i].lightingOutput.get());
        }

        ea::vector<UniqueGPUFramebufferPtr> framebuffers;
        framebuffers.resize(imageCount);

        world->timeManager.StartTiming();

        while (engine->window->IsOpen())
        {
            GPUSwapchain* swapchain = engine->gpu->GetSwapchain();
            uint32 frameIndex = swapchain->AcquireImage();

            UpdateEditor(*world, frameIndex);
            UpdateWorld(*world);
            RenderWorld(*world, swapchain, presentProcesses, framebuffers, frameIndex);
            world->timeManager.Update();
        }

        engine->gpu->Flush();

        for (FrameData& frame : world->frameData.frames)
        {
            frame.fence->Wait();
        }

        engine->gpu->ImGUIUninit();
    }
    Window::UninitSharedContext();
    LogSystem::Deinit();

    return 0;
}