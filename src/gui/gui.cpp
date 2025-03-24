#include "gui.h"
#include "system/application.h"
#include "actions_center/actions_center.h"
#include "canvas/canvas.h"
#include "io/io.h"

#define GUI_TOOLBAR_IMPLEMENTATION
#include "layout/gui_toolbar.h"

#if defined(__APPLE__)
    const int COMMAND_KEY{KEY_LEFT_SUPER};
#else
    const int COMMAND_KEY{KEY_LEFT_CONTROL};
#endif

GuiToolbarState ui{InitGuiToolbar()};

GUI::GUI()
    : panel_(Panel::NONE)
{
    resetLoadDirectory();
}

void GUI::draw(){
    DrawRectangleRec({0, 0, 1000, 40}, Fade(WHITE, .75f));
    DrawRectangleRec({0, 680, 1000, 40}, Fade(WHITE, .75f));
    GuiToolbar(&ui);

    drawPanels();
}

void GUI::update(){
    updateKeyboardShortcuts();

    updateFile();
    updateView();
    updateSettings();
    updateConstruction();
    updateControls();

    updateLoadPanel();
}

void switchMode(Canvas::Mode mode){
    Application::instance().canvas().switchMode(mode);
    ui.currentSelectedTool = static_cast<int>(mode);
}

void undo(){ Application::instance().actionCenter().undo();}
void redo(){ Application::instance().actionCenter().redo();}

void bulkDeleteVertex(){ Application::instance().canvas().doBulkDeleteVertices();}
void bulkDeleteEdge(){ Application::instance().canvas().doBulkDeleteEdges();}
void bulkDelete(){ 
    if(ui.isSelectingVertexChecked && ui.isSelectingEdgeChecked){
        Application::instance().canvas().doBulkDelete();
    }else if(ui.isSelectingVertexChecked){
        bulkDeleteVertex();
    }else if(ui.isSelectingEdgeChecked){
        bulkDeleteEdge();
    }
}

void dyeVertex(){ Application::instance().canvas().doDyeVertex();}
void dyeEdge(){ Application::instance().canvas().doDyeEdge();}
void dye(){ 
    if(ui.isSelectingVertexChecked && ui.isSelectingEdgeChecked){
        Application::instance().canvas().doDye();
    }else if(ui.isSelectingVertexChecked){
        dyeVertex();
    }else if(ui.isSelectingEdgeChecked){
        dyeEdge();
    }
}

void GUI::updateKeyboardShortcuts(){
    auto key{GetKeyPressed()};
    bool hasPress{false};

    if(IsKeyDown(COMMAND_KEY)){
        if(IsKeyDown(KEY_LEFT_SHIFT)){
            hasPress = updateCommandShiftKeys(key);
        }else{
            hasPress = updateCommandKeys(key);
        }
    }

    if(!hasPress) updateKeys(key);
}

bool GUI::updateKeys(int key){
    // No Modifier Keys
    switch(key){
    case KEY_P: switchMode(Canvas::Mode::PEN);    return true;
    case KEY_L: switchMode(Canvas::Mode::LINK);   return true;
    case KEY_E: switchMode(Canvas::Mode::ERASER); return true;
    case KEY_M: switchMode(Canvas::Mode::MOVE);   return true;
    case KEY_S: switchMode(Canvas::Mode::SELECT); return true;
    case KEY_Q: switchMode(Canvas::Mode::PAN);    return true;

    case KEY_DELETE: bulkDelete(); return true;
    // case KEY_BACKSPACE: 

    case KEY_R: 
        Application::instance().canvas().resetCamera();
        return true;
    
    default: break;
    }
    return false;
}

bool GUI::updateCommandKeys(int key){
    // Control
    switch(key){
    case KEY_Z: undo(); return true;
    case KEY_X: bulkDelete(); return true;
    default: break;
    }
    return false;
}

bool GUI::updateCommandShiftKeys(int key){
    // Control + Shift
    switch(key){
    case KEY_Z: redo(); return true;
    default: break;
    }
    return false;
}

void GUI::updateFile(){
    if(ui.newFilePressed){

    }

    if(ui.savePressed){
        Application::instance().serializer().save("save.grt");
    }

    if(ui.loadPressed) switchPanel(Panel::LOAD_FROM);
    if(ui.undoPressed) undo();
    if(ui.redoPressed) redo();
    if(ui.takeScreenshotPressed) takeScreenshot();

    // TODO: I don't want this to be continuously update on every frame
    ui.canUndo = Application::instance().actionCenter().canUndo();
    ui.canRedo = Application::instance().actionCenter().canRedo();
}

void GUI::updateView(){
    bool isSnapToGridEnabled{Application::instance().canvas().isSnapToGridEnabled()};
    if((!ui.snapToGridActive && isSnapToGridEnabled)
    || (ui.snapToGridActive && !isSnapToGridEnabled)
    ){
        Application::instance().canvas().toggleSnapToGrid(!isSnapToGridEnabled);
    }

    bool isGridShown{Application::instance().canvas().isGridShown()};
    if((!ui.showGridActive && isGridShown)
    || (ui.showGridActive && !isGridShown)
    ){
        Application::instance().canvas().toggleGrid(!isGridShown);
    }
}

void GUI::updateSettings(){
    // state.isDirectedChecked = false;
    // state.isWeightedChecked = false;
    // state.isShowingLabelsChecked = false;

    if(ui.openSettingPressed){

    }
}

void GUI::updateConstruction(){
    if(ui.penPressed) switchMode(Canvas::Mode::PEN);
    if(ui.linkPressed) switchMode(Canvas::Mode::LINK);
    if(ui.penColorPressed) switchPanel(Panel::COLOR_PANEL_PEN);
    if(ui.linkColorPressed) switchPanel(Panel::COLOR_PANEL_LINK);
    if(ui.dragPressed) switchMode(Canvas::Mode::MOVE);
    if(ui.eraserPressed) switchMode(Canvas::Mode::ERASER);
}

void GUI::updateControls(){
    if(ui.selectPressed) switchMode(Canvas::Mode::SELECT);
    if(ui.movePressed) switchMode(Canvas::Mode::PAN);
    if(ui.colorPanelButtonPressed) switchPanel(Panel::COLOR_PANEL_DYE);
    if(ui.deleteSelectedPressed) bulkDelete();
    if(ui.changeSelectedColorPressed) dye();

    // state.isSelectingVertexChecked = false;
    // state.isSelectingEdgeChecked = false;
}