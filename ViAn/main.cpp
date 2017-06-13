#include "Video/video_player.h"
#include "Test/test_video_player.h"
#include "Test/filehandlertest.h"
#include "Test/test_mainwindow.h"
#include "Test/test_report_generator.h"
#include "Test/overlayintegrationtest.h"
#include "GUI/mainwindow.h"
#include <QApplication>
/**
 * @brief qMain
 * Constructor
 * @param argc an int variable
 * @param argv a list of chars
 * @return exits the program
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qRegisterMetaType<Analysis>();
    w.show();

    bool testing = false;
    bool integration_testing = false;
    if (testing) {
        //For testing video player.
        test_video_player* video_test = new test_video_player();
        QTest::qExec(video_test);
        //For testing filehandler.
        FileHandlerTest* file_test = new FileHandlerTest();
        QTest::qExec(file_test);
        //For testing the mainwindow
        test_mainwindow* window_test = new test_mainwindow(&w);
        QTest::qExec(window_test);

        //For testing the report_generator(THIS TEST WILL CREATE FILES)
        test_report_generator* report_test = new test_report_generator();
        QTest::qExec(report_test);
    }
    if (integration_testing) {
        OverlayIntegrationTest* ot = new OverlayIntegrationTest();
        ot->exec();
    }
    return a.exec();
}
