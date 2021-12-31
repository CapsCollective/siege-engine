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
    };

    class Shape
    {
        public: 

        Shape();
        Shape(SnekVk::Model* model);

        ~Shape();

        SnekVk::Model::Transform GetTransform() { return { CalculateTransform(transform), CalculateNormalMatrix(transform) }; };
        SnekVk::Model::Transform2D GetTransform2D() { return { CalculateTransform(transform) }; };
        glm::vec3& GetColor() { return fillColor; }
        SnekVk::Model* GetModel() { return model; }

        glm::vec3& GetRotation() { return transform.rotation; }
        glm::vec3& GetPosition() { return transform.position; }
        glm::vec3& GetScale() { return transform.scale; } 

        glm::vec2 GetPosition2D() { return glm::vec2(transform.position); }
        glm::vec2 GetScale2D() { return glm::vec2(transform.scale); } 

        const glm::vec3& GetRotation() const { return transform.rotation; }
        const glm::vec3& GetPosition() const { return transform.position; }
        const glm::vec3& GetScale() const { return transform.scale; } 

        const glm::vec2 GetPosition2D() const { return glm::vec2(transform.position); }
        const glm::vec2 GetScale2D() const { return glm::vec2(transform.scale); } 

        void SetColor(glm::vec3 newColor);
        void SetScale(glm::vec3 newScale);
        void SetPosition(glm::vec3 newPos);
        void SetRotation(glm::vec3 rotation);
        void SetRotationX(float rotation);
        void SetRotationY(float rotation);
        void SetRotationZ(float rotation);

        private: 

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 CalculateTransform(Transform& transform);

        glm::mat3 CalculateNormalMatrix(Transform& transform);
        
        SnekVk::Model* model;
        Transform transform{};
        glm::vec3 fillColor{0.0f};
    };
}