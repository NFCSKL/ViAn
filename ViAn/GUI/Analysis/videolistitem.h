#ifndef VIDEOLISTITEM_H
#define VIDEOLISTITEM_H
#include <QListWidgetItem>
#include "Project/videoproject.h"
class VideoListItem : public QListWidgetItem
{    

public:
    VideoProject* m_vid_proj;
    VideoListItem(VideoProject* vid_proj);
};

#endif // VIDEOLISTITEM_H
