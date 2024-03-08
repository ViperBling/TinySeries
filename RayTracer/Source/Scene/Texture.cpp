#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "Texture.hpp"

namespace Scene
{
    ImageTexture::ImageTexture(const char *filename)
    {
        auto componentsPerPixel = mBytesPerPixel;
        mImageData = stbi_load(filename, &mWidth, &mHeight, &componentsPerPixel, componentsPerPixel);
        if (!mImageData)
        {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            mWidth = mHeight = 0;
        }
        mBytesPerScanline = mBytesPerPixel * mWidth;
    }

    ImageTexture::~ImageTexture()
    {
        STBI_FREE(mImageData);
    }

    Math::Color ImageTexture::Value(double u, double v, const Math::Point3 &point) const
    {
        // If we have no texture data, then return solid cyan as a debugging aid.
        if (mImageData == nullptr) { return Math::Color(0, 1, 1); }

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = Math::Clamp(u, 0.0, 1.0);
        v = 1.0 - Math::Clamp(v, 0.0, 1.0);  // Flip V to image coordinates

        auto i = static_cast<int>(u * mWidth);
        auto j = static_cast<int>(v * mHeight);

        if (i >= mWidth) { i = mWidth - 1; }
        if (j >= mHeight) { j = mHeight - 1; }

        const auto colorScale = 1.0 / 255.0;
        auto pixel = mImageData + j * mBytesPerScanline + i * mBytesPerPixel;

        return Math::Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
    }
}