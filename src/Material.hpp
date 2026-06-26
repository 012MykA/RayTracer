#pragma once

#include "Hittable.hpp"
#include "Ray.hpp"

#include <glm/glm.hpp>

namespace rt
{
    class Material
    {
    public:
        virtual ~Material() = default;

        virtual bool Scatter(const Ray &rayIn, const HitDesc &desc, glm::vec3 &attenuation, Ray &scattered) const { return false; }
    };

    class Lambertian : public Material
    {
    public:
        Lambertian(const glm::vec3 &albedo);

        bool Scatter(const Ray &rayIn, const HitDesc &desc, glm::vec3 &attenuation, Ray &scattered) const override;

    private:
        glm::vec3 m_Albedo;
    };

    class Metal : public Material
    {
    public:
        Metall(const glm::vec3 &albedo);

        bool Scatter(const Ray &rayIn, const HitDesc &desc, glm::vec3 &attenuation, Ray &scattered) const override;

    private:
        glm::vec3 m_Albedo;
    };

} // namespace rt
