#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include <set>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"
#include "poi.h"
#include "detectionbox.h"
#include "basicanalysis.h"

/**
 * @brief The Analysis class
 *
 */
class Analysis : public BasicAnalysis {
    friend class AnalysisMeta;
public:
    ANALYSIS_TYPE type;
public:
    virtual void read(const QJsonObject& json) override;
    virtual void write(QJsonObject& json) override;
    virtual SAVE_TYPE get_save_type() const override;
    virtual ANALYSIS_TYPE get_type() const override;
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    void set_name(const std::string &name);
    std::string get_name() const;

};

#endif // ANALYSIS_H
