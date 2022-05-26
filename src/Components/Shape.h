#pragma once

#include "../Renderer/Renderer.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Components 
{
    struct Transform
    {
        glm::vec3 position {0.0f};
        glm::vec3 scale {1.f, 1.f, 1.f};
        glm::vec3 rotation {};
        glm::mat4 transform;
    };

    // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 CalculateTransform(Transform& transform);

    class Shape
    {
        public: 

        Shape();
        Shape(SnekVk::Model* model);

        ~Shape();

        glm::mat4 GetTransform() { return CalculateTransform(transform); };
        glm::vec3& GetColor() { return fillColor; }
        SnekVk::Model* GetModel() { return model; }
        glm::vec3& GetRotation() { return transform.rotation; }

        void SetColor(glm::vec3 newColor);
        void SetScale(glm::vec3 newScale);
        void SetPosition(glm::vec3 newPos);
        void SetRotation(glm::vec3 rotation);
        void SetRotationX(float rotation);
        void SetRotationY(float rotation);
        void SetRotationZ(float rotation);

        private: 
        
        SnekVk::Model* model;
        Transform transform{};
        glm::vec3 fillColor{0.0f};
    };
}