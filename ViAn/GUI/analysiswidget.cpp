#include "analysiswidget.h"
#include "Analysis/AnalysisController.h"
#include "videowidget.h"
#include <QDebug>
#include <QTreeWidgetItem>

AnalysisWidget::AnalysisWidget(QWidget *parent) {

}

void AnalysisWidget::start_analysis(std::string save_path, std::string video_path, QTreeWidgetItem* item) {
    std::size_t index = video_path.find_last_of('/') + 1;
    std::string vid_name = video_path.substr(index);
    index = vid_name.find_last_of('.');
    vid_name = vid_name.substr(0,index);
    AnalysisController* an_col = new AnalysisController(save_path+vid_name+"-analysis", video_path, MOTION_DETECTION);

    std::pair<AnalysisController*, QTreeWidgetItem*> pair = make_pair(an_col, item);
    if (!analysis_queue.empty()) {
        analysis_queue.push_back(pair);
        std::string name = "Queued #"+to_string(analysis_queue.size()-1);
        emit name_in_tree(item, QString::fromStdString(name));
    } else {
        analysis_queue.push_back(pair);
        perform_analysis(an_col);
        current_analysis = item;
    }
}

void AnalysisWidget::perform_analysis(AnalysisController* an_col) {
    emit add_analysis_bar();
    connect(an_col, SIGNAL(progress_signal(int)), this, SLOT(send_progress(int)));
    //QObject::connect(an_col, SIGNAL(progress_signal(int)), this, SLOT(send_progress(int)));
    connect(an_col, SIGNAL(analysis_done(AnalysisMeta)), this, SLOT(analysis_done(AnalysisMeta)));
    start = std::clock();
    an_col->start();
}

void AnalysisWidget::analysis_done(AnalysisMeta analysis) {
    //std::pair<AnalysisController*, QTreeWidgetItem*> pair = analysis_queue.front();
    //AnalysisController* an_col = analysis_queue.front().first;
    analysis_queue.pop_front();
    //delete an_col;

    //analysis_queue.erase(analysis_queue.begin());
    //delete pair.first;
    //delete analysis_queue.front();

    emit remove_analysis_bar();
    emit name_in_tree(current_analysis, "Analysis");
    current_analysis = nullptr;
    duration = 0;
    std::vector<std::pair<int,int>> pois = analysis.m_poi_intervals;
    for (std::pair<int,int> poi : pois) {
        std::cout << "start - end " << poi.first << " - " << poi.second << std::endl;
    }
    std::cout << (std::clock()-start)/(double)CLOCKS_PER_SEC << std::endl;
    if (!analysis_queue.empty()) {
        std::pair<AnalysisController*, QTreeWidgetItem*> pair = analysis_queue.front();
        current_analysis = pair.second;
        move_queue();
        perform_analysis(pair.first);
    }
}

void AnalysisWidget::move_queue() {
    for (unsigned int i = 0; i < analysis_queue.size(); i++) {
        std::string name = "Queued #"+to_string(i);
        emit name_in_tree(analysis_queue.at(i).second, QString::fromStdString(name));
    }
}

/**
 * @brief AnalysisWidget::send_progress
 * @param progress
 * Slot for sending the analysis progress
 */
void AnalysisWidget::send_progress(int progress) {
    int time = (std::clock()-start)/CLOCKS_PER_SEC;
    if (time > duration) {
        duration = time;
        if (dots.length() == 3) {
            dots.clear();
        } else {
            dots += ".";
        }
    }
    std::string name = "Loading " + to_string(progress) + "%" + dots;
    emit name_in_tree(current_analysis, QString::fromStdString(name));
    emit show_progress(progress);
}
