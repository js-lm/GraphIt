#include "canvas.h"

#define RAYGUI_IMPLEMENTATION
#include "lib/raygui.h"

int main(){
    const int screenWidth{800};
    const int screenHeight{600};

    InitWindow(screenWidth, screenHeight, "Graph Test");
    // SetTargetFPS(300);

    Canvas canvas;

    while (!WindowShouldClose()) {
        canvas.update();

        BeginDrawing(); {
            ClearBackground(RAYWHITE);
            canvas.drawGraph();
            canvas.drawTools();
        } EndDrawing();
    }

    CloseWindow();
    return 0;
}
