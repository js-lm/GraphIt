#include "application.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

int main(){
    Application &app{Application::instance()};
    app.run();
    return 0;
}
