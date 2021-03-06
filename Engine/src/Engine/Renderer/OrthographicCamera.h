#pragma once

#include <glm/glm.hpp>

namespace Engine
{
    class OrthographicCamera
    {
    public:
        struct Bounds
        {
            float Left, Right, Bottom, Top;

            float GetWidth() const { return Right - Left; }
            float GetHeight() const { return Top - Bottom; }
        };

    public:
        OrthographicCamera();
        OrthographicCamera(float left, float right, float bottom, float top);

        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

        float GetRotation() const { return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        const Bounds& GetBounds() const { return m_Bounds; }

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position = glm::vec3(0.0f);
        float m_Rotation = 0.0f;

        Bounds m_Bounds;
    };
}