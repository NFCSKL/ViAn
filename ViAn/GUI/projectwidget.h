#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "Project/project.h"
#include "TreeItems/itemtypes.h"
#include <stack>

class Project;
class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    QTreeWidgetItem* clicked_item = nullptr;
    QPoint* clicked_point = nullptr;
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    Project* m_proj = nullptr;

signals:
    void selected_media();
    void marked_video(VideoProject* vid_proj, int frame = 0);
    void proj_path(std::string);
    void load_bookmarks(VideoProject* vid_proj);
    void marked_analysis(Analysis*);
    void set_detections(bool);
    void enable_poi_btns(bool);
    void set_poi_slider(bool);
    void set_status_bar(QString);
    void begin_analysis(std::string, std::string, QTreeWidgetItem*);

public slots:
    void new_project(void);
    void add_project(const QString project_name, const QString project_path);
    void add_video();
    void start_analysis(VideoProject*);
    void set_tree_item_name(QTreeWidgetItem *item, QString);
    void save_project();
    void open_project();
    void close_project();
    void remove_project();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
private slots:
    void tree_item_clicked(QTreeWidgetItem *item, const int& col);
    void context_menu(const QPoint& point);
    void remove_item();
    void rename_item();
    void create_folder_item();
private:
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, const QString& video_name);
    QStringList mimeTypes() const;
    void file_dropped(QString path);
    void folder_dropped(QString path);
    void insert_dropped(VideoItem* item);
    std::stack<int> get_index_path(QTreeWidgetItem* item);
    VideoItem* get_video_item(VideoProject* v_proj);
    VideoItem* search_folder(FolderItem* f_item, VideoProject* v_proj);
    void insert_to_path_index(VideoProject* vid_proj);
    void update_index_paths(QTreeWidgetItem* item = nullptr);
signals:
    void project_closed();


};

#endif // PROJECTWIDGET_H
