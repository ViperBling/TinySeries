#include "Scene/Sphere.hpp"
#include "Scene/Material.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>

int main()
{
    Scene::GeometryList world;

    auto matGround = std::make_shared<Scene::Lambertian>(Math::Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, matGround));

    for (int i = -11; i < 11; i++)
    {
        for (int j = -11; j < 11; j++)
        {
            auto matChoose = Utilities::RandomDouble();
            Math::Point3 center(i + 0.9 * Utilities::RandomDouble(), 0.2, j + 0.9 * Utilities::RandomDouble());

            if ((center - Math::Point3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<Scene::Material> sphereMaterial;

                if (matChoose < 0.8)
                {
                    // Diffuse
                    auto albedo = Math::Vector3::Random() * Math::Vector3::Random();
                    sphereMaterial = std::make_shared<Scene::Lambertian>(albedo);
                    world.Add(std::make_shared<Scene::Sphere>(center, 0.2, sphereMaterial));
                }
                else if (matChoose < 0.95)
                {
                    // Metal
                    auto albedo = Math::Vector3::Random(0.5, 1);
                    auto fuzz = Utilities::RandomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<Scene::Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Scene::Sphere>(center, 0.2, sphereMaterial));
                }
                else
                {
                    // Glass
                    sphereMaterial = std::make_shared<Scene::Dielectric>(1.5);
                    world.Add(std::make_shared<Scene::Sphere>(center, 0.2, sphereMaterial));
                }

                world.Add(std::make_shared<Scene::Sphere>(center, 0.2, sphereMaterial));
            }
        }
    }

    auto matLambertian = std::make_shared<Scene::Lambertian>(Math::Color(0.4, 0.2, 0.1));
    auto matGlass = std::make_shared<Scene::Dielectric>(1.5);
    auto matMetal = std::make_shared<Scene::Metal>(Math::Color(0.7, 0.6, 0.5), 0.1);

    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(-4.0, 1.0, 0.0), 1.0, matLambertian));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0, 1.0, 0.0), 1.0, matGlass));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 4.0, 1.0, 0.0), 1.0, matMetal));

    Scene::Camera camera;
    camera.mImageWidth = 1280;
    camera.mImageHeight = 720;
    camera.mSamplesPerPixel = 500;
    camera.mMaxDepth = 50;
    camera.mFov = 20;
    camera.mLookFrom = Math::Point3(13, 2, 3);
    camera.mLookAt = Math::Point3(0, 0, 0);
    camera.mUp = Math::Vector3(0, 1, 0);
    camera.mDefocusAngle = 0.6;
    camera.mFocusDistance = 10;

    Renderer::SceneRenderer renderer;
    renderer.Initialize(camera);
    renderer.Render(world);

    return 0;
}
