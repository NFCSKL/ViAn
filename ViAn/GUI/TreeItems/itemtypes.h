#ifndef ITEMTYPES_H
#define ITEMTYPES_H

#include "Project/videoproject.h"
#include <QTreeWidgetItem>

enum ITEM_TYPE {VIDEO_ITEM, ANALYSIS_ITEM, FOLDER_ITEM, TAG_ITEM};


class VideoProject;
class TreeItem : public QTreeWidgetItem {
public:
    TreeItem(int type);

    virtual void remove() = 0;
    virtual void rename() = 0;
};


class AnalysisItem : public TreeItem {
    Analysis m_analysis;
public:
    AnalysisItem(Analysis analysis);
    AnalysisItem();
    ~AnalysisItem();
    void set_analysis(Analysis analysis);
    Analysis *get_analysis();
    void remove();
    void rename();
};


class FolderItem : public TreeItem {
public:
    FolderItem();
    void remove();
    void rename();
};


class TagItem : public TreeItem {
    Analysis* m_tag;
public:
    TagItem(Analysis *tag);
    Analysis *get_tag();
    void remove();
    void rename();
};


class VideoItem : public TreeItem {
    VideoProject* m_vid_proj;
public:
    VideoItem(VideoProject* video_project);
    VideoItem();
    VideoProject* get_video_project();
    void set_video_project(VideoProject* vid_proj);
    ~VideoItem();

    void remove();
    void rename();
};

#endif // ITEMTYPES_H
