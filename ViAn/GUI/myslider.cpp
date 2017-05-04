#include "myslider.h"
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QSlider>
#include <QBrush>
#include <QPaintEvent>
#include <iostream>
#include <QDebug>

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

    //QRect rect(groove_rect.left() + 0.2 * groove_rect.width(), groove_rect.top(), 0.6 * groove_rect.width(), groove_rect.height());

    /*for (vector<QRect>::iterator i = rects.begin(); i != rects.end(); ++i) {
        painter.fillRect(i, QBrush(Qt::yellow));
    }*/
    if (!rects.empty()) {
        for (auto it = rects.begin(); it != rects.end(); ++it) {
            QRect rect(groove_rect.left() + (*it).first * groove_rect.width(), groove_rect.top(), ((*it).second - (*it).first) * groove_rect.width(), groove_rect.height());
            painter.fillRect(rect, QBrush(Qt::yellow));
        }
    }

    option.subControls = QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, option);
}

void MySlider::add_slider_rect(double start, double end) {
    std::pair<double, double> pair = std::make_pair(start, end);
    rects.push_back(pair);
}

void MySlider::clear_rects() {
    rects.clear();
}
