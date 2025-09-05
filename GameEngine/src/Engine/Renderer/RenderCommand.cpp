#include "enginepch.h"
#include "RenderCommand.h"
#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace Engine {

    std::unique_ptr<RendererAPI>& RenderCommand::API() {
        static std::unique_ptr<RendererAPI> sApi;
        if (!sApi) {
            // one simple switch here if more APIs appear in future
            sApi = std::make_unique<OpenGLRendererAPI>();
        }
        return sApi;
    }

} // namespace Engine
