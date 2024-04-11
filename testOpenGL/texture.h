#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

// uses cases
// 1. read from file and input to shader
// 2. rw image

// Texture vs Image
// Texture: mipmap, multiple images
// Image: 1
// glBindTexture vs glBindImageTexture

class Texture
{
public:
    Texture() = delete;
    // read
    explicit Texture(GLenum type, const std::string &path) : _type{type}
    {
        // 1 byte alignment for row pixel
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // Direct State Access
        glCreateTextures(type, 1, &_handle);
        glTextureParameteri(_handle, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(_handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // only support GL_TEXTURE_2D
        switch (type)
        {
        case GL_TEXTURE_2D:
        {
            int width, height, numChannels;
            uint8_t *img = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
            assert(img);
            auto numMipmapLevels = mipmapLevels2D(width, height);
            // allocate memory
            glTextureStorage2D(_handle, numMipmapLevels, GL_RGB8, width, height);
            // upload data to vram for mipmap level: 0
            glTextureSubImage2D(_handle, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, img);
            stbi_image_free(img);
            glGenerateTextureMipmap(_handle);
            glTextureParameteri(_handle, GL_TEXTURE_MAX_LEVEL, numMipmapLevels - 1);
            glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(_handle, GL_TEXTURE_MAX_ANISOTROPY, 16);
            break;
        }
        default:
            assert(false);
        }

        _bindlessHandle = glGetTextureHandleARB(_handle);
        glMakeTextureHandleResidentARB(_bindlessHandle);
    }
    // rw
    explicit Texture(GLenum type, int width, int height, GLenum internalFormat) : _type{type}
    {
        glCreateTextures(type, 1, &_handle);
        glTextureParameteri(_handle, GL_TEXTURE_MAX_LEVEL, 0);
        glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // allocate memory
        glTextureStorage2D(_handle, mipmapLevels2D(width, height), internalFormat, width, height);
    }
    // read from raw memory
    explicit Texture(int width, int height, const void *img) : _type{GL_TEXTURE_2D}
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glCreateTextures(_type, 1, &_handle);
        auto numMipmapLevels = mipmapLevels2D(width, height);
        glTextureStorage2D(_handle, numMipmapLevels, GL_RGBA8, width, height);
        glTextureSubImage2D(_handle, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, img);
        glGenerateTextureMipmap(_handle);
        glTextureParameteri(_handle, GL_TEXTURE_MAX_LEVEL, numMipmapLevels - 1);
        glTextureParameteri(_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(_handle, GL_TEXTURE_MAX_ANISOTROPY, 16);
        _bindlessHandle = glGetTextureHandleARB(_handle);
        glMakeTextureHandleResidentARB(_bindlessHandle);
    }

    ~Texture()
    {
        if (_bindlessHandle)
            glMakeTextureHandleNonResidentARB(_bindlessHandle);
        glDeleteTextures(1, &_handle);
    }

    Texture(const Texture &) = delete;

    Texture(Texture &&other) noexcept
    {
        _type = other._type;
        _handle = other._handle;
        _bindlessHandle = other._bindlessHandle;
    }

    inline auto getType() const
    {
        return _type;
    }

    inline auto getHandle() const
    {
        return _handle;
    }

    inline auto getBindlessHandle() const
    {
        return _bindlessHandle;
    }

    static int mipmapLevels2D(int w, int h)
    {
        int levels = 1;
        while ((w | h) >> levels)
        {
            levels += 1;
        }
        return levels;
    }

private:
    GLenum _type = 0;
    GLuint _handle = 0;
    // GLSL to convert a 64-bit unsigned integer texture handle value into a sampler or image variab
    // If NV_vertex_attrib_integer_64bit and ARB_gpu_shader_int64 are not available, then you can achieve much the same effect using the uvec2 type.
    GLuint64 _bindlessHandle = 0;
};
