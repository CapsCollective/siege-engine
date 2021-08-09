#include "Shape.h"

namespace Components
{
    Shape::Shape() {}

    Shape::Shape(SnekVk::Model* model) : model{model} {}

    Shape::~Shape() {}

    void Shape::SetColor(glm::vec3 newColor) 
    { 
        fillColor = newColor;
        pushConstantData.color = newColor;
    }
    void Shape::SetPosition(glm::vec3 newPos) 
    { 
        transform.position = newPos;
        pushConstantData.transform = transform.transform();
    }

    void Shape::SetScale(glm::vec3 newScale) 
    { 
        transform.scale = newScale;
        pushConstantData.transform = transform.transform();
    }

    void Shape::SetRotation(glm::vec3 rotation)
    {
        transform.rotation = rotation;
        pushConstantData.transform = transform.transform();
    }

    void Shape::SetRotationX(float rotation)
    {
        transform.rotation.x = rotation;
        pushConstantData.transform = transform.transform();
    }

    void Shape::SetRotationY(float rotation)
    {
        transform.rotation.y = rotation;
        pushConstantData.transform = transform.transform();
    }

    void Shape::SetRotationZ(float rotation)
    {
        transform.rotation.z = rotation;
        pushConstantData.transform = transform.transform();
    }
}