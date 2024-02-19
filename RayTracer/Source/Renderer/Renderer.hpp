#pragma once

#include "Scene/Camera.hpp"

namespace Scene
{
    class Camera;
    class Sphere;
    class GeometryList;
}

namespace Renderer
{
    class SceneRenderer
    {
    public:
        SceneRenderer() = default;
        ~SceneRenderer() = default;

        void Initialize(int imageWidth, int imageHeight, int samplesPerPixel);
        void Render(const Scene::GeometryList& world);

    private:
        Scene::Camera mCamera;
    };
}
