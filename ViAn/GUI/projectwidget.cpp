#include "projectwidget.h"
#include "projectdialog.h"
#include "TreeItems/itemtypes.h"
#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <QFileInfo>
#include <QDirIterator>
#include <iostream>
#include <algorithm>

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {
    header()->close();
    setContextMenuPolicy(Qt::CustomContextMenu);
//    setSelectionMode(QAbstractItemView::SingleSelection);
//    setDragDropMode(QAbstractItemView::DragDrop);
    setAcceptDrops(true);
//    setDropIndicatorShown(true);
    connect(this, &ProjectWidget::customContextMenuRequested, this, &ProjectWidget::context_menu);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this , SLOT(tree_item_clicked(QTreeWidgetItem*,int)));
}

/**
 * @brief ProjectWidget::new_project
 * Creates a create project dialog
 */
void ProjectWidget::new_project() {
    if (m_proj == nullptr) {
        ProjectDialog* proj_dialog = new ProjectDialog();
        QObject::connect(proj_dialog, SIGNAL(project_path(QString, QString)), this, SLOT(add_project(QString, QString)));
    } else {
        close_project();
        new_project();
    }
}

/**
 * @brief ProjectWidget::add_project
 * Slot function called by the create project dialog.
 * Creates a new project and generates the default tree structure
 * @param project_name
 * @param project_path
 */
void ProjectWidget::add_project(QString project_name, QString project_path) {
    std::string _tmp_name = project_name.toStdString();
    std::string _tmp_path = project_path.toStdString();    
    m_proj = new Project(_tmp_name, _tmp_path);
    create_default_tree();
    _tmp_path.append(_tmp_name);
    emit proj_path(m_proj->getDir());
}

/**
 * @brief ProjectWidget::create_default_tree
 * Creates and adds the default tree structure
 */
void ProjectWidget::create_default_tree() {
    m_videos = new FolderItem(FOLDER_ITEM);
    m_videos->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    m_videos->setText(0, tr("Videos"));
    addTopLevelItem(m_videos);
}

/**
 * @brief ProjectWidget::add_video
 * Slot function to add a video.
 * Creates a file dialog and creates a video project based on file path
 */
void ProjectWidget::add_video() {
    if (m_proj == nullptr)  return;
    // TODO: HANDLE CASE. Only open video files
    QStringList video_paths = QFileDialog().getOpenFileNames(this, tr("Add video"), m_proj->getDir_videos().c_str());
    for (auto video_path : video_paths){
        int index = video_path.lastIndexOf('/') + 1;
        QString vid_name = video_path.right(video_path.length() - index);

        // TODO Check if file is already added
        VideoProject* vid_proj = new VideoProject(new Video(video_path.toStdString()));
        m_proj->add_video_project(vid_proj);
        tree_add_video(vid_proj, vid_name);
    }
}

/**
 * @brief ProjectWidget::start_analysis
 * @param vid_proj
 * Start analysis on the selected video
 */
void ProjectWidget::start_analysis(VideoProject* vid_proj) {
    AnalysisItem* ana = new AnalysisItem(ANALYSIS_ITEM);
    for (int i = 0; i < m_videos->childCount(); i++) {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(m_videos->child(i));
        if (vid_item->get_video_project() == vid_proj) {
            m_videos->child(i)->addChild(ana);
            ana->setText(0, "Loading");
            m_videos->child(i)->setExpanded(true);
            QTreeWidgetItem* item = dynamic_cast<QTreeWidgetItem*>(ana);
            emit begin_analysis(m_proj->getDir(), vid_proj->get_video()->file_path, item);
        }
    }
}

/**
 * @brief ProjectWidget::set_tree_item_name
 * @param item
 * @param name
 * Slot to set the name if an item in the project tree
 */

void ProjectWidget::set_tree_item_name(QTreeWidgetItem* item, QString name) {
    item->setText(0, name);
}

/**
 * @brief ProjectWidget::tree_add_video
 * Adds the video to the project tree
 * @param vid_proj
 * @param vid_name
 */
