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
    void add_detections(int frame_num, std::vector<OOI> detections);
    void set_end_frame(int frame_num);
    std::vector<cv::Rect> get_detections_on_frame(int frame_num);
    friend class Analysis;
public:
    POI(int start_frame, int end_frame);
    void read(const QJsonObject& json);
    void write(QJsonObject& json);
};


#endif // POI_H
