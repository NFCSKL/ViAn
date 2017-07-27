#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H
#include "Project/Analysis/analysis.h"
#include <opencv2/core.hpp>

class AnalysisSettings {
    ANALYSIS_TYPE type;
    AnalysisInterval interval;
    cv::Rect bounding_box;
public:
    const int SAMPLE_RATE = 1;
    bool default_settings;
    bool use_bounding_box = false;
    bool use_interval = false;
    AnalysisSettings(ANALYSIS_TYPE type, AnalysisInterval interval, cv::Rect bounding_box,
                     int sample_rate);
    AnalysisSettings(ANALYSIS_TYPE type);
    virtual ~AnalysisSettings();
    cv::Rect getBounding_box() const;
    void setBounding_box(const cv::Rect &value);

    ANALYSIS_TYPE getType() const;
    AnalysisInterval getInterval() const;
    void setInterval(const AnalysisInterval &value);
};
class MotionDetSettings : public AnalysisSettings{
public:
    const cv::Size BLUR_SIZE = cv::Size(31,31);
    const int DILATION_DEGREE = 16;
    const int SMALLEST_OBJECT_SIZE = 500;
    const int DETECTION_THRESHOLD = 25;
    const int GRAYSCALE_WHITE = 255;
    const int BACKGROUND_HISTORY = 500;
    const int MOG2_THRESHOLD = 50;
    const bool DETECT_SHADOWS = false;
    MotionDetSettings();
    MotionDetSettings(MotionDetSettings& settings)  = default;
    MotionDetSettings(AnalysisInterval interval,
                      cv::Rect bounding_box,
                      int sample_rate = 1,
                      int smallest_obj = 500);
    MotionDetSettings(AnalysisInterval interval, cv::Rect bounding_box, int sample_rate = 1,
                      int dilation = 16,
                      int min_obj_size = 500,
                      int det_treshhold = 25,
                      int grayscale_white = 255,
                      int backround_history = 500,
                      int mog2_treshold = 50,
                      bool detect_shadows = false);

};

#endif // ANALYSISSETTINGS_H
