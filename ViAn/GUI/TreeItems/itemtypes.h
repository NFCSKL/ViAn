#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include "Project/videoproject.h"
#include <QTreeWidgetItem>

enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM, FOLDER_ITEM};

class VideoProject;
class TreeItem : public QTreeWidgetItem {
public:
    TreeItem(int type);

    virtual void remove() = 0;
};

class AnalysisItem : public QTreeWidgetItem{
    Analysis m_analysis;

public:
    AnalysisItem(int type);
    ~AnalysisItem();
    void set_analysis(Analysis analysis);
    Analysis *get_analysis();
};

class FolderItem : public QTreeWidgetItem
{
public:
    FolderItem(int type);
};

class VideoItem : public TreeItem {
    VideoProject* m_vid_proj;

public:
    VideoItem(VideoProject* video_project, int type);
    VideoItem(int type);
    VideoProject* get_video_project();
    void set_video_project(VideoProject* vid_proj);
    ~VideoItem();

    void remove();
signals:

public slots:
};
#endif // ITEMTYPES_H
