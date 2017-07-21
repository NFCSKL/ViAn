#ifndef TAG_H
#define TAG_H
#include "analysis.h"
class Tag : public BasicAnalysis
{
public:
    Tag();
    void add_frame(int frame);
    void remove_frame(int frame);
    virtual void add_interval(AnalysisInterval *an_interval);
    void remove_interval(AnalysisInterval *rm_interval);
private:
    void merge_intervals();
    bool is_interval(int start, int end);
    std::pair<int, int> get_overlap(AnalysisInterval *intval1, AnalysisInterval *intval2);
    std::pair<AnalysisInterval *, AnalysisInterval *> remove_overlap(AnalysisInterval *interval, std::pair<int, int> overlap);
};

#endif // TAG_H
