#include "statusbar.h"
#include <QDebug>
#include <QProgressBar>

StatusBar::StatusBar() {

}

/**
 * @brief StatusBar::on_set_status_bar
 * @param status String to be shown on the status bar
 * @param timer Duration of the status bar message
 * Set status to the status bar for timer duration.
 */
void StatusBar::on_set_status_bar(QString status, int timer) {
    showMessage(status, timer);
}

void StatusBar::add_analysis_bar() {
    ana_prog = new QProgressBar();
    ana_prog->setOrientation(Qt::Horizontal);
    ana_prog->setMaximumHeight(PROG_BAR_HEIGHT);
    ana_prog->setMaximumWidth(PROG_BAR_WIDTH);
    //ana_prog->setMaximum(100);
    addPermanentWidget(ana_prog);
}

void StatusBar::update_analysis_bar(int progress) {
    if(ana_prog == nullptr) return;
    ana_prog->setValue(progress);
}

void StatusBar::remove_analysis_bar() {
    if(ana_prog == nullptr) return;

    qDebug() << "remove widget";
    removeWidget(ana_prog);
    qDebug() << "delete widget";
    delete ana_prog;
    ana_prog = nullptr;
}
