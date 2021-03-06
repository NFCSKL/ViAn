#ifndef RECENTPROJECTDIALOG_H
#define RECENTPROJECTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QObject>
#include <QBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLabel>

#include "Project/recentproject.h"
#include "GUI/projectdialog.h"
/**
 * @brief The RecentProjectDialog class
 * Startup dialog used to open recently used projects
 * create new or open an existing one.
 */
class RecentProjectDialog : public QDialog {
    Q_OBJECT
    QHBoxLayout* h_layout;
    QVBoxLayout* v_main_layout;
    QVBoxLayout* v_btn_layout;
    QPushButton* new_btn;
    QPushButton* open_btn;
    QListWidget* recent_list;
public:
    RecentProjectDialog(QWidget* parent = nullptr);
signals:
    void open_project(QString project_path);
    void open_project_from_file(void);
    void new_project(void);
private slots:
    void on_item_double_clicked(QListWidgetItem* item);
    void on_new_btn_clicked();
    void on_open_btn_clicked();
};

#endif // RECENTPROJECTDIALOG_H
