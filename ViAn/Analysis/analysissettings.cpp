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

MotionDetSettings::MotionDetSettings(AnalysisInterval interval,
                                     cv::Rect bounding_box,
                                     int sample_rate,
                                     int smallest_obj)
    : AnalysisSettings(MOTION_DETECTION,interval,bounding_box,sample_rate),
      SMALLEST_OBJECT_SIZE(smallest_obj)
{

}


AnalysisSettings::AnalysisSettings(ANALYSIS_TYPE type,
                                   AnalysisInterval interval,
                                   cv::Rect bounding_box,
                                   int sample_rate)
    :type(type),
      interval(interval),
      bounding_box(bounding_box),
      SAMPLE_RATE(sample_rate)
{

}
