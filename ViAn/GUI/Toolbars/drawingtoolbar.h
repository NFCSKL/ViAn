#ifndef DRAWINGTOOLBAR_H
#define DRAWINGTOOLBAR_H

#include <QToolBar>
#include <QAction>

class DrawingToolbar : public QToolBar
{
    void create_actions();
    void create_buttons();
public:
    DrawingToolbar();

    QAction* color_tool_act;
    QAction* pen_tool_act;
    QAction* arrow_tool_act;
    QAction* box_tool_act;
    QAction* circle_tool_act;
    QAction* line_tool_act;
    QAction* text_tool_act;
    QAction* undo_tool_act;
    QAction* clear_tool_act;
};

#endif // DRAWINGTOOLBAR_H
