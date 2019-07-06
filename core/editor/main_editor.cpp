#include "main_editor.h"

bool trigger::edit::main_editor::draw() noexcept
{
    ImGui::Begin("Finally!");
    this->window_size = ImGui::GetWindowSize();
	this->window_pos = ImGui::GetWindowPos();
	ImGui::Text("%f : %f", window_pos.x, window_pos.y);
	ImGui::Text("%f : %f", window_size.x, window_size.y);

    ImGui::BeginTabBar("Test");
    if(ImGui::BeginTabItem("Editor View"))
    {
		current_tab = windows::editor;
        ImGui::EndTabItem();
    }
    static bool hello;

    if(ImGui::BeginTabItem("Game View"))
    {
		current_tab = windows::game_view;
        ImGui::Checkbox("Hello", &hello);
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Lua View"))
    {
		current_tab = windows::text_editor;
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