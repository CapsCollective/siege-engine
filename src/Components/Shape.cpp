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
    void Shape::SetTransform(glm::vec2 newPos) 
    { 
        transform.position = newPos;
        pushConstantData.offset = newPos;
    }

    void Shape::SetScale(glm::vec2 newScale) 
    { 
        transform.scale = newScale;
        pushConstantData.transform = transform.transform();
    }

    void Shape::SetRotation(float rotation)
    {
        transform.rotation = rotation;
        pushConstantData.transform = transform.transform();
    }
}