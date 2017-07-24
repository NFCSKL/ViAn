#ifndef OVERLAY_H
#define OVERLAY_H

#include <iostream>
#include <QImage>
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QInputDialog>
#include "shapes/shape.h"
#include "shapes/rectangle.h"
#include "shapes/circle.h"
#include "shapes/line.h"
#include "shapes/arrow.h"
#include "shapes/pen.h"
#include "shapes/text.h"
#include "shapes/zoomrectangle.h"
#include "shapes/analysarea.h"
#include "Library/customdialog.h"
#include "opencv2/opencv.hpp"

class Overlay : public QObject {
    Q_OBJECT

public slots:
    void set_tool(SHAPES s);
public:
    Overlay();
    bool is_showing_overlay();
    void set_showing_overlay(bool value);
    void draw_overlay(cv::Mat &frame, int frame_nr);

    void set_colour(QColor col);
    QColor get_colour();
    SHAPES get_tool();
    void mouse_pressed(QPoint pos, int frame_nr);
    void mouse_released(QPoint pos, int frame_nr);
    void mouse_moved(QPoint pos, int frame_nr);
    void undo(int frame_nr);
    void redo(int frame_nr);
    void clear(int frame_nr);

    void read(const QJsonObject& json);
    void write(QJsonObject& json);

private:
    void update_drawing_position(QPoint pos, int frame_nr);
    Shape* get_empty_shape(SHAPES shape_type);
    void empty_undo_list(int frame_nr);
    void add_drawing(Shape *shape, int frame_nr);

    bool show_overlay = true;

    SHAPES current_shape = RECTANGLE;
    QColor current_colour = Qt::red;
    QString current_string = "Enter text";
    float current_font_scale = 1;

    struct FrameOverlay{
        std::vector<Shape*> overlay;
        std::vector<Shape*>::iterator drawn = overlay.end();
    };

    std::map<int, FrameOverlay> overlays;
};

#endif // OVERLAY_H
