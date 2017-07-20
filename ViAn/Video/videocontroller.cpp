#include "videocontroller.h"
#include "videoplayer.h"

#include <QDebug>

VideoController::VideoController(){}

void VideoController::run() {
    qDebug() << "VIDEOTHREAD ID::" << QThread::currentThreadId();
    VideoPlayer* v_player = new VideoPlayer();
    // TO PLAYER
    connect(this, &VideoController::load_video, v_player, &VideoPlayer::on_load_video);
    connect(this, &VideoController::play_pause, v_player, &VideoPlayer::on_play_pause);
    connect(this, &VideoController::stop, v_player, &VideoPlayer::on_stop);
    connect(this, &VideoController::set_frame, v_player, &VideoPlayer::on_set_frame);
    connect(this, &VideoController::step_forward, v_player, &VideoPlayer::on_step_backward);
    connect(this, &VideoController::step_backward, v_player, &VideoPlayer::on_step_backward);
    // FROM PLAYER
    connect(v_player, &VideoPlayer::display, this, &VideoController::display);
    connect(v_player, &VideoPlayer::display_index, this, &VideoController::display_index);
    exec();
    delete v_player;
}
