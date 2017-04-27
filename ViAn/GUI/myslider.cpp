#include "myslider.h"
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QSlider>
#include <QBrush>

MySlider::MySlider(QWidget *parent) :
    QSlider(parent)
{
}

void MySlider::paintEvent(QPaintEvent *ev) {
    QSlider::paintEvent(ev);
    QStylePainter p(this);
    QStyleOptionSlider option;
    initStyleOption(&option);

    option.subControls = QStyle::SC_SliderGroove | QStyle::SC_SliderHandle;

    QRect groove_rect = style()->subControlRect(QStyle::CC_Slider, &option, QStyle::SC_SliderGroove, this);
    QRect rect(groove_rect.left() + 0.2 * groove_rect.width(), groove_rect.top(), 0.6 * groove_rect.width(), groove_rect.height());
    //p.drawComplexControl(QStyle::CC_Slider, option);

    //p.drawComplexControl(QStyle::CC_Slider, option);
    p.fillRect(rect, QBrush(Qt::red));
}
