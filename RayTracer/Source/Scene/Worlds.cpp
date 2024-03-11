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
        camera.mMaxDepth = 100;

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

        std::shared_ptr<Geometry> box1 = Scene::Box(Math::Point3(0, 0, 0), Math::Point3(165, 330, 165), white);
        box1 = std::make_shared<Scene::RotateY>(box1, 15);
        box1 = std::make_shared<Scene::Translate>(box1, Math::Vector3(265, 0, 295));
        world.Add(box1);
        std::shared_ptr<Geometry> box2 = Scene::Box(Math::Point3(0, 0, 0), Math::Point3(165, 165, 165), white);
        box2 = std::make_shared<Scene::RotateY>(box2, -18);
        box2 = std::make_shared<Scene::Translate>(box2, Math::Vector3(130, 0, 65));
        world.Add(box2);

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldCornellSmoke(Scene::Camera &camera)
    {
        camera.mBackgroundColor = Math::Color(0, 0, 0);
        camera.mLookFrom = Math::Point3(278, 278, -800);
        camera.mLookAt = Math::Point3(278, 278, 0);
        camera.mFov = 40;
        camera.mImageWidth = 512;
        camera.mImageHeight = 512;
        camera.mSamplesPerPixel = 500;
        camera.mMaxDepth = 100;

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

        std::shared_ptr<Geometry> box1 = Scene::Box(Math::Point3(0, 0, 0), Math::Point3(165, 330, 165), white);
        box1 = std::make_shared<Scene::RotateY>(box1, 15);
        box1 = std::make_shared<Scene::Translate>(box1, Math::Vector3(265, 0, 295));
        
        std::shared_ptr<Geometry> box2 = Scene::Box(Math::Point3(0, 0, 0), Math::Point3(165, 165, 165), white);
        box2 = std::make_shared<Scene::RotateY>(box2, -18);
        box2 = std::make_shared<Scene::Translate>(box2, Math::Vector3(130, 0, 65));

        world.Add(std::make_shared<Scene::ConstantMedium>(box1, 0.01, Math::Color(0, 1, 1)));
        world.Add(std::make_shared<Scene::ConstantMedium>(box2, 0.01, Math::Color(1, 1, 0)));

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }

    Scene::GeometryList WorldFinal(Scene::Camera &camera, int width, int height, int samples, int maxDepth)
    {
        camera.mBackgroundColor = Math::Color(0, 0, 0);
        camera.mLookFrom = Math::Point3(478, 278, -600);
        camera.mLookAt = Math::Point3(278, 278, 0);
        camera.mFov = 40;
        camera.mImageWidth = width;
        camera.mImageHeight = height;
        camera.mSamplesPerPixel = samples;
        camera.mMaxDepth = maxDepth;

        Scene::GeometryList world;
        Scene::GeometryList boxes1;

        int boxesPerSide = 20;
        auto ground = std::make_shared<Scene::Lambertian>(Math::Color(0.48, 0.83, 0.53));
        for (int i = 0; i < boxesPerSide; i++)
        {
            for (int j = 0; j < boxesPerSide; j++)
            {
                double w = 100;
                double x0 = -1000 + i * w;
                double z0 = -1000 + j * w;
                double y0 = 0;
                double x1 = x0 + w;
                double y1 = Math::RandomDouble(1, 101);
                double z1 = z0 + w;

                boxes1.Add(Scene::Box(Math::Point3(x0, y0, z0), Math::Point3(x1, y1, z1), ground));
            }
        }
        world.Add(std::make_shared<Scene::BVHNode>(boxes1));

        auto light = std::make_shared<Scene::DiffuseLight>(Math::Color(7, 7, 7));
        world.Add(std::make_shared<Scene::Quad>(Math::Point3(123, 554, 147), Math::Vector3(300, 0, 0), Math::Vector3(0, 0, 265), light));

        auto center1 = Math::Point3(400, 400, 200);
        auto center2 = center1 + Math::Vector3(30, 0, 0);
        auto movingSphereMaterial = std::make_shared<Scene::Lambertian>(Math::Color(0.7, 0.3, 0.1));
        world.Add(std::make_shared<Scene::Sphere>(center1, center2, 50, movingSphereMaterial));

        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(260, 150, 45), 50, std::make_shared<Scene::Dielectric>(1.5)));
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(0, 150, 145), 50, std::make_shared<Scene::Metal>(Math::Color(0.8, 0.8, 0.9), 1.0)));

        auto boundary = std::make_shared<Scene::Sphere>(Math::Point3(360, 150, 145), 70, std::make_shared<Scene::Dielectric>(1.5));
        world.Add(boundary);
        world.Add(std::make_shared<Scene::ConstantMedium>(boundary, 0.2, Math::Color(0.2, 0.4, 0.9)));
        boundary = std::make_shared<Scene::Sphere>(Math::Point3(0, 0, 0), 5000, std::make_shared<Scene::Dielectric>(1.5));
        world.Add(std::make_shared<Scene::ConstantMedium>(boundary, 0.0001, Math::Color(1, 1, 1)));

        auto emat = std::make_shared<Scene::Lambertian>(std::make_shared<Scene::ImageTexture>("Assets/Textures/earthmap.jpg"));
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(400, 200, 400), 100, emat));
        auto pertext = std::make_shared<Scene::NoiseTexture>(0.1);
        world.Add(std::make_shared<Scene::Sphere>(Math::Point3(220, 280, 300), 80, std::make_shared<Scene::Lambertian>(pertext)));

        Scene::GeometryList boxes2;
        auto white = std::make_shared<Scene::Lambertian>(Math::Color(0.73, 0.73, 0.73));
        int ns = 1000;
        for (int j = 0; j < ns; j++)
        {
            boxes2.Add(Scene::Box(Math::Point3::Random(0, 165), Math::Point3::Random(0, 165), white));
        }
        world.Add(std::make_shared<Scene::Translate>(std::make_shared<Scene::RotateY>(std::make_shared<Scene::BVHNode>(boxes2), 15), Math::Vector3(-100, 270, 395)));

        return Scene::GeometryList(std::make_shared<Scene::BVHNode>(world));
    }
}