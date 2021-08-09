#include "Shape.h"

namespace Components
{
    glm::mat4 CalculateTransform(Transform& transform)
    {
        const float c3 = glm::cos(transform.rotation.z);
        const float s3 = glm::sin(transform.rotation.z);
        const float c2 = glm::cos(transform.rotation.x);
        const float s2 = glm::sin(transform.rotation.x);
        const float c1 = glm::cos(transform.rotation.y);
        const float s1 = glm::sin(transform.rotation.y);

        return glm::mat4{
        {
            transform.scale.x * (c1 * c3 + s1 * s2 * s3),
            transform.scale.x * (c2 * s3),
            transform.scale.x * (c1 * s2 * s3 - c3 * s1),
            0.0f,
        },
        {
            transform.scale.y * (c3 * s1 * s2 - c1 * s3),
            transform.scale.y * (c2 * c3),
            transform.scale.y * (c1 * c3 * s2 + s1 * s3),
            0.0f,
        },
        {
            transform.scale.z * (c2 * s1),
            transform.scale.z * (-s2),
            transform.scale.z * (c1 * c2),
            0.0f,
        },
        {transform.position.x, transform.position.y, transform.position.z, 1.0f}};
    }

    Shape::Shape() {}

    Shape::Shape(SnekVk::Model* model) : model{model} {}

    Shape::~Shape() {}

    void Shape::SetColor(glm::vec3 newColor) 
    { 
        fillColor = newColor;
    }
    void Shape::SetPosition(glm::vec3 newPos) 
    { 
        transform.position = newPos;
    }

    void Shape::SetScale(glm::vec3 newScale) 
    { 
        transform.scale = newScale;
    }

    void Shape::SetRotation(glm::vec3 rotation)
    {
        transform.rotation = rotation;
    }

    void Shape::SetRotationX(float rotation)
    {
        transform.rotation.x = rotation;
    }

    void Shape::SetRotationY(float rotation)
    {
        transform.rotation.y = rotation;
    }

    void Shape::SetRotationZ(float rotation)
    {
        transform.rotation.z = rotation;
    }
}