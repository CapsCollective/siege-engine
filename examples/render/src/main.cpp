//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#define VOLK_IMPLEMENTATION

#include <render/components/Shape.h>
#include <render/input/Input.h>
#include <render/renderer/Renderer.h>
#include <render/renderer/material/Material.h>
#include <render/renderer/model/Model.h>
#include <render/renderer/shader/Shader.h>
#include <render/window/Window.h>

#include <chrono>
#include <cmath>
#include <vector>

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>

#define WINDOWS_ATTACH_CONSOLE            \
    AttachConsole(ATTACH_PARENT_PROCESS); \
    freopen("CON", "w", stdout);          \
    freopen("CON", "w", stderr);          \
    freopen("CON", "r", stdin);
#else
#define WINDOWS_ATTACH_CONSOLE
#endif

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

Siege::Vertex2D triangleVerts[] = {{{0.f, -1.f}, {1.f, 0.f, 0.f}},
                                   {{1.f, 1.f}, {0.f, 1.f, 0.f}},
                                   {{-1.f, 1.f}, {0.f, 0.f, 1.f}}};

Siege::Mesh::MeshData triangleMeshData {
    sizeof(Siege::Vertex2D),
    triangleVerts, // Vertex array
    3, // 3 vertices
    nullptr, // no indices
    0 // no indices specified
};

Siege::Vertex2D squareVerts[] = {
    {{1.f, 1.f}, {1.f, 0.f, 0.f}}, // top right
    {{1.f, -1.f}, {1.f, 0.f, 0.f}}, // bottom right
    {{-1.f, -1.f}, {1.f, 0.f, 0.f}}, // bottom left
    {{-1.f, 1.f}, {1.f, 0.f, 0.f}}, // top left
};

u32 squareIndices[] = {0, 1, 3, 1, 2, 3};

Siege::Mesh::MeshData squareMeshData {sizeof(Siege::Vertex2D), squareVerts, 4, squareIndices, 6};

void MoveCameraXZ(float deltaTime, Components::Shape& viewerObject)
{
    static auto oldMousePos = Input::GetCursorPosition();
    auto mousePos = Input::GetCursorPosition();

    Vec3 rotate {Vec3::Zero};
    float lookSpeed = 4.0f;

    double differenceX = mousePos.x - oldMousePos.x;
    double differenceY = oldMousePos.y - mousePos.y;

    rotate.y += float(differenceX);
    rotate.x += float(differenceY);

    if (Vec3::Dot(rotate, rotate) > Float::Epsilon())
    {
        Vec3 newRotation = viewerObject.GetRotation() + lookSpeed * rotate.Normalise();
        viewerObject.SetRotation(Vec3::Lerp(viewerObject.GetRotation(), newRotation, deltaTime));
    }

    // Limit the pitch values to avoid objects rotating upside-down.
    viewerObject.SetRotationX(glm::clamp(viewerObject.GetRotation().x, -1.5f, 1.5f));
    viewerObject.SetRotationY(glm::mod(viewerObject.GetRotation().y, glm::two_pi<float>()));

    float yaw = viewerObject.GetRotation().y;
    const Vec3 forwardDir {glm::sin(yaw), 0.f, glm::cos(yaw)};
    const Vec3 rightDir {forwardDir.z, 0.f, -forwardDir.x};
    const Vec3 upDir {0.f, -1.f, 0.f};

    Vec3 moveDir {Vec3::Zero};
    if (Input::IsKeyDown(KEY_W)) moveDir += forwardDir;
    if (Input::IsKeyDown(KEY_S))
    {
        moveDir -= forwardDir;
    }
    if (Input::IsKeyDown(KEY_A)) moveDir -= rightDir;
    if (Input::IsKeyDown(KEY_D)) moveDir += rightDir;

    if (Input::IsKeyDown(KEY_E)) moveDir += upDir;
    if (Input::IsKeyDown(KEY_Q)) moveDir -= upDir;

    float moveSpeed = 2.f;

    if (Vec3::Dot(moveDir, moveDir) > Float::Epsilon())
    {
        Vec3 newMove = viewerObject.GetPosition() + moveSpeed * moveDir.Normalise();
        viewerObject.SetPosition(Vec3::Lerp(viewerObject.GetPosition(), newMove, deltaTime));
    }

    oldMousePos = mousePos;
}

