#ifndef VIDEOITEM_H
#define VIDEOITEM_H
#include "treeitem.h"
#include "Project/videoproject.h"
#include "itemstates.h"
class VideoItem : public TreeItem {
    VideoProject* m_vid_proj;
    VideoState* state;
public:
    VideoItem(VideoProject* video_project);
    VideoItem();
    VideoProject* get_video_project();
    void set_video_project(VideoProject* vid_proj);
    ~VideoItem();

    void remove();
    void rename();
};
#endif // VIDEOITEM_H
