#include "Scene/Sphere.hpp"
#include "Scene/Material.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>

int main()
{
    Scene::GeometryList world;

    auto matGround = std::make_shared<Scene::Lambertian>(Math::Color(0.5, 0.5, 0.5));
    auto matLeft = std::make_shared<Scene::Lambertian>(Math::Color(0.8, 0.8, 0.0));
    auto matCenter = std::make_shared<Scene::Dielectric>(1.5);
    auto matRight = std::make_shared<Scene::Metal>(Math::Color(0.8, 0.6, 0.2), 0.1);

    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0, -100.5, -1.0), 100.0, matGround));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0,    0.0, -1.0),   0.5, matCenter));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(-1.0,    0.0, -1.0),   0.5, matLeft));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 1.0,    0.0, -1.0),   0.5, matRight));

    Scene::Camera camera;
    camera.mImageWidth = 960;
    camera.mImageHeight = 540;
    camera.mSamplesPerPixel = 80;
    camera.mMaxDepth = 50;
    camera.mFov = 20;
    camera.mLookFrom = Math::Point3(-2, 2, 1);
    camera.mLookAt = Math::Point3(0, 0, -1);
    camera.mUp = Math::Vector3(0, 1, 0);
    camera.mDefocusAngle = 10.0;
    camera.mFocusDistance = 3.4;

    Renderer::SceneRenderer renderer;
    renderer.Initialize(camera);
    renderer.Render(world);

    return 0;
}
