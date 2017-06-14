#include "myslider.h"
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QSlider>
#include <QBrush>
#include <QPaintEvent>
#include <iostream>

/**
 * @brief MySlider::MySlider
 * Constructor
 * @param parent
 */
MySlider::MySlider(QWidget *parent) : QSlider(parent) {
    this->setOrientation(Qt::Horizontal);
}

/**
 * @brief MySlider::paintEvent
 * New paint event for the slider that will also
 * draw the rectangle areas on the slider for showing POIs.
 * @param ev
 */
void MySlider::paintEvent(QPaintEvent *ev) {
    QStyleOptionSlider option;
    initStyleOption(&option);
    QStylePainter painter(this);
    QSlider::paintEvent(ev);

    option.subControls = QStyle::SC_SliderGroove;
    painter.drawComplexControl(QStyle::CC_Slider, option);
    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);

    if (!rects.empty()) {
        for (auto it = rects.begin(); it != rects.end(); ++it) {
            QRect rect(groove_rect.left() + (*it).first * groove_rect.width(), groove_rect.top(), ((*it).second - (*it).first) * groove_rect.width(), groove_rect.height());
            painter.fillRect(rect, QBrush(Qt::yellow));
        }
    }
    option.subControls = QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, option);
}

/**
 * @brief MySlider::add_slider_rect
 * Adds a pair of doubles to a pair vector. This pair represents
 * a certain area of the slider from start to end.
 * @param start in the range 0-1
 * @param end in the range 0-1, should be > than start
 */
void MySlider::add_slider_rect(double start, double end) {
    std::pair<double, double> pair = std::make_pair(start, end);
    rects.push_back(pair);
}

/**
 * @brief MySlider::clear_rects
 * Clear the rects vector.
 */
void MySlider::clear_rects() {
    rects.clear();
}
