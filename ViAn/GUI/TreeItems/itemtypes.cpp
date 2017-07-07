#include "itemtypes.h"


VideoItem::VideoItem(VideoProject* video_project, int type): QTreeWidgetItem(type) {
    setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
    m_vid_proj = video_project;
    setText(0, QString::fromStdString(video_project->get_video()->get_name()));
}

VideoItem::~VideoItem(){
    delete m_vid_proj;
}

VideoProject* VideoItem::get_video_project() {
    return m_vid_proj;
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
