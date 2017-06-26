#include "AnalysisController.h"
#include <vector>
#include <QApplication>
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Project/Analysis/analysis.h"
#include "Project/Analysis/analysismeta.h"
#include "Analysis/MotionDetection.h"

/**
 * @brief AnalysisController::AnalysisController
 * @param file_path path to the video file to be analysed
 * @param type      analysis type
 * @param parent
 */
AnalysisController::AnalysisController(std::string save_path, std::string video_path, ANALYSIS_TYPE type, QObject* parent) : QThread(parent) {
    m_save_path = save_path;
    m_video_path = video_path;
    setup_analysis(video_path, type);
}

/**
 * @brief AnalysisController::AnalysisController
 * @param save_path                     path to the save
 * @param file_path                     path to the video file to be analysed
 * @param type                          analysis type
 * @param inclusion_exclusion_points    Points for the inclusion/exclusion polygon
 * @param exclude_poly                  Bool to determine whether to exclude or include the polygon
 * @param parent
 */
AnalysisController::AnalysisController(std::string save_path, std::string video_path, ANALYSIS_TYPE type, std::vector<cv::Point> inclusion_exclusion_points, bool exclude_poly, QObject* parent) : QThread(parent) {
    m_save_path = save_path;
    m_video_path = video_path;

    setup_analysis(video_path, type);
    if(!inclusion_exclusion_points.empty())
    method->set_include_exclude_area(inclusion_exclusion_points, exclude_poly);
}

/**
 * @brief AnalysisController::setup_analysis
 * Sets up the analysis method and connects needed signals and slots.
 * @param file_path     path to the video file to be analysed
 * @param type          analysis type
 */
void AnalysisController::setup_analysis(std::string video_path, ANALYSIS_TYPE type) {
    switch (type) {
    case MOTION_DETECTION:
        method = new MotionDetection(video_path);
        break;
    default:
        method = new MotionDetection(video_path);
        break;
    }
    QObject::connect(method, SIGNAL(send_progress(int)),
                     this, SLOT(on_progress_update(int)));
}

/**
 * @brief Analysis::run
 * Starts the analysis loop.
 */
void AnalysisController::run() {
    method->setup_analysis();
    Analysis analysis = method->run_analysis();
    analysis.save_saveable(m_save_path);
    AnalysisMeta analysis_meta (analysis);
    emit analysis_done(analysis_meta);
    delete method;
}

/**
 * @brief AnalysisController::on_start
 * Flips the paused bool to false.
 * Starts or resumes the analysis
 */
void AnalysisController::on_start() {
    // TODO add code for resuming from paused state.
}

/**
 * @brief AnalysisController::on_pause
 * Flips the paused bool to true.
 * Pauses the analysis
 */
void AnalysisController::on_pause() {
    method->pause_analysis();
}

/**
 * @brief AnalysisController::on_abort
 * Flips the aborted bool to true.
 * Aborts the analysis
 */
void AnalysisController::on_abort() {
    method->abort_analysis();
}

/**
 * @brief AnalysisController::on_progress_update
 * Sends the progress of the current analysis in percent to the gui
 * @param progress
 */
void AnalysisController::on_progress_update(int progress) {
    emit progress_signal(progress);
}
