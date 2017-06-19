#include "drawingtoolbar.h"

#include <QIcon>

DrawingToolbar::DrawingToolbar() {
    create_actions();
    create_buttons();
}

void DrawingToolbar::create_actions() {
    color_tool_act = new QAction(QIcon("../ViAn/Icons/color.png"), tr("Color picker"), this);
    pen_tool_act = new QAction(QIcon("../ViAn/Icons/pen.png"), tr("Pen tool"), this);
    arrow_tool_act = new QAction(QIcon("../ViAn/Icons/arrow.png"), tr("Arrow tool"), this);
    box_tool_act = new QAction(QIcon("../ViAn/Icons/box.png"), tr("Rectangle tool"), this);
    circle_tool_act = new QAction(QIcon("../ViAn/Icons/circle.png"), tr("Circle tool"), this);
    line_tool_act = new QAction(QIcon("../ViAn/Icons/line.png"), tr("Line tool"), this);
    text_tool_act = new QAction(QIcon("../ViAn/Icons/text.png"), tr("Text tool"), this);
    undo_tool_act = new QAction(QIcon("../ViAn/Icons/undo.png"), tr("Undo last drawing"), this);
    clear_tool_act = new QAction(QIcon("../ViAn/Icons/clear.png"), tr("Clear all drawings"), this);


}

void DrawingToolbar::create_buttons() {
    addAction(color_tool_act);
    addAction(pen_tool_act);
    addAction(arrow_tool_act);
    addAction(box_tool_act);
    addAction(circle_tool_act);
    addAction(line_tool_act);
    addAction(text_tool_act);
    addAction(undo_tool_act);
    addAction(clear_tool_act);
}
