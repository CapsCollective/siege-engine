#define VOLK_IMPLEMENTATION

#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model/Model.h"
#include "Components/Shape.h"

#include <glm/gtc/constants.hpp>
#include <vector>

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
const float GRAVITY_STRENGTH = 0.81f;

SnekVk::Model::Vertex triangleVerts[] = {
    {{0.0f, -0.5f}},
    {{0.5f, 0.5f}}, 
    {{-0.5f, 0.5f}}
};

SnekVk::Model::Vertex squareVerts[] = {
    {{-0.5f, -0.5f}},
    {{0.5f, 0.5f}},
    {{-0.5f, 0.5f}},
    {{-0.5f, -0.5f}},
    {{0.5f, -0.5f}},
    {{0.5f, 0.5f}}, 
};

struct RigidBody2D 
{
    glm::vec2 velocity;
    float mass{1.0f};
};

std::vector<SnekVk::Model::Vertex> GenerateCircleVertices(u32 numSides)
{
    std::vector<SnekVk::Model::Vertex> uniqueVertices {};
    for (size_t i = 0 ; i < numSides; i++)
    {
        float angle = i * glm::two_pi<float>() / numSides;
        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
    }

    uniqueVertices.push_back({});
    std::vector<SnekVk::Model::Vertex> vertices {};
    for (size_t i = 0 ; i < numSides; i++)
    {
        vertices.push_back(uniqueVertices[i]);
        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
        vertices.push_back(uniqueVertices[numSides]);
    }
    return vertices;
}

glm::vec2 ComputeForce(
    Components::Shape& modelA, 
    RigidBody2D& modelARigid, 
    Components::Shape& modelB, 
    RigidBody2D& modelBRigid)
{
    auto offset = modelA.GetTransform().position - modelB.GetTransform().position;
    float distanceSquared = glm::dot(offset, offset);
 
    // clown town - just going to return 0 if objects are too close together...
    if (glm::abs(distanceSquared) < 1e-10f) {
      return {.0f, .0f};
    }
 
    float force =
        GRAVITY_STRENGTH * modelBRigid.mass * modelARigid.mass / distanceSquared;
    return force * offset / glm::sqrt(distanceSquared);
}

int main() 
{
    WINDOWS_ATTACH_CONSOLE
    
    SnekVk::Window window("Snek", WIDTH, HEIGHT);

    SnekVk::Renderer renderer(window);

    SnekVk::Model triangleModel(SnekVk::Renderer::GetDevice(), triangleVerts, 3);

    SnekVk::Model squareModel(SnekVk::Renderer::GetDevice(), squareVerts, 6);

    auto circleVertices = GenerateCircleVertices(64);

    SnekVk::Model circleModel(SnekVk::Renderer::GetDevice(), circleVertices.data(), circleVertices.size());

    RigidBody2D redCircleBody{};
    auto redCircle = Components::Shape(&circleModel);
    redCircle.SetScale(glm::vec2(.05f));
    redCircle.SetTransform({.5f, .5f});
    redCircle.SetColor({1.0f, 0.0f, 0.0f});
    redCircleBody.velocity = {-.5f, .0f};

    RigidBody2D blueCircleBody{};
    auto blueCircle = Components::Shape(&circleModel);
    blueCircle.SetScale(glm::vec2(.05f));
    blueCircle.SetTransform({-.45f, -.25f});
    blueCircle.SetColor({0.0f, 0.0f, 1.0f});
    blueCircleBody.velocity = {.5f, .0f};

    Components::Shape circles[] = {
        redCircle,
        blueCircle
    };

    int subSteps = 5;
    
    while(!window.WindowShouldClose()) {

        window.Update();

        float dt = 1.0f / 60;

        const float stepDelta = dt / subSteps;

        auto forceA = ComputeForce(redCircle, redCircleBody, blueCircle, blueCircleBody);
        std::cout << "FORCE A" << forceA.x << forceA.y << std::endl;
        redCircleBody.velocity += dt * -forceA / redCircleBody.mass; 
        blueCircleBody.velocity += dt * forceA / blueCircleBody.mass;

        auto forceB = ComputeForce(blueCircle, blueCircleBody, redCircle, redCircleBody);
        std::cout << "FORCE B " << forceB.x << forceB.y << std::endl;
        blueCircleBody.velocity += dt * -forceB / blueCircleBody.mass; 
        redCircleBody.velocity += dt * forceB / redCircleBody.mass; 

        redCircle.SetTransform(redCircle.GetTransform().position += dt * redCircleBody.velocity * 0.001f);
        blueCircle.SetTransform(blueCircle.GetTransform().position += dt * blueCircleBody.velocity * 0.001f);

        if (renderer.StartFrame()) 
        {

            renderer.DrawModel(redCircle.GetModel(), redCircle.GetPushConstantData());
            renderer.DrawModel(blueCircle.GetModel(), blueCircle.GetPushConstantData());
            renderer.EndFrame();
        }
    }

    renderer.ClearDeviceQueue();

    return 0;
}
