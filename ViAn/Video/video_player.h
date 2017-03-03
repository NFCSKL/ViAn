#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"

#include <QThread>
#include <QMutex>
#include <QImage>
#include <QWaitCondition>

#include <chrono>

using namespace std;
using namespace cv;


class video_player : public QThread { Q_OBJECT
public:
    video_player(QObject* parent = 0);
    bool load_video(string filename);
    bool is_paused();
    int get_num_frames();    
    void play_pause();
    void set_frame_width(int new_value);
    void set_frame_height(int new_value);
    void set_playback_frame(int frame_num);
    void set_speed_multiplier(double mult);

    double get_speed_multiplier();

    void inc_playback_speed();
    void dec_playback_speed();

    friend class test_video_player;

    const double MAX_SPEED_MULT = 16;
    const double MIN_SPEED_MULT = 1.0/16;
    const double DEFAULT_SPEED_MULT = 1;
    const double SPEED_STEP_MULT = 2;

signals:
    void processedImage(const QImage &image);
    void currentFrame(const int frame);

protected:
    void run() override;
    void msleep(int ms);

private:
    VideoCapture capture;
    Mat frame;
    Mat RGBframe;

    int num_frames;
    unsigned int current_frame = 0;
    unsigned int frame_width;
    unsigned int frame_height;

    double frame_rate;
    double speed_multiplier = DEFAULT_SPEED_MULT;

    bool stop = false;
    bool video_paused;

    QImage img;
    QWaitCondition condition;   
};

#endif // VIDEO_PLAYER_H
