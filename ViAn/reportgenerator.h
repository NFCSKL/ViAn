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
enum TABLE_STYLE {NO_BORDER = 0, BORDER=36};
using RefDisp = std::vector<std::pair<std::vector<BookmarkItem*>,std::vector<BookmarkItem*>>>;
class ReportGenerator : public QObject {
    Q_OBJECT
    std::string m_path;
    QAxObject* word;
    std::vector<Bookmark*> all_bookmarks;
    const double IMAGE_WIDTH_REFERENCE = 136.0;
    RefDisp m_ref_disp;
public:
    friend class test_report_generator;
    explicit ReportGenerator(std::string proj_path,RefDisp RefDisp);
    ~ReportGenerator();
    void create_report();    
private:
    static void make_doc(QAxObject* obj, QString file_name);
    void add_paragraph(QAxObject* selection);

    QString get_bookmark_fig_txt(BookmarkItem *bm, int fig_num);
    QString get_bookmark_descr(BookmarkItem *bm);

    void create_bookmark_table(QAxObject *para, RefDisp bookmark_list);


    QAxObject* add_table(QAxObject* range, int rows, int cols, TABLE_STYLE style= NO_BORDER);
    QAxObject* get_cell(QAxObject* table, int row, int cols);

    void resize_picture(QString pic_path, QAxObject* inline_shape);

    void cell_insert_category(QAxObject* cell, std::vector<BookmarkItem *> bm_list);
    void cell_add_bookmark();
    void cell_add_text(QAxObject* range, QString entry, int row, int col);
    void cell_add_img(QAxObject* table, QString file_name, int row, int col);

    QString save_report(QAxObject* active_document);
    void close_report(QAxObject* doc, QAxObject*  word);

    std::string date_time_generator();
    QString calculate_time(int ms);
signals:
    void done(void);
};
#endif // REPORTGENERERATOR_H
