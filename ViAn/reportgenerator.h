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
using RefDisp = std::vector<std::pair<std::vector<BookmarkItem*>,std::vector<BookmarkItem*>>>;
class ReportGenerator : public QObject {
    Q_OBJECT
public:

    friend class test_report_generator;
    explicit ReportGenerator(std::string proj_path,RefDisp RefDisp);
    ~ReportGenerator();
    void create_report();
    RefDisp m_ref_disp;    
private:
    std::string m_path;
    QAxObject* word;   
    std::vector<Bookmark*> all_bookmarks;
    const double IMAGE_WIDTH_REFERENCE = 272.0;
    QString get_bookmark_descr(Bookmark *bm);
    void create_list_of_names();
    void make_doc(QAxObject* obj, QString file_name);
    std::vector<std::pair<std::string, std::string> > add_bookmarks(QAxObject *active_document, QAxObject* selection, RefDisp bookmark_list);
    void add_bookmark_pair(QAxObject* selection, BookmarkItem* bm1, BookmarkItem* bm2);
    void add_bookmark(QAxObject* selection, Bookmark *bm, int alignment);
    QAxObject *add_table(QAxObject *active_document, QAxObject* selection, int rows, int cols);
    void add_pic_ref(std::vector<std::pair<std::string, std::string> >  table_contents);
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