void ProjectWidget::tree_add_video(VideoProject* vid_proj, const QString& vid_name) {
    VideoItem* vid = new VideoItem(vid_proj, VIDEO_ITEM);
    vid->setText(0, vid_name);
    m_videos->addChild(vid);
    emit set_status_bar("Video added: " + vid_name);
    m_videos->setExpanded(true);
}

QStringList ProjectWidget::mimeTypes() const {
    return QStringList() << "application/x-qabstractitemmodeldatalist" << "text/uri-list";
}

void ProjectWidget::file_dropped(QString path) {
    std::set<std::string> exts {"mkv", "flv", "vob", "ogv", "ogg",
                                "264", "263", "mjpeg", "avc", "m2ts",
                                "mts", "avi", "mov", "qt", "wmv", "mp4",
                                "m4p", "m4v", "mpg", "mp2", "mpeg",
                                "mpe", "mpv", "m2v", "m4v", "3gp", "3g2",
                                "flv", "f4v", "f4p", "f4a", "f4b"};
    QFileInfo tmp(path);
    std::string ext = tmp.suffix().toStdString();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    if (exts.find(ext) != exts.end()) {
        // Add file
        int index = path.lastIndexOf('/') + 1;
        QString vid_name = path.right(path.length() - index);

        // TODO Check if file is already added
        VideoProject* vid_proj = new VideoProject(new Video(path.toStdString()));
        m_proj->add_video_project(vid_proj);
        tree_add_video(vid_proj, vid_name);
    }

}

void ProjectWidget::folder_dropped(QString path) {
    QDirIterator d_iter(path);
    while (d_iter.hasNext()) {
        QFileInfo tmp(d_iter.next());
        if (tmp.isDir() && tmp.absoluteFilePath().length() > path.length())
            folder_dropped(tmp.absoluteFilePath());
        else
            file_dropped(tmp.absoluteFilePath());
    }
}


void ProjectWidget::dragEnterEvent(QDragEnterEvent *event) {
    qDebug()  << "dragEnterEvent";
    if (event->mimeData()->hasUrls() && m_proj != nullptr) event->acceptProposedAction();
}

void ProjectWidget::dropEvent(QDropEvent *event) {
    qDebug()  << "dropEvent";
    for (auto &url : event->mimeData()->urls()) {
        QString video_path = url.toLocalFile();
        QFileInfo f_info(video_path);
        qDebug() << video_path;
        if (f_info.isDir()) {
            folder_dropped(video_path);
        } else {
            file_dropped(video_path);
        }
    }
}

/**
 * @brief ProjectWidget::tree_item_clicked
 * Slot function for when a tree item is clicked.
 * Preforms different operations based on tree item type.
 * @param item
 * @param col
 */
void ProjectWidget::tree_item_clicked(QTreeWidgetItem* item, const int& col) {
    switch(item->type()){
    case VIDEO_ITEM: {
        VideoItem* vid_item = dynamic_cast<VideoItem*>(item);
        emit marked_video(vid_item->get_video_project());
        emit set_detections(false);
        emit set_poi_slider(false);
        emit enable_poi_btns(false);
        break;
    } case ANALYSIS_ITEM: {
        tree_item_clicked(item->parent(), 0);
        AnalysisItem* ana_item = dynamic_cast<AnalysisItem*>(item);
        emit marked_analysis(ana_item->get_analysis());
        emit set_detections(true);
        emit set_poi_slider(true);
        if (!ana_item->get_analysis()->POIs.empty()) {
            emit enable_poi_btns(true);
        }

        break;
    } case FOLDER_ITEM: {
        break;
    } default:
        break;
    }

}

/**
 * @brief ProjectWidget::context_menu
 * Slot function triggered by customContextMenuRequested
 * Creates a context menu
 * @param point :   click position
 */
