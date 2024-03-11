#include "Scene/Worlds.hpp"
#include "Renderer/Renderer.hpp"

#include <iostream>
#include <chrono>

int main()
{
    Scene::GeometryList world;

    Scene::Camera camera;
    camera.mImageWidth = 960;
    camera.mImageHeight = 540;
    camera.mSamplesPerPixel = 500;
    camera.mMaxDepth = 100;
    camera.mFocusDistance = 10;
    camera.mUp = Math::Vector3(0, 1, 0);
    camera.mBackgroundColor = Math::Color(0.7, 0.8, 1.0);
    camera.mFov = 20;

    switch (8)
    {
    case 0 :
        world = Scene::WorldRandomSpheres(camera);       
        break;
    case 1 :
        world = Scene::WorldTwoSpheres(camera);
        break;
    case 2 :
        world = Scene::WorldEarth(camera);
        break;
    case 3 : 
        world = Scene::WorldTwoPerlinSpheres(camera);
        break;
    case 4 : 
        world = Scene::WorldQuads(camera);
        break;
    case 5 :
        world = Scene::WorldSimpleLight(camera);
        break;
    case 6 :
        world = Scene::WorldCornellBox(camera);
        break;
    case 7 :
        world = Scene::WorldCornellSmoke(camera);
        break;
    case 8 :
        world = Scene::WorldFinal(camera, 960, 540, 100, 100);
        break;
    default:
        break;
    }
    // camera.mDefocusAngle = 0.6;

    Renderer::SceneRenderer renderer;
    renderer.Initialize(camera);

    auto tStart = std::chrono::high_resolution_clock::now();
    renderer.Render(world);
    auto tEnd = std::chrono::high_resolution_clock::now();

    std::clog << "Time: " << std::chrono::duration_cast<std::chrono::seconds>(tEnd - tStart).count() << "s" << std::flush;

    return 0;
}
