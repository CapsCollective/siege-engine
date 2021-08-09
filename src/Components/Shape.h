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

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 transform() 
        { 
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);

            return glm::mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {position.x, position.y, position.z, 1.0f}};
        };
    };

    class Shape
    {
        public: 

        Shape();
        Shape(SnekVk::Model* model);

        ~Shape();

        Transform& GetTransform() { return transform; };
        glm::vec3& GetColor() { return fillColor; }
        SnekVk::Model* GetModel() { return model; }
        glm::vec3& GetRotation() { return transform.rotation; }

        const SnekVk::Model::PushConstantData& GetPushConstantData() const { return pushConstantData; }

        void SetColor(glm::vec3 newColor);
        void SetScale(glm::vec3 newScale);
        void SetPosition(glm::vec3 newPos);
        void SetRotation(glm::vec3 rotation);
        void SetRotationX(float rotation);
        void SetRotationY(float rotation);
        void SetRotationZ(float rotation);

        private: 
        
        SnekVk::Model* model;
        SnekVk::Model::PushConstantData pushConstantData{};
        Transform transform{};
        glm::vec3 fillColor{0.0f};
    };
}