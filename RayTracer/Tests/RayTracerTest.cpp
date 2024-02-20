#include "Scene/Sphere.hpp"
#include "Scene/Material.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>

int main()
{
    Scene::GeometryList world;

    auto matGround = std::make_shared<Scene::Lambertian>(Math::Color(0.8, 0.8, 0.8));
    auto matCenter = std::make_shared<Scene::Lambertian>(Math::Color(0.7, 0.3, 0.3));
    auto matLeft = std::make_shared<Scene::Metal>(Math::Color(0.8, 0.8, 0.0), 0.3);
    auto matRight = std::make_shared<Scene::Metal>(Math::Color(0.8, 0.6, 0.2), 1.0);

    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0, -100.5, -1.0), 100.0, matGround));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0,    0.0, -1.0),   0.5, matCenter));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(-1.0,    0.0, -1.0),   0.5, matLeft));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 1.0,    0.0, -1.0),   0.5, matRight));

    Renderer::SceneRenderer renderer;
    renderer.Initialize(960, 540, 80);
    renderer.Render(world);

    return 0;
}
