#pragma once

#include "Sphere.hpp"
#include "Quad.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "BVH.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

namespace Scene
{
    Scene::GeometryList WorldRandomSpheres(Scene::Camera& camera);
    Scene::GeometryList WorldTwoSpheres(Scene::Camera& camera);
    Scene::GeometryList WorldEarth(Scene::Camera& camera);
    Scene::GeometryList WorldTwoPerlinSpheres(Scene::Camera& camera);
    Scene::GeometryList WorldQuads(Scene::Camera& camera);
    Scene::GeometryList WorldSimpleLight(Scene::Camera& camera);
    Scene::GeometryList WorldCornellBox(Scene::Camera& camera);
}