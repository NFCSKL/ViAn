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
MySlider::MySlider(QSlider *parent) {
    slider = parent;
}

void MySlider::paintEvent(QPaintEvent *ev) {
    //std::cout << "paintEvent" << std::endl;


    QStyleOptionSlider option;
    initStyleOption(&option);

    option.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;
    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
    QSlider::paintEvent(ev);
    QRect rect(groove_rect.left() + 0.2 * groove_rect.width(), groove_rect.top(), 0.6 * groove_rect.width(), groove_rect.height());
    qDebug() << groove_rect;

    QStylePainter painter(this);
    //painter.drawComplexControl(QStyle::CC_Slider, option);

    //painter.drawComplexControl(QStyle::CC_Slider, option);
    painter.fillRect(rect, QBrush(Qt::yellow));
    this->setStyleSheet("background: blue");
}

void MySlider::set_background() {
    std::cout << "paintEvent" << std::endl;
    //slider->setStyleSheet("background: pink");
    slider->repaint();
}
