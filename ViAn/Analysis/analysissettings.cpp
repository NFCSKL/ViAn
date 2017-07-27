#include "analysissettings.h"
MotionDetSettings::MotionDetSettings(AnalysisInterval interval,
                                     cv::Rect bounding_box,
                                     int sample_rate,
                                     int dilation,
                                     int min_obj_size,
                                     int det_treshhold,
                                     int grayscale_white,
                                     int backround_history,
                                     int mog2_treshold,
                                     bool detect_shadows)
    : AnalysisSettings(MOTION_DETECTION,interval,bounding_box,sample_rate),
      DILATION_DEGREE(dilation),
      SMALLEST_OBJECT_SIZE(min_obj_size),
      DETECTION_THRESHOLD(det_treshhold),
      GRAYSCALE_WHITE(grayscale_white),
      BACKGROUND_HISTORY(backround_history),
      MOG2_THRESHOLD(mog2_treshold),
      DETECT_SHADOWS(detect_shadows)
{


}

MotionDetSettings::MotionDetSettings() : AnalysisSettings(MOTION_DETECTION)
{

}

MotionDetSettings::MotionDetSettings(AnalysisInterval interval,
                                     cv::Rect bounding_box,
                                     int sample_rate,
                                     int smallest_obj)
    : AnalysisSettings(MOTION_DETECTION,interval,bounding_box,sample_rate),
      SMALLEST_OBJECT_SIZE(smallest_obj)
{

}



cv::Rect AnalysisSettings::getBounding_box() const
{
    return bounding_box;
}

void AnalysisSettings::setBounding_box(const cv::Rect &value)
{
    bounding_box = value;
    use_bounding_box = true;
}

ANALYSIS_TYPE AnalysisSettings::getType() const
{
    return type;
}

AnalysisInterval AnalysisSettings::getInterval() const
{
    return interval;
}

void AnalysisSettings::setInterval(const AnalysisInterval &value)
{
    interval = value;
    use_interval = true;
}
AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type,
                                   AnalysisInterval interval,
                                   cv::Rect bounding_box,
                                   int sample_rate)
    : type(type),
      SAMPLE_RATE(sample_rate)
{
    setInterval(interval);
    setBounding_box(bounding_box);
}

AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type) : type(type)
{
    default_settings = true;
}

AnalysisSettings::~AnalysisSettings()
{

}
