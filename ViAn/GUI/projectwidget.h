#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include "Project/project.h"
class ProjectWidget : public QTreeWidget
{
    Q_OBJECT
    QTreeWidgetItem* m_videos;
public:
    explicit ProjectWidget(QWidget *parent = nullptr);
    Project* m_proj = nullptr;

signals:
    void selected_media();
    void marked_video(VideoProject* vid_proj);
    void set_status_bar(QString);
    void send_path(std::string);
    void begin_analysis(std::string, std::string, QTreeWidgetItem*);
public slots:
    void new_project(void) const;
    void add_project(const QString project_name, const QString project_path);
    void add_video();
    void add_analysis(VideoProject* vid_proj, QString name);
    void start_analysis(VideoProject*);
    void set_analysis_name(QTreeWidgetItem *item, QString);
    void save_project();
    void open_project();
    void close_project();
    void remove_project();
private slots:
    void tree_item_clicked(QTreeWidgetItem *item, const int& col);
private:
    void create_default_tree();
    void tree_add_video();
    void tree_add_video(VideoProject* vid_proj, const QString& video_name);
    void tree_add_analysis(VideoProject* vid_proj, QString name);

};

#endif // PROJECTWIDGET_H
