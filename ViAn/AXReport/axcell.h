#ifndef AXCELL_H
#define AXCELL_H
#include <QAxObject>
#include <GUI/bookmarkitem.h>
#include "axrange.h"
#include "axtable.h"
class AxCell
{
    AxTable table;
    AxRange range;
    int m_row, m_col;
public:
    AxCell(int row,int col);

    void insert_category(QAxObject* cell, std::vector<BookmarkItem *> bm_list);
    void add_bookmark();
    void add_text(QAxObject* range, QString entry, int _row, int col);
    void add_img(QAxObject* table, QString file_name, int _row, int col);

};

#endif // AXCELL_H
