#include "analysiswidget.h"
#include "Analysis/AnalysisController.h"
#include "GUI/TreeItems/analysisitem.h"
#include "GUI/videowidget.h"
#include <QDebug>
#include <QTreeWidgetItem>
#include <tuple>
AnalysisWidget::AnalysisWidget(QWidget *parent) {
    an_col = new AnalysisController(this);    
    connect(an_col, SIGNAL(progress_signal(int)), this, SLOT(send_progress(int)));
    connect(an_col, SIGNAL(analysis_done(AnalysisProxy)), this, SLOT(analysis_done(AnalysisProxy)));
}


/**
 * @brief AnalysisWidget::start_analysis
 * @param save_path     : path to the save map
 * @param video_path    : full path of the video
 * @param item          : video to be analysed
 * Puts the analysis in the queue and if the was empty starts the analysis directly
 */
void AnalysisWidget::start_analysis(QTreeWidgetItem* item, AnalysisMethod *method) {
    tuple<AnalysisMethod*,QTreeWidgetItem*> analys (method,item);
    if (!analysis_queue.empty()) {
        analysis_queue.push_back(analys);
        std::string name = "Queued #"+to_string(analysis_queue.size()-1);
        emit name_in_tree(item, QString::fromStdString(name));
    } else {
        analysis_queue.push_back(analys);
        perform_analysis(analys);
        current_analysis = item;
    }
}

/**
 * @brief AnalysisWidget::perform_analysis
 * @param analys
 * Actually starts the analysis
 * Takes in a tuple consisting of <savepath, videopath, video to be analysed>
 */
void AnalysisWidget::perform_analysis(tuple<AnalysisMethod*, QTreeWidgetItem*> analys) {
    emit add_analysis_bar();
    //an_col->new_analysis(get<1>(analys),get<0>(analys));
//    start = std::clock();
//    an_col->start();
    AnalysisMethod* method = get<0>(analys);
    qDebug()<<"movetothread";
    //QThread* analysis_thread = new QThread();

    //method->moveToThread(analysis_thread);
    //connect(analysis_thread, &QThread::started, method, &AnalysisMethod::run_analysis);
    //connect(method, &AnalysisMethod::finito, analysis_thread, &QThread::quit);
    //connect(analysis_thread, &QThread::finished, analysis_thread, &QThread::deleteLater);

    connect(method, &AnalysisMethod::send_progress, this,&AnalysisWidget::send_progress);
   // connect(method, &AnalysisMethod::finito, method, &AnalysisMethod::deleteLater);
    connect(method, SIGNAL(send_progress(int)),this, SLOT(send_progress(int)));
    connect(method, &AnalysisMethod::finished_analysis, this, &AnalysisWidget::analysis_done);
    QThreadPool::globalInstance()->start(method);
    //analysis_thread->start();

}

/**
 * @brief AnalysisWidget::analysis_done
 * @param analysis
 * Slot function to be called when an analysis is completed
 * Removes the current analysis from the queue and start the next one if there is one
 */
void AnalysisWidget::analysis_done(AnalysisProxy analysis) {
    analysis_queue.pop_front();
    emit remove_analysis_bar();
    emit name_in_tree(current_analysis, "Analysis");
    AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(current_analysis);
    AnalysisProxy* am = new AnalysisProxy(analysis);
    ana_item->set_analysis(am);
    VideoItem* vid = dynamic_cast<VideoItem*>(current_analysis->parent());
    vid->get_video_project()->add_analysis(am);
    current_analysis = nullptr;
    duration = 0;   
    if (!analysis_queue.empty()) {
        current_analysis = get<1>(analysis_queue.front());
        move_queue();
        perform_analysis(analysis_queue.front());
    }
}

/**
 * @brief AnalysisWidget::move_queue
 * Update all the analyses in the queue with the correct queue number in the project tree
 */
void AnalysisWidget::move_queue() {
    for (unsigned int i = 0; i < analysis_queue.size(); i++) {
        std::string name = "Queued #"+to_string(i);
        emit name_in_tree(get<1>(analysis_queue.at(i)), QString::fromStdString(name));
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
