#include "videoplayer.h"
#include <QThread>
#include <QDebug>

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {
    cv::namedWindow("test");
}

void VideoPlayer::on_load_video(std::string video_path){
    m_is_playing = false;
    qDebug() << QThread::currentThreadId() << "::on_load_video" << QString::fromStdString(video_path);
    capture.open(video_path);
    if (!capture.isOpened()) return;
    last_frame = capture.get(CV_CAP_PROP_FRAME_COUNT) - 1;
    capture.read(frame);
    current_frame = 0;
    cv::imshow("test", frame);

}

void VideoPlayer::on_play(){
    qDebug() << QThread::currentThreadId() << "::on_play";
    if (m_is_playing) return;
    m_is_playing = true;
    playback_loop();
}

void VideoPlayer::on_pause(){
    qDebug() << QThread::currentThreadId() << "::on_pause";
    m_is_playing = false;
}

void VideoPlayer::on_stop(){
    qDebug() << QThread::currentThreadId() << "::on_stop";
    m_is_playing = false;
    capture.set(CV_CAP_PROP_POS_FRAMES, 0);
    capture.read(frame);
    current_frame = 0;
    cv::imshow("test", frame);
}

void VideoPlayer::on_set_frame(int frame_index) {
    qDebug() << QThread::currentThreadId() << "::" << QString::number(frame_index - 1);
    if (frame_index >= 0 && frame_index <= last_frame && !QCoreApplication::hasPendingEvents()) {
        capture.set(CV_CAP_PROP_POS_FRAMES, frame_index);
        capture.read(frame);
        current_frame = frame_index;
        cv::imshow("test", frame);
    }
}

void VideoPlayer::playback_loop() {
    while (capture.read(frame) && m_is_playing) {
        current_frame++;
        qDebug() << QThread::currentThreadId() << "::" << QString::number(capture.get(CV_CAP_PROP_POS_FRAMES ) - 1);
        cv::imshow("test", frame);
        QCoreApplication::processEvents();
    }
    m_is_playing = false;
}
