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
    anal_prog = new QProgressBar();
    anal_prog->setOrientation(Qt::Horizontal);
    anal_prog->setMaximumHeight(PROG_BAR_HEIGHT);
    anal_prog->setMaximumWidth(PROG_BAR_WIDTH);
    //anal_prog->setMaximum(100);

    addPermanentWidget(anal_prog);
}

void StatusBar::update_analysis_bar(int progress) {
    anal_prog->setValue(progress);
}

void StatusBar::remove_analysis_bar() {
    removeWidget(anal_prog);
    delete anal_prog;
}
