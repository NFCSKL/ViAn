#ifndef ANALYSISINTERVAL_H
#define ANALYSISINTERVAL_H
#include <QJsonArray>
#include <QJsonObject>
#include "Filehandler/saveable.h"
class AnalysisInterval : public Saveable
{
protected:
    int m_start = -1;
    int m_end = -1;
public:
    AnalysisInterval(int start, int end);
    AnalysisInterval();
    bool in_interval(int frame) const;
    int getStart() const;
    int getEnd() const;
    std::pair<int,int> getInterval();
    bool at_edge(int frame_num);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};

#endif // ANALYSISINTERVAL_H
