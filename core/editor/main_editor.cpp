#include "main_editor.h"

bool trigger::edit::main_editor::draw() const noexcept
{
    ImGui::Begin("Finally!");
    ImGui::Text("Dx12 + new trigger engine");
    ImGui::End();
    return true;
};

void trigger::edit::main_editor::update(float delta) noexcept
{

};