#include "videolistitem.h"

VideoListItem::VideoListItem(VideoProject *vid_proj)
{
    m_vid_proj =vid_proj;
    setText(QString::fromStdString(m_vid_proj->get_video()->get_name()));
}
