#include "enginepch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Engine/Core/Application.h"
#include "Engine/Core/Window.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine {

    void ImGuiLayer::ApplyDarkTheme()
    {
        EG_PROFILE_FUNCTION();

        ImGui::StyleColorsDark();
        auto& style = ImGui::GetStyle();

        style.WindowRounding = 6.0f;
        style.FrameRounding = 5.0f;
        style.GrabRounding = 5.0f;
        style.ScrollbarRounding = 8.0f;

        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.11f, 0.12f, 1.0f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.07f, 0.08f, 0.09f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.16f, 0.18f, 1.0f);
        colors[ImGuiCol_Header] = ImVec4(0.20f, 0.21f, 0.24f, 1.0f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.30f, 0.34f, 1.0f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.26f, 0.30f, 1.0f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.17f, 0.19f, 1.0f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.22f, 0.23f, 0.26f, 1.0f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.28f, 0.31f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.22f, 1.0f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.27f, 0.29f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.35f, 0.38f, 1.0f);
    }

    ImGuiLayer::ImGuiLayer(const Options& opts)
        : Layer("ImGuiLayer"), m_Opts(opts)
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
        if (m_Initialized)
            OnDetach();
    }

    ImFont* ImGuiLayer::AddFontFromFile(const char* path, float sizePixels, bool setAsDefault)
    {
        EG_PROFILE_FUNCTION();

        IM_ASSERT(ImGui::GetCurrentContext() != nullptr && "AddFontFromFile: call after OnAttach()");
        auto& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF(path, sizePixels);
        if (setAsDefault && font)
            io.FontDefault = font;
        return font;
    }

    void ImGuiLayer::OnAttach()
    {
        if (m_Initialized) return;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        if (m_Opts.EnableDocking)   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (m_Opts.EnableViewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ApplyDarkTheme();

        // Pobierz okno z aplikacji
        GLFWwindow* glfwWin = nullptr;
        {
            Application& app = Application::Get();
            glfwWin = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        }

        // Jeœli nie mamy wa¿nego okna – **przerwij bez inicjalizacji backendów**
        if (!glfwWin) {
            // Zostaw kontekst ImGui, ale nie w³¹czaj backendów – nic nie bêdzie renderowane
            m_Initialized = false;
            return;
        }

        ImGui_ImplGlfw_InitForOpenGL(glfwWin, false);
        ImGui_ImplOpenGL3_Init("#version 410");
        m_Initialized = true;
    }


    void ImGuiLayer::OnDetach()
    {
        EG_PROFILE_FUNCTION();

        if (!m_Initialized)
            return;

        {
            EG_PROFILE_SCOPE("ImGui_ImplOpenGL3_Shutdown");
            ImGui_ImplOpenGL3_Shutdown();
        }
        {
            EG_PROFILE_SCOPE("ImGui_ImplGlfw_Shutdown");
            ImGui_ImplGlfw_Shutdown();
        }
        ImGui::DestroyContext();

        m_Initialized = false;
    }

    void ImGuiLayer::Begin()
    {
        EG_PROFILE_FUNCTION();

        if (!m_Initialized)
            return;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End()
    {
        EG_PROFILE_FUNCTION();

        if (!m_Initialized)
            return;

        ImGuiIO& io = ImGui::GetIO();

        uint32_t fbw = 0, fbh = 0;
        auto& window = Application::Get().GetWindow();
        window.GetFramebufferSize(fbw, fbh);
        io.DisplaySize = ImVec2(static_cast<float>(fbw), static_cast<float>(fbh));

        ImGui::Render();

        {
            EG_PROFILE_SCOPE("ImGui_ImplOpenGL3_RenderDrawData");
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            EG_PROFILE_SCOPE("ImGui Multi-Viewport Render");
            GLFWwindow* backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }
    }

} // namespace Engine
