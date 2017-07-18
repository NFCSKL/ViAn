#include "projectwidget.h"
#include "projectdialog.h"
#include "TreeItems/itemtypes.h"
#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <QFileInfo>
#include <QDirIterator>
#include <QShortcut>
#include <iostream>
#include <algorithm>
#include <sstream>

#include "Project/projecttreestate.h"

ProjectWidget::ProjectWidget(QWidget *parent) : QTreeWidget(parent) {
    header()->close();
    setContextMenuPolicy(Qt::CustomContextMenu);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);

    connect(this, &ProjectWidget::customContextMenuRequested, this, &ProjectWidget::context_menu);
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this , SLOT(tree_item_clicked(QTreeWidgetItem*,int)));

    QShortcut* new_folder_sc = new QShortcut(this);
    new_folder_sc->setContext(Qt::WidgetWithChildrenShortcut);
    new_folder_sc->setKey(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_N));
    connect(new_folder_sc, &QShortcut::activated, this, &ProjectWidget::create_folder_item);
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
    _tmp_path.append(_tmp_name);
    emit proj_path(m_proj->getDir());
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
    VideoItem* v_item = get_video_item(vid_proj);
    if (vid_proj == nullptr) return;
    v_item->addChild(ana);
    ana->setText(0, "Loading");
    v_item->setExpanded(true);
    QTreeWidgetItem* item = dynamic_cast<QTreeWidgetItem*>(ana);
    emit begin_analysis(m_proj->getDir(), vid_proj->get_video()->file_path, item);
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
    VideoItem* vid_item = new VideoItem(vid_proj, VIDEO_ITEM);
    insertTopLevelItem(topLevelItemCount(), vid_item);
    vid_proj->set_tree_index(get_index_path(dynamic_cast<QTreeWidgetItem*>(vid_item)));
    emit set_status_bar("Video added: " + vid_name);
}

/**
 * @brief ProjectWidget::mimeTypes
 * @return Accepted mime types
 */
QStringList ProjectWidget::mimeTypes() const {
    return QStringList() << "application/x-qabstractitemmodeldatalist" << "text/uri-list";
}

/**
 * @brief ProjectWidget::file_dropped
 * Loads the video into the program if it is of accepted type
 * @param path  : path to the video file
 */
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
        VideoItem* v_item = new VideoItem(vid_proj, VIDEO_ITEM);
        insertTopLevelItem(topLevelItemCount(), v_item);
        vid_proj->set_tree_index(get_index_path(v_item));
    }

}

/**
 * @brief ProjectWidget::folder_dropped
 * Searches the path for folders and files
 * @param path  : path to the folder
 */
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

/**
 * @brief ProjectWidget::get_index_path
 * Returns the index path for the item.
 * For a item two folders down the path could be 011 or 051.
 * @param item
 * @return index path
 */
std::stack<int> ProjectWidget::get_index_path(QTreeWidgetItem *item){
    std::stack<int> index_path;
    QTreeWidgetItem* p = &*item;
    while (p->parent() != nullptr) {
        index_path.push(p->parent()->indexOfChild(p));
        p = p->parent();
    }
    index_path.push(indexOfTopLevelItem(p));
    return index_path;
}

/**
 * @brief ProjectWidget::get_video_item
 * Searches the project tree for the VideoItem containing the VideoProjectt v_proj
 * @param v_proj    :   target VideoProject
 * @return VideoItem*   :   the correct VideoItem if found else nullptr
 */
VideoItem *ProjectWidget::get_video_item(VideoProject *v_proj) {
    for (auto i = 0; i < topLevelItemCount(); ++i) {
        auto item = topLevelItem(i);
        if (item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);
            if (v_item->get_video_project() == v_proj) return v_item;
        } else if (item->type() == FOLDER_ITEM) {
            VideoItem* v_item = search_folder(dynamic_cast<FolderItem*>(item), v_proj);
            if (v_item != nullptr) return v_item;
        }
    }
    return nullptr;
}

/**
 * @brief ProjectWidget::search_folder
 * Searches a FolderItem f_item for the VideoProject v_proj
 * @param f_item    :   FolderItem where the search starts
 * @param v_proj    :   Target VideoProject
 * @return VideoItem*   :   VideoItem containing v_proj or nullptr
 */
VideoItem *ProjectWidget::search_folder(FolderItem* f_item, VideoProject *v_proj) {
    for (auto i = 0; i < f_item->childCount(); ++i) {
        QTreeWidgetItem* item = f_item->child(i);
        if (item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);
            if (v_item->get_video_project() == v_proj) return v_item;
        } else if (item->type() == FOLDER_ITEM) {
            VideoItem* v_item = search_folder(dynamic_cast<FolderItem*>(item), v_proj);
            if (v_item != nullptr) return v_item;
        }
    }
    return nullptr;
}

void ProjectWidget::insert_to_path_index(VideoProject *vid_proj) {
    // Index path is on form x:x:x:x..
    // Split at delimiter ':' to get index
    std::stringstream ss(vid_proj->get_index_path());
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, ':')) {
       elems.push_back(std::move(item)); // if C++11 (based on comment from @mchiasson)
    }

    // Get item at index path and set video project
    if (elems.size() > 0) {
        QTreeWidgetItem* item = topLevelItem(std::stoi(elems[0]));
        for (auto i = 1; i < elems.size(); ++i) {
            if (item->child(std::stoi(elems[i]))) item = item->child(std::stoi(elems[i]));
        }
        if (item != nullptr && item->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(item);
            v_item->set_video_project(vid_proj);
        }
    }
}

/**
 * @brief ProjectWidget::update_index_paths
 * Updates the position index for each VideoProject
 * @param item
 */
