void ApplyImGuiTheme()
{
    ImGuiStyle* style = &ImGui::GetStyle();
    style->FrameRounding = 4.0f;
    style->WindowBorderSize = 0.0f;
    style->PopupBorderSize = 0.0f;
    style->GrabRounding = 4.0f;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f); // Убрана прозрачность, фон теперь полностью непрозрачный
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Тёмно-серый цвет для фона выпадающего списка
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Тёмно-серый для фона неактивного комбобокса
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f); // Немного светлее тёмно-серого для фона комбобокса при наведении
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Ещё чуть светлее тёмно-серого для фона активного комбобокса
    colors[ImGuiCol_TitleBg] = ImVec4(1.00f * 0.5, 0.55f * 0.5, 0.00f, 0.67f); // Темнее оранжевый для заголовка окна
    colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f * 0.5, 0.55f * 0.5, 0.00f, 1.00f); // Темнее оранжевый когда окно активно
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f * 0.5, 0.55f * 0.5, 0.00f, 0.67f); // Темнее оранжевый для свёрнутого заголовка
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.34f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.55f, 0.00f, 1.00f); // Оранжевый для галочек
    colors[ImGuiCol_SliderGrab] = ImVec4(0.71f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.84f, 0.66f, 0.66f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(1.00f, 0.55f, 0.00f, 0.65f); // Оранжевый для кнопок
    colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.65f, 0.00f, 0.65f); // Оранжевый при наведении
    colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.55f, 0.00f, 0.80f); // Оранжевый при нажатии
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Тёмно-серый для фона элемента выпадающего списка
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f); // Немного светлее тёмно-серого для фона элемента при наведении
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f); // Ещё чуть светлее тёмно-серого для фона активного элемента
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
    colors[ImGuiCol_Tab] = ImVec4(0.71f, 0.39f, 0.39f, 0.54f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
    colors[ImGuiCol_TabActive] = ImVec4(0.84f, 0.66f, 0.66f, 0.66f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
enum class LUA_CMD
{
    BLOCK_GUI = 1011,
};
void ExecuteLuaCommand(LUA_CMD cmd, std::string argument)
{
    HackInjectAvailable = true;
    char cmd_str[256]; memset(cmd_str, 0, sizeof(cmd_str));
    sprintf(cmd_str, xorstr_("%d"), (int)cmd);
    gLuaCode = cmd_str;
    gLuaArg = argument;
}
std::optional<HRESULT> on_present(const decltype(present_hook)& hook, IDirect3DDevice9* device_ptr,
const RECT*, const RECT*, HWND wnd, const RGNDATA*)
{
    static bool ImGui_inited = false;
    if (!ImGui_inited)
    {
        ImGui::CreateContext();
        ApplyImGuiTheme();
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig fontCfg = ImFontConfig();
        io.DeltaTime = 1.0f / 60.0f;
        fontCfg.RasterizerMultiply = 1.0f;
        fontCfg.OversampleV = 1;
        fontCfg.OversampleH = fontCfg.OversampleV;
        fontCfg.PixelSnapH = true;
        ImWchar ranges[] = {
        0x0020, 0x007F, // Основной латинский диапазон
        0x0400, 0x0450, // Диапазон для русских символов, также включает некоторые украинские
        0x0454, 0x0454, // Добавляем символ "є"
        0x0456, 0x0456, // Добавляем символ "і"
        0x0457, 0x0457, // Добавляем символ "ї"
        0x0491, 0x0491, // Добавляем символ "ґ"
        0x0000 // Заканчиваем массив нулевым символом
            };
        io.Fonts->AddFontFromFileTTF(xorstr_("Shrift.otf"), 16.0f, &fontCfg, ranges);
        D3DXCreateTextureFromFileA(device_ptr, xorstr_("texture.jpg"), &texture);
        ImGui_ImplWin32_Init(**reinterpret_cast<HWND**>(0xC17054));
        ImGui_ImplDX9_Init(device_ptr);
        ImGui_inited = true;
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    if (ShowMenu)
    {
        POINT p; GetCursorPos(&p); ScreenToClient(gameWnd, &p);
        ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(p.x - 10, p.y), ImVec2(p.x, p.y + 10), ImVec2(p.x + 10, p.y), ImColor(185, 17, 17));
        ImGui::SetNextWindowSize(ImVec2(610, 500));
        char capWin[350]; memset(capWin, 0, sizeof(capWin));
        sprintf(capWin, cp1251_to_utf8(xorstr_("U-Mod by DroidZero | build %s | t.me/ugta_cheats")).c_str(), PLUGIN_VER);
        ImGui::Begin(capWin, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
        ImGui::Image(texture, ImVec2(610, 166));
        ImGui::BeginChild(xorstr_("main"), ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);
        ////////////////////// gui code //////////////////////////////////////////////////////////
        static const char* current_select = "All players are Offline!";
        if (ImGui::BeginChild(xorstr_("ChildDrawList"), ImVec2(605, 200), true))
        {
            for (const auto& item : CModelsList)
            {
                bool is_selected = (current_select == item.first.c_str());
                if (ImGui::Selectable(item.first.c_str(), is_selected))
                {
                    current_select = item.first.c_str();
                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                        //VictimNick = item;
                    }
                }
            }
            ImGui::EndChild();
        }
        if (ImGui::Button(cp1251_to_utf8(xorstr_("Обновить список")).c_str()))
        {

        }
        ImGui::SameLine();
        if (ImGui::Button(cp1251_to_utf8(xorstr_("Применить")).c_str()))
        {
            
        }
        ImGui::SameLine();
        if (ImGui::Button(cp1251_to_utf8(xorstr_("Сбросить")).c_str()))
        {

        }
        const char* mtype_items[] = { xorstr_("Player"), xorstr_("Vehicle"), xorstr_("Weapon") };
        static const char* mtype_current = NULL;
        if (ImGui::BeginCombo(cp1251_to_utf8(xorstr_("Тип модели")).c_str(), mtype_current))
        {
            for (int n = 0; n < IM_ARRAYSIZE(mtype_items); n++)
            {
                bool is_selected = (mtype_current == mtype_items[n]);
                if (ImGui::Selectable(mtype_items[n], is_selected)) mtype_current = mtype_items[n];
                {
                    if (is_selected) ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::BulletText(status_message.c_str());
        /////////////////////////////////////////////////////////////////////////////////////////
        ImGui::EndChild();
        ImGui::End();
    }
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
    return std::nullopt;
}