#ifndef ANALYSISSETTINGS_H
#define ANALYSISSETTINGS_H
#include "Project/Analysis/analysis.h"
#include <opencv2/core.hpp>

class AnalysisSettings {
public:
    ANALYSIS_TYPE type;
    AnalysisInterval interval;
    cv::Rect bounding_box;
    const int SAMPLE_RATE = 1;

    AnalysisSettings(ANALYSIS_TYPE type,
                     AnalysisInterval interval,
                     cv::Rect bounding_box,
                     int sample_rate);
};
class MotionDetSettings : AnalysisSettings{
    const cv::Size BLUR_SIZE = cv::Size(31,31);
    const int DILATION_DEGREE = 16;
    const int SMALLEST_OBJECT_SIZE = 500;
    const int DETECTION_THRESHOLD = 25;
    const int GRAYSCALE_WHITE = 255;
    const int BACKGROUND_HISTORY = 500;
    const int MOG2_THRESHOLD = 50;
    const bool DETECT_SHADOWS = false;
    MotionDetSettings(AnalysisInterval interval, cv::Rect bounding_box, int sample_rate = 1,
                      int dilation = 16,
                      int min_obj_size = 500,
                      int det_treshhold = 25,
                      int grayscale_white = 255,
                      int backround_history = 500,
                      int mog2_treshold = 50,
                      bool detect_shadows = false);
    MotionDetSettings(AnalysisInterval interval,
                      cv::Rect bounding_box,
                      int sample_rate = 1,
                      int smallest_obj = 500);
};

#endif // ANALYSISSETTINGS_H
