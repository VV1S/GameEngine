#include "enginepch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine {

    static inline void LabelTexture(uint32_t id, const std::string& name) {
#ifdef GL_KHR_debug
        if (glObjectLabel) glObjectLabel(GL_TEXTURE, id, (GLsizei)name.size(), name.c_str());
#endif
    }

    OpenGLTexture2D::OpenGLTexture2D(uint32_t w, uint32_t h, bool srgb)
        : m_W(w), m_H(h) {
        EG_PROFILE_FUNCTION();
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
        m_Internal = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
        m_Pixel = GL_RGBA;
        glTextureStorage2D(m_ID, 1, m_Internal, (GLsizei)m_W, (GLsizei)m_H);
        commonParams();
        LabelTexture(m_ID, "Texture2D (empty)");
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool srgb)
        : m_Path(path) {
        EG_PROFILE_FUNCTION();

        int w, h, ch;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &w, &h, &ch, 0);
        EG_CORE_CHECK(data, "Failed to load image: {}", path);

        m_W = (uint32_t)w; m_H = (uint32_t)h;
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

        // choose formats
        if (ch == 4) { m_Internal = srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8; m_Pixel = GL_RGBA; }
        else if (ch == 3) { m_Internal = srgb ? GL_SRGB8 : GL_RGB8; m_Pixel = GL_RGB; }
        else EG_CORE_CHECK(false, "Unsupported channel count: {}", ch);

        glTextureStorage2D(m_ID, 1, m_Internal, (GLsizei)m_W, (GLsizei)m_H);
        commonParams();

        glTextureSubImage2D(m_ID, 0, 0, 0, (GLsizei)m_W, (GLsizei)m_H, m_Pixel, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_ID);

        stbi_image_free(data);
        LabelTexture(m_ID, m_Path);
    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        if (m_ID) glDeleteTextures(1, &m_ID);
    }

    OpenGLTexture2D::OpenGLTexture2D(OpenGLTexture2D&& o) noexcept {
        std::swap(m_ID, o.m_ID);
        std::swap(m_W, o.m_W);
        std::swap(m_H, o.m_H);
        std::swap(m_Internal, o.m_Internal);
        std::swap(m_Pixel, o.m_Pixel);
        std::swap(m_Path, o.m_Path);
    }

    OpenGLTexture2D& OpenGLTexture2D::operator=(OpenGLTexture2D&& o) noexcept {
        if (this != &o) {
            if (m_ID) glDeleteTextures(1, &m_ID);
            std::swap(m_ID, o.m_ID);
            std::swap(m_W, o.m_W);
            std::swap(m_H, o.m_H);
            std::swap(m_Internal, o.m_Internal);
            std::swap(m_Pixel, o.m_Pixel);
            std::swap(m_Path, o.m_Path);
        }
        return *this;
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) {
        EG_PROFILE_FUNCTION();
        const uint32_t bpp = (m_Pixel == GL_RGBA) ? 4u : 3u;
        EG_CORE_CHECK(size == m_W * m_H * bpp, "Texture data size mismatch");
        glTextureSubImage2D(m_ID, 0, 0, 0, (GLsizei)m_W, (GLsizei)m_H, m_Pixel, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(m_ID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const {
        glBindTextureUnit((GLuint)slot, m_ID);
    }

    void OpenGLTexture2D::commonParams() const {
        glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

} // namespace Engine
