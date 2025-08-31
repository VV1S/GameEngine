#pragma once
#include "Engine/Renderer/Shader.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace Engine {

    class OpenGLShader final : public Shader {
    public:
        explicit OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vs, const std::string& fs);
        ~OpenGLShader() override;

        OpenGLShader(const OpenGLShader&) = delete;
        OpenGLShader& operator=(const OpenGLShader&) = delete;
        OpenGLShader(OpenGLShader&&) = delete;
        OpenGLShader& operator=(OpenGLShader&&) = delete;

        void Bind() const override;
        void Unbind() const override;

        void SetInt(const std::string& n, int v) override;
        void SetFloat(const std::string& n, float v) override;
        void SetFloat3(const std::string& n, const glm::vec3& v) override;
        void SetFloat4(const std::string& n, const glm::vec4& v) override;
        void SetMat4(const std::string& n, const glm::mat4& m) override;

        const std::string& GetName() const override { return m_Name; }

        // kept for compatibility:
        void UploadUniformInt(const std::string& n, int v);
        void UploadUniformFloat(const std::string& n, float v);
        void UploadUniformFloat2(const std::string& n, const glm::vec2& v);
        void UploadUniformFloat3(const std::string& n, const glm::vec3& v);
        void UploadUniformFloat4(const std::string& n, const glm::vec4& v);
        void UploadUniformMat3(const std::string& n, const glm::mat3& m);
        void UploadUniformMat4(const std::string& n, const glm::mat4& m);

    private:
        std::string ReadFile(const std::string& path) const;
        std::unordered_map<unsigned, std::string> Preprocess(const std::string& src) const;
        void CompileLink(const std::unordered_map<unsigned, std::string>& sources);

        int Locate(const std::string& n) const; // cached uniform location

    private:
        unsigned m_Program = 0;
        std::string m_Name;
        mutable std::unordered_map<std::string, int> m_LocCache;
    };

} // namespace Engine