// TODO(Aryeh): Move the following to Utils:
// * Math
// * StackArray

// TODO(Aryeh): Change all vec3 colors to the Color object.
// TODO(Aryeh): Change pipeline shader reading function to use utils filesystem.
// TODO(Aryeh): Move Input to another module (probably Engine).
// TODO(Aryeh): Write Unit Tests for renderer.

int main()
{
    WINDOWS_ATTACH_CONSOLE

    Siege::Window window("Render Example", WIDTH, HEIGHT);

    window.DisableCursor();

    Input::SetWindowPointer(&window);

    Siege::Renderer renderer(window);

    Siege::Camera camera;

    Components::Shape cameraObject;

    // Shader Declaration

    // Vertex shaders

    auto diffuseShader =
        Siege::Shader::BuildShader()
            .FromShader("assets/shaders/simpleShader.vert.spv")
            .WithStage(Siege::PipelineConfig::VERTEX)
            .WithVertexType(sizeof(Siege::Vertex))
            .WithVertexAttribute(offsetof(Siege::Vertex, position), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(Siege::Vertex, color), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(Siege::Vertex, normal), Siege::VertexDescription::VEC3)
            .WithVertexAttribute(offsetof(Siege::Vertex, uv), Siege::VertexDescription::VEC2)
            .WithStorage(0, "objectBuffer", sizeof(Siege::Model::Transform), 1000)
            .WithUniform(1, "globalData", sizeof(Siege::Renderer3D::GlobalData), 1);

    auto spriteShader =
        Siege::Shader::BuildShader()
            .FromShader("assets/shaders/simpleShader2D.vert.spv")
            .WithStage(Siege::PipelineConfig::VERTEX)
            .WithVertexType(sizeof(Siege::Vertex2D))
            .WithVertexAttribute(offsetof(Siege::Vertex2D, position),
                                 Siege::VertexDescription::VEC2)
            .WithVertexAttribute(offsetof(Siege::Vertex2D, color), Siege::VertexDescription::VEC3)
            .WithStorage(0, "objectBuffer", sizeof(Siege::Model::Transform2D), 1000)
            .WithUniform(1, "globalData", sizeof(Siege::Renderer2D::GlobalData));

    // Fragment shaders

    auto fragShader = Siege::Shader::BuildShader()
                          .FromShader("assets/shaders/simpleShader.frag.spv")
                          .WithStage(Siege::PipelineConfig::FRAGMENT);

    auto diffuseFragShader =
        Siege::Shader::BuildShader()
            .FromShader("assets/shaders/diffuseFragShader.frag.spv")
            .WithStage(Siege::PipelineConfig::FRAGMENT)
            .WithUniform(1,
                         "globalData",
                         sizeof(Siege::Renderer3D::GlobalData)); // TIL: bindings must be unique
                                                                 // accross all available shaders

    // Material Declaration
    // vertex       // fragment
    Siege::Material diffuseMat(&diffuseShader, &diffuseFragShader); // 3D diffuse material
    Siege::Material spriteMat(&spriteShader, &fragShader); // 2D sprite material

    // Siege::Material pointLightMat(&pointLightVertShader, &pointLightFragShader); // point light
    // shader

    Siege::Material::BuildMaterials({&diffuseMat, &spriteMat});

    // Generate models

    // Generating models from raw vertices

    Siege::Model triangleModel(triangleMeshData);
    Siege::Model squareModel(squareMeshData);

    // Generating models from .obj files

    Siege::Model cubeObjModel("assets/models/cube.obj");
    Siege::Model vaseObjModel("assets/models/smooth_vase.obj");

    // Set 2D sprite material
    triangleModel.SetMaterial(&spriteMat);
    squareModel.SetMaterial(&spriteMat);

    // Set 3D diffuse material
    cubeObjModel.SetMaterial(&diffuseMat);
    vaseObjModel.SetMaterial(&diffuseMat);

    // Create shapes for use
    std::vector<Components::Shape> shapes = {Components::Shape(&cubeObjModel),
                                             Components::Shape(&cubeObjModel),
                                             Components::Shape(&vaseObjModel)};

    // TODO(Aryeh): create a separate object for representing 2D shapes
    std::vector<Components::Shape> shapes2D = {Components::Shape(&triangleModel),
                                               Components::Shape(&squareModel)};

    shapes[0].SetPosition({0.f, -.5f, 0.f});
    shapes[0].SetScale({.5f, .5f, .5f});
    shapes[0].SetColor({.5f, 0.f, 0.f});

    shapes[1].SetPosition({0.f, 0.f, 0.f});
    shapes[1].SetScale({3.f, 3.f, 0.01f});
    shapes[1].SetColor({.5f, 0.f, 0.f});
    shapes[1].SetRotationX(1.570796f);

    shapes[2].SetPosition({0.f, -1.f, 0.f});
    shapes[2].SetScale({2.f, 2.f, 2.f});
    shapes[2].SetColor({.5f, 0.f, 0.f});

    shapes2D[0].SetPosition2D({1.5f, -1.f});
    shapes2D[0].SetScale2D({.5f, 0.5f});
    shapes2D[0].SetZIndex(0.f);

    shapes2D[1].SetPosition2D({-1.5f, -1.f});
    shapes2D[1].SetScale2D({.5f, 0.5f});
    shapes2D[1].SetZIndex(0.f);

    cameraObject.SetPosition({0.f, -1.f, -2.5f});

    auto currentTime = std::chrono::high_resolution_clock::now();

    bool inputEnabled = true;

    renderer.SetMainCamera(&camera);

    renderer.SetClearValue(1.f, 1.f, 1.f, 1.f);

    while (!window.WindowShouldClose())
    {
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
                .count();
        currentTime = newTime;

        auto alpha = std::clamp<float>(abs(sin(static_cast<float>(glfwGetTime()))), 0.001f, 1.f);

        Siege::Window::Update();

        if (Input::IsKeyJustPressed(KEY_ESCAPE))
        {
            inputEnabled = !inputEnabled;
            window.ToggleCursor(inputEnabled);
        }

        float aspect = renderer.GetAspectRatio();

        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (inputEnabled)
        {
            MoveCameraXZ(frameTime, cameraObject);
            camera.SetViewYXZ(cameraObject.GetPosition(), cameraObject.GetRotation());
        }

        if (!renderer.StartFrame()) continue;

        for (auto& shape : shapes)
        {
            Siege::Renderer3D::DrawModel(shape.GetModel(),
                                         shape.GetPosition(),
                                         shape.GetScale(),
                                         shape.GetRotation());
        }

        // TODO(Aryeh): This will eventually need to take in multiple lights.
        Siege::Renderer3D::DrawPointLight({0.0f, -1.f, -1.5f},
                                          0.05f,
                                          {1.f, 0.f, 0.f, alpha},
                                          {1.f, 1.f, 1.f, .02f});

        Siege::Renderer3D::DrawBillboard({-1.f, -2.5f, 0.f}, {1.f, 1.f}, {0.f, 1.f, 0.f, 1.f});
        Siege::Renderer3D::DrawBillboard({1.f, -2.5f, 0.f}, {1.f, 1.f}, {1.f, 0.f, 0.f, 1.f});

        Siege::Renderer3D::DrawLine({0.0f, -1.f, -1.5f}, {0.f, -1.f, 0.f}, {1.f, 1.f, 1.f});

        for (auto& shape : shapes2D)
        {
            Siege::Renderer2D::DrawModel(shape.GetModel(),
                                         shape.GetPosition2D(),
                                         shape.GetScale2D(),
                                         shape.GetRotation2D(),
                                         shape.GetZIndex());
        }

        renderer.EndFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
