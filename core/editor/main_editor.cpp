#include "main_editor.h"
#include "../core/game/object_renderer.h"

bool trigger::edit::main_editor::draw() noexcept
{
    {
        ImGui::Begin("Finally!");
        this->window_size = ImGui::GetWindowSize();
        this->window_pos = ImGui::GetWindowPos();
        ImGui::Text("%f : %f", window_pos.x, window_pos.y);
        ImGui::Text("%f : %f", window_size.x, window_size.y);

        ImGui::BeginTabBar("Test");
        if (ImGui::BeginTabItem("Editor View"))
        {
            current_tab = windows::editor;
            if (ImGui::Button("Create Object"))
            {
                auto tmp = new trigger::transform();
                this->world->add(tmp);
            }

            {
                auto objs = this->world->get_all();

                if (this->current_id != 0)
                {
                    ImGui::Text("%s", objs[current_id]->get_name().c_str());
                    ImGui::Text("Instance ID : %d", this->current_id);
                    auto comps = objs[this->current_id]->get_components();
                    for (auto c : comps)
                    {
                        ImGui::Separator();
                        ImGui::Text(c->get_type_name().c_str());
                        for (auto p : c->properties)
                        {
                            switch (p.second.type)
                            {
                            case trigger::property::data_type::Float:
                                float f = dynamic_cast<trigger::comp::object_renderer *>(c)->t;
                                ImGui::Text(p.second.get_name().c_str());
                                ImGui::SameLine();
                                ImGui::InputFloat("", &f);
                                break;
                            }
                        }
                        ImGui::Separator();
                    }
                }
            }

            ImGui::EndTabItem();
        }
        static bool hello;

        if (ImGui::BeginTabItem("Game View"))
        {
            current_tab = windows::game_view;
            ImGui::Checkbox("Hello", &hello);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Lua View"))
        {
            current_tab = windows::text_editor;
            lua_editor.Render("TEST");

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
        ImGui::End();
    }

    return true;
};

void trigger::edit::main_editor::update(float delta) noexcept {

};