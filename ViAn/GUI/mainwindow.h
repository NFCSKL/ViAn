#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHBoxLayout>
#include <QDockWidget>
#include <QMainWindow>
#include <QBasicTimer>
#include <QCloseEvent>

#include <QSlider>
#include <QFileDialog>

#include <string>

#include "Library/customdialog.h"
#include "reportgenerator.h"
#include "action.h"
#include "qtreeitems.h"

#include "Analysis/AnalysisController.h"
#include "Project/Analysis/analysismeta.h"

#include "videowidget.h"
#include "projectwidget.h"
#include "bookmarkwidget.h"

using namespace std;
class AnalysisWindow;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:    

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void open_project(void);
    void save_project(void);
    void gen_report(void);
    void close_project(void);
    void remove_project(void);
    void options(void);
private:

    VideoWidget* video_wgt;
    ProjectWidget* project_wgt;
    BookmarkWidget* bookmark_wgt;   

    QAction* toggle_project_wgt;
    QAction* toggle_bookmark_wgt;

    AnalysisWindow *analysis_window;

    void init_file_menu();
    void init_edit_menu();
    void init_view_menu();
    void init_tools_menu();
    void init_help_menu();
};

#endif // MAINWINDOW_H
