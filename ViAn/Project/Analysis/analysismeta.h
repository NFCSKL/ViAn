#ifndef ANALYSISPROXY_H
#define ANALYSISPROXY_H
#include "basicanalysis.h"
#include "analysis.h"
#include "tag.h"
#include "Filehandler/saveable.h"
#include <iostream>

/**
 * @brief The AnalysisMeta class
 * This class is used to improve performance of basic
 * operations related to analyses. Full analysis is therefor
 * only fully loaded upon direct request.
 */
class AnalysisProxy : public BasicAnalysis
{
public:
    AnalysisProxy();
    Analysis *load_analysis(); // Only use this if all analysisinformation is needed
    AnalysisProxy(const Analysis &other);
    AnalysisProxy(const AnalysisProxy &other);
    virtual void read(const QJsonObject& json);
    virtual void write(QJsonObject& json);
};
#endif // ANALYSISPROXY_H
