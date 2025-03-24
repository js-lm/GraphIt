#pragma once

namespace UI{

    class Panel{
    public:
        enum class ID{
            TOOLBAR,
            SAVE,
            LOAD,
            CONFIRM_REPLACE,
            SETTINGS
        };

    public:
        Panel() = default;
        virtual ~Panel() = default;

        virtual void draw() = 0;
        virtual void update() = 0;

    protected:
        ID id_;
    };

} // namespace UI