#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model/Model.h"
#include "Components/Shape.h"
#include "Input/Input.h"
#include "Utils/Math.h"

#include <glm/gtc/constants.hpp>
#include <vector>
#include <chrono>

#if (defined(_WIN32) || defined(_WIN64)) && defined(DEBUG)
#include <windows.h>

#define WINDOWS_ATTACH_CONSOLE\
    AttachConsole(ATTACH_PARENT_PROCESS);\
    freopen("CON", "w", stdout);\
    freopen("CON", "w", stderr);\
    freopen("CON", "r", stdin);
#else
    #define WINDOWS_ATTACH_CONSOLE
#endif

static const constexpr int WIDTH = 800;
static const constexpr int HEIGHT = 600;

SnekVk::Vertex triangleVerts[] = {
    {{0.0f, -0.5f, 0.f}, {1.f, 0.f, 0.f}},
    {{0.5f, 0.5f, 0.f}, {0.f, 1.f, 0.f}}, 
    {{-0.5f, 0.5f, 0.f}, {0.f, 0.f, 1.f}}
};

SnekVk::Mesh::MeshData triangleMeshData {
    triangleVerts, // Vertex array
    3, // 3 vertices
    0, // no indices
    0  // no indices specified
};

SnekVk::Vertex squareVerts[] = {
    {{0.5f, 0.5f, 0.f}, {1.f, 0.f, 0.f}}, // top right
    {{0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}}, // bottom right
    {{-0.5f, -0.5f, 0.f}, {1.f, 0.f, 0.f}}, // bottom left
    {{-0.5f, 0.5f, 0.f}, {1.f, 0.f, 0.f}}, // top left
};

u32 squareIndices[] = {
    0, 1, 3, 1, 2, 3
};

SnekVk::Mesh::MeshData squareMeshData {
    squareVerts,
    4,
    squareIndices,
    6
};

