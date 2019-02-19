#ifndef MAIN_EDITOR_H
#define MAIN_EDITOR_H

#include "impl_editor.h"

namespace trigger
{
    namespace edit
    {
        class impl_editor;
        class main_editor : public impl_editor
        {
            public:
            virtual bool draw() const noexcept override;
            virtual void update(float delta) noexcept override;
        };
    }
}


#endif