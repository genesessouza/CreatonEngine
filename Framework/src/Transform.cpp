#include "Engine/Framework/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Framework
{
    glm::mat4 Transform::GetMatrix() const
    {
        glm::mat4 modelMatrix(1.0f);
        
        modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Rotation.x), { 1,0,0 });
        modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Rotation.y), { 0,1,0 });
        modelMatrix = glm::rotate(modelMatrix, glm::radians(m_Rotation.z), { 0,0,1 });

        modelMatrix = glm::translate(modelMatrix, m_Position);
        modelMatrix = glm::scale(modelMatrix, m_Scale);

        return modelMatrix;
    }
}