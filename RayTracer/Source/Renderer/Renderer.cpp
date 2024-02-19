#include "Renderer.hpp"
#include "Scene/Geometry.hpp"

namespace Renderer
{
    void SceneRenderer::Initialize(int imageWidth, int imageHeight, int samplesPerPixel)
    {
        mCamera.Initialize(imageWidth, imageHeight, samplesPerPixel);
    }

    void SceneRenderer::Render(const Scene::GeometryList& world)
    {
        mCamera.Render(world);
    }
}