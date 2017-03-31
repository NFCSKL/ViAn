#include "test_video_player.h"
#include <thread>
#include <QMutex>
#include <QWaitCondition>

/**
 * @brief test_video_player::test_video_player
 * constructor
 * @param parent
 */
test_video_player::test_video_player(QObject *parent) : QObject(parent) {
    QMutex mutex;
    QWaitCondition wait;
    mvideo = new video_player(&mutex, &wait);
}

/**
 * @brief test_video_player::test_load_video
 */
void test_video_player::test_load_video() {
    string working_filename = "seq_01.mp4";
    string non_existing_filename = "ex.biz";
    string existing_non_video = "Makefile";

    QVERIFY(mvideo->capture.open(working_filename));
    mvideo->frame_rate = mvideo->capture.get(CV_CAP_PROP_FPS);
    mvideo->num_frames = mvideo->capture.get(cv::CAP_PROP_FRAME_COUNT);
    mvideo->video_paused = false;

    QVERIFY(mvideo->capture.isOpened());

}

/**
 * @brief test_video_player::test_is_paused
 */
void test_video_player::test_is_paused() {
    mvideo->video_paused = false;
    QVERIFY(!mvideo->is_paused());
    mvideo->video_paused = true;
    QVERIFY(mvideo->is_paused());
}

/**
 * @brief test_video_player::test_get_num_frames
 */
void test_video_player::test_get_num_frames() {
    QVERIFY(mvideo->capture.get(cv::CAP_PROP_FRAME_COUNT) == mvideo->get_num_frames());
}

/**
 * @brief test_video_player::test_set_frame_width
 */
void test_video_player::test_set_frame_width() {
    mvideo->set_frame_width(50);
    QVERIFY(mvideo->frame_width == 50);
}

/**
 * @brief test_video_player::test_set_frame_height
 */
void test_video_player::test_set_frame_height() {
    mvideo->set_frame_height(50);
    QVERIFY(mvideo->frame_height == 50);
}

/**
 * @brief test_video_player::test_set_current_frame
 */
void test_video_player::test_set_current_frame() {
    mvideo->set_current_frame_num(100);
    QVERIFY(mvideo->get_current_frame_num() == 100);
    mvideo->set_current_frame_num(10);
    mvideo->set_current_frame_num(-10);
    QVERIFY(mvideo->get_current_frame_num() == 10);
}

/**
 * @brief test_video_player::test_next_frame
 */
void test_video_player::test_next_frame() {
    mvideo->set_current_frame_num(100);
    mvideo->next_frame();
    QVERIFY(mvideo->get_current_frame_num() == 101);
}

/**
 * @brief test_video_player::test_previous_frame
 */
void test_video_player::test_previous_frame() {
    mvideo->set_current_frame_num(100);
    mvideo->previous_frame();
    QVERIFY(mvideo->get_current_frame_num() == 99);
}

/**
 * @brief test_video_player::test_inc_playback_speed
 */
void test_video_player::test_inc_playback_speed(){
    mvideo->set_speed_multiplier(1);
    QVERIFY(mvideo->get_speed_multiplier() == 1);

    mvideo->inc_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 0.5);

    mvideo->set_speed_multiplier(1.0/16);
    mvideo->inc_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 1.0/16);
}

/**
 * @brief test_video_player::test_dec_playback_speed
 */
void test_video_player::test_dec_playback_speed(){
    mvideo->set_speed_multiplier(1);
    QVERIFY(mvideo->get_speed_multiplier() == 1);

    mvideo->dec_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 2);

    mvideo->set_speed_multiplier(16);
    mvideo->dec_playback_speed();
    QVERIFY(mvideo->get_speed_multiplier() == 16);
}

/**
 * @brief test_toggle_overlay
 */
void test_video_player::test_toggle_overlay() {
    mvideo->on_stop_video();
    mvideo->video_overlay->set_showing_overlay(false);
    mvideo->toggle_overlay();
    QVERIFY(mvideo->is_showing_overlay());
    mvideo->toggle_overlay();
    QVERIFY(!mvideo->is_showing_overlay());
}

/**
 * @brief test_video_player::test_set_overlay_tool
 */
void test_video_player::test_set_overlay_tool() {
    mvideo->set_overlay_tool(RECTANGLE);
    QVERIFY(mvideo->video_overlay->get_shape() == RECTANGLE);
}

/**
 * @brief test_video_player::test_set_overlay_colour
 */
void test_video_player::test_set_overlay_colour() {
    mvideo->set_overlay_colour(Qt::black);
    QVERIFY(mvideo->video_overlay->get_colour() == Qt::black);
}

/**
 * @brief test_video_player::test_reset_brightness_contrast
 */
void test_video_player::test_reset_brightness_contrast() {
    mvideo->reset_brightness_contrast();
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_DEFAULT);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_DEFAULT);
}

/**
 * @brief test_video_player::test_set_contrast
 */
