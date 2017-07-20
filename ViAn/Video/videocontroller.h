#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QThread>

#include <opencv2/core/core.hpp>

class VideoController : public QThread {
    Q_OBJECT
public:
    VideoController();

signals:
    // TO VIDEO PLAYER
    void load_video(std::string video_path);
    void play_pause(bool play);
    void stop();

    void set_frame(int frame_index);
    void step_forward(void);
    void step_backward(void);

    // FROM VIDEO PLAYER
    void display(cv::Mat);
    void display_index(int frame_index);
protected:
    void run();
};

#endif // VIDEOCONTROLLER_H
