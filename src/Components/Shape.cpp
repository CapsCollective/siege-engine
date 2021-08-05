#include "Shape.h"

namespace Components
{
    Shape::Shape() {}

    Shape::Shape(SnekVk::Model* model) : model{model} {}

    Shape::~Shape() {}

    void Shape::SetColor(glm::vec3 newColor) 
    { 
        fillColor = newColor; 
        SnekVk::Model::PushConstantData oldData = model->GetPushConstant();
        
        model->SetPushConstant({ oldData.transform, oldData.offset, fillColor });
    }
    void Shape::SetTransform(glm::vec2 newPos) 
    { 
        SnekVk::Model::PushConstantData oldData = model->GetPushConstant();
        
        model->SetPushConstant({ oldData.transform, newPos, oldData.color });
        transform.position = newPos; 
    }

    void Shape::SetScale(glm::vec2 newScale) 
    { 
        transform.scale = newScale;

        SnekVk::Model::PushConstantData oldData = model->GetPushConstant();
        model->SetPushConstant({ transform.transform(), oldData.offset, oldData.color });
    }

    void Shape::SetRotation(float rotation)
    {
        transform.rotation = rotation;

        SnekVk::Model::PushConstantData oldData = model->GetPushConstant();
        model->SetPushConstant({ transform.transform(), oldData.offset, oldData.color });
    }
}