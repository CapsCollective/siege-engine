#pragma once

#include "../Renderer/Renderer.h"

namespace Components 
{
    struct Transform2D
    {
        glm::vec2 position {0.0f};
        glm::vec2 scale {1.0f, 1.0f};
        float rotation {0};

        glm::mat2 transform() 
        { 
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};

            glm::mat2 scaleMat = {{scale.x, 0.0f}, {0.0f, scale.y}};
            return rotMatrix * scaleMat;
        };
    };

    class Shape
    {
        public: 

        Shape();
        Shape(SnekVk::Model* model);

        ~Shape();

        Transform2D& GetTransform() { return transform; };
        glm::vec3& GetColor() { return fillColor; }
        SnekVk::Model* GetModel() { return model; }

        void SetColor(glm::vec3 newColor);
        void SetScale(glm::vec2 newScale);
        void SetTransform(glm::vec2 newPos);
        void SetRotation(float rotation);

        private: 
        
        SnekVk::Model* model;
        Transform2D transform{};
        glm::vec3 fillColor{0.0f};
    };
}