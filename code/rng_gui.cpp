#include "rng.h"
#include "rng_gui.h"
#include <ctime>
#include <string>
#include "font_inter_embed.cpp"

void InitUI(ui_state *State)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Register font
    // NOTE(liam): Inter-regular.ttf is embedded in the source code so we don't have to worry
    // about shipping fonts with the binaries
    ImFontConfig FontConfig = ImFontConfig();
    FontConfig.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF((void *)Inter_data, Inter_size, 16.0f * State->DpiScale, &FontConfig);

    // Setup Dear ImGui base style and then override
    ImGui::StyleColorsClassic();
    
    ImVec4 *colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
    colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = (ImVec4)ImColor(0.47058823529f, 0.12549019608f, 0.62745098039f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = (ImVec4)ImColor(0.37254901961f, 0.10196078431f, 0.50196078431f, 1.0f);
    colors[ImGuiCol_ButtonActive] = (ImVec4)ImColor(0.37254901961f, 0.10196078431f, 0.50196078431f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = (ImVec4)ImColor(0.47058823529f, 0.12549019608f, 0.62745098039f, 1.0f);
    ImGui::GetStyle().ScaleAllSizes(State->DpiScale);
}

void PushButtonTextColour()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
}

void PopButtonTextColour()
{
    ImGui::PopStyleColor(1);
}

global_variable ImVec2 DefaultMainPanelSize = ImVec2(400.0f, 275.0f);



inline void UpdateMainPanelSize(ui_state *State)
{
    ImVec2 ResultSize = ImVec2(DefaultMainPanelSize.x * State->DpiScale, DefaultMainPanelSize.y * State->DpiScale);   
    State->MainPanelSize = ResultSize;
}

ui_state InitUiState()
{
    ui_state State = {};
    State.ShowFlags = SHOW_NONE;
    State.ExpandFlags = EXPAND_NONE;
    State.ClearColor = ImVec4(0.96862745098f, 0.96078431373f, 0.97254901961f, 1.0f);
    State.ButtonSize = ImVec2(80.0f, 25.0f);
    State.MainPanelSize = DefaultMainPanelSize;
    State.DpiScale = 1.f;
    return State;
}

void ResetUiState(ui_state *State)
{
}

// This is the Central UI Loop of the application
// As this function is not in the platform layer, it's nice and simple to add new functionality to application
// cross plat.
void RenderUi(ui_state *State)
{
    PlatformImguiStartFrame();
    UpdateMainPanelSize(State);

    if ((State->ShowFlags & SHOW_DEMO) == SHOW_DEMO)
        ImGui::ShowDemoWindow((bool *)1);

    {
        
        ImGui::SetNextWindowSize(State->MainPanelSize);
        ImGui::Begin(
            "C GUI App",
            (bool *)1,
            ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoScrollbar |
                ImGuiWindowFlags_NoScrollWithMouse); // Create a virtual ImGui window

        
        
        ImGui::End();
        ImVec2 AboutPos = ImVec2(60, State->MainPanelSize.y + (60 + 50 * State->DpiScale));
        ImGui::SetNextWindowPos(AboutPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(400 * State->DpiScale, 57 * State->DpiScale), ImGuiCond_FirstUseEver);
        ImGui::Begin("About");
        ImGui::Text("Version: %s", VERSION);
        ImGui::End();

#if RNG_INTERNAL
        const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(State->DpiScale * (main_viewport->WorkPos.x + 650), State->DpiScale * (main_viewport->WorkPos.y + 20)), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(550 * State->DpiScale, 680 * State->DpiScale), ImGuiCond_FirstUseEver);
        ImGui::Begin("DEBUG");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
#endif

        ImGui::Render();
        PlatformImguiRender(State);
    }
}



void ShutdownUi()
{
    PlatformImguiShutdown();
    ImGui::DestroyContext();
}
