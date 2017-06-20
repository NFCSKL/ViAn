#include "framewidget.h"
#include <QDebug>

FrameWidget::FrameWidget(QWidget *parent) : QWidget(parent) {
}

void FrameWidget::draw_from_playback(cv::Mat frame) {
    if (do_update)
        draw_image(frame);
}

void FrameWidget::draw_image(cv::Mat image) {
    // Convert the image to the RGB888 format
    switch (image.type()) {
        case CV_8UC1:
            cvtColor(image, _tmp, CV_GRAY2RGB);
            break;
        case CV_8UC3:
            cvtColor(image, _tmp, CV_BGR2RGB);
            break;
    }

    // QImage needs the data to be stored continuously in memory
    assert(_tmp.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols*3, QImage::Format_RGB888);
    this->setFixedSize(image.cols, image.rows);
    repaint();
}

void FrameWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), _qimage);
    painter.end();
}

void FrameWidget::accept_update() {
    qDebug("accepting");
    do_update = true;
}

void FrameWidget::block_update() {
    qDebug("blocking");
    do_update = false;
}
