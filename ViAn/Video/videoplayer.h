#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QObject>
#include <QCoreApplication>


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/core/core.hpp>

class VideoPlayer : public QObject
{
    Q_OBJECT
    cv::VideoCapture capture;
    cv::Mat frame;


    std::string m_vid_path = "D:/Testdata/Sequence 01.mp4";
    int current_frame = -1;
    int last_frame = 0;
    int m_delay = 0;    // Delay time to reach the right frame rate

    bool m_is_playing = false;
    bool m_video_loaded = true;
public:
    explicit VideoPlayer(QObject *parent = nullptr);

signals:
    void display(cv::Mat);
    void display_index(int frame_index);


public slots:
    void on_load_video(std::string);
    void on_play();
    void on_pause();
    void on_stop();
    void on_set_frame(int frame_index);

private:
    void playback_loop();
};

#endif // VIDEOPLAYER_H
