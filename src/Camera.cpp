#include "Camera.hpp"

namespace rt
{
    void Camera::Initialize()
    {
        m_ImageHeight = static_cast<int>(imageWidth / aspectRatio);
        m_ImageHeight = (m_ImageHeight < 1) ? 1 : m_ImageHeight;

        float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / m_ImageHeight);

        glm::vec3 viewportU = glm::vec3(viewportWidth, 0.0f, 0.0f);
        glm::vec3 viewportV = glm::vec3(0.0f, -viewportHeight, 0.0f);

        m_PixelDeltaU = viewportU / static_cast<float>(imageWidth);
        m_PixelDeltaV = viewportV / static_cast<float>(m_ImageHeight);

        glm::vec3 viewportUpperLeft = cameraCenter - glm::vec3(0.0f, 0.0f, focalLength) - (viewportU / 2.0f) - (viewportV / 2.0f);
        m_Pixel00Loc = viewportUpperLeft + 0.5f * (m_PixelDeltaU + m_PixelDeltaV);
    }

    Ray Camera::GetRay(int x, int y) const
    {
        return GetRay(x, y, glm::vec2(0.0f));
    }

    Ray Camera::GetRay(int x, int y, const glm::vec2 &offset) const
    {
        glm::vec3 pixelCenter = m_Pixel00Loc + (static_cast<float>(x) + offset.x) * m_PixelDeltaU + (static_cast<float>(y) + offset.y) * m_PixelDeltaV;
        glm::vec3 rayDirection = pixelCenter - cameraCenter;

        return Ray(cameraCenter, rayDirection);
    }

} // namespace rt
