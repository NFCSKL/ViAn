#ifndef ITEMSTATES_H
#define ITEMSTATES_H

struct VideoState{
    int frame = 0;
    int contrast = 0;
    int brightness = 0;
    VideoState(int frame) : frame(frame){}
    VideoState(){}
};

struct ITEMSTATES
{
    ITEMSTATES();
};

struct TagState{

};

#endif // ITEMSTATES_H
