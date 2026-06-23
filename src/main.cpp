#include "RayTracer.hpp"

#include <glm/glm.hpp>

int main()
{
    rt::RayTracer rayTracer;

    rayTracer.Render("images/result.png");
}
