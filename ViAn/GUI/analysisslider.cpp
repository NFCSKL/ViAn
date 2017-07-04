#include "analysisslider.h"
#include <QSlider>
#include <QBrush>
#include <QPaintEvent>
#include <QStylePainter>
#include <QStyleOptionSlider>
#include <QDebug>

AnalysisSlider::AnalysisSlider(Qt::Orientation orientation, QWidget * parent) : QSlider(parent) {
    setOrientation(orientation);
    setPageStep(10);
}

/**
 * @brief AnalysisSlider::paintEvent
 * New paint event for the slider that will also
 * draw the rectangle areas on the slider for showing POIs.
 * @param ev
 */
void AnalysisSlider::paintEvent(QPaintEvent *ev) {
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

    if (!frames.empty()) {
        for (auto it = frames.begin(); it != frames.end(); ++it) {
            int a = (*it) / maximum();
            int b = 10/maximum();
            QRect rect(groove_rect.left() + a * groove_rect.width(), groove_rect.top(), (a+b) * groove_rect.width(), groove_rect.height());
            painter.fillRect(rect, QBrush(Qt::red));
        }
    }
    option.subControls = QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, option);
}

/**
 * @brief AnalysisSlider::add_slider_rect
 * Adds a pair of doubles to a pair vector. This pair represents
 * a certain area of the slider from start to end.
 * @param start frame
 * @param end frame
 */
void AnalysisSlider::add_slider_interval(int start_frame, int end_frame) {
    double first, second;
    first = (double)start_frame/maximum();
    second = (double)end_frame/maximum();
    std::pair<double, double> pair;
    if (first > second) {
        pair = std::make_pair(second, first);
    } else {
        pair = std::make_pair(first, second);
    }
    rects.push_back(pair);
}

void AnalysisSlider::set_tag(Tag* tag) {
    frames = tag->frames;
}

/**
 * @brief AnalysisSlider::clear_slider
 * Clear the rects vector.
 */
void AnalysisSlider::clear_slider() {
    rects.clear();
}

void AnalysisSlider::set_blocked(bool value) {
    m_blocked = value;
}

bool AnalysisSlider::is_blocked() {
    return m_blocked;
}

bool AnalysisSlider::get_was_paused() {
    return m_was_paused;
}

void AnalysisSlider::set_was_paused(bool value) {
    m_was_paused = value;
}
