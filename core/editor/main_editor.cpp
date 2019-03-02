#include "main_editor.h"

bool trigger::edit::main_editor::draw() const noexcept
{
    ImGui::Begin("Finally!");
    ImGui::Text("Dx12 + new trigger engine");

    ImGui::BeginTabBar("Test");
    if(ImGui::BeginTabItem("Editor View"))
    {
        ImGui::EndTabItem();
    }
    static bool hello;

    if(ImGui::BeginTabItem("Game View"))
    {
        ImGui::Checkbox("Hello", &hello);
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Lua View"))
    {
        lua_editor.Render("TEST");

        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
    ImGui::End();
    return true;
};

void trigger::edit::main_editor::update(float delta) noexcept
{

};