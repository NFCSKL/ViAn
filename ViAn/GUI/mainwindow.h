#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include <string>
#include <QCloseEvent>
#include <QSlider>
#include <QFileDialog>
#include "Video/video_player.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/core/core.hpp"
#include "icononbuttonhandler.h"
#include "ui_mainwindow.h"
#include "Filehandler/filehandler.h"
#include "inputwindow.h"
#include "action.h"
#include "qtreeitems.h"
#include <QMutex>
#include <QWaitCondition>

using namespace std;
class inputwindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void set_status_bar(string status, int timer = 750);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void input_switch_case(ACTION action, QString qInput);
    bool eventFilter(QObject *obj, QEvent *event); //cannot follow namestandard, generated code
    
    // Lock and wait condition to sleep player when video is paused
    QMutex mutex;
    QWaitCondition paused_wait;

signals:
    void set_play_video();
    void set_pause_video();
    void set_stop_video();
    void resize_video_frame(int width, int height);
    void next_video_frame();
    void prev_video_frame();

private slots:

    void on_playPauseButton_clicked();

    void on_stopButton_clicked();

    void on_actionExit_triggered();

    void set_shortcuts();

    void closeEvent (QCloseEvent *event);

    void update_video(QImage frame);

    void set_video_slider_pos(int pos);

    void resizeEvent(QResizeEvent* event);

    void on_previousFrameButton_clicked();

    void on_nextFrameButton_clicked();

    void on_fastBackwardButton_clicked();

    void on_fastForwardButton_clicked();

    void on_bookmarkButton_clicked();

    void on_actionAddProject_triggered();

    void on_videoSlider_valueChanged(int newPos);
    
    void on_actionShow_hide_overlay_triggered();

    void on_actionColour_triggered();

    void on_actionRectangle_triggered();

    void on_actionCircle_triggered();

    void on_actionArrow_triggered();

    void on_actionLine_triggered();

    void prepare_menu(const QPoint & pos);

    void play_video();

    void on_actionSave_triggered();

    void on_actionPen_triggered();

    void on_actionText_triggered();

    void on_actionUndo_triggered();

    void on_actionClear_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionLoad_triggered();

    void on_actionAddVideo_triggered();

    void on_actionChoose_Workspace_triggered();

    void on_actionDeleteProject_triggered();

    void on_actionDeleteVideo_triggered();

    void on_ProjectTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_ProjectTree_itemClicked(QTreeWidgetItem *item, int column);

private:

    Ui::MainWindow *ui;
    inputwindow *inputWindow;
    video_player* mvideo_player;
    IconOnButtonHandler *iconOnButtonHandler;

    QSlider *video_slider;

    FileHandler *fileHandler;

    void setup_video_player(video_player *mplayer);
    void add_project_to_tree(Project* proj);

    void add_video_to_tree(string file_path, ID id);

    void remove_selected_project_from_tree();
    void remove_video_from_tree(MyQTreeWidgetItem *my_video);

    void toggle_toolbar();
    void enable_video_buttons();
    QTreeWidgetItem *get_project_from_object(QTreeWidgetItem *item);
};

#endif // MAINWINDOW_H
