#include "analysisinterval.h"
AnalysisInterval::AnalysisInterval(int start_frame, int end_frame)
{
    m_start_frame = start_frame;
    m_end_frame = end_frame;
}

AnalysisInterval::AnalysisInterval()
{

}

void AnalysisInterval::read(const QJsonObject &json)
{
    this->m_start_frame = json["start"].toInt();
    this->m_end_frame = json["end"].toInt();
}

void AnalysisInterval::write(QJsonObject &json)
{
    json["start"] = m_start_frame;
    json["end"] = m_end_frame;
}

bool AnalysisInterval::in_interval(int frame)
{
   return frame >= m_start_frame && frame <= m_end_frame;
}

int AnalysisInterval::getStart_frame() const
{
    return m_start_frame;
}

int AnalysisInterval::getEnd_frame() const
{
    return m_end_frame;
}

std::pair<int, int> AnalysisInterval::getInterval()
{
    return std::make_pair(m_start_frame, m_end_frame);
}