void test_video_player::test_set_contrast() {
    mvideo->set_contrast(mvideo->CONTRAST_MIN - 10);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_MIN);
    mvideo->set_contrast(mvideo->CONTRAST_MIN - 0.01);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_MIN);
    mvideo->set_contrast(mvideo->CONTRAST_MIN);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_MIN);
    mvideo->set_contrast(mvideo->CONTRAST_MIN + 0.1);
    double near_min = mvideo->CONTRAST_MIN + 0.1;
    QVERIFY(mvideo->get_contrast() == near_min);
    mvideo->set_contrast(mvideo->CONTRAST_DEFAULT);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_DEFAULT);
    mvideo->set_contrast(mvideo->CONTRAST_MAX - 0.1);
    double near_max = mvideo->CONTRAST_MAX - 0.1;
    QVERIFY(mvideo->get_contrast() == near_max);
    mvideo->set_contrast(mvideo->CONTRAST_MAX);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_MAX);
    mvideo->set_contrast(mvideo->CONTRAST_MAX + 0.01);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_MAX);
    mvideo->set_contrast(mvideo->CONTRAST_MAX + 10);
    QVERIFY(mvideo->get_contrast() == mvideo->CONTRAST_MAX);

    // Values should be doubles 0.5 to 5.0
    double test05 = 0.5;
    double test051 = 0.51;
    double test1 = 1;
    double test2 = 2;
    double test499 = 4.99;
    double test5 = 5;
    mvideo->set_contrast(-10);
    QVERIFY(mvideo->get_contrast() == test05);
    mvideo->set_contrast(0);
    QVERIFY(mvideo->get_contrast() == test05);
    mvideo->set_contrast(0.49);
    QVERIFY(mvideo->get_contrast() == test05);
    mvideo->set_contrast(0.5);
    QVERIFY(mvideo->get_contrast() == test05);
    mvideo->set_contrast(0.51);
    QVERIFY(mvideo->get_contrast() == test051);
    mvideo->set_contrast(1);
    QVERIFY(mvideo->get_contrast() == test1);
    mvideo->set_contrast(2);
    QVERIFY(mvideo->get_contrast() == test2);
    mvideo->set_contrast(4.99);
    QVERIFY(mvideo->get_contrast() == test499);
    mvideo->set_contrast(5);
    QVERIFY(mvideo->get_contrast() == test5);
    mvideo->set_contrast(5.01);
    QVERIFY(mvideo->get_contrast() == test5);
    mvideo->set_contrast(500);
    QVERIFY(mvideo->get_contrast() == test5);
}

/**
 * @brief test_video_player::test_set_brightness
 */
void test_video_player::test_set_brightness() {
    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN - 10);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN - 1);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MIN + 1);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MIN + 1);
    mvideo->set_brightness(mvideo->BRIGHTNESS_DEFAULT);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_DEFAULT);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX - 1);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX - 1);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX + 1);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX);
    mvideo->set_brightness(mvideo->BRIGHTNESS_MAX + 10);
    QVERIFY(mvideo->get_brightness() == mvideo->BRIGHTNESS_MAX);

    // Values should be integers -100 to 100
    mvideo->set_brightness(-101);
    QVERIFY(mvideo->get_brightness() == -100);
    mvideo->set_brightness(-100);
    QVERIFY(mvideo->get_brightness() == -100);
    mvideo->set_brightness(-99);
    QVERIFY(mvideo->get_brightness() == -99);
    mvideo->set_brightness(0);
    QVERIFY(mvideo->get_brightness() == 0);
    mvideo->set_brightness(1);
    QVERIFY(mvideo->get_brightness() == 1);
    mvideo->set_brightness(2);
    QVERIFY(mvideo->get_brightness() == 2);
    mvideo->set_brightness(99);
    QVERIFY(mvideo->get_brightness() == 99);
    mvideo->set_brightness(100);
    QVERIFY(mvideo->get_brightness() == 100);
    mvideo->set_brightness(101);
    QVERIFY(mvideo->get_brightness() == 100);
    mvideo->set_brightness(500);
    QVERIFY(mvideo->get_brightness() == 100);
}

/**
 * @brief test_video_player::test_video_open
 */
void test_video_player::test_video_open() {
    mvideo->capture.release();
    QVERIFY(!mvideo->video_open());
    mvideo->capture.open("seq_01.mp4");
    QVERIFY(mvideo->video_open());
}

/**
 * @brief test_video_player::test_scaling_event
 * Tests that the aspect ratio is kept when the QLabel signals a scaling event.
 */
void test_video_player::test_scaling_event() {
    mvideo->capture.release();
    mvideo->capture.open("seq_01.mp4");
    mvideo->scaling_event(640,1080);

    QVERIFY(mvideo->frame_height < 1080);

    mvideo->scaling_event(1920,480);
    QVERIFY(mvideo->frame_width < 1920);
}

/**
 * @brief test_video_player::test_scale_frame
 */
void test_video_player::test_scale_frame() {
    mvideo->capture.release();
    mvideo->capture.open("seq_01.mp4");
    int video_width = mvideo->capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int video_height = mvideo->capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    mvideo->capture.read(mvideo->frame);

    mvideo->scaling_event(video_width*2,
                          video_height*2);
    cv::Mat temp = mvideo->scale_frame(mvideo->frame);
    QVERIFY(temp.cols == video_width*2 && temp.rows == video_height*2);

    mvideo->scaling_event(0,0);
    temp = mvideo->scale_frame(mvideo->frame);
    QVERIFY(temp.cols == video_width && temp.rows == video_height);
}

/**
 * @brief test_video_player::test_set_play_video
 */
void test_video_player::test_set_play_video() {
    mvideo->video_paused = true;
    mvideo-> video_stopped = true;
    mvideo->on_play_video();
    QVERIFY(!mvideo->video_paused);
    QVERIFY(!mvideo->video_stopped);
}

/**
 * @brief test_video_player::test_set_paused_video
 */
void test_video_player::test_set_pause_video() {
    mvideo->video_paused = false;
    mvideo->on_pause_video();
    QVERIFY(mvideo->video_paused);
}

/**
 * @brief test_video_player::test_set_stop_video
 */
void test_video_player::test_set_stop_video() {
    mvideo->video_paused = true;
    mvideo->video_stopped = false;
    mvideo->on_stop_video();
    QVERIFY(!mvideo->video_paused);
    QVERIFY(mvideo->get_current_frame_num() == 0);
    QVERIFY(mvideo->video_stopped);
}
