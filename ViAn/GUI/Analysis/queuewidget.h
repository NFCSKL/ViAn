#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLayout>
#include <QProgressBar>
#include "videolistitem.h"
class QueueWidget : public QWidget
{
    Q_OBJECT
    QListWidget* m_queue;
    QVBoxLayout m_layout;
    QProgressBar* progressbar;
    QLabel* m_line;
public:
    QueueWidget(QWidget* parent = 0);
    void next();
    void enqueue(AnalysisMethod *method);
    void remove();
    void update_progress(int i);
};

#endif // QUEUEWIDGET_H