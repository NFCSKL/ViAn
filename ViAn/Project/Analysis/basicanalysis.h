#ifndef BASICANALYSIS_H
#define BASICANALYSIS_H
#include <string>
#include <utility>
#include <set>
#include "analysisinterval.h"
#include <QJsonArray>
enum ANALYSIS_TYPE {MOTION_DETECTION = 0, FACIAL_DETECTION = 1, TAG = 2};

const std::vector<std::string> ANALYSIS_NAMES = {"Motion detection", "Facial detection", "Tag"};
const std::map<std::string, ANALYSIS_TYPE> ANALYSIS_NAMES_TYPE_MAP = {std::make_pair("Motion detection",MOTION_DETECTION),
                                                                     std::make_pair("Facial detection",FACIAL_DETECTION),
                                                                     std::make_pair("Tag",TAG)};
struct interval_cmp {
    bool operator()(const AnalysisInterval* lhs, const AnalysisInterval* rhs) const {
        return lhs->getEnd() < rhs->getStart();
    }
};
using interval_set = std::set<AnalysisInterval*, interval_cmp> ;
class BasicAnalysis : public Saveable
{   

protected:
public:
    int type;
    std::string m_name;
    std::string file_analysis;
    interval_set m_intervals;
protected:
public:
    BasicAnalysis();
    BasicAnalysis(const BasicAnalysis& other);

    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);

    std::string getFile_analysis() const;
    std::string getName() const;
    int getType() const;
    virtual void add_interval(AnalysisInterval *ai);
    interval_set getIntervals() const;
};
Q_DECLARE_METATYPE(BasicAnalysis)
#endif // BASICANALYSIS_H
