#include "Renderer.hpp"
#include "Scene/Geometry.hpp"

namespace Renderer
{
    void SceneRenderer::Initialize(const Scene::Camera& camera)
    {
        mCamera = camera;
        mCamera.Initialize();
    }

    void SceneRenderer::Render(const Scene::GeometryList& world)
    {
        mCamera.Render(world);
    }
}