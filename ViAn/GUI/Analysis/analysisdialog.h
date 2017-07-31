#ifndef ANALYSISDIALOG_H
#define ANALYSISDIALOG_H
#include <QDialog>
#include <QLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QLineEdit>
class AnalysisDialog : public QDialog
{
    Q_OBJECT
    QLineEdit* dilation;
    QLineEdit* small_obj;
    QLineEdit* det_tresh;

    QLineEdit* graywhite;
    QLineEdit* bg_hist;
    QLineEdit* mog2_thresh;
    QLineEdit* det_shadows;
public:

    AnalysisDialog();
};

#endif // ANALYSISDIALOG_H
