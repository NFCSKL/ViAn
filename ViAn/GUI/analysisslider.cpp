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

    if (m_show_pois) {
        for (auto it = rects.begin(); it != rects.end(); ++it) {
            std::cout << "first - second " << groove_rect.left() + (double)(*it).first * groove_rect.width() / maximum() << " - " << ((double)(*it).second - (*it).first) * groove_rect.width() / maximum()  << std::endl;
            QRect rect(groove_rect.left() + (double)(*it).first * groove_rect.width() / maximum(), groove_rect.top(),
                       ((double)(*it).second - (*it).first) * groove_rect.width() / maximum(), groove_rect.height());
            painter.fillRect(rect, QBrush(Qt::yellow));
        }
    }

    if (m_show_tags) {
        double c = (double)(groove_rect.right()-groove_rect.left())/maximum();
        for (auto it = frames.begin(); it != frames.end(); ++it) {
            double frame = (double)(*it);
            double first = (groove_rect.left()+frame*c);
            QRect rect(first, groove_rect.top(), 1, groove_rect.height());
            painter.fillRect(rect, QBrush(Qt::red));
        }
    }
    option.subControls = QStyle::SC_SliderHandle;
    painter.drawComplexControl(QStyle::CC_Slider, option);
}

/**
 * @brief AnalysisSlider::set_analysis
 * Adds all POIs to the slider
 * @param analysis
 */
void AnalysisSlider::set_analysis(Analysis* analysis) {
    for (POI p : analysis->POIs) {
        add_slider_interval(p.start_frame, p.end_frame);
    }
}

/**
 * @brief AnalysisSlider::add_slider_rect
 * Adds a pair of doubles to a pair vector. This pair represents
 * a certain area of the slider from start to end.
 * @param start frame
 * @param end frame
 */
void AnalysisSlider::add_slider_interval(int start_frame, int end_frame) {
    std::pair<int, int> pair;
    if (start_frame > end_frame) {
        pair = std::make_pair(end_frame, start_frame);
    } else {
        pair = std::make_pair(start_frame, end_frame);
    }
    rects.push_back(pair);
    last_poi_end = end_frame;
}

/**
 * @brief AnalysisSlider::get_next_poi_start
 * Return the start frame of the POI after frame
 * @param frame     : current frame
 * @return
 */
int AnalysisSlider::get_next_poi_start(int curr_frame) {
    if (!rects.empty()) {
        for (std::pair<int, int> rect : rects) {
            if ( rect.first > curr_frame) {
                return rect.first;
            }
        }
    } else if (!frames.empty()) {
        for (int frame : frames) {
            if (frame > curr_frame) {
                return frame;
            }
        }
    }
    return curr_frame;
}

/**
 * @brief AnalysisSlider::get_next_poi_end
 * Return the end frame of the POI after frame
 * @param frame     : current frame
 * @return
 */
int AnalysisSlider::get_next_poi_end(int frame) {
    for (std::pair<int, int> rect : rects) {
        if ( rect.first > frame) {
            return rect.second;
        }
    }
    return frame;
}

/**
 * @brief AnalysisSlider::get_prev_poi_start
 * Return the start frame of the POI before frame
 * @param frame     : current frame
 * @return
 */
int AnalysisSlider::get_prev_poi_start(int curr_frame) {
    int new_frame = curr_frame;
    if (!rects.empty()) {
        for (std::pair<int, int> rect : rects) {
            if ( rect.second > curr_frame) {
                break;
            } else new_frame = rect.first;
        }
    } else if (!frames.empty()) {
        for (int frame : frames) {
            if (frame >= curr_frame) {
                break;
            } else new_frame = frame;
        }
    }
    return new_frame;
}

/**
 * @brief AnalysisSlider::is_in_POI
 * Checks to see if frame is in a POI.
 * @param frame
 * @return
 */
bool AnalysisSlider::is_in_POI(int frame) {
    for (std::pair<int, int> rect : rects) {
        if (frame >= rect.first && frame < rect.second) {
            return true;
        }
    }return false;
}

/**
 * @brief AnalysisSlider::set_show_pois
 * @param show_pois
 */
void AnalysisSlider::set_show_pois(bool show_pois) {
    m_show_pois = show_pois;
    repaint();
}

/**
 * @brief AnalysisSlider::set_show_tags
 * @param show_tags
 */
void AnalysisSlider::set_show_tags(bool show_tags) {
    std::cout << "set show tags" << std::endl;
    m_show_tags = show_tags;
    repaint();
}

void AnalysisSlider::set_tag(Tag* tag) {
    std::cout << "set me" << std::endl;
    frames = tag->frames;
    for (int i : frames) {
        std::cout << i << std::endl;
    }
}

/**
 * @brief AnalysisSlider::clear_slider
 * Clear the rects vector.
 */
void AnalysisSlider::clear_slider() {
    rects.clear();
}

void AnalysisSlider::clear_tags() {
    frames.clear();
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