SnekVk::Vertex cubeVerts[] =  {
    {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
    {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
    {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

    // right face (yellow)
    {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
    {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
    {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

    // top face (orange, remember y axis points down)
    {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
    {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

    // bottom face (red)
    {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
    {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
    {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

    // nose face (blue)
    {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
    {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

    // tail face (green)
    {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
    {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
};

u32 cubeIndices[] = {
    0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
    12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21
};

SnekVk::Mesh::MeshData cubeMeshData {
    cubeVerts,
    24,
    cubeIndices,
    36
};

void MoveCameraXZ(float deltaTime, Components::Shape& viewerObject)
{
    static auto oldMousePos = Input::GetCursorPosition();
    auto mousePos = Input::GetCursorPosition();

    glm::vec3 rotate{0};
    float lookSpeed = 4.0f;

    float differenceX = mousePos.x - oldMousePos.x;
    float differenceY = oldMousePos.y - mousePos.y;

    rotate.y += differenceX;
    rotate.x += differenceY;

    if (glm::dot(rotate, rotate) > glm::epsilon<float>())
    {
        glm::vec3 newRotation = viewerObject.GetRotation() + lookSpeed * glm::normalize(rotate);
        viewerObject.SetRotation(Utils::Math::Lerp(viewerObject.GetRotation(), newRotation, deltaTime));
    }

    // Limit the pitch values to avoid objects rotating upside-down.
    viewerObject.SetRotationX(glm::clamp(viewerObject.GetRotation().x, -1.5f, 1.5f));
    viewerObject.SetRotationY(glm::mod(viewerObject.GetRotation().y, glm::two_pi<float>()));

    float yaw = viewerObject.GetRotation().y;
    const glm::vec3 forwardDir{glm::sin(yaw), 0.f, glm::cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (Input::IsKeyDown(KEY_W)) moveDir += forwardDir;
    if (Input::IsKeyDown(KEY_S)) moveDir -= forwardDir;
    if (Input::IsKeyDown(KEY_A)) moveDir -= rightDir;
    if (Input::IsKeyDown(KEY_D)) moveDir += rightDir;

    if (Input::IsKeyDown(KEY_Q)) moveDir += upDir;
    if (Input::IsKeyDown(KEY_E)) moveDir -= upDir;

    float moveSpeed = 2.f;

    if (glm::dot(moveDir, moveDir) > glm::epsilon<float>())
    {
        glm::vec3 newMove = viewerObject.GetPosition() + moveSpeed * glm::normalize(moveDir);
        viewerObject.SetPosition(Utils::Math::Lerp(viewerObject.GetPosition(), newMove, deltaTime));
    }

    oldMousePos = mousePos;
}

int main() 
{
    WINDOWS_ATTACH_CONSOLE
    
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    window.DisableCursor();

    Input::SetWindowPointer(&window);

    SnekVk::Renderer renderer(window);

    SnekVk::Camera camera;

    Components::Shape cameraObject;

    SnekVk::Material diffuseMat; 

    diffuseMat.SetDescriptor({SnekVk::PipelineConfig::VERTEX, sizeof(SnekVk::Model::Transform) * 10000});

    diffuseMat.AddShader("shaders/simpleShader.vert.spv", SnekVk::PipelineConfig::VERTEX);
    diffuseMat.AddShader("shaders/simpleShader.frag.spv", SnekVk::PipelineConfig::FRAGMENT);

    diffuseMat.SetVertexInputSize(0, sizeof(SnekVk::Vertex));

    diffuseMat.AddVertexAttribute(0, offsetof(SnekVk::Vertex, position), SnekVk::VertexDescription::VEC3);
    diffuseMat.AddVertexAttribute(0, offsetof(SnekVk::Vertex, color), SnekVk::VertexDescription::VEC3);
    diffuseMat.AddVertexAttribute(0, offsetof(SnekVk::Vertex, normal), SnekVk::VertexDescription::VEC3);
    diffuseMat.AddVertexAttribute(0, offsetof(SnekVk::Vertex, uv), SnekVk::VertexDescription::VEC2);

    diffuseMat.BuildMaterial();

    renderer.RegisterMaterial(&diffuseMat);

    // Generate models

    // Generating models from raw vertices

    SnekVk::Model triangleModel(triangleMeshData);
    triangleModel.SetMaterial(&diffuseMat);

    SnekVk::Model squareModel(squareMeshData);
    squareModel.SetMaterial(&diffuseMat);

    SnekVk::Model cubeModel(cubeMeshData);
    cubeModel.SetMaterial(&diffuseMat);
    // Generating models from .obj files

    SnekVk::Model cubeObjModel("assets/models/cube.obj");
    cubeObjModel.SetMaterial(&diffuseMat);

    SnekVk::Model vaseObjModel("assets/models/smooth_vase.obj");
    vaseObjModel.SetMaterial(&diffuseMat);

    // Create shapes for use
    std::vector<Components::Shape> shapes = 
    {
        Components::Shape(&cubeObjModel),
        Components::Shape(&vaseObjModel),
        Components::Shape(&triangleModel),
        Components::Shape(&squareModel)
    };

    shapes[0].SetPosition({0.f, 0.5f, 2.5f});
    shapes[0].SetScale({.5f, .5f, .5f});
    shapes[0].SetColor({.5f, 0.f, 0.f});

    shapes[1].SetPosition({0.f, 0.f, 2.5f});
    shapes[1].SetScale({2.f, 2.f, 2.f});
    shapes[1].SetColor({.5f, 0.f, 0.f});

    shapes[2].SetPosition({1.5f, 0.f, 2.5f});

    shapes[3].SetPosition({-1.5f, 0.f, 2.5f});

    auto currentTime = std::chrono::high_resolution_clock::now();

    bool inputEnabled = true;

    renderer.SetMainCamera(&camera);

    while(!window.WindowShouldClose()) {
        
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        window.Update();

        if (Input::IsKeyJustPressed(KEY_ESCAPE)) 
        {
            inputEnabled = !inputEnabled;
            window.ToggleCursor(inputEnabled);
        }

        float aspect = renderer.GetAspectRatio();

        camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

        if (inputEnabled)
        {
            MoveCameraXZ(frameTime, cameraObject);
            camera.SetViewYXZ(cameraObject.GetPosition(), cameraObject.GetRotation());
        }

        if (!renderer.StartFrame()) continue;
        
        for (auto& shape : shapes)
        {
            renderer.DrawModel(shape.GetModel(), shape.GetTransform());
        }
        
        renderer.EndFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
