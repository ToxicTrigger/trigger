#ifndef MAIN_EDITOR_H
#define MAIN_EDITOR_H

#include "impl_editor.h"
#include "../../ImGuiColorTextEdit/TextEditor.h"
#include <string>

static TextEditor lua_editor;
namespace trigger::edit
{
    class main_editor : public impl_editor
    {
        public:
            TextEditor::LanguageDefinition lang;
            std::string TEST;

        public:
            explicit main_editor()
            {
                lang = TextEditor::LanguageDefinition::Lua();
                lua_editor.SetLanguageDefinition(lang);
                lua_editor.SetText(this->TEST);
            }
            virtual bool draw() const noexcept override;
            virtual void update(float delta) noexcept override;
    };
}


#endif