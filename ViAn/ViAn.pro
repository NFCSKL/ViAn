#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T11:51:07
#
#-------------------------------------------------

QT       += core gui
QT       += testlib
QT       += axcontainer

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = ViAn
TEMPLATE = app
#
# GENERAL
#
SOURCES += main.cpp \
    reportgenerator.cpp \
    GUI/Toolbars/maintoolbar.cpp \
    GUI/Toolbars/drawingtoolbar.cpp \
    utility.cpp \
    GUI/drawscrollarea.cpp \
    Video/zoomer.cpp \
    GUI/Analysis/analysisslider.cpp \
    GUI/Project/projectdialog.cpp \
    Project/Test/videoprojecttest.cpp \
    GUI/Bookmark/bookmarkcategory.cpp \
    imagegenerator.cpp \
    GUI/Bookmark/bookmarklist.cpp \
    GUI/statusbar.cpp \
    GUI/Analysis/analysiswidget.cpp \
    GUI/manipulatordialog.cpp \
    Video/frameprocessor.cpp \
    Project/projecttreestate.cpp \
    GUI/Analysis/tagdialog.cpp \
    imageexporter.cpp \
    GUI/Video/frameexporterdialog.cpp \
    Project/Analysis/tag.cpp \
    Video/videocontroller.cpp \
    Video/videoplayer.cpp \
    GUI/Project/recentprojectdialog.cpp \
    Project/recentproject.cpp \
    Project/Analysis/basicanalysis.cpp \
    Project/Analysis/analysisinterval.cpp \
    Filehandler/writeable.cpp \
    Project/Analysis/analysisproxy.cpp \
    Project/Analysis/detectionbox.cpp \
    AxReport/axtable.cpp \
    AxReport/axrange.cpp \
    AxReport/axcell.cpp \
    GUI/TreeItems/tagitem.cpp \
    GUI/TreeItems/videoitem.cpp \
    GUI/TreeItems/analysisitem.cpp \
    GUI/TreeItems/treeitem.cpp \
    GUI/TreeItems/folderitem.cpp \
    GUI/TreeItems/itemstates.cpp

#
# TEST
#
SOURCES +=\
    Test/test_report_generator.cpp

HEADERS += reportgenerator.h\
    Test/test_report_generator.h \
    GUI/Toolbars/maintoolbar.h \
    GUI/Toolbars/drawingtoolbar.h \
    utility.h \
    GUI/drawscrollarea.h \
    Video/zoomer.h \
    GUI/Analysis/analysisslider.h \
    GUI/Project/projectdialog.h \
    Project/Test/videoprojecttest.h \
    GUI/Bookmark/bookmarkcategory.h \
    imagegenerator.h \
    GUI/Bookmark/bookmarklist.h \
    GUI/statusbar.h \
    GUI/Analysis/analysiswidget.h \
    GUI/manipulatordialog.h \
    Video/frameprocessor.h \
    Project/projecttreestate.h \
    GUI/Analysis/tagdialog.h \
    imageexporter.h \
    GUI/Video/frameexporterdialog.h \
    Project/Analysis/tag.h \
    Video/videocontroller.h \
    Video/videoplayer.h \
    GUI/Project/recentprojectdialog.h \
    Project/recentproject.h \
    Project/Analysis/basicanalysis.h \
    Project/Analysis/analysisinterval.h \
    Filehandler/writeable.h \
    Project/Analysis/analysisproxy.h \
    Project/Analysis/detectionbox.h \
    AxReport/axtable.h \
    AxReport/axrange.h \
    AxReport/axcell.h \
    GUI/TreeItems/tagitem.h \
    GUI/TreeItems/videoitem.h \
    GUI/TreeItems/analysisitem.h \
    GUI/TreeItems/treeitem.h \
    GUI/TreeItems/folderitem.h \
    GUI/TreeItems/itemstates.h

#
# LIBRARY
#
SOURCES += Library/customdialog.cpp
HEADERS += Library/customdialog.h

#
# GUI
#
SOURCES += GUI/mainwindow.cpp \
    GUI/Bookmark/bookmarkitem.cpp \
    GUI/Video/fpsdialog.cpp \
    GUI/Video/videowidget.cpp \
    GUI/Video/framewidget.cpp \
    GUI/Project/projectwidget.cpp \
    GUI/Bookmark/bookmarkwidget.cpp


HEADERS  += GUI/mainwindow.h \
    GUI/action.h \
    GUI/Bookmark/bookmarkitem.h \
    GUI/Video/fpsdialog.h \
    GUI/Video/videowidget.h \
    GUI/Video/framewidget.h \
    GUI/Project/projectwidget.h \
    GUI/Bookmark/bookmarkwidget.h

FORMS    +=

RESOURCES += resources.qrc
#
# START :
# VIDEOPLAYER
# OPENCV
#
SOURCES += \
    Video/overlay.cpp \
    Video/analysisoverlay.cpp \
    Video/shapes/arrow.cpp \
    Video/shapes/circle.cpp \
    Video/shapes/line.cpp \
    Video/shapes/pen.cpp \
    Video/shapes/rectangle.cpp \
    Video/shapes/shape.cpp \
    Video/shapes/text.cpp \
    Video/shapes/zoomrectangle.cpp \
    Video/shapes/analysarea.cpp \
    Video/framemanipulator.cpp
HEADERS += \
    Video/overlay.h \
    Video/analysisoverlay.h \
    Video/shapes/arrow.h \
    Video/shapes/circle.h \
    Video/shapes/line.h \
    Video/shapes/pen.h \
    Video/shapes/rectangle.h \
    Video/shapes/shape.h \
    Video/shapes/text.h \
    Video/shapes/zoomrectangle.h \
    Video/shapes/analysarea.h \
    Video/framemanipulator.h

win32 {
    INCLUDEPATH += C:\opencv\release\install\include
    LIBS += C:\opencv\release\bin\libopencv_core320.dll
    LIBS += C:\opencv\release\bin\libopencv_imgproc320.dll
    LIBS += C:\opencv\release\bin\libopencv_imgcodecs320.dll
    LIBS += C:\opencv\release\bin\libopencv_video320.dll
    LIBS += C:\opencv\release\bin\libopencv_videoio320.dll
    LIBS += C:\opencv\release\bin\libopencv_highgui320.dll
}

unix {
    INCLUDEPATH += -L/usr/include/opencv2
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_imgcodecs
    LIBS += -lopencv_video
    LIBS += -lopencv_videoio
    LIBS += -lopencv_highgui
}

#
# START: FILEHANDLER
#
SOURCES += \
    Filehandler/saveable.cpp \



HEADERS  += \
    Filehandler/saveable.h \



#
# END: FILEHANDLER
#

#
# Project
#
SOURCES += Project/Test/projecttestsuite.cpp \
    Project/report.cpp \
    Project/project.cpp \
    Project/video.cpp \
    Project/videoproject.cpp \
    Project/bookmark.cpp \
    Project/Analysis/analysis.cpp \
    Project/Analysis/poi.cpp


HEADERS +=Project/Test/projecttestsuite.h \
    Project/project.h \
    Project/video.h \
    Project/videoproject.h \
    Project/bookmark.h  \
    Project/report.h \
    Project/Analysis/analysis.h \
    Project/Analysis/poi.h




#
# ANALYSIS
#
SOURCES += Analysis/AnalysisMethod.cpp\
    Analysis/MotionDetection.cpp \
    Analysis/AnalysisController.cpp

HEADERS +=Analysis/AnalysisMethod.h\
    Analysis/MotionDetection.h \
    Analysis/AnalysisController.h
