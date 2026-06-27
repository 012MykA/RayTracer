#include "RayTracer.hpp"

#include <string>
#include <iostream>

int main()
{
    rt::RayTracer rayTracer;

    rt::HittableList world;

    auto materialGround = std::make_shared<rt::Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
    auto materialCenter = std::make_shared<rt::Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
    auto materialLeft = std::make_shared<rt::Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 0.3f);
    auto materialRight = std::make_shared<rt::Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.0f);
    auto materialUp = std::make_shared<rt::Metal>(glm::vec3(0.9f, 0.9f, 0.9f), 0.0f);

    world.Add(std::make_shared<rt::Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, materialGround));
    world.Add(std::make_shared<rt::Sphere>(glm::vec3(0.0f, 0.0f, -0.5f), 0.5f, materialCenter));
    world.Add(std::make_shared<rt::Sphere>(glm::vec3(-1.0f, 0.0f, 0.0f), 0.5f, materialLeft));
    world.Add(std::make_shared<rt::Sphere>(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, materialRight));
    world.Add(std::make_shared<rt::Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, materialUp));

    rayTracer.SetWorld(world);

    rt::RenderConfig renderConfig{
        .samplesPerPixel = 10000,
        .maxDepth = 1000,
    };
    rayTracer.SetRenderConfig(renderConfig);

    rt::Camera cam;
    cam.cameraCenter = {0.0f, 0.0f, 1.2f};

    rayTracer.SetCamera(cam);

    rayTracer.Render("images/result.png");
}
