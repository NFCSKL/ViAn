#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QSlider>
#include <vector>

class MySlider : public QSlider {

public:
    explicit MySlider(QWidget *parent = 0);
    void add_slider_rect(double start, double end);
    void clear_rects();

protected:
    void paintEvent(QPaintEvent *ev);

private:
    std::vector<std::pair<double, double>> rects;
};

#endif // MYSLIDER_H
