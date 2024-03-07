#include "Scene/Sphere.hpp"
#include "Scene/Material.hpp"
#include "Scene/BVH.hpp"
#include "Scene/Texture.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>
#include <chrono>

int main()
{
    Scene::GeometryList world;

    // auto matGround = std::make_shared<Scene::Lambertian>(Math::Color(0.5, 0.5, 0.5));
    // world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, matGround));
    auto checkerGround = std::make_shared<Scene::CheckerTexture>(0.32, Math::Color(0.2, 0.3, 0.1), Math::Color(0.9, 0.9, 0.9));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, std::make_shared<Scene::Lambertian>(checkerGround)));

    for (int i = -2; i < 2; i++)
    {
        for (int j = -2; j < 2; j++)
        {
            auto matChoose = Utilities::RandomDouble();
            float radius = 0.2;
            Math::Point3 center(i + 0.9 * Utilities::RandomDouble(), radius, j + 0.9 * Utilities::RandomDouble());
            // Math::Point3 center(i, radius, j);

            if ((center - Math::Point3(4, radius, 0)).Length() > 0.9)
            {
                std::shared_ptr<Scene::Material> sphereMaterial;

                if (matChoose < 0.8)
                {
                    // Diffuse
                    auto albedo = Math::Vector3::Random() * Math::Vector3::Random();
                    sphereMaterial = std::make_shared<Scene::Lambertian>(albedo);
                    // auto center2 = center + Math::Vector3(0, 0, Utilities::RandomDouble(0, 0.5));
                    // world.Add(std::make_shared<Scene::Sphere>(center, center2, radius, sphereMaterial));
                    world.Add(std::make_shared<Scene::Sphere>(center, radius, sphereMaterial));
                }
                else if (matChoose < 0.95)
                {
                    // Metal
                    auto albedo = Math::Vector3::Random(0.5, 1);
                    auto fuzz = Utilities::RandomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<Scene::Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Scene::Sphere>(center, radius, sphereMaterial));
                }
                else
                {
                    // Glass
                    sphereMaterial = std::make_shared<Scene::Dielectric>(1.5);
                    world.Add(std::make_shared<Scene::Sphere>(center, radius, sphereMaterial));
                }

                world.Add(std::make_shared<Scene::Sphere>(center, radius, sphereMaterial));
            }
        }
    }

    auto matLambertian = std::make_shared<Scene::Lambertian>(Math::Color(0.4, 0.2, 0.1));
    auto matGlass = std::make_shared<Scene::Dielectric>(1.5);
    auto matMetal = std::make_shared<Scene::Metal>(Math::Color(0.7, 0.6, 0.5), 0.1);

    world.Add(std::make_shared<Scene::Sphere>(Math::Point3(-4.0, 1.0, 0.0), 1.0, matLambertian));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0, 1.0, 0.0), 1.0, matGlass));
    world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 4.0, 1.0, 0.0), 1.0, matMetal));
    world = Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));

    Scene::Camera camera;
    camera.mImageWidth = 960;
    camera.mImageHeight = 540;
    camera.mSamplesPerPixel = 100;
    camera.mMaxDepth = 10;
    camera.mFov = 20;
    camera.mLookFrom = Math::Point3(13, 2, 3);
    camera.mLookAt = Math::Point3(0, 0, 0);
    camera.mUp = Math::Vector3(0, 1, 0);
    camera.mDefocusAngle = 0.6;
    camera.mFocusDistance = 10;

    Renderer::SceneRenderer renderer;
    renderer.Initialize(camera);

    auto tStart = std::chrono::high_resolution_clock::now();
    renderer.Render(world);
    auto tEnd = std::chrono::high_resolution_clock::now();

    std::clog << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(tEnd - tStart).count() << "s" << std::flush;

    return 0;
}
