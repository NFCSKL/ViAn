#include "videoplayer.h"
#include <QThread>
#include <QDebug>
#include <QTime>

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {
    cv::namedWindow("test");
}

/**
 * @brief VideoPlayer::on_load_video
 * Loads the video and updates member variables with video information.
 * It also emits the first frame back to the controller
 * @param video_path    :   Path to the video
 */
void VideoPlayer::on_load_video(std::string video_path){
    m_is_playing = false;
    qDebug() << QThread::currentThreadId() << "::on_load_video" << QString::fromStdString(video_path);
    capture.open(video_path);
    if (!capture.isOpened()) return;
    m_delay = 1000 / capture.get(CV_CAP_PROP_FPS);
    last_frame = capture.get(CV_CAP_PROP_FRAME_COUNT) - 1;
    capture.read(frame);
    current_frame = 0;
    cv::imshow("test", frame);

}

/**
 * @brief VideoPlayer::on_play
 */
void VideoPlayer::on_play(){
    qDebug() << QThread::currentThreadId() << "::on_play";
    if (m_is_playing) return;
    m_is_playing = true;
    playback_loop();
}

/**
 * @brief VideoPlayer::on_pause
 */
void VideoPlayer::on_pause(){
    qDebug() << QThread::currentThreadId() << "::on_pause";
    m_is_playing = false;
}

/**
 * @brief VideoPlayer::on_stop
 */
void VideoPlayer::on_stop(){
    qDebug() << QThread::currentThreadId() << "::on_stop";
    m_is_playing = false;
    capture.set(CV_CAP_PROP_POS_FRAMES, 0);
    capture.read(frame);
    current_frame = 0;
    cv::imshow("test", frame);
    display(frame.clone());
}

/**
 * @brief VideoPlayer::on_set_frame
 * Updates the playback position if there are no more events on the event queue
 * @param frame_index   :   index of the frame that should be displayed
 */
void VideoPlayer::on_set_frame(int frame_index) {
    if (frame_index >= 0 && frame_index <= last_frame && !QCoreApplication::hasPendingEvents()) {
        qDebug() << QThread::currentThreadId() << "::" << QString::number(frame_index - 1);
        capture.set(CV_CAP_PROP_POS_FRAMES, frame_index);
        capture.read(frame);
        current_frame = frame_index;
        cv::imshow("test", frame);
        display(frame.clone());
        display_index(current_frame);
    }
}

/**
 * @brief VideoPlayer::playback_loop
 * Main loop for video playback.
 * This function is executed whilst the video is playing/
 * It emits both the current frame and its index back to the controller object.
 */
void VideoPlayer::playback_loop() {
    QTime frame_rate_timer;
    frame_rate_timer.start();

    while (m_is_playing) {
        // Handle events from controller
        QCoreApplication::processEvents();

        // Make sure playback sticks to the correct frame rate
        if (frame_rate_timer.elapsed() < m_delay) continue;
        frame_rate_timer.restart();

        if (!capture.read(frame)) {
            m_is_playing = false;
            continue;
        }

        cv::imshow("test", frame);
        display(frame.clone());
        display_index(++current_frame);

    }
    m_is_playing = false;
}
