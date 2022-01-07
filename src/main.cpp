#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model/Model.h"
#include "Components/Shape.h"
#include "Input/Input.h"
#include "Utils/Math.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Lights/PointLight.h"

#include <vector>
#include <chrono>
#include <cmath>

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

SnekVk::Vertex2D triangleVerts[] = {
    {{0.f, -1.f}, {1.f, 0.f, 0.f}},
    {{1.f, 1.f}, {0.f, 1.f, 0.f}}, 
    {{-1.f, 1.f}, {0.f, 0.f, 1.f}}
};

SnekVk::Mesh::MeshData triangleMeshData {
    sizeof(SnekVk::Vertex2D),
    triangleVerts, // Vertex array
    3, // 3 vertices
    0, // no indices
    0  // no indices specified
};

SnekVk::Vertex2D squareVerts[] = {
    {{1.f, 1.f}, {1.f, 0.f, 0.f}}, // top right
    {{1.f, -1.f}, {1.f, 0.f, 0.f}}, // bottom right
    {{-1.f, -1.f}, {1.f, 0.f, 0.f}}, // bottom left
    {{-1.f, 1.f}, {1.f, 0.f, 0.f}}, // top left
};

u32 squareIndices[] = {
    0, 1, 3, 1, 2, 3
};

SnekVk::Mesh::MeshData squareMeshData {
    sizeof(SnekVk::Vertex2D),
    squareVerts,
    4,
    squareIndices,
    6
};

glm::vec2 squareVertsRaw[] = {
    {1.f, 1.f},
    {1.f, -1.f},
    {-1.f, -1.f},
    {-1.f, 1.f}
};

