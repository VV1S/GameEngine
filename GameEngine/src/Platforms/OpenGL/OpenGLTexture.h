#pragma once
#include "Engine/Renderer/Texture.h"
#include <string>
#include <cstdint>

namespace Engine {

    class OpenGLTexture2D final : public Texture2D {
    public:
        OpenGLTexture2D(uint32_t w, uint32_t h, bool srgb = true);
        explicit OpenGLTexture2D(const std::string& path, bool srgb = true);
        ~OpenGLTexture2D() override;

        OpenGLTexture2D(const OpenGLTexture2D&) = delete;
        OpenGLTexture2D& operator=(const OpenGLTexture2D&) = delete;
        OpenGLTexture2D(OpenGLTexture2D&&) noexcept;
        OpenGLTexture2D& operator=(OpenGLTexture2D&&) noexcept;

        uint32_t GetWidth()  const override { return m_W; }
        uint32_t GetHeight() const override { return m_H; }

        void SetData(void* data, uint32_t size) override;
        void Bind(uint32_t slot = 0) const override;

        uint32_t id() const noexcept { return m_ID; }

    private:
        void allocateStorage(bool srgb, int channels);
        void commonParams() const;

    private:
        std::string m_Path;
        uint32_t m_W = 0, m_H = 0;
        uint32_t m_ID = 0;
        unsigned m_Internal = 0, m_Pixel = 0;
    };

} // namespace Engine
