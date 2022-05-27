#pragma once

#include "../Core.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace SnekVk
{
    class Camera
    {
        public:

        struct GPUCameraData
        {
            glm::mat4 projection;
            glm::mat4 view;
            glm::mat4 projView;
        };

        void SetOrthographicProjection(
            float left, float right, float top, float bottom, float near, float far);

        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

        void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});

        void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});

        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

        const glm::mat4& GetProjection() { return projectionMatrix; }
        const glm::mat4& GetView() { return viewMatrix; }
        glm::mat4 GetProjView() { return projectionMatrix * viewMatrix; }
        GPUCameraData GetCameraData() { return { projectionMatrix, viewMatrix, GetProjView() }; }

        private:
        
        glm::mat4 projectionMatrix{1.f};
        glm::mat4 viewMatrix{1.f};
    };
}