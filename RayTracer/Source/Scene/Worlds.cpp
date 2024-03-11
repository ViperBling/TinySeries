#pragma once

#include "Worlds.hpp"

namespace Scene
{
    Scene::GeometryList WorldRandomSpheres(Scene::Camera &camera)
    {
        camera.mLookFrom = Math::Point3(13, 2, 3);
        camera.mLookAt = Math::Point3(0, 0, 0);

        Scene::GeometryList world;

        // auto matGround = std::make_shared<Scene::Lambertian>(Math::Color(0.5, 0.5, 0.5));
        // world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, matGround));
        auto checkerGround = std::make_shared<Scene::CheckerTexture>(0.8, Math::Color(0.2, 0.3, 0.1), Math::Color(0.9, 0.9, 0.9));
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, std::make_shared<Scene::Lambertian>(checkerGround)));

        for (int i = -5; i < 5; i++)
        {
            for (int j = -5; j < 5; j++)
            {
                auto matChoose = Math::RandomDouble();
                float radius = 0.2;
                Math::Point3 center(i + 0.9 * Math::RandomDouble(), radius, j + 0.9 * Math::RandomDouble());
                // Math::Point3 center(i, radius, j);

                if ((center - Math::Point3(4, radius, 0)).Length() > 0.9)
                {
                    std::shared_ptr<Scene::Material> sphereMaterial;

                    if (matChoose < 0.8)
                    {
                        // Diffuse
                        auto albedo = Math::Vector3::Random() * Math::Vector3::Random();
                        sphereMaterial = std::make_shared<Scene::Lambertian>(albedo);
                        // auto center2 = center + Math::Vector3(0, 0, Math::RandomDouble(0, 0.5));
                        // world.Add(std::make_shared<Scene::Sphere>(center, center2, radius, sphereMaterial));
                        world.Add(std::make_shared<Scene::Sphere>(center, radius, sphereMaterial));
                    }
                    else if (matChoose < 0.95)
                    {
                        // Metal
                        auto albedo = Math::Vector3::Random(0.5, 1);
                        auto fuzz = Math::RandomDouble(0, 0.5);
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

        auto matLambertian = std::make_shared<Scene::Lambertian>(Math::Color(0.0, 0.5, 0.5));
        auto matGlass = std::make_shared<Scene::Dielectric>(1.5);
        auto matMetal = std::make_shared<Scene::Metal>(Math::Color(0.7, 0.6, 0.5), 0.1);

        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(-4.0, 1.0, 0.0), 1.0, matLambertian));
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 0.0, 1.0, 0.0), 1.0, matGlass));
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3( 4.0, 1.0, 0.0), 1.0, matMetal));

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldTwoSpheres(Scene::Camera &camera)
    {
        camera.mLookFrom = Math::Point3(13, 2, 0);
        camera.mLookAt = Math::Point3(0, 0, 0);

        Scene::GeometryList world;

        auto checker = std::make_shared<Scene::CheckerTexture>(0.1, Math::Color(0.2, 0.3, 0.1), Math::Color(0.9, 0.9, 0.9));

        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, -10, 0), 10, std::make_shared<Scene::Lambertian>(checker)));
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0,  10, 0), 10, std::make_shared<Scene::Lambertian>(checker)));

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldEarth(Scene::Camera &camera)
    {
        camera.mLookFrom = Math::Point3(0, 0, 12);
        camera.mLookAt = Math::Point3(0, 0, 0);

        auto earthTexture = std::make_shared<Scene::ImageTexture>("Assets/Textures/earthmap.jpg");
        auto earthSurface = std::make_shared<Scene::Lambertian>(earthTexture);
        auto globe = std::make_shared<Scene::Sphere>(Math::Point3(0, 0, 0), 2, earthSurface);

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(Scene::GeometryList(globe)));
    }

    Scene::GeometryList WorldTwoPerlinSpheres(Scene::Camera &camera)
    {
        camera.mLookFrom = Math::Point3(13, 2, 3);
        camera.mLookAt = Math::Point3(0, 0, 0);

        Scene::GeometryList world;

        auto perlinTexture = std::make_shared<Scene::NoiseTexture>(8);
        auto perlinSurface = std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, std::make_shared<Scene::Lambertian>(perlinTexture));
        auto perlinGlobe = std::make_shared<Scene::Sphere>(Math::Point3(0, 2, 0), 2, std::make_shared<Scene::Lambertian>(perlinTexture));
    
        world.Add(perlinSurface);
        world.Add(perlinGlobe);
    
        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldQuads(Scene::Camera &camera)
    {
        camera.mLookFrom = Math::Point3(0, 0, 9);
        camera.mLookAt = Math::Point3(0, 0, 0);
        camera.mFov = 80;
        camera.mImageWidth = 512;
        camera.mImageHeight = 512;

        Scene::GeometryList world;

        auto leftRed        = std::make_shared<Scene::Lambertian>(Math::Color(1.0, 0.2, 0.2));
        auto backGreen      = std::make_shared<Scene::Lambertian>(Math::Color(0.2, 1.0, 0.2));
        auto rightBlue      = std::make_shared<Scene::Lambertian>(Math::Color(0.2, 0.2, 1.0));
        auto upperOrange    = std::make_shared<Scene::Lambertian>(Math::Color(1.0, 0.5, 0.2));
        auto lowerTeal      = std::make_shared<Scene::Lambertian>(Math::Color(0.2, 0.8, 0.8));

        world.Add(std::make_shared<Scene::Quad>(Math::Point3(-3, -2, 5), Math::Vector3(0, 0, -4), Math::Vector3(0, 4,  0), leftRed));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(-2, -2, 0), Math::Vector3(4, 0,  0), Math::Vector3(0, 4,  0), backGreen));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3( 3, -2, 1), Math::Vector3(0, 0,  4), Math::Vector3(0, 4,  0), rightBlue));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(-2,  3, 1), Math::Vector3(4, 0,  0), Math::Vector3(0, 0,  4), upperOrange));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(-2, -3, 5), Math::Vector3(4, 0,  0), Math::Vector3(0, 0, -4), lowerTeal));

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldSimpleLight(Scene::Camera &camera)
    {
        camera.mBackgroundColor = Math::Color(0, 0, 0);
        camera.mLookFrom = Math::Point3(26, 3, 6);
        camera.mLookAt = Math::Point3(0, 2, 0);
        camera.mFov = 20;

        Scene::GeometryList world;

        auto perlinTexture = std::make_shared<Scene::NoiseTexture>(4);
        auto perlinSurface = std::make_shared<Scene::Sphere>(Math::Point3(0, -1000, 0), 1000, std::make_shared<Scene::Lambertian>(perlinTexture));
        auto perlinGlobe = std::make_shared<Scene::Sphere>(Math::Point3(0, 2, 0), 2, std::make_shared<Scene::Lambertian>(perlinTexture));

        auto diffuseLightMat = std::make_shared<Scene::DiffuseLight>(Math::Color(4, 4, 4));
        auto quadLight = std::make_shared<Scene::Quad>(Math::Point3(3, 1, -2), Math::Vector3(2, 0, 0), Math::Vector3(0, 2, 0), diffuseLightMat);
        auto sphereLight = std::make_shared<Scene::Sphere>(Math::Point3(0, 7, 0), 2, diffuseLightMat);

        world.Add(perlinSurface);
        world.Add(perlinGlobe);
        world.Add(quadLight);
        world.Add(sphereLight);

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldCornellBox(Scene::Camera &camera)
    {
        camera.mBackgroundColor = Math::Color(0, 0, 0);
        camera.mLookFrom = Math::Point3(278, 278, -800);
        camera.mLookAt = Math::Point3(278, 278, 0);
        camera.mFov = 40;
        camera.mImageWidth = 512;
        camera.mImageHeight = 512;
        camera.mSamplesPerPixel = 100;
        camera.mMaxDepth = 50;

        Scene::GeometryList world;

        auto red = std::make_shared<Scene::Lambertian>(Math::Color(0.65, 0.05, 0.05));
        auto white = std::make_shared<Scene::Lambertian>(Math::Color(0.73, 0.73, 0.73));
        auto green = std::make_shared<Scene::Lambertian>(Math::Color(0.12, 0.45, 0.15));
        auto light = std::make_shared<Scene::DiffuseLight>(Math::Color(15, 15, 15));

        world.Add(std::make_shared<Scene::Quad>(Math::Point3(555, 0, 0), Math::Vector3(0, 555, 0), Math::Vector3(0, 0, 555), green));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(0, 0, 0), Math::Vector3(0, 555, 0), Math::Vector3(0, 0, 555), red));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(0, 0, 0), Math::Vector3(555, 0, 0), Math::Vector3(0, 0, 555), white));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(0, 0, 555), Math::Vector3(555, 0, 0), Math::Vector3(0, 555, 0), white));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(555, 555, 555), Math::Vector3(-555, 0, 0), Math::Vector3(0, 0, -555), white));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(343, 554, 332), Math::Vector3(-130, 0, 0), Math::Vector3(0, 0, -105), light));

        // world.Add(std::make_shared<Scene::Sphere>(Math::Point3(190, 90, 190), 90, std::make_shared<Scene::Dielectric>(1.5)));
        world.Add(Scene::Box(Math::Point3(130, 0, 65), Math::Point3(295, 165, 230), white));
        world.Add(Scene::Box(Math::Point3(265, 0, 295), Math::Point3(430, 330, 460), white));

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }
}