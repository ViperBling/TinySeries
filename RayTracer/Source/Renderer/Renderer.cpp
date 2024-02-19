#include "Renderer.hpp"
#include "Scene/Geometry.hpp"

namespace Renderer
{
    void SceneRenderer::Initialize(int imageWidth, int imageHeight)
    {
        mCamera.Initialize(imageWidth, imageHeight);
    }

    void SceneRenderer::Render(const Scene::GeometryList& world)
    {
        mCamera.Render(world);
    }
}