#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QSlider>

class MySlider : public QSlider {

public:
    explicit MySlider(QSlider *parent = 0);
    void set_background();

protected:
    void paintEvent(QPaintEvent *ev);

private:
    QWidget* slider;
};

#endif // MYSLIDER_H
