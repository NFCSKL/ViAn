#ifndef TAG_H
#define TAG_H
#include "analysis.h"
class Tag : public Analysis
{
public:
    Tag();
    bool add_frame(int frame);
    void remove_frame(int frame);
    virtual void add_interval(POI *an_interval);
public:
    const int type = TAG;
private:
    void merge_intervals();
};

#endif // TAG_H
