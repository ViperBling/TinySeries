#pragma once

#include <iostream>

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(size_t imageWidth, size_t imageHeight)
        : mImageWidth(imageWidth)
        , mImageHeight(imageHeight)
        , mAspectRatio(static_cast<float>(imageWidth) / imageHeight)
    {}

    void Draw();

private:
    float mAspectRatio = 1;
    size_t mImageWidth = 1;
    size_t mImageHeight = 1;
};