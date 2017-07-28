#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H
#include "Project/Analysis/analysis.h"
#include <opencv2/core.hpp>

class AnalysisSettings {

public:
    ANALYSIS_TYPE type;
    cv::Rect bounding_box;
    AnalysisInterval interval;
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
    cv::Size BLUR_SIZE = cv::Size(31,31);
    int DILATION_DEGREE = 10;
    int SMALLEST_OBJECT_SIZE = 500;
    int DETECTION_THRESHOLD = 25;
    int GRAYSCALE_WHITE = 255;
    int BACKGROUND_HISTORY = 500;
    int MOG2_THRESHOLD = 50;
    bool DETECT_SHADOWS = true;
public:

    MotionDetSettings();
    MotionDetSettings(AnalysisSettings& settings);
    MotionDetSettings(MotionDetSettings& settings)  = default;
    MotionDetSettings(AnalysisInterval interval,
                      cv::Rect bounding_box,
                      int sample_rate = 1);
};

#endif // ANALYSISSETTINGS_H
