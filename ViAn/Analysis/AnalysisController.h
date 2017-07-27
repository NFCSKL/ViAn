#ifndef ANALYSISCONTROLLER_H
#define ANALYSISCONTROLLER_H

#include <QThread>
#include "Project/Analysis/analysisproxy.h"
#include "Analysis/AnalysisMethod.h"
#include "analysissettings.h"
class AnalysisController : public QThread {
    Q_OBJECT

private:
    std::string m_save_path;
    std::string m_video_path;
public:
    AnalysisController(QObject* parent = 0);
    void run() override;
    void new_analysis(std::string save_path, std::string video_path, AnalysisSettings *settings);
private:
    void setup_analysis(std::string video_path, AnalysisSettings *settings);
    AnalysisMethod* method;
signals:
    void analysis_done(AnalysisProxy);
    void progress_signal(int);
private slots:
    void on_start();                // Start or resume the analysis
    void on_pause();                // Pause the analysis
    void on_abort();                // Abort the analysis
    void on_progress_update(int progress);
};

#endif // ANALYSISCONTROLLER_H
