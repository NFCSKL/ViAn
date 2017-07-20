#include "videocontroller.h"
#include "videoplayer.h"

#include <QDebug>

VideoController::VideoController(){}

void VideoController::run() {
    qDebug() << "VIDEOTHREAD ID::" << QThread::currentThreadId();
    VideoPlayer* v_player = new VideoPlayer();
    connect(this, &VideoController::load_video, v_player, &VideoPlayer::on_load_video);
    connect(this, &VideoController::play, v_player, &VideoPlayer::on_play);
    connect(this, &VideoController::pause, v_player, &VideoPlayer::on_pause);
    connect(this, &VideoController::stop, v_player, &VideoPlayer::on_stop);
    connect(this, &VideoController::set_frame, v_player, &VideoPlayer::on_set_frame);
    exec();
    delete v_player;
}
