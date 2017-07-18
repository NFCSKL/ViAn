#include "poi.h"

/**
 * @brief POI::POI
 */
POI::POI(){
}


POI::POI(int start_frame, int end_frame)
{
    m_start = start_frame;
    m_end = end_frame;
}

/**
 * @brief POI::add_detections
 * Adds OOIs for a specific frame in a POI.
 * @param frame_num
 * @param detections
 */
void POI::add_detections(int frame_num, std::vector<OOI> detections) {    
    if (m_start == -1)
        m_start = frame_num;
    OOIs[frame_num] = detections;
}

/**
 * @brief AnalysisInterval::set_end_frame
 * Sets the end frame of the POI.
 * @param frame_num
 */
void POI::set_end_frame(int frame_num) {
    m_end = frame_num;
}


/**
 * @brief Analysis::get_detections_on_frame
 * Returns all detections on a specified frame in the analysed video.
 * @param frame_num
 * @return
 */
std::vector<cv::Rect> POI::get_detections_on_frame(int frame_num) {
    std::vector<cv::Rect> rects = {};
    if(in_interval((frame_num))){
        for(OOI ooi: OOIs[frame_num]){
            rects.push_back(ooi.get_rect());
        }
    }
    return rects;
}


/**
 * @brief POI::read
 * Reads POI from json format.
 * @param json
 */
void POI::read(const QJsonObject& json) {
    this->m_start = json["start"].toInt();
    this->m_end = json["end"].toInt();
    for(int i = m_start; i != m_end; i++){
        QJsonArray json_frame_OOIs = json[QString::number(i)].toArray();
        std::vector<OOI> oois;
        for(int j = 0; j != json_frame_OOIs.size(); j++){
            OOI ooi;;
            ooi.read(json_frame_OOIs[j].toObject());
            oois.push_back(ooi);
        }
        this->OOIs.insert(std::make_pair(i, oois));
    }
}

/**
 * @brief POI::write
 * Writes POI to json format.
 * @param json
 */
void POI::write(QJsonObject& json) {
    json["start"] = m_start;
    json["end"] = m_end;
    for(const auto& ooi_pair : OOIs){
        QJsonArray json_frame_OOIs;
        int frame = ooi_pair.first;
        std::vector<OOI> oois = ooi_pair.second;
        for (OOI o : oois) {
            QJsonObject json_ooi;
            o.write(json_ooi);
            json_frame_OOIs.append(json_ooi);
        }
        json[QString::number(frame)] = json_frame_OOIs;
    }
}
