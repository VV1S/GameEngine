#include "enginepch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <array>
#include <filesystem>

namespace Engine {

    static unsigned ShaderTypeFromString(const std::string& t) {
        if (t == "vertex")   return GL_VERTEX_SHADER;
        if (t == "fragment" || t == "pixel")  return GL_FRAGMENT_SHADER;
        EG_CORE_CHECK(false, "Unknown shader stage");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath) {
        EG_PROFILE_FUNCTION();
        const std::string src = ReadFile(filepath);
        CompileLink(Preprocess(src));
        m_Name = std::filesystem::path(filepath).stem().string();
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vs, const std::string& fs)
        : m_Name(name) {
        EG_PROFILE_FUNCTION();
        std::unordered_map<unsigned, std::string> sources;
        sources[GL_VERTEX_SHADER] = vs;
        sources[GL_FRAGMENT_SHADER] = fs;
        CompileLink(sources);
    }

    OpenGLShader::~OpenGLShader() {
        if (m_Program) glDeleteProgram(m_Program);
    }

    void OpenGLShader::Bind() const { glUseProgram(m_Program); }
    void OpenGLShader::Unbind() const { glUseProgram(0); }

    void OpenGLShader::SetInt(const std::string& n, int v) { UploadUniformInt(n, v); }
    void OpenGLShader::SetFloat(const std::string& n, float v) { UploadUniformFloat(n, v); }
    void OpenGLShader::SetFloat3(const std::string& n, const glm::vec3& v) { UploadUniformFloat3(n, v); }
    void OpenGLShader::SetFloat4(const std::string& n, const glm::vec4& v) { UploadUniformFloat4(n, v); }
    void OpenGLShader::SetMat4(const std::string& n, const glm::mat4& m) { UploadUniformMat4(n, m); }

    int OpenGLShader::Locate(const std::string& n) const {
        if (auto it = m_LocCache.find(n); it != m_LocCache.end()) return it->second;
        int loc = glGetUniformLocation(m_Program, n.c_str());
        m_LocCache.emplace(n, loc);
        return loc;
    }

    void OpenGLShader::UploadUniformInt(const std::string& n, int v) { glUniform1i(Locate(n), v); }
    void OpenGLShader::UploadUniformFloat(const std::string& n, float v) { glUniform1f(Locate(n), v); }
    void OpenGLShader::UploadUniformFloat2(const std::string& n, const glm::vec2& v) { glUniform2f(Locate(n), v.x, v.y); }
    void OpenGLShader::UploadUniformFloat3(const std::string& n, const glm::vec3& v) { glUniform3f(Locate(n), v.x, v.y, v.z); }
    void OpenGLShader::UploadUniformFloat4(const std::string& n, const glm::vec4& v) { glUniform4f(Locate(n), v.x, v.y, v.z, v.w); }
    void OpenGLShader::UploadUniformMat3(const std::string& n, const glm::mat3& m) { glUniformMatrix3fv(Locate(n), 1, GL_FALSE, glm::value_ptr(m)); }
    void OpenGLShader::UploadUniformMat4(const std::string& n, const glm::mat4& m) { glUniformMatrix4fv(Locate(n), 1, GL_FALSE, glm::value_ptr(m)); }

    std::string OpenGLShader::ReadFile(const std::string& path) const {
        std::ifstream in(path, std::ios::binary);
        EG_CORE_CHECK(in.good(), "Cannot open shader: {}", path);
        std::string out;
        in.seekg(0, std::ios::end);
        out.resize((size_t)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(out.data(), out.size());
        return out;
    }

    std::unordered_map<unsigned, std::string> OpenGLShader::Preprocess(const std::string& src) const {
        std::unordered_map<unsigned, std::string> res;
        const char* token = "#type";
        size_t pos = src.find(token, 0);
        while (pos != std::string::npos) {
            size_t eol = src.find_first_of("\r\n", pos);
            EG_CORE_CHECK(eol != std::string::npos, "Shader syntax");
            size_t begin = pos + strlen(token) + 1;
            std::string type = src.substr(begin, eol - begin);
            size_t nextLine = src.find_first_not_of("\r\n", eol);
            pos = src.find(token, nextLine);
            res[ShaderTypeFromString(type)] = src.substr(nextLine, pos - (nextLine == std::string::npos ? src.size() - 1 : nextLine));
        }
        return res;
    }

    void OpenGLShader::CompileLink(const std::unordered_map<unsigned, std::string>& sources) {
        EG_PROFILE_FUNCTION();
        EG_CORE_CHECK(!sources.empty() && sources.size() <= 2, "Unsupported shader stages");

        const GLuint prog = glCreateProgram();
        std::array<GLuint, 2> ids{};
        int idx = 0;

        for (const auto& [stage, code] : sources) {
            GLuint sh = glCreateShader(stage);
            const char* src = code.c_str();
            glShaderSource(sh, 1, &src, nullptr);
            glCompileShader(sh);

            GLint ok = 0; glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
            if (!ok) {
                GLint len = 0; glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);
                std::string log((size_t)len, '\0');
                glGetShaderInfoLog(sh, len, &len, log.data());
                glDeleteShader(sh);
                EG_CORE_ERROR("Shader compile error:\n{}", log);
                EG_CORE_CHECK(false, "Compile failed");
            }
            glAttachShader(prog, sh);
            ids[idx++] = sh;
        }

        glLinkProgram(prog);
        GLint linked = 0; glGetProgramiv(prog, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint len = 0; glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
            std::string log((size_t)len, '\0');
            glGetProgramInfoLog(prog, len, &len, log.data());
            for (auto id : ids) if (id) glDeleteShader(id);
            glDeleteProgram(prog);
            EG_CORE_ERROR("Program link error:\n{}", log);
            EG_CORE_CHECK(false, "Link failed");
        }
        for (auto id : ids) if (id) { glDetachShader(prog, id); glDeleteShader(id); }

        m_Program = prog;
    }

} // namespace Engine
