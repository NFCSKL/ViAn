#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QThread>

class VideoController : public QThread {
    Q_OBJECT
public:
    VideoController();

signals:
    void start_player();

    void load_video(std::string video_path);
    void play();
    void pause();
    void stop();

    void set_frame(int frame_index);
protected:
    void run();
};

#endif // VIDEOCONTROLLER_H
