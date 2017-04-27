#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QSlider>

class MySlider : public QSlider
{
    Q_OBJECT
public:
    MySlider(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *ev);
};

#endif // MYSLIDER_H
