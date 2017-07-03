#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include "Project/videoproject.h"
#include "Project/Analysis/tag.h"
#include <QTreeWidgetItem>

enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM, FOLDER_ITEM, TAG_ITEM};

class AnalysisItem : public QTreeWidgetItem
{
    Analysis m_analysis;

public:
    AnalysisItem(int type);
    void set_analysis(Analysis analysis);
    Analysis *get_analysis();
};

class TagItem : public QTreeWidgetItem
{
    Tag* m_tag;
public:
    TagItem(Tag *tag, int type);
    Tag* get_tag();
};

class VideoItem : public QTreeWidgetItem
{
    VideoProject* m_vid_proj;

public:
    VideoItem(VideoProject* video_project, int type);
    VideoProject* get_video_project();
signals:

public slots:
};

class FolderItem : public QTreeWidgetItem
{
public:
    FolderItem(int type);
};
#endif // ITEMTYPES_H
