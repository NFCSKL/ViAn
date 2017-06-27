#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include "videowidget.h"
#include <QWidget>
#include <QTreeWidgetItem>
#include <ctime>
#include "Analysis/AnalysisController.h"


class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisWidget(QWidget *parent = nullptr);
    VideoProject* vid_proj;
    std::string dots = "";
    std::clock_t start;
    int duration = 0;
    std::vector<std::pair<AnalysisController*, QTreeWidgetItem*>> analysis_queue;
    QTreeWidgetItem* current_analysis;
    
private:

    void perform_analysis(AnalysisController *an_col);
    void move_queue();
    
public slots:
    void start_analysis(std::string save_path, std::string video_path, QTreeWidgetItem* item);
    void send_progress(int);
    void analysis_done(AnalysisMeta);
    
signals:
    void add_analysis_bar(void);
    void remove_analysis_bar(void);
    void show_progress(int);
    void name_in_tree(QTreeWidgetItem*, QString);
};

#endif // ANALYSISWIDGET_H
