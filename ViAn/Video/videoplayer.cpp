#include "videoplayer.h"
#include <QThread>
#include <QDebug>
#include <QTime>

VideoPlayer::VideoPlayer(QObject *parent) : QObject(parent) {}

/**
 * @brief VideoPlayer::on_load_video
 * Loads the video and updates member variables with video information.
 * It also emits the first frame back to the controller
 * @param video_path    :   Path to the video
 */
void VideoPlayer::on_load_video(std::string video_path){
    m_is_playing = false;

    capture.open(video_path);
    if (!capture.isOpened()) return;

    m_delay = 1000 / capture.get(CV_CAP_PROP_FPS);
    last_frame = capture.get(CV_CAP_PROP_FRAME_COUNT) - 1;
    set_frame(0);
}

/**
 * @brief VideoPlayer::on_play
 * Sets the playback status according to play
 */
void VideoPlayer::on_play_pause(bool play){
    if (!play) {
        m_is_playing = false;
        return;
    }

    if (m_is_playing) return;
    m_is_playing = true;
    playback_loop();
}

/**
 * @brief VideoPlayer::on_stop
 */
void VideoPlayer::on_stop(){
    m_is_playing = false;
    set_frame(0);
}

/**
 * @brief VideoPlayer::on_set_frame
 * Updates the playback position if there are no more events on the event queue
 * @param frame_index   :   index of the frame that should be displayed
 */
void VideoPlayer::on_set_frame(int frame_index) {
    if (!QCoreApplication::hasPendingEvents()) set_frame(frame_index);
}

void VideoPlayer::on_step_forward() {
    on_set_frame(current_frame + 1);
}

void VideoPlayer::on_step_backward() {
    on_set_frame(current_frame - 1);
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
        display(frame.clone());
        display_index(++current_frame);
    }
    m_is_playing = false;
}

void VideoPlayer::set_frame(int frame_index) {
    if (frame_index < 0 || frame_index > last_frame) return;
    capture.set(CV_CAP_PROP_POS_FRAMES, frame_index);
    capture.read(frame);
    current_frame = frame_index;
    display(frame.clone());
}