void ProjectWidget::update_index_paths(QTreeWidgetItem* item) {
    if (item == nullptr) item = invisibleRootItem();
    for (auto i = 0; i < item->childCount(); ++i){
        auto child = item->child(i);
        if (child->type() == VIDEO_ITEM) {
            VideoItem* v_item = dynamic_cast<VideoItem*>(child);
            v_item->get_video_project()->set_tree_index(get_index_path(item->child(i)));
        } else if (child->type() == FOLDER_ITEM) {
            update_index_paths(child);
        }
    }
}

/**
 * @brief ProjectWidget::dragEnterEvent
 * Makes sure the ProjectWidget only accepts drops containing urls or objects of the correct mime type
 * @param event
 */
void ProjectWidget::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls() && m_proj != nullptr) {
        // Files
        event->acceptProposedAction();
    } else if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")){
        // TreeItem
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

/**
 * @brief ProjectWidget::dropEvent
 * Handels drop events. Will try to add files if they are dropped.
 * Calls upon standard dropEvent for TreeItems.
 * @param event
 */
void ProjectWidget::dropEvent(QDropEvent *event) {
    if (event->mimeData()->hasUrls()) {
        // Files
        for (auto &url : event->mimeData()->urls()) {
            QString video_path = url.toLocalFile();
            QFileInfo f_info(video_path);
            if (f_info.isDir()) {
                folder_dropped(video_path);
            } else {
                file_dropped(video_path);
            }
        }
    } else {
        QList<QTreeWidgetItem*> items = selectedItems();
        QTreeWidget::dropEvent(event);
        // Update index paths
        for (auto item : items) {
            if (item->type() == VIDEO_ITEM) {
                auto vid_item = dynamic_cast<VideoItem*>(item);
                vid_item->get_video_project()->set_tree_index(get_index_path(item));
            }
        }
        update_index_paths();
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
    get_index_path(item);
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
    qDebug() << selectedItems().count();
    if (m_proj == nullptr) return;
    clicked_item = itemAt(point);
    QMenu menu(this);
    const int item_count = selectedItems().count();
    if (item_count == 0) {
        // Clicked on root tree
        menu.addAction("New Folder", this, SLOT(create_folder_item()));
    } else if (item_count == 1) {
        // Clicked on item
        menu.addAction("New Folder", this, SLOT(create_folder_item()));
        menu.addSeparator();
        switch (selectedItems().front()->type()) {
        case ANALYSIS_ITEM:
            menu.addAction("Rename", this, SLOT(rename_item()));
            break;
        case FOLDER_ITEM:
            menu.addAction("Rename", this, SLOT(rename_item()));
            menu.addAction("Remove", this, SLOT(remove_item()));
            break;
        case VIDEO_ITEM:
            menu.addAction("Remove", this, SLOT(remove_item()));
        default:
            // VIDEO_ITEM
            break;
        }
    } else if (item_count > 1) {
        // Clicked with selected items
        menu.addAction("Remove", this, SLOT(remove_item()));
    }
    menu.exec(mapToGlobal(point));
    clicked_item = nullptr;

}

void ProjectWidget::remove_item() {
    QMessageBox delete_box(this);
    delete_box.setIcon(QMessageBox::Warning);
    delete_box.setText("Deleting item(s)\n"
                       "(Unselected items within selected folders will be deleted as well)");
    delete_box.setInformativeText("Do you wish to continue?");
    delete_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    delete_box.setDefaultButton(QMessageBox::No);
    if (delete_box.exec() == QMessageBox::Yes) {
        for (auto item : selectedItems()) {
            delete item;
        }
    }

}

void ProjectWidget::rename_item(){
    qDebug() << "Renaming item: " << clicked_item->text(0);
    editItem(clicked_item);
}

void ProjectWidget::create_folder_item() {
    if (m_proj == nullptr) return;
    FolderItem* item = new FolderItem(FOLDER_ITEM);
    item->setText(0, tr("New Folder"));
    QTreeWidgetItem* s_item = (!selectedItems().count()) ? invisibleRootItem() : selectedItems().front();
    if (s_item == invisibleRootItem()) {
        // Click occured on background add to top level
        insertTopLevelItem(topLevelItemCount(), item);
    } else if (s_item->type() == FOLDER_ITEM) {
        // Clicked on folder item. Add new folder as child
        s_item->insertChild(0, item);
        s_item->setExpanded(true);
    } else if (s_item->type() == VIDEO_ITEM) {
        QTreeWidgetItem* p_item =  s_item->parent();
        if (p_item == nullptr) {
            insertTopLevelItem(indexOfTopLevelItem(s_item) + 1, item);
        } else {
            int index = p_item->indexOfChild(s_item);
            p_item->insertChild(index + 1, item);
        }
    }
    editItem(item);
    clearSelection();
    item->setSelected(true);
}

/**
 * @brief ProjectWidget::save_project
 * Slot function to save the open project
 */
void ProjectWidget::save_project() {
    update_index_paths();
    m_proj->save_project();
    ProjectTreeState tree_state;
    tree_state.set_tree(invisibleRootItem());
    tree_state.save_state(m_proj->getDir() + "treestate");
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
        m_proj = Project::fromFile(project_path.toStdString());
        // Load project tree structure
        ProjectTreeState tree_state;
        tree_state.set_tree(invisibleRootItem());
        tree_state.load_state(m_proj->getDir() + "treestate.json");

        emit proj_path(m_proj->getDir());
        for (auto vid_proj : m_proj->get_videos()) {
            insert_to_path_index(vid_proj);
            emit load_bookmarks(vid_proj);
//            QString video_path = QString::fromStdString(vid_proj->get_video()->file_path);
//            int index = video_path.lastIndexOf('/') + 1;
//            QString vid_name = video_path.right(video_path.length() - index);
//            tree_add_video(vid_proj, vid_name);
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
