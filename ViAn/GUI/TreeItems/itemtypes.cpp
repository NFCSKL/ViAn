#include "itemtypes.h"

TreeItem::TreeItem(int type) : QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
}

VideoItem::VideoItem(VideoProject* video_project, int type): TreeItem(type) {

    m_vid_proj = video_project;
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));
}

VideoItem::VideoItem(int type): TreeItem(type) {
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    setText(0,"placeholder");
}

VideoItem::~VideoItem(){
    m_vid_proj->remove_from_project();
    delete m_vid_proj;
}

void VideoItem::remove(){
    qDebug() << "Remove VideoItem";

}

VideoProject* VideoItem::get_video_project() {
    return m_vid_proj;
}

void VideoItem::set_video_project(VideoProject *vid_proj) {
    m_vid_proj = vid_proj;
    setText(0, QString::fromStdString(vid_proj->get_video()->get_name()));
}

AnalysisItem::AnalysisItem(int type) : QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
}

AnalysisItem::~AnalysisItem() {
}

void AnalysisItem::set_analysis(Analysis analysis) {
    m_analysis = analysis;
}

Analysis* AnalysisItem::get_analysis() {
    return &m_analysis;
}

FolderItem::FolderItem(int type) : QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    const QIcon folder_icon("../ViAn/Icons/folder.png");
    setIcon(0, folder_icon);
}


