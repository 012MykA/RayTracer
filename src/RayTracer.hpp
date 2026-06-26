#pragma once

#include "Config.hpp"
#include "Camera.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
#include "Material.hpp"

#include <filesystem>

namespace rt
{
    namespace fs = std::filesystem;

    class RayTracer
    {
    public:
        void Render(const fs::path &output);

        void SetCamera(const Camera &camera) { m_Camera = camera; }
        void SetWorld(const HittableList &world) { m_World = world; }
        void SetRenderConfig(const RenderConfig &config) { m_RenderConfig = config; }

    private:
        Camera m_Camera;
        HittableList m_World;
        RenderConfig m_RenderConfig;
    };

} // namespace rt
