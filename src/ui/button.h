#pragma once

#include <raylib.h>
#include <string>
#include <functional>

struct Button{
public:
    struct Config{
        unsigned int borderColor;
        unsigned int baseColor;
        unsigned int labelColor;
        int borderWidth;
    };

    enum class Type{
        TOOLBAR
    };

public:
    Button(
        std::string label, 
        std::function<void()> action, 
        Config configs, 
        int index,
        Type type
    );

    void update();

    const Rectangle &getBounds() const;
    const std::string &getLabel() const;
    const std::function<void()> &getAction() const;
    const Config &getConfig() const;
    int getIndex() const;
    Type getType() const;

private:
    Rectangle bounds_;
    std::string label_;
    std::function<void()> action_;
    Config colors_;
    int index_;
    Type type_;
};