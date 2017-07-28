#include "AnalysisMethod.h"
#include <qdebug.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"

/**
 * @brief AnalysisMethod::abort_analysis
 * Sets the necessary bools to abort an analysis.
 */
AnalysisMethod::AnalysisMethod(AnalysisSettings *settings)
{
    m_settings = settings;
    sample_freq = settings->SAMPLE_RATE;
}
/**
 * @brief AnalysisMethod::set_include_exclude_area
 * Sets an exlusion frame that will be used to exclude detections in a specific area of each frame.
 * @param points for the polygon that defines the exclusion area.
 */
void AnalysisMethod::set_include_exclude_area(std::vector<cv::Point> points, bool exclude_polygon) {
    if (!capture.isOpened())
        return;

    do_exclusion = true;
    cv::Scalar poly_scalar = cv::Scalar(255);
    cv::Scalar background_scalar = cv::Scalar(0);

    if (exclude_polygon) {
        poly_scalar = cv::Scalar(0);
        background_scalar = cv::Scalar(255);
    }

    cv::Mat img(int(capture.get(cv::CAP_PROP_FRAME_HEIGHT)),int(capture.get(cv::CAP_PROP_FRAME_WIDTH)),CV_8UC1,background_scalar);
    cv::Point* rook_points[1];
    rook_points[0] = &points[0];
    const cv::Point* ppt[1] = {rook_points[0]};
    int npt[1] = {points.size()};
    cv::fillPoly(img, ppt, npt, 1, poly_scalar);
    exclude_frame = img;
}

/**
 * @brief AnalysisMethod::sample_current_frame
 * Checks if the current frame is to be analysed.
 * @return true if the current frame should be analysed.
 */
bool AnalysisMethod::sample_current_frame() {
    return current_frame_index % sample_freq == 0;
}

/**
 * @brief AnalysisMethod::run_analysis
 * This is the main loop method for doing an analysis.
 * @return all detections from the performed analysis.
 */
Analysis AnalysisMethod::run_analysis() {
    if (!capture.isOpened()) {
        return m_analysis;
    }
    calculate_scaling_factor();
    std::vector<DetectionBox> detections;
    num_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);    
    POI* m_POI = new POI();    
    // If Interval is use, start analysis at frame
    int end_frame = num_frames -1;
    int start_frame = m_settings->interval.get_start();
    if( m_settings->use_interval){
        capture.set(CV_CAP_PROP_POS_FRAMES, start_frame);
        end_frame = m_settings->interval.get_end();
        num_frames = end_frame - start_frame;
        current_frame_index = start_frame -1;
    }
    while(!aborted && capture.read(original_frame) &&
          !(m_settings->use_interval && current_frame_index <= end_frame)) {

        // Slice frame if bounding box should be used
        if(m_settings->use_bounding_box) analysis_frame = original_frame(m_settings->getBounding_box());
        else analysis_frame = original_frame;

        // do frame analysis
        if (sample_current_frame() || current_frame_index == end_frame) {
            if (scaling_needed)
                scale_frame();            
            detections = analyse_frame();

            // This if statement handles the sorting of OOIs detected
            // in a frame into the correct POIs.
            if (detections.empty() && detecting) {
                m_POI->set_end_frame(current_frame_index - 1);
                m_analysis.add_interval(m_POI);
                m_POI = new POI();
                detecting = false;
            } else if (!detections.empty()) {
                detecting = true;
                if (scaling_needed) {
                    for (DetectionBox detection : detections) {
                        detection.scale_coordinates(1.0/scaling_ratio);
                    }
                }
                m_POI->add_detections(current_frame_index, detections);
            }
        } else if (!detections.empty()) {
            /* If the current frame is not sampled, the detections from the previously
             * sampled frame should still be valid and should therefore be shown as
             * detections for the current frame as well.
             */
            m_POI->add_detections(current_frame_index, detections);
        }

        if (paused) {
            // TODO do pause stuff
            paused = false;
        }

        emit send_progress(get_progress(start_frame));
        ++current_frame_index;
    }
    // Makes sure that a POI that stretches to the end of the
    // video gets an end frame.
    if (detecting) {
        m_POI->set_end_frame(current_frame_index);
        m_analysis.add_interval(m_POI);
    }
    capture.release();
    return m_analysis;
}

/**
 * @brief AnalysisMethod::get_progress
 * @return Progression of analysis in whole percent.
 */
int AnalysisMethod::get_progress(int start_frame) {
    qDebug() << (current_frame_index-start_frame)/num_frames;
    qDebug() << num_frames;
    qDebug() << ((double)(current_frame_index-start_frame)/(double)num_frames) * 100;
    return ((double)(current_frame_index-start_frame)/(double)num_frames) * 100;

}


void AnalysisMethod::abort_analysis() {
    aborted = true;
    paused = false;
}

/**
 * @brief AnalysisMethod::pause_analysis
 * Sets the necessary bool to pause an analysis.
 */
void AnalysisMethod::pause_analysis() {
    paused = true;
}

/**
 * @brief AnalysisMethod::calculate_scaling_factor
 * This method is used when videos with large resolutions are analysed.
 * To handle the analysis without using too much RAM the frames are
 * resized to fit into a resolution of 1920x1080 before they are analysed.
 * When they are resized, a scaling factor is needed to map detections
 * on a frame to the original resolution of the video. This method does that.
 */
void AnalysisMethod::calculate_scaling_factor() {
    int video_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int video_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    float height_ratio = float(FULL_HD_HEIGHT)/float(video_height);
    float width_ratio = float(FULL_HD_WIDTH)/float(video_width);
    if (height_ratio >= 1 && width_ratio >= 1) return;

    scaling_needed = true;
    //This statement ensures that the original aspect ratio of the video is kept when scaling
    if (width_ratio >= height_ratio) {
        scaling_ratio = height_ratio;
        scaled_width = int(video_width * scaling_ratio);
        scaled_height = FULL_HD_HEIGHT;
    } else {
        scaling_ratio = width_ratio;
        scaled_width = FULL_HD_WIDTH;
        scaled_height = int(video_height * scaling_ratio);
    }
}

/**
 * @brief AnalysisMethod::scale_frame
 * This method scales the frames of a video according to the scaling factor.
 */
void AnalysisMethod::scale_frame() {
    cv::Size size(scaled_width,scaled_height);
    cv::Mat dst(size,analysis_frame.type());
    cv::resize(analysis_frame,dst,size); //resize frame
    analysis_frame = dst;
}
