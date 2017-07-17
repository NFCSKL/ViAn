#ifndef ANALYSISINTERVAL_H
#define ANALYSISINTERVAL_H
#include "Filehandler/saveable.h"
#include <utility>
class AnalysisInterval: public Saveable
{
protected:
    int m_start_frame = -1;
    int m_end_frame = -1;
public:
    AnalysisInterval(int start_frame, int end_frame);
    AnalysisInterval();
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
    bool in_interval(int frame);
    int getStart_frame() const;
    int getEnd_frame() const;
    std::pair<int,int> getInterval();
};

#endif // ANALYSISINTERVAL_H
