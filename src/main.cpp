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

void StepSimulation(std::vector<Components::Shape>& physicsObjs, std::vector<RigidBody2D>& rigidbodies, float dt) {
    // Loops through all pairs of objects and applies attractive force between them
    size_t firstObjIndex = 0;
    for (auto iterA = physicsObjs.begin(); iterA != physicsObjs.end(); ++iterA) {
        auto& objA = *iterA;
        size_t secondObjIndex = 0;
        for (auto iterB = iterA; iterB != physicsObjs.end(); ++iterB) {
            if (iterA == iterB) 
            {
                secondObjIndex++;
                continue;
            }
            auto& objB = *iterB;

            auto force = ComputeForce(objA, rigidbodies[firstObjIndex], objB, rigidbodies[secondObjIndex]);
            rigidbodies[firstObjIndex].velocity += dt * -force / rigidbodies[firstObjIndex].mass;
            rigidbodies[secondObjIndex].velocity += dt * force / rigidbodies[secondObjIndex].mass;

            secondObjIndex++;
        }
        firstObjIndex++;
    }

    // update each objects position based on its final velocity
    size_t index = 0; 
    for (auto& obj : physicsObjs) {
        obj.SetTransform(obj.GetTransform().position += dt * rigidbodies[index].velocity);
        index++;
    }
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

    std::vector<Components::Shape> circles = {Components::Shape(&circleModel), Components::Shape(&circleModel)};

    std::vector<RigidBody2D> rigidBodies = {{}, {}};

    circles[0].SetScale(glm::vec2(.05f));
    circles[0].SetTransform({.5f, .5f});
    circles[0].SetColor({1.0f, 0.0f, 0.0f});
    rigidBodies[0].velocity = {-.5f, .0f};

    circles[1].SetScale(glm::vec2(.05f));
    circles[1].SetTransform({-.45f, -.25f});
    circles[1].SetColor({0.0f, 0.0f, 1.0f});
    rigidBodies[1].velocity = {.5f, .0f};

    int subSteps = 5;
    
    while(!window.WindowShouldClose()) {

        window.Update();

        float dt = 1.0f / 60;

        const float stepDelta = dt / subSteps;

        for (size_t i = 0; i < subSteps; i++) StepSimulation(circles, rigidBodies, stepDelta);

        if (renderer.StartFrame()) 
        {
            for (auto& circle : circles)
            {
                renderer.DrawModel(circle.GetModel(), circle.GetPushConstantData());
            }
            renderer.EndFrame();
        }
    }

    renderer.ClearDeviceQueue();

    return 0;
}
