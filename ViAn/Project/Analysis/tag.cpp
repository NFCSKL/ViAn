#include "tag.h"

Tag::Tag(QString name) {
    m_name = name.toStdString();
}

Tag::~Tag() {

}

void Tag::add_frame(int frame) {
    this->frames.insert(frame);
}

/**
 * @brief Analysis::read
 * Reads analysis from json format.
 * @param json
 */
void Tag::read(const QJsonObject &json){
    /*this->type = (ANALYSIS_TYPE)json["type"].toInt();
    this->name = json["name"].toString().toStdString();
    QJsonArray json_pois = json["POI:s"].toArray();
    for (int i = 0; i < json_pois.size(); ++i) {
        QJsonObject json_poi = json_pois[i].toObject();
        POI poi;
        poi.read(json_poi);
        this->add_POI(poi);
    }*/
}

/**
 * @brief Analysis::write
 * Writes analysis to json format.
 * @param json
 */
void Tag::write(QJsonObject &json){
    /*json["type"] = this->type;
    json["name"] = QString::fromStdString(this->name);
    QJsonArray json_POIs;
    for(auto it = this->POIs.begin(); it != this->POIs.end(); it++){
        QJsonObject json_POI;
        POI p = *it;
        p.write(json_POI);
        json_POIs.append(json_POI);
    }
    json["POI:s"] = json_POIs;*/
}