void ProjectWidget::context_menu(const QPoint &point) {
    if (m_proj == nullptr) return;
    clicked_item = itemAt(point);
    QMenu menu(this);
    menu.addAction("New Folder", this, SLOT(create_folder_item()));
    if (clicked_item == nullptr) {
        // Not clicking any item

    } else {
        menu.addAction("Rename", this, SLOT(rename_item()));
        menu.addSeparator();
        menu.addAction("Remove", this, SLOT(remove_item()));
        switch (clicked_item->type()) {
        case VIDEO_ITEM:
            //rename
            // remove
            break;
        case ANALYSIS_ITEM:
            //rename
            //remove
            break;
        case FOLDER_ITEM:
            //rename
            //remove - should remove all children
            break;
        default:
            break;
        }
    }
    menu.exec(mapToGlobal(point));
    clicked_item = nullptr;
//    delete clicked_point;
//    if (clicked_point ==  nullptr) qDebug() << "NULLPTR";
}

void ProjectWidget::remove_item() {
    qDebug() << "Removing item: " << clicked_item->text(0);
    delete clicked_item;
}

void ProjectWidget::rename_item(){
    qDebug() << "Renaming item: " << clicked_item->text(0);
    editItem(clicked_item);
}

void ProjectWidget::create_folder_item() {
    FolderItem* item = new FolderItem(FOLDER_ITEM);
    item->setText(0, tr("New Folder"));
    if (clicked_item == nullptr) {
        // Click occured on background add to top level
        insertTopLevelItem(0, item);
    } else if (clicked_item->type() == FOLDER_ITEM) {
        // Clicked on folder item. Add new folder as child
        clicked_item->insertChild(0, item);
    } else if (clicked_item->type() == VIDEO_ITEM) {
        QTreeWidgetItem* p_item =  clicked_item->parent();
        if (p_item == nullptr) {
            insertTopLevelItem(0, item);
        } else {
            int index = p_item->indexOfChild(clicked_item);
            p_item->insertChild(index + 1, item);
        }
    }
}

/**
 * @brief ProjectWidget::save_project
 * Slot function to save the open project
 */
void ProjectWidget::save_project() {
    m_proj->save_project();
    emit set_status_bar("Project saved");
}

/**
 * @brief ProjectWidget::open_project
 * Slot function to open a previously created project
 */
void ProjectWidget::open_project() {
    if (m_proj != nullptr) close_project();
    QString project_path = QFileDialog().getOpenFileName(this, tr("Open project"), QDir::homePath());
    if (!project_path.isEmpty()) {
        emit set_status_bar("Opening project");
        clear();
        create_default_tree();
        m_proj = Project::fromFile(project_path.toStdString());
        emit proj_path(m_proj->getDir());
        for (auto vid_pair : m_proj->get_videos()) {
            VideoProject* vid_proj = vid_pair.second;
            emit load_bookmarks(vid_proj);
            QString video_path = QString::fromStdString(vid_proj->get_video()->file_path);
            int index = video_path.lastIndexOf('/') + 1;
            QString vid_name = video_path.right(video_path.length() - index);
            tree_add_video(vid_proj, vid_name);
        }
    }
}

/**
 * @brief ProjectWidget::close_project
 * Closes the current project if there is one
 */
void ProjectWidget::close_project() {
    // TODO Check for unsaved changes before closing
    if (m_proj == nullptr) return;
    emit set_status_bar("Closing project");
    emit project_closed();
    this->clear();
    delete m_proj;
    m_proj = nullptr;
}

/**
 * @brief ProjectWidget::remove_project
 * Removes the current project if there is one
 */
void ProjectWidget::remove_project() {
    // TODO Does this delete all images?
    if (m_proj == nullptr) return;
    QMessageBox msg_box;
    msg_box.setText("Are you sure you want to remove the project?");
    msg_box.setInformativeText("This will delete all project files (images, reports, etc).");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int reply = msg_box.exec();

    if (reply != QMessageBox::Yes) return;
    emit set_status_bar("Removing project and associated files");
    m_proj->delete_artifacts();

    this->clear();
    delete m_proj;
    m_proj = nullptr;
    emit project_closed();
}
