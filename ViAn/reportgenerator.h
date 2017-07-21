#ifndef REPORTGENERERATOR_H
#define REPORTGENERERATOR_H

#include <iostream>
#include <ActiveQt/QAxObject>
#include <ActiveQt/QAxWidget>
#include <QString>
#include "Project/project.h"
#include "dirent.h"
#include "GUI/bookmarkitem.h"
#include <ctime>
class ReportGenerator : public QObject {
    Q_OBJECT
public:
    using ref_disp = std::pair<std::vector<BookmarkItem*>,std::vector<BookmarkItem*>>;
    friend class test_report_generator;
    explicit ReportGenerator(ref_disp ref_disp);
    ~ReportGenerator();
    void create_report();
    ref_disp m_ref_disp;
private:
    QAxObject* word;   
    std::vector<Bookmark*> all_bookmarks;
    const double IMAGE_WIDTH_REFERENCE = 272.0;

    void create_list_of_names();
    void add_bookmarks(QAxObject* selection, ref_disp ref_disp);
    QString save_report(QAxObject* active_document);
    void close_report(QAxObject* doc, QAxObject*  word);
    void resize_picture(QString pic_path, QAxObject* inline_shape);
    std::string date_time_generator();
    void add_paragraph(QAxObject* selection);
    QString calculate_time(int ms);
signals:
    void done(void);
};
#endif // REPORTGENERERATOR_H