SnekVk::Mesh::MeshData rawSquareMeshData {
    sizeof(glm::vec2),
    squareVertsRaw,
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
    sizeof(SnekVk::Vertex),
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

    // Shader Declaration

    // Vertex shaders

    auto diffuseShader = SnekVk::Shader::BuildShader()
        .FromShader("shaders/simpleShader.vert.spv")
        .WithStage(SnekVk::PipelineConfig::VERTEX)
        .WithVertexType(sizeof(SnekVk::Vertex))
        .WithVertexAttribute(offsetof(SnekVk::Vertex, position), SnekVk::VertexDescription::VEC3)
        .WithVertexAttribute(offsetof(SnekVk::Vertex, color), SnekVk::VertexDescription::VEC3)
        .WithVertexAttribute(offsetof(SnekVk::Vertex, normal), SnekVk::VertexDescription::VEC3)
        .WithVertexAttribute(offsetof(SnekVk::Vertex, uv), SnekVk::VertexDescription::VEC2)
        .WithStorage(0, "objectBuffer", sizeof(SnekVk::Model::Transform), 1000)
        .WithUniform(1, "globalData", sizeof(SnekVk::Renderer3D::GlobalData), 1);
    
    auto spriteShader = SnekVk::Shader::BuildShader()
        .FromShader("shaders/simpleShader2D.vert.spv")
        .WithStage(SnekVk::PipelineConfig::VERTEX)
        .WithVertexType(sizeof(SnekVk::Vertex2D))
        .WithVertexAttribute(offsetof(SnekVk::Vertex2D, position), SnekVk::VertexDescription::VEC2)
        .WithVertexAttribute(offsetof(SnekVk::Vertex2D, color), SnekVk::VertexDescription::VEC3)
        .WithStorage(0, "objectBuffer", sizeof(SnekVk::Model::Transform2D), 1000)
        .WithUniform(1, "globalData", sizeof(SnekVk::Renderer2D::GlobalData));

    auto pointLightVertShader = SnekVk::Shader::BuildShader()
        .FromShader("shaders/pointLight.vert.spv")
        .WithStage(SnekVk::PipelineConfig::VERTEX)
        .WithVertexType(sizeof(glm::vec2))
        .WithVertexAttribute(0, SnekVk::VertexDescription::VEC2)
        .WithUniform(0, "globalData", sizeof(SnekVk::Renderer3D::GlobalData));

    // Fragment shaders

    auto fragShader = SnekVk::Shader::BuildShader()
        .FromShader("shaders/simpleShader.frag.spv")
        .WithStage(SnekVk::PipelineConfig::FRAGMENT);

    auto diffuseFragShader = SnekVk::Shader::BuildShader()
        .FromShader("shaders/diffuseFragShader.frag.spv")
        .WithStage(SnekVk::PipelineConfig::FRAGMENT)
        .WithUniform(1, "globalData", sizeof(SnekVk::Renderer3D::GlobalData)); // TIL: bindings must be unique accross all available shaders 
    
    auto pointLightFragShader = SnekVk::Shader::BuildShader()
        .FromShader("shaders/pointLight.frag.spv")
        .WithStage(SnekVk::PipelineConfig::FRAGMENT)
        .WithUniform(0, "globalData", sizeof(SnekVk::Renderer3D::GlobalData));

    // Material Declaration
                                // vertex       // fragment  
    SnekVk::Material diffuseMat(&diffuseShader, &diffuseFragShader); // 3D diffuse material
    SnekVk::Material spriteMat(&spriteShader, &fragShader);  // 2D sprite material 

    SnekVk::Material pointLightMat(&pointLightVertShader, &pointLightFragShader); // point light shader

    SnekVk::Material::BuildMaterials({&diffuseMat, &spriteMat, &pointLightMat});

    // Generate models

    // Generating models from raw vertices

    SnekVk::Model triangleModel(triangleMeshData);
    SnekVk::Model squareModel(squareMeshData);

    SnekVk::Model pointLightModel(rawSquareMeshData);

    // Generating models from .obj files

    SnekVk::Model cubeObjModel("assets/models/cube.obj");
    SnekVk::Model vaseObjModel("assets/models/smooth_vase.obj");

    // Set 2D sprite material
    triangleModel.SetMaterial(&spriteMat);
    squareModel.SetMaterial(&spriteMat);

    // Set 3D diffuse material
    cubeObjModel.SetMaterial(&diffuseMat);
    vaseObjModel.SetMaterial(&diffuseMat);

    pointLightModel.SetMaterial(&pointLightMat);

    // Create shapes for use
    std::vector<Components::Shape> shapes = 
    {
        Components::Shape(&cubeObjModel),
        Components::Shape(&cubeObjModel),
        Components::Shape(&vaseObjModel)
    };

    // TODO(Aryeh): create a separate object for representing 2D shapes
    std::vector<Components::Shape> shapes2D = 
    {
        Components::Shape(&triangleModel),
        Components::Shape(&squareModel)
    };

    shapes[0].SetPosition({0.f, .5f, 2.5f});
    shapes[0].SetScale({.5f, .5f, .5f});
    shapes[0].SetColor({.5f, 0.f, 0.f});

    shapes[1].SetPosition({0.f, 1.f, 2.5f});
    shapes[1].SetScale({3.f, 3.f, 0.01f});
    shapes[1].SetColor({.5f, 0.f, 0.f});
    shapes[1].SetRotationX(1.570796f);

    shapes[2].SetPosition({0.f, 0.f, 2.5f});
    shapes[2].SetScale({2.f, 2.f, 2.f});
    shapes[2].SetColor({.5f, 0.f, 0.f});

    shapes2D[0].SetPosition2D({1.5f, 0.f});
    shapes2D[0].SetScale2D({.5f, 0.5f});
    shapes2D[0].SetRotation2D(10.f);
    shapes2D[0].SetZIndex(2.5f);

    shapes2D[1].SetPosition2D({-1.5f, 0.f});
    shapes2D[1].SetScale2D({.5f, 0.5f});
    shapes2D[1].SetZIndex(2.5f);

    // Lights

    SnekVk::PointLight light(
        {0.0f, -1.0f, 1.5f}, 
        {1.f, 0.f, 0.f, 1.0f}, 
        {1.f, 1.f, 1.f, .02f}
    );

    light.SetModel(&pointLightModel);

    renderer.SetPointLight(&light);

    auto currentTime = std::chrono::high_resolution_clock::now();

    bool inputEnabled = true;

    renderer.SetMainCamera(&camera);

    while(!window.WindowShouldClose()) {
        
        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        auto alpha = std::clamp<float>(abs(sin(glfwGetTime())), 0.001f, 1.f);
        
        light.SetColor({1.f, 0.f, 0.f, alpha});

        window.Update();

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

        SnekVk::Renderer3D::DrawLine({4.f, 1.f, -1.5f}, {4.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({3.f, 1.f, -1.5f}, {3.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({2.f, 1.f, -1.5f}, {2.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({1.f, 1.f, -1.5f}, {1.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({0.f, 1.f, -1.5f}, {0.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-1.f, 1.f, -1.5f}, {-1.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-2.f, 1.f, -1.5f}, {-2.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-3.f, 1.f, -1.5f}, {-3.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, -1.5f}, {-4.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});

        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 6.5f}, {4.f, 1.f, 6.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 5.5f}, {4.f, 1.f, 5.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 4.5f}, {4.f, 1.f, 4.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 3.5f}, {4.f, 1.f, 3.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 2.5f}, {4.f, 1.f, 2.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 1.5f}, {4.f, 1.f, 1.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, 0.5f}, {4.f, 1.f, 0.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, -0.5f}, {4.f, 1.f, -0.5f}, {1.f, 1.f, 1.f});
        SnekVk::Renderer3D::DrawLine({-4.f, 1.f, -1.5f}, {4.f, 1.f, -1.5f}, {1.f, 1.f, 1.f});

        SnekVk::Renderer3D::DrawGrid(10, 1, {1.f, 1.f});
        
        for (auto& shape : shapes)
        {
            SnekVk::Renderer3D::DrawModel(shape.GetModel(), shape.GetPosition(), shape.GetScale(), shape.GetRotation());
        }

        // TODO(Aryeh): This will eventually need to take in multiple lights.
        SnekVk::Renderer3D::DrawLight(&pointLightModel);

        for (auto& shape : shapes2D)
        {
            SnekVk::Renderer2D::DrawModel(shape.GetModel(), shape.GetPosition2D(), shape.GetScale2D(), shape.GetRotation2D(), shape.GetZIndex());
        }
        
        renderer.EndFrame();
    }

    renderer.ClearDeviceQueue();

    return 0;
}
