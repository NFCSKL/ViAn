#include "analysismeta.h"
#include <QJsonObject>
#include <QString>


/**
 * @brief AnalysisMeta::AnalysisMeta
 */
AnalysisProxy::AnalysisProxy() {
}

/**
 * @brief AnalysisMeta::get_analysis
 * @return
 */
Analysis* AnalysisProxy::load_analysis() {
    Analysis* analysis = new Analysis();
    analysis->load_saveable(file_analysis);
    return analysis;
}
AnalysisProxy::AnalysisProxy(const Analysis &other) : BasicAnalysis(other)
{

}

AnalysisProxy::AnalysisProxy(const AnalysisProxy &other) : BasicAnalysis(other)
{

}

/**
 * @brief AnalysisMeta::read
 * @param json
 */
void AnalysisProxy::read(const QJsonObject &json) {
    m_name = json["name"].toString().toStdString();
    file_analysis = json["full_path"].toString().toStdString();
    QJsonArray json_intervals = json["intervals"].toArray();
    for (int i = 0; i < json_intervals.size() ; ++i) {
        QJsonObject json_pair = json_intervals[i].toObject();
        std::pair<int,int> pair;
        pair.first = json_pair["start"].toInt();
        pair.second = json_pair["end"].toInt();
        m_intervals.insert(new POI(pair.first, pair.second));
    }
}

/**
 * @brief AnalysisMeta::write
 * @param json
 */
void AnalysisProxy::write(QJsonObject &json) {
    json["name"] = QString::fromStdString(m_name);
    json["full_path"] = QString::fromStdString(file_analysis);
    QJsonArray intervals;
    for (auto it = m_intervals.begin(); it != m_intervals.end(); ++it) {
        QJsonObject interval;
        AnalysisInterval* pair = *it;
        interval["start"] = pair->getStart();
        interval["end"] = pair->getEnd();
        intervals.push_back(interval);
    }
    json["intervals"] = intervals;
}
