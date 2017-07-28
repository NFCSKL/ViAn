#ifndef ANALYSISWIDGET_H
#define ANALYSISWIDGET_H

#include "GUI/videowidget.h"
#include <QWidget>
#include <QTreeWidgetItem>
#include <ctime>
#include "Analysis/AnalysisController.h"
#include "GUI/TreeItems/videoitem.h"

class AnalysisWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalysisWidget(QWidget *parent = nullptr);
    AnalysisController* an_col;
    std::string dots = "";
    std::clock_t start;
    int duration = 0;
    std::deque<std::tuple<std::string, std::string, QTreeWidgetItem*, AnalysisSettings*>> analysis_queue;
    QTreeWidgetItem* current_analysis;
    
private:

    void perform_analysis(tuple<string, string, QTreeWidgetItem *, AnalysisSettings *> analys);
    void move_queue();
    
public slots:
    void start_analysis(std::string save_path, std::string video_path,
                        QTreeWidgetItem* item, AnalysisSettings* settings);
    void send_progress(int);
    void analysis_done(AnalysisProxy);
    
signals:
    void add_analysis_bar(void);
    void remove_analysis_bar(void);
    void show_progress(int);
    void name_in_tree(QTreeWidgetItem*, QString);
    void add_slider_interval(int, int);
};

#endif // ANALYSISWIDGET_H
