#include "Scene/Sphere.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>

int main()
{
    Scene::GeometryList world;

    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, 0, -1), 0.5));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -100.5, -1), 100));

    Renderer::SceneRenderer renderer;
    renderer.Initialize(960, 540);
    renderer.Render(world);

    return 0;
}
