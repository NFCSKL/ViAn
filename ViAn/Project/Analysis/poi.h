#ifndef POI_H
#define POI_H
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QObject>
#include <vector>
#include <map>
#include "Filehandler/saveable.h"
#include "opencv2/core/core.hpp"
#include "ooi.h"
#include "analysisinterval.h"
class POI : public AnalysisInterval{
    std::map<int,std::vector<OOI>> OOIs = {};
public:
    POI();    
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    void add_detections(int frame_num, std::vector<OOI> detections);
    void set_end_frame(int frame_num);
    bool is_in_POI(int frame_num);
    bool at_edge(int frame_num);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    friend class Analysis;
};


#endif // POI_H
